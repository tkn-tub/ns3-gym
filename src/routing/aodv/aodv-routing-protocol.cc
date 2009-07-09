/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 1997, 1998 Carnegie Mellon University.
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
 * Authors: The AODV code developed by the CMU/MONARCH group was optimized and
 * tuned by Samir Das and Mahesh Marina, University of Cincinnati.
 * The work was partially done in Sun Microsystems.
 *
 * Ported to ns-3 by Elena Borovkova <borovkovaes@iitp.ru>
 */
#include "aodv-routing-protocol.h"

#include "ns3/socket-factory.h"
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
  if(LookupBroadcastId(id, bid) )
    return;
  struct BroadcastId broadcastId = {id, bid,BCAST_ID_SAVE + Simulator::Now()};
  bi.push_back(broadcastId);
}
bool
RoutingProtocol::LookupBroadcastId (Ipv4Address id, uint32_t bid)
{
  std::vector<BroadcastId>::const_iterator i;
  for(i = bi.begin(); i != bi.end(); ++i)
    if(i->src == id && i->id == bid)
      return true;
  return false;
}


void
RoutingProtocol::PurgeBroadcastId ()
{
  std::vector<BroadcastId>::iterator i = remove_if(bi.begin(), bi.end(), IsExpired());
  bi.erase(i, bi.end());
}

RoutingProtocol::RoutingProtocol() : 
  ALLOWED_HELLO_LOSS (2),
  BAD_LINK_LIFETIME (Seconds (3)),
  FREQUENCY (Seconds (0.5)),
  bid(1), seqno(2/*??*/), 
  btimer(Timer::REMOVE_ON_DESTROY),
  htimer(Timer::REMOVE_ON_DESTROY),
  ntimer(Timer::REMOVE_ON_DESTROY),
  rtimer(Timer::REMOVE_ON_DESTROY),
  lrtimer(Timer::REMOVE_ON_DESTROY)

{
  MaxHelloInterval = Scalar(1.25) * HELLO_INTERVAL;
  MinHelloInterval = Scalar(0.75) * HELLO_INTERVAL;
}

TypeId 
RoutingProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::aodv::RoutingProtocol")
      .SetParent<Ipv4RoutingProtocol> ()
      .AddConstructor<RoutingProtocol> ()
      .AddAttribute ("HelloInterval", "HELLO messages emission interval.",
          TimeValue (Seconds (1)),
          MakeTimeAccessor (&RoutingProtocol::HELLO_INTERVAL),
          MakeTimeChecker ())
      .AddAttribute ("Broadcast id save", "Broadcast id save interval.",
          TimeValue (Seconds (6)),
          MakeTimeAccessor (&RoutingProtocol::BCAST_ID_SAVE),
          MakeTimeChecker ())

            ;
  return tid;
}

RoutingProtocol::~RoutingProtocol()
{
}

void
RoutingProtocol::DoDispose ()
{
  m_ipv4 = 0;
  for (std::map< Ptr<Socket>, Ipv4Address >::iterator iter = m_socketAddresses.begin ();
       iter != m_socketAddresses.end (); iter++)
    {
      iter->first->Close ();
    }
  m_socketAddresses.clear ();
  Ipv4RoutingProtocol::DoDispose ();
}

Ptr<Ipv4Route> 
RoutingProtocol::RouteOutput (Ptr<Packet> p, const Ipv4Header &header, uint32_t oif, Socket::SocketErrno &sockerr)
{
  Ptr<Ipv4Route> rtentry;
  sockerr = Socket::ERROR_NOROUTETOHOST;
  // TODO resolve route
  return rtentry;
}

bool
RoutingProtocol::RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                           UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                           LocalDeliverCallback lcb, ErrorCallback ecb)
{
  // TODO
  return false;
}

void 
RoutingProtocol::SetIpv4 (Ptr<Ipv4> ipv4)
{
  NS_ASSERT (ipv4 != 0);
  NS_ASSERT (m_ipv4 == 0);
  
  btimer.SetFunction (& RoutingProtocol::BroadcastTimerExpire, this);
  ntimer.SetFunction (& RoutingProtocol::NeighborTimerExpire, this);
  rtimer.SetFunction (& RoutingProtocol::RouteCacheTimerExpire, this);
  lrtimer.SetFunction (& RoutingProtocol::LocalRepairTimerExpire, this);
  htimer.SetFunction (& RoutingProtocol::LocalRepairTimerExpire, this);

  m_ipv4 = ipv4;
  Simulator::ScheduleNow (&RoutingProtocol::Start, this);
}

void
RoutingProtocol::Start ()
{
  // Open UDP sockets for control traffic on each IP interface
  const Ipv4Address loopback ("127.0.0.1");
  for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i++)
    {
      Ipv4Address addr = m_ipv4->GetAddress (i, 0).GetLocal ();
      if (addr == loopback)
        continue;

      // Create a socket to listen only on this interface
      Ptr<Socket> socket = Socket::CreateSocket (GetObject<Node> (), 
        UdpSocketFactory::GetTypeId()); 
      socket->SetRecvCallback (MakeCallback (&RoutingProtocol::RecvAodv,  this));
      if (socket->Bind (InetSocketAddress (addr, AODV_PORT)))
        {
          NS_FATAL_ERROR ("Failed to bind() AODV receive socket");
        }
      socket->Connect (InetSocketAddress (Ipv4Address (0xffffffff), AODV_PORT));
      m_socketAddresses.insert(std::make_pair(socket, addr));
    }
}

