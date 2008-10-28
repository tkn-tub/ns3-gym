#include "bridge-helper.h"

#include "ns3/bridge-net-device.h"
#include "ns3/node.h"

namespace ns3 {

BridgeHelper::BridgeHelper ()
{
  m_deviceFactory.SetTypeId ("ns3::BridgeNetDevice");
}

void 
BridgeHelper::SetDeviceAttribute (std::string n1, const AttributeValue &v1)
{
  m_deviceFactory.Set (n1, v1);
}

NetDeviceContainer
BridgeHelper::Install (Ptr<Node> node, NetDeviceContainer c)
{
  NetDeviceContainer devs;
  Ptr<BridgeNetDevice> dev = m_deviceFactory.Create<BridgeNetDevice> ();
  devs.Add (dev);
  node->AddDevice (dev);

  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      dev->AddBridgePort (*i);
    }
  return devs;
}

} // namespace ns3
