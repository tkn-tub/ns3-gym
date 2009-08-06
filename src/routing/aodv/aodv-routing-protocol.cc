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

#include "ns3/raw-socket-factory.h"
#include "src/internet-stack/udp-l4-protocol.h"
#include <algorithm>

NS_LOG_COMPONENT_DEFINE ("AodvRoutingProtocol");

namespace ns3
{
namespace aodv
{
NS_OBJECT_ENSURE_REGISTERED (RoutingProtocol);

/// UDP Port for AODV control traffic
const uint32_t RoutingProtocol::AODV_PORT = 654;

RoutingProtocol::RoutingProtocol () :
  RreqRetries (2),
  ActiveRouteTimeout (Seconds (3)),
  NetDiameter (35),
  NodeTraversalTime (MilliSeconds (40)),
  NetTraversalTime (Scalar (2 * NetDiameter) * NodeTraversalTime),
  PathDiscoveryTime ( Scalar (2) * NetTraversalTime),
  MyRouteTimeout (Scalar (2) * std::max (PathDiscoveryTime, ActiveRouteTimeout)),
  HelloInterval(Seconds (1)),
  AllowedHelloLoss (2),
  DeletePeriod (Scalar(5) * std::max(ActiveRouteTimeout, HelloInterval)),
  NextHopWait (NodeTraversalTime + MilliSeconds (10)),
  TtlStart (2),
  TtlIncrement (2),
  TtlThreshold (7),
  MaxRepairTtl (0.3* NetDiameter),
  LocalAddTtl (2),
  TimeoutBuffer (2),
  BlackListTimeout(Scalar (RreqRetries) * NetTraversalTime),
  MaxQueueLen (64),
  MaxQueueTime (Seconds(30)),
  DestinationOnly (false),
  GratuitousReply (true),
  EnableExpandingRingSearch (true),
  EnableHello (true),
  EnableLocalRepair (true),
  m_routingTable (DeletePeriod),
  m_queue (MaxQueueLen, MaxQueueTime),
  m_requestId (0),
  m_seqNo (0),
  m_nb(HelloInterval),
  htimer (Timer::CANCEL_ON_DESTROY),
  rtimer (Timer::CANCEL_ON_DESTROY),
  lrtimer (Timer::CANCEL_ON_DESTROY)

{
  if (EnableHello)
    {
      m_nb.SetCallback (MakeCallback (&RoutingProtocol::HandleLinkFailure, this));
    }
  /* BlackListTimeout should be suitably increased if an expanding ring search is used.  In such cases, it should be
   *    {[(TtlThreshold - TtlStart)/TtlIncrement] + 1 + RreqRetries} *NetTraversalTime.
   * This is to account for possible additional route discovery attempts.
   */
  if (EnableExpandingRingSearch)
    {
      BlackListTimeout = Scalar ((((TtlThreshold - TtlStart) / TtlIncrement) + 1 + RreqRetries)) * NetTraversalTime;
    }
}

TypeId
RoutingProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::aodv::RoutingProtocol")
      .SetParent<Ipv4RoutingProtocol> ()
      .AddConstructor<RoutingProtocol> ()
      .AddAttribute ("HelloInterval", "HELLO messages emission interval.",
                     TimeValue (Seconds (1)),
                     MakeTimeAccessor (&RoutingProtocol::HelloInterval),
                     MakeTimeChecker ())
      .AddAttribute ("RreqRetries", "Maximum number of retransmissions of RREQ to discover a route",
                     UintegerValue (2),
                     MakeUintegerAccessor (&RoutingProtocol::RreqRetries),
                     MakeUintegerChecker<uint32_t> ())
      .AddAttribute ("NodeTraversalTime", "Conservative estimate of the average one hop traversal time for packets and should include "
                     "queuing delays, interrupt processing times and transfer times.",
                     TimeValue (MilliSeconds (40)),
                     MakeTimeAccessor (&RoutingProtocol::NodeTraversalTime),
                     MakeTimeChecker ())
      .AddAttribute ("ActiveRouteTimeout", "Period of time during which the route is considered to be valid",
                     TimeValue (Seconds (3)),
                     MakeTimeAccessor (&RoutingProtocol::ActiveRouteTimeout),
                     MakeTimeChecker ())
      .AddAttribute ("NetDiameter", "Net diameter measures the maximum possible number of hops between two nodes in the network",
                     UintegerValue (35),
                     MakeUintegerAccessor (&RoutingProtocol::NetDiameter),
                     MakeUintegerChecker<uint32_t> ())
      .AddAttribute ("TtlStart", "Initial value of TTL in RREQ  when use an expanding ring search "
                     "(should be set to at least 2 if Hello messages are used for local connectivity information.)",
                     UintegerValue (2),
                     MakeUintegerAccessor (&RoutingProtocol::TtlStart),
                     MakeUintegerChecker<uint16_t> ())
      .AddAttribute ("TtlIncrement", "Increment value of RREQ TTL when use an expanding ring search",
                     UintegerValue (2),
                     MakeUintegerAccessor (&RoutingProtocol::TtlIncrement),
                     MakeUintegerChecker<uint16_t> ())
      .AddAttribute ("TtlThreshold", "Threshold, beyond which TTL = NetDiameter is used for each attempt in RREQ.",
                     UintegerValue (7),
                     MakeUintegerAccessor (&RoutingProtocol::TtlThreshold),
                     MakeUintegerChecker<uint16_t> ())
      .AddAttribute ("LocalAddTtl", "Value used in calculation RREQ TTL when use local repair.",
                     UintegerValue (7),
                     MakeUintegerAccessor (&RoutingProtocol::LocalAddTtl),
                     MakeUintegerChecker<uint16_t> ())
      .AddAttribute ("MaxQueueLen", "Maximum number of packets that we allow a routing protocol to buffer.",
                     UintegerValue (64),
                     MakeUintegerAccessor (&RoutingProtocol::MaxQueueLen),
                     MakeUintegerChecker<uint32_t> ())
      .AddAttribute ("MaxQueueTime", "Maximum time packets can be queued (in seconds)",
                     TimeValue (Seconds (30)),
                     MakeTimeAccessor (&RoutingProtocol::MaxQueueTime),
                     MakeTimeChecker ())
      .AddAttribute ("AllowedHelloLoss", "Number of hello messages which may be loss for valid link.",
                     UintegerValue (2),
                     MakeUintegerAccessor (&RoutingProtocol::AllowedHelloLoss),
                     MakeUintegerChecker<uint16_t> ())
      .AddAttribute ("GratuitousReply", "indicates whether a gratuitous RREP should be unicast to the node originated route discovery.",
                     BooleanValue (true),
                     MakeBooleanAccessor (&RoutingProtocol::SetGratuitousReplyFlag,
                                          &RoutingProtocol::GetGratuitousReplyFlag),
                     MakeBooleanChecker ())
      .AddAttribute ("DestinationOnly", "Indicates only the destination may respond to this RREQ.",
                     BooleanValue (false),
                     MakeBooleanAccessor (&RoutingProtocol::SetDesinationOnlyFlag,
                                          &RoutingProtocol::GetDesinationOnlyFlag),
                     MakeBooleanChecker ())
      .AddAttribute ("EnableExpandingRingSearch", "Enable expanding ring search technique.",
                     BooleanValue (true),
                     MakeBooleanAccessor (&RoutingProtocol::SetExpandingRingSearchEnable,
                                          &RoutingProtocol::GetExpandingRingSearchEnable),
                     MakeBooleanChecker ())
      .AddAttribute ("EnableHello", "Enable hello message.",
                     BooleanValue (true),
                     MakeBooleanAccessor (&RoutingProtocol::SetHelloEnable,
                                          &RoutingProtocol::GetHelloEnable),
                     MakeBooleanChecker ())
      .AddAttribute ("EnableLocalRepair", "Enable local repair.",
                     BooleanValue (true),
                     MakeBooleanAccessor (&RoutingProtocol::SetLocalRepairEnable,
                                          &RoutingProtocol::GetLocalRepairEnable),
                     MakeBooleanChecker ())
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
  // Open sockets for control traffic on each IP interface
  const Ipv4Address loopback ("127.0.0.1");
  for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i++)
  {
    Ipv4InterfaceAddress iface = m_ipv4->GetAddress (i, 0);
    if (iface.GetLocal () == loopback)
      continue;

    // Create a socket to listen only on this interface

    Ptr<Ipv4L3Protocol> l3 = m_ipv4->GetObject<Ipv4L3Protocol> ();
    Ptr<Socket> socket = l3->CreateRawSocket2();
    NS_ASSERT (socket != 0);
    socket->SetRecvCallback (MakeCallback (&RoutingProtocol::RecvAodv, this));
    socket->Bind(InetSocketAddress (iface.GetLocal(), AODV_PORT));
    socket->Connect (InetSocketAddress (iface.GetBroadcast(), AODV_PORT));
    m_socketAddresses.insert (std::make_pair (socket, iface));

    // Add local broadcast record to the routing table
    Ptr<NetDevice> dev = m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (iface.GetLocal ()));
    RoutingTableEntry rt (/*device=*/dev, /*dst=*/iface.GetBroadcast (), /*know seqno=*/true, /*seqno=*/0, /*iface=*/iface,
                          /*hops=*/1, /*next hop=*/iface.GetBroadcast (), /*lifetime=*/Seconds (1e9)); // TODO use infty
    m_routingTable.AddRoute (rt);
  }

  m_scb = MakeCallback (&RoutingProtocol::Send, this);
  m_ecb = MakeCallback (&RoutingProtocol::Drop, this);

  if (EnableHello)
    {
      m_nb.ScheduleTimer ();
    }
}

