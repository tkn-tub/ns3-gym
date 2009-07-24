/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Based on 
 *      NS-2 AODV model developed by the CMU/MONARCH group and optimized and
 *      tuned by Samir Das and Mahesh Marina, University of Cincinnati;
 * 
 *      AODV-UU implementation by Erik Nordström of Uppsala University
 *      http://core.it.uu.se/core/index.php/AODV-UU
 *
 * Authors: Elena Borovkova <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */
#ifndef AODVROUTINGPROTOCOL_H_
#define AODVROUTINGPROTOCOL_H_

#include "aodv-rtable.h"
#include "aodv-rqueue.h"
#include "aodv-packet.h"

#include "src/internet-stack/ipv4-l3-protocol.h"

#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/timer.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-routing-protocol.h"
#include <map>

namespace ns3
{
namespace aodv
{

/**
 * \ingroup aodv
 * \brief AODV routing protocol
 */
class RoutingProtocol : public Ipv4RoutingProtocol
{
public:
  static TypeId GetTypeId (void);

  RoutingProtocol();
  virtual ~RoutingProtocol();
  virtual void DoDispose ();
  
  ///\name From Ipv4RoutingProtocol
  //\{
  Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, const Ipv4Header &header, uint32_t oif, Socket::SocketErrno &sockerr);
  bool RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                           UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                           LocalDeliverCallback lcb, ErrorCallback ecb);  
  virtual void NotifyInterfaceUp (uint32_t interface);
  virtual void NotifyInterfaceDown (uint32_t interface);
  virtual void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);
  virtual void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);
  virtual void SetIpv4 (Ptr<Ipv4> ipv4);
  //\}
  
private:

  ///\name Protocol parameters. TODO document
  //\{
 // Time AODV_RTQ_TIMEOUT;
  Time RTQ_TIMEOUT;
  uint32_t RREQ_RETRIES;        // 2
  Time ACTIVE_ROUTE_TIMEOUT;    // 3 seconds
  Time MY_ROUTE_TIMEOUT;       // 2 * ACTIVE_ROUTE_TIMEOUT
  uint16_t NET_DIAMETER;
  Time NODE_TRAVERSAL_TIME;         //  40 milliseconds
  Time BCAST_ID_SAVE;
  Time HELLO_INTERVAL;
  uint32_t ALLOWED_HELLO_LOSS;
  Time DELETE_PERIOD;
  Time MaxHelloInterval; //        (1.25 * HELLO_INTERVAL)
  Time MinHelloInterval; //        (0.75 * HELLO_INTERVAL)
  Time FREQUENCY;
  Time NET_TRAVERSAL_TIME;          // 2 * NODE_TRAVERSAL_TIME * NET_DIAMETER
  //\}

  /// \name Handle Broadcast sequence number cache
  //\{
  void InsertBroadcastId (Ipv4Address id, uint32_t bid);
  bool LookupBroadcastId (Ipv4Address id, uint32_t bid);
  void PurgeBroadcastId ();
  struct BroadcastId
  {
    Ipv4Address src;
    uint32_t    id;
    Time        expire;
  };
  struct IsExpired
  {
    bool operator()(const struct BroadcastId & b) const
    {
      return (b.expire < Simulator::Now());
    }
  };
  std::vector<BroadcastId> m_broadcastIdCache;
  //\}

  ///\name Handle duplicated packets
  //\{
  void InsertPacketUid (Ipv4Address src, uint32_t uid);
  bool LookupPacketUid (Ipv4Address src, uint32_t uid);
  void PurgePacketUid ();
  struct PacketUid
   {
     Ipv4Address m_src;
     uint32_t    m_uid;
     Time        m_expire;
   };
  struct IsExpiredForPacket
  {
    bool operator()(const struct PacketUid & p) const
    {
      return (p.m_expire < Simulator::Now());
    }
  };
  std::vector<PacketUid> m_packetUidCache;
  //\}

  /**\name Handle neighbors
   *  - from which node has received a Hello message, or
   *  - which are active next hops, or
   *  - which are active precursors
   */
  //\{
  struct Neighbor
  {
    Ipv4Address m_neighborAddress;
    Time m_expireTime;
  };
  struct IsExpiredForNeighbor
  {
     bool operator()(const struct Neighbor & nb) const
     {
       return (nb.m_expireTime < Simulator::Now());
     }
   };
  bool LookupNeighbor (Ipv4Address addr, Neighbor & n);
  bool IsNeighbor (Ipv4Address addr);
  void UpdateNeighbor (Ipv4Address addr, Time expire);
  void PurgeNeighbor ();
  std::vector<Neighbor> m_nb;
  //\}

  /// IP protocol
  Ptr<Ipv4> m_ipv4;
  /// Raw socket per each IP interface, map socket -> iface address (IP + mask)
  std::map< Ptr<Socket>, Ipv4InterfaceAddress > m_socketAddresses;

  /// Routing table
  RoutingTable m_routingTable;
  /// Routing entry for desired destination
  std::vector<RoutingTableEntry>  m_desired;
  /// A "drop-front" queue used by the routing layer to buffer packets to which it does not have a route.
  AodvQueue m_queue;
  /// Broadcast ID
  uint32_t m_broadcastID;
  /// Request sequence number
  uint32_t m_seqNo;

  UnicastForwardCallback m_scb;
  ErrorCallback m_ecb;

