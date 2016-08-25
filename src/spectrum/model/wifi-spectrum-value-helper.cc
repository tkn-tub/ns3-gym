/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Nicola Baldo <nbaldo@cttc.es>
 *         Giuseppe Piro  <g.piro@poliba.it>
 */

#include <map>
#include <cmath>
#include "wifi-spectrum-value-helper.h"
#include "ns3/log.h"
#include "ns3/fatal-error.h"
#include "ns3/assert.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WifiSpectrumValueHelper");

struct WifiSpectrumModelId
{
  WifiSpectrumModelId (uint32_t f, uint32_t w);
  uint32_t m_centerFrequency;
  uint32_t m_channelWidth;
};

WifiSpectrumModelId::WifiSpectrumModelId (uint32_t f, uint32_t w)
  : m_centerFrequency (f),
    m_channelWidth (w)
{
  NS_LOG_FUNCTION (this << f << w);
}

bool
operator < (const WifiSpectrumModelId& a, const WifiSpectrumModelId& b)
{
  return ( (a.m_centerFrequency < b.m_centerFrequency) || ( (a.m_centerFrequency == b.m_centerFrequency) && (a.m_channelWidth < b.m_channelWidth)));
}

static std::map<WifiSpectrumModelId, Ptr<SpectrumModel> > g_wifiSpectrumModelMap;

Ptr<SpectrumModel>
WifiSpectrumValueHelper::GetSpectrumModel (uint32_t centerFrequency, uint32_t channelWidth)
{
  NS_LOG_FUNCTION (centerFrequency << channelWidth);
  Ptr<SpectrumModel> ret;
  WifiSpectrumModelId key (centerFrequency, channelWidth);
  std::map<WifiSpectrumModelId, Ptr<SpectrumModel> >::iterator it = g_wifiSpectrumModelMap.find (key);
  if (it != g_wifiSpectrumModelMap.end ())
    {
      ret = it->second;
    }
  else
    {
      Bands bands;
      double centerFrequencyHz = centerFrequency * 1e6;
      // Overall bandwidth will be channelWidth plus 10 MHz guards on each side
      double bandwidth = (channelWidth + 20) * 1e6;
      // Use OFDM subcarrier width of 312.5 KHz as band granularity
      double bandBandwidth = 312500;
      // For OFDM, the center subcarrier is null (at center frequency)
      uint32_t numBands = static_cast<uint32_t> (bandwidth / bandBandwidth + 0.5);
      NS_ASSERT (numBands > 0);
      if (numBands % 2 == 0)
        {
          // round up to the nearest odd number of subbands so that bands
          // are symmetric around center frequency
          NS_LOG_DEBUG ("Total bandwidth evenly divided by 312.5 KHz");
          numBands += 1;    
        }
      NS_ASSERT_MSG (numBands % 2 == 1, "Number of bands should be odd");
      // lay down numBands/2 bands symmetrically around center frequency
      // and place an additional band at center frequency
      double startingFrequencyHz = centerFrequencyHz - (numBands/2 * bandBandwidth) - bandBandwidth/2;
      for (size_t i = 0; i < numBands; i++) 
        {
          BandInfo info;
          double f = startingFrequencyHz + (i * bandBandwidth);
          info.fl = f;
          f += bandBandwidth/2;
          info.fc = f;
          f += bandBandwidth/2;
          info.fh = f;
          NS_LOG_DEBUG ("creating band " << i << " (" << info.fl << ":" << info.fc << ":" << info.fh << ")");
          bands.push_back (info);
        }
      ret = Create<SpectrumModel> (bands);
      g_wifiSpectrumModelMap.insert (std::pair<WifiSpectrumModelId, Ptr<SpectrumModel> > (key, ret));
    }
  NS_LOG_LOGIC ("returning SpectrumModel::GetUid () == " << ret->GetUid ());
  return ret;
}

