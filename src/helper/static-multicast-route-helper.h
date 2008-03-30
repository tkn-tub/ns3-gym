#ifndef STATIC_MULTICAST_ROUTE_HELPER_H
#define STATIC_MULTICAST_ROUTE_HELPER_H

#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "node-container.h"
#include "net-device-container.h"

namespace ns3 {

class StaticMulticastRouteHelper
{
public:
  StaticMulticastRouteHelper ();

  void AddMulticastRoute (Ptr<Node>, Ipv4Address source, Ipv4Address group,  
    Ptr<NetDevice> input, NetDeviceContainer output);

  void SetDefaultMulticastRoute (Ptr<Node> n, Ptr<NetDevice> nd);

  void JoinMulticastGroup (Ptr<Node> n, Ipv4Address source, Ipv4Address group);

};

} // namespace ns3

#endif /* STATIC_MULTICAST_ROUTE_HELPER_H */
