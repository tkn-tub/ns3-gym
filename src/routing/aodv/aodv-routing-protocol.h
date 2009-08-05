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
#include "id-cache.h"
#include "aodv-neighbor.h"

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
  
  ///\name Handle protocol parameters
  //\{
  bool GetDesinationOnlyFlag () const { return DestinationOnly; }
  void SetDesinationOnlyFlag (bool f) { DestinationOnly = f; }
  bool GetGratuitousReplyFlag () const { return GratuitousReply; }
  void SetGratuitousReplyFlag (bool f) { GratuitousReply = f; }
  //\}
private:
  ///\name Protocol parameters. TODO document
  //\{
  uint32_t RreqRetries;        ///< Maximum number of retransmissions of RREQ with TTL = NetDiameter to discover a route
  Time ActiveRouteTimeout;    // 3 seconds
  Time MyRouteTimeout;       // 2 * ActiveRouteTimeout
  uint32_t NetDiameter;
  Time NodeTraversalTime;         //  40 milliseconds
  Time NetTraversalTime;          // 2 * NodeTraversalTime * NetDiameter
  Time PathDiscoveryTime;
  Time HelloInterval;
  uint32_t AllowedHelloLoss;
  /**
   * DeletePeriod is intended to provide an upper bound on the time for which an upstream node A
   * can have a neighbor B as an active next hop for destination D, while B has invalidated the route to D.
   */
  Time DeletePeriod;
  Time MaxHelloInterval; //        (1.25 * HelloInterval)
  Time MinHelloInterval; //        (0.75 * HelloInterval)
  Time FREQUENCY;
  Time NextHopWait;
  uint16_t TtlStart;
  uint16_t TtlIncrement;
  uint16_t TtlThreshold;
  uint16_t  MaxRepairTtl;           // 0.3 * NetDiameter
  uint16_t LOCAL_ADD_TTL;
  uint16_t TIMEOUT_BUFFER;
  Time BLACKLIST_TIMEOUT;
  uint32_t MaxQueueLen;
  Time MaxQueueTime;
  bool DestinationOnly;
  bool GratuitousReply;
  //\}

  /// IP protocol
  Ptr<Ipv4> m_ipv4;
  /// Raw socket per each IP interface, map socket -> iface address (IP + mask)
  std::map< Ptr<Socket>, Ipv4InterfaceAddress > m_socketAddresses;

  /// Routing table
  RoutingTable m_routingTable;
  /// A "drop-front" queue used by the routing layer to buffer packets to which it does not have a route.
  RequestQueue m_queue;
  /// Broadcast ID
  uint32_t m_requestId;
  /// Request sequence number
  uint32_t m_seqNo;
  /// Handle duplicated packets
  IdCache m_idCache;
  /// Handle neighbors
  Neighbors m_nb;

  UnicastForwardCallback m_scb;
  ErrorCallback m_ecb;

private:
  /// Start protocol operation
  void Start ();
  /// Start local route repair procedure
  void LocalRouteRepair (Ipv4Address dst, Ipv4Address origin);
  /// Process broken link
  void HandleLinkFailure (Ipv4Address id);
  /**
  * To reduce congestion in a network, repeated attempts by a source node at route discovery
  * for a single destination MUST utilize a binary exponential backoff.
  */
  void ScheduleRreqRetry (Ipv4Address dst,  uint16_t ttl);
  /**
   * Update route lifetime.
   * \param addr - destination address
   * \return true if route to destination address addr exist
   */
  bool UpdateRouteLifeTime(Ipv4Address addr, Time lifetime);
  /// Update neighbor record. \param receiver is supposed to be my interface
  void UpdateRouteToNeighbor (Ipv4Address sender, Ipv4Address receiver);
  /// Check that packet is send from own interface
  bool IsMyOwnPacket(Ipv4Address src);
  /// Find socket with local interface address iface
  Ptr<Socket> FindSocketWithInterfaceAddress(Ipv4Address iface) const;
  /**
  * Add UDP and IP header to packet.
  * \param sport - source port number
  * \param dport - destination port number
  * \param src - source IP address
  * \param dst - destination IP address
  * \param identification - used as sequence number for broadcast traffic
  */
  void BuildPacket(Ptr<Packet> packet, uint16_t sport, uint16_t dport, Ipv4Address src, Ipv4Address dst, uint32_t identification, uint16_t ttl);
  /// Process hello message
  void ProcessHello(RrepHeader const & rrepHeader, Ipv4Address receiverIfaceAddr);
  
  ///\name Receive control packets
  //\{
  /// Receive and process control packet
  void RecvAodv (Ptr<Socket> socket);
  /// Receive RREQ
  void RecvRequest (Ptr<Packet> p, Ipv4Address receiver, Ipv4Address src, Ipv4Header ipv4Header);
  /// Receive RREP
  void RecvReply (Ptr<Packet> p, Ipv4Address my ,Ipv4Address src, Ipv4Header ipv4Header);
  /// Receive RREP_ACK
  void RecvReplyAck(Ipv4Address neighbor);
  /// Receive RERR from node with address src
  void RecvError (Ptr<Packet> p, Ipv4Address src);
  //\}
  
  ///\name Send
  //\{
  /// Forward packet from route request queue
  void SendPacketFromQueue(Ipv4Address dst, Ptr<Ipv4Route> route);
  /// Aux. send helper
  void Send(Ptr<Ipv4Route>, Ptr<const Packet>, const Ipv4Header &);
  /// Send hello
  void SendHello ();
  /// Send RREQ
  void SendRequest (Ipv4Address dst, uint16_t ttl);
  /// Send RREP
  void SendReply (RreqHeader const & rreqHeader, RoutingTableEntry const & toOrigin);
  /** Send RREP by intermediate node
   * \param toDst routing table entry to destination
   * \param toOrigin routing table entry to originator
   * \param gratRep indicates whether a gratuitous RREP should be unicast to destination
   */
  void SendReplyByIntermediateNode (RoutingTableEntry & toDst, RoutingTableEntry & toOrigin, bool gratRep);
  /// Send RREP_ACK
  void SendReplyAck (Ipv4Address neighbor);
  /// Initiate RERR
  void SendRerrWhenBreaksLinkToNextHop (Ipv4Address nextHop);
  /// Forward RERR
  void SendRerrMessage(Ptr<Packet> packet,  std::vector<Ipv4Address> precursors);
  //\}
  
  /// Notify that packet is dropped for some reason 
  void Drop(Ptr<const Packet>, const Ipv4Header &, Socket::SocketErrno) {}
  void DropFromQueue (Ipv4Address dst);

  ///\name Timers. TODO comment each one
  //\{
  Timer htimer; // TODO independent hello timers for all interfaces
  void HelloTimerExpire ();
  Timer rtimer;
  void RouteCacheTimerExpire ();
  Timer lrtimer;
  void LocalRepairTimerExpire ();
  std::map<Ipv4Address, Timer> m_addressReqTimer;
  void RouteRequestTimerExpire(Ipv4Address dst, uint16_t lastTtl);
  void AckTimerExpire (Ipv4Address neighbor,  Time blacklistTimeout);
  void RoutingTableEntryTimerExpire(Ipv4Address dst);
  //\}
};

}
}
#endif /* AODVROUTINGPROTOCOL_H_ */