Ptr<SpectrumValue>
WifiSpectrumValueHelper::CreateHtOfdmTxPowerSpectralDensity (uint32_t centerFrequency, uint32_t channelWidth, double txPowerW)
{
  NS_LOG_FUNCTION (centerFrequency << channelWidth << txPowerW);
  Ptr<SpectrumValue> c = Create<SpectrumValue> (GetSpectrumModel (centerFrequency, channelWidth));
  Values::iterator vit = c->ValuesBegin ();
  Bands::const_iterator bit = c->ConstBandsBegin ();
  double txPowerPerBand;
  switch (channelWidth)
    {
    case 20:
      // 56 subcarriers (52 data + 4 pilot) 
      NS_ASSERT_MSG (c->GetSpectrumModel ()->GetNumBands () == 129, "Unexpected number of bands");
      // skip 32 subbands, then place power in 28 of the next 32 subbands, then
      // skip the center subband, then place power in 28 of the next 32
      // subbands, then skip the final 32 subbands.  
      txPowerPerBand = txPowerW / 56;
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >=36 && i <=63) || (i >=65 && i <=92)) 
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands 36-63 and 65-92");
      NS_LOG_DEBUG ("Integrated power " << Integral (*c));
      NS_ASSERT_MSG (std::abs (txPowerW - Integral (*c)) < 1e-6, "Power allocation failed"); 
      break;
    case 40:
      // 112 subcarriers (104 data + 8 pilot) 
      // possible alternative:  114 subcarriers (108 data + 6 pilot)
      NS_ASSERT_MSG (c->GetSpectrumModel ()->GetNumBands () == 193, "Unexpected number of bands");
      txPowerPerBand = txPowerW / 112;
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >=36 && i <=63) || (i >=65 && i <=92) || (i >=100 && i<=127) || (i >=129 && i<= 156)) 
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands 36-63, 65-92, 100-127, and 129-156");
      NS_LOG_DEBUG ("Integrated power " << Integral (*c));
      NS_ASSERT_MSG (std::abs (txPowerW - Integral (*c)) < 1e-6, "Power allocation failed"); 
      break;
    case 80:
      // 224 subcarriers (208 data + 16 pilot) 
      // possible alternative:  242 subcarriers (234 data + 8 pilot)
      NS_ASSERT_MSG (c->GetSpectrumModel ()->GetNumBands () == 321, "Unexpected number of bands");
      txPowerPerBand = txPowerW / 224;
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >= 36 && i <= 63) || (i >= 65 && i <= 92) || 
              (i >= 100 && i <= 127) || (i >= 129 && i <= 156) ||
              (i >= 164 && i <= 191) || (i >= 193 && i <= 220) ||
              (i >= 228 && i <= 255) || (i >= 257 && i <= 284))
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands 36-63, 65-92, 100-127, 129-156, 164-191, 193-220, 228-255, 257-284");
      NS_LOG_DEBUG ("Integrated power " << Integral (*c));
      NS_ASSERT_MSG (std::abs (txPowerW - Integral (*c)) < 1e-6, "Power allocation failed"); 
      break;
    case 160:
      // 448 subcarriers (416 data + 32 pilot) VHT 
      // possible alternative:  484 subcarriers (468 data + 16 pilot)
      NS_ASSERT_MSG (c->GetSpectrumModel ()->GetNumBands () == 577, "Unexpected number of bands");
      txPowerPerBand = txPowerW / 448;
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >= 36 && i <= 63) || (i >= 65 && i <= 92) || 
              (i >= 100 && i <= 127) || (i >= 129 && i <= 156) ||
              (i >= 164 && i <= 191) || (i >= 193 && i <= 220) ||
              (i >= 228 && i <= 255) || (i >= 257 && i <= 284) ||
              (i >= 292 && i <= 319) || (i >= 321 && i <= 348) ||
              (i >= 356 && i <= 383) || (i >= 385 && i <= 412) ||
              (i >= 420 && i <= 447) || (i >= 449 && i <= 476) ||
              (i >= 484 && i <= 511) || (i >= 513 && i <= 540))
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands 36-63, 65-92, 100-127, 129-156, 164-191, 193-220, 228-255, 257-284, 292-319, 321-348, 356-383, 385-412, 420-447, 449-476, 484-511, and 513-540");
      NS_LOG_DEBUG ("Integrated power " << Integral (*c));
      NS_ASSERT_MSG (std::abs (txPowerW - Integral (*c)) < 1e-6, "Power allocation failed"); 
      break;
    default:
      NS_FATAL_ERROR ("ChannelWidth " << channelWidth << " unsupported");
      break;
    }
  return c;
}

