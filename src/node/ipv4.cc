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

NS_OBJECT_ENSURE_REGISTERED (Ipv4);

TypeId 
Ipv4::GetTypeId (void)
{
  static TypeId iid = TypeId ("Ipv4")
    .SetParent<Object> ();
  return iid;
}

Ipv4::Ipv4 ()
{}

Ipv4::~Ipv4 ()
{}

uint32_t 
Ipv4::GetIfIndexByAddress (Ipv4Address addr, Ipv4Mask mask)
{
  for (uint32_t i = 0; i < GetNInterfaces (); i++)
    {
      if (GetAddress (i).CombineMask(mask) == addr.CombineMask(mask) )
        {
          return i;
        }
    }
  // Mapping not found
  NS_ASSERT_MSG (false, "Ipv4::GetIfIndexByAddress failed");
  return 0;
}

} // namespace ns3
