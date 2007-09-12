/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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

#include "ns3/assert.h" 
#include "ns3/node.h" 
#include "ipv4.h"

namespace ns3 {

const InterfaceId Ipv4::iid = MakeInterfaceId ("Ipv4", Object::iid);

Ipv4::Ipv4 ()
{
  SetInterfaceId (Ipv4::iid);
}

Ipv4::~Ipv4 ()
{}

uint32_t 
Ipv4::GetIfIndexByAddress (Ptr<Node> node, Ipv4Address a, Ipv4Mask amask)
{
  Ptr<Ipv4> ipv4 = node->QueryInterface<Ipv4> (Ipv4::iid);
  NS_ASSERT_MSG (ipv4, "Ipv4::GetIfIndexByAddress:  No Ipv4 interface");
  for (uint32_t i = 0; i < ipv4->GetNInterfaces (); i++)
    {
      if (ipv4->GetAddress (i).CombineMask(amask) == a.CombineMask(amask) )
        {
          return i;
        }
    }
  // Mapping not found
  NS_ASSERT_MSG (false, "Ipv4::GetIfIndexByAddress failed");
  return 0;
}

//
// XXX BUGBUG I don't think this is really the right approach here.  The call
// to GetRoute () filters down into Ipv4L3Protocol where it translates into
// a call into the Ipv4 static routing package.  This bypasses any other
// routing packages.  At a minimum, the name is misleading.
//
bool 
Ipv4::GetRouteToDestination (
  Ptr<Node> node, 
  Ipv4Route& route, 
  Ipv4Address a, 
  Ipv4Mask amask)
{
  Ipv4Route tempRoute;
  Ptr<Ipv4> ipv4 = node->QueryInterface<Ipv4> (Ipv4::iid);
  NS_ASSERT_MSG (ipv4, "Ipv4::GetRouteToDestination:  No Ipv4 interface");
  for (uint32_t i = 0; i < ipv4->GetNRoutes (); i++) 
    {
      tempRoute = ipv4->GetRoute (i);
      // Host route found
      if ( tempRoute.IsNetwork () == false && tempRoute.GetDest () == a ) 
        {
          route = tempRoute;
          return true;
        }
      else if ( tempRoute.IsNetwork () && 
                tempRoute.GetDestNetwork () == a.CombineMask(amask) )
        {
          route = tempRoute;
          return true;
        }
      else if ( tempRoute.IsDefault () )
        {
          route = tempRoute;
          return true;
        }
    }
  return false;
}

} // namespace ns3
