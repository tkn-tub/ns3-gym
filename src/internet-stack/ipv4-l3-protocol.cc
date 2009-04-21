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
#include "arp-l3-protocol.h"

#include "ipv4-l3-protocol.h"
#include "ipv4-l4-protocol.h"
#include "icmpv4-l4-protocol.h"
#include "ipv4-interface.h"
#include "ipv4-loopback-interface.h"
#include "arp-ipv4-interface.h"
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
    .AddAttribute ("CalcChecksum", "If true, we calculate the checksum of outgoing packets"
                   " and verify the checksum of incoming packets.",
                   BooleanValue (false),
                   MakeBooleanAccessor (&Ipv4L3Protocol::m_calcChecksum),
                   MakeBooleanChecker ())
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
  m_staticRouting = CreateObject<Ipv4StaticRouting> ();
  AddRoutingProtocol (m_staticRouting, 0);
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

void 
Ipv4L3Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
 
  for (SocketList::iterator i = m_sockets.begin (); i != m_sockets.end (); ++i)
    {
      *i = 0;
    }

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
  m_routingProtocols.clear ();
  m_node = 0;
  m_staticRouting->Dispose ();
  m_staticRouting = 0;
  Object::DoDispose ();
}

void
Ipv4L3Protocol::SetupLoopback (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Ptr<Ipv4LoopbackInterface> interface = CreateObject<Ipv4LoopbackInterface> ();
  interface->SetNode (m_node);
  Ipv4InterfaceAddress ifaceAddr = Ipv4InterfaceAddress (Ipv4Address::GetLoopback (), Ipv4Mask::GetLoopback ());
  interface->AddAddress (ifaceAddr);
  uint32_t index = AddIpv4Interface (interface);
  AddHostRouteTo (Ipv4Address::GetLoopback (), index);
  interface->SetUp ();
}

void 
Ipv4L3Protocol::SetDefaultTtl (uint8_t ttl)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_defaultTtl = ttl;
}
    

void 
Ipv4L3Protocol::AddHostRouteTo (Ipv4Address dest, 
                      Ipv4Address nextHop, 
                      uint32_t interface)
{
  NS_LOG_FUNCTION (this << dest << nextHop << interface);
  m_staticRouting->AddHostRouteTo (dest, nextHop, interface);
}

void 
Ipv4L3Protocol::AddHostRouteTo (Ipv4Address dest, 
				uint32_t interface)
{
  NS_LOG_FUNCTION (this << dest << interface);
  m_staticRouting->AddHostRouteTo (dest, interface);
}

void 
Ipv4L3Protocol::AddNetworkRouteTo (Ipv4Address network, 
				   Ipv4Mask networkMask, 
				   Ipv4Address nextHop, 
				   uint32_t interface)
{
  NS_LOG_FUNCTION (this << network << networkMask << nextHop << interface);
  m_staticRouting->AddNetworkRouteTo (network, networkMask, nextHop, interface);
}

void 
Ipv4L3Protocol::AddNetworkRouteTo (Ipv4Address network, 
				   Ipv4Mask networkMask, 
				   uint32_t interface)
{
  NS_LOG_FUNCTION (this << network << networkMask << interface);
  m_staticRouting->AddNetworkRouteTo (network, networkMask, interface);
}

void 
Ipv4L3Protocol::SetDefaultRoute (Ipv4Address nextHop, 
				 uint32_t interface)
{
  NS_LOG_FUNCTION (this << nextHop << interface);
  m_staticRouting->SetDefaultRoute (nextHop, interface);
}

void
Ipv4L3Protocol::Lookup (
  Ipv4Header const &ipHeader,
  Ptr<Packet> packet,
  Ipv4RoutingProtocol::RouteReplyCallback routeReply)
{
  NS_LOG_FUNCTION (this << &ipHeader << packet << &routeReply);

  Lookup (Ipv4RoutingProtocol::INTERFACE_ANY, ipHeader, packet, routeReply);
}

