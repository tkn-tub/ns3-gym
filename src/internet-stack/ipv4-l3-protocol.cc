// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: George F. Riley<riley@ece.gatech.edu>
//

#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/callback.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-route.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/net-device.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/object-vector.h"
#include "ns3/ipv4-header.h"
#include "ns3/boolean.h"
#include "ns3/ipv4-routing-table-entry.h"

#include "loopback-net-device.h"
#include "arp-l3-protocol.h"
#include "ipv4-l3-protocol.h"
#include "ipv4-l4-protocol.h"
#include "icmpv4-l4-protocol.h"
#include "ipv4-interface.h"
#include "ipv4-raw-socket-impl.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4L3Protocol");

namespace ns3 {

const uint16_t Ipv4L3Protocol::PROT_NUMBER = 0x0800;

NS_OBJECT_ENSURE_REGISTERED (Ipv4L3Protocol);

TypeId 
Ipv4L3Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4L3Protocol")
    .SetParent<Ipv4> ()
    .AddConstructor<Ipv4L3Protocol> ()
    .AddAttribute ("DefaultTtl", "The TTL value set by default on all outgoing packets generated on this node.",
                   UintegerValue (64),
                   MakeUintegerAccessor (&Ipv4L3Protocol::m_defaultTtl),
                   MakeUintegerChecker<uint8_t> ())
    .AddTraceSource ("Tx", "Send ipv4 packet to outgoing interface.",
                   MakeTraceSourceAccessor (&Ipv4L3Protocol::m_txTrace))
    .AddTraceSource ("Rx", "Receive ipv4 packet from incoming interface.",
                     MakeTraceSourceAccessor (&Ipv4L3Protocol::m_rxTrace))
    .AddTraceSource ("Drop", "Drop ipv4 packet",
                     MakeTraceSourceAccessor (&Ipv4L3Protocol::m_dropTrace))
    .AddAttribute ("InterfaceList", "The set of Ipv4 interfaces associated to this Ipv4 stack.",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&Ipv4L3Protocol::m_interfaces),
                   MakeObjectVectorChecker<Ipv4Interface> ())
    ;
  return tid;
}

Ipv4L3Protocol::Ipv4L3Protocol()
  : m_nInterfaces (0),
    m_identification (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ipv4L3Protocol::~Ipv4L3Protocol ()
{
  NS_LOG_FUNCTION (this);
}

void
Ipv4L3Protocol::Insert(Ptr<Ipv4L4Protocol> protocol)
{
  m_protocols.push_back (protocol);
}
Ptr<Ipv4L4Protocol>
Ipv4L3Protocol::GetProtocol(int protocolNumber) const
{
  for (L4List_t::const_iterator i = m_protocols.begin(); i != m_protocols.end(); ++i)
    {
      if ((*i)->GetProtocolNumber () == protocolNumber)
	{
	  return *i;
	}
    }
  return 0;
}
void
Ipv4L3Protocol::Remove (Ptr<Ipv4L4Protocol> protocol)
{
  m_protocols.remove (protocol);
}

void
Ipv4L3Protocol::SetNode (Ptr<Node> node)
{
  m_node = node;
  // Add a LoopbackNetDevice if needed, and an Ipv4Interface on top of it
  SetupLoopback ();
}

Ptr<Socket> 
Ipv4L3Protocol::CreateRawSocket (void)
{
  NS_LOG_FUNCTION (this);
  Ptr<Ipv4RawSocketImpl> socket = CreateObject<Ipv4RawSocketImpl> ();
  socket->SetNode (m_node);
  m_sockets.push_back (socket);
  return socket;
}
void 
Ipv4L3Protocol::DeleteRawSocket (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  for (SocketList::iterator i = m_sockets.begin (); i != m_sockets.end (); ++i)
    {
      if ((*i) == socket)
        {
          m_sockets.erase (i);
          return;
        }
    }
  return;
}
/*
 * This method is called by AddAgregate and completes the aggregation
 * by setting the node in the ipv4 stack
 */
void
Ipv4L3Protocol::NotifyNewAggregate ()
{
  Ptr<Node>node = this->GetObject<Node>();
  // verify that it's a valid node and that
  // the node has not been set before
  if (node!= 0 && m_node == 0)
    {
      this->SetNode (node);
    }
  Object::NotifyNewAggregate ();
}

void 
Ipv4L3Protocol::SetRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol)
{
  NS_LOG_FUNCTION (this);
  m_routingProtocol = routingProtocol;
  m_routingProtocol->SetIpv4 (this);
}


