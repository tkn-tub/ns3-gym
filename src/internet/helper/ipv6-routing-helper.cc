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

#include "ns3/node.h"
#include "ns3/node-list.h"
#include "ns3/simulator.h"
#include "ns3/ipv6-routing-protocol.h"
#include "ipv6-routing-helper.h"

namespace ns3 {

Ipv6RoutingHelper::~Ipv6RoutingHelper ()
{
}

void
Ipv6RoutingHelper::PrintRoutingTableAllAt (Time printTime, Ptr<OutputStreamWrapper> stream) const
{
  for (uint32_t i = 0; i < NodeList::GetNNodes (); i++)
    {
      Ptr<Node> node = NodeList::GetNode (i);
      Simulator::Schedule (printTime, &Ipv6RoutingHelper::Print, this, node, stream);
    }
}

void
Ipv6RoutingHelper::PrintRoutingTableAllEvery (Time printInterval, Ptr<OutputStreamWrapper> stream) const
{
  for (uint32_t i = 0; i < NodeList::GetNNodes (); i++)
    {
      Ptr<Node> node = NodeList::GetNode (i);
      Simulator::Schedule (printInterval, &Ipv6RoutingHelper::PrintEvery, this, printInterval, node, stream);
    }
}

void
Ipv6RoutingHelper::PrintRoutingTableAt (Time printTime, Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const
{
  Simulator::Schedule (printTime, &Ipv6RoutingHelper::Print, this, node, stream);
}

void
Ipv6RoutingHelper::PrintRoutingTableEvery (Time printInterval,Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const
{
  Simulator::Schedule (printInterval, &Ipv6RoutingHelper::PrintEvery, this, printInterval, node, stream);
}

void
Ipv6RoutingHelper::Print (Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const
{
  Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
  Ptr<Ipv6RoutingProtocol> rp = ipv6->GetRoutingProtocol ();
  NS_ASSERT (rp);
  rp->PrintRoutingTable (stream);
}

void
Ipv6RoutingHelper::PrintEvery (Time printInterval, Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const
{
  Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
  Ptr<Ipv6RoutingProtocol> rp = ipv6->GetRoutingProtocol ();
  NS_ASSERT (rp);
  rp->PrintRoutingTable (stream);
  Simulator::Schedule (printInterval, &Ipv6RoutingHelper::PrintEvery, this, printInterval, node, stream);
}

} // namespace ns3
