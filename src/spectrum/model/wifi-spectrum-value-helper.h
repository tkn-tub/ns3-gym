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
 *  This class defines all functions to create a spectrum model for 
 *  Wi-Fi based on a a spectral model aligned with an OFDM subcarrier
 *  spacing of 312.5 KHz (model also reused for DSSS modulations)
 */
class WifiSpectrumValueHelper
{
public:
  /**
   * Destructor
   */
  virtual ~WifiSpectrumValueHelper ();
  
  /**
   * Return a SpectrumModel instance corresponding to the center frequency
   * and channel width.  The model includes +/- 10 MHz of guard bands
   * (i.e. the model will span (channelWidth + 20) MHz of bandwidth).
   *
   * \param centerFrequency center frequency (MHz)
   * \param channelWidth channel width (MHz)
   * \param bandBandwidth width of each band (Hz)
   * \param guardBandwidth width of the guard band (MHz)
   *
   * \return the static SpectrumModel instance corresponding to the
   * given carrier frequency and channel width configuration. 
   */
  static Ptr<SpectrumModel> GetSpectrumModel (uint32_t centerFrequency, uint8_t channelWidth, double bandBandwidth, uint8_t guardBandwidth);

  /**
   * Create a transmit power spectral density corresponding to DSSS 
   *
   * The center frequency typically corresponds to 802.11b channel 
   * center frequencies but is not restricted to those frequencies.
   *
   * \param centerFrequency center frequency (MHz)
   * \param txPowerW  transmit power (W) to allocate
   * \param guardBandwidth width of the guard band (MHz)
   * \returns a pointer to a newly allocated SpectrumValue representing the DSSS Transmit Power Spectral Density in W/Hz
   */
  static Ptr<SpectrumValue> CreateDsssTxPowerSpectralDensity (uint32_t centerFrequency, double txPowerW, uint8_t guardBandwidth);

  /**
   * Create a transmit power spectral density corresponding to OFDM 
   * (802.11a/g).  Channel width may vary between 20, 10, and 5 MHz.
   *
   * \param centerFrequency center frequency (MHz)
   * \param channelWidth channel width (MHz)
   * \param txPowerW  transmit power (W) to allocate
   * \param guardBandwidth width of the guard band (MHz)
   * \return a pointer to a newly allocated SpectrumValue representing the OFDM Transmit Power Spectral Density in W/Hz for each Band
   */
  static Ptr<SpectrumValue> CreateOfdmTxPowerSpectralDensity (uint32_t centerFrequency, uint8_t channelWidth, double txPowerW, uint8_t guardBandwidth);

  /**
   * Create a transmit power spectral density corresponding to OFDM 
   * High Throughput (HT) (802.11n/ac).  Channel width may vary between 
   * 20, 40, 80, and 160 MHz.
   *
   * \param centerFrequency center frequency (MHz)
   * \param channelWidth channel width (MHz)
   * \param txPowerW  transmit power (W) to allocate
   * \param guardBandwidth width of the guard band (MHz)
   * \return a pointer to a newly allocated SpectrumValue representing the HT OFDM Transmit Power Spectral Density in W/Hz for each Band
   */
  static Ptr<SpectrumValue> CreateHtOfdmTxPowerSpectralDensity (uint32_t centerFrequency, uint8_t channelWidth, double txPowerW, uint8_t guardBandwidth);

  /**
   * Create a transmit power spectral density corresponding to OFDM 
   * High Efficiency (HE) (802.11ax).  Channel width may vary between 
   * 20, 40, 80, and 160 MHz.
   *
   * \param centerFrequency center frequency (MHz)
   * \param channelWidth channel width (MHz)
   * \param txPowerW  transmit power (W) to allocate
   * \param guardBandwidth width of the guard band (MHz)
   * \return a pointer to a newly allocated SpectrumValue representing the HE OFDM Transmit Power Spectral Density in W/Hz for each Band
   */
  static Ptr<SpectrumValue> CreateHeOfdmTxPowerSpectralDensity (uint32_t centerFrequency, uint8_t channelWidth, double txPowerW, uint8_t guardBandwidth);

