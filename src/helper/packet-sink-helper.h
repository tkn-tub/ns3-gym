#ifndef PACKET_SINK_HELPER_H
#define PACKET_SINK_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "node-container.h"
#include "application-container.h"

namespace ns3 {

class PacketSinkHelper
{
public:
  PacketSinkHelper ();

  void SetupUdp (Ipv4Address ip, uint16_t port);
  void SetupTcp (Ipv4Address ip, uint16_t port);

  ApplicationContainer Build (NodeContainer c);
private:
  ObjectFactory m_factory;
};

} // namespace ns3

#endif /* PACKET_SINK_HELPER_H */
