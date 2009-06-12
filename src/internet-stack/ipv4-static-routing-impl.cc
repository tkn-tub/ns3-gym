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
//         Gustavo Carneiro <gjc@inescporto.pt>

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/ipv4-route.h"
#include "ipv4-static-routing-impl.h"
#include "ns3/ipv4-routing-table-entry.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4StaticRoutingImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Ipv4StaticRoutingImpl);

TypeId
Ipv4StaticRoutingImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4StaticRoutingImpl")
    .SetParent<Ipv4StaticRouting> ()
    .AddConstructor<Ipv4StaticRoutingImpl> ()
    ;
  return tid;
}

Ipv4StaticRoutingImpl::Ipv4StaticRoutingImpl () 
: m_defaultRoute (0), m_ipv4 (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
Ipv4StaticRoutingImpl::AddHostRouteTo (Ipv4Address dest, 
                                   Ipv4Address nextHop, 
                                   uint32_t interface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4RoutingTableEntry *route = new Ipv4RoutingTableEntry ();
  *route = Ipv4RoutingTableEntry::CreateHostRouteTo (dest, nextHop, interface);
  m_hostRoutes.push_back (route);
}

void 
Ipv4StaticRoutingImpl::AddHostRouteTo (Ipv4Address dest, 
                                   uint32_t interface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4RoutingTableEntry *route = new Ipv4RoutingTableEntry ();
  *route = Ipv4RoutingTableEntry::CreateHostRouteTo (dest, interface);
  m_hostRoutes.push_back (route);
}

void 
Ipv4StaticRoutingImpl::AddNetworkRouteTo (Ipv4Address network, 
                                      Ipv4Mask networkMask, 
                                      Ipv4Address nextHop, 
                                      uint32_t interface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4RoutingTableEntry *route = new Ipv4RoutingTableEntry ();
  *route = Ipv4RoutingTableEntry::CreateNetworkRouteTo (network,
                                            networkMask,
                                            nextHop,
                                            interface);
  m_networkRoutes.push_back (route);
}

void 
Ipv4StaticRoutingImpl::AddNetworkRouteTo (Ipv4Address network, 
                                      Ipv4Mask networkMask, 
                                      uint32_t interface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4RoutingTableEntry *route = new Ipv4RoutingTableEntry ();
  *route = Ipv4RoutingTableEntry::CreateNetworkRouteTo (network,
                                            networkMask,
                                            interface);
  m_networkRoutes.push_back (route);
}

void 
Ipv4StaticRoutingImpl::SetDefaultRoute (Ipv4Address nextHop, 
                                    uint32_t interface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4RoutingTableEntry *route = new Ipv4RoutingTableEntry ();
  *route = Ipv4RoutingTableEntry::CreateDefaultRoute (nextHop, interface);
  delete m_defaultRoute;
  m_defaultRoute = route;
}

void 
Ipv4StaticRoutingImpl::AddMulticastRoute(Ipv4Address origin,
                                     Ipv4Address group,
                                     uint32_t inputInterface,
                                     std::vector<uint32_t> outputInterfaces)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4MulticastRoutingTableEntry *route = new Ipv4MulticastRoutingTableEntry ();
  *route = Ipv4MulticastRoutingTableEntry::CreateMulticastRoute (origin, group, 
    inputInterface, outputInterfaces);
  m_multicastRoutes.push_back (route);
}

// default multicast routes are stored as a network route
// these routes are _not_ consulted in the forwarding process-- only
// for originating packets
void 
Ipv4StaticRoutingImpl::SetDefaultMulticastRoute(uint32_t outputInterface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4RoutingTableEntry *route = new Ipv4RoutingTableEntry ();
  Ipv4Address network = Ipv4Address ("224.0.0.0");
  Ipv4Mask networkMask = Ipv4Mask ("240.0.0.0");
  *route = Ipv4RoutingTableEntry::CreateNetworkRouteTo (network,
                                            networkMask,
                                            outputInterface);
  m_networkRoutes.push_back (route);
}

uint32_t 
Ipv4StaticRoutingImpl::GetNMulticastRoutes (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_multicastRoutes.size ();
}

