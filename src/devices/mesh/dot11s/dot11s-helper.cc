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

#include "ns3/mesh-point-device.h"
#include "ns3/wifi-net-device.h"
#include "ns3/wifi-phy.h"
#include "ns3/wifi-channel.h"
#include "ns3/wifi-remote-station-manager.h"
#include "ns3/mesh-wifi-interface-mac.h"
#include "ns3/aarf-wifi-manager.h"

namespace ns3 {
namespace dot11s {

MeshWifiHelper::MeshWifiHelper () : m_ssid("mesh"), m_randomStartDelay (Seconds (0))
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
  
  return device;
}
  
NetDeviceContainer
MeshWifiHelper::Install (const WifiPhyHelper &phyHelper, NodeContainer c, uint32_t nInterfaces) const
{
  NetDeviceContainer devices;
  
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
    
    // Install 802.11s protocols
    Ptr<PeerManagementProtocol> pmp = CreateObject<PeerManagementProtocol> ();
    bool install_ok = pmp->Install (mp);
    NS_ASSERT (install_ok);
    
    Ptr<HwmpProtocol> hwmp = CreateObject<HwmpProtocol> ();
    install_ok = hwmp->Install (mp);
    NS_ASSERT (install_ok);
    
    pmp->SetPeerLinkStatusCallback(MakeCallback(&HwmpProtocol::PeerLinkStatus, hwmp));
    hwmp->SetNeighboursCallback(MakeCallback(&PeerManagementProtocol::GetActiveLinks, pmp));
    
    devices.Add (mp);
  }
  return devices;
}

NetDeviceContainer
MeshWifiHelper::Install (const WifiPhyHelper &phy, Ptr<Node> node, uint32_t nInterfaces) const
{
  return Install (phy, NodeContainer (node), nInterfaces);
}
  
} // namespace dot11s
} //namespace ns3

