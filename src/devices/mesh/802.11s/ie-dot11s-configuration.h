/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Aleksey Kovalenko <kovalenko@iitp.ru>
 */


#ifndef MESH_CONFIGURATION_H
#define MESH_CONFIGURATION_H

#include "ns3/wifi-information-element.h"

namespace ns3 {
/**
 * \ingroup dot11s
 * \brief See 7.3.2.81.1 in 802.11s draft 2.07
 */
enum dot11sPathSelectionProtocol
{
  PROTOCOL_HWMP = 0x000fac00,
  PROTOCOL_NULL = 0x000facff,
};
/**
 * \ingroup dot11s
 * \brief See 7.3.2.81.2 in 802.11s draft 2.07
 */
enum dot11sPathSelectionMetric
{
  METRIC_AIRTIME = 0x000fac00,
  METRIC_NULL    = 0x000facff,
};
/**
 * \ingroup dot11s
 * \brief See 7.3.2.81.3 in 802.11s draft 2.07
 */
enum dot11sCongestionControlMode
{
  CONGESTION_DEFAULT = 0x000fac00,
  CONGESTION_NULL    = 0x000facff,
};
/**
 * \ingroup dot11s
 * \brief See 7.3.2.81.4 in 802.11s draft 2.07
 */
enum dot11sChannelPrecedence
{
  CHANNEL_PRECEDENCE_OFF = 0x000fac00,
};

/**
 * \ingroup dot11s
 * \brief See 7.3.2.81.5 in 802.11s draft 2.07
 */
class dot11sMeshCapability
{
public:
  dot11sMeshCapability();
  uint8_t  GetSerializedSize () const;
  Buffer::Iterator Serialize (Buffer::Iterator i) const;
  Buffer::Iterator Deserialize (Buffer::Iterator i);
  bool acceptPeerLinks;
  bool MDAEnabled;
  bool forwarding;
  bool beaconTimingReport;
  bool TBTTAdjustment;
  bool powerSaveLevel;

  bool Is(uint16_t cap,uint8_t n) const;
};

/**
 * \ingroup dot11s
 * \brief Describes Mesh Configuration Element 
 * see 7.3.2.81 of 802.11s draft 2.07
 */
class IeDot11sConfiguration : public WifiInformationElement
{
public:
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;

  IeDot11sConfiguration();
  void   SetRouting(dot11sPathSelectionProtocol routingId);
  void   SetMetric(dot11sPathSelectionMetric metricId);
  bool   IsHWMP();
  bool   IsAirtime();

  dot11sMeshCapability const& MeshCapability();
private:
  WifiElementId ElementId () const
  {
    return IE11S_MESH_CONFIGURATION;
  }
  uint8_t  GetInformationSize () const;
  void SerializeInformation (Buffer::Iterator i) const;
  uint8_t DeserializeInformation (Buffer::Iterator i, uint8_t length);
  void PrintInformation(std::ostream& os) const;
  /** Active Path Selection Protocol ID */
  dot11sPathSelectionProtocol m_APSId;
  /** Active Path Metric ID */
  dot11sPathSelectionMetric   m_APSMId;
  /** Congestion Control Mode ID */
  dot11sCongestionControlMode m_CCMId;
  /* Channel Precedence */
  dot11sChannelPrecedence m_CP;
  dot11sMeshCapability m_meshCap;
};
} //name space NS3
#endif
