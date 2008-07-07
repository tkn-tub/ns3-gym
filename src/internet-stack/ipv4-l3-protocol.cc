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
#include "ipv4-interface.h"
#include "ipv4-loopback-interface.h"
#include "arp-ipv4-interface.h"
#include "ipv4-l4-demux.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4L3Protocol");

namespace ns3 {

const uint16_t Ipv4L3Protocol::PROT_NUMBER = 0x0800;

NS_OBJECT_ENSURE_REGISTERED (Ipv4L3Protocol);

TypeId 
Ipv4L3Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4L3Protocol")
    .SetParent<Object> ()
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
Ipv4L3Protocol::SetNode (Ptr<Node> node)
{
  m_node = node;
  SetupLoopback ();
}

void 
Ipv4L3Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); i != m_interfaces.end (); ++i)
    {
      Ptr<Ipv4Interface> interface = *i;
      interface->Dispose ();
    }
  m_interfaces.clear ();
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
  interface->SetAddress (Ipv4Address::GetLoopback ());
  interface->SetNetworkMask (Ipv4Mask::GetLoopback ());
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

  Lookup (Ipv4RoutingProtocol::IF_INDEX_ANY, ipHeader, packet, routeReply);
}

void
Ipv4L3Protocol::Lookup (
  uint32_t ifIndex,
  Ipv4Header const &ipHeader,
  Ptr<Packet> packet,
  Ipv4RoutingProtocol::RouteReplyCallback routeReply)
{
  NS_LOG_FUNCTION (this << ifIndex << &ipHeader << packet << &routeReply);

  for (Ipv4RoutingProtocolList::const_iterator rprotoIter = 
         m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end (); 
       rprotoIter++)
    {
      NS_LOG_LOGIC ("Requesting route");
      if ((*rprotoIter).second->RequestRoute (ifIndex, ipHeader, packet, 
                                              routeReply))
        return;
    }

  if (ipHeader.GetDestination ().IsMulticast () && 
      ifIndex == Ipv4RoutingProtocol::IF_INDEX_ANY)
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
                                    int priority)
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

Ipv4Route *
Ipv4L3Protocol::GetRoute (uint32_t index)
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_staticRouting->GetRoute (index);
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

Ipv4MulticastRoute *
Ipv4L3Protocol::GetMulticastRoute (uint32_t index) const
{
  NS_LOG_FUNCTION (this << index);
  return m_staticRouting->GetMulticastRoute (index);
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

  uint32_t ifIndex = 0;
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); 
       i != m_interfaces.end (); 
       i++, ifIndex++)
    {
      if ((*i)->GetAddress () == addr)
        {
          return ifIndex;
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

  uint32_t ifIndex = 0;
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); 
       i != m_interfaces.end (); 
       i++, ifIndex++)
    {
      if ((*i)->GetAddress ().CombineMask (mask) == addr.CombineMask (mask))
        {
          return ifIndex;
        }
    }

  NS_ASSERT_MSG(false, "Ipv4L3Protocol::FindInterfaceForAddr (): "
    "Interface not found for masked IP address");
  return 0;
}

