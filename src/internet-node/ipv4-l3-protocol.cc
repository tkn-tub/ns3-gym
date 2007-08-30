// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
// All rights reserved.
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
#include "ns3/debug.h"
#include "ns3/composite-trace-resolver.h"
#include "ns3/callback.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-route.h"
#include "ns3/node.h"
#include "ns3/net-device.h"

#include "ipv4-l3-protocol.h"
#include "ipv4-l4-protocol.h"
#include "ipv4-header.h"
#include "ipv4-interface.h"
#include "ipv4-loopback-interface.h"
#include "arp-ipv4-interface.h"
#include "ipv4-l4-demux.h"

NS_DEBUG_COMPONENT_DEFINE ("Ipv4L3Protocol");

namespace ns3 {

const InterfaceId Ipv4L3Protocol::iid = MakeInterfaceId ("Ipv4L3Protocol", Object::iid);
const uint16_t Ipv4L3Protocol::PROT_NUMBER = 0x0800;

Ipv4L3ProtocolTraceContextElement::Ipv4L3ProtocolTraceContextElement ()
  : m_type (TX)
{}
Ipv4L3ProtocolTraceContextElement::Ipv4L3ProtocolTraceContextElement (enum Type type)
  : m_type (type)
{}
bool 
Ipv4L3ProtocolTraceContextElement::IsTx (void) const
{
  return m_type == TX;
}
bool 
Ipv4L3ProtocolTraceContextElement::IsRx (void) const
{
  return m_type == RX;
}
bool 
Ipv4L3ProtocolTraceContextElement::IsDrop (void) const
{
  return m_type == DROP;
}
void 
Ipv4L3ProtocolTraceContextElement::Print (std::ostream &os) const
{
  os << "ipv4=";
  switch (m_type)
    {
    case TX:
      os << "tx";
      break;
    case RX:
      os << "rx";
      break;
    case DROP:
      os << "drop";
      break;
    }
}
uint16_t 
Ipv4L3ProtocolTraceContextElement::GetUid (void)
{
  static uint16_t uid = AllocateUid<Ipv4L3ProtocolTraceContextElement> ("Ipv4L3ProtocolTraceContextElement");
  return uid;
}
std::string 
Ipv4L3ProtocolTraceContextElement::GetTypeName (void) const
{
  return "ns3::Ipv4L3ProtocolTraceContextElement";
}


Ipv4L3ProtocolInterfaceIndex::Ipv4L3ProtocolInterfaceIndex ()
  : m_index (0)
{}
Ipv4L3ProtocolInterfaceIndex::Ipv4L3ProtocolInterfaceIndex (uint32_t index)
  : m_index (index)
{}
uint32_t 
Ipv4L3ProtocolInterfaceIndex::Get (void) const
{
  return m_index;
}
void 
Ipv4L3ProtocolInterfaceIndex::Print (std::ostream &os) const
{
  os << "ipv4-interface=" << m_index;
}
uint16_t 
Ipv4L3ProtocolInterfaceIndex::GetUid (void)
{
  static uint16_t uid = AllocateUid<Ipv4L3ProtocolInterfaceIndex> ("Ipv4L3ProtocolInterfaceIndex");
  return uid;
}
std::string
Ipv4L3ProtocolInterfaceIndex::GetTypeName (void) const
{
  return "ns3::Ipv4L3ProtocolInterfaceIndex";
}


Ipv4L3Protocol::Ipv4L3Protocol(Ptr<Node> node)
  : m_nInterfaces (0),
    m_defaultTtl (64),
    m_identification (0),
    m_node (node)
{
  SetInterfaceId (Ipv4L3Protocol::iid);
  m_staticRouting = Create<Ipv4StaticRouting> ();
  AddRoutingProtocol (m_staticRouting, 0);
  SetupLoopback ();
}
Ipv4L3Protocol::~Ipv4L3Protocol ()
{}

void 
Ipv4L3Protocol::DoDispose (void)
{
  m_interfaces.clear ();
  m_node = 0;
  m_staticRouting->Dispose ();
  m_staticRouting = 0;
  Object::DoDispose ();
}

void
Ipv4L3Protocol::SetupLoopback (void)
{
  Ptr<Ipv4LoopbackInterface> interface = Create<Ipv4LoopbackInterface> (m_node);
  interface->SetAddress (Ipv4Address::GetLoopback ());
  interface->SetNetworkMask (Ipv4Mask::GetLoopback ());
  uint32_t index = AddIpv4Interface (interface);
  AddHostRouteTo (Ipv4Address::GetLoopback (), index);
  interface->SetUp ();
}

Ptr<TraceResolver>
Ipv4L3Protocol::GetTraceResolver (void) const
{
  Ptr<CompositeTraceResolver> resolver = Create<CompositeTraceResolver> ();
  resolver->AddSource ("tx", 
                       TraceDoc ("send ipv4 packet to outgoing interface",
                                 "const Packet &", "packet sent",
                                 "uint32_t", "index of output ipv4 interface"),
                       m_txTrace, Ipv4L3ProtocolTraceContextElement(Ipv4L3ProtocolTraceContextElement::TX));
  resolver->AddSource ("rx",
                       TraceDoc ("receive ipv4 packet from incoming interface",
                                 "const Packet &", "packet received",
                                 "uint32_t", "index of input ipv4 interface"),
                       m_rxTrace, Ipv4L3ProtocolTraceContextElement(Ipv4L3ProtocolTraceContextElement::RX));
  resolver->AddSource ("drop", 
                       TraceDoc ("drop ipv4 packet",
                                 "const Packet &", "packet dropped"),
                       m_dropTrace, Ipv4L3ProtocolTraceContextElement (Ipv4L3ProtocolTraceContextElement::DROP));
  resolver->AddArray ("interfaces", 
                      m_interfaces.begin (), m_interfaces.end (), 
                      Ipv4L3ProtocolInterfaceIndex ());
  return resolver;
}

void 
Ipv4L3Protocol::SetDefaultTtl (uint8_t ttl)
{
  m_defaultTtl = ttl;
}
    

void 
Ipv4L3Protocol::AddHostRouteTo (Ipv4Address dest, 
                      Ipv4Address nextHop, 
                      uint32_t interface)
{
  m_staticRouting->AddHostRouteTo (dest, nextHop, interface);
}
void 
Ipv4L3Protocol::AddHostRouteTo (Ipv4Address dest, 
				uint32_t interface)
{
  m_staticRouting->AddHostRouteTo (dest, interface);
}
void 
Ipv4L3Protocol::AddNetworkRouteTo (Ipv4Address network, 
				   Ipv4Mask networkMask, 
				   Ipv4Address nextHop, 
				   uint32_t interface)
{
  m_staticRouting->AddNetworkRouteTo (network, networkMask, nextHop, interface);
}
void 
Ipv4L3Protocol::AddNetworkRouteTo (Ipv4Address network, 
				   Ipv4Mask networkMask, 
				   uint32_t interface)
{
  m_staticRouting->AddNetworkRouteTo (network, networkMask, interface);
}
void 
Ipv4L3Protocol::SetDefaultRoute (Ipv4Address nextHop, 
				 uint32_t interface)
{
  m_staticRouting->SetDefaultRoute (nextHop, interface);
}


void
Ipv4L3Protocol::Lookup (Ipv4Header const &ipHeader,
                        Packet packet,
                        Ipv4RoutingProtocol::RouteReplyCallback routeReply)
{
  for (Ipv4RoutingProtocolList::const_iterator rprotoIter = m_routingProtocols.begin ();
       rprotoIter != m_routingProtocols.end (); rprotoIter++)
    {
      if ((*rprotoIter).second->RequestRoute (ipHeader, packet, routeReply))
        return;
    }
  // No route found
  routeReply (false, Ipv4Route (), packet, ipHeader);
}

void
Ipv4L3Protocol::AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol,
                                    int priority)
{
  m_routingProtocols.push_back
    (std::pair<int, Ptr<Ipv4RoutingProtocol> > (-priority, routingProtocol));
  m_routingProtocols.sort ();
}