Ptr<Ipv4RoutingProtocol> 
Ipv4L3Protocol::GetRoutingProtocol (void) const
{
  return m_routingProtocol;
}

void 
Ipv4L3Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  for (L4List_t::iterator i = m_protocols.begin(); i != m_protocols.end(); ++i)
    {
      *i = 0;
    }
  m_protocols.clear ();

  for (Ipv4InterfaceList::iterator i = m_interfaces.begin (); i != m_interfaces.end (); ++i)
    {
      *i = 0;
    }
  m_interfaces.clear ();
  m_node = 0;
  if (m_routingProtocol)
    {
      m_routingProtocol->Dispose ();
      m_routingProtocol = 0;
    }
  Object::DoDispose ();
}

void
Ipv4L3Protocol::SetupLoopback (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Ptr<Ipv4Interface> interface = CreateObject<Ipv4Interface> ();
  Ptr<LoopbackNetDevice> device = 0;
  // First check whether an existing LoopbackNetDevice exists on the node
  for (uint32_t i = 0; i < m_node->GetNDevices (); i++)
    {
      if (device = DynamicCast<LoopbackNetDevice> (m_node->GetDevice (i)))
        {
          break;
        }
    }
  if (device == 0)
    {
      device = CreateObject<LoopbackNetDevice> (); 
      m_node->AddDevice (device);
    }
  interface->SetDevice (device);
  interface->SetNode (m_node);
  Ipv4InterfaceAddress ifaceAddr = Ipv4InterfaceAddress (Ipv4Address::GetLoopback (), Ipv4Mask::GetLoopback ());
  interface->AddAddress (ifaceAddr);
  uint32_t index = AddIpv4Interface (interface);
  Ptr<Node> node = GetObject<Node> ();
  node->RegisterProtocolHandler (MakeCallback (&Ipv4L3Protocol::Receive, this), 
                                 Ipv4L3Protocol::PROT_NUMBER, device);
  interface->SetUp ();
  if (m_routingProtocol != 0)
    {
      m_routingProtocol->NotifyInterfaceUp (index);
    }
}

void 
Ipv4L3Protocol::SetDefaultTtl (uint8_t ttl)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_defaultTtl = ttl;
}
    
uint32_t 
Ipv4L3Protocol::AddInterface (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << &device);

  Ptr<Node> node = GetObject<Node> ();
  node->RegisterProtocolHandler (MakeCallback (&Ipv4L3Protocol::Receive, this), 
                                 Ipv4L3Protocol::PROT_NUMBER, device);
  node->RegisterProtocolHandler (MakeCallback (&ArpL3Protocol::Receive, PeekPointer (GetObject<ArpL3Protocol> ())),
                                 ArpL3Protocol::PROT_NUMBER, device);

  Ptr<Ipv4Interface> interface = CreateObject<Ipv4Interface> ();
  interface->SetNode (m_node);
  interface->SetDevice (device);
  return AddIpv4Interface (interface);
}

uint32_t 
Ipv4L3Protocol::AddIpv4Interface (Ptr<Ipv4Interface>interface)
{
  NS_LOG_FUNCTION (this << interface);
  uint32_t index = m_nInterfaces;
  m_interfaces.push_back (interface);
  m_nInterfaces++;
  return index;
}

Ptr<Ipv4Interface>
Ipv4L3Protocol::GetInterface (uint32_t index) const
{
  NS_LOG_FUNCTION (this << index);
  uint32_t tmp = 0;
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); i != m_interfaces.end (); i++)
    {
      if (index == tmp) 
	{
	  return *i;
	}
      tmp++;
    }
  return 0;
}

