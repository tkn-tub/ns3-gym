/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
 * All rights reserved.
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

#include "ipv4-route.h"
#include <cassert>

namespace ns3 {

/*****************************************************
 *     Network Ipv4Route
 *****************************************************/

Ipv4Route::Ipv4Route ()
{}
Ipv4Route::Ipv4Route (Ipv4Route const &route)
  : m_dest (route.m_dest),
    m_destNetworkMask (route.m_destNetworkMask),
    m_gateway (route.m_gateway),
    m_interface (route.m_interface)
{}

Ipv4Route::Ipv4Route (Ipv4Address dest,
                      Ipv4Address gateway,
                      uint32_t interface)
  : m_dest (dest),
    m_destNetworkMask (Ipv4Mask::GetZero ()),
    m_gateway (gateway),
    m_interface (interface)
{}
Ipv4Route::Ipv4Route (Ipv4Address dest,
                      uint32_t interface)
  : m_dest (dest),
    m_destNetworkMask (Ipv4Mask::GetZero ()),
    m_gateway (Ipv4Address::GetZero ()),
    m_interface (interface)
{}
Ipv4Route::Ipv4Route (Ipv4Address network,
                      Ipv4Mask networkMask,
                      Ipv4Address gateway,
                      uint32_t interface)
  : m_dest (network),
    m_destNetworkMask (networkMask),
    m_gateway (gateway),
    m_interface (interface)
{}
Ipv4Route::Ipv4Route (Ipv4Address network,
                      Ipv4Mask networkMask,
                      uint32_t interface)
  : m_dest (network),
    m_destNetworkMask (networkMask),
    m_gateway (Ipv4Address::GetZero ()),
    m_interface (interface)
{}

bool 
Ipv4Route::IsHost (void) const
{
  if (m_destNetworkMask.IsEqual (Ipv4Mask::GetZero ())) 
    {
      return true;
    } 
  else 
    {
      return false;
    }
}
Ipv4Address 
Ipv4Route::GetDest (void) const
{
  return m_dest;
}
bool 
Ipv4Route::IsNetwork (void) const
{
  return !IsHost ();
}
bool 
Ipv4Route::IsDefault (void) const
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
Ipv4Route::GetDestNetwork (void) const
{
  return m_dest;
}
Ipv4Mask 
Ipv4Route::GetDestNetworkMask (void) const
{
  return m_destNetworkMask;
}
bool 
Ipv4Route::IsGateway (void) const
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
Ipv4Route::GetGateway (void) const
{
  return m_gateway;
}
uint32_t
Ipv4Route::GetInterface (void) const
{
  return m_interface;
}


Ipv4Route 
Ipv4Route::CreateHostRouteTo (Ipv4Address dest, 
			      Ipv4Address nextHop, 
			      uint32_t interface)
{
  return Ipv4Route (dest, nextHop, interface);
}
Ipv4Route 
Ipv4Route::CreateHostRouteTo (Ipv4Address dest,
			      uint32_t interface)
{
  return Ipv4Route (dest, interface);
}
Ipv4Route 
Ipv4Route::CreateNetworkRouteTo (Ipv4Address network, 
				 Ipv4Mask networkMask, 
				 Ipv4Address nextHop, 
				 uint32_t interface)
{
  return Ipv4Route (network, networkMask, 
                    nextHop, interface);
}
Ipv4Route 
Ipv4Route::CreateNetworkRouteTo (Ipv4Address network, 
				 Ipv4Mask networkMask, 
				 uint32_t interface)
{
  return Ipv4Route (network, networkMask, 
                    interface);
}
Ipv4Route 
Ipv4Route::CreateDefaultRoute (Ipv4Address nextHop, 
			       uint32_t interface)
{
  return Ipv4Route (Ipv4Address::GetZero (), nextHop, interface);
}


std::ostream& operator<< (std::ostream& os, Ipv4Route const& route)
{
  if (route.IsDefault ())
    {
      assert (route.IsGateway ());
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
      assert (false);
    }
  return os;
}

}//namespace ns3