uint32_t 
Ipv4L3Protocol::GetNRoutes (void)
{
  return m_staticRouting->GetNRoutes ();
}

Ipv4Route *
Ipv4L3Protocol::GetRoute (uint32_t index)
{
  return m_staticRouting->GetRoute (index);
}

void 
Ipv4L3Protocol::RemoveRoute (uint32_t index)
{
  m_staticRouting->RemoveRoute (index);
}


uint32_t 
Ipv4L3Protocol::AddInterface (Ptr<NetDevice> device)
{
  Ptr<Ipv4Interface> interface = Create<ArpIpv4Interface> (m_node, device);
  return AddIpv4Interface (interface);
}
uint32_t 
Ipv4L3Protocol::AddIpv4Interface (Ptr<Ipv4Interface>interface)
{
  uint32_t index = m_nInterfaces;
  m_interfaces.push_back (interface);
  m_nInterfaces++;
  return index;
}
Ptr<Ipv4Interface>
Ipv4L3Protocol::GetInterface (uint32_t index) const
{
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
  return m_nInterfaces;
}

Ptr<Ipv4Interface>
Ipv4L3Protocol::FindInterfaceForDevice (Ptr<const NetDevice> device)
{
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
Ipv4L3Protocol::Receive( Ptr<NetDevice> device, const Packet& p, uint16_t protocol, const Address &from)
{
  uint32_t index = 0;
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin (); i != m_interfaces.end (); i++)
    {
      if ((*i)->GetDevice () == device)
        {
          m_rxTrace (p, index);
          break;
        }
      index++;
    }
  Packet packet = p;
  Ipv4Header ipHeader;
  packet.RemoveHeader (ipHeader);

  if (!ipHeader.IsChecksumOk ()) 
    {
      return;
    }

  if (Forwarding (packet, ipHeader, device)) 
    {
      return;
    }

  ForwardUp (packet, ipHeader);
}