Ipv4MulticastRoutingTableEntry
Ipv4StaticRoutingImpl::GetMulticastRoute (uint32_t index) const
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT_MSG(index < m_multicastRoutes.size (),
    "Ipv4StaticRoutingImpl::GetMulticastRoute ():  Index out of range");

  if (index < m_multicastRoutes.size ())
    {
      uint32_t tmp = 0;
      for (MulticastRoutesCI i = m_multicastRoutes.begin (); 
           i != m_multicastRoutes.end (); 
           i++) 
        {
          if (tmp  == index)
            {
              return *i;
            }
          tmp++;
        }
    }
  return 0;
}

bool
Ipv4StaticRoutingImpl::RemoveMulticastRoute(Ipv4Address origin,
                                        Ipv4Address group,
                                        uint32_t inputInterface)
{
  NS_LOG_FUNCTION_NOARGS ();
  for (MulticastRoutesI i = m_multicastRoutes.begin (); 
       i != m_multicastRoutes.end (); 
       i++) 
    {
      Ipv4MulticastRoutingTableEntry *route = *i;
      if (origin == route->GetOrigin () &&
          group == route->GetGroup () &&
          inputInterface == route->GetInputInterface ())
        {
          delete *i;
          m_multicastRoutes.erase (i);
          return true;
        }
    }
  return false;
}

void 
Ipv4StaticRoutingImpl::RemoveMulticastRoute(uint32_t index)
{
  NS_LOG_FUNCTION_NOARGS ();
  uint32_t tmp = 0;
  for (MulticastRoutesI i = m_multicastRoutes.begin (); 
       i != m_multicastRoutes.end (); 
       i++) 
    {
      if (tmp  == index)
        {
          delete *i;
          m_multicastRoutes.erase (i);
          return;
        }
      tmp++;
    }
}

Ptr<Ipv4Route>
Ipv4StaticRoutingImpl::LookupStatic (Ipv4Address dest)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<Ipv4Route> rtentry = 0;
  for (HostRoutesCI i = m_hostRoutes.begin (); 
       i != m_hostRoutes.end (); 
       i++) 
    {
      NS_ASSERT ((*i)->IsHost ());
      if ((*i)->GetDest ().IsEqual (dest)) 
        {
          NS_LOG_LOGIC ("Found global host route" << *i);
          Ipv4RoutingTableEntry* route = (*i);
          rtentry = Create<Ipv4Route> ();
          rtentry->SetDestination (route->GetDest ());
          // XXX handle multi-address case
          rtentry->SetSource (m_ipv4->GetAddress (route->GetInterface(), 0).GetLocal ());
          rtentry->SetGateway (route->GetGateway ());
          uint32_t interfaceIdx = route->GetInterface ();
          rtentry->SetOutputDevice (m_ipv4->GetNetDevice (interfaceIdx));
          return rtentry;
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
          NS_LOG_LOGIC ("Found global network route" << *j);
          Ipv4RoutingTableEntry* route = (*j);
          rtentry = Create<Ipv4Route> ();
          rtentry->SetDestination (route->GetDest ());
          // XXX handle multi-address case
          rtentry->SetSource (m_ipv4->GetAddress (route->GetInterface(), 0).GetLocal ());
          rtentry->SetGateway (route->GetGateway ());
          uint32_t interfaceIdx = route->GetInterface ();
          rtentry->SetOutputDevice (m_ipv4->GetNetDevice (interfaceIdx));
          return rtentry;
        }
    }
  if (m_defaultRoute != 0) 
    {
      NS_ASSERT (m_defaultRoute->IsDefault ());
      NS_LOG_LOGIC ("Found global network route" << m_defaultRoute);
      Ipv4RoutingTableEntry* route = m_defaultRoute;
      rtentry = Create<Ipv4Route> ();
      rtentry->SetDestination (route->GetDest ());
      // XXX handle multi-address case
      rtentry->SetSource (m_ipv4->GetAddress (route->GetInterface(), 0).GetLocal ());
      rtentry->SetGateway (route->GetGateway ());
      uint32_t interfaceIdx = route->GetInterface ();
      rtentry->SetOutputDevice (m_ipv4->GetNetDevice (interfaceIdx));
      return rtentry;
    }
  return 0;
}

