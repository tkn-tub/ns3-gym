#ifndef CSMA_HELPER_H
#define CSMA_HELPER_H

#include <string>
#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "ns3/csma-channel.h"

namespace ns3 {

/**
 * \brief build a set of CsmaNetDevice objects
 */
class CsmaHelper
{
public:
  CsmaHelper ();

  /**
   * \param type the type of queue
   * \param n1 the name of the attribute to set on the queue
   * \param v1 the value of the attribute to set on the queue
   * \param n2 the name of the attribute to set on the queue
   * \param v2 the value of the attribute to set on the queue
   * \param n3 the name of the attribute to set on the queue
   * \param v3 the value of the attribute to set on the queue
   * \param n4 the name of the attribute to set on the queue
   * \param v4 the value of the attribute to set on the queue
   *
   * Set the type of queue to create and associated to each
   * CsmaNetDevice created through CsmaHelper::Build.
   */
  void SetQueue (std::string type,
		 std::string n1 = "", Attribute v1 = Attribute (),
		 std::string n2 = "", Attribute v2 = Attribute (),
		 std::string n3 = "", Attribute v3 = Attribute (),
		 std::string n4 = "", Attribute v4 = Attribute ());

  /**
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   *
   * Set these parameters on each CsmaNetDevice created
   * by CsmaHelper::Build
   */
  void SetDeviceParameter (std::string n1, Attribute v1);

  /**
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   *
   * Set these parameters on each CsmaChannel created
   * by CsmaHelper::Build
   */
  void SetChannelParameter (std::string n1, Attribute v1);

  /**
   * \param c a set of nodes
   *
   * This method creates a simple ns3::CsmaChannel with the
   * attributes configured by CsmaHelper::SetChannelParameter and
   * then calls CsmaHelper::Build.
   */
  NetDeviceContainer Build (const NodeContainer &c);

  /**
   * \param c a set of nodes
   * \param channel the channel to use as a backbone.
   *
   * For each node in the input container, we create a ns::CsmaNetDevice with
   * the requested parameters, a queue for this NetDevice, and associate
   * the resulting ns3::NetDevice with the ns3::Node and ns3::CsmaChannel.
   */
  NetDeviceContainer Build (const NodeContainer &c, Ptr<CsmaChannel> channel);

private:
  ObjectFactory m_queueFactory;
  ObjectFactory m_deviceFactory;
  ObjectFactory m_channelFactory;
};


} // namespace ns3

#endif /* CSMA_HELPER_H */
