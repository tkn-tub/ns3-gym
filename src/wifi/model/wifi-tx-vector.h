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

#include <ns3/wifi-mode.h>
#include <ostream>

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
   * \param retries retries
   * \param shortGuardInterval enable or disable short guard interval
   * \param nss the number of spatial STBC streams (NSS)
   * \param ness the number of extension spatial streams (NESS)
   * \param stbc enable or disable STBC
   */
  WifiTxVector (WifiMode mode, uint8_t powerLevel, uint8_t retries, bool shortGuardInterval, uint8_t nss, uint8_t ness, bool stbc);
  /**
   * \returns the txvector payload mode
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
   * \returns the number of retries
   */
  uint8_t GetRetries (void) const;
  /**
   * Sets the number of retries
   *
   * \param retries
   */
  void SetRetries (uint8_t retries);
  /**
   * \returns if ShortGuardInterval is used or not
   */
  bool IsShortGuardInterval (void) const;
  /**
  * Sets if short gurad interval is being used
  *
  * \param guardinterval enable or disable short guard interval
  */
  void SetShortGuardInterval (bool guardinterval);
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


private:
  WifiMode m_mode;               /**< The DATARATE parameter in Table 15-4.
                                 It is the value that will be passed
                                 to PMD_RATE.request */
  uint8_t  m_txPowerLevel;       /**< The TXPWR_LEVEL parameter in Table 15-4.
                                 It is the value that will be passed
                                 to PMD_TXPWRLVL.request */
  uint8_t  m_retries;            /**< The DATA_RETRIES/RTS_RETRIES parameter
                                 for Click radiotap information */

  bool     m_shortGuardInterval; /**< true if short GI is going to be used */
  uint8_t  m_nss;                /**< number of streams */
  uint8_t  m_ness;               /**< number of streams in beamforming */
  bool     m_stbc;               /**< STBC used or not */

  bool     m_modeInitialized;         //*< Internal initialization flag */
  bool     m_txPowerLevelInitialized; //*< Internal initialization flag */

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
