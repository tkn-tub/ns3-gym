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
#include "src/internet-stack/ipv4-raw-socket-impl.h"


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
  ACTIVE_ROUTE_TIMEOUT(Seconds(3)),
  MY_ROUTE_TIMEOUT(Scalar(2)*ACTIVE_ROUTE_TIMEOUT),
  NET_DIAMETER(35),
  NODE_TRAVERSAL_TIME(MilliSeconds(40)),
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
  NET_TRAVERSAL_TIME = Scalar(2 * NET_DIAMETER) * NODE_TRAVERSAL_TIME;
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
  for (std::map< Ptr<Socket>, Ipv4InterfaceAddress >::iterator iter = m_socketAddresses.begin ();
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
  NS_LOG_FUNCTION (this << p->GetUid() << header.GetDestination());
  Ptr<Ipv4Route> rtentry;
  Ipv4Address dst = header.GetDestination();
  aodv_rt_entry toDst;
  if ( !rtable.rt_lookup(dst, toDst))
  {
    SendRequest(dst, false, false);
    sockerr = Socket::ERROR_NOROUTETOHOST;
  }
  else
  {
    rtentry = Create<Ipv4Route> ();
    rtentry->SetDestination (dst);
    rtentry->SetSource (toDst.GetInterface());
    rtentry->SetGateway (toDst.GetNextHop());
  }
  return rtentry;
}

bool
RoutingProtocol::RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                           UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                           LocalDeliverCallback lcb, ErrorCallback ecb)
{
  NS_LOG_FUNCTION (this << p->GetUid() <<  header.GetDestination() << idev->GetAddress());
  return false;
#if 0
  NS_ASSERT (m_ipv4 != 0);
  // Check if input device supports IP
  NS_ASSERT (m_ipv4->GetInterfaceForDevice (idev) >= 0);
  uint32_t iif = m_ipv4->GetInterfaceForDevice (idev);
  Ipv4Address  dst = header.GetDestination();

  for(std::vector<Ipv4InterfaceAddress>::const_iterator j = m_myAddresses.begin(); j != m_myAddresses.end(); ++j)
  {
    if (j->GetLocal() == dst)
    {
      NS_LOG_LOGIC ("Local delivery of unicast");
      lcb(p,header, iif);
      return true;
    }
    if (j->GetBroadcast() == dst)
    {
      NS_LOG_LOGIC ("Local delivery of broadcast");
      // TODO not duplicate
      lcb(p,header, iif);
      // TODO forward
      return true;
    }
  }
  return false;

  Ptr<Ipv4Route> rtentry;
  aodv_rt_entry toDst;
  if(rtable.rt_lookup(dst, toDst))
  {
    rtentry->SetDestination(dst);
    rtentry->SetGateway(toDst.GetNextHop());
    rtentry->SetSource(toDst.GetInterface());

    uint32_t interfaceIdx = 0;
    rtentry->SetOutputDevice(m_ipv4->GetNetDevice (interfaceIdx));
    ucb (rtentry, p, header);
    return true;
  }
  return false;
#endif
}

void 
RoutingProtocol::SetIpv4 (Ptr<Ipv4> ipv4)
{
  NS_LOG_FUNCTION (this);
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
  NS_LOG_FUNCTION (this);
  // Open UDP sockets for control traffic on each IP interface
  const Ipv4Address loopback ("127.0.0.1");
  for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i++)
    {
      Ipv4InterfaceAddress iface = m_ipv4->GetAddress (i, 0);
      if (iface.GetLocal() == loopback) continue;
      
      // Create a socket to listen only on this interface
      Ptr<Socket> socket = Socket::CreateSocket( GetObject<Node> (), TypeId::LookupByName ("ns3::Ipv4RawSocketFactory"));
      NS_ASSERT (socket != 0);
      socket->SetAttribute ("Protocol", UintegerValue (17)); // UDP
      int status = socket->Bind (InetSocketAddress (iface.GetLocal(), AODV_PORT));
      NS_ASSERT (status != -1);
      socket->SetRecvCallback (MakeCallback (&RoutingProtocol::RecvAodv,  this));
      status = socket->Connect (InetSocketAddress (iface.GetBroadcast(), AODV_PORT));
      NS_ASSERT (status != -1);
      
      m_socketAddresses.insert(std::make_pair(socket, iface));
      NS_LOG_INFO ("Interface " << iface << " used by AODV");
      
      // Add local broadcast record to the routing table
      aodv_rt_entry rt(/*dst=*/iface.GetBroadcast (), 
                       /*know seqno=*/true, /*seqno=*/0,
                       /*iface=*/iface.GetLocal (),
                       /*hops=*/1, 
                       /*next hop=*/iface.GetBroadcast (), 
                       /*lifetime=*/Seconds(1e9)); // TODO use infty
      rtable.rt_add (rt);
    }
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

