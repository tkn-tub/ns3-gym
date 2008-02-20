#ifndef POINT_TO_POINT_HELPER_H
#define POINT_TO_POINT_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include <string>

namespace ns3 {

class PointToPointHelper
{
public:
  // by default, create queues of type DropTailQueue.
  PointToPointHelper ();

  void SetQueue (std::string type,
		 std::string n1 = "", Attribute v1 = Attribute (),
		 std::string n2 = "", Attribute v2 = Attribute (),
		 std::string n3 = "", Attribute v3 = Attribute (),
		 std::string n4 = "", Attribute v4 = Attribute ());

  /**
   * Set these parameters on each PointToPointNetDevice created
   * by this helper.
   */
  void SetDeviceParameters (std::string n1, Attribute v1,
			    std::string n2 = "", Attribute v2 = Attribute (),
			    std::string n3 = "", Attribute v3 = Attribute (),
			    std::string n4 = "", Attribute v4 = Attribute ());

  NetDeviceContainer Build (NodeContainer c);
  NetDeviceContainer Build (Ptr<Node> a, Ptr<Node> b);

private:
  ObjectFactory m_queueFactory;
};


} // namespace ns3

#endif /* POINT_TO_POINT_HELPER_H */
