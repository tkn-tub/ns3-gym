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
#include "aodv-routing-protocol.h"
#include "ns3/socket.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/random-variable.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-route.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#include "ns3/nstime.h"
#include "ns3/net-device.h"

/// UDP Port for AODV control traffic
#define AODV_PORT  654

NS_LOG_COMPONENT_DEFINE ("AodvRoutingProtocol");

namespace ns3
{
namespace aodv
{
NS_OBJECT_ENSURE_REGISTERED (RoutingProtocol);

void
RoutingProtocol::InsertBroadcastId (Ipv4Address id, uint32_t bid)
{
  NS_LOG_FUNCTION(this);
  if (LookupBroadcastId (id, bid))
    return;
  struct BroadcastId broadcastId = { id, bid, BCAST_ID_SAVE + Simulator::Now () };
  m_broadcastIdCache.push_back (broadcastId);
}
bool
RoutingProtocol::LookupBroadcastId (Ipv4Address id, uint32_t bid)
{
  PurgeBroadcastId ();
  for (std::vector<BroadcastId>::const_iterator i = m_broadcastIdCache.begin (); i != m_broadcastIdCache.end (); ++i)
    if (i->src == id && i->id == bid)
      return true;
  return false;
}
void
RoutingProtocol::PurgeBroadcastId ()
{
  NS_LOG_FUNCTION(this);
  std::vector<BroadcastId>::iterator i = remove_if (m_broadcastIdCache.begin (), m_broadcastIdCache.end (), IsExpired ());
  m_broadcastIdCache.erase (i, m_broadcastIdCache.end ());
}

void
RoutingProtocol::InsertPacketUid(Ipv4Address src, uint32_t uid)
{
  NS_LOG_FUNCTION(this);
   if (LookupPacketUid (src, uid))
     return;
   struct PacketUid packetUid = { src, uid, BCAST_ID_SAVE + Simulator::Now () };
   m_packetUidCache.push_back (packetUid);
}
bool
RoutingProtocol::LookupPacketUid(Ipv4Address src, uint32_t uid)
{
  PurgePacketUid ();
  for (std::vector<PacketUid>::const_iterator i = m_packetUidCache.begin (); i != m_packetUidCache.end (); ++i)
    if (i->m_src == src && i->m_uid == uid)
    {
      NS_LOG_LOGIC("duplicated packet from " << src << " uid " << uid);
      return true;
    }
  return false;
}
void
RoutingProtocol::PurgePacketUid ()
{
  NS_LOG_FUNCTION(this);
  std::vector<PacketUid>::iterator i = remove_if (m_packetUidCache.begin (), m_packetUidCache.end (), IsExpiredForPacket ());
  m_packetUidCache.erase (i, m_packetUidCache.end ());
}

bool
RoutingProtocol::LookupNeighbor (Ipv4Address addr, Neighbor & n)
{
  PurgeNeighbor ();
   for (std::vector<Neighbor>::const_iterator i = m_nb.begin (); i != m_nb.end (); ++i)
     if (i->m_neighborAddress == addr)
     {
       n = *i;
       return true;
     }
   return false;
}
bool
RoutingProtocol::IsNeighbor(Ipv4Address addr)
{
  for (std::vector<Neighbor>::const_iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    if (i->m_neighborAddress == addr) return true;
  return false;
}

void
RoutingProtocol::UpdateNeighbor(Ipv4Address addr, Time expire)
{
  NS_LOG_FUNCTION(this);
  for (std::vector<Neighbor>::iterator i = m_nb.begin (); i != m_nb.end (); ++i)
     if (i->m_neighborAddress == addr)
     {
       i->m_expireTime = expire + Simulator::Now();
       return;
     }
  struct Neighbor neighbor = { addr, expire + Simulator::Now () };
  m_nb.push_back (neighbor);
  PurgeNeighbor ();
}
void
RoutingProtocol::PurgeNeighbor ()
{
  NS_LOG_FUNCTION(this);
  for(std::vector<Neighbor>::const_iterator i = m_nb.begin(); i != m_nb.end(); ++i)
    if(i->m_expireTime < Simulator::Now()) HandleLinkFailure(i->m_neighborAddress);
  std::vector<Neighbor>::iterator i = remove_if (m_nb.begin (), m_nb.end (), IsExpiredForNeighbor ());
  m_nb.erase (i, m_nb.end ());

}

RoutingProtocol::RoutingProtocol () :
  RTQ_TIMEOUT (Seconds (30)),
  RREQ_RETRIES (2),
  ACTIVE_ROUTE_TIMEOUT (Seconds (3)),
  MY_ROUTE_TIMEOUT (Scalar (2) * ACTIVE_ROUTE_TIMEOUT),
  NET_DIAMETER (35),
  NODE_TRAVERSAL_TIME (MilliSeconds (40)),
  ALLOWED_HELLO_LOSS (2),
  FREQUENCY (Seconds (0.5)),
  BLACKLIST_TIMEOUT(Scalar(RREQ_RETRIES) * NET_TRAVERSAL_TIME),
    /* BLACKLIST_TIMEOUT should be suitably increased if an expanding ring search is used.  In such cases, it should be
     *    {[(TTL_THRESHOLD - TTL_START)/TTL_INCREMENT] + 1 + RREQ_RETRIES} *NET_TRAVERSAL_TIME.
     * This is to account for possible additional route discovery attempts.
     */
  NEXT_HOP_WAIT(NODE_TRAVERSAL_TIME + MilliSeconds (10)),
  m_broadcastID (0), m_seqNo (0), btimer (Timer::CANCEL_ON_DESTROY), htimer (Timer::CANCEL_ON_DESTROY), ntimer (Timer::CANCEL_ON_DESTROY),
  rtimer (Timer::CANCEL_ON_DESTROY), lrtimer (Timer::CANCEL_ON_DESTROY), m_routeRequestTimer (Timer::CANCEL_ON_DESTROY)


{
  NET_TRAVERSAL_TIME = Scalar (2 * NET_DIAMETER) * NODE_TRAVERSAL_TIME;
  BCAST_ID_SAVE = Scalar (2) * NET_TRAVERSAL_TIME;
  MaxHelloInterval = Scalar (1.25) * HELLO_INTERVAL;
  MinHelloInterval = Scalar (0.75) * HELLO_INTERVAL;
  if(ACTIVE_ROUTE_TIMEOUT > HELLO_INTERVAL) DELETE_PERIOD = Scalar(5) * ACTIVE_ROUTE_TIMEOUT;
  else DELETE_PERIOD = Scalar(5) * HELLO_INTERVAL;
}

TypeId
RoutingProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::aodv::RoutingProtocol") .SetParent<Ipv4RoutingProtocol> () .AddConstructor<RoutingProtocol> () .AddAttribute (
      "HelloInterval", "HELLO messages emission interval.", TimeValue (Seconds (1)), MakeTimeAccessor (&RoutingProtocol::HELLO_INTERVAL),
      MakeTimeChecker ()) .AddAttribute ("Broadcast id save", "Broadcast id save interval.", TimeValue (Seconds (6)), MakeTimeAccessor (
      &RoutingProtocol::BCAST_ID_SAVE), MakeTimeChecker ()) .AddAttribute ("RreqRetries",
      "Maximum number of retransmissions of RREQ to discover a route", UintegerValue (2), MakeUintegerAccessor (&RoutingProtocol::RREQ_RETRIES),
      MakeUintegerChecker<uint32_t> ()) .AddAttribute ("NodeTraversalTime",
      "Conservative estimate of the average one hop traversal time for packets and should include"
        "queuing delays, interrupt processing times and transfer times.", TimeValue (MilliSeconds (40)), MakeTimeAccessor (
          &RoutingProtocol::NODE_TRAVERSAL_TIME), MakeTimeChecker ()) .AddAttribute ("ActiveRouteTimeout",
      "Period of time during which the route is considered to be valid", TimeValue (Seconds (3)), MakeTimeAccessor (
          &RoutingProtocol::ACTIVE_ROUTE_TIMEOUT), MakeTimeChecker ())

