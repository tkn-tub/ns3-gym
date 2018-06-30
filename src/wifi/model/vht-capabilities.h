/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015
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
 * Authors: Ghada Badawy <gbadawy@gmail.com>
 *          Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#ifndef VHT_CAPABILITIES_H
#define VHT_CAPABILITIES_H

#include "wifi-information-element.h"

namespace ns3 {

/**
 * \ingroup wifi
 *
 * The IEEE 802.11ac VHT Capabilities
 */
class VhtCapabilities : public WifiInformationElement
{
public:
  VhtCapabilities ();
  /**
   * Set the VHT supported field.
   *
   * \param vhtsupported the VHT supported field
   */
  void SetVhtSupported (uint8_t vhtsupported);

  /**
   * Return the element ID.
   *
   * \returns the wifi information element ID
   */
  WifiInformationElementId ElementId () const;
  /**
   * Return the information field size.
   *
   * \returns the information field size
   */
  uint8_t GetInformationFieldSize () const;
  /**
   * Serialize the information field.
   *
   * \param start the information field iterator
   */
  void SerializeInformationField (Buffer::Iterator start) const;
  /**
   * Deserialize the information field.
   *
   * \param start the information field iterator
   * \param length the information field
   * \returns the information field
   */
  uint8_t DeserializeInformationField (Buffer::Iterator start, uint8_t length);

  /**
   * Set the VHT Capabilities Info field in the VHT Capabilities information element.
   *
   * \param ctrl the VHT Capabilities Info field in the VHT Capabilities information element
   */
  void SetVhtCapabilitiesInfo (uint32_t ctrl);
  /**
   * Set the MCS and NSS field in the VHT Capabilities information element.
   *
   * \param ctrl the MCS and NSS field in the VHT Capabilities information element
   */
  void SetSupportedMcsAndNssSet (uint64_t ctrl);

  /**
   * Return the VHT Capabilities Info field in the VHT Capabilities information element.
   *
   * \return the VHT Capabilities Info field in the VHT Capabilities information element
   */
  uint32_t GetVhtCapabilitiesInfo () const;
  /**
   * Return the MCS and NSS field in the VHT Capabilities information element.
   *
   * \return the MCS and NSS field in the VHT Capabilities information element
   */
  uint64_t GetSupportedMcsAndNssSet () const;

  //Capabilities Info fields
  /**
   * Set the maximum MPDU length.
   *
   * \param length the maximum MPDU length
   */
  void SetMaxMpduLength (uint8_t length);
  /**
   * Set the supported channel width set.
   *
   * \param channelwidthset the supported channel width set
   */
  void SetSupportedChannelWidthSet (uint8_t channelwidthset);
  /**
   * Set the receive LDPC.
   *
   * \param rxldpc the receive LDPC
   */
  void SetRxLdpc (uint8_t rxldpc);
  /**
   * Set the short guard interval 80 Mhz.
   *
   * \param shortguardinterval the short guard interval 80 Mhz
   */
  void SetShortGuardIntervalFor80Mhz (uint8_t shortguardinterval);
  /**
   * Set the short guard interval 160 Mhz.
   *
   * \param shortguardinterval the short guard interval 160 Mhz
   */
  void SetShortGuardIntervalFor160Mhz (uint8_t shortguardinterval);
  /**
   * Set the receive STBC.
   *
   * \param rxstbc the receive STBC
   */
  void SetRxStbc (uint8_t rxstbc);
  /**
   * Set the transmit STBC.
   *
   * \param txstbc the receive STBC
   */
  void SetTxStbc (uint8_t txstbc);
  /**
   * Set the maximum AMPDU length exponent.
   *
   * \param exponent the maximum AMPDU length exponent
   */
  void SetMaxAmpduLengthExponent (uint8_t exponent);

  /**
   * Get the supported channel width set.
   *
   * \returns the maximum MPDU length
   */
  uint8_t GetSupportedChannelWidthSet () const;
  /**
   * Get the receive LDPC.
   *
   * \returns the receive LDPC
   */
  uint8_t GetRxLdpc () const;
  /**
   * Get the receive STBC.
   *
   * \returns the receive STBC
   */
  uint8_t GetRxStbc () const;
  /**
   * Get the transmit STBC.
   *
   * \returns the transmit STBC
   */
  uint8_t GetTxStbc () const;