Ptr<SpectrumValue>
WifiSpectrumValueHelper::CreateOfdmTxPowerSpectralDensity (uint32_t centerFrequency, uint32_t channelWidth, double txPowerW)
{
  NS_LOG_FUNCTION (centerFrequency << channelWidth << txPowerW);
  Ptr<SpectrumValue> c = Create<SpectrumValue> (GetSpectrumModel (centerFrequency, channelWidth));
  Values::iterator vit = c->ValuesBegin ();
  Bands::const_iterator bit = c->ConstBandsBegin ();
  double txPowerPerBand;
  switch (channelWidth)
    {
    case 20:
      // 52 subcarriers (48 data + 4 pilot)
      // skip 38 subbands, then place power in 26 subbands, then
      // skip the center subband, then place power in 26 subbands, then skip
      // the final 38 subbands.  
      NS_ASSERT_MSG (c->GetSpectrumModel ()->GetNumBands () == 129, "Unexpected number of bands");
      txPowerPerBand = txPowerW / 52;
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >=38 && i <=63) || (i >=65 && i <=90)) 
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
          else
            {
              *vit = 0;
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands 38-63 and 65-90");
      NS_LOG_DEBUG ("Integrated power " << Integral (*c));
      NS_ASSERT_MSG (std::abs (txPowerW - Integral (*c)) < 1e-6, "Power allocation failed"); 
      break;
    case 10:
      // 28 subcarriers (24 data + 4 pilot)
      // skip 34 subbands, then place power in 14 subbands, then
      // skip the center subband, then place power in 14 subbands, then skip
      // the final 34 subbands.  
      NS_ASSERT_MSG (c->GetSpectrumModel ()->GetNumBands () == 97, "Unexpected number of bands");
      txPowerPerBand = txPowerW / 28;
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >=34 && i <=47) || (i >=49 && i <=62)) 
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
          else
            {
              *vit = 0;
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands 34-47 and 49-62");
      NS_LOG_DEBUG ("Integrated power " << Integral (*c));
      NS_ASSERT_MSG (std::abs (txPowerW - Integral (*c)) < 1e-6, "Power allocation failed"); 
      break;
    case 5:
      // 16 subcarriers (12 data + 4 pilot)
      // skip 34 subbands, then place power in 14 subbands, then
      // skip the center subband, then place power in 14 subbands, then skip
      // the final 34 subbands.  
      NS_ASSERT_MSG (c->GetSpectrumModel ()->GetNumBands () == 81, "Unexpected number of bands");
      txPowerPerBand = txPowerW / 16;
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >=32 && i <=39) || (i >=41 && i <=48)) 
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
          else
            {
              *vit = 0;
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands 32-39 and 41-48");
      NS_LOG_DEBUG ("Integrated power " << Integral (*c));
      NS_ASSERT_MSG (std::abs (txPowerW - Integral (*c)) < 1e-6, "Power allocation failed"); 
      break;
    default:
      NS_FATAL_ERROR ("ChannelWidth " << channelWidth << " unsupported");
      break;
    }
  return c;
}