  ;
  return tid;
}
RoutingProtocol::~RoutingProtocol ()
{
}

void
RoutingProtocol::DoDispose ()
{
  m_ipv4 = 0;
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::iterator iter = m_socketAddresses.begin (); iter != m_socketAddresses.end (); iter++)
  {
    iter->first->Close ();
  }
  m_socketAddresses.clear ();
  Ipv4RoutingProtocol::DoDispose ();
}

void
RoutingProtocol::Start ()
{
  NS_LOG_FUNCTION (this);
  // Open UDP sockets for control traffic on each IP interface
  const Ipv4Address loopback ("127.0.0.1");
  for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i++)
  {
    Ipv4InterfaceAddress iface = m_ipv4->GetAddress (i, 0);
    if (iface.GetLocal () == loopback)
      continue;

    // Create a socket to listen only on this interface
    Ptr<Socket> socket = Socket::CreateSocket (GetObject<Node> (), TypeId::LookupByName ("ns3::UdpSocketFactory"));
    NS_ASSERT (socket != 0);
    int status = socket->Bind (InetSocketAddress (iface.GetLocal (), AODV_PORT));
    NS_ASSERT (status != -1);
    socket->SetRecvCallback (MakeCallback (&RoutingProtocol::RecvAodv, this));
    status = socket->Connect (InetSocketAddress (iface.GetBroadcast (), AODV_PORT));
    NS_ASSERT (status != -1);
    m_socketAddresses.insert (std::make_pair (socket, iface));
    NS_LOG_INFO ("Interface " << iface << " used by AODV");

    // Add local broadcast record to the routing table
    Ptr<NetDevice> dev = m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (iface.GetLocal ()));
    RoutingTableEntry rt (/*device=*/dev, /*dst=*/iface.GetBroadcast (), /*know seqno=*/true, /*seqno=*/0, /*iface=*/iface.GetLocal (),
                          /*hops=*/1, /*next hop=*/iface.GetBroadcast (), /*lifetime=*/Seconds (1e9)); // TODO use infty
    m_routingTable.AddRoute (rt);
  }

  m_scb = MakeCallback (&RoutingProtocol::Send, this);
  m_ecb = MakeCallback (&RoutingProtocol::Drop, this);

  btimer.Schedule();
  ntimer.Schedule();
  rtimer.Schedule();
}

Ptr<Ipv4Route>
RoutingProtocol::RouteOutput (Ptr<Packet> p, const Ipv4Header &header, uint32_t oif, Socket::SocketErrno &sockerr)
{
  NS_LOG_FUNCTION (this << p->GetUid() << header.GetDestination());
  Ptr<Ipv4Route> route;
  Ipv4Address dst = header.GetDestination ();
  RoutingTableEntry rt;
  bool result = m_routingTable.LookupRoute (dst, rt);
  if (result && (rt.GetFlag () == RTF_UP))
  {
    m_routingTable.Print (std::cout);
    route = rt.GetRoute ();
    NS_ASSERT (route != 0);
    sockerr = Socket::ERROR_NOTERROR;
    NS_LOG_LOGIC("exist route to " << route->GetDestination() << " from iface " << route->GetSource());
    UpdateRouteLifeTime(dst, ACTIVE_ROUTE_TIMEOUT);
    return route;
  }
  else
  {
    QueueEntry newEntry (p, header, m_scb, m_ecb);
    m_queue.Enqueue (newEntry);
    sockerr = Socket::ERROR_NOROUTETOHOST;
    if(rt.GetFlag() != RTF_IN_SEARCH) SendRequest (dst, false, true);
    return route;
  }

}

