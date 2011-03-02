#ifndef V4PING_HELPER_H
#define V4PING_HELPER_H

#include "ns3/node-container.h"
#include "ns3/application-container.h"
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
   * Create a V4PingHelper which is used to make life easier for people wanting
   * to use ping Applications.
   *
   * \param remote The address which should be pinged
   */
  V4PingHelper (Ipv4Address remote);

  /**
   * Install a pinger application on each Node in the provided NodeContainer.
   *
   * \param nodes The NodeContainer containing all of the nodes to get a V4Ping
   *              application.
   *
   * \returns A list of pinger applications, one for each input node
   */
  ApplicationContainer Install (NodeContainer nodes) const;

  /**
   * Install a pinger application on the provided Node.  The Node is specified
   * directly by a Ptr<Node>
   *
   * \param node The node to install the V4PingApplication on.
   *
   * \returns An ApplicationContainer holding the pinger application created.
   */
  ApplicationContainer Install (Ptr<Node> node) const;

  /**
   * Install a pinger application on the provided Node.  The Node is specified
   * by a string that must have previously been associated with a Node using the
   * Object Name Service.
   *
   * \param nodeName The node to install the V4PingApplication on.
   *
   * \returns An ApplicationContainer holding the pinger application created.
   */
  ApplicationContainer Install (std::string nodeName) const;

  /**
   * \brief Configure ping applications attribute 
   * \param name   attribute's name
   * \param value  attribute's value
   */
  void SetAttribute (std::string name, const AttributeValue &value);
private:
  /**
   * \internal
   */
  Ptr<Application> InstallPriv (Ptr<Node> node) const;
  ObjectFactory m_factory;
};

} // namespace ns3

#endif /* V4PING_HELPER_H */
