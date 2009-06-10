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
#include "ns3/dot11s-installator.h"
#include "ns3/simulator.h"
#include "ns3/mesh-point-device.h"
#include "ns3/wifi-net-device.h"
namespace ns3 {
using namespace dot11s ;
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
  uint16_t node_counter = 0;
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
    bool root = false;
    for (unsigned int j = 0; j < roots.size (); j ++)
      if(node_counter == roots[j])
        root = true;
    if(!Dot11sStackInstallator::InstallDot11sStack (mp, root))
    {
      NS_ASSERT(false);
    }
    devices.Add (mp);
    node_counter ++;
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
  Dot11sStackInstallator::Report (device, os);
  os << "</MeshPointDevice>\n";
}
void
MeshWifiHelper::ResetStats (const ns3::Ptr<ns3::NetDevice>& device)
{
  Dot11sStackInstallator::ResetStats (device);
}
} //namespace ns3