  /**
   * Create a power spectral density corresponding to the noise
   *
   * \param centerFrequency center frequency (MHz)
   * \param channelWidth channel width (MHz)
   * \param bandBandwidth width of each band (Hz)
   * \param noiseFigure the noise figure in dB w.r.t. a reference temperature of 290K
   * \param guardBandwidth width of the guard band (MHz)
   * \return a pointer to a newly allocated SpectrumValue representing the noise Power Spectral Density in W/Hz for each Band
   */
  static Ptr<SpectrumValue> CreateNoisePowerSpectralDensity (uint32_t centerFrequency, uint8_t channelWidth, double bandBandwidth, double noiseFigure, uint8_t guardBandwidth);

  /**
   * Create a thermal noise power spectral density
   *
   * \param noiseFigure the noise figure
   * \param spectrumModel the spectrum model
   * \return a pointer to a newly allocated SpectrumValue representing the noise Power Spectral Density in W/Hz corresponding to thermal noise, for each Band
   */
  static Ptr<SpectrumValue> CreateNoisePowerSpectralDensity (double noiseFigure, Ptr<SpectrumModel> spectrumModel);

  /**
   * Create a spectral density corresponding to the RF filter
   *
   * \param centerFrequency center frequency (MHz)
   * \param channelWidth channel width (MHz)
   * \param bandBandwidth width of each band (Hz)
   * \param guardBandwidth width of the guard band (MHz)
   *
   * \return a pointer to a SpectrumValue representing the RF filter applied
   * to an received power spectral density
   */
  static Ptr<SpectrumValue> CreateRfFilter (uint32_t centerFrequency, uint8_t channelWidth, double bandBandwidth, uint8_t guardBandwidth);
};

/**
 * \ingroup spectrum
 *
 * Implements Wifi SpectrumValue for the 2.4 GHz ISM band only, with a
 * 5 MHz spectrum resolution.
 *
 */
class WifiSpectrumValue5MhzFactory
{
public:
  /**
   * Destructor
   */
  virtual ~WifiSpectrumValue5MhzFactory ();
  /**
   * Creates a SpectrumValue instance with a constant value for all frequencies
   *
   * @param psd the constant value
   *
   * @return a Ptr to a newly created SpectrumValue
   */
  virtual Ptr<SpectrumValue> CreateConstant (double psd);
  /**
   * Creates a SpectrumValue instance that represents the TX Power Spectral 
   * Density  of a wifi device corresponding to the provided parameters
   *
   * Since the spectrum model has a resolution of 5 MHz, we model
   * the transmitted signal with a constant density over a 20MHz
   * bandwidth centered on the center frequency of the channel. The
   * transmission power outside the transmission power density is
   * calculated considering the transmit spectrum mask, see IEEE
   * Std. 802.11-2007, Annex I.  The two bands just outside of the main
   * 20 MHz are allocated power at -28 dB down from the center 20 MHz,
   * and the two bands outside of this are allocated power at -40 dB down
   * (with a total bandwidth of 60 MHz containing non-zero power allocation).
   *
   * @param txPower the total TX power in W
   * @param channel the number of the channel (1 <= channel <= 13)
   *
   * @return a Ptr to a newly created SpectrumValue
   */
  virtual Ptr<SpectrumValue> CreateTxPowerSpectralDensity (double txPower, uint8_t channel);
  /**
   * Creates a SpectrumValue instance which
   * represents the frequency response of the RF filter which is used
   * by a wifi device to receive signals when tuned to a particular channel
   *
   * @param channel the number of the channel (1 <= channel <= 13)
   *
   * @return a Ptr to a newly created SpectrumValue
   */
  virtual Ptr<SpectrumValue> CreateRfFilter (uint8_t channel);
};

} // namespace ns3

#endif /*  WIFI_SPECTRUM_VALUE_HELPER_H */