bool
RoutingProtocol::RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev, UnicastForwardCallback ucb,
    MulticastForwardCallback mcb, LocalDeliverCallback lcb, ErrorCallback ecb)
{
  NS_LOG_FUNCTION (this << p->GetUid() << header.GetDestination() << idev->GetAddress());

  NS_ASSERT (m_ipv4 != 0);
  // Check if input device supports IP
  NS_ASSERT (m_ipv4->GetInterfaceForDevice (idev) >= 0);
  int32_t iif = m_ipv4->GetInterfaceForDevice (idev);

  Ipv4Address dst = header.GetDestination ();
  Ipv4Address origin = header.GetSource ();

  if (IsMyOwnPacket (origin)) return true;

  // Local delivery to AODV interfaces
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
  {
    Ipv4InterfaceAddress iface = j->second;
    if (m_ipv4->GetInterfaceForAddress (iface.GetLocal ()) == iif)
      if (dst == iface.GetBroadcast ())
      {
        if(LookupPacketUid(origin, p->GetUid())) return true;
        InsertPacketUid(origin, p->GetUid());
        NS_LOG_LOGIC ("Broadcast local delivery to " << iface.GetLocal ());
        lcb (p, header, iif);
        // TODO has TTL, forward
        return true;
      }
  }
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
  {
    Ipv4InterfaceAddress iface = j->second;
    if (dst == iface.GetLocal ())
    {
      NS_LOG_LOGIC ("Unicast local delivery to " << iface.GetLocal ());
      lcb (p, header, iif);
      return true;
    }
  }

  // TODO: local delivery to non-AODV interfaces

  // Forwarding
  RoutingTableEntry toDst;
  if (m_routingTable.LookupRoute (dst, toDst))
  {
    Ptr<Ipv4Route> route = toDst.GetRoute ();
    NS_LOG_LOGIC(route->GetSource()<<" forwarding to " << dst << " from" << origin);

    /**
    *  Each time a route is used to forward a data packet, its Active Route
    *  Lifetime field of the source, destination and the next hop on the
    *  path to the destination is updated to be no less than the current
    *  time plus ACTIVE_ROUTE_TIMEOUT.
    */
    UpdateRouteLifeTime(origin, ACTIVE_ROUTE_TIMEOUT);
    UpdateRouteLifeTime(dst, ACTIVE_ROUTE_TIMEOUT);
    UpdateRouteLifeTime(route->GetGateway(), ACTIVE_ROUTE_TIMEOUT);
    /**
    *  Since the route between each originator and destination pair is expected to be symmetric, the
    *  Active Route Lifetime for the previous hop, along the reverse path back to the IP source, is also updated
    *  to be no less than the current time plus ACTIVE_ROUTE_TIMEOUT
    */
    RoutingTableEntry toOrigin;
    m_routingTable.LookupRoute(origin, toOrigin);
    UpdateRouteLifeTime(toOrigin.GetNextHop(), ACTIVE_ROUTE_TIMEOUT);

    UpdateNeighbor(route->GetGateway(), ACTIVE_ROUTE_TIMEOUT); //?
    UpdateNeighbor(toOrigin.GetNextHop(), ACTIVE_ROUTE_TIMEOUT);

    ucb (route, p, header);
    return true;
  }


  QueueEntry newEntry (p, header, ucb, ecb, RTQ_TIMEOUT);
  m_queue.Enqueue (newEntry);
  NS_LOG_LOGIC("route not found to "<< dst);
  m_routingTable.Print (std::cout);
  return false;
}

void
RoutingProtocol::SetIpv4 (Ptr<Ipv4> ipv4)
{
  NS_LOG_FUNCTION (this);NS_ASSERT (ipv4 != 0);NS_ASSERT (m_ipv4 == 0);

  btimer.SetFunction (&RoutingProtocol::BroadcastTimerExpire, this);
  ntimer.SetFunction (&RoutingProtocol::NeighborTimerExpire, this);
  rtimer.SetFunction (&RoutingProtocol::RouteCacheTimerExpire, this);
  lrtimer.SetFunction (&RoutingProtocol::LocalRepairTimerExpire, this);
  htimer.SetFunction (&RoutingProtocol::HelloTimerExpire, this);
  m_routeRequestTimer.SetFunction(&RoutingProtocol::RouteRequestTimerExpire, this);


  htimer.SetDelay (HELLO_INTERVAL);
  btimer.SetDelay (BCAST_ID_SAVE);
  rtimer.SetDelay (FREQUENCY);

  m_ipv4 = ipv4;
  Simulator::ScheduleNow (&RoutingProtocol::Start, this);
}

void
RoutingProtocol::NotifyInterfaceUp (uint32_t i)
{
  NS_LOG_FUNCTION (this << m_ipv4->GetAddress (i, 0).GetLocal ());
  // TODO
}

void
RoutingProtocol::NotifyInterfaceDown (uint32_t i)
{
  NS_LOG_FUNCTION (this << m_ipv4->GetAddress (i, 0).GetLocal ());
  // TODO
}

void
RoutingProtocol::NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
}

void
RoutingProtocol::NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
}

bool
RoutingProtocol::IsMyOwnPacket (Ipv4Address src)
{
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
  {
    Ipv4InterfaceAddress iface = j->second;
    if (src == iface.GetLocal ())
    {
      NS_LOG_LOGIC(iface.GetLocal() << " receive own packet");
      return true;
    }
  }
  return false;
}

// TODO add use an expanding ring search technique
void
RoutingProtocol::SendRequest (Ipv4Address dst, bool D, bool G)
{
  NS_LOG_FUNCTION (this << dst);

  // Create RREQ header
  TypeHeader tHeader (AODVTYPE_RREQ);
  RreqHeader rreqHeader;
  rreqHeader.SetDst (dst);

  RoutingTableEntry rt;
  if (m_routingTable.LookupRoute (dst, rt))
  {
    // if we already send maximum number of RREQ
    if (rt.GetRreqCnt () == RREQ_RETRIES)
    {
      NS_LOG_LOGIC ("Route does not found to " << dst);
      return;
    }
    else
    {
      rt.IncrementRreqCnt();
      m_routingTable.Update(dst, rt);
    }

    rreqHeader.SetHopCount (rt.GetHop ());
    if(rt.GetValidSeqNo()) rreqHeader.SetDstSeqno (rt.GetSeqNo ());
    else rreqHeader.SetUnknownSeqno (true);
  }
  else
  {
    Ptr<NetDevice> dev;
    rreqHeader.SetUnknownSeqno (true);
    RoutingTableEntry newEntry (dev, dst, false, 0, Ipv4Address (), 0, Ipv4Address (), Simulator::Now ());
    newEntry.IncrementRreqCnt ();
    newEntry.SetFlag (RTF_IN_SEARCH);
    m_routingTable.AddRoute (newEntry);
  }

  if (G)
    rreqHeader.SetGratiousRrep (true);
  if (D)
    rreqHeader.SetDestinationOnly (true);

  m_seqNo++;
  rreqHeader.SetOriginSeqno (m_seqNo);
  m_broadcastID++;
  rreqHeader.SetId (m_broadcastID);
  rreqHeader.SetHopCount (0);

  // Send RREQ as subnet directed broadcast from each (own) interface
  Ptr<Packet> packet = Create<Packet> ();
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
  {
    Ptr<Socket> socket = j->first;
    Ipv4InterfaceAddress iface = j->second;

    rreqHeader.SetOrigin (iface.GetLocal ());
    InsertBroadcastId (iface.GetLocal (), m_broadcastID);

    packet->AddHeader (rreqHeader);
    packet->AddHeader (tHeader);
    socket->Send (packet);
  }

  /**
  * To reduce congestion in a network, repeated attempts by a source node at route discovery
  * for a single destination MUST utilize a binary exponential backoff.
  */
  m_routeRequestTimer.SetArguments(dst);
  m_routeRequestTimer.Cancel();
  m_routingTable.LookupRoute (dst, rt);
  m_routeRequestTimer.Schedule(Scalar(rt.GetRreqCnt()) * NET_TRAVERSAL_TIME);

  htimer.Cancel();
  htimer.Schedule (HELLO_INTERVAL);
}