Ptr<Ipv4MulticastRoute>
Ipv4StaticRoutingImpl::LookupStatic (
  Ipv4Address origin, 
  Ipv4Address group,
  uint32_t    interface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ptr<Ipv4MulticastRoute> mrtentry = 0;

  for (MulticastRoutesI i = m_multicastRoutes.begin (); 
       i != m_multicastRoutes.end (); 
       i++) 
    {
      Ipv4MulticastRoutingTableEntry *route = *i;
//
// We've been passed an origin address, a multicast group address and an 
// interface index.  We have to decide if the current route in the list is
// a match.
//
// The first case is the restrictive case where the origin, group and index
// matches.  
//
      if (origin == route->GetOrigin () && group == route->GetGroup ())
        {
          // Skipping this case (SSM) for now
          NS_LOG_LOGIC ("Found multicast source specific route" << *i);
        }
      if (group == route->GetGroup ())
        {
          if (interface == Ipv4::IF_ANY || 
              interface == route->GetInputInterface ())
            {
              NS_LOG_LOGIC ("Found multicast route" << *i);
              mrtentry = Create<Ipv4MulticastRoute> ();
              mrtentry->SetGroup (route->GetGroup ());
              mrtentry->SetOrigin (route->GetOrigin ());
              mrtentry->SetParent (route->GetInputInterface ());
              for (uint32_t j = 0; j < route->GetNOutputInterfaces (); j++)
                {
                  if (route->GetOutputInterface (j))
                    {
                      NS_LOG_LOGIC ("Setting output interface index " << route->GetOutputInterface (j));
                      mrtentry->SetOutputTtl (route->GetOutputInterface (j), Ipv4MulticastRoute::MAX_TTL - 1);
                    }
                }
              return mrtentry;
            }
        }
    }
  return mrtentry;
}

uint32_t 
Ipv4StaticRoutingImpl::GetNRoutes (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  uint32_t n = 0;
  if (m_defaultRoute != 0)
    {
      n++;
    }
  n += m_hostRoutes.size ();
  n += m_networkRoutes.size ();
  return n;
}

Ipv4RoutingTableEntry
Ipv4StaticRoutingImpl::GetDefaultRoute ()
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_defaultRoute != 0)
    {
      return *m_defaultRoute;
    }
  else
    {
      return Ipv4RoutingTableEntry ();
    }
}

