/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007-2009 Strasbourg University
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
 * Author: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 */

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ipv6-route.h"
#include "ns3/net-device.h"

#include "ipv6-static-routing.h"
#include "ipv6-routing-table-entry.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("Ipv6StaticRouting");

TypeId Ipv6StaticRouting::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::Ipv6StaticRouting")
    .SetParent<Ipv6RoutingProtocol> ()
    .AddConstructor<Ipv6StaticRouting> ()
    ;
  return tid;
}

Ipv6StaticRouting::Ipv6StaticRouting ()
  : m_defaultRoute (0),
  m_ipv6 (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ipv6StaticRouting::~Ipv6StaticRouting ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void Ipv6StaticRouting::SetIpv6 (Ptr<Ipv6> ipv6)
{
  NS_LOG_FUNCTION (this << ipv6);
  NS_ASSERT (m_ipv6 == 0 && ipv6 != 0);
  uint32_t i = 0; 
  m_ipv6 = ipv6;
  
  for (i = 0 ; i < m_ipv6->GetNInterfaces () ; i++)
  {
    if (m_ipv6->IsUp (i))
    {
      NotifyInterfaceUp (i);
    }
    else
    {
      NotifyInterfaceDown (i);
    }
  }
}

void Ipv6StaticRouting::AddHostRouteTo (Ipv6Address dst, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse)
{
  NS_LOG_FUNCTION (this << dst << nextHop << interface << prefixToUse);
  Ipv6RoutingTableEntry* route = new Ipv6RoutingTableEntry ();
  *route = Ipv6RoutingTableEntry::CreateHostRouteTo (dst, nextHop, interface, prefixToUse);
  m_hostRoutes.push_back (route);
}

void Ipv6StaticRouting::AddHostRouteTo (Ipv6Address dst, uint32_t interface)
{
  NS_LOG_FUNCTION (this << dst << interface);
  Ipv6RoutingTableEntry* route = new Ipv6RoutingTableEntry ();
  *route = Ipv6RoutingTableEntry::CreateHostRouteTo (dst, interface);
  m_hostRoutes.push_back (route);
}

void Ipv6StaticRouting::AddNetworkRouteTo (Ipv6Address network, Ipv6Prefix networkPrefix, Ipv6Address nextHop, uint32_t interface)
{
  NS_LOG_FUNCTION (this << network << networkPrefix << nextHop << interface);
  Ipv6RoutingTableEntry* route = new Ipv6RoutingTableEntry ();
  *route = Ipv6RoutingTableEntry::CreateNetworkRouteTo (network, networkPrefix, nextHop, interface);
  m_networkRoutes.push_back (route);
}

void Ipv6StaticRouting::AddNetworkRouteTo (Ipv6Address network, Ipv6Prefix networkPrefix, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse)
{
  NS_LOG_FUNCTION (this << network << networkPrefix << nextHop << interface << prefixToUse);
  Ipv6RoutingTableEntry* route = new Ipv6RoutingTableEntry ();
  *route = Ipv6RoutingTableEntry::CreateNetworkRouteTo (network, networkPrefix, nextHop, interface, prefixToUse);
  m_networkRoutes.push_back (route);
}


void Ipv6StaticRouting::AddNetworkRouteTo (Ipv6Address network, Ipv6Prefix networkPrefix, uint32_t interface)
{
  NS_LOG_FUNCTION (this << network << networkPrefix << interface);
  Ipv6RoutingTableEntry* route = new Ipv6RoutingTableEntry ();
  *route = Ipv6RoutingTableEntry::CreateNetworkRouteTo (network, networkPrefix, interface);
  m_networkRoutes.push_back (route);
}

void Ipv6StaticRouting::SetDefaultRoute (Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse)
{
  NS_LOG_FUNCTION (this << nextHop << interface << prefixToUse);
  Ipv6RoutingTableEntry* route = new Ipv6RoutingTableEntry ();
  *route = Ipv6RoutingTableEntry::CreateDefaultRoute (nextHop, interface);
  route->SetPrefixToUse (prefixToUse);
  delete m_defaultRoute;
  m_defaultRoute = route;
}

void Ipv6StaticRouting::RemoveDefaultRoute ()
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_defaultRoute)
  {
    delete m_defaultRoute;
    m_defaultRoute = 0;
  }
}

