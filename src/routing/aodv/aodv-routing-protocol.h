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
  Time ACTIVE_ROUTE_TIMEOUT;    // 3 seconds
  Time MY_ROUTE_TIMEOUT;       // 2 * ACTIVE_ROUTE_TIMEOUT
  uint16_t NET_DIAMETER;
  Time NODE_TRAVERSAL_TIME;         //  40 milliseconds
  Time BCAST_ID_SAVE;
  Time HELLO_INTERVAL;
  uint32_t ALLOWED_HELLO_LOSS;
  Time BAD_LINK_LIFETIME;
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

  /// IP protocol
  Ptr<Ipv4> m_ipv4;
  /// Raw socket per each IP interface, map socket -> iface address (IP + mask)
  std::map< Ptr<Socket>, Ipv4InterfaceAddress > m_socketAddresses;

  /// Routing table
  RoutingTable m_routingTable;
  /// A "drop-front" queue used by the routing layer to buffer packets to which it does not have a route.
  AodvQueue m_queue;
  /// Broadcast ID
  uint32_t m_broadcastID;
  /// Request sequence number
  uint32_t m_seqNo;

private:
  /// Start protocol operation
  void Start ();
  /// Start local route repair procedure
  void LocalRouteRepair (Ipv4Address dst, Ptr<Packet> p);
  /// Process broken link
  void HandleLinkFailure (Ipv4Address id);
  /// Purge all expired records from m_routingTable
  void RtPurge ();
  /// Update neighbor record. \param receiver is supposed to be my interface
  void UpdateNeighbor (Ipv4Address sender, Ipv4Address receiver);
  /// Check that packet is an AODV control message
  bool LooksLikeAodvControl (Ptr<const Packet> p, Ipv4Header const & header) const;
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
  /// Receive RERR
  void RecvError (Ptr<Packet> p);
  //\}
  
  ///\name Send
  //\{
  /// Send hello. TODO send independent hello per interface
  void SendHello ();
  /// Send RREQ
  void SendRequest (Ipv4Address dst, bool G, bool D);
  /// Send RREP
  void SendReply (RreqHeader const & rreqHeader, RoutingTableEntry const & toOrigin, Ptr<Socket> socket);
  /** Send RREP by intermediate node
   * \param toDst routing table entry to destination
   * \param toOrigin routing table entry to originator
   * \param gratRep indicates whether a gratuitous RREP should be unicast to destination
   */
  void SendReplyByIntermediateNode (RoutingTableEntry & toDst, RoutingTableEntry & toOrigin, bool gratRep,  Ptr<Socket> socket);
  /// Send RERR
  void SendError (Ipv4Address failed);
  //\}
  
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
  //\}
};

}
}
#endif /* AODVROUTINGPROTOCOL_H_ */