void 
RoutingProtocol::RecvAodv (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this);
#if 0
  Ptr<Packet> packet;
  Address sourceAddress;
  packet = socket->RecvFrom (sourceAddress);
  Ipv4Header ipv4Header;
  packet->RemoveHeader(ipv4Header);

  InetSocketAddress inetSourceAddr = InetSocketAddress::ConvertFrom (sourceAddress);
  Ipv4Address senderIfaceAddr = inetSourceAddr.GetIpv4 ();
  Ipv4Address receiverIfaceAddr = m_socketAddresses[socket];
  
  NS_ASSERT (receiverIfaceAddr != Ipv4Address ());
  NS_LOG_DEBUG ("AODV node " << this << " received a AODV packet from " << senderIfaceAddr << " to " << receiverIfaceAddr);
  NS_ASSERT (inetSourceAddr.GetPort () == AODV_PORT);
  

  aodv_rt_entry toNeighbor;
  if(!rtable.rt_lookup(senderIfaceAddr, toNeighbor ))
  {
    toNeighbor.SetDest(senderIfaceAddr);
    toNeighbor.SetFlag(RTF_UP);
    toNeighbor.SetHop(1);
    toNeighbor.SetInterface(receiverIfaceAddr);
    toNeighbor.SetLifeTime(Simulator::Now() + ACTIVE_ROUTE_TIMEOUT);
    toNeighbor.SetNextHop(senderIfaceAddr);
    toNeighbor.SetValidSeqNo(false);
  }
  else
  {
    toNeighbor.SetFlag(RTF_UP);
    Time t = toNeighbor.GetLifeTime();
    if(t < Simulator::Now() + ACTIVE_ROUTE_TIMEOUT)
    toNeighbor.SetLifeTime(Simulator::Now() + ACTIVE_ROUTE_TIMEOUT);
  }
  TypeHeader tHeader(AODVTYPE_RREQ);
  packet->PeekHeader(tHeader);
  if (!tHeader.IsValid())
    return;
  switch(tHeader.Get())
  {
  case AODVTYPE_RREQ:
  {
    RecvRequest(packet, ipv4Header);
    break;
  }
  case AODVTYPE_RREP:
  {
    RecvReply (packet, receiverIfaceAddr, senderIfaceAddr);
    break;
  }
  case AODVTYPE_RERR:
  {
    RecvError (packet);
  }
  case AODVTYPE_RREP_ACK:
    break;
  }
#endif
  // TODO check packet type and call RecvSmth
}