int32_t 
Ipv4L3Protocol::FindInterfaceIndexForDevice (Ptr<NetDevice> device) const
{
  NS_LOG_FUNCTION (this << device);

  uint32_t ifIndex = 0;
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); 
       i != m_interfaces.end (); 
       i++, ifIndex++)
    {
      if ((*i)->GetDevice () == device)
        {
          return ifIndex;
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
Ipv4L3Protocol::Receive( Ptr<NetDevice> device, Ptr<Packet> packet, uint16_t protocol, const Address &from,
                         const Address &to, NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (this << &device << packet << protocol <<  from);

  if (packetType == NetDevice::PACKET_OTHERHOST)
    {
      return;
    }

  NS_LOG_LOGIC ("Packet from " << from << " received on node " << m_node->GetId ());

  uint32_t index = 0;
  Ptr<Ipv4Interface> ipv4Interface;
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); 
       i != m_interfaces.end (); 
       i++)
    {
      ipv4Interface = *i;
      if (ipv4Interface->GetDevice () == device)
        {
          m_rxTrace (packet, index);
          break;
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

  if (Forwarding (index, packet, ipHeader, device)) 
    {
      return;
    }

  ForwardUp (packet, ipHeader, ipv4Interface);
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
  ipHeader.SetTtl (m_defaultTtl);
  ipHeader.SetMayFragment ();
  ipHeader.SetIdentification (m_identification);

  m_identification ++;

  // Set TTL to 1 if it is a broadcast packet of any type.  Otherwise,
  // possibly override the default TTL if the packet is tagged
  SocketIpTtlTag tag;
  bool found = packet->FindFirstMatchingTag (tag);

  if (destination.IsBroadcast ()) 
    {
      ipHeader.SetTtl (1);
    }
  else if (found)
    {
      ipHeader.SetTtl (tag.GetTtl ());
      // XXX remove tag here?  
    }
  else
    {
      uint32_t ifaceIndex = 0;
      for (Ipv4InterfaceList::iterator ifaceIter = m_interfaces.begin ();
           ifaceIter != m_interfaces.end (); ifaceIter++, ifaceIndex++)
        {
          Ptr<Ipv4Interface> outInterface = *ifaceIter;
          if (destination.IsSubnetDirectedBroadcast (
                outInterface->GetNetworkMask ()))
          {
            ipHeader.SetTtl (1);
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

          NS_ASSERT (packetCopy->GetSize () <= outInterface->GetMtu ());
          packetCopy->AddHeader (ipHeader);
          m_txTrace (packetCopy, ifaceIndex);
          outInterface->Send (packetCopy, destination);
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
  NS_ASSERT (packet->GetSize () <= outInterface->GetMtu ());
  m_txTrace (packet, route.GetInterface ());
  if (route.IsGateway ()) 
    {
      NS_LOG_LOGIC ("Send to gateway " << route.GetGateway ());
      outInterface->Send (packet, route.GetGateway ());
    } 
  else 
    {
      NS_LOG_LOGIC ("Send to destination " << ipHeader.GetDestination ());
      outInterface->Send (packet, ipHeader.GetDestination ());
    }
}

bool
Ipv4L3Protocol::Forwarding (
  uint32_t ifIndex, 
  Ptr<Packet> packet, 
  Ipv4Header &ipHeader, 
  Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (ifIndex << packet << &ipHeader<< device);
  NS_LOG_LOGIC ("Forwarding logic for node: " << m_node->GetId ());

  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin ();
       i != m_interfaces.end (); i++) 
    {
      if ((*i)->GetAddress ().IsEqual (ipHeader.GetDestination ())) 
        {
          NS_LOG_LOGIC ("For me (destination match)");
          return false;
        }
    }
  
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin ();
       i != m_interfaces.end (); i++) 
    {
      Ptr<Ipv4Interface> interface = *i;
      if (interface->GetDevice () == device)
	{
	  if (ipHeader.GetDestination ().IsEqual (interface->GetBroadcast ())) 
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

  if (ipHeader.GetTtl () == 1) 
    {
      // Should send ttl expired here
      // XXX
      NS_LOG_LOGIC ("Not for me (TTL expired).  Drop");
      m_dropTrace (packet);
      return true;
    }
  ipHeader.SetTtl (ipHeader.GetTtl () - 1);

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
          Lookup (ifIndex, ipHeader, packet->Copy (),
          MakeCallback (&Ipv4L3Protocol::SendRealOut, this));
          return false;
        }   
    }     
  NS_LOG_LOGIC ("Not for me, forwarding.");
  Lookup (ifIndex, ipHeader, packet,
  MakeCallback (&Ipv4L3Protocol::SendRealOut, this));
  
  return true;
}

void
Ipv4L3Protocol::ForwardUp (Ptr<Packet> p, Ipv4Header const&ip,
                           Ptr<Ipv4Interface> incomingInterface)
{
  NS_LOG_FUNCTION (this << p << &ip);

  Ptr<Ipv4L4Demux> demux = m_node->GetObject<Ipv4L4Demux> ();
  Ptr<Ipv4L4Protocol> protocol = demux->GetProtocol (ip.GetProtocol ());
  protocol->Receive (p, ip.GetSource (), ip.GetDestination (), incomingInterface);
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

void 
Ipv4L3Protocol::SetAddress (uint32_t i, Ipv4Address address)
{
  NS_LOG_FUNCTION (this << i << address);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  interface->SetAddress (address);
}

void 
Ipv4L3Protocol::SetNetworkMask (uint32_t i, Ipv4Mask mask)
{
  NS_LOG_FUNCTION (this << i << mask);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  interface->SetNetworkMask (mask);
}

Ipv4Mask 
Ipv4L3Protocol::GetNetworkMask (uint32_t i) const
{
  NS_LOG_FUNCTION (this << i);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  return interface->GetNetworkMask ();
}

Ipv4Address 
Ipv4L3Protocol::GetAddress (uint32_t i) const
{
  NS_LOG_FUNCTION (this << i);
  Ptr<Ipv4Interface> interface = GetInterface (i);
  return interface->GetAddress ();
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
Ipv4L3Protocol::GetIfIndexForDestination (
  Ipv4Address destination, uint32_t& ifIndex) const
{
  NS_LOG_FUNCTION (this << destination << &ifIndex);
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
      uint32_t ifIndexTmp;

      if ((*i).second->RequestIfIndex (destination, ifIndexTmp))
        {
          NS_LOG_LOGIC ("Found ifIndex " << ifIndexTmp);
          ifIndex = ifIndexTmp;
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
      ifIndex = 1;
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
      NS_LOG_LOGIC ("Ipv4L3Protocol::GetIfIndexForDestination (): "
                    "Unable to determine outbound interface.  No default route set");
      return false;
    }

  ifIndex = route->GetInterface ();

  NS_LOG_LOGIC ("Default route specifies interface " << ifIndex);
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
  if ((interface->GetAddress ()) != (Ipv4Address ())
      && (interface->GetNetworkMask ()) != (Ipv4Mask ()))
    {
      AddNetworkRouteTo (interface->GetAddress ().CombineMask (interface->GetNetworkMask ()),
                         interface->GetNetworkMask (), i);
    }
}

void 
Ipv4L3Protocol::SetDown (uint32_t ifaceIndex)
{
  NS_LOG_FUNCTION (this << ifaceIndex);
  Ptr<Ipv4Interface> interface = GetInterface (ifaceIndex);
  interface->SetDown ();

  // Remove all routes that are going through this interface
  bool modified = true;
  while (modified)
    {
      modified = false;
      for (uint32_t i = 0; i < GetNRoutes (); i++)
        {
          Ipv4Route *route = GetRoute (i);
          if (route->GetInterface () == ifaceIndex)
            {
              RemoveRoute (i);
              modified = true;
              break;
            }
        }
    }
}

}//namespace ns3
