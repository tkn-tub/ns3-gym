/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 CTTC
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
 * Authors: Nicola Baldo <nbaldo@cttc.es>
 *          Ghada Badawy <gbadawy@gmail.com>
 */

#ifndef WIFI_TX_VECTOR_H
#define WIFI_TX_VECTOR_H

#include "wifi-mode.h"
#include "wifi-preamble.h"

namespace ns3 {


/**
 * This class mimics the TXVECTOR which is to be
 * passed to the PHY in order to define the parameters which are to be
 * used for a transmission. See IEEE 802.11-2007 15.2.6 "Transmit PLCP",
 * and also 15.4.4.2 "PMD_SAP peer-to-peer service primitive
 * parameters".
 *
 * If this class is constructed with the constructor that takes no
 * arguments, then the client must explicitly set the mode and
 * transmit power level parameters before using them.  Default
 * member initializers are provided for the other parameters, to
 * conform to a non-MIMO/long guard configuration, although these
 * may also be explicitly set after object construction.
 *
 * When used in a infrastructure context, WifiTxVector values should be
 * drawn from WifiRemoteStationManager parameters since rate adaptation
 * is responsible for picking the mode, number of streams, etc., but in
 * the case in which there is no such manager (e.g. mesh), the client
 * still needs to initialize at least the mode and transmit power level
 * appropriately.
 *
 * \note the above reference is valid for the DSSS PHY only (clause
 * 15). TXVECTOR is defined also for the other PHYs, however they
 * don't include the TXPWRLVL explicitly in the TXVECTOR. This is
 * somewhat strange, since all PHYs actually have a
 * PMD_TXPWRLVL.request primitive. We decide to include the power
 * level in WifiTxVector for all PHYs, since it serves better our
 * purposes, and furthermore it seems close to the way real devices
 * work (e.g., madwifi).
 */
class WifiTxVector
{
public:
  WifiTxVector ();
  /**
   * Create a TXVECTOR with the given parameters.
   *
   * \param mode WifiMode
   * \param powerLevel transmission power level
   * \param preamble preamble type
   * \param guardInterval the guard interval duration in nanoseconds
   * \param nTx the number of TX antennas
   * \param nss the number of spatial STBC streams (NSS)
   * \param ness the number of extension spatial streams (NESS)
   * \param channelWidth the channel width in MHz
   * \param aggregation enable or disable MPDU aggregation
   * \param stbc enable or disable STBC
   */
  WifiTxVector (WifiMode mode,
                uint8_t powerLevel,
                WifiPreamble preamble,
                uint16_t guardInterval,
                uint8_t nTx,
                uint8_t nss,
                uint8_t ness,
                uint16_t channelWidth,
                bool aggregation,
                bool stbc);
  /**
   * \returns the selected payload transmission mode
   */
  WifiMode GetMode (void) const;
  /**
  * Sets the selected payload transmission mode
  *
  * \param mode
  */
  void SetMode (WifiMode mode);
  /**
   * \returns the transmission power level
   */
  uint8_t GetTxPowerLevel (void) const;
  /**
   * Sets the selected transmission power level
   *
   * \param powerlevel
   */
  void SetTxPowerLevel (uint8_t powerlevel);
  /**
   * \returns the preamble type
   */
  WifiPreamble GetPreambleType (void) const;
  /**
   * Sets the preamble type
   *
   * \param preamble
   */
  void SetPreambleType (WifiPreamble preamble);
  /**
   * \returns the channel width (in MHz)
   */
  uint16_t GetChannelWidth (void) const;
  /**
   * Sets the selected channelWidth (in MHz)
   *
   * \param channelWidth
   */
  void SetChannelWidth (uint16_t channelWidth);
  /**
   * \returns the guard interval duration (in nanoseconds)
   */
  uint16_t GetGuardInterval (void) const;
  /**
  * Sets the guard interval duration (in nanoseconds)
  *
  * \param guardInterval the guard interval duration (in nanoseconds)
  */
  void SetGuardInterval (uint16_t guardInterval);
  /**
   * \returns the number of TX antennas
   */
  uint8_t GetNTx (void) const;
  /**
   * Sets the number of TX antennas
   *
   * \param nTx
   */
  void SetNTx (uint8_t nTx);
  /**
   * \returns the number of Nss
   */
  uint8_t GetNss (void) const;
  /**
   * Sets the number of Nss refer to IEEE 802.11n Table 20-28 for explanation and range
   *
   * \param nss
   */
  void SetNss (uint8_t nss);
  /**
   * \returns the number of Ness
   */
  uint8_t GetNess (void) const;
  /**
   * Sets the Ness number refer to IEEE 802.11n Table 20-6 for explanation
   *
   * \param ness
   */
  void SetNess (uint8_t ness);
  /**
   * Checks whether the PSDU contains A-MPDU.
   *  \returns true if this PSDU has A-MPDU aggregation,
   *           false otherwise.
   */
  bool IsAggregation (void) const;
  /**
   * Sets if PSDU contains A-MPDU.
   *
   * \param aggregation whether the PSDU contains A-MPDU or not.
   */
  void SetAggregation (bool aggregation);
  /**
   * Check if STBC is used or not
   *
   * \returns true if STBC is used,
   *           false otherwise
   */
  bool IsStbc (void) const;
  /**
   * Sets if STBC is being used
   *
   * \param stbc enable or disable STBC
   */
  void SetStbc (bool stbc);
  /**
   * The standard disallows certain combinations of WifiMode, number of
   * spatial streams, and channel widths.  This method can be used to
   * check whether this WifiTxVector contains an invalid combination.
   *
   * \return true if the WifiTxVector parameters are allowed by the standard
   */
   bool IsValid (void) const;


private:
  WifiMode m_mode;               /**< The DATARATE parameter in Table 15-4.
                                 It is the value that will be passed
                                 to PMD_RATE.request */
  uint8_t  m_txPowerLevel;       /**< The TXPWR_LEVEL parameter in Table 15-4.
                                 It is the value that will be passed
                                 to PMD_TXPWRLVL.request */
  WifiPreamble m_preamble;       /**< preamble */
  uint16_t m_channelWidth;       /**< channel width in MHz */
  uint16_t m_guardInterval;      /**< guard interval duration in nanoseconds */
  uint8_t  m_nTx;                /**< number of TX antennas */
  uint8_t  m_nss;                /**< number of spatial streams */
  uint8_t  m_ness;               /**< number of spatial streams in beamforming */
  bool     m_aggregation;        /**< Flag whether the PSDU contains A-MPDU. */
  bool     m_stbc;               /**< STBC used or not */

  bool     m_modeInitialized;         /**< Internal initialization flag */
  bool     m_txPowerLevelInitialized; /**< Internal initialization flag */
};

/**
 * Serialize WifiTxVector to the given ostream.
 *
 * \param os
 * \param v
 *
 * \return ostream
 */
std::ostream & operator << (std::ostream & os,const WifiTxVector &v);

} //namespace ns3

#endif /* WIFI_TX_VECTOR_H */