void
Ipv4L3Protocol::Lookup (
  uint32_t interface,
  Ipv4Header const &ipHeader,
  Ptr<Packet> packet,
  Ipv4RoutingProtocol::RouteReplyCallback routeReply)
{
  NS_LOG_FUNCTION (this << interface << &ipHeader << packet << &routeReply);

  for (Ipv4RoutingProtocolList::const_iterator rprotoIter = 
         m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end (); 
       rprotoIter++)
    {
      NS_LOG_LOGIC ("Requesting route");
      if ((*rprotoIter).second->RequestRoute (interface, ipHeader, packet, 
                                              routeReply))
        return;
    }

  if (ipHeader.GetDestination ().IsMulticast () && 
      interface == Ipv4RoutingProtocol::INTERFACE_ANY)
    {
      NS_LOG_LOGIC ("Multicast destination with local source");
//
// We have a multicast packet originating from the current node and were not
// able to send it using the usual RequestRoute process.  Since the usual
// process includes trying to use a default multicast route, this means that
// there was no specific route out of the node found, and there was no default
// multicast route set.
//
// The fallback position is to look for a default unicast route and use that
// to get the packet off the node if we have one.
//
      Ipv4Route *route = m_staticRouting->GetDefaultRoute ();

      if (route)
        {
          NS_LOG_LOGIC ("Local source. Using unicast default route for "
            "multicast packet");

          routeReply (true, *route, packet, ipHeader);
          return;
        }
    }
//
// No route found
//
  routeReply (false, Ipv4Route (), packet, ipHeader);
}

void
Ipv4L3Protocol::AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol,
                                    int16_t priority)
{
  NS_LOG_FUNCTION (this << &routingProtocol << priority);
  m_routingProtocols.push_back
    (std::pair<int, Ptr<Ipv4RoutingProtocol> > (-priority, routingProtocol));
  m_routingProtocols.sort ();
}

uint32_t 
Ipv4L3Protocol::GetNRoutes (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_staticRouting->GetNRoutes ();
}

Ipv4Route 
Ipv4L3Protocol::GetRoute (uint32_t index)
{
  NS_LOG_FUNCTION_NOARGS ();
  return *m_staticRouting->GetRoute (index);
}

void 
Ipv4L3Protocol::RemoveRoute (uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
  m_staticRouting->RemoveRoute (index);
}

void 
Ipv4L3Protocol::AddMulticastRoute (Ipv4Address origin,
                                   Ipv4Address group,
                                   uint32_t inputInterface,
                                   std::vector<uint32_t> outputInterfaces)
{
  NS_LOG_FUNCTION (this << origin << group << inputInterface << &outputInterfaces);

  m_staticRouting->AddMulticastRoute (origin, group, inputInterface,
    outputInterfaces);
}

void 
Ipv4L3Protocol::SetDefaultMulticastRoute (uint32_t outputInterface)
{
  NS_LOG_FUNCTION (this << outputInterface);

  m_staticRouting->SetDefaultMulticastRoute (outputInterface);
}

uint32_t 
Ipv4L3Protocol::GetNMulticastRoutes (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_staticRouting->GetNMulticastRoutes ();
}

Ipv4MulticastRoute 
Ipv4L3Protocol::GetMulticastRoute (uint32_t index) const
{
  NS_LOG_FUNCTION (this << index);
  return *m_staticRouting->GetMulticastRoute (index);
}

void 
Ipv4L3Protocol::RemoveMulticastRoute (Ipv4Address origin,
                                       Ipv4Address group,
                                       uint32_t inputInterface)
{
  NS_LOG_FUNCTION (this << origin << group << inputInterface);
  m_staticRouting->RemoveMulticastRoute (origin, group, inputInterface);
}

