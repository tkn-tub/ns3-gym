#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <string>
#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"

namespace ns3 {

class WifiChannel;

/**
 * \brief helps to create WifiNetDevice objects
 *
 * This class can help to create a large set of similar
 * WifiNetDevice objects and to configure a large set of
 * their parameters during creation.
 */
class WifiHelper
{
public:
  WifiHelper ();

  /**
   * \param type the type of ns3::WifiRemoteStationManager to create.
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   *
   * All the attributes specified in this method should exist
   * in the requested station manager.
   */
  void SetRemoteStationManager (std::string type,
				std::string n0 = "", Attribute v0 = Attribute (),
				std::string n1 = "", Attribute v1 = Attribute (),
				std::string n2 = "", Attribute v2 = Attribute (),
				std::string n3 = "", Attribute v3 = Attribute (),
				std::string n4 = "", Attribute v4 = Attribute (),
				std::string n5 = "", Attribute v5 = Attribute (),
				std::string n6 = "", Attribute v6 = Attribute (),
				std::string n7 = "", Attribute v7 = Attribute ());

  /**
   * \param type the type of ns3::WifiMac to create.
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   *
   * All the attributes specified in this method should exist
   * in the requested mac.
   */
  void SetMac (std::string type,
	       std::string n0 = "", Attribute v0 = Attribute (),
	       std::string n1 = "", Attribute v1 = Attribute (),
	       std::string n2 = "", Attribute v2 = Attribute (),
	       std::string n3 = "", Attribute v3 = Attribute (),
	       std::string n4 = "", Attribute v4 = Attribute (),
	       std::string n5 = "", Attribute v5 = Attribute (),
	       std::string n6 = "", Attribute v6 = Attribute (),
	       std::string n7 = "", Attribute v7 = Attribute ());

  /**
   * \param phyType the type of ns3::WifiPhy to create.
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   *
   * All the attributes specified in this method should exist
   * in the requested phy.
   */
  void SetPhy (std::string phyType,
	       std::string n0 = "", Attribute v0 = Attribute (),
	       std::string n1 = "", Attribute v1 = Attribute (),
	       std::string n2 = "", Attribute v2 = Attribute (),
	       std::string n3 = "", Attribute v3 = Attribute (),
	       std::string n4 = "", Attribute v4 = Attribute (),
	       std::string n5 = "", Attribute v5 = Attribute (),
	       std::string n6 = "", Attribute v6 = Attribute (),
	       std::string n7 = "", Attribute v7 = Attribute ());


  static void EnablePcap (std::string filename, uint32_t nodeid, uint32_t deviceid);

  /**
   * \param c a set of nodes
   *
   * This method creates a simple ns3::WifiChannel (with a default
   * ns3::PropagationLossModel and ns3::PropagationDelayModel) and 
   * creates, for each of the input nodes, a new ns3::WifiNetDevice 
   * attached to this shared channel. Each ns3::WifiNetDevice is also
   * configured with an ns3::WifiRemoteStationManager, ns3::WifiMac, and,
   * ns3::WifiPhy, all of which are created based on the user-specified
   * attributes specified in WifiHelper::SetRemoteStationManager, 
   * WifiHelper::SetMac, and, WifiHelper::SetPhy.
   */
  NetDeviceContainer Build (NodeContainer c) const;
  /**
   * \param channel a channel to use
   * \param c a set of nodes
   *
   * For each of the input nodes, a new ns3::WifiNetDevice is attached 
   * to the shared input channel. Each ns3::WifiNetDevice is also
   * configured with an ns3::WifiRemoteStationManager, ns3::WifiMac, and,
   * ns3::WifiPhy, all of which are created based on the user-specified
   * attributes specified in WifiHelper::SetRemoteStationManager, 
   * WifiHelper::SetMac, and, WifiHelper::SetPhy.
   *
   * The user is expected to attach to the input channel a proper 
   * ns3::PropagationLossModel, and ns3::PropagationDelayModel.
   */
  NetDeviceContainer Build (NodeContainer c, Ptr<WifiChannel> channel) const;

private:
  ObjectFactory m_stationManager;
  ObjectFactory m_mac;
  ObjectFactory m_phy;
};

} // namespace ns3

#endif /* WIFI_HELPER_H */
