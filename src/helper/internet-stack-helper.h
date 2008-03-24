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
  void Build (NodeContainer c);
};

} // namespace ns3

#endif /* INTERNET_STACK_HELPER_H */
