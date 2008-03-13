#ifndef CSMA_HELPER_H
#define CSMA_HELPER_H

#include <string>
#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "ns3/csma-channel.h"

namespace ns3 {

class CsmaHelper
{
public:
  CsmaHelper ();

  void SetQueue (std::string type,
		 std::string n1 = "", Attribute v1 = Attribute (),
		 std::string n2 = "", Attribute v2 = Attribute (),
		 std::string n3 = "", Attribute v3 = Attribute (),
		 std::string n4 = "", Attribute v4 = Attribute ());

  /**
   * Set these parameters on each PointToPointNetDevice created
   * by this helper.
   */
  void SetDeviceParameter (std::string n1, Attribute v1);

  void SetChannelParameter (std::string n1, Attribute v1);

  NetDeviceContainer Build (const NodeContainer &c);

  NetDeviceContainer Build (const NodeContainer &c, Ptr<CsmaChannel> channel);

private:
  ObjectFactory m_queueFactory;
  ObjectFactory m_deviceFactory;
  ObjectFactory m_channelFactory;
};


} // namespace ns3

#endif /* CSMA_HELPER_H */