Ptr<Ipv4Route>
RoutingProtocol::RouteOutput (Ptr<Packet> p, const Ipv4Header &header, uint32_t oif, Socket::SocketErrno &sockerr )
{
  NS_LOG_FUNCTION (this << p->GetUid() << header.GetDestination());
  sockerr = Socket::ERROR_NOTERROR;
  Ptr<Ipv4Route> route;
  Ipv4Address dst = header.GetDestination ();
  RoutingTableEntry rt;
  if (m_routingTable.LookupRoute (dst, rt))
    {
      if (rt.GetFlag () == RTF_UP)
        {
          route = rt.GetRoute ();
          NS_ASSERT (route != 0);
          NS_LOG_LOGIC("exist route to " << route->GetDestination() << " from iface " << route->GetSource());
          UpdateRouteLifeTime (dst, ActiveRouteTimeout);
          UpdateRouteLifeTime (route->GetGateway(), ActiveRouteTimeout);
          m_nb.Update (route->GetGateway(), ActiveRouteTimeout);  //?
        }
      else
        {
          QueueEntry newEntry (p, header, m_scb, m_ecb);

          m_queue.Enqueue (newEntry);
          if (rt.GetFlag () == RTF_DOWN)
            {
              m_routingTable.SetEntryState (dst, RTF_IN_SEARCH);
              if (EnableExpandingRingSearch)
                SendRequest (dst, rt.GetHop () + TtlIncrement);
              else
                SendRequest (dst, NetDiameter);
            }
        }
    }
  else
    {
      QueueEntry newEntry (p, header, m_scb, m_ecb);
      m_queue.Enqueue (newEntry);
      if (EnableExpandingRingSearch)
        SendRequest (dst, TtlStart);
      else
        SendRequest (dst, NetDiameter);
    }
  return route;
}

bool
RoutingProtocol::RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev, UnicastForwardCallback ucb,
    MulticastForwardCallback mcb, LocalDeliverCallback lcb, ErrorCallback ecb )
{
  NS_LOG_FUNCTION (this << p->GetUid() << header.GetDestination() << idev->GetAddress());

  NS_ASSERT (m_ipv4 != 0);
  // Check if input device supports IP
  NS_ASSERT (m_ipv4->GetInterfaceForDevice (idev) >= 0);
  int32_t iif = m_ipv4->GetInterfaceForDevice (idev);

  Ipv4Address dst = header.GetDestination ();
  Ipv4Address origin = header.GetSource ();

  if (IsMyOwnAddress (origin))
    return true;

  // Local delivery to AODV interfaces
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
    {
      Ipv4InterfaceAddress iface = j->second;
      if (m_ipv4->GetInterfaceForAddress (iface.GetLocal ()) == iif)
        if (dst == iface.GetBroadcast ())
          {
            if (m_idCache.LookupId (origin, p->GetUid ()))
              {
                NS_LOG_LOGIC("Duplicated packet from " << origin);
                return true;
              }
            m_idCache.InsertId (origin, p->GetUid (), PathDiscoveryTime);
            UpdateRouteLifeTime (origin, ActiveRouteTimeout);
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
          UpdateRouteLifeTime (origin, ActiveRouteTimeout);
          RoutingTableEntry toOrigin;
          m_routingTable.LookupRoute (origin, toOrigin);
          UpdateRouteLifeTime (toOrigin.GetNextHop (), ActiveRouteTimeout);
          m_nb.Update (toOrigin.GetNextHop (), ActiveRouteTimeout);
          NS_LOG_LOGIC ("Unicast local delivery to " << iface.GetLocal ());
          lcb (p, header, iif);
          return true;
        }
    }

  // TODO: local delivery to non-AODV interfaces
  // Forwarding
  return Forwarding (p, header, ucb, ecb);
}