uint32_t 
Ipv4L3Protocol::GetNInterfaces (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_nInterfaces;
}

int32_t 
Ipv4L3Protocol::GetInterfaceForAddress (
  Ipv4Address address) const
{
  NS_LOG_FUNCTION (this << address);

  int32_t interface = 0;
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); 
       i != m_interfaces.end (); 
       i++, interface++)
    {
      for (uint32_t j = 0; j < (*i)->GetNAddresses (); j++)
        {
          if ((*i)->GetAddress (j).GetLocal () == address)
            {
              return interface;
            }
        }
    }

  return -1;
}

int32_t 
Ipv4L3Protocol::GetInterfaceForPrefix (
  Ipv4Address address, 
  Ipv4Mask mask) const
{
  NS_LOG_FUNCTION (this << address << mask);

  int32_t interface = 0;
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); 
       i != m_interfaces.end (); 
       i++, interface++)
    {
      for (uint32_t j = 0; j < (*i)->GetNAddresses (); j++)
        {
          if ((*i)->GetAddress (j).GetLocal ().CombineMask (mask) == address.CombineMask (mask))
            {
              return interface;
            }
        }
    }

  return -1;
}

int32_t 
Ipv4L3Protocol::GetInterfaceForDevice (
  Ptr<const NetDevice> device) const
{
  NS_LOG_FUNCTION (this << device->GetIfIndex());

  int32_t interface = 0;
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); 
       i != m_interfaces.end (); 
       i++, interface++)
    {
      if ((*i)->GetDevice () == device)
        {
          return interface;
        }
    }

  return -1;
}

void 
Ipv4L3Protocol::Receive( Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                         const Address &to, NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (this << &device << p << protocol <<  from);

  NS_LOG_LOGIC ("Packet from " << from << " received on node " << 
    m_node->GetId ());

  uint32_t interface = 0;
  Ptr<Packet> packet = p->Copy ();

  Ptr<Ipv4Interface> ipv4Interface;
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); 
       i != m_interfaces.end (); 
       i++)
    {
      ipv4Interface = *i;
      if (ipv4Interface->GetDevice () == device)
        {
          if (ipv4Interface->IsUp ())
            {
              m_rxTrace (packet, interface);
              break;
            }
          else
            {
              NS_LOG_LOGIC ("Dropping received packet-- interface is down");
              m_dropTrace (packet);
              return;
            }
        }
      interface++;
    }

  Ipv4Header ipHeader;
  if (Node::ChecksumEnabled ())
    {
      ipHeader.EnableChecksum ();
    }
  packet->RemoveHeader (ipHeader);

  if (!ipHeader.IsChecksumOk ()) 
    {
      m_dropTrace (packet);
      return;
    }

  for (SocketList::iterator i = m_sockets.begin (); i != m_sockets.end (); ++i)
    {
      Ptr<Ipv4RawSocketImpl> socket = *i;
      socket->ForwardUp (packet, ipHeader, device);
    }

  m_routingProtocol->RouteInput (packet, ipHeader, device, 
    MakeCallback (&Ipv4L3Protocol::IpForward, this),
    MakeCallback (&Ipv4L3Protocol::IpMulticastForward, this),
    MakeCallback (&Ipv4L3Protocol::LocalDeliver, this),
    MakeNullCallback <void, Ptr<const Packet>, const Ipv4Header &> ()
  );

}

Ptr<Icmpv4L4Protocol> 
Ipv4L3Protocol::GetIcmp (void) const
{
  Ptr<Ipv4L4Protocol> prot = GetProtocol (Icmpv4L4Protocol::GetStaticProtocolNumber ());
  if (prot != 0)
    {
      return prot->GetObject<Icmpv4L4Protocol> ();
    }
  else
    {
      return 0;
    }
}

bool
Ipv4L3Protocol::IsUnicast (Ipv4Address ad, Ipv4Mask interfaceMask) const
{
  return !ad.IsMulticast () && !ad.IsSubnetDirectedBroadcast (interfaceMask);
}

