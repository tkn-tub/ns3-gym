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

///< Wifi Spectrum Model structure
struct WifiSpectrumModelId
{
  /**
   * Constructor
   * \param f the frequency in Mhz
   * \param w the channel width in Mhz
   */ 
  WifiSpectrumModelId (uint32_t f, uint8_t w);
  uint32_t m_centerFrequency;  ///< center frequency
  uint8_t m_channelWidth; ///< channel width
};

WifiSpectrumModelId::WifiSpectrumModelId (uint32_t f, uint8_t w)
  : m_centerFrequency (f),
    m_channelWidth (w)
{
  NS_LOG_FUNCTION (this << f << (uint16_t)w);
}

/**
 * Less than operator
 * \param a the first wifi spectrum to compare
 * \param b the second wifi spectrum to compare
 * \returns true if the first spectrum is less than the second spectrum
 */ 
bool
operator < (const WifiSpectrumModelId& a, const WifiSpectrumModelId& b)
{
  return ( (a.m_centerFrequency < b.m_centerFrequency) || ( (a.m_centerFrequency == b.m_centerFrequency) && (a.m_channelWidth < b.m_channelWidth)));
}

static std::map<WifiSpectrumModelId, Ptr<SpectrumModel> > g_wifiSpectrumModelMap; ///< static initializer for the class