void 
RoutingProtocol::NotifyInterfaceUp (uint32_t interface)
{
  // TODO
}

void 
RoutingProtocol::NotifyInterfaceDown (uint32_t interface)
{ 
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

void 
RoutingProtocol::RecvAodv (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address sourceAddress;
  packet = socket->RecvFrom (sourceAddress);

  InetSocketAddress inetSourceAddr = InetSocketAddress::ConvertFrom (sourceAddress);
  Ipv4Address senderIfaceAddr = inetSourceAddr.GetIpv4 ();
  Ipv4Address receiverIfaceAddr = m_socketAddresses[socket];
  
  NS_ASSERT (receiverIfaceAddr != Ipv4Address ());
  NS_LOG_DEBUG ("AODV node " << this << " received a AODV packet from " << senderIfaceAddr << " to " << receiverIfaceAddr);
  NS_ASSERT (inetSourceAddr.GetPort () == AODV_PORT);
  
  // TODO check packet type and call RecvSmth
}

void 
RoutingProtocol::RecvRequest (Ptr<Packet> p)
{
  Ipv4Header ipv4Header;
  p->RemoveHeader(ipv4Header);
}

void 
RoutingProtocol::RecvReply (Ptr<Packet> p)
{
  // TODO
}

void 
RoutingProtocol::RecvError (Ptr<Packet> p)
{
  // TODO
}

void 
RoutingProtocol::BroadcastTimerExpire ()
{
  // id_purge();
  btimer.Schedule (BCAST_ID_SAVE);
}

void 
RoutingProtocol::HelloTimerExpire ()
{
  SendHello ();
  // TODO select random time for the next hello
  htimer.Schedule (HELLO_INTERVAL);
}

void 
RoutingProtocol::NeighborTimerExpire ()
{
  // nb_purge();
  ntimer.Schedule (HELLO_INTERVAL);
}

void 
RoutingProtocol::RouteCacheTimerExpire ()
{
  // rt_purge();
  rtimer.Schedule (FREQUENCY);
}

void 
RoutingProtocol::LocalRepairTimerExpire ()
{
  // TODO start local repair procedure
}

void
RoutingProtocol::SendHello ()
{
  // TODO send hello packet from interfaces
}

// TODO add use an expanding ring search technique
void 
RoutingProtocol::SendRequest (Ipv4Address dst, bool G, bool D)
{
  TypeHeader tHeader(AODVTYPE_RREQ);

  Ipv4Header ipv4Header;
  ipv4Header.SetTtl(NETWORK_DIAMETER);
  ipv4Header.SetDestination(dst);
//ipv4Header.SetProtocol( ? );

  RreqHeader h;
  h.SetDst(dst);
  aodv_rt_entry rt;
  if(rtable.rt_lookup(dst, rt))
    h.SetHopCount(rt.GetLastValidHopCount());
  else
    h.SetUnknownSeqno(true);
  if(G)
    h.SetGratiousRrep(true);
  if(D)
    h.SetDestinationOnly(true);
  seqno++;
  h.SetSrcSeqno(seqno);
  bid++;
  h.SetId(bid);
  h.SetHopCount(0);
  Ipv4Address loopback ("127.0.0.1");
  std::map<Ipv4Address, Timer>::const_iterator i;
  for (uint32_t k = 0; k < m_ipv4->GetNInterfaces (); k++)
  {
    Ipv4Address addr = m_ipv4->GetAddress (k, 0).GetLocal ();
    if (addr != loopback)
    {
      ipv4Header.SetSource(addr);
      h.SetSrc(addr);
      InsertBroadcastId(addr, bid);
      Ptr<Packet> packet = Create<Packet> ();
      packet->AddHeader(ipv4Header);
      packet->AddHeader(tHeader);
      packet->AddHeader(h);

//      i = htimer.find(addr);
//      if(i == htimer.end())
//      {
//        Timer t;
//        t.SetDelay(HELLO_INTERVAL);
//        t.Schedule();
//        htimer.insert(std::make_pair(addr, t));
//      }
//      else
//      {
//        i->second.SetDelay(HELLO_INTERVAL);
//        t.Schedule();
//      }

      std::map< Ipv4Address, Ptr<Socket> >::const_iterator j = m_addressSocket.find(addr);
      j->second->Send(packet);

    }
  }
  btimer.SetDelay(BCAST_ID_SAVE);
  btimer.Schedule();
  htimer.SetDelay(HELLO_INTERVAL);
  htimer.Schedule();
}

void 
RoutingProtocol::SendReply (Ipv4Address ipdst, uint32_t hop_count, Ipv4Address rpdst, uint32_t rpseq, Time lifetime)
{
  // TODO
}

void 
RoutingProtocol::SendError (Ipv4Address failed)
{
  // TODO
}

void 
RoutingProtocol::LocalRouteRepair (Ipv4Address dst, Ptr<Packet> p)
{
  // TODO local_rt_repair
}

void 
RoutingProtocol::HandleLinkFailure (Ipv4Address id)
{
  // TODO
}

void
RoutingProtocol::RtPurge ()
{
  // TODO AODV::rt_purge()
}
//-----------------------------------------------------------------------------
// TODO: NeighborList, BroadcastIdCache
//-----------------------------------------------------------------------------
}}