void 
Ipv4L3Protocol::Send (Ptr<Packet> packet, 
            Ipv4Address source, 
            Ipv4Address destination,
            uint8_t protocol,
            Ptr<Ipv4Route> route)
{
  NS_LOG_FUNCTION (this << packet << source << destination << uint32_t(protocol) << route);

  Ipv4Header ipHeader;
  bool mayFragment = true;
  uint8_t ttl = m_defaultTtl;
  SocketIpTtlTag tag;
  bool found = packet->RemovePacketTag (tag);
  if (found)
    {
      ttl = tag.GetTtl ();
    }

  // Handle a few cases:
  // 1) packet is destined to limited broadcast address
  // 2) packet is destined to a subnet-directed broadcast address
  // 3) packet is not broadcast, and is passed in with a route entry
  // 4) packet is not broadcast, and is passed in with a route entry but route->GetGateway is not set (e.g., on-demand)
  // 5) packet is not broadcast, and route is NULL (e.g., a raw socket call, or ICMP)
  
  // 1) packet is destined to limited broadcast address
  if (destination.IsBroadcast ()) 
    {
      NS_LOG_LOGIC ("Ipv4L3Protocol::Send case 1:  limited broadcast");
      ttl = 1;
      ipHeader = BuildHeader (source, destination, protocol, packet->GetSize (), ttl, mayFragment);
      uint32_t ifaceIndex = 0;
      for (Ipv4InterfaceList::iterator ifaceIter = m_interfaces.begin ();
           ifaceIter != m_interfaces.end (); ifaceIter++, ifaceIndex++)
        {
          Ptr<Ipv4Interface> outInterface = *ifaceIter;
          Ptr<Packet> packetCopy = packet->Copy ();

          NS_ASSERT (packetCopy->GetSize () <= outInterface->GetDevice()->GetMtu ());
          packetCopy->AddHeader (ipHeader);
          m_txTrace (packetCopy, ifaceIndex);
          outInterface->Send (packetCopy, destination);
        }
      return;
    }

  // 2) check: packet is destined to a subnet-directed broadcast address
  uint32_t ifaceIndex = 0;
  for (Ipv4InterfaceList::iterator ifaceIter = m_interfaces.begin ();
    ifaceIter != m_interfaces.end (); ifaceIter++, ifaceIndex++)
    {
      Ptr<Ipv4Interface> outInterface = *ifaceIter;
      for (uint32_t j = 0; j < GetNAddresses (ifaceIndex); j++)
        {
          Ipv4InterfaceAddress ifAddr = GetAddress (ifaceIndex, j);
          NS_LOG_LOGIC ("Testing address " << ifAddr.GetLocal () << " with mask " << ifAddr.GetMask ());
          if (destination.IsSubnetDirectedBroadcast (ifAddr.GetMask ()) && 
              destination.CombineMask (ifAddr.GetMask ()) == ifAddr.GetLocal ().CombineMask (ifAddr.GetMask ())   )  
            {
              NS_LOG_LOGIC ("Ipv4L3Protocol::Send case 2:  subnet directed bcast to " << ifAddr.GetLocal ());
              ttl = 1;
              ipHeader = BuildHeader (source, destination, protocol, packet->GetSize (), ttl, mayFragment);
              Ptr<Packet> packetCopy = packet->Copy ();
              packetCopy->AddHeader (ipHeader);
              m_txTrace (packetCopy, ifaceIndex);
              outInterface->Send (packetCopy, destination);
              return;
            }
        }
    }

  // 3) packet is not broadcast, and is passed in with a route entry
  //    with a valid Ipv4Address as the gateway
  if (route && route->GetGateway () != Ipv4Address ())
    {
      NS_LOG_LOGIC ("Ipv4L3Protocol::Send case 3:  passed in with route");
      ipHeader = BuildHeader (source, destination, protocol, packet->GetSize (), ttl, mayFragment);
      SendRealOut (route, packet, ipHeader);
      return; 
    } 
  // 4) packet is not broadcast, and is passed in with a route entry but route->GetGateway is not set (e.g., on-demand)
  if (route && route->GetGateway () != Ipv4Address ())
    {
      // This could arise because the synchronous RouteOutput() call
      // returned to the transport protocol with a source address but
      // there was no next hop available yet (since a route may need
      // to be queried).  
      NS_FATAL_ERROR ("This case not yet implemented");
    }
  // 5) packet is not broadcast, and route is NULL (e.g., a raw socket call)
  NS_LOG_LOGIC ("Ipv4L3Protocol::Send case 4:  passed in with no route " << destination);
  Socket::SocketErrno errno_; 
  uint32_t oif = 0; // unused for now
  ipHeader = BuildHeader (source, destination, protocol, packet->GetSize (), ttl, mayFragment);
  Ptr<Ipv4Route> newRoute = m_routingProtocol->RouteOutput (ipHeader, oif, errno_);
  if (newRoute)
    {
      SendRealOut (newRoute, packet, ipHeader);
    }
  else
    {
      NS_LOG_WARN ("No route to host.  Drop.");
      m_dropTrace (packet);
    }
}