void
RoutingProtocol::RecvAodv (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this);
  Ptr<Packet> packet;
  Address sourceAddress;
  packet = socket->RecvFrom (sourceAddress);

  InetSocketAddress inetSourceAddr = InetSocketAddress::ConvertFrom (sourceAddress);
  NS_ASSERT (inetSourceAddr.GetPort () == AODV_PORT);
  Ipv4Address sender = inetSourceAddr.GetIpv4 ();
  Ipv4Address receiver = m_socketAddresses[socket].GetLocal ();

  NS_LOG_DEBUG ("AODV node " << this << " received a AODV packet from " << sender << " to " << receiver);

  UpdateRouteToNeighbor (sender, receiver);
  NS_LOG_LOGIC("xxx");
  TypeHeader tHeader (AODVTYPE_RREQ);
  packet->RemoveHeader (tHeader);
  if (!tHeader.IsValid ())
  {
    NS_LOG_WARN ("AODV message with unknown type received: " << tHeader.Get());
    return; // drop
  }

  switch (tHeader.Get ())
    {
    case AODVTYPE_RREQ:
      {
        RecvRequest (packet, receiver, sender, socket);
        break;
      }
    case AODVTYPE_RREP:
      {
        RecvReply (packet, receiver, sender);
        break;
      }
    case AODVTYPE_RERR:
      {
        RecvError (packet, sender);
        break;
      }
    case AODVTYPE_RREP_ACK:
      {
        RecvReplyAck(sender);
        break;
      }
    }
}

bool
RoutingProtocol::UpdateRouteLifeTime(Ipv4Address addr, Time lifetime)
{
  RoutingTableEntry rt;
  if(m_routingTable.LookupRoute(addr, rt))
  {
    rt.SetFlag(RTF_UP);
    rt.SetRreqCnt(0);
    if(rt.GetLifeTime() < lifetime + Simulator::Now()) rt.SetLifeTime(lifetime);
    m_routingTable.Update(addr, rt);
    return true;
  }
  return false;
}

void
RoutingProtocol::UpdateRouteToNeighbor (Ipv4Address sender, Ipv4Address receiver)
{
  NS_LOG_FUNCTION (this << "sender " << sender << " receiver " << receiver );
  RoutingTableEntry toNeighbor;
  if (!m_routingTable.LookupRoute (sender, toNeighbor))
  {
    Ptr<NetDevice> dev = m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (receiver));
    RoutingTableEntry newEntry (/*device=*/dev, /*dst=*/sender, /*know seqno=*/false, /*seqno=*/0, /*iface=*/receiver,
                                /*hops=*/1, /*next hop=*/sender,/*lifetime=*/Simulator::Now () + ACTIVE_ROUTE_TIMEOUT);
    m_routingTable.AddRoute (newEntry);
  }
  else
  {
    toNeighbor.SetFlag (RTF_UP);
    toNeighbor.SetValidSeqNo(false);
    if (toNeighbor.GetLifeTime () < Simulator::Now () + ACTIVE_ROUTE_TIMEOUT)
    {

      toNeighbor.SetLifeTime (Simulator::Now () + ACTIVE_ROUTE_TIMEOUT);
    }
    m_routingTable.Update (sender, toNeighbor);
  }
}

void
RoutingProtocol::RecvRequest (Ptr<Packet> p, Ipv4Address receiver, Ipv4Address src, Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << receiver << src);

  RreqHeader rreqHeader;
  p->RemoveHeader (rreqHeader); // TODO check that header correctly found
  uint32_t id = rreqHeader.GetId ();
  Ipv4Address origin = rreqHeader.GetOrigin ();

  /*
   *  Node checks to determine whether it has received a RREQ with the same Originator IP Address and RREQ ID.
   *  If such a RREQ has been received, the node silently discards the newly received RREQ.
   */
  if (LookupBroadcastId (origin, id))
  {
    NS_LOG_DEBUG ("My interface " << receiver <<" RREQ duplicate from " << origin << " dropped by id " << id);
    return;
  }
  InsertBroadcastId (origin, id);

  // Increment RREQ hop count
  uint8_t hop = rreqHeader.GetHopCount () + 1;
  rreqHeader.SetHopCount (hop);

  /*
   *  When the reverse route is created or updated, the following actions on the route are also carried out:
   *  1. the Originator Sequence Number from the RREQ is compared to the corresponding destination sequence number
   *     in the route table entry and copied if greater than the existing value there
   *  2. the valid sequence number field is set to true;
   *  3. the next hop in the routing table becomes the node from which the  RREQ was received
   *  4. the hop count is copied from the Hop Count in the RREQ message;
   *  5. the Lifetime is set to be the maximum of (ExistingLifetime, MinimalLifetime), where
   *     MinimalLifetime = current time + 2*NET_TRAVERSAL_TIME - 2*HopCount*NODE_TRAVERSAL_TIME
   */
  RoutingTableEntry toOrigin;
  if (!m_routingTable.LookupRoute (origin, toOrigin))
  {
    Ptr<NetDevice> dev = m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (receiver));
    RoutingTableEntry newEntry (/*device=*/dev, /*dst=*/origin, /*validSeno=*/true, /*seqNo=*/rreqHeader.GetOriginSeqno (),/*iface=*/receiver,
    /*hops=*/hop, /*nextHop*/src, /*timeLife=*/Simulator::Now () + Scalar (2) * NET_TRAVERSAL_TIME - Scalar (2 * hop) * NODE_TRAVERSAL_TIME);
    m_routingTable.AddRoute (newEntry);
  }
  else
  {
    if (int32_t (rreqHeader.GetOriginSeqno ()) - int32_t (toOrigin.GetSeqNo ()) > 0)
      toOrigin.SetSeqNo (rreqHeader.GetOriginSeqno ());
    toOrigin.SetValidSeqNo (true);
    toOrigin.SetNextHop (src);
    toOrigin.SetOutputDevice (m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (receiver)));
    toOrigin.SetHop (hop);
    Time minimalLifetime = Simulator::Now () + Scalar (2) * NET_TRAVERSAL_TIME - Scalar (2 * hop) * NODE_TRAVERSAL_TIME;

    if (toOrigin.GetLifeTime () < minimalLifetime)
    {
      toOrigin.SetLifeTime (minimalLifetime);
    }
    m_routingTable.Update (origin, toOrigin);
  }

  //  A node generates a RREP if either:
  //  (i)  it is itself the destination, or
  for (uint32_t k = 0; k < m_ipv4->GetNInterfaces (); k++)
  {
    Ipv4Address addr = m_ipv4->GetAddress (k, 0).GetLocal ();
    if (addr == rreqHeader.GetDst ())
    {
      m_routingTable.LookupRoute (origin, toOrigin);
      SendReply (rreqHeader, toOrigin, socket);
      return;
    }
  }
