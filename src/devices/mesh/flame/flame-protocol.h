/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#ifndef FLAME_PROTOCOL_H
#define FLAME_PROTOCOL_H

#include "flame-protocol-mac.h"
#include "flame-header.h"
#include "flame-rtable.h"

#include "ns3/mesh-l2-routing-protocol.h"
#include "ns3/tag.h"
#include <map>
namespace ns3 {
namespace flame {
/**
 * \brief Seqno and TTL tag
 */

class FlameTag : public Tag
{
public:
  /// Retransmitter:
  Mac48Address  address;
  
  FlameTag (Mac48Address a = Mac48Address ()) :
    address (a){}
  
  ///\name Inherited from Tag
  //\{
  static  TypeId  GetTypeId ();
  TypeId  GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void  Serialize (TagBuffer i) const;
  void  Deserialize (TagBuffer i);
  void  Print (std::ostream &os) const;
  //\}
};

/**
 * \ingroup flame
 * 
 * \brief FLAME = Forwarding Layer for Meshing
 */

class FlameProtocol : public MeshL2RoutingProtocol
{
public:
  static TypeId GetTypeId ();
  FlameProtocol ();
  ~FlameProtocol ();
  void DoDispose ();
  
  /// Route request, inherited from MeshL2RoutingProtocol
  bool RequestRoute (uint32_t  sourceIface, const Mac48Address source, const Mac48Address destination,
      Ptr<const Packet>  packet, uint16_t  protocolType, RouteReplyCallback  routeReply);
  /// Cleanup flame headers!
  bool RemoveRoutingStuff (uint32_t fromIface, const Mac48Address source,
      const Mac48Address destination, Ptr<Packet>  packet, uint16_t&  protocolType);
  /** 
   * \brief Install FLAME on given mesh point. 
   * 
   * Installing protocol cause installing its interface MAC plugins.
   *  
   * Also MP aggregates all installed protocols, FLAME protocol can be accessed 
   * via MeshPointDevice::GetObject<dot11s::FlameProtocol>();
   */
  bool Install (Ptr<MeshPointDevice>);
  Mac48Address GetAddress ();
  ///\brief Statistics:
  void Report (std::ostream &) const;
  void ResetStats ();
private:
  /**
   * \brif Handles a packet: adds a routing information and drops
   * packets by TTL or Seqno
   * \returns true if packet shall be dropeed
   */
  bool HandleDataFrame (uint16_t seqno, Mac48Address source, const FlameHeader flameHdr, Mac48Address receiver, uint32_t fromIface);
  static const uint16_t FLAME_PORT = 0x4040;
  /**
   * \name Information about MeshPointDeviceaddress , plugins
   * \{
   */
  typedef std::map<uint32_t, Ptr<FlameProtocolMac> > FlamePluginMap;
  FlamePluginMap m_interfaces;
  Mac48Address m_address;
  ///\}
  /**
   * \name Broadcast timers:
   * \{
   */
  Time m_broadcastInterval;
  Time m_lastBroadcast;
  ///\}
  /// Max Cost value (or TTL, because cost is actually hopcount)
  uint8_t m_maxCost;
  /// Sequence number:
  uint16_t m_myLastSeqno;
  /// Routng table:
  Ptr<FlameRtable> m_rtable;
};
} //namespace flame
} //namespace ns3
#endif /* FLAME_PROTOCOL_H */