Ipv4RoutingTableEntry 
Ipv4StaticRoutingImpl::GetRoute (uint32_t index)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (index == 0 && m_defaultRoute != 0)
    {
      return *m_defaultRoute;
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
Ipv4StaticRoutingImpl::RemoveRoute (uint32_t index)
{
  NS_LOG_FUNCTION_NOARGS ();
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

Ptr<Ipv4Route> 
Ipv4StaticRoutingImpl::RouteOutput (const Ipv4Header &header, uint32_t oif, Socket::SocketErrno &sockerr)
{
  NS_LOG_FUNCTION (this << header << oif);
  Ipv4Address destination = header.GetDestination ();
  Ptr<Ipv4Route> rtentry = 0;

  // Multicast goes here
  if (destination.IsMulticast ())
    {
      // Note:  Multicast routes for outbound packets are stored in the
      // normal unicast table.  An implication of this is that it is not
      // possible to source multicast datagrams on multiple interfaces.
      // This is a well-known property of sockets implementation on 
      // many Unix variants.
      // So, we just log it and fall through to LookupStatic ()
      NS_LOG_LOGIC ("RouteOutput()::Multicast destination");
    }
  rtentry = LookupStatic (destination);
  if (rtentry)
    { 
      sockerr = Socket::ERROR_NOTERROR;
    }
  else
    { 
      sockerr = Socket::ERROR_NOROUTETOHOST;
    }
  return rtentry;
}

// XXX this method not robust enough to work independent of ListRouting
bool 
Ipv4StaticRoutingImpl::RouteInput  (Ptr<const Packet> p, const Ipv4Header &ipHeader, Ptr<const NetDevice> idev,
                             UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                             LocalDeliverCallback lcb, ErrorCallback ecb)
{
  NS_LOG_FUNCTION (this << p << ipHeader << ipHeader.GetSource () << ipHeader.GetDestination () << idev);

  if (ipHeader.GetDestination ().IsMulticast ())
    {
      NS_LOG_LOGIC ("Multicast destination");
      Ptr<Ipv4MulticastRoute> mrtentry =  LookupStatic(ipHeader.GetSource (),
        ipHeader.GetDestination (), m_ipv4->GetInterfaceForDevice (idev));

      if (mrtentry)
        {
          NS_LOG_LOGIC ("Multicast route found");
          mcb (mrtentry, p, ipHeader); // multicast forwarding callback
          return true;
        }
      else
        {
          NS_LOG_LOGIC ("Multicast route not found");
          return false; // Let other routing protocols try to handle this
        }
    }
//
// This is a unicast packet.  Check to see if we have a route for it.
//
  NS_LOG_LOGIC ("Unicast destination");
  Ptr<Ipv4Route> rtentry = LookupStatic (ipHeader.GetDestination ());
  if (rtentry != 0)
    {
      NS_LOG_LOGIC ("Found unicast destination- calling unicast callback");
      ucb (rtentry, p, ipHeader);  // unicast forwarding callback
      return true;
    }
  else
    {
      NS_LOG_LOGIC ("Did not find unicast destination- returning false");
      return false; // Let other routing protocols try to handle this
    }
}

Ipv4StaticRoutingImpl::~Ipv4StaticRoutingImpl ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
Ipv4StaticRoutingImpl::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
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
  if (m_defaultRoute != 0)
    {
      delete m_defaultRoute;
      m_defaultRoute = 0;
    }
  for (MulticastRoutesI i = m_multicastRoutes.begin (); 
       i != m_multicastRoutes.end (); 
       i = m_multicastRoutes.erase (i)) 
    {
      delete (*i);
    }
  m_ipv4 = 0;
  Ipv4RoutingProtocol::DoDispose ();
}

void 
Ipv4StaticRoutingImpl::NotifyInterfaceUp (uint32_t i)
{
  // If interface address and network mask have been set, add a route
  // to the network of the interface (like e.g. ifconfig does on a
  // Linux box)
  for (uint32_t j = 0; j < m_ipv4->GetNAddresses (i); j++)
    {
      if (m_ipv4->GetAddress (i,j).GetLocal () != Ipv4Address () &&
          m_ipv4->GetAddress (i,j).GetMask () != Ipv4Mask ())
        {
          AddNetworkRouteTo (m_ipv4->GetAddress (i,j).GetLocal ().CombineMask (m_ipv4->GetAddress (i,j).GetMask ()),
                             m_ipv4->GetAddress (i,j).GetMask (), i);
        }
    }
}

void 
Ipv4StaticRoutingImpl::NotifyInterfaceDown (uint32_t i)
{
  // Remove all static routes that are going through this interface
  for (uint32_t j = 0; j < GetNRoutes (); j++)
    {
      Ipv4RoutingTableEntry route = GetRoute (j);
      if (route.GetInterface () == i)
        {
          RemoveRoute (j);
        }
    }
}

void 
Ipv4StaticRoutingImpl::NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  NS_LOG_FUNCTION (this << interface);
  if (!m_ipv4->IsUp (interface))
    {
      return;
    }

  Ipv4Address networkAddress = address.GetLocal ().CombineMask (address.GetMask ());
  Ipv4Mask networkMask = address.GetMask ();
  if (address.GetLocal () != Ipv4Address () &&
      address.GetMask () != Ipv4Mask ())
    {
      AddNetworkRouteTo (networkAddress,
                         networkMask, interface);
    }
}
void 
Ipv4StaticRoutingImpl::NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
  if (!m_ipv4->IsUp (interface))
    {
      return;
    }
  Ipv4Address networkAddress = address.GetLocal ().CombineMask (address.GetMask ());
  Ipv4Mask networkMask = address.GetMask ();
  // Remove all static routes that are going through this interface
  // which reference this network
  for (uint32_t j = 0; j < GetNRoutes (); j++)
    {
      Ipv4RoutingTableEntry route = GetRoute (j);
      if (route.GetInterface () == interface &&
          route.IsNetwork () && 
          route.GetDestNetwork () == networkAddress &&
          route.GetDestNetworkMask () == networkMask)
        {
          RemoveRoute (j);
        }
    }
}

void 
Ipv4StaticRoutingImpl::SetIpv4 (Ptr<Ipv4> ipv4)
{
  NS_LOG_FUNCTION (this << ipv4);
  NS_ASSERT (m_ipv4 == 0 && ipv4 != 0);
  m_ipv4 = ipv4;
  for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i++)
    {
      if (m_ipv4->IsUp (i))
        {
          NotifyInterfaceUp (i);
        }
      else
        {
          NotifyInterfaceDown (i);
        }
    }
}

}//namespace ns3
