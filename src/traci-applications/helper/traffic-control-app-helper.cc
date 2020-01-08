#include "traffic-control-app-helper.h"

#include "ns3/traffic-control-app.h"
#include "ns3/uinteger.h"
#include "ns3/names.h"

namespace ns3 {

RsuSpeedControlHelper::RsuSpeedControlHelper (uint16_t port)
{
  m_factory.SetTypeId (RsuSpeedControl::GetTypeId ());
  SetAttribute ("Port", UintegerValue (port));
}

void 
RsuSpeedControlHelper::SetAttribute (
  std::string name, 
  const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
RsuSpeedControlHelper::Install (Ptr<Node> node) const
{
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
RsuSpeedControlHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
RsuSpeedControlHelper::Install (NodeContainer c) const
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}

Ptr<Application>
RsuSpeedControlHelper::InstallPriv (Ptr<Node> node) const
{
  Ptr<Application> app = m_factory.Create<RsuSpeedControl> ();
  node->AddApplication (app);

  return app;
}

VehicleSpeedControlHelper::VehicleSpeedControlHelper (uint16_t port)
{
  m_factory.SetTypeId (VehicleSpeedControl::GetTypeId ());
  SetAttribute ("Port", UintegerValue (port));
}

void 
VehicleSpeedControlHelper::SetAttribute (
  std::string name, 
  const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
VehicleSpeedControlHelper::Install (Ptr<Node> node) const
{
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
VehicleSpeedControlHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
VehicleSpeedControlHelper::Install (NodeContainer c) const
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}

Ptr<Application>
VehicleSpeedControlHelper::InstallPriv (Ptr<Node> node) const
{
  Ptr<Application> app = m_factory.Create<VehicleSpeedControl> ();
  node->AddApplication (app);

  return app;
}

} // namespace ns3
