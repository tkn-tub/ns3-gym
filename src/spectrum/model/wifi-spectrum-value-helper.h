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

#ifndef WIFI_SPECTRUM_VALUE_HELPER_H
#define WIFI_SPECTRUM_VALUE_HELPER_H


#include <ns3/spectrum-value.h>

namespace ns3 {



/**
 * \ingroup spectrum
 *
 * this abstract class defines the interface for interacting with all WifiSpectrumValue implementations.
 *
 */
class WifiSpectrumValueHelper
{
public:
  virtual ~WifiSpectrumValueHelper ();
  /*
   *
   * @param value the constant value
   *
   * @return a Ptr to a newly created SpectrumValue instance which
   * has a constant value for all frequencies
   */
  virtual Ptr<SpectrumValue> CreateConstant (double psd) = 0;

  /*
   *
   * @param txPower the total TX power in W
   * @param channel the number of the channel
   *
   * @return a Ptr to a newly created SpectrumValue instance which
   * represents the TX Power Spectral Density  of a wifi device
   * corresponding to the provided parameters
   */
  virtual Ptr<SpectrumValue> CreateTxPowerSpectralDensity (double txPower, uint32_t channel) = 0;

  /*
   *
   * @param channel the number of the channel
   *
   * @return a Ptr to a newly created SpectrumValue instance which
   * represents the frequency response of the RF filter which is used
   * by a wifi device to receive signals when tuned to a particular channel
   */
  virtual Ptr<SpectrumValue> CreateRfFilter (uint32_t channel) = 0;

};



/**
 * \ingroup spectrum
 *
 * Implements WifiSpectrumValue for the 2.4 GHz ISM band only, with a
 * 5 MHz spectrum resolution.
 *
 */
class WifiSpectrumValue5MhzFactory
{
public:
  virtual ~WifiSpectrumValue5MhzFactory ();
  // inherited from WifiSpectrumValue
  virtual Ptr<SpectrumValue> CreateConstant (double psd);
  virtual Ptr<SpectrumValue> CreateTxPowerSpectralDensity (double txPower, uint32_t channel);
  virtual Ptr<SpectrumValue> CreateRfFilter (uint32_t channel);

};



} // namespace ns3



#endif /*  WIFI_SPECTRUM_VALUE_HELPER_H */