// Power allocated to 71 center subbands out of 135 total subbands in the band 
Ptr<SpectrumValue>
WifiSpectrumValueHelper::CreateDsssTxPowerSpectralDensity (uint32_t centerFrequency, double txPowerW)
{
  NS_LOG_FUNCTION (centerFrequency << txPowerW);
  uint32_t channelWidth = 22;  // DSSS channels are 22 MHz wide
  Ptr<SpectrumValue> c = Create<SpectrumValue> (GetSpectrumModel (centerFrequency, channelWidth));
  Values::iterator vit = c->ValuesBegin ();
  Bands::const_iterator bit = c->ConstBandsBegin ();
  double txPowerPerBand;
  // Evenly spread power across 22 MHz (71 bands)
  NS_ASSERT (c->GetSpectrumModel ()->GetNumBands () == 135);
  txPowerPerBand = txPowerW / 71;
  for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
    {
      if (i >=32 && i <=102) 
        {
          *vit = txPowerPerBand / (bit->fh - bit->fl);
        }
    }
  return c;
}

Ptr<SpectrumValue>
WifiSpectrumValueHelper::CreateNoisePowerSpectralDensity (uint32_t centerFrequency, uint32_t channelWidth, double noiseFigure)
{
  Ptr<SpectrumModel> model = GetSpectrumModel (centerFrequency, channelWidth);
  return CreateNoisePowerSpectralDensity (noiseFigure, model);
}

Ptr<SpectrumValue>
WifiSpectrumValueHelper::CreateNoisePowerSpectralDensity (double noiseFigureDb, Ptr<SpectrumModel> spectrumModel)
{
  NS_LOG_FUNCTION (noiseFigureDb << spectrumModel);

  // see "LTE - From theory to practice"
  // Section 22.4.4.2 Thermal Noise and Receiver Noise Figure
  const double kT_dBm_Hz = -174.0;  // dBm/Hz
  double kT_W_Hz = std::pow (10.0, (kT_dBm_Hz - 30) / 10.0);
  double noiseFigureLinear = std::pow (10.0, noiseFigureDb / 10.0);
  double noisePowerSpectralDensity =  kT_W_Hz * noiseFigureLinear;

  Ptr<SpectrumValue> noisePsd = Create <SpectrumValue> (spectrumModel);
  (*noisePsd) = noisePowerSpectralDensity;
  NS_LOG_DEBUG ("NoisePowerSpectralDensity has integrated power of " << Integral (*noisePsd));
  return noisePsd;
}

Ptr<SpectrumValue>
WifiSpectrumValueHelper::CreateRfFilter (uint32_t centerFrequency, uint32_t channelWidth)
{
  NS_LOG_FUNCTION (centerFrequency << channelWidth);
  Ptr<SpectrumValue> c = Create <SpectrumValue> (GetSpectrumModel (centerFrequency, channelWidth));
  size_t numBands = c->GetSpectrumModel ()->GetNumBands ();
  Bands::const_iterator bit = c->ConstBandsBegin ();
  Values::iterator vit = c->ValuesBegin ();
  uint32_t bandBandwidth = static_cast<uint32_t> (((bit->fh - bit->fl) + 0.5));
  NS_LOG_DEBUG ("Band bandwidth: " << bandBandwidth);
  size_t numBandsInFilter = static_cast<size_t> (channelWidth * 1e6 / bandBandwidth); 
  if (channelWidth % bandBandwidth != 0)
    {
      numBandsInFilter += 1;
    }
  NS_LOG_DEBUG ("Num bands in filter: " << numBandsInFilter);
  // Set the value of the filter to 1 for the center-most numBandsInFilter
  NS_ASSERT_MSG ((numBandsInFilter % 2 == 1) && (numBands % 2 == 1), "Should have odd number of bands");
  size_t startIndex = (numBands - numBandsInFilter) / 2;
  vit += startIndex;
  bit += startIndex;
  for (size_t i = startIndex; i < startIndex + numBandsInFilter; i++, vit++, bit++)
    {
      *vit = 1;
    }
  NS_LOG_DEBUG ("Added subbands " << startIndex << " to " << startIndex + numBandsInFilter << " to filter");
  return c;
}

