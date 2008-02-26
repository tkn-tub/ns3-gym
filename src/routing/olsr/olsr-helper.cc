#include "olsr-helper.h"
#include "olsr-agent.h"
#include "ns3/node-list.h"

namespace ns3 {

OlsrHelper::OlsrHelper ()
{
  m_agentFactory.SetTypeId ("olsr::AgentImpl");
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
  Enable (container.Begin (), container.End ());
}
void 
OlsrHelper::Enable (Ptr<Node> node)
{
  Ptr<olsr::Agent> agent = m_agentFactory.Create<olsr::Agent> ();
  agent->SetNode (node);
  node->AggregateObject (node);
}
void 
OlsrHelper::EnableAll (void)
{
  Enable (NodeList::Begin (), NodeList::End ());
}

} // namespace ns3
