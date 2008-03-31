/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
#include "olsr-helper.h"
#include "ns3/olsr-agent.h"
#include "ns3/node-list.h"

namespace ns3 {

OlsrHelper::OlsrHelper ()
{
  m_agentFactory.SetTypeId ("ns3::olsr::AgentImpl");
}

void 
OlsrHelper::SetAgent (std::string tid,
		      std::string n0, Attribute v0,
		      std::string n1, Attribute v1,
		      std::string n2, Attribute v2,
		      std::string n3, Attribute v3,
		      std::string n4, Attribute v4,
		      std::string n5, Attribute v5,
		      std::string n6, Attribute v6,
		      std::string n7, Attribute v7)
{
  m_agentFactory.SetTypeId (tid);
  m_agentFactory.Set (n0, v0);
  m_agentFactory.Set (n1, v1);
  m_agentFactory.Set (n2, v2);
  m_agentFactory.Set (n3, v3);
  m_agentFactory.Set (n4, v4);
  m_agentFactory.Set (n5, v5);
  m_agentFactory.Set (n6, v6);
  m_agentFactory.Set (n7, v7);
}

void 
OlsrHelper::Enable (NodeContainer container)
{
  for (NodeContainer::Iterator i = container.Begin (); i != container.End (); ++i)
    {
      Ptr<Node> node = *i;
      Enable (node);
    }
}
void 
OlsrHelper::Enable (Ptr<Node> node)
{
  Ptr<olsr::Agent> agent = m_agentFactory.Create<olsr::Agent> ();
  agent->SetNode (node);
  node->AggregateObject (agent);
  agent->Start ();
}
void 
OlsrHelper::EnableAll (void)
{
  Enable (NodeContainer::GetGlobal ());
}

} // namespace ns3