bool
RoutingProtocol::Forwarding (Ptr<const Packet> p, const Ipv4Header & header, UnicastForwardCallback ucb, ErrorCallback ecb)
{
  Ipv4Address dst = header.GetDestination ();
  Ipv4Address origin = header.GetSource ();
  m_routingTable.Purge ();
  RoutingTableEntry toDst;
  if (m_routingTable.LookupRoute (dst, toDst))
    {
      if (toDst.GetFlag () == RTF_DOWN && EnableLocalRepair)
        {
          if (toDst.GetHop () > MaxRepairTtl)
            return false;
          LocalRouteRepair (dst, origin);
          QueueEntry newEntry (p, header, ucb, ecb, MaxQueueTime);
          m_queue.Enqueue (newEntry);
          NS_LOG_LOGIC("Local repair "<< dst);
          return true;
        }
      else if (toDst.GetFlag () == RTF_IN_REPAIR)
        {
          QueueEntry newEntry (p, header, ucb, ecb, MaxQueueTime);
          m_queue.Enqueue (newEntry);
          return true;
        }
      else if (toDst.GetFlag () == RTF_UP)
        {
          Ptr<Ipv4Route> route = toDst.GetRoute ();
          NS_LOG_LOGIC(route->GetSource()<<" forwarding to " << dst << " from" << origin);

          /**
           *  Each time a route is used to forward a data packet, its Active Route
           *  Lifetime field of the source, destination and the next hop on the
           *  path to the destination is updated to be no less than the current
           *  time plus ActiveRouteTimeout.
           */
          UpdateRouteLifeTime (origin, ActiveRouteTimeout);
          UpdateRouteLifeTime (dst, ActiveRouteTimeout);
          UpdateRouteLifeTime (route->GetGateway (), ActiveRouteTimeout);
          /**
           *  Since the route between each originator and destination pair is expected to be symmetric, the
           *  Active Route Lifetime for the previous hop, along the reverse path back to the IP source, is also updated
           *  to be no less than the current time plus ActiveRouteTimeout
           */
          RoutingTableEntry toOrigin;
          m_routingTable.LookupRoute (origin, toOrigin);
          UpdateRouteLifeTime (toOrigin.GetNextHop (), ActiveRouteTimeout);

          m_nb.Update (route->GetGateway (), ActiveRouteTimeout); //?
          m_nb.Update (toOrigin.GetNextHop (), ActiveRouteTimeout);

          ucb (route, p, header);
          return true;
        }
    }
  NS_LOG_LOGIC("route not found to "<< dst);
  return false;
}

void
RoutingProtocol::SetIpv4 (Ptr<Ipv4> ipv4)
{
  NS_ASSERT (ipv4 != 0);
  NS_ASSERT (m_ipv4 == 0);

  if (EnableLocalRepair)
    {
      lrtimer.SetFunction (&RoutingProtocol::LocalRepairTimerExpire, this);
    }
  if (EnableHello)
    {
      htimer.SetFunction (&RoutingProtocol::HelloTimerExpire, this);
      htimer.SetDelay (HelloInterval);
    }

  m_ipv4 = ipv4;
  Simulator::ScheduleNow (&RoutingProtocol::Start, this);
}

void
RoutingProtocol::NotifyInterfaceUp (uint32_t i )
{
  // TODO
}

void
RoutingProtocol::NotifyInterfaceDown (uint32_t i )
{
  NS_LOG_FUNCTION (this << m_ipv4->GetAddress (i, 0).GetLocal ());
  // TODO
}

void
RoutingProtocol::NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address )
{
}

void
RoutingProtocol::NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address )
{
}

bool
RoutingProtocol::IsMyOwnAddress (Ipv4Address src )
{
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
    {
      Ipv4InterfaceAddress iface = j->second;
      if (src == iface.GetLocal ())
        {
          return true;
        }
    }
  return false;
}

void
RoutingProtocol::SendRequest (Ipv4Address dst, uint16_t ttl)
{
  NS_LOG_FUNCTION ( this << dst);
  // Create RREQ header
  RreqHeader rreqHeader;
  rreqHeader.SetDst (dst);

  RoutingTableEntry rt;
  if (m_routingTable.LookupRoute (dst, rt))
    {
      // if we already send maximum number of RREQ
      if (rt.GetRreqCnt () == RreqRetries)
        {
          NS_LOG_LOGIC ("Route does not found to " << dst);
          return;
        }
      rreqHeader.SetHopCount (rt.GetHop ());
      if (rt.GetValidSeqNo ())
        rreqHeader.SetDstSeqno (rt.GetSeqNo ());
      else
        rreqHeader.SetUnknownSeqno (true);
    }
  else
    {
      Ptr<NetDevice> dev;
      rreqHeader.SetUnknownSeqno (true);
      RoutingTableEntry newEntry (dev, dst, false, 0, Ipv4InterfaceAddress (), 0, Ipv4Address (), Seconds (0));
      newEntry.SetFlag (RTF_IN_SEARCH);
      m_routingTable.AddRoute (newEntry);
    }

  if (GratuitousReply)
    rreqHeader.SetGratiousRrep (true);
  if (DestinationOnly)
    rreqHeader.SetDestinationOnly (true);

  m_seqNo++;
  rreqHeader.SetOriginSeqno (m_seqNo);
  m_requestId++;
  rreqHeader.SetId (m_requestId);
  rreqHeader.SetHopCount (0);

  // Send RREQ as subnet directed broadcast from each interface used by aodv
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
    {
      Ptr<Socket> socket = j->first;
      Ipv4InterfaceAddress iface = j->second;

      rreqHeader.SetOrigin (iface.GetLocal ());
      m_idCache.InsertId (iface.GetLocal (), m_requestId, PathDiscoveryTime);

      Ptr<Packet> packet = Create<Packet> ();
      packet->AddHeader (rreqHeader);
      TypeHeader tHeader (AODVTYPE_RREQ);
      packet->AddHeader (tHeader);
      SendPacketViaRawSocket (/*packet*/packet, /*pair<Ptr<Socket> , Ipv4InterfaceAddress>*/ *j, /*dst*/iface.GetBroadcast (), /*TTL*/ ttl, /*id*/0);
    }
  ScheduleRreqRetry (dst, ttl);
  if (EnableHello)
    {
      htimer.Cancel ();
      htimer.Schedule (HelloInterval);
    }
}

