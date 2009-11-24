/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Authors: Pavel Boyko <boyko@iitp.ru>, written after OlsrHelper by Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "aodv-helper.h"
#include "ns3/aodv-routing-protocol.h"
#include "ns3/node-list.h"
#include "ns3/names.h"
#include "ns3/ipv4-list-routing.h"

namespace ns3
{

AodvHelper::AodvHelper() : 
  Ipv4RoutingHelper ()
{
  m_agentFactory.SetTypeId ("ns3::aodv::RoutingProtocol");
}

AodvHelper* 
AodvHelper::Copy (void) const 
{
  return new AodvHelper (*this); 
}

Ptr<Ipv4RoutingProtocol> 
AodvHelper::Create (Ptr<Node> node) const
{
  Ptr<aodv::RoutingProtocol> agent = m_agentFactory.Create<aodv::RoutingProtocol> ();
  node->AggregateObject (agent);
  return agent;
}

void 
AodvHelper::Set (std::string name, const AttributeValue &value)
{
  m_agentFactory.Set (name, value);
}

}
