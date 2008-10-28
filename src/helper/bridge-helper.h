#ifndef BRIDGE_HELPER_H
#define BRIDGE_HELPER_H

#include "net-device-container.h"
#include "ns3/object-factory.h"
#include <string>

namespace ns3 {

class Node;
class AttributeValue;

class BridgeHelper
{
public:
  BridgeHelper ();
  void SetDeviceAttribute (std::string n1, const AttributeValue &v1);
  NetDeviceContainer Install (Ptr<Node> node, NetDeviceContainer c);
private:
  ObjectFactory m_deviceFactory;
};

} // namespace ns3


#endif /* BRIDGE_HELPER_H */
