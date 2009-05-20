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
 *         Pavel Boyko <boyko@iitp.ru>
 */


#include "dot11s-helper.h"
#include "ns3/simulator.h"
#include "ns3/mesh-point-device.h"
#include "ns3/wifi-net-device.h"
#include "ns3/wifi-phy.h"
#include "ns3/wifi-channel.h"
#include "ns3/wifi-remote-station-manager.h"
#include "ns3/mesh-wifi-interface-mac.h"
#include "ns3/aarf-wifi-manager.h"
#include "airtime-metric.h"
#include "ns3/log.h"
namespace ns3 {
namespace dot11s {

MeshWifiHelper::MeshWifiHelper () : 
    m_ssid("mesh"), 
    m_randomStartDelay (Seconds (0)),
    m_spreadInterfaceChannels (false)
{
}
  
void
MeshWifiHelper::SetSsid (Ssid const & s)
{
  m_ssid = s;
}

Ssid MeshWifiHelper::GetSsid () const
{
  return m_ssid;
}

void 
MeshWifiHelper::SetRandomStartDelay (Time t)
{
  m_randomStartDelay = t;
}

void 
MeshWifiHelper::SetSpreadInterfaceChannels (bool s)
{
  m_spreadInterfaceChannels = s;
}


Ptr<WifiNetDevice> 
MeshWifiHelper::CreateInterface (const WifiPhyHelper &phyHelper, Ptr<Node> node) const
{
  Ptr<WifiNetDevice> device = CreateObject<WifiNetDevice> ();
  
  // Creating MAC for this interface
  Ptr<MeshWifiInterfaceMac> mac = CreateObject<MeshWifiInterfaceMac> ();
  mac->SetSsid (m_ssid);
  if (m_randomStartDelay > Seconds (0))
    mac->SetRandomStartDelay (m_randomStartDelay);
  Ptr<WifiRemoteStationManager> manager = CreateObject<AarfWifiManager> ();
  Ptr<WifiPhy> phy = phyHelper.Create (node, device);
  mac->SetAddress (Mac48Address::Allocate ());
  device->SetMac (mac);
  device->SetPhy (phy);
  device->SetRemoteStationManager (manager);
  Ptr<AirtimeLinkMetricCalculator> metric = Create <AirtimeLinkMetricCalculator> ();
  mac->SetLinkMetricCallback (MakeCallback(&AirtimeLinkMetricCalculator::CalculateMetric, metric));
  /*
  if (channel > 0)
    mac->SwitchFrequencyChannel (channel);
  */
  return device;
}
  
NetDeviceContainer
MeshWifiHelper::Install (const WifiPhyHelper &phyHelper, NodeContainer c,  std::vector<uint32_t> roots, uint32_t nInterfaces) const
{
  NetDeviceContainer devices;
  uint32_t node_index = 0;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
  {
    Ptr<Node> node = *i;
    
    // Create a mesh point device
    Ptr<MeshPointDevice> mp = CreateObject<MeshPointDevice> ();
    node->AddDevice (mp);
    
    // Create wifi interfaces (single interface by default)
    for (uint32_t i = 0; i < nInterfaces; ++i)
      {
        Ptr<WifiNetDevice> iface = CreateInterface (phyHelper, node);
        
        node->AddDevice (iface);
        mp->AddInterface (iface);
      }
    
    // Set different channels on different ifaces
    if (m_spreadInterfaceChannels)
      {
        std::vector<Ptr<NetDevice> > ifaces = mp->GetInterfaces ();
        for (size_t i = 0; i < ifaces.size(); ++i)
          {
            uint32_t channel = i * 5;
            
            Ptr<WifiNetDevice> iface = ifaces[i]->GetObject<WifiNetDevice> ();
            NS_ASSERT (iface);
            Ptr<MeshWifiInterfaceMac> mac = iface->GetMac ()->GetObject<MeshWifiInterfaceMac> ();
            NS_ASSERT (mac);
            
            mac->SwitchFrequencyChannel (channel);
          }
      }
    
    // Install 802.11s protocols
    Ptr<PeerManagementProtocol> pmp = CreateObject<PeerManagementProtocol> ();
    bool install_ok = pmp->Install (mp);
    NS_ASSERT (install_ok);
    
    Ptr<HwmpProtocol> hwmp = CreateObject<HwmpProtocol> ();
    install_ok = hwmp->Install (mp);
    NS_ASSERT (install_ok);
    
    pmp->SetPeerLinkStatusCallback(MakeCallback(&HwmpProtocol::PeerLinkStatus, hwmp));
    hwmp->SetNeighboursCallback(MakeCallback(&PeerManagementProtocol::GetActiveLinks, pmp));
    // Setting root mesh point
    for(std::vector<uint32_t>::const_iterator root_iterator = roots.begin (); root_iterator != roots.end (); root_iterator ++)
      //if(*root_iterator == i.GetDistanceFrom(c.Begin ()))
      if(*root_iterator == node_index)
        hwmp->SetRoot ();
    devices.Add (mp);
    node_index ++;
  }
  return devices;
}

NetDeviceContainer
MeshWifiHelper::Install (const WifiPhyHelper &phy, Ptr<Node> node,  std::vector<uint32_t> roots, uint32_t nInterfaces) const
{
  return Install (phy, NodeContainer (node), roots, nInterfaces);
}
void
MeshWifiHelper::Report (const ns3::Ptr<ns3::NetDevice>& device, std::ostream& os)
{
  NS_LOG_UNCOND("Report must be here:");
  Ptr <MeshPointDevice> mp = device->GetObject<MeshPointDevice> ();
  NS_ASSERT (mp != 0);
  std::vector<Ptr<NetDevice> > ifaces = mp->GetInterfaces ();
  os << "<MeshPointDevice ReportTime=\"" << Simulator::Now().GetSeconds() << "s\">\n";
  for (std::vector<Ptr<NetDevice> >::const_iterator i = ifaces.begin(); i != ifaces.end(); ++i)
  {
    Ptr<WifiNetDevice> device = (*i)->GetObject<WifiNetDevice> ();
    NS_ASSERT (device != 0);
    Ptr<MeshWifiInterfaceMac> mac = device->GetMac()->GetObject<MeshWifiInterfaceMac> ();
    NS_ASSERT (mac != 0);
    os << "<Interface "
      "Index=\"" << device->GetIfIndex () << "\" "
      "BeaconInterval=\"" << mac->GetBeaconInterval ().GetSeconds() << "s\" "
      "Channel=\"" << mac->GetFrequencyChannel () << "\" "
      "/>\n";
  }
  os << "</MeshPointDevice>\n";
  Ptr <HwmpProtocol> hwmp = mp->GetObject<HwmpProtocol> ();
  NS_ASSERT(hwmp != 0);
  hwmp->Report (os);

  Ptr <PeerManagementProtocol> pmp = mp->GetObject<PeerManagementProtocol> ();
  NS_ASSERT(pmp != 0);
  pmp->Report (os);
}

} // namespace dot11s
} //namespace ns3

