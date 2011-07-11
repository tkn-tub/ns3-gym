/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 The Georgia Institute of Technology 
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
 * Authors: Josh Pelkey <jpelkey@gatech.edu>
 */

#include "ipv4-nix-vector-helper.h"
#include "ns3/ipv4-nix-vector-routing.h"

namespace ns3 {

Ipv4NixVectorHelper::Ipv4NixVectorHelper ()
{
  m_agentFactory.SetTypeId ("ns3::Ipv4NixVectorRouting");
}

Ipv4NixVectorHelper::Ipv4NixVectorHelper (const Ipv4NixVectorHelper &o)
  : m_agentFactory (o.m_agentFactory)
{
}

Ipv4NixVectorHelper* 
Ipv4NixVectorHelper::Copy (void) const 
{
  return new Ipv4NixVectorHelper (*this); 
}

Ptr<Ipv4RoutingProtocol> 
Ipv4NixVectorHelper::Create (Ptr<Node> node) const
{
  Ptr<Ipv4NixVectorRouting> agent = m_agentFactory.Create<Ipv4NixVectorRouting> ();
  agent->SetNode (node);
  node->AggregateObject (agent);
  return agent;
}
} // namespace ns3
