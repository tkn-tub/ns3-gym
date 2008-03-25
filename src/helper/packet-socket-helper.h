#ifndef PACKET_SOCKET_HELPER_H
#define PACKET_SOCKET_HELPER_H

#include "ns3/node-container.h"

namespace ns3 {

/**
 * \brief Give ns3::PacketSocket powers to ns3::Node.
 */
class PacketSocketHelper
{
public:
  /**
   * \param c container of node pointers
   *
   * For each node in the input container, aggregate a ns3::PacketSocketFactory
   * object instance.
   */
  void Build (NodeContainer c);
};

} // namespace ns3

#endif /* PACKET_SOCKET_HELPER_H */
