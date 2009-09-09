/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
#include "ipv6-list-routing-helper.h"
#include "ns3/ipv6-list-routing.h"
#include "ns3/node.h"

namespace ns3 {

Ipv6ListRoutingHelper::Ipv6ListRoutingHelper ()
{}
void 
Ipv6ListRoutingHelper::Add (const Ipv6RoutingHelper &routing, int16_t priority)
{
  m_list.push_back (std::make_pair (&routing,priority));
}
Ptr<Ipv6RoutingProtocol> 
Ipv6ListRoutingHelper::Create (Ptr<Node> node) const
{
  Ptr<Ipv6ListRouting> list = CreateObject<Ipv6ListRouting> ();
  for (std::list<std::pair<const Ipv6RoutingHelper *,int16_t> >::const_iterator i = m_list.begin ();
       i != m_list.end (); ++i)
    {
      Ptr<Ipv6RoutingProtocol> prot = i->first->Create (node);
      list->AddRoutingProtocol (prot,i->second);
    }
  return list;
}

} // namespace ns3