/*
 * (ii) it has an active route to the destination, the destination sequence number in the node's existing route table entry for the destination
 *      is valid and greater than or equal to the Destination Sequence Number of the RREQ, and the "destination only" flag is NOT set.
 */
  RoutingTableEntry toDst;
  Ipv4Address dst = rreqHeader.GetDst ();
  if (m_routingTable.LookupRoute (dst, toDst))
  {
    /*
     * The Destination Sequence number for the requested destination is set to the maximum of the corresponding value
     * received in the RREQ message, and the destination sequence value currently maintained by the node for the requested destination.
     * However, the forwarding node MUST NOT modify its maintained value for the destination sequence number, even if the value
     * received in the incoming RREQ is larger than the value currently maintained by the forwarding node.
     */
    if (rreqHeader.GetUnknownSeqno () || (int32_t (toDst.GetSeqNo ()) - int32_t (rreqHeader.GetDstSeqno ()) > 0))
    {
      if (!rreqHeader.GetDestinationOnly () && toDst.GetValidSeqNo () && (toDst.GetFlag () == RTF_UP))
      {
        SendReplyByIntermediateNode (toDst, toOrigin, rreqHeader.GetGratiousRrep (), socket);
        return;
      }
      rreqHeader.SetDstSeqno (toDst.GetSeqNo ());
      rreqHeader.SetUnknownSeqno (false);
    }
  }
  /*
   * If a node does not generate a RREP the incoming IP header has TTL larger than 1, the node updates
   * and broadcasts the RREQ on each of its configured interfaces.
   */
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (rreqHeader);
  TypeHeader tHeader (AODVTYPE_RREQ);
  packet->AddHeader (tHeader);

  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
  {
    Ptr<Socket> socket = j->first;
    Ipv4InterfaceAddress iface = j->second;
    socket->Send (packet);
  }

  htimer.Cancel();
  htimer.Schedule (HELLO_INTERVAL);
}

void
RoutingProtocol::SendReply (RreqHeader const & rreqHeader, RoutingTableEntry const & toOrigin, Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << toOrigin.GetDestination ());
  /*
   * Destination node MUST increment its own sequence number by one if the sequence number in the RREQ packet is equal to that
   * incremented value. Otherwise, the destination does not change its sequence number before generating the  RREP message.
   */
  if (!rreqHeader.GetUnknownSeqno () && (rreqHeader.GetDstSeqno () == m_seqNo + 1)) m_seqNo++;
  RrepHeader rrepHeader ( /*prefixSize=*/0, /*hops=*/toOrigin.GetHop (), /*dst=*/rreqHeader.GetDst (),
                        /*dstSeqNo=*/m_seqNo, /*origin=*/toOrigin.GetDestination (), /*lifeTime=*/MY_ROUTE_TIMEOUT);
  // TODO when??
  rrepHeader.SetAckRequired(true);
  RoutingTableEntry toNextHop;
  m_routingTable.LookupRoute(toOrigin.GetNextHop (), toNextHop);
  toNextHop.m_ackTimer.SetFunction(&RoutingProtocol::AckTimerExpire, this);
  toNextHop.m_ackTimer.SetArguments(toNextHop.GetDestination(), BLACKLIST_TIMEOUT);
  toNextHop.m_ackTimer.SetDelay(NEXT_HOP_WAIT);

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (rrepHeader);
  TypeHeader tHeader (AODVTYPE_RREP);
  packet->AddHeader (tHeader);
  socket->SendTo (packet, 0, InetSocketAddress (toOrigin.GetNextHop(), AODV_PORT));
}

void
RoutingProtocol::SendReplyByIntermediateNode (RoutingTableEntry & toDst, RoutingTableEntry & toOrigin, bool gratRep, Ptr<Socket> socket)
{

  RrepHeader rrepHeader (/*prefix size=*/0, /*hops=*/toDst.GetHop (), /*dst=*/toDst.GetDestination (), /*dst seqno=*/toDst.GetSeqNo (),
                         /*origin=*/toOrigin.GetDestination (), /*lifetime=*/toDst.GetLifeTime () - Simulator::Now ());

  toDst.InsertPrecursor (toOrigin.GetNextHop ());
  toOrigin.InsertPrecursor (toDst.GetNextHop ());
  m_routingTable.Update (toDst.GetDestination (), toDst);
  m_routingTable.Update (toOrigin.GetDestination (), toOrigin);

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (rrepHeader);
  TypeHeader tHeader (AODVTYPE_RREP);
  packet->AddHeader (tHeader);
  socket->SendTo (packet, 0, InetSocketAddress (toOrigin.GetNextHop (), AODV_PORT));

  // Generating gratuitous RREPs
  if (gratRep)
  {
    RrepHeader gratRepHeader (/*prefix size=*/0, /*hops=*/toOrigin.GetHop (), /*dst=*/toOrigin.GetDestination (), /*dst seqno=*/toOrigin.GetSeqNo (),
                              /*origin=*/toDst.GetDestination (), /*lifetime=*/toOrigin.GetLifeTime () - Simulator::Now ());
    Ptr<Packet> packetToDst = Create<Packet> ();
    packetToDst->AddHeader (rrepHeader);
    packetToDst->AddHeader (tHeader);
    socket->SendTo (packetToDst, 0, InetSocketAddress (toDst.GetNextHop(), AODV_PORT));

  }
}