void 
Ipv4L3Protocol::RemoveMulticastRoute (uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
  m_staticRouting->RemoveMulticastRoute (index);
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

  Ptr<ArpIpv4Interface> interface = CreateObject<ArpIpv4Interface> ();
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

uint32_t 
Ipv4L3Protocol::FindInterfaceForAddr (Ipv4Address addr) const
{
  NS_LOG_FUNCTION (this << addr);

  uint32_t interface = 0;
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); 
       i != m_interfaces.end (); 
       i++, interface++)
    {
      for (uint32_t j = 0; j < (*i)->GetNAddresses (); j++) 
        {
          if ((*i)->GetAddress (j).GetLocal () == addr)
            {
              return interface;
            }
        }
    }

  NS_FATAL_ERROR ("Ipv4L3Protocol::FindInterfaceForAddr (): "
                  "Interface not found for IP address " << addr);
  return 0;
}

uint32_t 
Ipv4L3Protocol::FindInterfaceForAddr (Ipv4Address addr, Ipv4Mask mask) const
{
  NS_LOG_FUNCTION (this << addr << mask);

  uint32_t interface = 0;
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); 
       i != m_interfaces.end (); 
       i++, interface++)
    {
      for (uint32_t j = 0; j < (*i)->GetNAddresses (); j++)
        {
          if ((*i)->GetAddress (j).GetLocal ().CombineMask (mask) == addr.CombineMask (mask))
            {
              return interface;
            }
        }
    }

  NS_ASSERT_MSG(false, "Ipv4L3Protocol::FindInterfaceForAddr (): "
    "Interface not found for masked IP address");
  return 0;
}

int32_t 
Ipv4L3Protocol::FindInterfaceForDevice (Ptr<NetDevice> device) const
{
  NS_LOG_FUNCTION (this << device);

  uint32_t interface = 0;
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

Ptr<Ipv4Interface>
Ipv4L3Protocol::FindInterfaceForDevice (Ptr<const NetDevice> device)
{
  NS_LOG_FUNCTION (this << &device);
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); i != m_interfaces.end (); i++)
    {
      if ((*i)->GetDevice () == device)
        {
          return *i;
        }
    }
  return 0;
}  

