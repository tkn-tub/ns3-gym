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
#include "ns3/array-trace-resolver.h"
#include "ns3/callback.h"

#include "ipv4.h"
#include "ipv4-l4-protocol.h"
#include "ipv4-address.h"
#include "ipv4-header.h"
#include "ipv4-interface.h"
#include "ipv4-route.h"
#include "ipv4-loopback-interface.h"
#include "arp-ipv4-interface.h"
// the two following headers are needed for Ipv4::ForwardUp
#include "node.h"
#include "ipv4-l4-demux.h"

NS_DEBUG_COMPONENT_DEFINE ("Ipv4");

namespace ns3 {

const uint16_t Ipv4::PROT_NUMBER = 0x0800;

Ipv4::Ipv4(Node *node)
  : L3Protocol (PROT_NUMBER, 4),
    m_nInterfaces (0),
    m_defaultTtl (64),
    m_identification (0),
    m_defaultRoute (0),
    m_node (node)
{
  SetupLoopback ();
}
Ipv4::~Ipv4 ()
{
  for (Ipv4InterfaceList::iterator i = m_interfaces.begin (); i != m_interfaces.end (); i++)
    {
      delete (*i);
    }
  for (HostRoutesI i = m_hostRoutes.begin (); 
       i != m_hostRoutes.end (); 
       i = m_hostRoutes.erase (i)) 
    {
      delete (*i);
    }
  for (NetworkRoutesI j = m_networkRoutes.begin (); 
       j != m_networkRoutes.end (); 
       j = m_networkRoutes.erase (j)) 
    {
      delete (*j);
    }
  delete m_defaultRoute;
}

void
Ipv4::SetupLoopback (void)
{
  Ipv4LoopbackInterface * interface = new Ipv4LoopbackInterface (m_node);
  interface->SetAddress (Ipv4Address::GetLoopback ());
  interface->SetNetworkMask (Ipv4Mask::GetLoopback ());
  uint32_t index = AddIpv4Interface (interface);
  AddHostRouteTo (Ipv4Address::GetLoopback (), index);
  interface->SetUp ();
}

TraceResolver *
Ipv4::CreateTraceResolver (TraceContext const &context)
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  resolver->Add ("tx", m_txTrace, Ipv4::TX);
  resolver->Add ("rx", m_rxTrace, Ipv4::RX);
  resolver->Add ("drop", m_dropTrace, Ipv4::DROP);
  resolver->Add ("interfaces", 
                 MakeCallback (&Ipv4::InterfacesCreateTraceResolver, this), 
                 Ipv4::INTERFACES);
  return resolver;
}

TraceResolver *
Ipv4::InterfacesCreateTraceResolver (TraceContext const &context) const
{
  ArrayTraceResolver<Ipv4Interface> *resolver = 
    new ArrayTraceResolver<Ipv4Interface> 
    (context,
     MakeCallback (&Ipv4::GetNInterfaces, this),
     MakeCallback (&Ipv4::GetInterface, this));
  return resolver;
}

void 
Ipv4::SetDefaultTtl (uint8_t ttl)
{
  m_defaultTtl = ttl;
}
    

void 
Ipv4::AddHostRouteTo (Ipv4Address dest, 
                      Ipv4Address nextHop, 
                      uint32_t interface)
{
  Ipv4Route *route = new Ipv4Route ();
  *route = Ipv4Route::CreateHostRouteTo (dest, nextHop, interface);
  m_hostRoutes.push_back (route);
}
void 
Ipv4::AddHostRouteTo (Ipv4Address dest, 
				uint32_t interface)
{
  Ipv4Route *route = new Ipv4Route ();
  *route = Ipv4Route::CreateHostRouteTo (dest, interface);
  m_hostRoutes.push_back (route);
}
void 
Ipv4::AddNetworkRouteTo (Ipv4Address network, 
				   Ipv4Mask networkMask, 
				   Ipv4Address nextHop, 
				   uint32_t interface)
{
  Ipv4Route *route = new Ipv4Route ();
  *route = Ipv4Route::CreateNetworkRouteTo (network,
                                            networkMask,
                                            nextHop,
                                            interface);
  m_networkRoutes.push_back (route);
}
void 
Ipv4::AddNetworkRouteTo (Ipv4Address network, 
				   Ipv4Mask networkMask, 
				   uint32_t interface)
{
  Ipv4Route *route = new Ipv4Route ();
  *route = Ipv4Route::CreateNetworkRouteTo (network,
                                            networkMask,
                                            interface);
  m_networkRoutes.push_back (route);
}
void 
Ipv4::SetDefaultRoute (Ipv4Address nextHop, 
				 uint32_t interface)
{
  Ipv4Route *route = new Ipv4Route ();
  *route = Ipv4Route::CreateDefaultRoute (nextHop, interface);
  delete m_defaultRoute;
  m_defaultRoute = route;
}