void 
RoutingProtocol::RecvRequest (Ptr<Packet> p, Ipv4Header & ipv4Header)
{
  uint8_t ttl = ipv4Header.GetTtl();
  Ipv4Address src = ipv4Header.GetSource();
  TypeHeader tHeader(AODVTYPE_RREQ);
  p->RemoveHeader(tHeader);

  RreqHeader rreqHeader;
  p->RemoveHeader(rreqHeader);
  uint32_t id = rreqHeader.GetId();
  Ipv4Address origin = rreqHeader.GetOrigin();

  // Node checks to determine whether it has received a RREQ
  // with the same Originator IP Address and RREQ ID.
  // If such a RREQ has been received, the node
  // silently discards the newly received RREQ.
  if(LookupBroadcastId(origin, id))
  {
    //drop();
    return;
  }

  uint8_t hop = rreqHeader.GetHopCount();
  hop++;
  rreqHeader.SetHopCount(hop);

  // Reverse route to the Originator IP Address is created, or updating
  aodv_rt_entry toOrigin;
  if(!rtable.rt_lookup(origin, toOrigin))
  {
    toOrigin.SetDest(origin);
    toOrigin.SetValidSeqNo(true);
    toOrigin.SetSeqNo(rreqHeader.GetOriginSeqno());
    toOrigin.SetNextHop(src);
    toOrigin.SetFlag(RTF_UP);
    toOrigin.SetHop(hop);
//    toOrigin.SetInterface(ipv4Header.GetDestination());          ??? which intrface
    toOrigin.SetLifeTime(Simulator::Now() + Scalar(2)*NET_TRAVERSAL_TIME - Scalar(2*hop)*NODE_TRAVERSAL_TIME);
  }
  else
  {
    if(toOrigin.GetSeqNo() < rreqHeader.GetOriginSeqno())
      toOrigin.SetSeqNo(rreqHeader.GetOriginSeqno());
    toOrigin.SetValidSeqNo(true);
    toOrigin.SetNextHop(src);
    toOrigin.SetHop(hop);
    Time existingLifetime = toOrigin.GetLifeTime();
    Time minimalLifetime = Simulator::Now() + Scalar(2)*NET_TRAVERSAL_TIME - Scalar(2*hop)*NODE_TRAVERSAL_TIME;
    if(existingLifetime < minimalLifetime)
      toOrigin.SetLifeTime(minimalLifetime);
  }

  //  A node generates a RREP if either:
  //
  //  (i)  it is itself the destination, or
  //
  //  (ii) it has an active route to the destination, the destination
  //       sequence number in the node's existing route table entry
  //       for the destination is valid and greater than or equal to
  //       the Destination Sequence Number of the RREQ ,
  //       and the "destination only" ('D') flag is NOT set.

  Ipv4Address loopback ("127.0.0.1");
  for (uint32_t k = 0; k < m_ipv4->GetNInterfaces (); k++)
  {
    Ipv4Address addr = m_ipv4->GetAddress (k, 0).GetLocal ();
    if (addr != loopback)
    {
      if(addr == rreqHeader.GetDst())
        SendReply(rreqHeader, toOrigin);
    }
  }

  aodv_rt_entry toDst;
  Ipv4Address dst = rreqHeader.GetDst();
  if(rtable.rt_lookup(dst, toDst))
  {
    // The Destination Sequence number for the requested destination is set
    // to the maximum of the corresponding value received in the RREQ message,
    // and the destination sequence value currently maintained by the node for
    // the requested destination. However, the forwarding node MUST NOT modify
    // its maintained value for the destination sequence number, even if the value
    // received in the incoming RREQ is larger than the value currently maintained
    // by the forwarding node.
    uint32_t dstSeqNo =  toDst.GetSeqNo();
    if(rreqHeader.GetUnknownSeqno() || (!rreqHeader.GetUnknownSeqno() && (rreqHeader.GetDstSeqno() < dstSeqNo)) )
    {
      if(!rreqHeader.GetDestinationOnly() && toDst.GetValidSeqNo() && (toDst.GetFlag() == RTF_UP))
      {
        SendReplyByIntermediateNode(toDst, toOrigin, rreqHeader.GetGratiousRrep());
        return;
      }
      rreqHeader.SetOriginSeqno(dstSeqNo);
      rreqHeader.SetUnknownSeqno(false);
    }
  }


  // If a node does not generate a RREP the incoming IP header has
  // TTL larger than 1, the node updates and broadcasts the RREQ
  // to address 255.255.255.255 on each of its configured interfaces.
  if (ttl > 1)
  {
    ipv4Header.SetDestination(Ipv4Address::GetBroadcast());
    ttl--;
    ipv4Header.SetTtl(ttl);
    p->AddHeader(rreqHeader);
    p->AddHeader(tHeader);

    for(std::map<  Ptr<Socket>, Ipv4InterfaceAddress >::const_iterator j = m_socketAddresses.begin(); j != m_socketAddresses.end(); ++j)
    {
      Ipv4Address myInterfaceAddress = j->second.GetLocal();
      ipv4Header.SetSource(myInterfaceAddress);
      p->AddHeader(ipv4Header);
      j->first->Send(p);
      p->RemoveHeader(ipv4Header);
    }

    btimer.SetDelay(BCAST_ID_SAVE);
    btimer.Schedule();
    htimer.SetDelay(HELLO_INTERVAL);
    htimer.Schedule();
  }
  else
  {
    //drop();
    return;
  }


}

