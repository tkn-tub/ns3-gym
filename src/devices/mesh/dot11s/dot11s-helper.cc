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
namespace ns3 {
namespace dot11s {

MeshWifiHelper::MeshWifiHelper () : 
    m_spreadInterfaceChannels (false)
{
}
void 
MeshWifiHelper::SetSpreadInterfaceChannels (bool s)
{
  m_spreadInterfaceChannels = s;
}
NetDeviceContainer
MeshWifiHelper::Install (const WifiPhyHelper &phyHelper, const MeshInterfaceHelper &interfaceHelper, NodeContainer c,  std::vector<uint32_t> roots, uint32_t nInterfaces) const
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
        uint32_t channel = i * 5;
        Ptr<WifiNetDevice> iface = interfaceHelper.CreateInterface (phyHelper,node, (m_spreadInterfaceChannels ? channel : 0));
        mp->AddInterface (iface);
      }
    // Install 802.11s protocols
    Ptr<PeerManagementProtocol> pmp = CreateObject<PeerManagementProtocol> ();
    pmp->SetMeshId("mesh");
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
MeshWifiHelper::Install (const WifiPhyHelper &phy, const MeshInterfaceHelper &interfaceHelper, Ptr<Node> node,  std::vector<uint32_t> roots, uint32_t nInterfaces) const
{
  return Install (phy, interfaceHelper, NodeContainer (node), roots, nInterfaces);
}
void
MeshWifiHelper::Report (const ns3::Ptr<ns3::NetDevice>& device, std::ostream& os)
{
  Ptr <MeshPointDevice> mp = device->GetObject<MeshPointDevice> ();
  NS_ASSERT (mp != 0);
  std::vector<Ptr<NetDevice> > ifaces = mp->GetInterfaces ();
  os << "<MeshPointDevice ReportTime=\"" << Simulator::Now().GetSeconds() << "s\" MpAddress=\"" << mp->GetAddress () << "\">\n";
  for (std::vector<Ptr<NetDevice> >::const_iterator i = ifaces.begin(); i != ifaces.end(); ++i)
  {
    Ptr<WifiNetDevice> device = (*i)->GetObject<WifiNetDevice> ();
    NS_ASSERT (device != 0);
    MeshInterfaceHelper::Report(device, os);
  }
  Ptr <HwmpProtocol> hwmp = mp->GetObject<HwmpProtocol> ();
  NS_ASSERT(hwmp != 0);
  hwmp->Report (os);

  Ptr <PeerManagementProtocol> pmp = mp->GetObject<PeerManagementProtocol> ();
  NS_ASSERT(pmp != 0);
  pmp->Report (os);
  os << "</MeshPointDevice>\n";
}
void
MeshWifiHelper::ResetStats (const ns3::Ptr<ns3::NetDevice>& device)
{
  Ptr <MeshPointDevice> mp = device->GetObject<MeshPointDevice> ();
  NS_ASSERT (mp != 0);
  std::vector<Ptr<NetDevice> > ifaces = mp->GetInterfaces ();
  for (std::vector<Ptr<NetDevice> >::const_iterator i = ifaces.begin(); i != ifaces.end(); ++i)
  {
    Ptr<WifiNetDevice> device = (*i)->GetObject<WifiNetDevice> ();
    NS_ASSERT (device != 0);
    MeshInterfaceHelper::ResetStats (device);
  }
  Ptr <HwmpProtocol> hwmp = mp->GetObject<HwmpProtocol> ();
  NS_ASSERT(hwmp != 0);
  hwmp->ResetStats ();

  Ptr <PeerManagementProtocol> pmp = mp->GetObject<PeerManagementProtocol> ();
  NS_ASSERT(pmp != 0);
  pmp->ResetStats ();
}
} // namespace dot11s
} //namespace ns3