void 
Ipv4L3Protocol::Receive( Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                         const Address &to, NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (this << &device << p << protocol <<  from);

  NS_LOG_LOGIC ("Packet from " << from << " received on node " << m_node->GetId ());

  Ptr<Packet> packet = p->Copy ();

  uint32_t index = 0;
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
              m_rxTrace (packet, index);
              break;
            } 
          else
            {
              NS_LOG_LOGIC ("Dropping received packet-- interface is down");
              m_dropTrace (packet);
              return;
            }
        }
      index++;
    }
  Ipv4Header ipHeader;
  if (m_calcChecksum)
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

  if (Forwarding (index, packet, ipHeader, device)) 
    {
      return;
    }

  ForwardUp (packet, ipHeader, ipv4Interface);
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
            uint8_t protocol)
{
  NS_LOG_FUNCTION (this << packet << source << destination << protocol);

  Ipv4Header ipHeader;

  if (m_calcChecksum)
    {
      ipHeader.EnableChecksum ();
    }

  ipHeader.SetSource (source);
  ipHeader.SetDestination (destination);
  ipHeader.SetProtocol (protocol);
  ipHeader.SetPayloadSize (packet->GetSize ());
  ipHeader.SetIdentification (m_identification);

  m_identification ++;

  SocketSetDontFragmentTag dfTag;
  bool found = packet->RemovePacketTag (dfTag);
  if (found)
    {
      if (dfTag.IsEnabled ())
        {
          ipHeader.SetDontFragment ();
        }
      else
        {
          ipHeader.SetMayFragment ();
        }
    }
  

  // Set TTL to 1 if it is a broadcast packet of any type.  Otherwise,
  // possibly override the default TTL if the packet is tagged
  SocketIpTtlTag tag;
  found = packet->RemovePacketTag (tag);

  if (destination.IsBroadcast ()) 
    {
      ipHeader.SetTtl (1);
    }
  else if (found)
    {
      ipHeader.SetTtl (tag.GetTtl ());
    }
  else
    {
      ipHeader.SetTtl (m_defaultTtl);
      uint32_t ifaceIndex = 0;
      for (Ipv4InterfaceList::iterator ifaceIter = m_interfaces.begin ();
           ifaceIter != m_interfaces.end (); ifaceIter++, ifaceIndex++)
        {
          Ptr<Ipv4Interface> outInterface = *ifaceIter;
          // XXX this logic might not be completely correct for multi-addressed interface
          for (uint32_t j = 0; j < outInterface->GetNAddresses(); j++)
            {
              if (destination.IsSubnetDirectedBroadcast (
                outInterface->GetAddress (j).GetMask ()))
                {
                  ipHeader.SetTtl (1);
                }
            }
        }
    }
  if (destination.IsBroadcast ())
    {
      uint32_t ifaceIndex = 0;
      for (Ipv4InterfaceList::iterator ifaceIter = m_interfaces.begin ();
           ifaceIter != m_interfaces.end (); ifaceIter++, ifaceIndex++)
        {
          Ptr<Ipv4Interface> outInterface = *ifaceIter;
          Ptr<Packet> packetCopy = packet->Copy ();

          packetCopy->AddHeader (ipHeader);
          // XXX Handle multiple address on interface
          if (packetCopy->GetSize () > outInterface->GetMtu () &&
              ipHeader.IsDontFragment () &&
              IsUnicast (ipHeader.GetDestination (), outInterface->GetAddress (0).GetMask ()))
            {
              Ptr<Icmpv4L4Protocol> icmp = GetIcmp ();
              NS_ASSERT (icmp != 0);
              icmp->SendDestUnreachFragNeeded (ipHeader, packet, outInterface->GetMtu ());
              m_dropTrace (packetCopy);
            }
          else if (packet->GetSize () > outInterface->GetMtu () &&
                   !ipHeader.IsDontFragment ())
            {
              NS_LOG_LOGIC ("Too big: need fragmentation but no frag support.");
              m_dropTrace (packet);
            }
          else
            {
              NS_ASSERT (packetCopy->GetSize () <= outInterface->GetMtu ());
              if (outInterface->IsUp ())
                {
                  m_txTrace (packetCopy, ifaceIndex);
                  outInterface->Send (packetCopy, destination);
                }
              else
                {
                  m_dropTrace (packetCopy);
                }
            }
        }
    }
  else
    {
      // XXX Note here that in most ipv4 stacks in the world,
      // the route calculation for an outgoing packet is not
      // done in the ip layer. It is done within the application
      // socket when the first packet is sent to avoid this
      // costly lookup on a per-packet basis.
      // That would require us to get the route from the packet,
      // most likely with a packet tag. The higher layers do not
      // do this yet for us.
      Lookup (ipHeader, packet,
              MakeCallback (&Ipv4L3Protocol::SendRealOut, this));
    }
}