// XXX when should we set ip_id?   check whether we are incrementing
// m_identification on packets that may later be dropped in this stack
// and whether that deviates from Linux
Ipv4Header
Ipv4L3Protocol::BuildHeader (
            Ipv4Address source, 
            Ipv4Address destination,
            uint8_t protocol,
            uint16_t payloadSize,
            uint8_t ttl,
            bool mayFragment)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4Header ipHeader;
  ipHeader.SetSource (source);
  ipHeader.SetDestination (destination);
  ipHeader.SetProtocol (protocol);
  ipHeader.SetPayloadSize (payloadSize);
  ipHeader.SetTtl (ttl);
  if (mayFragment == true)
    {
      ipHeader.SetMayFragment ();
      ipHeader.SetIdentification (m_identification);
      m_identification ++;
    }
  else
    {
      ipHeader.SetDontFragment ();
      // TBD:  set to zero here; will cause traces to change
      ipHeader.SetIdentification (m_identification);
      m_identification ++;
    }
  if (Node::ChecksumEnabled ())
    {
      ipHeader.EnableChecksum ();
    }
  return ipHeader;
}

void
Ipv4L3Protocol::SendRealOut (Ptr<Ipv4Route> route,
                             Ptr<Packet> packet,
                             Ipv4Header const &ipHeader)
{
  NS_LOG_FUNCTION (this << packet << &ipHeader);

  // We add a header regardless of whether there is a route, since 
  // we may want to drop trace
  packet->AddHeader (ipHeader);
  if (route == 0)
    {
      NS_LOG_WARN ("No route to host.  Drop.");
      m_dropTrace (packet);
      return;
    }

  Ptr<NetDevice> outDev = route->GetOutputDevice ();
  int32_t interface = GetInterfaceForDevice (outDev);
  NS_ASSERT (interface >= 0);
  Ptr<Ipv4Interface> outInterface = GetInterface (interface);
  NS_LOG_LOGIC ("Send via NetDevice ifIndex " << outDev->GetIfIndex () << " ipv4InterfaceIndex " << interface);

  NS_ASSERT (packet->GetSize () <= outInterface->GetDevice ()->GetMtu ());
  if (!route->GetGateway ().IsEqual (Ipv4Address ("0.0.0.0"))) 
    {
      if (outInterface->IsUp ())
        {
          NS_LOG_LOGIC ("Send to gateway " << route->GetGateway ());
          m_txTrace (packet, interface);
          outInterface->Send (packet, route->GetGateway ());
        }
      else
        {
          NS_LOG_LOGIC ("Dropping-- outgoing interface is down: " << route->GetGateway ());
          m_dropTrace (packet);
        }
    } 
  else 
    {
      if (outInterface->IsUp ())
        {
          NS_LOG_LOGIC ("Send to destination " << ipHeader.GetDestination ());
          m_txTrace (packet, interface);
          outInterface->Send (packet, ipHeader.GetDestination ());
        }
      else
        {
          NS_LOG_LOGIC ("Dropping-- outgoing interface is down: " << ipHeader.GetDestination ());
          m_dropTrace (packet);
        }
    }
}

