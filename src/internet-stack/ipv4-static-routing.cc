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
#include "ipv4-static-routing.h"
#include "ns3/packet.h"

NS_LOG_COMPONENT_DEFINE ("Ipv4StaticRouting");

namespace ns3 {

Ipv4StaticRouting::Ipv4StaticRouting () 
: m_defaultRoute (0), m_defaultMulticastRoute (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
Ipv4StaticRouting::AddHostRouteTo (Ipv4Address dest, 
                                   Ipv4Address nextHop, 
                                   uint32_t interface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4Route *route = new Ipv4Route ();
  *route = Ipv4Route::CreateHostRouteTo (dest, nextHop, interface);
  m_hostRoutes.push_back (route);
}

void 
Ipv4StaticRouting::AddHostRouteTo (Ipv4Address dest, 
                                   uint32_t interface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4Route *route = new Ipv4Route ();
  *route = Ipv4Route::CreateHostRouteTo (dest, interface);
  m_hostRoutes.push_back (route);
}

void 
Ipv4StaticRouting::AddNetworkRouteTo (Ipv4Address network, 
                                      Ipv4Mask networkMask, 
                                      Ipv4Address nextHop, 
                                      uint32_t interface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4Route *route = new Ipv4Route ();
  *route = Ipv4Route::CreateNetworkRouteTo (network,
                                            networkMask,
                                            nextHop,
                                            interface);
  m_networkRoutes.push_back (route);
}

void 
Ipv4StaticRouting::AddNetworkRouteTo (Ipv4Address network, 
                                      Ipv4Mask networkMask, 
                                      uint32_t interface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4Route *route = new Ipv4Route ();
  *route = Ipv4Route::CreateNetworkRouteTo (network,
                                            networkMask,
                                            interface);
  m_networkRoutes.push_back (route);
}

void 
Ipv4StaticRouting::SetDefaultRoute (Ipv4Address nextHop, 
                                    uint32_t interface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4Route *route = new Ipv4Route ();
  *route = Ipv4Route::CreateDefaultRoute (nextHop, interface);
  delete m_defaultRoute;
  m_defaultRoute = route;
}

void 
Ipv4StaticRouting::AddMulticastRoute(Ipv4Address origin,
                                     Ipv4Address group,
                                     uint32_t inputInterface,
                                     std::vector<uint32_t> outputInterfaces)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4MulticastRoute *route = new Ipv4MulticastRoute ();
  *route = Ipv4MulticastRoute::CreateMulticastRoute (origin, group, 
    inputInterface, outputInterfaces);
  m_multicastRoutes.push_back (route);
}

void 
Ipv4StaticRouting::SetDefaultMulticastRoute(uint32_t outputInterface)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv4Address origin = Ipv4Address::GetAny ();
  Ipv4Address group = Ipv4Address::GetAny ();
  uint32_t inputInterface = Ipv4RoutingProtocol::INTERFACE_ANY;

  std::vector<uint32_t> outputInterfaces (1);
  outputInterfaces[0] = outputInterface;
  
  Ipv4MulticastRoute *route = new Ipv4MulticastRoute ();
  *route = Ipv4MulticastRoute::CreateMulticastRoute (origin, group, 
    inputInterface, outputInterfaces);

  delete m_defaultMulticastRoute;
  m_defaultMulticastRoute = route;
}

uint32_t 
Ipv4StaticRouting::GetNMulticastRoutes (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_multicastRoutes.size () + m_defaultMulticastRoute ? 1 : 0;
}

Ipv4MulticastRoute *
Ipv4StaticRouting::GetMulticastRoute (uint32_t index) const
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT_MSG(index < m_multicastRoutes.size (),
    "Ipv4StaticRouting::GetMulticastRoute ():  Index out of range");
//
// From an external point of view the default route appears to be in slot 0
// of the routing table.  The implementation, however, puts it in a separate 
// place.  So, if a client asks for index 0 and we have a default multicast
// route, we have to return it from that different place 
// (m_defaultMulticastRoute).
//
  if (index == 0 && m_defaultMulticastRoute != 0)
    {
      return m_defaultMulticastRoute;
    }
//
// If there is a default multicast route present, a client will just assume
// that it is in slot zero and there is one "extra" zeroth route in the table.
// To return the correct indexed entry in our list, we have to decrement the
// index to take into account the default route not being in the actual list.
// Since we fell through to here, we've taken care of the case where the
// index was zero.
//
  if (m_defaultMulticastRoute != 0)
    {
      NS_ASSERT(index > 0);
      index--;
    }

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

