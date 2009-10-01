#ifndef V4PING_HELPER_H
#define V4PING_HELPER_H

#include "node-container.h"
#include "application-container.h"
#include "ns3/object-factory.h"

namespace ns3 {

/**
 * \brief create a pinger application and associate it to a node
 *
 * This class creates one or multiple instances of ns3::V4Ping and associates
 * it/them to one/multiple node(s).
 */
class V4PingHelper
{
public:
  /**
   * \param remote the address which should be pinged
   */
  V4PingHelper (Ipv4Address remote);

  /**
   * \param nodes the list of nodes.
   *
   * Install a pinger application on each node in the input list of nodes.
   * \returns a list of pinger applications, one for each input node
   */
  ApplicationContainer Install (NodeContainer nodes) const;
  /**
   * \param node the node
   *
   * Install a pinger application on the input node
   * \returns the pinger application created.
   */
  ApplicationContainer Install (Ptr<Node> node) const;
  /**
   * \param nodeName the node
   *
   * Install a pinger application on the input node
   * \returns the pinger application created.
   */
  ApplicationContainer Install (std::string nodeName) const;

private:
  Ptr<Application> InstallPriv (Ptr<Node> node) const;
  ObjectFactory m_factory;
};

} // namespace ns3

#endif /* V4PING_HELPER_H */
