#ifndef TAP_HELPER_H
#define TAP_HELPER_H

#include "ns3/ipv4-address.h"
#include "node-container.h"
#include "net-device-container.h"

namespace ns3 {

class PcapWriter;
class Packet;

class TapHelper 
{
public:
  /**
   * \param host a node which represents the host which our tap is connected to
   * \param tap the simulation node which should get a tap connected to the host.
   */
  NetDeviceContainer Install (NodeContainer host, NodeContainer tap);
  void SetupHost (NetDeviceContainer dev, Ipv4Address ad, Ipv4Mask mask, Ipv4Address gateway);

  static void EnablePcap (std::string filename, uint32_t nodeid, uint32_t deviceid);
  static void EnablePcap (std::string filename, NetDeviceContainer d);
  static void EnablePcap (std::string filename, NodeContainer n);
  static void EnablePcapAll (std::string filename);
};


} // namespace ns3

#endif /* TAP_HELPER_H */