void 
Ipv4L3Protocol::Send (Packet const &packet, 
            Ipv4Address source, 
            Ipv4Address destination,
            uint8_t protocol)
{
  Ipv4Header ipHeader;

  ipHeader.SetSource (source);
  ipHeader.SetDestination (destination);
  ipHeader.SetProtocol (protocol);
  ipHeader.SetPayloadSize (packet.GetSize ());
  ipHeader.SetTtl (m_defaultTtl);
  ipHeader.SetMayFragment ();
  ipHeader.SetIdentification (m_identification);

  m_identification ++;

  if (destination.IsBroadcast ())
    {
      uint32_t ifaceIndex = 0;
      for (Ipv4InterfaceList::iterator ifaceIter = m_interfaces.begin ();
           ifaceIter != m_interfaces.end (); ifaceIter++, ifaceIndex++)
        {
          Ptr<Ipv4Interface> outInterface = *ifaceIter;
          Packet packetCopy = packet;

          NS_ASSERT (packetCopy.GetSize () <= outInterface->GetMtu ());
          packetCopy.AddHeader (ipHeader);
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
                             Packet packet,
                             Ipv4Header const &ipHeader)
{
  if (!found)
    {
      NS_DEBUG ("no route to host. drop.");
      m_dropTrace (packet);
      return;
    }
  packet.AddHeader (ipHeader);
  Ptr<Ipv4Interface> outInterface = GetInterface (route.GetInterface ());
  NS_ASSERT (packet.GetSize () <= outInterface->GetMtu ());
  m_txTrace (packet, route.GetInterface ());
  if (route.IsGateway ()) 
    {
      outInterface->Send (packet, route.GetGateway ());
    } 
  else 
    {
      outInterface->Send (packet, ipHeader.GetDestination ());
    }
}


bool
Ipv4L3Protocol::Forwarding (Packet const &packet, Ipv4Header &ipHeader, Ptr<NetDevice> device)
{
  for (Ipv4InterfaceList::const_iterator i = m_interfaces.begin ();
       i != m_interfaces.end (); i++) 
    {
      if ((*i)->GetAddress ().IsEqual (ipHeader.GetDestination ())) 
        {
          NS_DEBUG ("for me 1");
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
	      NS_DEBUG ("for me 2");
	      return false;
	    }
	  break;
	}
    }
      
  if (ipHeader.GetDestination ().IsBroadcast ()) 
    {
      NS_DEBUG ("for me 3");
      return false;
    }
  if (ipHeader.GetDestination ().IsEqual (Ipv4Address::GetAny ())) 
    {
      NS_DEBUG ("for me 4");
      return false;
    }
  if (ipHeader.GetTtl () == 1) 
    {
      // Should send ttl expired here
      // XXX
      NS_DEBUG ("not for me -- ttl expired. drop.");
      m_dropTrace (packet);
      return true;
    }
  ipHeader.SetTtl (ipHeader.GetTtl () - 1);

  NS_DEBUG ("not for me -- forwarding.");
  Lookup (ipHeader, packet,
          MakeCallback (&Ipv4L3Protocol::SendRealOut, this));
  return true;
}


void
Ipv4L3Protocol::ForwardUp (Packet p, Ipv4Header const&ip)
{
  Ptr<Ipv4L4Demux> demux = m_node->QueryInterface<Ipv4L4Demux> (Ipv4L4Demux::iid);
  Ptr<Ipv4L4Protocol> protocol = demux->GetProtocol (ip.GetProtocol ());
  protocol->Receive (p, ip.GetSource (), ip.GetDestination ());
}

void 
Ipv4L3Protocol::SetAddress (uint32_t i, Ipv4Address address)
{
  Ptr<Ipv4Interface> interface = GetInterface (i);
  interface->SetAddress (address);
}
void 
Ipv4L3Protocol::SetNetworkMask (uint32_t i, Ipv4Mask mask)
{
  Ptr<Ipv4Interface> interface = GetInterface (i);
  interface->SetNetworkMask (mask);
}
Ipv4Mask 
Ipv4L3Protocol::GetNetworkMask (uint32_t i) const
{
  Ptr<Ipv4Interface> interface = GetInterface (i);
  return interface->GetNetworkMask ();
}
Ipv4Address 
Ipv4L3Protocol::GetAddress (uint32_t i) const
{
  Ptr<Ipv4Interface> interface = GetInterface (i);
  return interface->GetAddress ();
}
uint16_t 
Ipv4L3Protocol::GetMtu (uint32_t i) const
{
  Ptr<Ipv4Interface> interface = GetInterface (i);
  return interface->GetMtu ();
}
bool 
Ipv4L3Protocol::IsUp (uint32_t i) const
{
  Ptr<Ipv4Interface> interface = GetInterface (i);
  return interface->IsUp ();
}
void 
Ipv4L3Protocol::SetUp (uint32_t i)
{
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