Ptr<SpectrumModel>
WifiSpectrumValueHelper::GetSpectrumModel (uint32_t centerFrequency, uint8_t channelWidth, double bandBandwidth, uint8_t guardBandwidth)
{
  NS_LOG_FUNCTION (centerFrequency << (uint16_t)channelWidth << bandBandwidth << (uint16_t)guardBandwidth);
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
      double bandwidth = (channelWidth + (2 * guardBandwidth)) * 1e6;
      // For OFDM, the center subcarrier is null (at center frequency)
      uint32_t numBands = static_cast<uint32_t> ((bandwidth / bandBandwidth) + 0.5);
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

// Power allocated to 71 center subbands out of 135 total subbands in the band 
Ptr<SpectrumValue>
WifiSpectrumValueHelper::CreateDsssTxPowerSpectralDensity (uint32_t centerFrequency, double txPowerW, uint8_t guardBandwidth)
{
  NS_LOG_FUNCTION (centerFrequency << txPowerW << (uint16_t)guardBandwidth);
  uint8_t channelWidth = 22;  // DSSS channels are 22 MHz wide
  double bandBandwidth = 312500;
  Ptr<SpectrumValue> c = Create<SpectrumValue> (GetSpectrumModel (centerFrequency, channelWidth, bandBandwidth, guardBandwidth));
  Values::iterator vit = c->ValuesBegin ();
  Bands::const_iterator bit = c->ConstBandsBegin ();
  uint32_t nGuardBands = static_cast<uint32_t>(((2 * guardBandwidth * 1e6) / bandBandwidth) + 0.5);
  uint32_t nAllocatedBands = static_cast<uint32_t>(((channelWidth * 1e6) / bandBandwidth) + 0.5);
  NS_ASSERT (c->GetSpectrumModel ()->GetNumBands () == (nAllocatedBands + nGuardBands + 1));
  // Evenly spread power across 22 MHz
  double txPowerPerBand = txPowerW / nAllocatedBands;
  for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
    {
      if ((i >= (nGuardBands / 2)) && (i <= ((nGuardBands / 2) + nAllocatedBands - 1)))
        {
          *vit = txPowerPerBand / (bit->fh - bit->fl);
        }
    }
  return c;
}

Ptr<SpectrumValue>
WifiSpectrumValueHelper::CreateOfdmTxPowerSpectralDensity (uint32_t centerFrequency, uint8_t channelWidth, double txPowerW, uint8_t guardBandwidth)
{
  NS_LOG_FUNCTION (centerFrequency << (uint16_t)channelWidth << txPowerW << (uint16_t)guardBandwidth);
  double bandBandwidth = 312500;
  Ptr<SpectrumValue> c = Create<SpectrumValue> (GetSpectrumModel (centerFrequency, channelWidth, bandBandwidth, guardBandwidth));
  uint32_t nGuardBands = static_cast<uint32_t>(((2 * guardBandwidth * 1e6) / bandBandwidth) + 0.5);
  uint32_t nAllocatedBands = static_cast<uint32_t>(((channelWidth * 1e6) / bandBandwidth) + 0.5);
  NS_ASSERT_MSG (c->GetSpectrumModel ()->GetNumBands () == (nAllocatedBands + nGuardBands + 1), "Unexpected number of bands " << c->GetSpectrumModel ()->GetNumBands ());
  double txPowerPerBand = 0;
  uint32_t start1 = 0;
  uint32_t stop1 = 0;
  uint32_t start2 = 0;
  uint32_t stop2 = 0;
  switch (channelWidth)
    {
    case 20:
    default:
      // 52 subcarriers (48 data + 4 pilot)
      // skip guard band and 6 subbands, then place power in 26 subbands, then
      // skip the center subband, then place power in 26 subbands, then skip
      // the final 6 subbands and the guard band.
      txPowerPerBand = txPowerW / 52;
      start1 = (nGuardBands / 2) + 6;
      stop1 = start1 + 26 - 1;
      start2 = stop1 + 2;
      stop2 = start2 + 26 - 1;
      break;
    case 10:
      // 28 subcarriers (24 data + 4 pilot)
      // skip guard band and 2 subbands, then place power in 14 subbands, then
      // skip the center subband, then place power in 14 subbands, then skip
      // the final 2 subbands and the guard band.
      txPowerPerBand = txPowerW / 28;
      start1 = (nGuardBands / 2) + 2;
      stop1 = start1 + 14 - 1;
      start2 = stop1 + 2;
      stop2 = start2 + 14 - 1;
      break;
    case 5:
      // 16 subcarriers (12 data + 4 pilot)
      // skip guard band and 2 subbands, then place power in 8 subbands, then
      // skip the center subband, then place power in 8 subbands, then skip
      // the final 2 subbands and the guard.
      txPowerPerBand = txPowerW / 16;
      start1 = (nGuardBands / 2) + 2;
      stop1 = start1 + 8 - 1;
      start2 = stop1 + 2;
      stop2 = start2 + 8 - 1;
      break;
    }
  NS_LOG_DEBUG ("Power per band " << txPowerPerBand);
  Values::iterator vit = c->ValuesBegin ();
  Bands::const_iterator bit = c->ConstBandsBegin ();
  for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
    {
      if ((i >= start1 && i <= stop1) || (i >= start2 && i <= stop2))
        {
          *vit = txPowerPerBand / (bit->fh - bit->fl);
        }
      else
        {
          *vit = 0;
        }
    }
  NS_LOG_DEBUG ("Added signal power to subbands " << start1 << "-" << stop1 << " and " << start2 << "-" << stop2);
  NS_LOG_DEBUG ("Integrated power " << Integral (*c));
  NS_ASSERT_MSG (std::abs (txPowerW - Integral (*c)) < 1e-6, "Power allocation failed");
  return c;
}

Ptr<SpectrumValue>
WifiSpectrumValueHelper::CreateHtOfdmTxPowerSpectralDensity (uint32_t centerFrequency, uint8_t channelWidth, double txPowerW, uint8_t guardBandwidth)
{
  NS_LOG_FUNCTION (centerFrequency << (uint16_t)channelWidth << txPowerW << (uint16_t)guardBandwidth);
  double bandBandwidth = 312500;
  Ptr<SpectrumValue> c = Create<SpectrumValue> (GetSpectrumModel (centerFrequency, channelWidth, bandBandwidth, guardBandwidth));
  Values::iterator vit = c->ValuesBegin ();
  Bands::const_iterator bit = c->ConstBandsBegin ();
  uint32_t nGuardBands = static_cast<uint32_t>(((2 * guardBandwidth * 1e6) / bandBandwidth) + 0.5);
  uint32_t nAllocatedBands = static_cast<uint32_t>(((channelWidth * 1e6) / bandBandwidth) + 0.5);
  NS_ASSERT_MSG (c->GetSpectrumModel ()->GetNumBands () == (nAllocatedBands + nGuardBands + 1), "Unexpected number of bands " << c->GetSpectrumModel ()->GetNumBands ());
  double txPowerPerBand;
  // skip the guard band and 4 subbands, then place power in 28 subbands, then
  // skip the center subband, then place power in 28 subbands, then skip
  // the final 4 subbands and the guard band.
  // Repeat for each 20 MHz band.
  uint32_t start1 = (nGuardBands / 2) + 4;
  uint32_t stop1 = start1 + 28 - 1;
  uint32_t start2 = stop1 + 2;
  uint32_t stop2 = start2 + 28 - 1;
  uint32_t start3 = stop2 + (2 * 4);
  uint32_t stop3 = start3 + 28 - 1;
  uint32_t start4 = stop3 + 2;
  uint32_t stop4 = start4 + 28 - 1;
  uint32_t start5 = stop4 + (2 * 4);
  uint32_t stop5 = start5 + 28 - 1;
  uint32_t start6 = stop5 + 2;
  uint32_t stop6 = start6 + 28 - 1;
  uint32_t start7 = stop6 + (2 * 4);
  uint32_t stop7 = start7 + 28 - 1;
  uint32_t start8 = stop7 + 2;
  uint32_t stop8 = start8 + 28 - 1;
  uint32_t start9 = stop8 + (2 * 4);
  uint32_t stop9 = start9 + 28 - 1;
  uint32_t start10 = stop9 + 2;
  uint32_t stop10 = start10 + 28 - 1;
  uint32_t start11 = stop10 + (2 * 4);
  uint32_t stop11 = start11 + 28 - 1;
  uint32_t start12 = stop11 + 2;
  uint32_t stop12 = start12 + 28 - 1;
  uint32_t start13 = stop12 + (2 * 4);
  uint32_t stop13 = start13 + 28 - 1;
  uint32_t start14 = stop13 + 2;
  uint32_t stop14 = start14 + 28 - 1;
  uint32_t start15 = stop14 + (2 * 4);
  uint32_t stop15 = start15 + 28 - 1;
  uint32_t start16 = stop15 + 2;
  uint32_t stop16 = start16 + 28 - 1;
  switch (channelWidth)
    {
    case 20:
      // 56 subcarriers (52 data + 4 pilot)
      txPowerPerBand = txPowerW / 56;
      NS_LOG_DEBUG ("Power per band " << txPowerPerBand);
      start1 = (nGuardBands / 2) + 4;
      stop1 = start1 + 28 - 1;
      start2 = stop1 + 2;
      stop2 = start2 + 28 - 1;
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >= start1 && i <= stop1) || (i >= start2 && i <= stop2))
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
          else
            {
              *vit = 0;
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands " << start1 << "-" << stop1 <<
                    " and " << start2 << "-" << stop2);
      break;
    case 40:
      // 112 subcarriers (104 data + 8 pilot) 
      // possible alternative:  114 subcarriers (108 data + 6 pilot)
      txPowerPerBand = txPowerW / 112;
      NS_LOG_DEBUG ("Power per band " << txPowerPerBand);
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >= start1 && i <= stop1) || (i >= start2 && i <= stop2) ||
              (i >= start3 && i <= stop3) || (i >= start4 && i <= stop4))
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
          else
            {
              *vit = 0;
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands " << start1 << "-" << stop1 <<
                    ", " << start2 << "-" << stop2 <<
                    ", " << start3 << "-" << stop3 <<
                    ", " << start4 << "-" << stop4);
      break;
    case 80:
      // 224 subcarriers (208 data + 16 pilot) 
      // possible alternative:  242 subcarriers (234 data + 8 pilot)
      txPowerPerBand = txPowerW / 224;
      NS_LOG_DEBUG ("Power per band " << txPowerPerBand);
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >= start1 && i <= stop1) || (i >= start2 && i <= stop2) ||
              (i >= start3 && i <= stop3) || (i >= start4 && i <= stop4) ||
              (i >= start5 && i <= stop5) || (i >= start6 && i <= stop6) ||
              (i >= start7 && i <= stop7) || (i >= start8 && i <= stop8))
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
          else
            {
              *vit = 0;
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands " << start1 << "-" << stop1 <<
                    ", " << start2 << "-" << stop2 <<
                    ", " << start3 << "-" << stop3 <<
                    ", " << start4 << "-" << stop4 <<
                    ", " << start5 << "-" << stop5 <<
                    ", " << start6 << "-" << stop6 <<
                    ", " << start7 << "-" << stop7 <<
                    ", " << start8 << "-" << stop8);
      break;
    case 160:
      // 448 subcarriers (416 data + 32 pilot)
      // possible alternative:  484 subcarriers (468 data + 16 pilot)
      txPowerPerBand = txPowerW / 448;
      NS_LOG_DEBUG ("Power per band " << txPowerPerBand);
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >= start1 && i <= stop1) || (i >= start2 && i <= stop2) ||
              (i >= start3 && i <= stop3) || (i >= start4 && i <= stop4) ||
              (i >= start5 && i <= stop5) || (i >= start6 && i <= stop6) ||
              (i >= start7 && i <= stop7) || (i >= start8 && i <= stop8) ||
              (i >= start9 && i <= stop9) || (i >= start10 && i <= stop10) ||
              (i >= start11 && i <= stop11) || (i >= start12 && i <= stop12) ||
              (i >= start13 && i <= stop13) || (i >= start14 && i <= stop14) ||
              (i >= start15 && i <= stop15) || (i >= start16 && i <= stop16))
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
          else
            {
              *vit = 0;
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands " << start1 << "-" << stop1 <<
                    ", " << start2 << "-" << stop2 <<
                    ", " << start3 << "-" << stop3 <<
                    ", " << start4 << "-" << stop4 <<
                    ", " << start5 << "-" << stop5 <<
                    ", " << start6 << "-" << stop6 <<
                    ", " << start7 << "-" << stop7 <<
                    ", " << start8 << "-" << stop8 <<
                    ", " << start9 << "-" << stop9 <<
                    ", " << start10 << "-" << stop10 <<
                    ", " << start11 << "-" << stop11 <<
                    ", " << start12 << "-" << stop12 <<
                    ", " << start13 << "-" << stop13 <<
                    ", " << start14 << "-" << stop14 <<
                    ", " << start15 << "-" << stop15 <<
                    ", " << start16 << "-" << stop16);
      break;
    }
  NS_LOG_DEBUG ("Integrated power " << Integral (*c));
  NS_ASSERT_MSG (std::abs (txPowerW - Integral (*c)) < 1e-6, "Power allocation failed");
  return c;
}