void
RoutingProtocol::SendReplyAck(Ipv4Address neighbor)
{
  NS_LOG_FUNCTION(this);
  RrepAckHeader h;
  TypeHeader typeHeader(AODVTYPE_RREP_ACK);
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader(h);
  packet->AddHeader(typeHeader);
  RoutingTableEntry toNeighbor;
  m_routingTable.LookupRoute(neighbor, toNeighbor);
  Ptr<Socket> socket = FindSocketWithInterfaceAddress(toNeighbor.GetInterface());
  socket->SendTo (packet, 0, InetSocketAddress (neighbor, AODV_PORT));
}

void
RoutingProtocol::RecvReply (Ptr<Packet> p, Ipv4Address receiver, Ipv4Address sender)
{
  NS_LOG_FUNCTION(this << " src " << sender);
  RrepHeader rrepHeader;
  p->RemoveHeader (rrepHeader);
  NS_LOG_LOGIC("RREP destination " << rrepHeader.GetDst() << " RREP origin " << rrepHeader.GetOrigin());

  uint8_t hop = rrepHeader.GetHopCount () + 1;
  rrepHeader.SetHopCount (hop);

  // If RREP is Hello message
  if (rrepHeader.GetDst () == rrepHeader.GetOrigin ())
  {
    ProcessHello (rrepHeader, receiver);
    return;
  }

  /*
   * If the route table entry to the destination is created or updated, then the following actions occur:
   * -  the route is marked as active,
   * -  the destination sequence number is marked as valid,
   * -  the next hop in the route entry is assigned to be the node from which the RREP is received,
   *    which is indicated by the source IP address field in the IP header,
   * -  the hop count is set to the value of the hop count from RREP message + 1
   * -  the expiry time is set to the current time plus the value of the Lifetime in the RREP message,
   * -  and the destination sequence number is the Destination Sequence Number in the RREP message.
   */
  Ptr<NetDevice> dev = m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (receiver));
  RoutingTableEntry newEntry (/*device=*/dev, /*dst=*/rrepHeader.GetDst (), /*validSeqNo=*/
  true, /*seqno=*/rrepHeader.GetDstSeqno (), /*iface=*/receiver,
  /*hop=*/hop, /*nextHop=*/sender, /*lifeTime=*/Simulator::Now () + rrepHeader.GetLifeTime ());
  RoutingTableEntry toDst;
  if (m_routingTable.LookupRoute (rrepHeader.GetDst (), toDst))
  {
    /*
     * The existing entry is updated only in the following circumstances:
     * (i) the sequence number in the routing table is marked as invalid in route table entry.
     */
    if (!toDst.GetValidSeqNo ())
    {
      m_routingTable.Update (rrepHeader.GetDst (), newEntry);
    }
    // (ii)the Destination Sequence Number in the RREP is greater than the node's copy of the destination sequence number and the known value is valid,
    else if ((int32_t (rrepHeader.GetDstSeqno ()) - int32_t (toDst.GetSeqNo ())) > 0)
    {
      m_routingTable.Update (rrepHeader.GetDst (), newEntry);
    }
    else
    {
      // (iii) the sequence numbers are the same, but the route is marked as inactive.
      if ((rrepHeader.GetDstSeqno () == toDst.GetSeqNo ()) && (toDst.GetFlag () != RTF_UP))
      {
        m_routingTable.Update (rrepHeader.GetDst (), newEntry);
      }
      // (iv)  the sequence numbers are the same, and the New Hop Count is smaller than the hop count in route table entry.
      else if ((rrepHeader.GetDstSeqno () == toDst.GetSeqNo ()) && (hop < toDst.GetHop ()))
      {
        m_routingTable.Update (rrepHeader.GetDst (), newEntry);
      }
    }
  }
  else
  {
    // The forward route for this destination is created if it does not already exist.
    NS_LOG_LOGIC("add new route");
    m_routingTable.AddRoute (newEntry);
  }
  // Acknowledge receipt of the RREP by sending a RREP-ACK message back
  if(rrepHeader.GetAckRequired()) SendReplyAck(sender);
  if (receiver == rrepHeader.GetOrigin ())
  {
    if (toDst.GetFlag () == RTF_IN_SEARCH)
      m_routingTable.Update (rrepHeader.GetDst (), newEntry);
    SendPacketFromQueue (rrepHeader.GetDst (), newEntry.GetRoute ());
    return;
  }

  RoutingTableEntry toOrigin;
  if (!m_routingTable.LookupRoute (rrepHeader.GetOrigin (), toOrigin))
    return; // Impossible! drop.
  if (toOrigin.GetLifeTime () < (Simulator::Now () + ACTIVE_ROUTE_TIMEOUT))
  {
    toOrigin.SetLifeTime (Simulator::Now () + ACTIVE_ROUTE_TIMEOUT);
    m_routingTable.Update (toOrigin.GetDestination (), toOrigin);
  }

  // Update information about precursors
  m_routingTable.LookupRoute (rrepHeader.GetDst (), toDst);
  toDst.InsertPrecursor (toOrigin.GetNextHop ());
  m_routingTable.Update (rrepHeader.GetDst (), toDst);
  RoutingTableEntry toNextHopToDst;
  m_routingTable.LookupRoute (toDst.GetNextHop (), toNextHopToDst);
  toNextHopToDst.InsertPrecursor (toOrigin.GetNextHop ());
  m_routingTable.Update (toDst.GetNextHop (), toNextHopToDst);

  // TODO add operation over unidirctinal links
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (rrepHeader);
  TypeHeader tHeader (AODVTYPE_RREP);
  packet->AddHeader (tHeader);
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
  {
    dev = m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (j->second.GetLocal ()));
    if (dev->GetAddress () == toOrigin.GetOutputDevice ()->GetAddress ())
      j->first->SendTo (packet, 0, InetSocketAddress (toOrigin.GetNextHop (), AODV_PORT));
  }
}

void
RoutingProtocol::RecvReplyAck(Ipv4Address neighbor)
{
  // TODO
}