void
Ipv4L3Protocol::SendRealOut (bool found,
                             Ipv4Route const &route,
                             Ptr<Packet> packet,
                             Ipv4Header const &ipHeader)
{
  NS_LOG_FUNCTION (this << found << &route << packet << &ipHeader);

  packet->AddHeader (ipHeader);
  if (!found)
    {
      NS_LOG_WARN ("No route to host.  Drop.");
      m_dropTrace (packet);
      return;
    }

  NS_LOG_LOGIC ("Send via interface " << route.GetInterface ());

  Ptr<Ipv4Interface> outInterface = GetInterface (route.GetInterface ());
  // XXX handle multiple address on interface
  if (packet->GetSize () > outInterface->GetMtu () &&
      ipHeader.IsDontFragment () &&
      IsUnicast (ipHeader.GetDestination (), outInterface->GetAddress (0).GetMask ()))
    {
      NS_LOG_LOGIC ("Too big: need fragmentation but not allowed");
      Ptr<Icmpv4L4Protocol> icmp = GetIcmp ();
      NS_ASSERT (icmp != 0);
      Ptr<Packet> copyNoHeader = packet->Copy ();
      Ipv4Header tmp;
      copyNoHeader->RemoveHeader (tmp);
      icmp->SendDestUnreachFragNeeded (ipHeader, copyNoHeader, outInterface->GetMtu ());
      m_dropTrace (packet);
    }
  else if (packet->GetSize () > outInterface->GetMtu () &&
           !ipHeader.IsDontFragment ())
    {
      NS_LOG_LOGIC ("Too big: need fragmentation but no frag support.");
      m_dropTrace (packet);
    }
  else
    {
      if (route.IsGateway ()) 
        {
          if (outInterface->IsUp ())
            {
              NS_LOG_LOGIC ("Send to gateway " << route.GetGateway ());
              m_txTrace (packet, route.GetInterface ());
              outInterface->Send (packet, route.GetGateway ());
            }
          else
            {
              NS_LOG_LOGIC ("Dropping-- outgoing interface is down: " << route.GetGateway ());
              m_dropTrace (packet);
            }
        } 
      else 
        {
          if (outInterface->IsUp ())
            {
              NS_LOG_LOGIC ("Send to destination " << ipHeader.GetDestination ());
              m_txTrace (packet, route.GetInterface ());
              outInterface->Send (packet, ipHeader.GetDestination ());
            }
          else
            {
              NS_LOG_LOGIC ("Dropping-- outgoing interface is down: " << route.GetGateway ());
              m_dropTrace (packet);
            }
        }
    }
}

bool
Ipv4L3Protocol::Forwarding (
  uint32_t interface, 
  Ptr<Packet> packet, 
  Ipv4Header &ipHeader, 
  Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (interface << packet << &ipHeader<< device);
  NS_LOG_LOGIC ("Forwarding logic for node: " << m_node->GetId ());

  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin ();
       i != m_interfaces.end (); i++) 
    {
      for (uint32_t j = 0; j < (*i)->GetNAddresses (); j++)
        {
          if ((*i)->GetAddress (j).GetLocal ().IsEqual (ipHeader.GetDestination ())) 
            {
              NS_LOG_LOGIC ("For me (destination match)");
              return false;
            }
        }
    }
  
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin ();
       i != m_interfaces.end (); i++) 
    {
      Ptr<Ipv4Interface> interface = *i;
      if (interface->GetDevice () == device)
	{
          // XXX multi-address case
	  if (ipHeader.GetDestination ().IsEqual (interface->GetAddress (0).GetBroadcast ())) 
	    {
              NS_LOG_LOGIC ("For me (interface broadcast address)");
	      return false;
	    }
	  break;
	}
    }
      
  if (ipHeader.GetDestination ().IsBroadcast ()) 
    {
      NS_LOG_LOGIC ("For me (Ipv4Addr broadcast address)");
      return false;
    }

  if (ipHeader.GetDestination ().IsEqual (Ipv4Address::GetAny ())) 
    {
      NS_LOG_LOGIC ("For me (Ipv4Addr any address)");
      return false;
    }
//  
// If this is a to a multicast address and this node is a member of the 
// indicated group we need to return false so the multicast is forwarded up.
//        
  for (Ipv4MulticastGroupList::const_iterator i = m_multicastGroups.begin ();
       i != m_multicastGroups.end (); i++)
    {
      if ((*i).first.IsEqual (ipHeader.GetSource ()) &&
          (*i).second.IsEqual (ipHeader.GetDestination ()))
        {
          NS_LOG_LOGIC ("For me (Joined multicast group)");
          // We forward with a packet copy, since forwarding may change
          // the packet, affecting our local delivery
          NS_LOG_LOGIC ("Forwarding (multicast).");
          DoForward (interface, packet->Copy (), ipHeader);
          return false;
        }   
    }

  DoForward (interface, packet, ipHeader);
  return true;
}