Ptr<SpectrumValue>
WifiSpectrumValueHelper::CreateHeOfdmTxPowerSpectralDensity (uint32_t centerFrequency, uint8_t channelWidth, double txPowerW, uint8_t guardBandwidth)
{
  NS_LOG_FUNCTION (centerFrequency << (uint16_t)channelWidth << txPowerW << (uint16_t)guardBandwidth);
  double bandBandwidth = 78125;
  Ptr<SpectrumValue> c = Create<SpectrumValue> (GetSpectrumModel (centerFrequency, channelWidth, bandBandwidth, guardBandwidth));
  Values::iterator vit = c->ValuesBegin ();
  Bands::const_iterator bit = c->ConstBandsBegin ();
  uint32_t nGuardBands = static_cast<uint32_t>(((2 * guardBandwidth * 1e6) / bandBandwidth) + 0.5);
  uint32_t nAllocatedBands = static_cast<uint32_t>(((channelWidth * 1e6) / bandBandwidth) + 0.5);
  NS_ASSERT_MSG (c->GetSpectrumModel ()->GetNumBands () == (nAllocatedBands + nGuardBands + 1), "Unexpected number of bands " << c->GetSpectrumModel ()->GetNumBands ());
  double txPowerPerBand;
  uint32_t start1;
  uint32_t stop1;
  uint32_t start2;
  uint32_t stop2;
  uint32_t start3;
  uint32_t stop3;
  uint32_t start4;
  uint32_t stop4;
  switch (channelWidth)
    {
    case 20:
      // 242 subcarriers (234 data + 8 pilot)
      txPowerPerBand = txPowerW / 242;
      NS_LOG_DEBUG ("Power per band " << txPowerPerBand);
      // skip the guard band and 11 subbands, then place power in 121 subbands, then
      // skip 3 DC, then place power in 121 subbands, then skip
      // the final 11 subbands and the guard band.
      start1 = (nGuardBands / 2) + 12;
      stop1 = start1 + 121 - 1;
      start2 = stop1 + 4;
      stop2 = start2 + 121 - 1;
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >= start1 && i <= stop1) || (i >= start2 && i <= stop2))
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
          else
            {
              *vit = 0;
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands " << start1 << "-" << stop1 <<
                    " and " << start2 << "-" << stop2);
      break;
    case 40:
      // 484 subcarriers (468 data + 16 pilot)
      txPowerPerBand = txPowerW / 484;
      NS_LOG_DEBUG ("Power per band " << txPowerPerBand);
      // skip the guard band and 11 subbands, then place power in 242 subbands, then
      // skip 5 DC, then place power in 242 subbands, then skip
      // the final 11 subbands and the guard band.
      start1 = (nGuardBands / 2) + 12;
      stop1 = start1 + 242 - 1;
      start2 = stop1 + 6;
      stop2 = start2 + 242 - 1;
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >= start1 && i <= stop1) || (i >= start2 && i <= stop2))
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
          else
            {
              *vit = 0;
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands " << start1 << "-" << stop1 <<
                    " and " << start2 << "-" << stop2);
      break;
    case 80:
      // 996 subcarriers (980 data + 16 pilot)
      txPowerPerBand = txPowerW / 996;
      NS_LOG_DEBUG ("Power per band " << txPowerPerBand);
      // skip the guard band and 11 subbands, then place power in 498 subbands, then
      // skip 5 DC, then place power in 498 subbands, then skip
      // the final 11 subbands and the guard band.
      start1 = (nGuardBands / 2) + 12;
      stop1 = start1 + 498 - 1;
      start2 = stop1 + 6;
      stop2 = start2 + 498 - 1;
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >= start1 && i <= stop1) || (i >= start2 && i <= stop2))
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
          else
            {
              *vit = 0;
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands " << start1 << "-" << stop1 <<
                    " and " << start2 << "-" << stop2);
      break;
    case 160:
      // 2 x 996 subcarriers (2 x 80 MHZ bands)
      txPowerPerBand = txPowerW / (2 * 996);
      NS_LOG_DEBUG ("Power per band " << txPowerPerBand);
      start1 = (nGuardBands / 2) + 12;
      stop1 = start1 + 498 - 1;
      start2 = stop1 + 6;
      stop2 = start2 + 498 - 1;
      start3 = stop2 + (2 * 12);
      stop3 = start3 + 498 - 1;
      start4 = stop3 + 6;
      stop4 = start4 + 498 - 1;
      for (size_t i = 0; i < c->GetSpectrumModel ()->GetNumBands (); i++, vit++, bit++)
        {
          if ((i >= start1 && i <= stop1) || (i >= start2 && i <= stop2) ||
              (i >= start3 && i <= stop3) || (i >= start4 && i <= stop4))
            {
              *vit = txPowerPerBand / (bit->fh - bit->fl);
            }
          else
            {
              *vit = 0;
            }
        }
      NS_LOG_DEBUG ("Added signal power to subbands " << start1 << "-" << stop1 <<
                    ", " << start2 << "-" << stop2 <<
                    ", " << start3 << "-" << stop3 <<
                    ", " << start4 << "-" << stop4);
      break;
    default:
      NS_FATAL_ERROR ("ChannelWidth " << channelWidth << " unsupported");
      break;
    }
  NS_LOG_DEBUG ("Integrated power " << Integral (*c));
  NS_ASSERT_MSG (std::abs (txPowerW - Integral (*c)) < 1e-6, "Power allocation failed");
  return c;
}

