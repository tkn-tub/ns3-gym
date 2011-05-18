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

#ifndef LTE_SPECTRUM_VALUE_HELPER_H
#define LTE_SPECTRUM_VALUE_HELPER_H


#include <ns3/spectrum-value.h>
#include <vector>

namespace ns3 {


/**
 * \ingroup lte
 *
 * \brief This class defines all functions to create spectrum model for lte
 */
class LteSpectrumValueHelper
{
public:
  /**
   * \brief create spectrum value
   * \param powerTx the power transmission in dBm
   * \param channels the list of sub channels where the signal will be sent
   * \return a Ptr to a newly created SpectrumValue instance
   */
  Ptr<SpectrumValue> CreateDownlinkTxPowerSpectralDensity (double powerTx, std::vector <int> channels);

  /**
   * \brief create spectrum value
   * \param powerTx the power transmission in dBm
   * \param channels the list of sub channels where the signal will be sent
   * \return a Ptr to a newly created SpectrumValue instance
   */
  Ptr<SpectrumValue> CreateUplinkTxPowerSpectralDensity (double powerTx, std::vector <int> channels);


  /**
   * \brief create spectrum value for noise
   * \return a Ptr to a newly created SpectrumValue instance
   */
  Ptr<SpectrumValue> CreateDownlinkNoisePowerSpectralDensity (void);

  /**
   * \brief create spectrum value for noise
   * \return a Ptr to a newly created SpectrumValue instance
   */
  Ptr<SpectrumValue> CreateUplinkNoisePowerSpectralDensity (void);


};


} // namespace ns3



#endif /*  LTE_SPECTRUM_VALUE_HELPER_H */
