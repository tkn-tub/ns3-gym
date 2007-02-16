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

#include "ipv4.h"
#include "ipv4-l4-protocol.h"
#include "ipv4-address.h"
#include "ipv4-header.h"
#include "ipv4-interface.h"
#include "ipv4-route.h"
// the two following headers are needed for Ipv4::ForwardUp
#include "node.h"
#include "ipv4-l4-demux.h"

#define TRACE(x)

namespace ns3 {

Ipv4::Ipv4(Node *node)
  : m_nInterfaces (0),
    m_defaultTtl (64),
    m_identification (0),
    m_defaultRoute (0),
    m_node (node)
{}
Ipv4::~Ipv4 ()
{
  // XXX I am not sure we are really allowed to do this here.
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
Ipv4::AddInterface (Ipv4Interface *interface)
{
  uint32_t index = m_nInterfaces;
  m_interfaces.push_back (interface);
  m_nInterfaces++;
  return index;
}
Ipv4Interface *
Ipv4::GetInterface (uint32_t index)
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
  // XXX trace here.
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
      TRACE ("not for me -- forwarding but no route to host. drop.");
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
  // XXX log trace here.
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
          TRACE ("for me 1");
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
	      TRACE ("for me 2");
	      return false;
	    }
	  break;
	}
    }
      
  if (ipHeader.GetDestination ().IsEqual (Ipv4Address::GetBroadcast ())) 
    {
      TRACE ("for me 3");
      return false;
    }
  if (ipHeader.GetDestination ().IsEqual (Ipv4Address::GetAny ())) 
    {
      TRACE ("for me 4");
      return false;
    }
  if (ipHeader.GetTtl () == 1) 
    {
      // Should send ttl expired here
      // XXX
      TRACE ("not for me -- ttl expired. drop.");
      return true;
    }
  ipHeader.SetTtl (ipHeader.GetTtl () - 1);
  Ipv4Route *route = Lookup (ipHeader.GetDestination ());
  if (route == 0) 
    {
      TRACE ("not for me -- forwarding but no route to host. drop.");
      return true;
    }
  TRACE ("not for me -- forwarding.");
  SendRealOut (packet, ipHeader, *route);
  return true;
}


void
Ipv4::ForwardUp (Packet p, Ipv4Header const&ip)
{
  Ipv4L4Protocol *protocol = m_node->GetIpv4L4Demux ()->Lookup (ip.GetProtocol ());
  protocol->Receive (p, ip.GetSource (), ip.GetDestination ());
}

}//namespace ns3