void
RoutingProtocol::ScheduleRreqRetry (Ipv4Address dst,  uint16_t ttl)
{
  if (m_addressReqTimer.find (dst) == m_addressReqTimer.end ())
     {
       Timer timer (Timer::CANCEL_ON_DESTROY);
       m_addressReqTimer[dst] = timer;
     }
   m_addressReqTimer[dst].SetFunction (&RoutingProtocol::RouteRequestTimerExpire, this);
   m_addressReqTimer[dst].Cancel ();
   m_addressReqTimer[dst].SetArguments (dst, ttl);
   RoutingTableEntry rt;
   m_routingTable.LookupRoute (dst, rt);
   if (ttl == NetDiameter)
     {
       rt.IncrementRreqCnt ();
       m_routingTable.Update (rt);
       m_addressReqTimer[dst].Schedule (Scalar (rt.GetRreqCnt ()) * NetTraversalTime);
     }
   else
     m_addressReqTimer[dst].Schedule (Scalar (2) * NodeTraversalTime * Scalar (ttl + TimeoutBuffer));
}


void
RoutingProtocol::RecvAodv (Ptr<Socket> socket )
{
  NS_LOG_FUNCTION (this);
  Address sourceAddress;
  Ptr<Packet> packet = socket->RecvFrom (sourceAddress);

  Ipv4Header ipv4Header;
  packet->RemoveHeader (ipv4Header);

  UdpHeader udpHeader;
  packet->RemoveHeader (udpHeader);
  NS_ASSERT (udpHeader.GetDestinationPort () == AODV_PORT);
  NS_ASSERT (udpHeader.GetSourcePort () == AODV_PORT);

  Ipv4Address receiver = m_socketAddresses[socket].GetLocal ();
  Ipv4Address sender = ipv4Header.GetSource ();
  NS_LOG_DEBUG ("AODV node " << this << " received a AODV packet from " << sender << " to " << receiver);

  UpdateRouteToNeighbor (sender, receiver);
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
        RecvRequest (packet, receiver, sender, ipv4Header);
        break;
      }
    case AODVTYPE_RREP:
      {
        RecvReply (packet, receiver, sender, ipv4Header);
        break;
      }
    case AODVTYPE_RERR:
      {
        RecvError (packet, sender);
        break;
      }
    case AODVTYPE_RREP_ACK:
      {
        RecvReplyAck (sender);
        break;
      }
    }
}

bool
RoutingProtocol::UpdateRouteLifeTime (Ipv4Address addr, Time lifetime )
{
  RoutingTableEntry rt;
  if (m_routingTable.LookupRoute (addr, rt))
    {
      rt.SetFlag (RTF_UP);
      rt.SetRreqCnt (0);
      rt.SetLifeTime (std::max (lifetime, rt.GetLifeTime ()));
      m_routingTable.Update (rt);
      return true;
    }
  return false;
}

void
RoutingProtocol::UpdateRouteToNeighbor (Ipv4Address sender, Ipv4Address receiver )
{
  NS_LOG_FUNCTION (this << "sender " << sender << " receiver " << receiver );
  RoutingTableEntry toNeighbor;
  if (!m_routingTable.LookupRoute (sender, toNeighbor))
    {
      Ptr<NetDevice> dev = m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (receiver));
      RoutingTableEntry newEntry (/*device=*/dev, /*dst=*/sender, /*know seqno=*/false, /*seqno=*/0,
                                  /*iface=*/m_ipv4->GetAddress (m_ipv4->GetInterfaceForAddress (receiver), 0),
                                  /*hops=*/1, /*next hop=*/sender, /*lifetime=*/ActiveRouteTimeout);
      m_routingTable.AddRoute (newEntry);
    }
  else
    {
      toNeighbor.SetFlag (RTF_UP);
      toNeighbor.SetLifeTime (std::max (ActiveRouteTimeout, toNeighbor.GetLifeTime ()));
      m_routingTable.Update (toNeighbor);
    }
}