Ptr<SpectrumValue>
WifiSpectrumValueHelper::CreateNoisePowerSpectralDensity (uint32_t centerFrequency, uint8_t channelWidth, double bandBandwidth, double noiseFigure, uint8_t guardBandwidth)
{
  Ptr<SpectrumModel> model = GetSpectrumModel (centerFrequency, channelWidth, bandBandwidth, guardBandwidth);
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
WifiSpectrumValueHelper::CreateRfFilter (uint32_t centerFrequency, uint8_t channelWidth, double bandGranularity, uint8_t guardBandwidth)
{
  NS_LOG_FUNCTION (centerFrequency << (uint16_t)channelWidth << bandGranularity << (uint16_t)guardBandwidth);
  Ptr<SpectrumValue> c = Create <SpectrumValue> (GetSpectrumModel (centerFrequency, channelWidth, bandGranularity, guardBandwidth));
  size_t numBands = c->GetSpectrumModel ()->GetNumBands ();
  Bands::const_iterator bit = c->ConstBandsBegin ();
  Values::iterator vit = c->ValuesBegin ();
  uint32_t bandBandwidth = static_cast<uint32_t> (bandGranularity);
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

static Ptr<SpectrumModel> g_WifiSpectrumModel5Mhz; ///< static initializer for the class

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
WifiSpectrumValue5MhzFactory::CreateTxPowerSpectralDensity (double txPower, uint8_t channel)
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
WifiSpectrumValue5MhzFactory::CreateRfFilter (uint8_t channel)
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
