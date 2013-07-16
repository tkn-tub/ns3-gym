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
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-list-routing.h"
#include "ipv4-routing-helper.h"

namespace ns3 {

Ipv4RoutingHelper::~Ipv4RoutingHelper ()
{
}

void
Ipv4RoutingHelper::PrintRoutingTableAllAt (Time printTime, Ptr<OutputStreamWrapper> stream) const
{
  for (uint32_t i = 0; i < NodeList::GetNNodes (); i++)
    {
      Ptr<Node> node = NodeList::GetNode (i);
      Simulator::Schedule (printTime, &Ipv4RoutingHelper::Print, this, node, stream);
    }
}

void
Ipv4RoutingHelper::PrintRoutingTableAllEvery (Time printInterval, Ptr<OutputStreamWrapper> stream) const
{
  for (uint32_t i = 0; i < NodeList::GetNNodes (); i++)
    {
      Ptr<Node> node = NodeList::GetNode (i);
      Simulator::Schedule (printInterval, &Ipv4RoutingHelper::PrintEvery, this, printInterval, node, stream);
    }
}

void
Ipv4RoutingHelper::PrintRoutingTableAt (Time printTime, Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const
{
  Simulator::Schedule (printTime, &Ipv4RoutingHelper::Print, this, node, stream);
}

void
Ipv4RoutingHelper::PrintRoutingTableEvery (Time printInterval,Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const
{
  Simulator::Schedule (printInterval, &Ipv4RoutingHelper::PrintEvery, this, printInterval, node, stream);
}

void
Ipv4RoutingHelper::Print (Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const
{
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  Ptr<Ipv4RoutingProtocol> rp = ipv4->GetRoutingProtocol ();
  NS_ASSERT (rp);
  rp->PrintRoutingTable (stream);
}

void
Ipv4RoutingHelper::PrintEvery (Time printInterval, Ptr<Node> node, Ptr<OutputStreamWrapper> stream) const
{
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  Ptr<Ipv4RoutingProtocol> rp = ipv4->GetRoutingProtocol ();
  NS_ASSERT (rp);
  rp->PrintRoutingTable (stream);
  Simulator::Schedule (printInterval, &Ipv4RoutingHelper::PrintEvery, this, printInterval, node, stream);
}

template<class T>
Ptr<T> Ipv4RoutingHelper::GetRouting (Ptr<Ipv4RoutingProtocol> protocol)
{
  Ptr<T> ret = DynamicCast<T> (protocol);
  if (ret == 0)
    {
      // trying to check if protocol is a list routing
      Ptr<Ipv4ListRouting> lrp = DynamicCast<Ipv4ListRouting> (protocol);
      if (lrp != 0)
        {
          for (uint32_t i = 0; i < lrp->GetNRoutingProtocols ();  i++)
            {
              int16_t priority;
              ret = GetRouting<T> (lrp->GetRoutingProtocol (i, priority)); // potential recursion, if inside ListRouting is ListRouting
              if (ret != 0)
                break;
            }
        }
    }

  return ret;
}

} // namespace ns3