void Ipv6StaticRouting::AddMulticastRoute (Ipv6Address origin, Ipv6Address group, uint32_t inputInterface, std::vector<uint32_t> outputInterfaces)
{
  NS_LOG_FUNCTION (this << origin << group << inputInterface);
  Ipv6MulticastRoutingTableEntry* route = new Ipv6MulticastRoutingTableEntry ();
  *route = Ipv6MulticastRoutingTableEntry::CreateMulticastRoute (origin, group, inputInterface, outputInterfaces);
  m_multicastRoutes.push_back (route);
}

void Ipv6StaticRouting::SetDefaultMulticastRoute (uint32_t outputInterface)
{
  NS_LOG_FUNCTION (this << outputInterface);
  Ipv6RoutingTableEntry *route = new Ipv6RoutingTableEntry ();
  Ipv6Address network = Ipv6Address ("ff00::"); /* RFC 3513 */
  Ipv6Prefix networkMask = Ipv6Prefix (8);
  *route = Ipv6RoutingTableEntry::CreateNetworkRouteTo (network, networkMask, outputInterface);
  m_networkRoutes.push_back (route);
}

uint32_t Ipv6StaticRouting::GetNMulticastRoutes () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_multicastRoutes.size ();
}

Ipv6MulticastRoutingTableEntry Ipv6StaticRouting::GetMulticastRoute (uint32_t index) const
{
  NS_LOG_FUNCTION (this << index);
  NS_ASSERT_MSG (index < m_multicastRoutes.size (), "Ipv6StaticRouting::GetMulticastRoute () : Index out of range");

  if (index < m_multicastRoutes.size ())
  {
    uint32_t tmp = 0;
    for (MulticastRoutesCI i = m_multicastRoutes.begin () ; i != m_multicastRoutes.end () ; i++)
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

bool Ipv6StaticRouting::RemoveMulticastRoute (Ipv6Address origin, Ipv6Address group, uint32_t inputInterface)
{
  NS_LOG_FUNCTION (this << origin << group << inputInterface);
  for (MulticastRoutesI i = m_multicastRoutes.begin (); i != m_multicastRoutes.end (); i++)
  {
    Ipv6MulticastRoutingTableEntry *route = *i;
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

void Ipv6StaticRouting::RemoveMulticastRoute (uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
  uint32_t tmp = 0;
  
  for (MulticastRoutesI i = m_multicastRoutes.begin () ; i != m_multicastRoutes.end () ; i++)
  {
    if (tmp == index)
    {
      delete *i;
      m_multicastRoutes.erase (i);
      return;
    }
    tmp++;
  }
}

bool Ipv6StaticRouting::HasNetworkDest (Ipv6Address network, uint32_t interfaceIndex)
{
  NS_LOG_FUNCTION (this << network << interfaceIndex);

  /* in the network table */
  for (NetworkRoutesI j = m_networkRoutes.begin () ; j != m_networkRoutes.end () ; j++)
  {
    NS_ASSERT ((*j)->IsNetwork ());
    Ipv6Prefix prefix = (*j)->GetDestNetworkPrefix ();
    Ipv6Address entry = (*j)->GetDestNetwork ();

    if (prefix.IsMatch (network, entry) && (*j)->GetInterface () == interfaceIndex)
    {
      return true;
    }
  }

  /* beuh!!! not route at all */
  return false;
}

Ptr<Ipv6Route> Ipv6StaticRouting::LookupStatic (Ipv6Address dst, uint32_t interface)
{
  NS_LOG_FUNCTION (this << dst << interface);
  Ptr<Ipv6Route> rtentry = 0;

  /* when sending on link-local multicast, there have to be interface specified */
  if (dst == Ipv6Address::GetAllNodesMulticast () || dst.IsSolicitedMulticast () || 
     dst == Ipv6Address::GetAllRoutersMulticast () || dst == Ipv6Address::GetAllHostsMulticast ())
  {
    NS_ASSERT_MSG (interface > 0, "Try to send on link-local multicast address, and no interface index is given!");
    rtentry = Create<Ipv6Route> ();
    rtentry->SetSource (SourceAddressSelection (interface, dst));
    rtentry->SetDestination (dst);
    rtentry->SetGateway (Ipv6Address::GetZero ());
    rtentry->SetOutputDevice (m_ipv6->GetNetDevice (interface));
    return rtentry;
  }

  /* is the destination in hosts table */
  for (HostRoutesCI i = m_hostRoutes.begin () ; i != m_hostRoutes.end () ; i++)
  {
    NS_ASSERT ((*i)->IsHost ());
    if ((*i)->GetDest ().IsEqual (dst))
    {
      if (!interface || interface == (*i)->GetInterface ())
      {
        NS_LOG_LOGIC ("Found global host route " << *i);
        Ipv6RoutingTableEntry* route = (*i);
        rtentry = Create<Ipv6Route> ();
        uint32_t interfaceIdx = route->GetInterface ();
        rtentry->SetDestination (route->GetDest ());

        if (route->GetGateway ().IsAny ())
        {
          rtentry->SetSource (SourceAddressSelection (interfaceIdx, route->GetDest ()));
        }
        else
        {
          rtentry->SetSource (SourceAddressSelection (interfaceIdx, route->GetGateway ()));
        }

        rtentry->SetGateway (route->GetGateway ());
        rtentry->SetOutputDevice (m_ipv6->GetNetDevice (interfaceIdx));
        return rtentry;
      }
    }
  }

  /* or in the network table */
  for (NetworkRoutesI j = m_networkRoutes.begin () ; j != m_networkRoutes.end () ; j++)
  {
    NS_ASSERT ((*j)->IsNetwork ());
    Ipv6Prefix prefix = (*j)->GetDestNetworkPrefix ();
    Ipv6Address entry = (*j)->GetDestNetwork ();

    if (prefix.IsMatch (dst, entry))
    {
      /* if interface is given, check the route will output on this interface */
      if (!interface || interface == (*j)->GetInterface ())
      {
        NS_LOG_LOGIC ("Found global network route " << *j);
        Ipv6RoutingTableEntry* route = (*j);
        rtentry = Create<Ipv6Route>();
        uint32_t interfaceIdx = route->GetInterface ();
        rtentry->SetDestination (route->GetDest ());
        
        if (route->GetGateway ().IsAny ())
        {
          rtentry->SetSource (SourceAddressSelection (interfaceIdx, route->GetDest ()));
        }
        else
        {
          rtentry->SetSource (SourceAddressSelection (interfaceIdx, route->GetGateway ()));
        }

        rtentry->SetGateway (route->GetGateway ());
        rtentry->SetOutputDevice (m_ipv6->GetNetDevice (interfaceIdx));
        return rtentry;
      }
    }
  }

  /* not found, return the default route if any */
  if (m_defaultRoute != 0)
  {
    NS_ASSERT (m_defaultRoute->IsDefault ());
    NS_LOG_LOGIC ("Found global network route via default route " << m_defaultRoute);
    Ipv6RoutingTableEntry* route = m_defaultRoute;
    rtentry = Create<Ipv6Route>();
    uint32_t interfaceIdx = route->GetInterface ();
    rtentry->SetDestination (route->GetDest ());
    rtentry->SetSource (SourceAddressSelection (interfaceIdx, route->GetPrefixToUse ().IsAny () ? route->GetGateway () : route->GetPrefixToUse ()));
    rtentry->SetGateway (route->GetGateway ());
    rtentry->SetOutputDevice (m_ipv6->GetNetDevice (interfaceIdx));
    return rtentry;
  }  

  /* beuh!!! not route at all */
  return 0;
}

void Ipv6StaticRouting::DoDispose ()
{
  NS_LOG_FUNCTION_NOARGS ();
  for (HostRoutesI i = m_hostRoutes.begin () ; i != m_hostRoutes.end () ; i = m_hostRoutes.erase (i))
  {
    delete (*i);
  }
  m_hostRoutes.clear ();
  
  for (NetworkRoutesI j = m_networkRoutes.begin () ;  j != m_networkRoutes.end () ; j = m_networkRoutes.erase (j))
  {
    delete (*j);
  }
  m_networkRoutes.clear ();
  
  if (m_defaultRoute != 0)
  {
    delete m_defaultRoute;
    m_defaultRoute = 0;
  }
  
  for (MulticastRoutesI i = m_multicastRoutes.begin () ; i != m_multicastRoutes.end () ; i = m_multicastRoutes.erase (i))
  {
    delete (*i);
  }
  m_multicastRoutes.clear ();

  m_ipv6 = 0;
  Ipv6RoutingProtocol::DoDispose ();
}

Ptr<Ipv6MulticastRoute> Ipv6StaticRouting::LookupStatic (Ipv6Address origin, Ipv6Address group, uint32_t interface)
{
  NS_LOG_FUNCTION (this << origin << group << interface);
  Ptr<Ipv6MulticastRoute> mrtentry = 0;

  for (MulticastRoutesI i = m_multicastRoutes.begin () ; i != m_multicastRoutes.end () ; i++)
  {
    Ipv6MulticastRoutingTableEntry* route = *i;

    /*
       We've been passed an origin address, a multicast group address and an
       interface index.  We have to decide if the current route in the list is
       a match.

       The first case is the restrictive case where the origin, group and index
       matches.  This picks up exact routes during forwarded and exact routes from
       the local node (in which case the ifIndex is a wildcard).
     */

    if (origin == route->GetOrigin () && group == route->GetGroup ())
    {
      /* skipping SSM case */
      NS_LOG_LOGIC ("Find source specific multicast route" << *i);
    }

    if (group == route->GetGroup ())
    {
       if (interface == Ipv6::IF_ANY || interface == route->GetInputInterface ())
       {
          NS_LOG_LOGIC ("Found multicast route" << *i);
          mrtentry = Create<Ipv6MulticastRoute>();
          mrtentry->SetGroup (route->GetGroup ());
          mrtentry->SetOrigin (route->GetOrigin ());
          mrtentry->SetParent (route->GetInputInterface ());
          for (uint32_t j = 0 ; j < route->GetNOutputInterfaces () ; j++)
          {
            if (route->GetOutputInterface (j))
            {
              NS_LOG_LOGIC ("Setting output interface index " << route->GetOutputInterface (j));
              mrtentry->SetOutputTtl (route->GetOutputInterface (j), Ipv6MulticastRoute::MAX_TTL - 1);
            }
          }
          return mrtentry;
       } 
    }
  }
  return mrtentry;
}

uint32_t Ipv6StaticRouting::GetNRoutes ()
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

Ipv6RoutingTableEntry Ipv6StaticRouting::GetDefaultRoute ()
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_defaultRoute != 0)
  {
    return *m_defaultRoute;
  }
  else
  {
    return Ipv6RoutingTableEntry ();
  }
}

Ipv6RoutingTableEntry Ipv6StaticRouting::GetRoute (uint32_t index)
{
  NS_LOG_FUNCTION (this << index);

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
    for (HostRoutesCI i = m_hostRoutes.begin () ; i != m_hostRoutes.end () ; i++)
    {
      if (tmp == index)
      {
        return *i;
      }
      tmp++;
    }
  }
  
  index -= m_hostRoutes.size ();
  uint32_t tmp = 0;

  for (NetworkRoutesI j = m_networkRoutes.begin () ; j != m_networkRoutes.end () ; j++)
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

void Ipv6StaticRouting::RemoveRoute (uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
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
    for (HostRoutesI i = m_hostRoutes.begin () ; i != m_hostRoutes.end () ; i++)
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
  
  for (NetworkRoutesI j = m_networkRoutes.begin () ; j != m_networkRoutes.end () ; j++)
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

void Ipv6StaticRouting::RemoveRoute (Ipv6Address network, Ipv6Prefix prefix, uint32_t ifIndex)
{
  NS_LOG_FUNCTION (this << network << prefix << ifIndex);
  for (NetworkRoutesI i = m_networkRoutes.begin () ; i != m_networkRoutes.end () ; i++)
  {
    if (network == (*i)->GetDest () and (*i)->GetInterface () == ifIndex)
    {
      delete *i;
      m_networkRoutes.erase (i);
      return;
    }
  }
}

Ptr<Ipv6Route> Ipv6StaticRouting::RouteOutput (Ptr<Packet> p, const Ipv6Header &header, uint32_t oif, Socket::SocketErrno &sockerr)
{
  NS_LOG_FUNCTION (this << header << oif);
  Ipv6Address destination = header.GetDestinationAddress ();
  Ptr<Ipv6Route> rtentry = 0;

  if (destination.IsMulticast ())
  {
    // Note:  Multicast routes for outbound packets are stored in the
    // normal unicast table.  An implication of this is that it is not
    // possible to source multicast datagrams on multiple interfaces.
    // This is a well-known property of sockets implementation on
    // many Unix variants.
    // So, we just log it and fall through to LookupStatic ()
    NS_LOG_LOGIC ("RouteOutput ()::Multicast destination");
  }
  
  rtentry = LookupStatic (destination, oif);
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

bool Ipv6StaticRouting::RouteInput (Ptr<const Packet> p, const Ipv6Header &ipHeader, Ptr<const NetDevice> idev,
                                    UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                                    LocalDeliverCallback lcb, ErrorCallback ecb)
{
  NS_LOG_FUNCTION (this << p << ipHeader << ipHeader.GetSourceAddress () << ipHeader.GetDestinationAddress () << idev);

  if (ipHeader.GetDestinationAddress ().IsMulticast ())
  {
    NS_LOG_LOGIC ("Multicast destination");
    Ptr<Ipv6MulticastRoute> mrtentry = LookupStatic (ipHeader.GetSourceAddress (),
    ipHeader.GetDestinationAddress (), m_ipv6->GetInterfaceForDevice (idev));

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
  Ptr<Ipv6Route> rtentry = LookupStatic (ipHeader.GetDestinationAddress ());

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

void Ipv6StaticRouting::NotifyInterfaceUp (uint32_t i)
{
  for (uint32_t j = 0 ; j < m_ipv6->GetNAddresses (i) ; j++)
  {
    if (m_ipv6->GetAddress (i, j).GetAddress () != Ipv6Address () &&
       m_ipv6->GetAddress (i, j).GetPrefix () != Ipv6Prefix ())
    {
      if (m_ipv6->GetAddress (i, j).GetPrefix () == Ipv6Prefix (128))
      {
        /* host route */
        AddHostRouteTo (m_ipv6->GetAddress (i, j).GetAddress (), i);
      }
      else
      {
        AddNetworkRouteTo (m_ipv6->GetAddress (i, j).GetAddress ().CombinePrefix (m_ipv6->GetAddress (i, j).GetPrefix ()),
                          m_ipv6->GetAddress (i, j).GetPrefix (), i);
      }
    }
  }
}

void Ipv6StaticRouting::NotifyInterfaceDown (uint32_t i)
{
  /* remove all static routes that are going through this interface */
  for (uint32_t j = 0 ; j < GetNRoutes () ; j++)
  {
    Ipv6RoutingTableEntry route = GetRoute (j);

    if (route.GetInterface () == i)
    {
      RemoveRoute (j);
    }
  }
}

void Ipv6StaticRouting::NotifyAddAddress (uint32_t interface, Ipv6InterfaceAddress address)
{
  if (!m_ipv6->IsUp (interface))
  {
    return;
  }

  Ipv6Address networkAddress = address.GetAddress ().CombinePrefix (address.GetPrefix ());
  Ipv6Prefix networkMask = address.GetPrefix ();

  if (address.GetAddress () != Ipv6Address () && address.GetPrefix () != Ipv6Prefix ())
  {
    AddNetworkRouteTo (networkAddress, networkMask, interface);
  }
}

void Ipv6StaticRouting::NotifyRemoveAddress (uint32_t interface, Ipv6InterfaceAddress address)
{
  if (!m_ipv6->IsUp (interface))
  {
    return;
  }

  Ipv6Address networkAddress = address.GetAddress ().CombinePrefix (address.GetPrefix ());
  Ipv6Prefix networkMask = address.GetPrefix ();
  
  // Remove all static routes that are going through this interface
  // which reference this network
  for (uint32_t j = 0 ; j < GetNRoutes () ; j++)
  {
    Ipv6RoutingTableEntry route = GetRoute (j);
    
    if (route.GetInterface () == interface &&
        route.IsNetwork () &&
        route.GetDestNetwork () == networkAddress &&
        route.GetDestNetworkPrefix () == networkMask)
    {
      RemoveRoute (j);
    }
  }
}

void Ipv6StaticRouting::NotifyAddRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface, Ipv6Address prefixToUse)
{
  NS_LOG_INFO (this << dst << mask << nextHop << interface << prefixToUse);
  if (mask == Ipv6Prefix (128))
  {
    AddHostRouteTo (dst, nextHop, interface);
  }
  else if (dst != Ipv6Address::GetZero ())
  {
    AddNetworkRouteTo (dst, mask, nextHop, interface);
  }
  else /* default route */
  {
    /* this case is mainly used by configuring default route following RA processing,
     * in case of multipe prefix in RA, the first will configured default route
     */
    if (!m_defaultRoute)
    {
      SetDefaultRoute (nextHop, interface, prefixToUse);
    }
  }
}

void Ipv6StaticRouting::NotifyRemoveRoute (Ipv6Address dst, Ipv6Prefix mask, Ipv6Address nextHop, uint32_t interface)
{
  NS_LOG_FUNCTION (this << dst << mask << nextHop << interface);
  if (mask == Ipv6Prefix (128))
  {
    for (HostRoutesI j = m_hostRoutes.begin () ; j != m_hostRoutes.end () ; j++)
    {
      Ipv6Prefix prefix = (*j)->GetDestNetworkPrefix ();
      Ipv6Address entry = (*j)->GetDestNetwork ();

      if (dst == entry && prefix == mask && (*j)->GetInterface () == interface)
      {
        delete (*j);
        m_hostRoutes.erase (j);
      } 
    }
  }
  else if (dst != Ipv6Address::GetZero ())
  {
    for (NetworkRoutesI j = m_networkRoutes.begin () ; j != m_networkRoutes.end () ; j++)
    {
      Ipv6Prefix prefix = (*j)->GetDestNetworkPrefix ();
      Ipv6Address entry = (*j)->GetDestNetwork ();

      if (dst == entry && prefix == mask && (*j)->GetInterface () == interface)
      {
        delete (*j);
        m_networkRoutes.erase (j);
      } 
    }
  }
  else
  {
    /* default route case */
    if (!m_defaultRoute)
    {
      return;
    }

    if (m_defaultRoute->GetInterface () == interface && m_defaultRoute->GetGateway () == nextHop)
    {
      NS_LOG_LOGIC ("Remove default route (maybe because autoconfigured address expired)");
      delete m_defaultRoute;
      m_defaultRoute = 0;
    }
  }
}

Ipv6Address Ipv6StaticRouting::SourceAddressSelection (uint32_t interface, Ipv6Address dest)
{
  NS_LOG_FUNCTION (this << interface << dest);
  Ipv6Address ret;

  /* first address of an IPv6 interface is link-local ones */
  ret = m_ipv6->GetAddress (interface, 0).GetAddress ();

  if (dest == Ipv6Address::GetAllNodesMulticast () || dest == Ipv6Address::GetAllRoutersMulticast () || dest == Ipv6Address::GetAllHostsMulticast ())
  {
    return ret; 
  }

  /* useally IPv6 interfaces have one link-local address and one global address */

  for (uint32_t i = 1 ; i < m_ipv6->GetNAddresses (interface) ; i++)
  {
    Ipv6InterfaceAddress test = m_ipv6->GetAddress (interface, i);

    if (test.GetAddress ().CombinePrefix (test.GetPrefix ()) == dest.CombinePrefix (test.GetPrefix ()))
    {
      return test.GetAddress ();
    }
  }
  
  return ret;
}

} /* namespace ns3 */