void
Ipv4L3Protocol::DoForward (uint32_t interface, 
                           Ptr<Packet> packet, 
                           Ipv4Header ipHeader)
{
  NS_LOG_FUNCTION (this << interface << packet << ipHeader);

  ipHeader.SetTtl (ipHeader.GetTtl () - 1);
  // XXX handle multi-interfaces
  if (ipHeader.GetTtl () == 0)
    {
      if (IsUnicast (ipHeader.GetDestination (), GetInterface (interface)->GetAddress (0).GetMask ()))
        {
          Ptr<Icmpv4L4Protocol> icmp = GetIcmp ();
          icmp->SendTimeExceededTtl (ipHeader, packet);
        }
      NS_LOG_WARN ("TTL exceeded.  Drop.");
      m_dropTrace (packet);
      return;
    }  
  NS_LOG_LOGIC ("Not for me, forwarding.");
  Lookup (interface, ipHeader, packet,
          MakeCallback (&Ipv4L3Protocol::SendRealOut, this));
}


void
Ipv4L3Protocol::ForwardUp (Ptr<Packet> p, Ipv4Header const&ip,
                           Ptr<Ipv4Interface> incomingInterface)
{
  NS_LOG_FUNCTION (this << p << &ip);

  Ptr<Ipv4L4Protocol> protocol = GetProtocol (ip.GetProtocol ());
  if (protocol != 0)
    {
      // we need to make a copy in the unlikely event we hit the
      // RX_ENDPOINT_UNREACH codepath
      Ptr<Packet> copy = p->Copy ();
      enum Ipv4L4Protocol::RxStatus status = 
        protocol->Receive (p, ip.GetSource (), ip.GetDestination (), incomingInterface);
      switch (status) {
      case Ipv4L4Protocol::RX_OK:
        // fall through
      case Ipv4L4Protocol::RX_CSUM_FAILED:
        break;
      case Ipv4L4Protocol::RX_ENDPOINT_UNREACH:
        // XXX handle multi-interface case
        if (IsUnicast (ip.GetDestination (), incomingInterface->GetAddress (0).GetMask ()))
          {
            GetIcmp ()->SendDestUnreachPort (ip, copy);
          }
        break;
      }
    }
}

void 
Ipv4L3Protocol::JoinMulticastGroup (Ipv4Address origin, Ipv4Address group)
{
  NS_LOG_FUNCTION (this << origin << group);
  m_multicastGroups.push_back(
    std::pair<Ipv4Address, Ipv4Address> (origin, group));
}

void
Ipv4L3Protocol::LeaveMulticastGroup (Ipv4Address origin, Ipv4Address group)
{
  NS_LOG_FUNCTION (this << origin << group);

  for (Ipv4MulticastGroupList::iterator i = m_multicastGroups.begin ();
       i != m_multicastGroups.end (); 
       i++)
    {
      if ((*i).first.IsEqual(origin) && (*i).second.IsEqual(group))
        {
          m_multicastGroups.erase (i);
          return;
        }
    }
}