// This function analogous to Linux ip_mr_forward()
void
Ipv4L3Protocol::IpMulticastForward (Ptr<Ipv4MulticastRoute> mrtentry, Ptr<const Packet> p, const Ipv4Header &header)
{
  NS_LOG_FUNCTION (mrtentry << p << header);
  NS_LOG_LOGIC ("Multicast forwarding logic for node: " << m_node->GetId ());
  // The output interfaces we could forward this onto are encoded
  // in the OutputTtl of the Ipv4MulticastRoute
  for (uint32_t i = 0; i < Ipv4MulticastRoute::MAX_INTERFACES; i++)
    {
      if (mrtentry->GetOutputTtl (i) < Ipv4MulticastRoute::MAX_TTL)
        {
          Ptr<Packet> packet = p->Copy ();
          Ipv4Header h = header;
          h.SetTtl (header.GetTtl () - 1);
          if (h.GetTtl () == 0)
            {
              NS_LOG_WARN ("TTL exceeded.  Drop.");
              m_dropTrace (packet);
              return;
            }
          NS_LOG_LOGIC ("Forward multicast via interface " << i);
          Ptr<Ipv4Route> rtentry = Create<Ipv4Route> ();
          rtentry->SetSource (h.GetSource ());
          rtentry->SetDestination (h.GetDestination ());
          rtentry->SetGateway (Ipv4Address::GetAny ());
          rtentry->SetOutputDevice (GetNetDevice (i));
          SendRealOut (rtentry, packet, h);
          return; 
        }
    }
}

// This function analogous to Linux ip_forward()
void
Ipv4L3Protocol::IpForward (Ptr<Ipv4Route> rtentry, Ptr<const Packet> p, const Ipv4Header &header)
{
  NS_LOG_FUNCTION (rtentry << p << header);
  NS_LOG_LOGIC ("Forwarding logic for node: " << m_node->GetId ());
  // Forwarding
  Ipv4Header ipHeader = header;
  Ptr<Packet> packet = p->Copy ();
  ipHeader.SetTtl (ipHeader.GetTtl () - 1);
  if (ipHeader.GetTtl () == 0)
    {
      // Do not reply to ICMP or to multicast/broadcast IP address 
      if (ipHeader.GetProtocol () != Icmpv4L4Protocol::PROT_NUMBER && 
        ipHeader.GetDestination ().IsBroadcast () == false && 
        ipHeader.GetDestination ().IsMulticast () == false)
        {
          Ptr<Icmpv4L4Protocol> icmp = GetIcmp ();
          icmp->SendTimeExceededTtl (ipHeader, packet);
        }
      NS_LOG_WARN ("TTL exceeded.  Drop.");
      m_dropTrace (packet);
      return;
    }
  SendRealOut (rtentry, packet, ipHeader);
}

void
Ipv4L3Protocol::LocalDeliver (Ptr<const Packet> packet, Ipv4Header const&ip, uint32_t iif)
{
  NS_LOG_FUNCTION (this << packet << &ip);
  Ptr<Packet> p = packet->Copy (); // need to pass a non-const packet up

  Ptr<Ipv4L4Protocol> protocol = GetProtocol (ip.GetProtocol ());
  if (protocol != 0)
    {
      // we need to make a copy in the unlikely event we hit the
      // RX_ENDPOINT_UNREACH codepath
      Ptr<Packet> copy = p->Copy ();
      enum Ipv4L4Protocol::RxStatus status = 
        protocol->Receive (p, ip.GetSource (), ip.GetDestination (), GetInterface (iif));
      switch (status) {
      case Ipv4L4Protocol::RX_OK:
        // fall through
      case Ipv4L4Protocol::RX_CSUM_FAILED:
        break;
      case Ipv4L4Protocol::RX_ENDPOINT_UNREACH:
        if (ip.GetDestination ().IsBroadcast () == true || 
          ip.GetDestination ().IsMulticast () == true)
          {
            break;  // Do not reply to broadcast or multicast
          }
        // Another case to suppress ICMP is a subnet-directed broadcast
        bool subnetDirected = false;
        for (uint32_t i = 0; i < GetNAddresses (iif); i++)
          {
            Ipv4InterfaceAddress addr = GetAddress (iif, i);
            if (addr.GetLocal ().CombineMask (addr.GetMask ()) == ip.GetDestination().CombineMask (addr.GetMask ()) &&
              ip.GetDestination ().IsSubnetDirectedBroadcast (addr.GetMask ()))
              {
                subnetDirected = true;
              }
          }
        if (subnetDirected == false)
          {
            GetIcmp ()->SendDestUnreachPort (ip, copy);
          }
      }
    }
}

