#ifndef V4PING_HELPER_H
#define V4PING_HELPER_H

#include "node-container.h"
#include "application-container.h"
#include "ns3/object-factory.h"

namespace ns3 {

class V4PingHelper
{
public:
  V4PingHelper (Ipv4Address remote);

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer nodes);

private:
  ObjectFactory m_factory;
};

} // namespace ns3

#endif /* V4PING_HELPER_H */