static Ptr<SpectrumModel> g_WifiSpectrumModel5Mhz;

WifiSpectrumValueHelper::~WifiSpectrumValueHelper ()
{
}

WifiSpectrumValue5MhzFactory::~WifiSpectrumValue5MhzFactory ()
{
}

/**
 * Static class to initialize the values for the 2.4 GHz Wi-Fi spectrum model
 */
static class WifiSpectrumModel5MhzInitializer
{
public:
  WifiSpectrumModel5MhzInitializer ()
  {
    Bands bands;
    for (int i = -4; i < 13 + 7; i++)
      {
        BandInfo bi;
        bi.fl = 2407.0e6 + i * 5.0e6;
        bi.fh = 2407.0e6 + (i + 1) * 5.0e6;
        bi.fc = (bi.fl +  bi.fh) / 2;
        bands.push_back (bi);
      }
    g_WifiSpectrumModel5Mhz = Create<SpectrumModel> (bands);
  }
} g_WifiSpectrumModel5MhzInitializerInstance; //!< initialization instance for WifiSpectrumModel5Mhz



Ptr<SpectrumValue>
WifiSpectrumValue5MhzFactory::CreateConstant (double v)
{
  Ptr<SpectrumValue> c = Create <SpectrumValue> (g_WifiSpectrumModel5Mhz);
  (*c) = v;
  return c;
}


Ptr<SpectrumValue>
WifiSpectrumValue5MhzFactory::CreateTxPowerSpectralDensity (double txPower, uint32_t channel)
{
  Ptr<SpectrumValue> txPsd = Create <SpectrumValue> (g_WifiSpectrumModel5Mhz);

  // since the spectrum model has a resolution of 5 MHz, we model
  // the transmitted signal with a constant density over a 20MHz
  // bandwidth centered on the center frequency of the channel. The
  // transmission power outside the transmission power density is
  // calculated considering the transmit spectrum mask, see IEEE
  // Std. 802.11-2007, Annex I

  double txPowerDensity = txPower / 20e6;

  NS_ASSERT (channel >= 1);
  NS_ASSERT (channel <= 13);

  (*txPsd)[channel - 1] = txPowerDensity * 1e-4;      // -40dB
  (*txPsd)[channel]     = txPowerDensity * 1e-4;      // -40dB
  (*txPsd)[channel + 1] = txPowerDensity * 0.0015849; // -28dB
  (*txPsd)[channel + 2] = txPowerDensity * 0.0015849; // -28dB
  (*txPsd)[channel + 3] = txPowerDensity;
  (*txPsd)[channel + 4] = txPowerDensity;
  (*txPsd)[channel + 5] = txPowerDensity;
  (*txPsd)[channel + 6] = txPowerDensity;
  (*txPsd)[channel + 7] = txPowerDensity * 0.0015849; // -28dB
  (*txPsd)[channel + 8] = txPowerDensity * 0.0015849; // -28dB
  (*txPsd)[channel + 9] = txPowerDensity * 1e-4;      // -40dB
  (*txPsd)[channel + 10] = txPowerDensity * 1e-4;      // -40dB

  return txPsd;
}


Ptr<SpectrumValue>
WifiSpectrumValue5MhzFactory::CreateRfFilter (uint32_t channel)
{
  Ptr<SpectrumValue> rf = Create <SpectrumValue> (g_WifiSpectrumModel5Mhz);

  NS_ASSERT (channel >= 1);
  NS_ASSERT (channel <= 13);

  (*rf)[channel + 3] = 1;
  (*rf)[channel + 4] = 1;
  (*rf)[channel + 5] = 1;
  (*rf)[channel + 6] = 1;

  return rf;
}


} // namespace ns3
