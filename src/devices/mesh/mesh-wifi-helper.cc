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


#include "ns3/mesh-wifi-helper.h"
#include "ns3/wifi-net-device.h"
#include "ns3/wifi-mac.h"
#include "ns3/wifi-phy.h"
#include "ns3/wifi-remote-station-manager.h"
#include "ns3/wifi-channel.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/mobility-model.h"
#include "ns3/log.h"
#include "ns3/pcap-writer.h"
#include "ns3/config.h"
#include "ns3/simulator.h"
#include "ns3/hwmp.h"

NS_LOG_COMPONENT_DEFINE ("MeshWifiHelper");
namespace ns3 {

MeshWifiHelper::MeshWifiHelper ()
{
  //m_deviceFactory.SetTypeId ("ns3::L2RoutingNetDevice");
}

MeshWifiHelper::~MeshWifiHelper ()
{
}

void
MeshWifiHelper::SetRemoteStationManager (std::string type,
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

void
MeshWifiHelper::SetMac (std::string type,
                        std::string n0, const AttributeValue &v0,
                        std::string n1, const AttributeValue &v1,
                        std::string n2, const AttributeValue &v2,
                        std::string n3, const AttributeValue &v3,
                        std::string n4, const AttributeValue &v4,
                        std::string n5, const AttributeValue &v5,
                        std::string n6, const AttributeValue &v6,
                        std::string n7, const AttributeValue &v7)
{
  m_meshMac = ObjectFactory ();
  m_meshMac.SetTypeId (type);
  m_meshMac.Set (n0, v0);
  m_meshMac.Set (n1, v1);
  m_meshMac.Set (n2, v2);
  m_meshMac.Set (n3, v3);
  m_meshMac.Set (n4, v4);
  m_meshMac.Set (n5, v5);
  m_meshMac.Set (n6, v6);
  m_meshMac.Set (n7, v7);
}

void
MeshWifiHelper::SetL2RoutingProtocol (std::string type,
                                     std::string n0, const AttributeValue &v0,
                                     std::string n1, const AttributeValue &v1,
                                     std::string n2, const AttributeValue &v2,
                                     std::string n3, const AttributeValue &v3,
                                     std::string n4, const AttributeValue &v4,
                                     std::string n5, const AttributeValue &v5,
                                     std::string n6, const AttributeValue &v6,
                                     std::string n7, const AttributeValue &v7,
                                     std::string n8, const AttributeValue &v8,
                                     std::string n9, const AttributeValue &v9)
{
  m_routingProtocol = ObjectFactory ();
  m_routingProtocol.SetTypeId (type);
  m_routingProtocol.Set (n0, v0);
  m_routingProtocol.Set (n1, v1);
  m_routingProtocol.Set (n2, v2);
  m_routingProtocol.Set (n3, v3);
  m_routingProtocol.Set (n4, v4);
  m_routingProtocol.Set (n5, v5);
  m_routingProtocol.Set (n6, v6);
  m_routingProtocol.Set (n7, v7);
}

void
MeshWifiHelper::SetL2RoutingNetDevice (std::string type,
                                      std::string n0, const AttributeValue &v0,
                                      std::string n1, const AttributeValue &v1,
                                      std::string n2, const AttributeValue &v2,
                                      std::string n3, const AttributeValue &v3,
                                      std::string n4, const AttributeValue &v4,
                                      std::string n5, const AttributeValue &v5,
                                      std::string n6, const AttributeValue &v6,
                                      std::string n7, const AttributeValue &v7,
                                      std::string n8, const AttributeValue &v8,
                                      std::string n9, const AttributeValue &v9)
{
  m_deviceFactory = ObjectFactory ();
  m_deviceFactory.SetTypeId (type);
  m_deviceFactory.Set (n0, v0);
  m_deviceFactory.Set (n1, v1);
  m_deviceFactory.Set (n2, v2);
  m_deviceFactory.Set (n3, v3);
  m_deviceFactory.Set (n4, v4);
  m_deviceFactory.Set (n5, v5);
  m_deviceFactory.Set (n6, v6);
  m_deviceFactory.Set (n7, v7);
}

void
MeshWifiHelper::SetPeerLinkManager (std::string type,
                                    std::string n0, const AttributeValue &v0,
                                    std::string n1, const AttributeValue &v1,
                                    std::string n2, const AttributeValue &v2,
                                    std::string n3, const AttributeValue &v3,
                                    std::string n4, const AttributeValue &v4,
                                    std::string n5, const AttributeValue &v5,
                                    std::string n6, const AttributeValue &v6,
                                    std::string n7, const AttributeValue &v7,
                                    std::string n8, const AttributeValue &v8,
                                    std::string n9, const AttributeValue &v9)
{
  m_peerManager = ObjectFactory ();
  m_peerManager.SetTypeId (type);
  m_peerManager.Set (n0, v0);
  m_peerManager.Set (n1, v1);
  m_peerManager.Set (n2, v2);
  m_peerManager.Set (n3, v3);
  m_peerManager.Set (n4, v4);
  m_peerManager.Set (n5, v5);
  m_peerManager.Set (n6, v6);
  m_peerManager.Set (n7, v7);
  m_peerManager.Set (n8, v8);
  m_peerManager.Set (n9, v9);
}

NetDeviceContainer
MeshWifiHelper::Install (const WifiPhyHelper &phyHelper, NodeContainer c, uint8_t numOfPorts) const
{
    NetDeviceContainer devices;
    for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
      {
        Ptr<Node> node = *i;
        Ptr<MeshPointDevice> mp = m_deviceFactory.Create<MeshPointDevice> ();
        Ptr<WifiPeerManager> pPeer = m_peerManager.Create<WifiPeerManager > ();
        devices.Add (mp);
        std::vector<Ptr<WifiNetDevice> > nodeDevices;
        for (uint8_t k = 0; k < numOfPorts; k++)
          {
            Ptr<WifiNetDevice> device = CreateObject<WifiNetDevice> ();
            Ptr<MeshWifiMac> mac = m_meshMac.Create<MeshWifiMac> ();
            Ptr<WifiRemoteStationManager> manager = m_stationManager.Create<WifiRemoteStationManager> ();
            Ptr<WifiPhy> phy = phyHelper.Create (node, device);
            mac->SetAddress (Mac48Address::Allocate ());
            device->SetMac (mac);
            device->SetPhy (phy);
            device->SetRemoteStationManager (manager);
            //create L2RoutingNetDevice and add WifiNetDevice to it
            node->AddDevice (device);
            nodeDevices.push_back (device);
          }
        node -> AddDevice (mp);
        for (std::vector<Ptr<WifiNetDevice> > ::iterator iter=nodeDevices.begin ();iter != nodeDevices.end(); ++iter)
          mp->AddInterface (*iter);
        // nodeDevice.pop_back ()
        pPeer->AttachPorts (nodeDevices);
        
        // Install routing protocol
        Ptr<MeshL2RoutingProtocol> routing = m_routingProtocol.Create <MeshL2RoutingProtocol> ();
        routing->SetMeshPoint (mp);
        mp->SetRoutingProtocol (routing);
        
        //hwmp->SetRoot (device->GetIfIndex(), Seconds(5));
        nodeDevices.clear ();
      }
    return devices;
}

NetDeviceContainer
MeshWifiHelper::Install (const WifiPhyHelper &phy, Ptr<Node> node, uint8_t numOfPorts) const
{
  return Install (phy, NodeContainer (node), numOfPorts);
}

} //namespace ns3