Ipv4Route *
Ipv4::Lookup (Ipv4Address dest)
{
  for (HostRoutesCI i = m_hostRoutes.begin (); 
       i != m_hostRoutes.end (); 
       i++) 
    {
      NS_ASSERT ((*i)->IsHost ());
      if ((*i)->GetDest ().IsEqual (dest)) 
        {
          return (*i);
        }
    }
  for (NetworkRoutesI j = m_networkRoutes.begin (); 
       j != m_networkRoutes.end (); 
       j++) 
    {
      NS_ASSERT ((*j)->IsNetwork ());
      Ipv4Mask mask = (*j)->GetDestNetworkMask ();
      Ipv4Address entry = (*j)->GetDestNetwork ();
      if (mask.IsMatch (dest, entry)) 
        {
          return (*j);
        }
    }
  if (m_defaultRoute != 0) 
    {
      NS_ASSERT (m_defaultRoute->IsDefault ());
      return m_defaultRoute;
    }
  return 0;
}

uint32_t 
Ipv4::GetNRoutes (void)
{
  uint32_t n = 0;
  if (m_defaultRoute != 0)
    {
      n++;
    }
  n += m_hostRoutes.size ();
  n += m_networkRoutes.size ();
  return n;
}
Ipv4Route *
Ipv4::GetRoute (uint32_t index)
{
  if (index == 0 && m_defaultRoute != 0)
    {
      return m_defaultRoute;
    }
  if (index > 0 && m_defaultRoute != 0)
    {
      index--;
    }
  if (index < m_hostRoutes.size ())
    {
      uint32_t tmp = 0;
      for (HostRoutesCI i = m_hostRoutes.begin (); 
           i != m_hostRoutes.end (); 
           i++) 
        {
          if (tmp  == index)
            {
              return *i;
            }
          tmp++;
        }
    }
  index -= m_hostRoutes.size ();
  uint32_t tmp = 0;
  for (NetworkRoutesI j = m_networkRoutes.begin (); 
       j != m_networkRoutes.end (); 
       j++) 
    {
      if (tmp == index)
        {
          return *j;
        }
      tmp++;
    }
  NS_ASSERT (false);
  // quiet compiler.
  return 0;
}
void 
Ipv4::RemoveRoute (uint32_t index)
{
  if (index == 0 && m_defaultRoute != 0)
    {
      delete m_defaultRoute;
      m_defaultRoute = 0;
    }
  if (index > 0 && m_defaultRoute != 0)
    {
      index--;
    }
  if (index < m_hostRoutes.size ())
    {
      uint32_t tmp = 0;
      for (HostRoutesI i = m_hostRoutes.begin (); 
           i != m_hostRoutes.end (); 
           i++) 
        {
          if (tmp  == index)
            {
              delete *i;
              m_hostRoutes.erase (i);
              return;
            }
          tmp++;
        }
    }
  index -= m_hostRoutes.size ();
  uint32_t tmp = 0;
  for (NetworkRoutesI j = m_networkRoutes.begin (); 
       j != m_networkRoutes.end (); 
       j++) 
    {
      if (tmp == index)
        {
          delete *j;
          m_networkRoutes.erase (j);
          return;
        }
      tmp++;
    }
  NS_ASSERT (false);
}


uint32_t 
Ipv4::AddInterface (NetDevice *device)
{
  Ipv4Interface *interface = new ArpIpv4Interface (m_node, device);
  return AddIpv4Interface (interface);
}
uint32_t 
Ipv4::AddIpv4Interface (Ipv4Interface *interface)
{
  uint32_t index = m_nInterfaces;
  m_interfaces.push_back (interface);
  m_nInterfaces++;
  return index;
}
Ipv4Interface *
Ipv4::GetInterface (uint32_t index) const
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
Ipv4::GetNInterfaces (void) const
{
  return m_nInterfaces;
}