// TODO may be used for determining connectivity
void
RoutingProtocol::ProcessHello (RrepHeader const & rrepHeader, Ipv4Address receiverIfaceAddr)
{
  NS_LOG_FUNCTION(this << "from " << rrepHeader.GetDst ());
  /*
   *  Whenever a node receives a Hello message from a neighbor, the node
   * SHOULD make sure that it has an active route to the neighbor, and
   * create one if necessary.
   */
  RoutingTableEntry toNeighbor;
  if (!m_routingTable.LookupRoute (rrepHeader.GetDst (), toNeighbor))
  {
    Ptr<NetDevice> dev = m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (receiverIfaceAddr));
    RoutingTableEntry newEntry (/*device=*/dev, /*dst=*/rrepHeader.GetDst (), /*validSeqNo=*/
    true, /*seqno=*/rrepHeader.GetDstSeqno (), /*iface=*/receiverIfaceAddr,
    /*hop=*/rrepHeader.GetHopCount (), /*nextHop=*/rrepHeader.GetDst (), /*lifeTime=*/
    Simulator::Now () + rrepHeader.GetLifeTime ());
    m_routingTable.AddRoute (newEntry);
  }
  else
  {
    if (toNeighbor.GetLifeTime () < Simulator::Now () + Scalar (ALLOWED_HELLO_LOSS) * HELLO_INTERVAL)
    {
      toNeighbor.SetLifeTime (Simulator::Now () + Scalar (ALLOWED_HELLO_LOSS) * HELLO_INTERVAL);
    }
    toNeighbor.SetSeqNo (rrepHeader.GetDstSeqno ());
    toNeighbor.SetValidSeqNo (true);
    toNeighbor.SetFlag (RTF_UP);
    Ptr<NetDevice> dev = m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (receiverIfaceAddr));
    toNeighbor.SetOutputDevice (dev);
    m_routingTable.Update (rrepHeader.GetDst (), toNeighbor);
  }
  UpdateNeighbor(rrepHeader.GetDst (), Scalar (ALLOWED_HELLO_LOSS) * HELLO_INTERVAL);
}

// TODO process RERR with 'N' flag
void
RoutingProtocol::RecvError (Ptr<Packet> p, Ipv4Address src)
{
  RerrHeader rerrHeader;
  p->RemoveHeader(rerrHeader);
  std::map<Ipv4Address, uint32_t> dstWithNextHopSrc;
  std::map<Ipv4Address, uint32_t> unreachable;
  m_routingTable.GetListOfDestinationWithNextHop(src, dstWithNextHopSrc);
  std::pair<Ipv4Address, uint32_t>  un;
  while (rerrHeader.RemoveUnDestination(un))
  {
    if (IsNeighbor (un.first)) SendRerrWhenBreaksLinkToNextHop (un.first);
    else
    {
      for (std::map<Ipv4Address, uint32_t>::const_iterator i = dstWithNextHopSrc.begin(); i != dstWithNextHopSrc.end(); ++i)
        if (i->first == un.first) unreachable.insert(un);
    }
  }

  std::vector<Ipv4Address> precursors;
  for (std::map<Ipv4Address, uint32_t>::const_iterator i = unreachable.begin (); i != unreachable.end ();)
  {
    if (!rerrHeader.AddUnDestination (i->first, i->second))
    {
      TypeHeader typeHeader (AODVTYPE_RERR);
      Ptr<Packet> packet = Create<Packet> ();
      packet->AddHeader (rerrHeader);
      packet->AddHeader (typeHeader);
      SendRerrMessage (packet, precursors);
      rerrHeader.Clear();
    }
    else
    {
      RoutingTableEntry toDst;
      m_routingTable.LookupRoute (i->first, toDst);
      toDst.GetPrecursors(precursors);
      ++i;
    }
  }
  m_routingTable.InvalidateRoutesWithDst(unreachable, DELETE_PERIOD);
}

void
RoutingProtocol::RouteRequestTimerExpire(Ipv4Address dst)
{
  NS_LOG_LOGIC(this);
  RoutingTableEntry toDst;
  m_routingTable.LookupRoute(dst, toDst);
  if(toDst.GetFlag() == RTF_UP)
  {
    NS_LOG_LOGIC("route to " << dst << " found");
    return;
  }
  /**
   *  If a route discovery has been attempted RREQ_RETRIES times at the maximum TTL without
   *  receiving any RREP, all data packets destined for the corresponding destination SHOULD be
   *  dropped from the buffer and a Destination Unreachable message SHOULD be delivered to the application.
   */
  if(toDst.GetRreqCnt() == RREQ_RETRIES)
  {
    NS_LOG_LOGIC("route discovery to " << dst << " has been attempted RREQ_RETRIES times");
    // TODO drop packet from queue and deliver Destination Unreachable message to the application.
    return;
  }
  NS_LOG_LOGIC("Send new RREQ to " << dst);
  if (toDst.GetFlag() == RTF_IN_SEARCH) SendRequest (dst, false, true);
}

void
RoutingProtocol::BroadcastTimerExpire ()
{
  NS_LOG_FUNCTION(this);
  PurgeBroadcastId ();
  btimer.Cancel();
  btimer.Schedule ();
}

void
RoutingProtocol::HelloTimerExpire ()
{
  NS_LOG_FUNCTION(this);
  SendHello ();
  // TODO select random time for the next hello
  htimer.Cancel();
  htimer.Schedule (HELLO_INTERVAL);
}

void
RoutingProtocol::NeighborTimerExpire ()
{
  NS_LOG_FUNCTION(this);
  PurgeNeighbor();
  ntimer.Cancel();
  ntimer.Schedule (HELLO_INTERVAL);
}

void
RoutingProtocol::RouteCacheTimerExpire ()
{
  NS_LOG_FUNCTION(this);
  RtPurge();
  rtimer.Schedule (FREQUENCY);
}

void
RoutingProtocol::LocalRepairTimerExpire ()
{
  // TODO start local repair procedure
}

void
RoutingProtocol::AckTimerExpire (Ipv4Address neighbor, Time blacklistTimeout)
{
  NS_LOG_FUNCTION(this);
  m_routingTable.MarkLinkAsUinidirectional(neighbor, blacklistTimeout);
}

void
RoutingProtocol::RoutingTableEntryTimerExpire(Ipv4Address dst)
{
  // TODO
}

