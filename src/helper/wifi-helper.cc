/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "wifi-helper.h"
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
#include "ns3/names.h"

NS_LOG_COMPONENT_DEFINE ("WifiHelper");

namespace ns3 {

static void PcapSnifferEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet)
{
  writer->WritePacket (packet);
}

void 
WifiHelper::EnablePcap (std::string filename, uint32_t nodeid, uint32_t deviceid)
{
  std::ostringstream oss;
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Mac/";
  Config::MatchContainer matches = Config::LookupMatches (oss.str ());
  if (matches.GetN () == 0)
    {
      return;
    }
  oss.str ("");
  oss << filename << "-" << nodeid << "-" << deviceid << ".pcap";
  Ptr<PcapWriter> pcap = Create<PcapWriter> ();
  pcap->Open (oss.str ());
  pcap->WriteEthernetHeader ();
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Mac/Sniffer";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&PcapSnifferEvent, pcap));
}

void 
WifiHelper::EnablePcap (std::string filename, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnablePcap (filename, dev->GetNode ()->GetId (), dev->GetIfIndex ());
    }
}
void
WifiHelper::EnablePcap (std::string filename, NodeContainer n)
{
  NetDeviceContainer devs;
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      for (uint32_t j = 0; j < node->GetNDevices (); ++j)
        {
          devs.Add (node->GetDevice (j));
        }
    }
  EnablePcap (filename, devs);
}

void
WifiHelper::EnablePcapAll (std::string filename)
{
  EnablePcap (filename, NodeContainer::GetGlobal ());
}

WifiPhyHelper::~WifiPhyHelper ()
{}


WifiHelper::WifiHelper ()
{}

WifiHelper
WifiHelper::Default (void)
{
  WifiHelper helper;
  helper.SetRemoteStationManager ("ns3::ArfWifiManager");
  helper.SetMac ("ns3::AdhocWifiMac");
  return helper;
}

void 
WifiHelper::SetRemoteStationManager (std::string type,
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
WifiHelper::SetMac (std::string type,
                    std::string n0, const AttributeValue &v0,
                    std::string n1, const AttributeValue &v1,
                    std::string n2, const AttributeValue &v2,
                    std::string n3, const AttributeValue &v3,
                    std::string n4, const AttributeValue &v4,
                    std::string n5, const AttributeValue &v5,
                    std::string n6, const AttributeValue &v6,
                    std::string n7, const AttributeValue &v7)
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

NetDeviceContainer 
WifiHelper::Install (const WifiPhyHelper &phyHelper, NodeContainer c) const
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<WifiNetDevice> device = CreateObject<WifiNetDevice> ();
      Ptr<WifiRemoteStationManager> manager = m_stationManager.Create<WifiRemoteStationManager> ();
      Ptr<WifiMac> mac = m_mac.Create<WifiMac> ();
      Ptr<WifiPhy> phy = phyHelper.Create (node, device);
      mac->SetAddress (Mac48Address::Allocate ());
      device->SetMac (mac);
      device->SetPhy (phy);
      device->SetRemoteStationManager (manager);
      node->AddDevice (device);
      devices.Add (device);
      NS_LOG_DEBUG ("node="<<node<<", mob="<<node->GetObject<MobilityModel> ());
    }
  return devices;
}
NetDeviceContainer 
WifiHelper::Install (const WifiPhyHelper &phy, Ptr<Node> node) const
{
  return Install (phy, NodeContainer (node));
}
NetDeviceContainer 
WifiHelper::Install (const WifiPhyHelper &phy, std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return Install (phy, NodeContainer (node));
}

} // namespace ns3
