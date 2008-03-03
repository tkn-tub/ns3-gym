#include "wifi-helper.h"
#include "wifi-net-device.h"
#include "wifi-mac.h"
#include "wifi-phy.h"
#include "wifi-remote-station-manager.h"
#include "wifi-channel.h"
#include "propagation-delay-model.h"
#include "propagation-loss-model.h"
#include "ns3/mobility-model.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("WifiHelper");

namespace ns3 {

WifiHelper::WifiHelper ()
{
  m_stationManager.SetTypeId ("ArfWifiManager");
  m_phy.SetTypeId ("WifiPhy");
  m_mac.SetTypeId ("AdhocWifiMac");
}

void 
WifiHelper::SetRemoteStationManager (std::string type,
				     std::string n0, Attribute v0,
				     std::string n1, Attribute v1,
				     std::string n2, Attribute v2,
				     std::string n3, Attribute v3,
				     std::string n4, Attribute v4,
				     std::string n5, Attribute v5,
				     std::string n6, Attribute v6,
				     std::string n7, Attribute v7)
{
  m_stationManager = ObjectFactory ();
  m_stationManager.SetTypeId (type);
  m_stationManager.Set (n0, v0);
  m_stationManager.Set (n1, v1);
  m_stationManager.Set (n2, v2);
  m_stationManager.Set (n3, v3);
  m_stationManager.Set (n4, v4);
  m_stationManager.Set (n5, v5);
  m_stationManager.Set (n6, v6);
  m_stationManager.Set (n7, v7);
}

void 
WifiHelper::SetMac (std::string type,
		    std::string n0, Attribute v0,
		    std::string n1, Attribute v1,
		    std::string n2, Attribute v2,
		    std::string n3, Attribute v3,
		    std::string n4, Attribute v4,
		    std::string n5, Attribute v5,
		    std::string n6, Attribute v6,
		    std::string n7, Attribute v7)
{
  m_mac = ObjectFactory ();
  m_mac.SetTypeId (type);
  m_mac.Set (n0, v0);
  m_mac.Set (n1, v1);
  m_mac.Set (n2, v2);
  m_mac.Set (n3, v3);
  m_mac.Set (n4, v4);
  m_mac.Set (n5, v5);
  m_mac.Set (n6, v6);
  m_mac.Set (n7, v7);
}

void 
WifiHelper::SetPhy (std::string type,
		    std::string n0, Attribute v0,
		    std::string n1, Attribute v1,
		    std::string n2, Attribute v2,
		    std::string n3, Attribute v3,
		    std::string n4, Attribute v4,
		    std::string n5, Attribute v5,
		    std::string n6, Attribute v6,
		    std::string n7, Attribute v7)
{
  m_phy = ObjectFactory ();
  m_phy.SetTypeId (type);
  m_phy.Set (n0, v0);
  m_phy.Set (n1, v1);
  m_phy.Set (n2, v2);
  m_phy.Set (n3, v3);
  m_phy.Set (n4, v4);
  m_phy.Set (n5, v5);
  m_phy.Set (n6, v6);
  m_phy.Set (n7, v7);
}

NetDeviceContainer
WifiHelper::Build (NodeContainer c) const
{
  Ptr<WifiChannel> channel = CreateObjectWith<WifiChannel> ();
  channel->SetPropagationDelayModel (CreateObjectWith<ConstantSpeedPropagationDelayModel> ());
  Ptr<LogDistancePropagationLossModel> log = CreateObjectWith<LogDistancePropagationLossModel> ();
  log->SetReferenceModel (CreateObjectWith<FriisPropagationLossModel> ());
  channel->SetPropagationLossModel (log);
  return Build (c, channel);
}
NetDeviceContainer
WifiHelper::Build (NodeContainer c, Ptr<WifiChannel> channel) const
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;
      Ptr<WifiNetDevice> device = CreateObject<WifiNetDevice> ();
      Ptr<WifiRemoteStationManager> manager = m_stationManager.Create<WifiRemoteStationManager> ();
      Ptr<WifiMac> mac = m_mac.Create<WifiMac> ();
      Ptr<WifiPhy> phy = m_phy.Create<WifiPhy> ();
      device->Setup (node, mac, phy, manager, channel);
      node->AddDevice (device);
      devices.Add (device);
      NS_LOG_DEBUG ("node="<<node<<", mob="<<node->GetObject<MobilityModel> ());
    }
  return devices;
}
} // namespace ns3