void 
RoutingProtocol::RecvReply (Ptr<Packet> p, Ipv4Address my ,Ipv4Address src)
{
#if 0
  TypeHeader tHeader(AODVTYPE_RREP);
  p->RemoveHeader(tHeader);
  RrepHeader rrepHeader;
  p->RemoveHeader(rrepHeader);

  uint8_t hop = rrepHeader.GetHopCount();
  hop++;
  rrepHeader.SetHopCount(hop);
  aodv_rt_entry toDst;

//  If the route table entry to the destination is created or updated,
//    then the following actions occur:
//    -  the route is marked as active,
//    -  the destination sequence number is marked as valid,
//    -  the next hop in the route entry is assigned to be the node from
//       which the RREP is received, which is indicated by the source IP
//       address field in the IP header,
//    -  the hop count is set to the value of the New Hop Count,
//    -  the expiry time is set to the current time plus the value of the
//       Lifetime in the RREP message,
//    -  and the destination sequence number is the Destination Sequence
//       Number in the RREP message.

  // The forward route for this destination is created if it does not already exist.
  if(!rtable.rt_lookup(rrepHeader.GetDst(), toDst))
  {
    toDst.SetDest(rrepHeader.GetDst());
    toDst.SetFlag(RTF_UP);
    toDst.SetHop(hop);
    toDst.SetInterface(my);
    toDst.SetLifeTime(Simulator::Now() + rrepHeader.GetLifeTime());
    toDst.SetNextHop(src);
    toDst.SetSeqNo(rrepHeader.GetDstSeqno());
    toDst.SetValidSeqNo(true);
  }
//  The existing entry is updated only in the following circumstances:
  else
  {
    //  (i)   the sequence number in the routing table is marked as
    //        invalid in route table entry.
    if(!toDst.GetValidSeqNo())
    {
      toDst.SetSeqNo(rrepHeader.GetDstSeqno());
      toDst.SetValidSeqNo(true);
      toDst.SetFlag(RTF_UP);
      toDst.SetNextHop(src);
      toDst.SetLifeTime(Simulator::Now() + rrepHeader.GetLifeTime());
      toDst.SetHop(hop);
    }
    //  (ii)  the Destination Sequence Number in the RREP is greater than
    //        the node's copy of the destination sequence number and the
    //        known value is valid,
    else if(rrepHeader.GetDstSeqno() > toDst.GetSeqNo())
    {
      toDst.SetSeqNo(rrepHeader.GetDstSeqno());
      toDst.SetFlag(RTF_UP);
      toDst.SetNextHop(src);
      toDst.SetLifeTime(Simulator::Now() + rrepHeader.GetLifeTime());
      toDst.SetHop(hop);
    }
    else
    {
      //  (iii) the sequence numbers are the same, but the route is marked as inactive.
      if( (rrepHeader.GetDstSeqno() == toDst.GetSeqNo()) && (toDst.GetFlag() != RTF_UP) )
      {
        toDst.SetFlag(RTF_UP);
        toDst.SetNextHop(src);
        toDst.SetLifeTime(Simulator::Now() + rrepHeader.GetLifeTime());
        toDst.SetHop(hop);
      }
      //  (iv)   the sequence numbers are the same, and the New Hop Count is
      //         smaller than the hop count in route table entry.
      else if( (rrepHeader.GetDstSeqno() == toDst.GetSeqNo()) && (hop < toDst.GetHop()) )
      {
        toDst.SetFlag(RTF_UP);
        toDst.SetNextHop(src);
        toDst.SetLifeTime(Simulator::Now() + rrepHeader.GetLifeTime());
        toDst.SetHop(hop);
      }
    }

    if(my == rrepHeader.GetOrigin())
    {
      //TODO  may be send messeges from queue
      return;
    }
    else
    {
      aodv_rt_entry toOrigin;
      if(!rtable.rt_lookup(rrepHeader.GetOrigin(), toOrigin))
      {
//        imposible!
//        drop();
        return;
      }

      toDst.pc_insert(toOrigin.GetNextHop());
      if( toOrigin.GetLifeTime() < (Simulator::Now() + ACTIVE_ROUTE_TIMEOUT) )
        toOrigin.SetLifeTime(Simulator::Now() + ACTIVE_ROUTE_TIMEOUT);
      aodv_rt_entry toNextHopToDst;
      aodv_rt_entry toNextHopToOrigin;
      rtable.rt_lookup(toDst.GetNextHop(), toNextHopToDst);
      toNextHopToDst.pc_insert(toOrigin.GetNextHop());

      // TODO add operation over unidirctinal links
//      ipv4Header.SetSource(toOrigin.GetInterface());
//      ipv4Header.SetDestination(toOrigin.GetNextHop());
      p->AddHeader(rrepHeader);
      p->AddHeader(tHeader);
      std::map< Ipv4Address, Ptr<Socket> >::const_iterator j = m_addressSocket.find(toOrigin.GetInterface());
      j->second->Send(p);
    }



  }
#endif
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
  NS_LOG_FUNCTION (this << dst);
  
  // Create dummy UDP header
  UdpHeader udpHeader;
  udpHeader.SetSourcePort (AODV_PORT);
  udpHeader.SetDestinationPort (AODV_PORT);
  
  
  // Create RREQ header
  TypeHeader tHeader (AODVTYPE_RREQ);
  RreqHeader rreqHeader;
  rreqHeader.SetDst (dst);
  aodv_rt_entry rt;
  
  if(rtable.rt_lookup (dst, rt))
    {
      rreqHeader.SetHopCount (rt.GetLastValidHopCount());
      rreqHeader.SetDstSeqno (rt.GetSeqNo());
    }
  else
    {
      rreqHeader.SetUnknownSeqno(true);
    }
  
  if (G) rreqHeader.SetGratiousRrep(true);
  if (D) rreqHeader.SetDestinationOnly(true);
  
  seqno++;
  rreqHeader.SetOriginSeqno(seqno);
  bid++;
  rreqHeader.SetId(bid);
  rreqHeader.SetHopCount(0);
  
  // Send RREQ as subnet directed broadcast from each (own) interface
  Ipv4Header ipv4Header;
  Ptr<Packet> packet = Create<Packet> ();
  for(std::map<Ptr<Socket>, Ipv4InterfaceAddress>::const_iterator j = m_socketAddresses.begin(); j != m_socketAddresses.end(); ++j)
    {
      Ptr<Socket> socket = j->first;
      Ipv4InterfaceAddress iface = j->second;
  
      ipv4Header.SetDestination (iface.GetBroadcast());
      ipv4Header.SetSource (iface.GetLocal ());
      ipv4Header.SetTtl (NET_DIAMETER);
      NS_LOG_LOGIC ("Sending RREQ from " << iface.GetLocal() << " to " << iface.GetBroadcast() << " with TTL " << (int)ipv4Header.GetTtl());
      
      rreqHeader.SetOrigin (iface.GetLocal());
      InsertBroadcastId (iface.GetLocal(), bid);
      
      packet->AddHeader (rreqHeader);
      packet->AddHeader (tHeader);
      packet->AddHeader (udpHeader);
      packet->AddHeader (ipv4Header);
      
      udpHeader.Print (std::cout);
      std::cout << "\n";
      
      socket->Send (packet);
    }
//  htimer.SetDelay(HELLO_INTERVAL);
//  htimer.Schedule();
}