void
RoutingProtocol::RecvRequest (Ptr<Packet> p, Ipv4Address receiver, Ipv4Address src, Ipv4Header ipv4Header )
{
  NS_LOG_FUNCTION (this);
  RreqHeader rreqHeader;
  p->RemoveHeader (rreqHeader); // TODO check that header correctly found

  // A node ignores all RREQs received from any node in its blacklist
  RoutingTableEntry toPrev;
  if (m_routingTable.LookupRoute (src, toPrev))
    {
      if (toPrev.IsUnidirectional ())
        return;
    }

  uint32_t id = rreqHeader.GetId ();
  Ipv4Address origin = rreqHeader.GetOrigin ();

  /*
   *  Node checks to determine whether it has received a RREQ with the same Originator IP Address and RREQ ID.
   *  If such a RREQ has been received, the node silently discards the newly received RREQ.
   */
  if (m_idCache.LookupId (origin, id))
    {
      return;
    }
  m_idCache.InsertId (origin, id, PathDiscoveryTime);

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
   *     MinimalLifetime = current time + 2*NetTraversalTime - 2*HopCount*NodeTraversalTime
   */
  RoutingTableEntry toOrigin;
  if (!m_routingTable.LookupRoute (origin, toOrigin))
    {
      Ptr<NetDevice> dev = m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (receiver));
      RoutingTableEntry newEntry (/*device=*/dev, /*dst=*/origin, /*validSeno=*/true, /*seqNo=*/rreqHeader.GetOriginSeqno (),
                                  /*iface=*/m_ipv4->GetAddress (m_ipv4->GetInterfaceForAddress (receiver), 0), /*hops=*/hop,
                                  /*nextHop*/src, /*timeLife=*/Scalar (2) * NetTraversalTime - Scalar (2 * hop) * NodeTraversalTime);
      m_routingTable.AddRoute (newEntry);
    }
  else
    {
      if (toOrigin.GetValidSeqNo ())
        {
          if (int32_t (rreqHeader.GetOriginSeqno ()) - int32_t (toOrigin.GetSeqNo ()) > 0)
            toOrigin.SetSeqNo (rreqHeader.GetOriginSeqno ());
        }
      else
        toOrigin.SetSeqNo (rreqHeader.GetOriginSeqno ());
      toOrigin.SetValidSeqNo (true);
      toOrigin.SetNextHop (src);
      toOrigin.SetOutputDevice (m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (receiver)));
      toOrigin.SetInterface (m_ipv4->GetAddress (m_ipv4->GetInterfaceForAddress (receiver), 0));
      toOrigin.SetHop (hop);
      toOrigin.SetLifeTime (std::max (Scalar (2) * NetTraversalTime - Scalar (2 * hop) * NodeTraversalTime, toOrigin.GetLifeTime ()));
      m_routingTable.Update (toOrigin);
    }

  //  A node generates a RREP if either:
  //  (i)  it is itself the destination,
  if (IsMyOwnAddress (rreqHeader.GetDst ()))
    {
      m_routingTable.LookupRoute (origin, toOrigin);
      SendReply (rreqHeader, toOrigin);
      return;
    }
  /*
   * (ii) or it has an active route to the destination, the destination sequence number in the node's existing route table entry for the destination
   *      is valid and greater than or equal to the Destination Sequence Number of the RREQ, and the "destination only" flag is NOT set.
   */
  RoutingTableEntry toDst;
  Ipv4Address dst = rreqHeader.GetDst ();
  NS_LOG_LOGIC (receiver << " recieve RREQ to destination " << dst << " with ttl " << (uint16_t) ipv4Header.GetTtl ());
  if (m_routingTable.LookupRoute (dst, toDst))
    {
      /*
       * The Destination Sequence number for the requested destination is set to the maximum of the corresponding value
       * received in the RREQ message, and the destination sequence value currently maintained by the node for the requested destination.
       * However, the forwarding node MUST NOT modify its maintained value for the destination sequence number, even if the value
       * received in the incoming RREQ is larger than the value currently maintained by the forwarding node.
       */
      if (rreqHeader.GetUnknownSeqno () || ( (int32_t (toDst.GetSeqNo ()) - int32_t (rreqHeader.GetDstSeqno ()) > 0) && toDst.GetValidSeqNo () ))
        {
          if (!rreqHeader.GetDestinationOnly ())
            {
              SendReplyByIntermediateNode (toDst, toOrigin, rreqHeader.GetGratiousRrep ());
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
  if (ipv4Header.GetTtl () < 1)
    return;

  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
    {
      Ptr<Socket> socket = j->first;
      Ipv4InterfaceAddress iface = j->second;
      Ptr<Packet> packet = p->Copy ();
      packet->AddHeader (rreqHeader);
      TypeHeader tHeader (AODVTYPE_RREQ);
      packet->AddHeader (tHeader);
      SendPacketViaRawSocket (/*packet*/packet, /*pair<Ptr<Socket> , Ipv4InterfaceAddress>*/ *j, /*dst*/iface.GetBroadcast (),
                               /*TTL*/ ipv4Header.GetTtl () - 1, /*id*/ipv4Header.GetIdentification ());
    }

  if (EnableHello)
    {
      htimer.Cancel ();
      htimer.Schedule (HelloInterval);
    }
  NS_LOG_LOGIC ("After recieve request");
  m_routingTable.Print (std::cout);
}

void
RoutingProtocol::SendReply (RreqHeader const & rreqHeader, RoutingTableEntry const & toOrigin )
{
  NS_LOG_FUNCTION (this << toOrigin.GetDestination ());
  /*
   * Destination node MUST increment its own sequence number by one if the sequence number in the RREQ packet is equal to that
   * incremented value. Otherwise, the destination does not change its sequence number before generating the  RREP message.
   */
  if (!rreqHeader.GetUnknownSeqno () && (rreqHeader.GetDstSeqno () == m_seqNo + 1))
    m_seqNo++;
  RrepHeader rrepHeader ( /*prefixSize=*/0, /*hops=*/toOrigin.GetHop (), /*dst=*/rreqHeader.GetDst (),
  /*dstSeqNo=*/m_seqNo, /*origin=*/toOrigin.GetDestination (), /*lifeTime=*/MyRouteTimeout);
  if (0) // TODO when
    {
      rrepHeader.SetAckRequired (true);
      RoutingTableEntry toNextHop;
      m_routingTable.LookupRoute (toOrigin.GetNextHop (), toNextHop);
      toNextHop.m_ackTimer.SetFunction (&RoutingProtocol::AckTimerExpire, this);
      toNextHop.m_ackTimer.SetArguments (toNextHop.GetDestination (), BlackListTimeout);
      toNextHop.m_ackTimer.SetDelay (NextHopWait);
    }
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (rrepHeader);
  TypeHeader tHeader (AODVTYPE_RREP);
  packet->AddHeader (tHeader);
  Ptr<Socket> socket = FindSocketWithInterfaceAddress (toOrigin.GetInterface ());
  NS_ASSERT (socket);
  SendPacketViaRawSocket (/*packet*/packet, /*pair<Ptr<Socket> , Ipv4InterfaceAddress>*/ std::make_pair(socket, toOrigin.GetInterface ()),
                           /*dst*/toOrigin.GetNextHop (), /*TTL*/ toOrigin.GetHop (), /*id*/0);
}

void
RoutingProtocol::SendReplyByIntermediateNode (RoutingTableEntry & toDst, RoutingTableEntry & toOrigin, bool gratRep )
{

  RrepHeader rrepHeader (/*prefix size=*/0, /*hops=*/toDst.GetHop (), /*dst=*/toDst.GetDestination (), /*dst seqno=*/toDst.GetSeqNo (),
  /*origin=*/toOrigin.GetDestination (), /*lifetime=*/toDst.GetLifeTime ());

  toDst.InsertPrecursor (toOrigin.GetNextHop ());
  toOrigin.InsertPrecursor (toDst.GetNextHop ());
  m_routingTable.Update (toDst);
  m_routingTable.Update (toOrigin);

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (rrepHeader);
  TypeHeader tHeader (AODVTYPE_RREP);
  packet->AddHeader (tHeader);
  Ptr<Socket> socket = FindSocketWithInterfaceAddress (toOrigin.GetInterface ());
  NS_ASSERT (socket);
  SendPacketViaRawSocket (/*packet*/packet, /*pair<Ptr<Socket> , Ipv4InterfaceAddress>*/ std::make_pair(socket, toOrigin.GetInterface ()),
                           /*dst*/toOrigin.GetNextHop (), /*TTL*/ toOrigin.GetHop (), /*id*/0);

  // Generating gratuitous RREPs
  if (gratRep)
    {
      RrepHeader gratRepHeader (/*prefix size=*/0, /*hops=*/toOrigin.GetHop (), /*dst=*/toOrigin.GetDestination (), /*dst seqno=*/
          toOrigin.GetSeqNo (),
          /*origin=*/toDst.GetDestination (), /*lifetime=*/toOrigin.GetLifeTime ());
      Ptr<Packet> packetToDst = Create<Packet> ();
      packetToDst->AddHeader (rrepHeader);
      packetToDst->AddHeader (tHeader);
      Ptr<Socket> socket = FindSocketWithInterfaceAddress (toDst.GetInterface ());
      NS_ASSERT (socket);
      SendPacketViaRawSocket (/*packet*/packet, /*pair<Ptr<Socket> , Ipv4InterfaceAddress>*/ std::make_pair(socket, toDst.GetInterface ()),
                               /*dst*/toDst.GetNextHop (), /*TTL*/ toDst.GetHop (), /*id*/0);
    }
}

void
RoutingProtocol::SendReplyAck (Ipv4Address neighbor )
{
  NS_LOG_FUNCTION(this);
  RrepAckHeader h;
  TypeHeader typeHeader (AODVTYPE_RREP_ACK);
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (h);
  packet->AddHeader (typeHeader);
  RoutingTableEntry toNeighbor;
  m_routingTable.LookupRoute (neighbor, toNeighbor);
  Ptr<Socket> socket = FindSocketWithInterfaceAddress (toNeighbor.GetInterface ());
  NS_ASSERT (socket);
  SendPacketViaRawSocket (/*packet*/packet, /*pair<Ptr<Socket> , Ipv4InterfaceAddress>*/ std::make_pair(socket, toNeighbor.GetInterface ()),
                           /*dst*/neighbor, /*TTL*/ 1, /*id*/0);
}

void
RoutingProtocol::RecvReply (Ptr<Packet> p, Ipv4Address receiver, Ipv4Address sender, Ipv4Header ipv4Header )
{
  NS_LOG_FUNCTION(this << " src " << sender);
  RrepHeader rrepHeader;
  p->RemoveHeader (rrepHeader);
  Ipv4Address dst = rrepHeader.GetDst ();
  NS_LOG_LOGIC("RREP destination " << dst << " RREP origin " << rrepHeader.GetOrigin());

  uint8_t hop = rrepHeader.GetHopCount () + 1;
  rrepHeader.SetHopCount (hop);

  // If RREP is Hello message
  if (dst == rrepHeader.GetOrigin ())
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
  RoutingTableEntry newEntry (/*device=*/dev, /*dst=*/dst, /*validSeqNo=*/true, /*seqno=*/rrepHeader.GetDstSeqno (),
                              /*iface=*/m_ipv4->GetAddress (m_ipv4->GetInterfaceForAddress (receiver), 0),/*hop=*/hop,
                              /*nextHop=*/sender, /*lifeTime=*/rrepHeader.GetLifeTime ());
  RoutingTableEntry toDst;
  if (m_routingTable.LookupRoute (dst, toDst))
    {
      /*
       * The existing entry is updated only in the following circumstances:
       * (i) the sequence number in the routing table is marked as invalid in route table entry.
       */
      if (!toDst.GetValidSeqNo ())
        {
          m_routingTable.Update (newEntry);
        }
      // (ii)the Destination Sequence Number in the RREP is greater than the node's copy of the destination sequence number and the known value is valid,
      else if ((int32_t (rrepHeader.GetDstSeqno ()) - int32_t (toDst.GetSeqNo ())) > 0)
        {
          m_routingTable.Update (newEntry);
        }
      else
        {
          // (iii) the sequence numbers are the same, but the route is marked as inactive.
          if ((rrepHeader.GetDstSeqno () == toDst.GetSeqNo ()) && (toDst.GetFlag () != RTF_UP))
            {
              m_routingTable.Update (newEntry);
            }
          // (iv)  the sequence numbers are the same, and the New Hop Count is smaller than the hop count in route table entry.
          else if ((rrepHeader.GetDstSeqno () == toDst.GetSeqNo ()) && (hop < toDst.GetHop ()))
            {
              m_routingTable.Update (newEntry);
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
  if (rrepHeader.GetAckRequired ())
    SendReplyAck (sender);
  NS_LOG_LOGIC ("receiver " << receiver << " origin " << rrepHeader.GetOrigin ());
  if (IsMyOwnAddress (rrepHeader.GetOrigin ()))
    {
      if (toDst.GetFlag () == RTF_IN_SEARCH)
        {
          m_routingTable.Update (newEntry);
          m_addressReqTimer[dst].Cancel ();
          m_addressReqTimer.erase (dst);
        }
      SendPacketFromQueue (rrepHeader.GetDst (), newEntry.GetRoute ());
      return;
    }

  RoutingTableEntry toOrigin;
  if (!m_routingTable.LookupRoute (rrepHeader.GetOrigin (), toOrigin))
    {
      return; // Impossible! drop.
    }
  toOrigin.SetLifeTime (std::max (ActiveRouteTimeout, toOrigin.GetLifeTime ()));
  m_routingTable.Update (toOrigin);

  // Update information about precursors
  m_routingTable.LookupRoute (rrepHeader.GetDst (), toDst);
  toDst.InsertPrecursor (toOrigin.GetNextHop ());
  m_routingTable.Update (toDst);
  RoutingTableEntry toNextHopToDst;
  m_routingTable.LookupRoute (toDst.GetNextHop (), toNextHopToDst);
  toNextHopToDst.InsertPrecursor (toOrigin.GetNextHop ());
  m_routingTable.Update (toNextHopToDst);

  // TODO add operation over unidirectional links
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (rrepHeader);
  TypeHeader tHeader (AODVTYPE_RREP);
  packet->AddHeader (tHeader);
  Ptr<Socket> socket = FindSocketWithInterfaceAddress (toOrigin.GetInterface ());
  NS_ASSERT (socket);
  SendPacketViaRawSocket (/*packet*/packet, /*pair<Ptr<Socket> , Ipv4InterfaceAddress>*/ std::make_pair(socket, toOrigin.GetInterface ()),
                           /*dst*/toOrigin.GetNextHop (), /*TTL*/ ipv4Header.GetTtl () - 1, /*id*/0);
}

void
RoutingProtocol::RecvReplyAck (Ipv4Address neighbor )
{
  NS_LOG_LOGIC(this);
  // TODO
}

// TODO may be used for determining connectivity
void
RoutingProtocol::ProcessHello (RrepHeader const & rrepHeader, Ipv4Address receiver )
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
      Ptr<NetDevice> dev = m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (receiver));
      RoutingTableEntry newEntry (/*device=*/dev, /*dst=*/rrepHeader.GetDst (), /*validSeqNo=*/true, /*seqno=*/rrepHeader.GetDstSeqno (),
      /*iface=*/m_ipv4->GetAddress (m_ipv4->GetInterfaceForAddress (receiver), 0),
      /*hop=*/rrepHeader.GetHopCount (), /*nextHop=*/rrepHeader.GetDst (), /*lifeTime=*/rrepHeader.GetLifeTime ());
      m_routingTable.AddRoute (newEntry);
    }
  else
    {
      toNeighbor.SetLifeTime (std::max (Scalar (AllowedHelloLoss) * HelloInterval, toNeighbor.GetLifeTime ()));
      toNeighbor.SetSeqNo (rrepHeader.GetDstSeqno ());
      toNeighbor.SetValidSeqNo (true);
      toNeighbor.SetFlag (RTF_UP);
      toNeighbor.SetOutputDevice (m_ipv4->GetNetDevice (m_ipv4->GetInterfaceForAddress (receiver)));
      toNeighbor.SetInterface (m_ipv4->GetAddress (m_ipv4->GetInterfaceForAddress (receiver), 0));
      m_routingTable.Update (toNeighbor);
    }
  m_nb.Update (rrepHeader.GetDst (), Scalar (AllowedHelloLoss) * HelloInterval);
}

// TODO process RERR with 'N' flag
void
RoutingProtocol::RecvError (Ptr<Packet> p, Ipv4Address src )
{
  RerrHeader rerrHeader;
  p->RemoveHeader (rerrHeader);
  std::map<Ipv4Address, uint32_t> dstWithNextHopSrc;
  std::map<Ipv4Address, uint32_t> unreachable;
  m_routingTable.GetListOfDestinationWithNextHop (src, dstWithNextHopSrc);
  std::pair<Ipv4Address, uint32_t> un;
  while (rerrHeader.RemoveUnDestination (un))
    {
      if (m_nb.IsNeighbor (un.first))
        SendRerrWhenBreaksLinkToNextHop (un.first);
      else
        {
          for (std::map<Ipv4Address, uint32_t>::const_iterator i = dstWithNextHopSrc.begin (); i != dstWithNextHopSrc.end (); ++i)
            if (i->first == un.first)
              unreachable.insert (un);
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
          rerrHeader.Clear ();
        }
      else
        {
          RoutingTableEntry toDst;
          m_routingTable.LookupRoute (i->first, toDst);
          toDst.GetPrecursors (precursors);
          ++i;
        }
    }
  m_routingTable.InvalidateRoutesWithDst (unreachable);
}

void
RoutingProtocol::RouteRequestTimerExpire (Ipv4Address dst, uint16_t lastTtl )
{
  NS_LOG_LOGIC(this);
  RoutingTableEntry toDst;
  m_routingTable.LookupRoute (dst, toDst);
  if (toDst.GetFlag () == RTF_UP)
    {
      SendPacketFromQueue (dst, toDst.GetRoute ());
      NS_LOG_LOGIC ("route to " << dst << " found");
      return;
    }
  /**
   *  If a route discovery has been attempted RreqRetries times at the maximum TTL without
   *  receiving any RREP, all data packets destined for the corresponding destination SHOULD be
   *  dropped from the buffer and a Destination Unreachable message SHOULD be delivered to the application.
   */
  if (toDst.GetRreqCnt () == RreqRetries)
    {
      NS_LOG_LOGIC("route discovery to " << dst << " has been attempted RreqRetries times");
      m_addressReqTimer.erase(dst);
      NS_LOG_LOGIC ("Route not found. Drop packet with dst " << dst);
      DropFromQueue (dst);
      // TODO drop packet from queue and deliver Destination Unreachable message to the application.
      return;
    }

  if (toDst.GetFlag () == RTF_IN_SEARCH)
    {

      if (lastTtl + TtlIncrement > TtlThreshold)
        {
          NS_LOG_LOGIC ("Send new RREQ to " << dst << " ttl " << NetDiameter);
          SendRequest (dst, NetDiameter);
        }
      else
        {
          NS_LOG_LOGIC ("Send new RREQ to " << dst << " ttl " << lastTtl + TtlIncrement);
          SendRequest (dst, lastTtl + TtlIncrement);
        }
    }
  else
    {
      NS_LOG_LOGIC ("Route down. Stop search. Drop packet with destination " << dst);
      m_addressReqTimer.erase(dst);
      DropFromQueue (dst);
    }
}

void
RoutingProtocol::HelloTimerExpire ()
{
  NS_LOG_FUNCTION(this);
  SendHello ();
  // TODO select random time for the next hello
  htimer.Cancel ();
  htimer.Schedule (HelloInterval + MilliSeconds(UniformVariable().GetValue (0, 1)) );
}

void
RoutingProtocol::LocalRepairTimerExpire ()
{
  // TODO start local repair procedure
}

void
RoutingProtocol::AckTimerExpire (Ipv4Address neighbor, Time blacklistTimeout )
{
  NS_LOG_FUNCTION(this);
  m_routingTable.MarkLinkAsUinidirectional (neighbor, blacklistTimeout);
}

void
RoutingProtocol::RoutingTableEntryTimerExpire (Ipv4Address dst )
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
   *   Lifetime                       AllowedHelloLoss * HelloInterval
   */
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
    {
      Ptr<Socket> socket = j->first;
      Ipv4InterfaceAddress iface = j->second;
      RrepHeader helloHeader (/*prefix size=*/0, /*hops=*/0, /*dst=*/iface.GetLocal (), /*dst seqno=*/m_seqNo,
      /*origin=*/iface.GetLocal (),/*lifetime=*/Scalar (AllowedHelloLoss) * HelloInterval);
      Ptr<Packet> packet = Create<Packet> ();
      packet->AddHeader (helloHeader);
      TypeHeader tHeader (AODVTYPE_RREP);
      packet->AddHeader (tHeader);
      SendPacketViaRawSocket (/*packet*/packet, /*pair<Ptr<Socket> , Ipv4InterfaceAddress>*/ *j,
                               /*dst*/iface.GetBroadcast (), /*TTL*/ 1, /*id*/0);
    }
}

void
RoutingProtocol::SendPacketViaRawSocket (Ptr<Packet> packet, std::pair<Ptr<Socket> , Ipv4InterfaceAddress> socketAddress, Ipv4Address dst,
    uint16_t ttl, uint16_t id )
{
  UdpHeader udpHeader;
  udpHeader.SetDestinationPort (AODV_PORT);
  udpHeader.SetSourcePort (AODV_PORT);
  packet->AddHeader (udpHeader);

  Ipv4Header ipv4Header;
  ipv4Header.SetSource (socketAddress.second.GetLocal ());
  ipv4Header.SetDestination (dst);
  ipv4Header.SetIdentification (id);
  ipv4Header.EnableChecksum ();
  ipv4Header.SetProtocol (UdpL4Protocol::PROT_NUMBER);
  ipv4Header.SetTtl (ttl);
  ipv4Header.SetPayloadSize (packet->GetSize ());
  packet->AddHeader (ipv4Header);
  NS_LOG_LOGIC ("Send packet " << packet->GetUid() << " from " << socketAddress.second.GetLocal () << " to " << dst);
  socketAddress.first->SendTo (packet, 0, InetSocketAddress (dst, AODV_PORT));
}


void
RoutingProtocol::SendPacketFromQueue (Ipv4Address dst, Ptr<Ipv4Route> route )
{
  NS_LOG_FUNCTION(this);
  QueueEntry queueEntry;
  while (m_queue.Dequeue (dst, queueEntry))
    {
      NS_LOG_LOGIC (Simulator::Now ().GetSeconds () << "  SendPacketFromQueue");
      UnicastForwardCallback ucb = queueEntry.GetUnicastForwardCallback ();
      ucb (route, queueEntry.GetPacket (), queueEntry.GetIpv4Header ());
    }
}
void
RoutingProtocol::DropFromQueue (Ipv4Address dst)
{
  NS_LOG_FUNCTION(this);
  m_queue.DropPacketWithDst(dst);
}

void
RoutingProtocol::Send (Ptr<Ipv4Route> route, Ptr<const Packet> packet, const Ipv4Header & header )
{
  NS_LOG_FUNCTION(this << packet->GetUid() << (uint16_t) header.GetProtocol());
  Ptr<Ipv4L3Protocol> l3 = m_ipv4->GetObject<Ipv4L3Protocol> ();
  NS_ASSERT(l3 != 0);
  Ptr<Packet> p = packet->Copy ();
  // TODO know protocol number
  l3->Send (p, route->GetSource (), header.GetDestination (), 1, route);
}

void
RoutingProtocol::SendRerrWhenBreaksLinkToNextHop (Ipv4Address nextHop )
{
  RerrHeader rerrHeader;
  std::vector<Ipv4Address> precursors;
  std::map<Ipv4Address, uint32_t> unreachable;

  RoutingTableEntry toNextHop;
  if (!m_routingTable.LookupRoute (nextHop, toNextHop))
    return;
  toNextHop.GetPrecursors (precursors);
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
          rerrHeader.Clear ();
        }
      else
        {
          RoutingTableEntry toDst;
          m_routingTable.LookupRoute (i->first, toDst);
          toDst.GetPrecursors (precursors);
          ++i;
        }
    }
  unreachable.insert (std::make_pair (nextHop, toNextHop.GetSeqNo ()));
  m_routingTable.InvalidateRoutesWithDst (unreachable);
}

