#ifndef INTERNET_STACK_HELPER_H
#define INTERNET_STACK_HELPER_H

#include "node-container.h"

namespace ns3 {

/**
 * \brief aggregate ip/tcp/udp functionality to existing Nodes.
 */
class InternetStackHelper
{
public:
  /**
   * \param c the set of nodes
   *
   * For each node in the input container, aggregate implementations
   * of the ns3::Ipv4, ns3::Udp, and, ns3::Tcp classes.
   */
  void Build (NodeContainer c);
};

} // namespace ns3

#endif /* INTERNET_STACK_HELPER_H */
