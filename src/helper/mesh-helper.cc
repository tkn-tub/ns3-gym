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
#include "mesh-helper.h"
#include "ns3/simulator.h"
#include "ns3/mesh-point-device.h"
#include "ns3/wifi-net-device.h"
#include "ns3/log.h"
NS_LOG_COMPONENT_DEFINE ("MeshHelper");
namespace ns3 {
MeshHelper::MeshHelper () : 
    m_spreadInterfaceChannels (false),
    m_stack (0)
{
}
void 
MeshHelper::SetSpreadInterfaceChannels (bool s)
{
  m_spreadInterfaceChannels = s;
}
void 
MeshHelper::SetStackInstaller (std::string type)
{
  NS_LOG_FUNCTION (this << type);
  m_stackFactory = ObjectFactory ();
  m_stackFactory.SetTypeId (type);
  m_stack = m_stackFactory.Create<MeshStack> ();
  if (m_stack == 0)
    NS_FATAL_ERROR ("Stack has not been created: "<<type);
}

NetDeviceContainer
MeshHelper::Install (const WifiPhyHelper &phyHelper, const MeshInterfaceHelper &interfaceHelper, NodeContainer c, uint32_t nInterfaces) const
{
  NetDeviceContainer devices;
  NS_ASSERT (m_stack != 0);
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
    if(!m_stack->InstallStack (mp))
    {
      NS_FATAL_ERROR ("Stack is not installed!");
    }
    devices.Add (mp);
    node_counter ++;
  }
  return devices;
}

NetDeviceContainer
MeshHelper::Install (const WifiPhyHelper &phy, const MeshInterfaceHelper &interfaceHelper, Ptr<Node> node, uint32_t nInterfaces) const
{
  return Install (phy, interfaceHelper, NodeContainer (node), nInterfaces);
}
void
MeshHelper::Report (const ns3::Ptr<ns3::NetDevice>& device, std::ostream& os)
{
  NS_ASSERT (m_stack != 0);
  Ptr <MeshPointDevice> mp = device->GetObject<MeshPointDevice> ();
  NS_ASSERT (mp != 0);
  std::vector<Ptr<NetDevice> > ifaces = mp->GetInterfaces ();
  os << "<MeshPointDevice time=\"" << Simulator::Now().GetSeconds() << "\" address=\"" << Mac48Address::ConvertFrom(mp->GetAddress ()) << "\">\n";
  m_stack->Report (mp, os);
  os << "</MeshPointDevice>\n";
}
void
MeshHelper::ResetStats (const ns3::Ptr<ns3::NetDevice>& device)
{
  NS_ASSERT (m_stack != 0);
  Ptr <MeshPointDevice> mp = device->GetObject<MeshPointDevice> ();
  NS_ASSERT (mp != 0);
  m_stack->ResetStats (mp);
}
} //namespace ns3

