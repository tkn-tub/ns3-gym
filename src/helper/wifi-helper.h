#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <string>
#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"

namespace ns3 {

class WifiChannel;

class WifiHelper
{
public:
  WifiHelper ();

  void SetRemoteStationManager (std::string type,
				std::string n0 = "", Attribute v0 = Attribute (),
				std::string n1 = "", Attribute v1 = Attribute (),
				std::string n2 = "", Attribute v2 = Attribute (),
				std::string n3 = "", Attribute v3 = Attribute (),
				std::string n4 = "", Attribute v4 = Attribute (),
				std::string n5 = "", Attribute v5 = Attribute (),
				std::string n6 = "", Attribute v6 = Attribute (),
				std::string n7 = "", Attribute v7 = Attribute ());

  void SetMac (std::string type,
	       std::string n0 = "", Attribute v0 = Attribute (),
	       std::string n1 = "", Attribute v1 = Attribute (),
	       std::string n2 = "", Attribute v2 = Attribute (),
	       std::string n3 = "", Attribute v3 = Attribute (),
	       std::string n4 = "", Attribute v4 = Attribute (),
	       std::string n5 = "", Attribute v5 = Attribute (),
	       std::string n6 = "", Attribute v6 = Attribute (),
	       std::string n7 = "", Attribute v7 = Attribute ());

  void SetPhy (std::string phyType,
	       std::string n0 = "", Attribute v0 = Attribute (),
	       std::string n1 = "", Attribute v1 = Attribute (),
	       std::string n2 = "", Attribute v2 = Attribute (),
	       std::string n3 = "", Attribute v3 = Attribute (),
	       std::string n4 = "", Attribute v4 = Attribute (),
	       std::string n5 = "", Attribute v5 = Attribute (),
	       std::string n6 = "", Attribute v6 = Attribute (),
	       std::string n7 = "", Attribute v7 = Attribute ());

  NetDeviceContainer Build (NodeContainer c) const;
  NetDeviceContainer Build (NodeContainer c, Ptr<WifiChannel> channel) const;

private:
  ObjectFactory m_stationManager;
  ObjectFactory m_mac;
  ObjectFactory m_phy;
};

} // namespace ns3

#endif /* WIFI_HELPER_H */