uint32_t
Ipv4L3Protocol::AddAddress (uint32_t i, Ipv4InterfaceAddress address)
{
  NS_LOG_FUNCTION (this << i << address);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  return interface->AddAddress (address);
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

bool
Ipv4L3Protocol::GetInterfaceForDestination (
  Ipv4Address destination, uint32_t& interface) const
{
  NS_LOG_FUNCTION (this << destination << &interface);
//
// The first thing we do in trying to determine a source address is to 
// consult the routing protocols.  These will also check for a default route
// if one has been set.
//
  for (Ipv4RoutingProtocolList::const_iterator i = m_routingProtocols.begin ();
       i != m_routingProtocols.end (); 
       i++)
    {
      NS_LOG_LOGIC ("Requesting Source Address");
      uint32_t interfaceTmp;

      if ((*i).second->RequestInterface (destination, interfaceTmp))
        {
          NS_LOG_LOGIC ("Found interface " << interfaceTmp);
          interface = interfaceTmp;
          return true;
        }
    }
//
// If there's no routing table entry telling us what *single* interface will 
// be used to send a packet to this destination, we'll have to just pick one.  
// If there's only one interface on this node, a good answer isn't very hard
// to come up with.  Before jumping to any conclusions, remember that the 
// zeroth interface is the loopback interface, so what we actually want is
// a situation where there are exactly two interfaces on the node, in which
// case interface one is the "single" interface connected to the outside world.
//
  if (GetNInterfaces () == 2)
    {
      NS_LOG_LOGIC ("One Interface.  Using interface 1.");
      interface = 1;
      return true;
    }
//
// If we fall through to here, we have a node with multiple interfaces and
// no routes to guide us in determining what interface to choose.  Either
// no default route was found (for unicast or multicast), or in the case of a
// multicast, the default route contained multiple outbound interfaces.
//
// The fallback position is to just get the unicast default route and use 
// the outgoing interface specified there.  We don't want to leave the source
// address unset, so we just assert here.
//
// N.B. that in the case of a multicast with a route containing multiple
// outgoing interfaces, the source address of packets from that node will be
// set to the IP address of the interface set in the default unicast route.
// Also, in the case of a broadcast, the same will be true.
//
  NS_LOG_LOGIC ("Using default unicast route");
  Ipv4Route *route = m_staticRouting->GetDefaultRoute ();

  if (route == NULL)
    {
      NS_LOG_LOGIC ("Ipv4L3Protocol::GetInterfaceForDestination (): "
                    "Unable to determine outbound interface.  No default route set");
      return false;
    }

  interface = route->GetInterface ();

  NS_LOG_LOGIC ("Default route specifies interface " << interface);
  return true;
}

uint16_t 
Ipv4L3Protocol::GetMtu (uint32_t i) const
{
  NS_LOG_FUNCTION (this << i);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  return interface->GetMtu ();
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

  // If interface address and network mask have been set, add a route
  // to the network of the interface (like e.g. ifconfig does on a
  // Linux box)
  for (uint32_t j = 0; j < interface->GetNAddresses (); j++)
    {
      if (((interface->GetAddress (j).GetLocal ()) != (Ipv4Address ()))
          && (interface->GetAddress (j).GetMask ()) != (Ipv4Mask ()))
        {
          AddNetworkRouteTo (interface->GetAddress (j).GetLocal ().CombineMask (interface->GetAddress (j).GetMask ()), interface->GetAddress (j).GetMask (), i);
        }
    }
}

void 
Ipv4L3Protocol::SetDown (uint32_t ifaceIndex)
{
  NS_LOG_FUNCTION (this << ifaceIndex);
  Ptr<Ipv4Interface> interface = GetInterface (ifaceIndex);
  interface->SetDown ();

  // Remove all static routes that are going through this interface
  bool modified = true;
  while (modified)
    {
      modified = false;
      for (uint32_t i = 0; i < GetNRoutes (); i++)
        {
          Ipv4Route route = GetRoute (i);
          if (route.GetInterface () == ifaceIndex)
            {
              RemoveRoute (i);
              modified = true;
              break;
            }
        }
    }
}

// Note:  This method will be removed in Ipv4 routing work
Ipv4Address
Ipv4L3Protocol::GetSourceAddress (Ipv4Address destination) const
{
  uint32_t interface = 0xffffffff;

  bool result = GetInterfaceForDestination (destination, interface);

  if (result)
    {
      // if multiple addresses exist, search for the first one on the same subnet
      for (uint32_t i = 0; i < GetNAddresses (interface); i++)
        {
          Ipv4InterfaceAddress ipv4InAddr = GetAddress (interface, i);
          if (ipv4InAddr.GetLocal().CombineMask(ipv4InAddr.GetMask ()) == destination.CombineMask (ipv4InAddr.GetMask ()))
            {
              return ipv4InAddr.GetLocal ();
            }
        }
      // Destination is off-link, so return first address.
      return GetAddress (interface, 0).GetLocal ();
    }
  else
    {
//
// If we can't find any address, just leave it 0.0.0.0
//
      return Ipv4Address::GetAny ();
    }
}

Ptr<NetDevice>
Ipv4L3Protocol::GetNetDevice (uint32_t i)
{
  return GetInterface (i)-> GetDevice ();
}



}//namespace ns3