Ipv4Interface *
Ipv4::FindInterfaceForDevice (NetDevice const*device)
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

Ipv4* 
Ipv4::Copy(Node *node) const
{
  Ipv4 *ipv4 = new Ipv4 (node);
  ipv4->SetDefaultTtl (m_defaultTtl);
  return ipv4;
}
void 
Ipv4::Receive(Packet& packet, NetDevice &device)
{
  m_rxTrace (packet);
  Ipv4Header ipHeader;
  packet.Peek (ipHeader);
  packet.Remove (ipHeader);

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
Ipv4::Send (Packet const &packet, 
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

  // XXX Note here that in most ipv4 stacks in the world,
  // the route calculation for an outgoing packet is not
  // done in the ip layer. It is done within the application
  // socket when the first packet is sent to avoid this
  // costly lookup on a per-packet basis.
  // That would require us to get the route from the packet,
  // most likely with a packet tag. The higher layers do not
  // do this yet for us.
  Ipv4Route *route = Lookup (ipHeader.GetDestination ());
  if (route == 0) 
    {
      NS_DEBUG ("not for me -- forwarding but no route to host. drop.");
      m_dropTrace (packet);
      return;
    }

  SendRealOut (packet, ipHeader, *route);
}

void
Ipv4::SendRealOut (Packet const &p, Ipv4Header const &ip, Ipv4Route const &route)
{
  Packet packet = p;
  packet.Add (ip);
  Ipv4Interface *outInterface = GetInterface (route.GetInterface ());
  NS_ASSERT (packet.GetSize () <= outInterface->GetMtu ());
  m_txTrace (packet);
  if (route.IsGateway ()) 
    {
      outInterface->Send (packet, route.GetGateway ());
    } 
  else 
    {
      outInterface->Send (packet, ip.GetDestination ());
    }
}


bool
Ipv4::Forwarding (Packet const &packet, Ipv4Header &ipHeader, NetDevice &device)
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
      Ipv4Interface *interface = *i;
      if (interface->GetDevice () == &device)
	{
	  if (ipHeader.GetDestination ().IsEqual (interface->GetBroadcast ())) 
	    {
	      NS_DEBUG ("for me 2");
	      return false;
	    }
	  break;
	}
    }
      
  if (ipHeader.GetDestination ().IsEqual (Ipv4Address::GetBroadcast ())) 
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
  Ipv4Route *route = Lookup (ipHeader.GetDestination ());
  if (route == 0) 
    {
      NS_DEBUG ("not for me -- forwarding but no route to host. drop.");
      m_dropTrace (packet);
      return true;
    }
  NS_DEBUG ("not for me -- forwarding.");
  SendRealOut (packet, ipHeader, *route);
  return true;
}


void
Ipv4::ForwardUp (Packet p, Ipv4Header const&ip)
{
  Ipv4L4Protocol *protocol = m_node->GetIpv4L4Demux ()->Lookup (ip.GetProtocol ());
  protocol->Receive (p, ip.GetSource (), ip.GetDestination ());
}

void 
Ipv4::SetAddress (uint32_t i, Ipv4Address address)
{
  Ipv4Interface *interface = GetInterface (i);
  interface->SetAddress (address);
}
void 
Ipv4::SetNetworkMask (uint32_t i, Ipv4Mask mask)
{
  Ipv4Interface *interface = GetInterface (i);
  interface->SetNetworkMask (mask);
}
Ipv4Mask 
Ipv4::GetNetworkMask (uint32_t i) const
{
  Ipv4Interface *interface = GetInterface (i);
  return interface->GetNetworkMask ();
}
Ipv4Address 
Ipv4::GetAddress (uint32_t i) const
{
  Ipv4Interface *interface = GetInterface (i);
  return interface->GetAddress ();
}
uint16_t 
Ipv4::GetMtu (uint32_t i) const
{
  Ipv4Interface *interface = GetInterface (i);
  return interface->GetMtu ();
}
bool 
Ipv4::IsUp (uint32_t i) const
{
  Ipv4Interface *interface = GetInterface (i);
  return interface->IsUp ();
}
void 
Ipv4::SetUp (uint32_t i)
{
  Ipv4Interface *interface = GetInterface (i);
  interface->SetUp ();
}
void 
Ipv4::SetDown (uint32_t i)
{
  Ipv4Interface *interface = GetInterface (i);
  interface->SetDown ();
}


}//namespace ns3