void
RoutingProtocol::SendHello ()
{
  NS_LOG_FUNCTION(this);
  /* Broadcast a RREP with TTL = 1 with the RREP message fields set as follows:
   *   Destination IP Address         The node's IP address.
   *   Destination Sequence Number    The node's latest sequence number.
   *   Hop Count                      0
   *   Lifetime                       ALLOWED_HELLO_LOSS * HELLO_INTERVAL
   */
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
  {
    Ptr<Socket> socket = j->first;
    Ipv4InterfaceAddress iface = j->second;
    RrepHeader helloHeader (/*prefix size=*/0, /*hops=*/0, /*dst=*/iface.GetLocal (), /*dst seqno=*/m_seqNo,
                            /*origin=*/iface.GetLocal (),/*lifetime=*/Scalar (ALLOWED_HELLO_LOSS) * HELLO_INTERVAL);
    Ptr<Packet> packet = Create<Packet> ();
    packet->AddHeader (helloHeader);
    TypeHeader tHeader (AODVTYPE_RREP);
    packet->AddHeader (tHeader);
    socket->Send (packet);
  }
}

void
RoutingProtocol::SendPacketFromQueue (Ipv4Address dst, Ptr<Ipv4Route> route)
{
  QueueEntry queueEntry;
  while (m_queue.Dequeue (dst, queueEntry))
  {
    UnicastForwardCallback ucb = queueEntry.GetUnicastForwardCallback ();
    ucb (route, queueEntry.GetPacket (), queueEntry.GetIpv4Header ());
  }
}

void
RoutingProtocol::Send (Ptr<Ipv4Route> route, Ptr<const Packet> packet, const Ipv4Header & header)
{
  NS_LOG_FUNCTION(this << packet->GetUid());
  Ptr<Ipv4L3Protocol> l3 = m_ipv4->GetObject<Ipv4L3Protocol> ();
  NS_ASSERT(l3 != 0);
  Ptr<Packet> p = Create<Packet> ();
  *p = *packet;
  // TODO know protocol number
  l3->Send (p, route->GetSource (), header.GetDestination (), 1, route);

}

void
RoutingProtocol::SendRerrWhenBreaksLinkToNextHop (Ipv4Address nextHop)
{
  RerrHeader rerrHeader;
  std::vector<Ipv4Address> precursors;
  std::map<Ipv4Address, uint32_t> unreachable;

  RoutingTableEntry toNextHop;
  if (!m_routingTable.LookupRoute (nextHop, toNextHop))
    return;
  toNextHop.GetPrecursors(precursors);
  rerrHeader.AddUnDestination (nextHop, toNextHop.GetSeqNo ());
  m_routingTable.GetListOfDestinationWithNextHop (nextHop, unreachable);
  for (std::map<Ipv4Address, uint32_t>::const_iterator i = unreachable.begin (); i != unreachable.end ();)
  {
    if (!rerrHeader.AddUnDestination (i->first, i->second))
    {
      TypeHeader typeHeader (AODVTYPE_RERR);
      Ptr<Packet> packet = Create<Packet> ();
      packet->AddHeader (rerrHeader);
      packet->AddHeader (typeHeader);
      SendRerrMessage (packet, precursors);
      rerrHeader.Clear();
    }
    else
    {
      RoutingTableEntry toDst;
      m_routingTable.LookupRoute (i->first, toDst);
      toDst.GetPrecursors(precursors);
      ++i;
    }
  }
  unreachable.insert(std::make_pair(nextHop, toNextHop.GetSeqNo ()));
  m_routingTable.InvalidateRoutesWithDst(unreachable, DELETE_PERIOD);
}

void
RoutingProtocol::SendRerrMessage (Ptr<Packet> packet, std::vector<Ipv4Address> precursors)
{
  if (precursors.empty ())
    return; // TODO too many unreachable destinations, but no precursors
  // If there is only one precursor, RERR SHOULD be unicast toward that precursor
  if (precursors.size () == 1)
  {
    RoutingTableEntry toPrecursor;
    m_routingTable.LookupRoute (precursors.front (), toPrecursor);
    Ptr<Socket> socket = FindSocketWithInterfaceAddress (toPrecursor.GetInterface ());
    socket->SendTo (packet, 0, InetSocketAddress (toPrecursor.GetDestination (), AODV_PORT));
    return;
  }

  //  Should only transmit RERR on those interfaces which have precursor nodes for the broken route
  std::vector<Ipv4Address> ifaces;
  RoutingTableEntry toPrecursor;
  for (std::vector<Ipv4Address>::const_iterator i = precursors.begin (); i != precursors.end (); ++i)
  {
    if (!m_routingTable.LookupRoute (*i, toPrecursor)) break;
    bool result = true;
    for (std::vector<Ipv4Address>::const_iterator i = ifaces.begin (); i != ifaces.end (); ++i)
      if (*i == toPrecursor.GetInterface ())
      {
        result = false;
        break;
      }
    if (result)
      ifaces.push_back (toPrecursor.GetInterface ());
  }

  for (std::vector<Ipv4Address>::const_iterator i = ifaces.begin (); i != ifaces.end (); ++i)
  {
    Ptr<Socket> socket = FindSocketWithInterfaceAddress (*i);
    socket->Send (packet);
  }

}

Ptr<Socket>
RoutingProtocol::FindSocketWithInterfaceAddress (Ipv4Address addr) const
{
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
  {
    Ptr<Socket> socket = j->first;
    Ipv4InterfaceAddress iface = j->second;
    if (iface.GetLocal () == addr)
      return socket;
  }
  Ptr<Socket> socket;
  return socket;
}

void
RoutingProtocol::LocalRouteRepair (Ipv4Address dst, Ptr<Packet> p)
{
  // TODO local_rt_repair
}


void
RoutingProtocol::HandleLinkFailure (Ipv4Address addr)
{
  NS_LOG_FUNCTION(this << addr);
  SendRerrWhenBreaksLinkToNextHop(addr);
  // TODO
}


void
RoutingProtocol::RtPurge ()
{
  NS_LOG_FUNCTION(this);
  m_routingTable.Purge(DELETE_PERIOD);
  // TODO AODV::rt_purge()
}
//-----------------------------------------------------------------------------
// TODO: NeighborList, BroadcastIdCache
//-----------------------------------------------------------------------------
}
}
