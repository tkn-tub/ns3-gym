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
		 std::string n1 = "", PValue v1 = PValue (),
		 std::string n2 = "", PValue v2 = PValue (),
		 std::string n3 = "", PValue v3 = PValue (),
		 std::string n4 = "", PValue v4 = PValue ());

  /**
   * Set these parameters on each PointToPointNetDevice created
   * by this helper.
   */
  void SetDeviceParameters (std::string n1, PValue v1,
			    std::string n2 = "", PValue v2 = PValue (),
			    std::string n3 = "", PValue v3 = PValue (),
			    std::string n4 = "", PValue v4 = PValue ());

  NetDeviceContainer Build (NodeContainer c);
  NetDeviceContainer Build (Ptr<Node> a, Ptr<Node> b);

private:
  ObjectFactory m_queueFactory;
};


} // namespace ns3

#endif /* POINT_TO_POINT_HELPER_H */
