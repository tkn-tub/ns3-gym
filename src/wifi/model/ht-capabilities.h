/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013
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
 * Author: Ghada Badawy <gbadawy@gmail.com>
 */
#ifndef HT_CAPABILITIES_H
#define HT_CAPABILITIES_H

#include <stdint.h>
#include "ns3/buffer.h"
#include "ns3/attribute-helper.h"
#include "ns3/wifi-information-element.h"

/**
 * This defines the maximum number of supported MCSs that a STA is
 * allowed to have. Currently this number is set for IEEE 802.11n
 */
#define MAX_SUPPORTED_MCS  (77)

namespace ns3 {

/**
 * \brief The Ht Capabilities Information Element
 * \ingroup wifi
 *
 * This class knows how to serialise and deserialise the Ht Capabilities Information Element
 */
class HtCapabilities: public WifiInformationElement
{
public:
   HtCapabilities ();
  void SetLdpc (uint8_t ldpc);
  void SetSupportedChannelWidth (uint8_t supportedchannelwidth);
  void SetGreenfield (uint8_t greenfield);
  void SetShortGuardInterval20(uint8_t shortguardinterval);
  void SetHtCapabilitiesInfo(uint16_t ctrl);
  void SetAmpduParameters (uint8_t ctrl);
  void SetSupportedMcsSet (uint64_t ctrl1, uint64_t ctrl2);
  void SetHtSupported(uint8_t htsupported);
  void SetRxMcsBitmask(uint8_t index);
  bool IsSupportedMcs (uint8_t mcs);
  //returns the HT Capabilties info field in the HT Capabilities information element
  uint16_t GetHtCapabilitiesInfo (void) const;
  //returns the Ampdu parameters field in the HT Capabilities information element
  uint8_t GetAmpduParameters (void) const;
  //returns the first 64bytes of the Supported MCS field in the HT Capabilities information element
  uint64_t GetSupportedMcsSet1 (void) const;
  //returns the first 64bytes of the Supported MCS field in the HT Capabilities information element
  uint64_t GetSupportedMcsSet2 (void) const;
  uint8_t GetLdpc (void) const;
  uint8_t GetGreenfield (void) const;
  uint8_t GetShortGuardInterval20 (void) const;
  uint8_t GetSupportedChannelWidth (void) const; //2040 supported or not
  uint8_t* GetRxMcsBitmask();
  
  WifiInformationElementId ElementId () const;
  uint8_t GetInformationFieldSize () const;
  void SerializeInformationField (Buffer::Iterator start) const;
  uint8_t DeserializeInformationField (Buffer::Iterator start,
                                       uint8_t length);
 /*
   * This information element is a bit special in that it is only
   * included if the STA is an HT STA. To support this we
   * override the Serialize and GetSerializedSize methods of
   * WifiInformationElement.
   */
  Buffer::Iterator Serialize (Buffer::Iterator start) const;
  uint16_t GetSerializedSize () const;
 
private:
  uint8_t m_ldpc;
  uint8_t m_supportedChannelWidth;
  uint8_t m_smPowerSave;
  uint8_t m_greenField;
  uint8_t m_shortGuardInterval20; 
  uint8_t m_shortGuardInterval40;
  uint8_t m_txStbc;
  uint8_t m_rxStbc;
  uint8_t m_htDelayedBlockAck;
  uint8_t m_maxAmsduLength;
  uint8_t m_dssMode40;
  uint8_t m_reserved;
  uint8_t m_fortyMhzIntolerant;
  uint8_t m_lsigProtectionSupport;
  uint8_t m_maxAmpduLength;
  uint8_t m_minMpduStartSpace;
  uint8_t m_ampduReserved;
  uint8_t m_reservedMcsSet1;
  uint16_t m_rxHighestSupportedDataRate;
  uint8_t m_reservedMcsSet2;
  uint8_t m_txMcsSetDefined;
  uint8_t m_txRxMcsSetUnequal;
  uint8_t m_txMaxNSpatialStreams;
  uint8_t m_txUnequalModulation;
  uint32_t m_reservedMcsSet3; 
  uint8_t m_rxMcsBitmask[MAX_SUPPORTED_MCS];
  //this is used to decide if this element should be added to the frame or not
  uint8_t m_htSupported;
};

std::ostream &operator << (std::ostream &os, const HtCapabilities &htcapabilities);
std::istream &operator >> (std::istream &is, HtCapabilities &htcapabilities);

ATTRIBUTE_HELPER_HEADER (HtCapabilities)

} // namespace ns3

#endif /* HT_CAPABILITY_H */
