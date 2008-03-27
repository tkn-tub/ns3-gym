#ifndef UDP_ECHO_HELPER_H
#define UDP_ECHO_HELPER_H

#include <stdint.h>
#include "application-container.h"
#include "node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

class UdpEchoServerHelper
{
public:
  UdpEchoServerHelper ();
  void SetPort (uint16_t port);
  ApplicationContainer Build (NodeContainer c);
private:
  uint16_t m_port;
};

class UdpEchoClientHelper
{
public:
  UdpEchoClientHelper ();

  void SetRemote (Ipv4Address ip, uint16_t port);
  void SetAppAttribute (std::string name, Attribute value);
  ApplicationContainer Build (NodeContainer c);
 private:
  ObjectFactory m_factory;
  Ipv4Address m_remoteIp;
  uint16_t m_remotePort;
};


} // namespace ns3

#endif /* UDP_ECHO_HELPER_H */