private:
  /// Start protocol operation
  void Start ();
  /// Start local route repair procedure
  void LocalRouteRepair (Ipv4Address dst, Ptr<Packet> p);
  /// Process broken link
  void HandleLinkFailure (Ipv4Address id);
  /// Purge all expired records from m_routingTable
  void RtPurge ();
  /**
   * Update route lifetime.
   * \param addr - destination address
   * \return true if route to destination address addr exist
   */
  bool UpdateRouteLifeTime(Ipv4Address addr, Time lifetime);
  /// Update neighbor record. \param receiver is supposed to be my interface
  void UpdateRouteToNeighbor (Ipv4Address sender, Ipv4Address receiver);
  /// Check that packet is an AODV control message
  bool LooksLikeAodvControl (Ptr<const Packet> p, Ipv4Header const & header) const;
  /// Check that packet is send from own interface
  bool IsMyOwnPacket(Ipv4Address src);
  /// Find socket with local interface address iface
  Ptr<Socket> FindSocketWithInterfaceAddress(Ipv4Address iface) const;
  /// Process hello message
  void ProcessHello(RrepHeader const & rrepHeader, Ipv4Address receiverIfaceAddr);
  
  ///\name Recv
  //\{
  /// Receive and process control packet
  void RecvAodv (Ptr<Socket> socket);
  /// Receive RREQ
  void RecvRequest (Ptr<Packet> p, Ipv4Address receiver, Ipv4Address src, Ptr<Socket> socket);
  /// Receive RREP
  void RecvReply (Ptr<Packet> p, Ipv4Address my ,Ipv4Address src);
  /// Receive RERR from node with address src
  void RecvError (Ptr<Packet> p, Ipv4Address src);
  //\}
  
  ///\name Send
  //\{
  void SendPacketFromQueue(Ipv4Address dst, Ptr<Ipv4Route> route);
  void Send(Ptr<Ipv4Route>, Ptr<const Packet>, const Ipv4Header &);
  /// Send hello. TODO send independent hello per interface
  void SendHello ();
  /// Send RREQ
  void SendRequest (Ipv4Address dst,  bool D,bool G = true);
  /// Send RREP
  void SendReply (RreqHeader const & rreqHeader, RoutingTableEntry const & toOrigin, Ptr<Socket> socket);
  /** Send RREP by intermediate node
   * \param toDst routing table entry to destination
   * \param toOrigin routing table entry to originator
   * \param gratRep indicates whether a gratuitous RREP should be unicast to destination
   */
  void SendReplyByIntermediateNode (RoutingTableEntry & toDst, RoutingTableEntry & toOrigin, bool gratRep,  Ptr<Socket> socket);
  ///\name Send RERR
  //\{
  void SendRerrWhenBreaksLinkToNextHop (Ipv4Address nextHop);
  void SendRerrMessage(Ptr<Packet> packet,  std::vector<Ipv4Address> precursors);
  //\}
  //\}
  
  void Drop(Ptr<const Packet>, const Ipv4Header &, Socket::SocketErrno) {}

  ///\name Timers. TODO comment each one
  //\{
  Timer  btimer;
  void BroadcastTimerExpire ();
  Timer htimer; // TODO independent hello timers for all interfaces
  void HelloTimerExpire ();
  Timer ntimer;
  void NeighborTimerExpire ();
  Timer rtimer;
  void RouteCacheTimerExpire ();
  Timer lrtimer;
  void LocalRepairTimerExpire ();
  Timer m_routeRequestTimer;
  void RouteRequestTimerExpire(Ipv4Address dst);
  //\}
};

}
}
#endif /* AODVROUTINGPROTOCOL_H_ */
