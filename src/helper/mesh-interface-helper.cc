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
#include "ns3/log.h"
namespace ns3
{

MeshInterfaceHelper::MeshInterfaceHelper ()
{
  struct QUEUE queue;
  queue.dcaTxop = ObjectFactory (); 
  queue.dcaTxop.SetTypeId ("ns3::DcaTxop");
  queue.ac = AC_VO;
  m_queues.push_back (queue);
  queue.ac = AC_VI;
  m_queues.push_back (queue);
  queue.ac = AC_BK;
  m_queues.push_back (queue);
  queue.ac = AC_BE;
  m_queues.push_back (queue);
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
  std::vector<QUEUE>::iterator queue = m_queues.end ();
  for (std::vector<QUEUE>::iterator i = m_queues.begin (); i != m_queues.end (); i ++)
    {
      if (i->ac == ac)
        {
          queue = i;
          break;
        }
    }
  if (queue != m_queues.end ())
    {
      queue->dcaTxop.Set (n0, v0);
      queue->dcaTxop.Set (n1, v1);
      queue->dcaTxop.Set (n2, v2);
      queue->dcaTxop.Set (n3, v3);
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
  for (std::vector<QUEUE>::const_iterator i = m_queues.begin (); i != m_queues.end (); i ++)
    {
      mac->SetQueue (i->dcaTxop.Create<DcaTxop> (), i->ac);
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

