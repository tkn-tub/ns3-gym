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
//         Gustavo Carneiro <gjc@inescporto.pt>

#include "ns3/debug.h"
#include "ipv4-static-routing.h"
#include "ns3/packet.h"

NS_DEBUG_COMPONENT_DEFINE ("Ipv4StaticRouting");

namespace ns3 {

Ipv4StaticRouting::Ipv4StaticRouting () 
: m_defaultRoute (0), m_defaultMulticastRoute (0)
{
}

void 
Ipv4StaticRouting::AddHostRouteTo (Ipv4Address dest, 
                                   Ipv4Address nextHop, 
                                   uint32_t interface)
{

  Ipv4Route *route = new Ipv4Route ();
  *route = Ipv4Route::CreateHostRouteTo (dest, nextHop, interface);
  m_hostRoutes.push_back (route);
}

void 
Ipv4StaticRouting::AddHostRouteTo (Ipv4Address dest, 
                                   uint32_t interface)
{
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
  Ipv4MulticastRoute *route = new Ipv4MulticastRoute ();
  *route = Ipv4MulticastRoute::CreateMulticastRoute (origin, group, 
    inputInterface, outputInterfaces);
  m_multicastRoutes.push_back (route);
}

void 
Ipv4StaticRouting::SetDefaultMulticastRoute(uint32_t outputInterface)
{
  Ipv4Address origin = Ipv4Address::GetAny ();
  Ipv4Address group = Ipv4Address::GetAny ();
  uint32_t inputInterface = Ipv4RoutingProtocol::IF_INDEX_ANY;

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
  return m_multicastRoutes.size () + m_defaultMulticastRoute ? 1 : 0;
}

Ipv4MulticastRoute *
Ipv4StaticRouting::GetMulticastRoute (uint32_t index) const
{
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
//
// This method does not attempt to delete the multicast route.
// 
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
  uint32_t    ifIndex)
{
  for (MulticastRoutesI i = m_multicastRoutes.begin (); 
       i != m_multicastRoutes.end (); 
       i++) 
    {
      Ipv4MulticastRoute *route = *i;
      if (  (origin == route->GetOrigin () || 
             origin == Ipv4Address::GetAny ()) &&
          group == route->GetGroup ())
        {
          if (ifIndex == Ipv4RoutingProtocol::IF_INDEX_ANY || 
              ifIndex == route->GetInputInterface ())
            {
              return *i;
            }
        }
    }

  if (m_defaultMulticastRoute != 0) 
    {
      return m_defaultMulticastRoute;
    }

  return 0;
}

uint32_t 
Ipv4StaticRouting::GetNRoutes (void)
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
Ipv4StaticRouting::GetDefaultRoute ()
{
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
  uint32_t ifIndex,
  Ipv4Header const &ipHeader,
  Packet packet,
  RouteReplyCallback routeReply)
{
  NS_DEBUG ("Ipv4StaticRouting::RequestRoute (" << &ipHeader << ", " <<
    &packet << ", " << &routeReply << ")");

  NS_DEBUG ("Ipv4StaticRouting::RequestRoute (): source = " << 
    ipHeader.GetSource ());

  NS_DEBUG ("Ipv4StaticRouting::RequestRoute (): destination = " << 
    ipHeader.GetDestination ());

  if (ipHeader.GetDestination ().IsMulticast ())
    {
      NS_DEBUG ("Ipv4StaticRouting::RequestRoute (): Multicast destination");
//
// We have a multicast packet we're going to send.  There are two distinct
// cases we need to support.  The first is if the current node is the source
// of the packet.  In that case, we don't want to have to consult multicast
// routing tables (nor build them) in order to send multicasts.  The interface
// index (ifIndex) is Ipv4RoutingProtocol::IF_INDEX_ANY if we're the source.
//
// The second case is if the current packet has gotten to us by being
// received over one of our interfaces.  In this case, ifIndex is set to the
// index over which we received the packet.  For these packets, we need to
// consult the multicast routing table for a disposition.
//
// So, first let's see if we're the source.  In this case, we don't consult
// the routing tables, but just return false and let the caller (up in 
// ipv4-l3-protocol) flood the multicast packet out of all of its interfaces.
// We can't really do it here even if we wanted to since we have no easy way
// to get to the Ipv4 interface which we would need.
//
      if (ifIndex == Ipv4RoutingProtocol::IF_INDEX_ANY)
        {
          return false;
        }
//
// If we fall through to this point, we have a multicast packet that has
// not originated at this node.  We need to deal with forwarding.  Let's
// see if we have a route, and if so go ahead and forward this puppy.
//
      Ipv4MulticastRoute *mRoute = LookupStatic(ipHeader.GetSource (),
        ipHeader.GetDestination (), ifIndex);

      if (mRoute)
        {
          NS_DEBUG ("Ipv4StaticRouting::RequestRoute (): "
            "Multicast route found");
          for (uint32_t i = 0; i < mRoute->GetNOutputInterfaces (); ++i)
            {
              Packet p = packet;
              Ipv4Header h = ipHeader;
              Ipv4Route route = 
                Ipv4Route::CreateHostRouteTo(h.GetDestination (), 
                  mRoute->GetOutputInterface(i));
              NS_DEBUG ("Ipv4StaticRouting::RequestRoute (): "
                "Send via interface " << mRoute->GetOutputInterface(i));
              routeReply (true, route, p, h);
            }
          return true;
        }
      return false; // Let other routing protocols try to handle this
    }
//
// See if this is a unicast packet we have a route for.
//
  NS_DEBUG ("Ipv4StaticRouting::RequestRoute (): Unicast destination");
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
Ipv4StaticRouting::RequestIfIndex (Ipv4Address destination, uint32_t& ifIndex)
{
  NS_DEBUG ("Ipv4StaticRouting::RequestIfIndex (" << destination << ", " <<
    &ifIndex << ")");
//
// First, see if this is a multicast packet we have a route for.  If we
// have a route, then send the packet down each of the specified interfaces.
//
  if (destination.IsMulticast ())
    {
      NS_DEBUG ("Ipv4StaticRouting::RequestIfIndex (): Multicast destination");

      Ipv4MulticastRoute *mRoute = LookupStatic(Ipv4Address::GetAny (),
        destination, Ipv4RoutingProtocol::IF_INDEX_ANY);

      if (mRoute)
        {
          NS_DEBUG ("Ipv4StaticRouting::RequestIfIndex (): "
            "Multicast route found");

          if (mRoute->GetNOutputInterfaces () != 1)
            {
              NS_DEBUG ("Ipv4StaticRouting::RequestIfIndex (): "
                "Route is to multiple interfaces.  Ignoring.");
              return false;
            }

          ifIndex = mRoute->GetOutputInterface(0);
          NS_DEBUG ("Ipv4StaticRouting::RequestIfIndex (): "
            "Found ifIndex " << ifIndex);
          return true;
        }
      return false; // Let other routing protocols try to handle this
    }
//
// See if this is a unicast packet we have a route for.
//
  NS_DEBUG ("Ipv4StaticRouting::RequestIfIndex (): Unicast destination");
  Ipv4Route *route = LookupStatic (destination);
  if (route)
    {
      ifIndex = route->GetInterface ();
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