  /**
   * \param mcs Max MCS value (between 7 and 9)
   * \param nss Spatial stream for which the Max MCS value is being set
   */
  void SetRxMcsMap (uint8_t mcs, uint8_t nss);
  /**
   * \param mcs Max MCS value (between 7 and 9)
   * \param nss Spatial stream for which the Max MCS value is being set
   */
  void SetTxMcsMap (uint8_t mcs, uint8_t nss);
  /**
   * Set the receive highest supported LGI data rate.
   *
   * \param supporteddatarate receive highest supported LGI data rate
   */
  void SetRxHighestSupportedLgiDataRate (uint16_t supporteddatarate);
  /**
   * Set the transmit highest supported LGI data rate.
   *
   * \param supporteddatarate transmit highest supported LGI data rate
   */
  void SetTxHighestSupportedLgiDataRate (uint16_t supporteddatarate);
  /**
   * Get the is MCS supported.
   *
   * \param mcs the MCS
   * \param Nss the NSS
   * \returns the is MCS supported
   */
  bool IsSupportedMcs (uint8_t mcs, uint8_t Nss) const;

  /**
   * Get the receive highest supported LGI data rate.
   *
   * \returns the receive highest supported LGI data rate.
   */
  uint16_t GetRxHighestSupportedLgiDataRate () const;

  /**
   * Get the is transmit MCS supported.
   *
   * \param mcs the MCS
   * \returns is transmit MCS supported
   */
  bool IsSupportedTxMcs (uint8_t mcs) const;
  /**
   * Get the is receive MCS supported.
   *
   * \param mcs the MCS
   * \returns is receive MCS supported
   */
  bool IsSupportedRxMcs (uint8_t mcs) const;

  /**
   * This information element is a bit special in that it is only
   * included if the STA is an VHT STA. To support this we
   * override the Serialize and GetSerializedSize methods of
   * WifiInformationElement.
   *
   * \param start the iterator
   * \returns the serialized size
   */
  Buffer::Iterator Serialize (Buffer::Iterator start) const;
  /**
   * Get the serialized size.
   *
   * \returns the serialized size
   */
  uint16_t GetSerializedSize () const;


private:
  //Capabilities Info fields
  uint8_t m_maxMpduLength; ///< maximum MPDU length
  uint8_t m_supportedChannelWidthSet; ///< supported channel width set
  uint8_t m_rxLdpc; ///< receive LDPC
  uint8_t m_shortGuardIntervalFor80Mhz; ///< short guard interval 80 MHz
  uint8_t m_shortGuardIntervalFor160Mhz;  ///< short guard interval 160 MHz
  uint8_t m_txStbc; ///< transmit STBC
  uint8_t m_rxStbc; ///< receive STBC
  uint8_t m_suBeamformerCapable; ///< SU beam former capable
  uint8_t m_suBeamformeeCapable; ///< SU beam formee capable
  uint8_t m_beamformeeStsCapable; ///< beam formee STS capable
  uint8_t m_numberOfSoundingDimensions; ///< number of sounding dimensions
  uint8_t m_muBeamformerCapable; ///< MU beam former capable
  uint8_t m_muBeamformeeCapable; ///< MU beam formee capable
  uint8_t m_vhtTxopPs; ///< VHT TXOP PS
  uint8_t m_htcVhtCapable; ///< HTC VHT capable
  uint8_t m_maxAmpduLengthExponent; ///< maximum AMPDU length exponent
  uint8_t m_vhtLinkAdaptationCapable; ///< VHT link adaptation capable
  uint8_t m_rxAntennaPatternConsistency; ///< receive antenna pattern consistency
  uint8_t m_txAntennaPatternConsistency; ///< transmit antenna pattern consistency

  //MCS and NSS field information
  std::vector<uint8_t> m_rxMcsMap; ///< receive MCS map
  uint16_t m_rxHighestSupportedLongGuardIntervalDataRate; ///< receive highest supported long guard interval data rate
  std::vector<uint8_t> m_txMcsMap; ///< transmit MCS map
  uint16_t m_txHighestSupportedLongGuardIntervalDataRate;  ///< transmit highest supported long guard interval data rate

  /// This is used to decide if this element should be added to the frame or not
  uint8_t m_vhtSupported;
};

std::ostream &operator << (std::ostream &os, const VhtCapabilities &vhtcapabilities);

} //namespace ns3

#endif /* VHT_CAPABILITY_H */
