/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#include "mesh-interface-helper.h"
#include "ns3/wifi-mac.h"
#include "ns3/pointer.h"
#include "ns3/dca-txop.h"
#include "ns3/wifi-remote-station-manager.h"
namespace ns3
{

MeshInterfaceHelper::MeshInterfaceHelper ()
{
  m_queues.insert (std::make_pair (AC_VO, ObjectFactory ()));
  m_queues.insert (std::make_pair (AC_VI, ObjectFactory ()));
  m_queues.insert (std::make_pair (AC_BE, ObjectFactory ()));
  m_queues.insert (std::make_pair (AC_BK, ObjectFactory ()));

  m_queues[AC_VO].SetTypeId ("ns3::DcaTxop");
  m_queues[AC_VI].SetTypeId ("ns3::DcaTxop");
  m_queues[AC_BE].SetTypeId ("ns3::DcaTxop");
  m_queues[AC_BK].SetTypeId ("ns3::DcaTxop");
}

MeshInterfaceHelper::~MeshInterfaceHelper ()
{}

MeshInterfaceHelper
MeshInterfaceHelper::Default (void)
{
  MeshInterfaceHelper helper;
  helper.SetType ();
  helper.SetRemoteStationManager ("ns3::ArfWifiManager");
    /* For more details about this default parameters see IEE802.11 section 7.3.2.29 */
  helper.SetQueueParameters (AC_VO, "MinCw", UintegerValue (3), "MaxCw", UintegerValue (7), "Aifsn",
      UintegerValue (2));
  helper.SetQueueParameters (AC_VI, "MinCw", UintegerValue (7), "MaxCw", UintegerValue (15), "Aifsn",
      UintegerValue (2));
  helper.SetQueueParameters (AC_BE, "MinCw", UintegerValue (15), "MaxCw", UintegerValue (1023), "Aifsn",
      UintegerValue (3));
  helper.SetQueueParameters (AC_BK, "MinCw", UintegerValue (15), "MaxCw", UintegerValue (1023), "Aifsn",
      UintegerValue (7));
  return helper;
}

void
MeshInterfaceHelper::SetType (std::string n0, const AttributeValue &v0,
                              std::string n1, const AttributeValue &v1,
                              std::string n2, const AttributeValue &v2,
                              std::string n3, const AttributeValue &v3,
                              std::string n4, const AttributeValue &v4,
                              std::string n5, const AttributeValue &v5,
                              std::string n6, const AttributeValue &v6,
                              std::string n7, const AttributeValue &v7)
{
  m_mac.SetTypeId ("ns3::MeshWifiInterfaceMac");
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
MeshInterfaceHelper::SetQueueParameters ( AccessClass ac,
    std::string n0, const AttributeValue &v0,
    std::string n1, const AttributeValue &v1,
    std::string n2, const AttributeValue &v2,
    std::string n3, const AttributeValue &v3)
{
  std::map<AccessClass, ObjectFactory>::iterator queue = m_queues.find (ac);
  if (queue != m_queues.end ())
    {
      queue->second.Set (n0, v0);
      queue->second.Set (n1, v1);
      queue->second.Set (n2, v2);
      queue->second.Set (n3, v3);
    }
  else
  {
    NS_FATAL_ERROR ("Queue is not set!");
  }
}
void
MeshInterfaceHelper::SetRemoteStationManager (std::string type,
                                     std::string n0, const AttributeValue &v0,
                                     std::string n1, const AttributeValue &v1,
                                     std::string n2, const AttributeValue &v2,
                                     std::string n3, const AttributeValue &v3,
                                     std::string n4, const AttributeValue &v4,
                                     std::string n5, const AttributeValue &v5,
                                     std::string n6, const AttributeValue &v6,
                                     std::string n7, const AttributeValue &v7)
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
Ptr<WifiNetDevice>
MeshInterfaceHelper::CreateInterface (const WifiPhyHelper &phyHelper, Ptr<Node> node, uint16_t channelId) const
{
  Ptr<WifiNetDevice> device = CreateObject<WifiNetDevice> ();

  Ptr<MeshWifiInterfaceMac> mac = DynamicCast<MeshWifiInterfaceMac> (Create ());
  NS_ASSERT (mac != 0);
  mac->SetSsid (Ssid ());
  Ptr<WifiRemoteStationManager> manager = m_stationManager.Create<WifiRemoteStationManager> ();
  Ptr<WifiPhy> phy = phyHelper.Create (node, device);
  mac->SetAddress (Mac48Address::Allocate ());
  device->SetMac (mac);
  device->SetPhy (phy);
  device->SetRemoteStationManager (manager);
  node->AddDevice (device);
  mac->SwitchFrequencyChannel (channelId);
  return device;
}
Ptr<WifiMac>
MeshInterfaceHelper::Create (void) const
{
  Ptr<MeshWifiInterfaceMac> mac = m_mac.Create<MeshWifiInterfaceMac> ();
  for (std::map<AccessClass, ObjectFactory>::const_iterator i = m_queues.begin (); i != m_queues.end (); i++)
    {
      mac->SetQueue (i->second.Create<DcaTxop> (), i->first);
    }
  return mac;
}
void
MeshInterfaceHelper::Report (const Ptr<WifiNetDevice>& device, std::ostream& os)
{
  Ptr<MeshWifiInterfaceMac> mac = device->GetMac ()->GetObject<MeshWifiInterfaceMac> ();
  NS_ASSERT (mac != 0);
  mac->Report (os);
}
void
MeshInterfaceHelper::ResetStats (const Ptr<WifiNetDevice>& device)
{
  Ptr<MeshWifiInterfaceMac> mac = device->GetMac ()->GetObject<MeshWifiInterfaceMac> ();
  NS_ASSERT (mac != 0);
  mac->ResetStats ();
}

} //namespace ns3