Ipv4MulticastRoute *
Ipv4StaticRouting::GetDefaultMulticastRoute () const
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_defaultMulticastRoute != 0)
    {
      return m_defaultMulticastRoute;
    }
  return 0;
}

bool
Ipv4StaticRouting::RemoveMulticastRoute(Ipv4Address origin,
                                        Ipv4Address group,
                                        uint32_t inputInterface)
{
  NS_LOG_FUNCTION_NOARGS ();
  for (MulticastRoutesI i = m_multicastRoutes.begin (); 
       i != m_multicastRoutes.end (); 
       i++) 
    {
      Ipv4MulticastRoute *route = *i;
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
Ipv4StaticRouting::RemoveMulticastRoute(uint32_t index)
{
  NS_LOG_FUNCTION_NOARGS ();
//
// From an external point of view the default route appears to be in slot 0
// of the routing table.  The implementation, however, puts it in a separate 
// place.  So, if a client asks to delete index 0 and we have a default
// multicast route set, we have to delete it from that different place 
// (m_defaultMulticastRoute).
//
  if (index == 0 && m_defaultMulticastRoute != 0)
    {
      delete m_defaultMulticastRoute;
      m_defaultMulticastRoute = 0;
    }
//
// If there is a default multicast route present, a client will just assume
// that it is in slot zero and there is one "extra" zeroth route in the table.
// To return the correct indexed entry in our list, we have to decrement the
// index to take into account the default route not being in the actual list.
// Since we fell through to here, we've taken care of the case where the
// index was zero.
//
  if (m_defaultMulticastRoute != 0)
    {
      NS_ASSERT(index > 0);
      index--;
    }

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

Ipv4Route *
Ipv4StaticRouting::LookupStatic (Ipv4Address dest)
{
  NS_LOG_FUNCTION_NOARGS ();
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

Ipv4MulticastRoute *
Ipv4StaticRouting::LookupStatic (
  Ipv4Address origin, 
  Ipv4Address group,
  uint32_t    interface)
{
  NS_LOG_FUNCTION_NOARGS ();
//
// We treat the "any" address (typically 0.0.0.0) as a wildcard in our matching
// scheme.
//
  Ipv4Address wildcard = Ipv4Address::GetAny ();

  for (MulticastRoutesI i = m_multicastRoutes.begin (); 
       i != m_multicastRoutes.end (); 
       i++) 
    {
      Ipv4MulticastRoute *route = *i;
//
// We've been passed an origin address, a multicast group address and an 
// interface index.  We have to decide if the current route in the list is
// a match.
//
// The first case is the restrictive case where the origin, group and index
// matches.  This picks up exact routes during forwarded and exact routes from
// the local node (in which case the interface is a wildcard).
//
      if (origin == route->GetOrigin () && group == route->GetGroup ())
        {
          if (interface == Ipv4RoutingProtocol::INTERFACE_ANY || 
              interface == route->GetInputInterface ())
            {
              return *i;
            }
        }
    }
//
// If the input interface index is not a wildcard (that means that the packet 
// did not originally come from this node), we're done.  We don't
// just happily forward packets we don't really know what to do with.  
// Multicast storms are not generally considered a good thing.
//
  if (interface != Ipv4RoutingProtocol::INTERFACE_ANY)
    {
      return 0;
    }
//
// Now, we're going to get a litle less restricive.  This only applies in the
// case where the packet in question is coming from the local node.  In order
// to avoid dependencies on the order in which routes were added, we will 
// actually walk the list two more times, the first time looking for routes
// with a single wildcard, and the last time looking for the first route
// with two wildcards.
//
  for (MulticastRoutesI i = m_multicastRoutes.begin (); 
       i != m_multicastRoutes.end (); 
       i++) 
    {
      Ipv4MulticastRoute *route = *i;
//
// Here we will ignore the origin.  We know that a single source address must
// be picked for a packet, but we may want to send multicast packets out
// multiple interfaces.  To support this case, a user would need to add
// a Multicast route with the route's origin set to wildcard.  N.B As a
// result, packets sourced from a node with multiple interface may have a
// source IP address different from that of the interface actually used to
// send the packet.
//
      if (route->GetOrigin () == wildcard && group == route->GetGroup ())
        {
          return *i;
        }
    }
//
// Finally we want to allow users to specify a default route that specifies
// sending all multicast packets out multiple interfaces.  The standard
// default multicast route is patterned after other systems and limits the 
// number of outputs to one.  If, however a client manually adds a multicast
// route with the origin, the multicast group and the input interface index
// all set to wildcard, she has created a default route with multiple output
// interfaces.
//
  for (MulticastRoutesI i = m_multicastRoutes.begin (); 
       i != m_multicastRoutes.end (); 
       i++) 
    {
      Ipv4MulticastRoute *route = *i;

      if (route->GetOrigin () == wildcard && route->GetGroup () == wildcard)
        {
          return *i;
        }
    }
//
// We also allow users to specify a typical default multicast route.  This
// default route is limited to specifying a single output interface.
//
  if (m_defaultMulticastRoute != 0) 
    {
      return m_defaultMulticastRoute;
    }

  return 0;
}

uint32_t 
Ipv4StaticRouting::GetNRoutes (void)
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

Ipv4Route *
Ipv4StaticRouting::GetDefaultRoute ()
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_defaultRoute != 0)
    {
      return m_defaultRoute;
    }
  else
    {
      return 0;
    }
}

Ipv4Route *
Ipv4StaticRouting::GetRoute (uint32_t index)
{
  NS_LOG_FUNCTION_NOARGS ();
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
Ipv4StaticRouting::RemoveRoute (uint32_t index)
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

bool
Ipv4StaticRouting::RequestRoute (
  uint32_t interface,
  Ipv4Header const &ipHeader,
  Ptr<Packet> packet,
  RouteReplyCallback routeReply)
{
  NS_LOG_FUNCTION (this << interface << &ipHeader << packet << &routeReply);

  NS_LOG_LOGIC ("source = " << ipHeader.GetSource ());

  NS_LOG_LOGIC ("destination = " << ipHeader.GetDestination ());

  if (ipHeader.GetDestination ().IsMulticast ())
    {
      NS_LOG_LOGIC ("Multicast destination");

      Ipv4MulticastRoute *mRoute = LookupStatic(ipHeader.GetSource (),
        ipHeader.GetDestination (), interface);

      if (mRoute)
        {
          NS_LOG_LOGIC ("Multicast route found");

          for (uint32_t i = 0; i < mRoute->GetNOutputInterfaces (); ++i)
            {
              Ptr<Packet> p = packet->Copy ();
              Ipv4Header h = ipHeader;
              Ipv4Route route = 
                Ipv4Route::CreateHostRouteTo(h.GetDestination (), 
                  mRoute->GetOutputInterface(i));
              NS_LOG_LOGIC ( "Send via interface " << 
                mRoute->GetOutputInterface(i));
              routeReply (true, route, p, h);
            }
          return true;
        }
      return false; // Let other routing protocols try to handle this
    }
//
// This is a unicast packet.  Check to see if we have a route for it.
//
  NS_LOG_LOGIC ("Unicast destination");
  Ipv4Route *route = LookupStatic (ipHeader.GetDestination ());
  if (route != 0)
    {
      routeReply (true, *route, packet, ipHeader);
      return true;
    }
  else
    {
      return false; // Let other routing protocols try to handle this
                    // route request.
    }
}

bool
Ipv4StaticRouting::RequestInterface (Ipv4Address destination, uint32_t& interface)
{
  NS_LOG_FUNCTION (this << destination << &interface);
//
// First, see if this is a multicast packet we have a route for.  If we
// have a route, then send the packet down each of the specified interfaces.
//
  if (destination.IsMulticast ())
    {
      NS_LOG_LOGIC ("Multicast destination");

      Ipv4MulticastRoute *mRoute = LookupStatic(Ipv4Address::GetAny (),
        destination, Ipv4RoutingProtocol::INTERFACE_ANY);

      if (mRoute)
        {
          NS_LOG_LOGIC ("Multicast route found");

          if (mRoute->GetNOutputInterfaces () != 1)
            {
              NS_LOG_LOGIC ("Route is to multiple interfaces.  Ignoring.");
              return false;
            }

          interface = mRoute->GetOutputInterface(0);
          NS_LOG_LOGIC ("Found interface " << interface);
          return true;
        }
      return false; // Let other routing protocols try to handle this
    }
//
// See if this is a unicast packet we have a route for.
//
  NS_LOG_LOGIC ("Unicast destination");
  Ipv4Route *route = LookupStatic (destination);
  if (route)
    {
      interface = route->GetInterface ();
      return true;
    }
  else
    {
      return false;
    }
}

void
Ipv4StaticRouting::DoDispose (void)
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
  if (m_defaultMulticastRoute != 0)
    {
      delete m_defaultMulticastRoute;
      m_defaultMulticastRoute = 0;
    }
  Ipv4RoutingProtocol::DoDispose ();
}

}//namespace ns3
