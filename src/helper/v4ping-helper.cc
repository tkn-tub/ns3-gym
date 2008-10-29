#include "v4ping-helper.h"
#include "ns3/v4ping.h"

namespace ns3 {

V4PingHelper::V4PingHelper (Ipv4Address remote)
{
  m_factory.SetTypeId ("ns3::V4Ping");
  m_factory.Set ("Remote", Ipv4AddressValue (remote));
}

void 
V4PingHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer 
V4PingHelper::Install (NodeContainer nodes)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<V4Ping> ping = m_factory.Create<V4Ping> ();
      node->AddApplication (ping);
      apps.Add (ping);
    }
  return apps;
}


} // namespace ns3
