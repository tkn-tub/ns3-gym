/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ipv4-routing-table-entry.h"
#include "ns3/assert.h"

namespace ns3 {

/*****************************************************
 *     Network Ipv4RoutingTableEntry
 *****************************************************/

Ipv4RoutingTableEntry::Ipv4RoutingTableEntry ()
{
}

Ipv4RoutingTableEntry::Ipv4RoutingTableEntry (Ipv4RoutingTableEntry const &route)
  : m_dest (route.m_dest),
    m_destNetworkMask (route.m_destNetworkMask),
    m_gateway (route.m_gateway),
    m_interface (route.m_interface)
{
}

Ipv4RoutingTableEntry::Ipv4RoutingTableEntry (Ipv4RoutingTableEntry const *route)
  : m_dest (route->m_dest),
    m_destNetworkMask (route->m_destNetworkMask),
    m_gateway (route->m_gateway),
    m_interface (route->m_interface)
{
}

Ipv4RoutingTableEntry::Ipv4RoutingTableEntry (Ipv4Address dest,
                                              Ipv4Address gateway,
                                              uint32_t interface)
  : m_dest (dest),
    m_destNetworkMask (Ipv4Mask::GetOnes ()),
    m_gateway (gateway),
    m_interface (interface)
{
}
Ipv4RoutingTableEntry::Ipv4RoutingTableEntry (Ipv4Address dest,
                                              uint32_t interface)
  : m_dest (dest),
    m_destNetworkMask (Ipv4Mask::GetOnes ()),
    m_gateway (Ipv4Address::GetZero ()),
    m_interface (interface)
{
}
Ipv4RoutingTableEntry::Ipv4RoutingTableEntry (Ipv4Address network,
                                              Ipv4Mask networkMask,
                                              Ipv4Address gateway,
                                              uint32_t interface)
  : m_dest (network),
    m_destNetworkMask (networkMask),
    m_gateway (gateway),
    m_interface (interface)
{
}
Ipv4RoutingTableEntry::Ipv4RoutingTableEntry (Ipv4Address network,
                                              Ipv4Mask networkMask,
                                              uint32_t interface)
  : m_dest (network),
    m_destNetworkMask (networkMask),
    m_gateway (Ipv4Address::GetZero ()),
    m_interface (interface)
{
}

bool
Ipv4RoutingTableEntry::IsHost (void) const
{
  if (m_destNetworkMask.IsEqual (Ipv4Mask::GetOnes ()))
    {
      return true;
    }
  else
    {
      return false;
    }
}
Ipv4Address
Ipv4RoutingTableEntry::GetDest (void) const
{
  return m_dest;
}
bool
Ipv4RoutingTableEntry::IsNetwork (void) const
{
  return !IsHost ();
}
bool
Ipv4RoutingTableEntry::IsDefault (void) const
{
  if (m_dest.IsEqual (Ipv4Address::GetZero ()))
    {
      return true;
    }
  else
    {
      return false;
    }
}
Ipv4Address
Ipv4RoutingTableEntry::GetDestNetwork (void) const
{
  return m_dest;
}
Ipv4Mask
Ipv4RoutingTableEntry::GetDestNetworkMask (void) const
{
  return m_destNetworkMask;
}
bool
Ipv4RoutingTableEntry::IsGateway (void) const
{
  if (m_gateway.IsEqual (Ipv4Address::GetZero ()))
    {
      return false;
    }
  else
    {
      return true;
    }
}
Ipv4Address
Ipv4RoutingTableEntry::GetGateway (void) const
{
  return m_gateway;
}
uint32_t
Ipv4RoutingTableEntry::GetInterface (void) const
{
  return m_interface;
}

Ipv4RoutingTableEntry 
Ipv4RoutingTableEntry::CreateHostRouteTo (Ipv4Address dest, 
                                          Ipv4Address nextHop,
                                          uint32_t interface)
{
  return Ipv4RoutingTableEntry (dest, nextHop, interface);
}
Ipv4RoutingTableEntry 
Ipv4RoutingTableEntry::CreateHostRouteTo (Ipv4Address dest,
                                          uint32_t interface)
{
  return Ipv4RoutingTableEntry (dest, interface);
}
Ipv4RoutingTableEntry 
Ipv4RoutingTableEntry::CreateNetworkRouteTo (Ipv4Address network, 
                                             Ipv4Mask networkMask,
                                             Ipv4Address nextHop,
                                             uint32_t interface)
{
  return Ipv4RoutingTableEntry (network, networkMask, 
                                nextHop, interface);
}
Ipv4RoutingTableEntry 
Ipv4RoutingTableEntry::CreateNetworkRouteTo (Ipv4Address network, 
                                             Ipv4Mask networkMask,
                                             uint32_t interface)
{
  return Ipv4RoutingTableEntry (network, networkMask, 
                                interface);
}
Ipv4RoutingTableEntry 
Ipv4RoutingTableEntry::CreateDefaultRoute (Ipv4Address nextHop, 
                                           uint32_t interface)
{
  return Ipv4RoutingTableEntry (Ipv4Address::GetZero (), nextHop, interface);
}


std::ostream& operator<< (std::ostream& os, Ipv4RoutingTableEntry const& route)
{
  if (route.IsDefault ())
    {
      NS_ASSERT (route.IsGateway ());
      os << "default out=" << route.GetInterface () << ", next hop=" << route.GetGateway ();
    }
  else if (route.IsHost ())
    {
      if (route.IsGateway ())
        {
          os << "host="<< route.GetDest () << 
          ", out=" << route.GetInterface () <<
          ", next hop=" << route.GetGateway ();
        }
      else
        {
          os << "host="<< route.GetDest () << 
          ", out=" << route.GetInterface ();
        }
    }
  else if (route.IsNetwork ()) 
    {
      if (route.IsGateway ())
        {
          os << "network=" << route.GetDestNetwork () <<
          ", mask=" << route.GetDestNetworkMask () <<
          ",out=" << route.GetInterface () <<
          ", next hop=" << route.GetGateway ();
        }
      else
        {
          os << "network=" << route.GetDestNetwork () <<
          ", mask=" << route.GetDestNetworkMask () <<
          ",out=" << route.GetInterface ();
        }
    }
  else
    {
      NS_ASSERT (false);
    }
  return os;
}

/*****************************************************
 *     Ipv4MulticastRoutingTableEntry
 *****************************************************/

Ipv4MulticastRoutingTableEntry::Ipv4MulticastRoutingTableEntry ()
{
}

Ipv4MulticastRoutingTableEntry::Ipv4MulticastRoutingTableEntry (Ipv4MulticastRoutingTableEntry const &route)
  :
    m_origin (route.m_origin),
    m_group (route.m_group),
    m_inputInterface (route.m_inputInterface),
    m_outputInterfaces (route.m_outputInterfaces)
{
}

Ipv4MulticastRoutingTableEntry::Ipv4MulticastRoutingTableEntry (Ipv4MulticastRoutingTableEntry const *route)
  :
    m_origin (route->m_origin),
    m_group (route->m_group),
    m_inputInterface (route->m_inputInterface),
    m_outputInterfaces (route->m_outputInterfaces)
{
}

Ipv4MulticastRoutingTableEntry::Ipv4MulticastRoutingTableEntry (
  Ipv4Address origin, 
  Ipv4Address group, 
  uint32_t inputInterface, 
  std::vector<uint32_t> outputInterfaces)
{
  m_origin = origin;
  m_group = group;
  m_inputInterface = inputInterface;
  m_outputInterfaces = outputInterfaces;
}

Ipv4Address 
Ipv4MulticastRoutingTableEntry::GetOrigin (void) const
{
  return m_origin;
}

Ipv4Address 
Ipv4MulticastRoutingTableEntry::GetGroup (void) const
{
  return m_group;
}

uint32_t 
Ipv4MulticastRoutingTableEntry::GetInputInterface (void) const
{
  return m_inputInterface;
}

uint32_t
Ipv4MulticastRoutingTableEntry::GetNOutputInterfaces (void) const
{
  return m_outputInterfaces.size ();
}

uint32_t
Ipv4MulticastRoutingTableEntry::GetOutputInterface (uint32_t n) const
{
  NS_ASSERT_MSG (n < m_outputInterfaces.size (),
                 "Ipv4MulticastRoutingTableEntry::GetOutputInterface (): index out of bounds");

  return m_outputInterfaces[n];
}

std::vector<uint32_t>
Ipv4MulticastRoutingTableEntry::GetOutputInterfaces (void) const
{
  return m_outputInterfaces;
}

Ipv4MulticastRoutingTableEntry 
Ipv4MulticastRoutingTableEntry::CreateMulticastRoute (
  Ipv4Address origin, 
  Ipv4Address group, 
  uint32_t inputInterface,
  std::vector<uint32_t> outputInterfaces)
{
  return Ipv4MulticastRoutingTableEntry (origin, group, inputInterface, outputInterfaces);
}

std::ostream& 
operator<< (std::ostream& os, Ipv4MulticastRoutingTableEntry const& route)
{
  os << "origin=" << route.GetOrigin () << 
  ", group=" << route.GetGroup () <<
  ", input interface=" << route.GetInputInterface () <<
  ", output interfaces=";

  for (uint32_t i = 0; i < route.GetNOutputInterfaces (); ++i)
    {
      os << route.GetOutputInterface (i) << " ";

    }

  return os;
}

} // namespace ns3
