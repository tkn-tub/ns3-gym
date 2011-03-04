/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */


#include "lte-spectrum-value-helper.h"
#include <cmath>

namespace ns3 {

Ptr<SpectrumModel> LteDownlinkSpectrumModel;
Ptr<SpectrumModel> LteUplinkSpectrumModel;

class static_LteDownlinkSpectrumModel_initializer
{
public:
  static_LteDownlinkSpectrumModel_initializer ()
  {

    /*
     * Operating  Bands for the UL: 1920 MHz – 1980 MHz
     * see for details TR.36.101 - Tab 5.5-1
     *
     */

    std::vector<double> freqs;
    for (int i = 0; i < 100; ++i)
      {

        double centralFrequencyOfPRB = 1.920 + (i * 0.00018);
        freqs.push_back (centralFrequencyOfPRB * 1e9);
      }

    LteDownlinkSpectrumModel = Create<SpectrumModel> (freqs);
  }

} static_LteDownlinkSpectrumModel_initializer_instance;



class static_LteUplinkSpectrumModel_initializer
{
public:
  static_LteUplinkSpectrumModel_initializer ()
  {

    /*
     * Operating  Bands for the DL: 2110 MHz – 2170 MHz
     * see for details TR.36.101 - Tab 5.5-1
     *
     */

    std::vector<double> freqs;
    for (int i = 0; i < 100; ++i)
      {

        double centralFrequencyOfPRB = 2.110 + (i * 0.00018);
        freqs.push_back (centralFrequencyOfPRB * 1e9);
      }

    LteUplinkSpectrumModel = Create<SpectrumModel> (freqs);
  }

} static_LteUplinkSpectrumModel_initializer_instance;




Ptr<SpectrumValue>
LteSpectrumValueHelper::CreateDownlinkTxPowerSpectralDensity (double powerTx, std::vector<int> channels)
{
  Ptr<SpectrumValue> txPsd = Create <SpectrumValue> (LteDownlinkSpectrumModel);

  // powerTx is expressed in dBm. We must convert it into natural unit.
  powerTx = pow (10., (powerTx - 30) / 10);

  double txPowerDensity = (powerTx / channels.size ()) / 180000;

  for (std::vector <int>::iterator it = channels.begin (); it != channels.end (); it++)
    {
      int idSubChannel = (*it);
      (*txPsd)[idSubChannel] = txPowerDensity;
    }

  return txPsd;
}


Ptr<SpectrumValue>
LteSpectrumValueHelper::CreateUplinkTxPowerSpectralDensity (double powerTx, std::vector<int> channels)
{
  Ptr<SpectrumValue> txPsd = Create <SpectrumValue> (LteUplinkSpectrumModel);

  // powerTx is expressed in dBm. We must convert it into natural unit.
  powerTx = pow (10., (powerTx - 30) / 10);

  double txPowerDensity = (powerTx / channels.size ()) / 180000;

  for (std::vector <int>::iterator it = channels.begin (); it != channels.end (); it++)
    {
      int idSubChannel = (*it);
      (*txPsd)[idSubChannel] = txPowerDensity;
    }

  return txPsd;
}


Ptr<SpectrumValue>
LteSpectrumValueHelper::CreateDownlinkNoisePowerSpectralDensity (void)
{
  Ptr<SpectrumValue> txPsd = Create <SpectrumValue> (LteDownlinkSpectrumModel);

  double noise_db = 2.5 + (-174) + (10. * log10 (180000)) - 30;
  double noisePowerDensity = (pow (10.,noise_db / 10)) / 180000;

  (*txPsd) = noisePowerDensity;

  return txPsd;
}


Ptr<SpectrumValue>
LteSpectrumValueHelper::CreateUplinkNoisePowerSpectralDensity (void)
{
  Ptr<SpectrumValue> txPsd = Create <SpectrumValue> (LteUplinkSpectrumModel);

  double noise_db = 2.5 + (-174) + (10. * log10 (180000)) - 30;
  double noisePowerDensity = (pow (10.,noise_db / 10)) / 180000;

  (*txPsd) = noisePowerDensity;

  return txPsd;
}

} // namespace ns3
