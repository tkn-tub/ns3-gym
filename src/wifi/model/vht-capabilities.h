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

#include <stdint.h>
#include "ns3/buffer.h"
#include "ns3/attribute-helper.h"
#include "ns3/wifi-information-element.h"

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
  void SetVhtSupported (uint8_t vhtsupported);

  WifiInformationElementId ElementId () const;
  uint8_t GetInformationFieldSize () const;
  void SerializeInformationField (Buffer::Iterator start) const;
  uint8_t DeserializeInformationField (Buffer::Iterator start, uint8_t length);

  /**
   * Set the VHT Capabilties Info field in the VHT Capabilities information element.
   *
   * \param ctrl the VHT Capabilties Info field in the VHT Capabilities information element
   */
  void SetVhtCapabilitiesInfo (uint32_t ctrl);
  /**
   * Set the MCS and NSS field in the VHT Capabilities information element.
   *
   * \param ctrl the MCS and NSS field in the VHT Capabilities information element
   */
  void SetSupportedMcsAndNssSet (uint64_t ctrl);

  /*
   * Return the VHT Capabilties Info field in the VHT Capabilities information element.
   *
   * \return the VHT Capabilties Info field in the VHT Capabilities information element
   */
  uint32_t GetVhtCapabilitiesInfo () const;
  /*
   * Return the MCS and NSS field in the VHT Capabilities information element.
   *
   * \return the MCS and NSS field in the VHT Capabilities information element
   */
  uint64_t GetSupportedMcsAndNssSet () const;

  //Capabilities Info fields
  void SetMaxMpduLength (uint8_t length);
  void SetSupportedChannelWidthSet (uint8_t channelwidthset);
  void SetRxLdpc (uint8_t rxldpc);
  void SetShortGuardIntervalFor80Mhz (uint8_t shortguardinterval);
  void SetShortGuardIntervalFor160Mhz (uint8_t shortguardinterval);
  void SetRxStbc (uint8_t rxstbc);
  void SetTxStbc (uint8_t txstbc);
  void SetMaxAmpduLengthExponent (uint8_t exponent);

  uint8_t GetMaxMpduLength () const;
  uint8_t GetSupportedChannelWidthSet () const;
  uint8_t GetRxLdpc () const;
  uint8_t GetShortGuardIntervalFor80Mhz () const;
  uint8_t GetShortGuardIntervalFor160Mhz () const;
  uint8_t GetRxStbc () const;
  uint8_t GetTxStbc () const;
  uint8_t GetMaxAmpduLengthExponent () const;

  //MCS and NSS field information
  void SetRxMcsMap (uint16_t map);
  void SetRxMcsMap (uint8_t mcs, uint8_t nss);
  void SetTxMcsMap (uint16_t map);
  void SetTxMcsMap (uint8_t mcs, uint8_t nss);
  void SetRxHighestSupportedLgiDataRate (uint16_t supporteddatarate);
  void SetTxHighestSupportedLgiDataRate (uint16_t supporteddatarate);
  bool IsSupportedMcs (uint8_t mcs, uint8_t Nss) const;

  uint16_t GetRxMcsMap () const;
  uint16_t GetTxMcsMap () const;
  uint16_t GetRxHighestSupportedLgiDataRate () const;
  uint16_t GetTxHighestSupportedLgiDataRate () const;

  bool IsSupportedTxMcs (uint8_t mcs) const;
  bool IsSupportedRxMcs (uint8_t mcs) const;

  /*
   * This information element is a bit special in that it is only
   * included if the STA is an VHT STA. To support this we
   * override the Serialize and GetSerializedSize methods of
   * WifiInformationElement.
   */
  Buffer::Iterator Serialize (Buffer::Iterator start) const;
  uint16_t GetSerializedSize () const;


private:
  //Capabilities Info fields
  uint16_t m_maxMpduLength;
  uint16_t m_supportedChannelWidthSet;
  uint8_t m_rxLdpc;
  uint8_t m_shortGuardIntervalFor80Mhz;
  uint8_t m_shortGuardIntervalFor160Mhz;
  uint8_t m_txStbc;
  uint8_t m_rxStbc;
  uint8_t m_suBeamformerCapable;
  uint8_t m_suBeamformeeCapable;
  uint8_t m_beamformeeStsCapable;
  uint8_t m_numberOfSoundingDimensions;
  uint8_t m_muBeamformerCapable;
  uint8_t m_muBeamformeeCapable;
  uint8_t m_vhtTxopPs;
  uint8_t m_htcVhtCapable;
  uint8_t m_maxAmpduLengthExponent;
  uint8_t m_vhtLinkAdaptationCapable;
  uint8_t m_rxAntennaPatternConsistency;
  uint8_t m_txAntennaPatternConsistency;

  //MCS and NSS field information
  std::vector<uint8_t> m_rxMcsMap;
  uint16_t m_rxHighestSupportedLongGuardIntervalDataRate;
  std::vector<uint8_t> m_txMcsMap;
  uint16_t m_txHighestSupportedLongGuardIntervalDataRate;

  //This is used to decide if this element should be added to the frame or not
  uint8_t m_vhtSupported;
};

std::ostream &operator << (std::ostream &os, const VhtCapabilities &vhtcapabilities);
std::istream &operator >> (std::istream &is, VhtCapabilities &vhtcapabilities);

ATTRIBUTE_HELPER_HEADER (VhtCapabilities)

} //namespace ns3

#endif /* VHT_CAPABILITY_H */
