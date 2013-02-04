/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
   * Calculates the carrier frequency from the E-UTRA Absolute
   * Radio Frequency Channel Number (EARFCN) according to 3GPP TS
   * 36.101 section 5.7.3 "Carrier frequency and EARFCN".
   *
   * \param earfcn the EARFCN
   *
   * \return the carrier frequency in Hz
   */
  static double GetCarrierFrequency (uint16_t earfcn);

  /**
   * Calculates the dowlink carrier frequency from the E-UTRA Absolute
   * Radio Frequency Channel Number (EARFCN) using the formula in 3GPP TS
   * 36.101 section 5.7.3 "Carrier frequency and EARFCN".
   *
   * \param earfcn the EARFCN
   *
   * \return the dowlink carrier frequency in Hz
   */
  static double GetDownlinkCarrierFrequency (uint16_t earfcn);

  /**
   * Calculates the uplink carrier frequency from the E-UTRA Absolute
   * Radio Frequency Channel Number (EARFCN) using the formula in 3GPP TS
   * 36.101 section 5.7.3 "Carrier frequency and EARFCN".
   *
   * \param earfcn the EARFCN
   *
   * \return the uplink carrier frequency in Hz
   */
  static double GetUplinkCarrierFrequency (uint16_t earfcn);

  /**
   *
   *
   * \param txBandwidthConf the tranmission bandwidth
   * configuration in number of resource blocks
   *
   * \return the nominal channel bandwidth in Hz as per 3GPP TS 36.101
   */
  static double GetChannelBandwidth (uint8_t txBandwidthConf);

  /**
   *
   * \param earfcn the carrier frequency (EARFCN) at which reception
   * is made
   * \param bandwidth the Transmission Bandwidth Configuration in
   * number of resource blocks
   *
   * \return the static SpectrumModel instance corresponding to the
   * given carrier frequency and transmission bandwidth
   * configuration. If such SpectrumModel does not exist, it is
   * created.
   */
  static Ptr<SpectrumModel> GetSpectrumModel (uint16_t earfcn, uint8_t bandwidth);


  /**
   * create a spectrum value representing the power spectral
   * density of a signal to be transmitted. See 3GPP TS 36.101 for
   * a definition of most of the parameters described here.
   *
   * \param earfcn the carrier frequency (EARFCN) of the transmission
   * \param bandwidth the Transmission Bandwidth Configuration in
   * number of resource blocks
   * \param txPower the total power in dBm over the whole bandwidth
   * \param ActiveRbs the list of Active Resource Blocks (PRBs)
   *
   * \return a pointer to a newly allocated SpectrumValue representing the TX Power Spectral Density in W/Hz for each Resource Block
   */
  static Ptr<SpectrumValue> CreateTxPowerSpectralDensity (uint16_t earfcn, uint8_t bandwidth, double powerTx, std::vector <int> activeRbs);


  /**
   * create a SpectrumValue that models the power spectral density of AWGN
   *
   * \param earfcn the carrier frequency (EARFCN) at which reception
   * is made
   * \param bandwidth the Transmission Bandwidth Configuration in
   * number of resource blocks
   * \param noiseFigure the noise figure in dB w.r.t. a reference temperature of 290K
   *
   * \return a pointer to a newly allocated SpectrumValue representing the noise Power Spectral Density in W/Hz for each Resource Block
   */
  static Ptr<SpectrumValue> CreateNoisePowerSpectralDensity (uint16_t earfcn, uint8_t bandwidth, double noiseFigure);

  /**
   *  create a SpectrumValue that models the power spectral density of AWGN
   *
   * \param noiseFigure  the noise figure in dB  w.r.t. a reference temperature of 290K
   * \param spectrumModel the SpectrumModel instance to be used
   *
   * \return a pointer to a newly allocated SpectrumValue representing the noise Power Spectral Density in W/Hz for each Resource Block
   */
  static Ptr<SpectrumValue> CreateNoisePowerSpectralDensity (double noiseFigure, Ptr<SpectrumModel> spectrumModel);

};


} // namespace ns3



#endif /*  LTE_SPECTRUM_VALUE_HELPER_H */
