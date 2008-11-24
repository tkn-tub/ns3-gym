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
V4PingHelper::Install (Ptr<Node> node) const
{
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
V4PingHelper::Install (NodeContainer c) const
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}

Ptr<Application>
V4PingHelper::InstallPriv (Ptr<Node> node) const
{
  Ptr<V4Ping> app = m_factory.Create<V4Ping> ();
  node->AddApplication (app);

  return app;
}

} // namespace ns3
