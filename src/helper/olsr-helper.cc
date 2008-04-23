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
		      std::string n0, const AttributeValue &v0,
		      std::string n1, const AttributeValue &v1,
		      std::string n2, const AttributeValue &v2,
		      std::string n3, const AttributeValue &v3,
		      std::string n4, const AttributeValue &v4,
		      std::string n5, const AttributeValue &v5,
		      std::string n6, const AttributeValue &v6,
		      std::string n7, const AttributeValue &v7)
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
OlsrHelper::Install (NodeContainer container)
{
  for (NodeContainer::Iterator i = container.Begin (); i != container.End (); ++i)
    {
      Ptr<Node> node = *i;
      Install (node);
    }
}
void 
OlsrHelper::Install (Ptr<Node> node)
{
  if (node->GetObject<olsr::Agent> () != 0)
    {
      NS_FATAL_ERROR ("OlsrHelper::Install(): Aggregating "
         "an Olsr Agent to a node with an existing Olsr Agent");
      return;
    }
  Ptr<olsr::Agent> agent = m_agentFactory.Create<olsr::Agent> ();
  agent->SetNode (node);
  node->AggregateObject (agent);
  agent->Start ();
}
void 
OlsrHelper::InstallAll (void)
{
  Install (NodeContainer::GetGlobal ());
}

} // namespace ns3