bool
Ipv4L3Protocol::AddAddress (uint32_t i, Ipv4InterfaceAddress address)
{
  NS_LOG_FUNCTION (this << i << address);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  bool retVal = interface->AddAddress (address);
  if (m_routingProtocol != 0)
    {
      m_routingProtocol->NotifyAddAddress (i, address);
    }
  return retVal;
}

Ipv4InterfaceAddress 
Ipv4L3Protocol::GetAddress (uint32_t interfaceIndex, uint32_t addressIndex) const
{
  NS_LOG_FUNCTION (this << interfaceIndex << addressIndex);
  Ptr<Ipv4Interface> interface = GetInterface (interfaceIndex);
  return interface->GetAddress (addressIndex);
}

uint32_t 
Ipv4L3Protocol::GetNAddresses (uint32_t interface) const
{
  NS_LOG_FUNCTION (this << interface);
  Ptr<Ipv4Interface> iface = GetInterface (interface);
  return iface->GetNAddresses ();
}

bool
Ipv4L3Protocol::RemoveAddress (uint32_t i, uint32_t addressIndex)
{
  NS_LOG_FUNCTION (this << i << addressIndex);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  Ipv4InterfaceAddress address = interface->RemoveAddress (addressIndex);
  if (address != Ipv4InterfaceAddress ())
    {
      if (m_routingProtocol != 0)
        {
          m_routingProtocol->NotifyRemoveAddress (i, address);
        }
      return true;
    }
  return false;
}

void 
Ipv4L3Protocol::SetMetric (uint32_t i, uint16_t metric)
{
  NS_LOG_FUNCTION (i << metric);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  interface->SetMetric (metric);
}

uint16_t
Ipv4L3Protocol::GetMetric (uint32_t i) const
{
  NS_LOG_FUNCTION (i);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  return interface->GetMetric ();
}

uint16_t 
Ipv4L3Protocol::GetMtu (uint32_t i) const
{
  NS_LOG_FUNCTION (this << i);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  return interface->GetDevice ()->GetMtu ();
}

bool 
Ipv4L3Protocol::IsUp (uint32_t i) const
{
  NS_LOG_FUNCTION (this << i);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  return interface->IsUp ();
}

void 
Ipv4L3Protocol::SetUp (uint32_t i)
{
  NS_LOG_FUNCTION (this << i);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  interface->SetUp ();

  if (m_routingProtocol != 0)
    {
      m_routingProtocol->NotifyInterfaceUp (i);
    }
}

void 
Ipv4L3Protocol::SetDown (uint32_t ifaceIndex)
{
  NS_LOG_FUNCTION (this << ifaceIndex);
  Ptr<Ipv4Interface> interface = GetInterface (ifaceIndex);
  interface->SetDown ();

  if (m_routingProtocol != 0)
    {
      m_routingProtocol->NotifyInterfaceDown (ifaceIndex);
    }
}

Ptr<NetDevice>
Ipv4L3Protocol::GetNetDevice (uint32_t i)
{
  return GetInterface (i)-> GetDevice ();
}

void 
Ipv4L3Protocol::SetIpForward (bool forward) 
{
  m_ipForward = forward;
}

bool 
Ipv4L3Protocol::GetIpForward (void) const
{
  return m_ipForward;
}


}//namespace ns3
