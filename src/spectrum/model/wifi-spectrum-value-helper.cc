/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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
 */


#include "wifi-spectrum-value-helper.h"

namespace ns3 {

/// The Wi-Fi spectrum model
static Ptr<SpectrumModel> g_WifiSpectrumModel5Mhz;

WifiSpectrumValueHelper::~WifiSpectrumValueHelper ()
{
}

WifiSpectrumValue5MhzFactory::~WifiSpectrumValue5MhzFactory ()
{
}

/**
 * Static class to initialize the values for the Wi-Fi spectrum model
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