void
RoutingProtocol::SendRerrMessage (Ptr<Packet> packet, std::vector<Ipv4Address> precursors )
{
  if (precursors.empty ())
    return; // TODO too many unreachable destinations, but no precursors
  // If there is only one precursor, RERR SHOULD be unicast toward that precursor
  if (precursors.size () == 1)
    {
      RoutingTableEntry toPrecursor;
      m_routingTable.LookupRoute (precursors.front (), toPrecursor);
      Ptr<Socket> socket = FindSocketWithInterfaceAddress (toPrecursor.GetInterface ());
      NS_ASSERT (socket);
      SendPacketViaRawSocket (/*packet*/packet, /*pair<Ptr<Socket> , Ipv4InterfaceAddress>*/ std::make_pair(socket, toPrecursor.GetInterface ()),
                               /*dst*/precursors.front (), /*TTL*/ 1, /*id*/0);
      return;
    }

  //  Should only transmit RERR on those interfaces which have precursor nodes for the broken route
  std::vector<Ipv4InterfaceAddress> ifaces;
  RoutingTableEntry toPrecursor;
  for (std::vector<Ipv4Address>::const_iterator i = precursors.begin (); i != precursors.end (); ++i)
    {
      if (!m_routingTable.LookupRoute (*i, toPrecursor))
        break;
      bool result = true;
      for (std::vector<Ipv4InterfaceAddress>::const_iterator i = ifaces.begin (); i != ifaces.end (); ++i)
        if (*i == toPrecursor.GetInterface ())
          {
            result = false;
            break;
          }
      if (result)
        ifaces.push_back (toPrecursor.GetInterface ());
    }

  for (std::vector<Ipv4InterfaceAddress>::const_iterator i = ifaces.begin (); i != ifaces.end (); ++i)
    {
      Ptr<Socket> socket = FindSocketWithInterfaceAddress (*i);
      NS_ASSERT (socket);
      SendPacketViaRawSocket (/*packet*/packet, /*pair<Ptr<Socket> , Ipv4InterfaceAddress>*/ std::make_pair(socket, toPrecursor.GetInterface ()),
                               /*dst*/m_socketAddresses[socket].GetBroadcast (), /*TTL*/ 1, /*id*/0);
    }

}

Ptr<Socket>
RoutingProtocol::FindSocketWithInterfaceAddress (Ipv4InterfaceAddress addr ) const
{
  for (std::map<Ptr<Socket> , Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin (); j != m_socketAddresses.end (); ++j)
    {
      Ptr<Socket> socket = j->first;
      Ipv4InterfaceAddress iface = j->second;
      if (iface == addr)
        return socket;
    }
  Ptr<Socket> socket;
  return socket;
}


void
RoutingProtocol::LocalRouteRepair (Ipv4Address dst, Ipv4Address origin )
{
  RoutingTableEntry toDst;
  RoutingTableEntry toOrigin;
  if (!m_routingTable.LookupRoute (dst, toDst))
    return;
  if (!m_routingTable.LookupRoute (origin, toOrigin))
    return;
  uint16_t ttl = std::max (toOrigin.GetHop () * 0.5, (double) toDst.GetHop ()) + LocalAddTtl;
  ttl++;

}

void
RoutingProtocol::HandleLinkFailure (Ipv4Address addr )
{
  NS_LOG_FUNCTION(this << addr);
  SendRerrWhenBreaksLinkToNextHop (addr);
  // TODO
}

}
}
