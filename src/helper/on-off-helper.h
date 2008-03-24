#ifndef ON_OFF_HELPER_H
#define ON_OFF_HELPER_H

#include <stdint.h>
#include <string>
#include "ns3/object-factory.h"
#include "ns3/address.h"
#include "ns3/attribute.h"
#include "ns3/net-device.h"
#include "node-container.h"
#include "application-container.h"

namespace ns3 {

class OnOffHelper
{
public:
  OnOffHelper ();

  void SetUdpRemote (Ipv4Address ip, uint16_t port);
  void SetTcpRemote (Ipv4Address ip, uint16_t port);
  void SetPacketRemote (Ptr<NetDevice> source, Address destination, uint16_t protocol);
  
  void SetAppAttribute (std::string name, Attribute value);

  ApplicationContainer Build (NodeContainer c);

private:
  std::string m_protocol;
  Address m_remote;
  ObjectFactory m_factory;
};

} // namespace ns3

#endif /* ON_OFF_HELPER_H */