void 
RoutingProtocol::SendReply (RreqHeader & rreqHeader, aodv_rt_entry & toOrigin)
{
  RrepHeader rrepHeader;
  TypeHeader tHeader(AODVTYPE_RREP);

  rrepHeader.SetHopCount(toOrigin.GetHop());
  rrepHeader.SetOrigin(toOrigin.GetDest());
  //  Destination node MUST increment its own sequence number by one
  //  if the sequence number in the RREQ packet is equal to that incremented value.
  //  Otherwise, the destination does not change its sequence number before
  //  generating the  RREP message.
  if(!rreqHeader.GetUnknownSeqno() && (rreqHeader.GetDstSeqno() == seqno + 1))
    seqno++;
  rrepHeader.SetDstSeqno(seqno);
  rrepHeader.SetDst(rreqHeader.GetDst());
  rrepHeader.SetLifeTime(MY_ROUTE_TIMEOUT);

  Ipv4Header ipv4Header;
//  ipv4Header.SetTtl(?)
//  ipv4Header.SetProtocol(?)
  ipv4Header.SetSource(toOrigin.GetInterface());
  ipv4Header.SetDestination(toOrigin.GetNextHop());

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader(rreqHeader);
  packet->AddHeader(tHeader);
  packet->AddHeader(ipv4Header);
  //std::map< Ipv4InterfaceAddress, Ptr<Socket> >::const_iterator j = m_addressSocket.find(rreqHeader.GetDst());
  //j->second->Send(packet);
}


void
RoutingProtocol::SendReplyByIntermediateNode(aodv_rt_entry & toDst, aodv_rt_entry & toOrigin, bool gratRep)
{
#if 0
  RrepHeader rrepHeader;
  TypeHeader tHeader(AODVTYPE_RREP);

  rrepHeader.SetDst(toDst.GetDest());
  rrepHeader.SetOrigin(toOrigin.GetDest());
  rrepHeader.SetDstSeqno(toDst.GetSeqNo());
  rrepHeader.SetHopCount(toDst.GetHop());
  rrepHeader.SetLifeTime(toDst.GetLifeTime() - Simulator::Now());

  toDst.pc_insert(toOrigin.GetNextHop());
  toOrigin.pc_insert(toDst.GetNextHop());

  Ipv4Header ipv4Header;

  ipv4Header.SetSource(toOrigin.GetInterface());
  ipv4Header.SetDestination(toOrigin.GetNextHop());

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader(rrepHeader);
  packet->AddHeader(tHeader);
  packet->AddHeader(ipv4Header);
  std::map< Ipv4Address, Ptr<Socket> >::const_iterator j = m_addressSocket.find(toOrigin.GetInterface());
  j->second->Send(packet);

  if(gratRep)
  {
    rrepHeader.SetHopCount(toOrigin.GetHop());
    rrepHeader.SetDst(toOrigin.GetDest());
    rrepHeader.SetDstSeqno(toOrigin.GetSeqNo());
    rrepHeader.SetOrigin(toDst.GetDest());
    rrepHeader.SetLifeTime(toOrigin.GetLifeTime() - Simulator::Now());


    ipv4Header.SetSource(toDst.GetInterface());
    ipv4Header.SetDestination(toDst.GetNextHop());

    Ptr<Packet> packetToDst = Create<Packet> ();
    packetToDst->AddHeader(rrepHeader);
    packetToDst->AddHeader(tHeader);
    packetToDst->AddHeader(ipv4Header);
    j = m_addressSocket.find(toDst.GetInterface());
    j->second->Send(packet);
  }
#endif
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
