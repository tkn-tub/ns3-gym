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
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/mobility-model.h"
#include "ns3/log.h"
#include "ns3/pcap-writer.h"
#include "ns3/wifi-mode.h"
#include "ns3/wifi-preamble.h"
#include "ns3/config.h"
#include "ns3/simulator.h"



NS_LOG_COMPONENT_DEFINE ("WifiHelper");

namespace ns3 {

static void PcapPhyTxEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet,
			    WifiMode mode, WifiPreamble preamble, 
			    uint8_t txLevel)
{
  writer->WritePacket (packet);
}

static void PcapPhyRxEvent (Ptr<PcapWriter> writer, 
			    Ptr<const Packet> packet, double snr, WifiMode mode, 
			    enum WifiPreamble preamble)
{
  writer->WritePacket (packet);
}

static void AsciiPhyTxEvent (std::ostream *os, std::string context, 
			     Ptr<const Packet> packet,
			     WifiMode mode, WifiPreamble preamble, 
			     uint8_t txLevel)
{
  *os << "+ " << Simulator::Now () << " " << context << " " << *packet << std::endl;
}

static void AsciiPhyRxOkEvent (std::ostream *os, std::string context, 
			       Ptr<const Packet> packet, double snr, WifiMode mode, 
			       enum WifiPreamble preamble)
{
  *os << "r " << Simulator::Now () << " " << context << " " << *packet << std::endl;
}


WifiHelper::WifiHelper ()
{
  m_stationManager.SetTypeId ("ns3::ArfWifiManager");
  m_phy.SetTypeId ("ns3::WifiPhy");
  m_mac.SetTypeId ("ns3::AdhocWifiMac");
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

void 
WifiHelper::EnablePcap (std::string filename, uint32_t nodeid, uint32_t deviceid)
{
  std::ostringstream oss;
  oss << filename << "-" << nodeid << "-" << deviceid << ".pcap";
  Ptr<PcapWriter> pcap = Create<PcapWriter> ();
  pcap->Open (oss.str ());
  pcap->WriteWifiHeader ();
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Phy/Tx";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&PcapPhyTxEvent, pcap));
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Phy/RxOk";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&PcapPhyRxEvent, pcap));
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

void 
WifiHelper::EnableAscii (std::ostream &os, uint32_t nodeid, uint32_t deviceid)
{
    Packet::EnableMetadata ();
  std::ostringstream oss;
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Phy/RxOk";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiPhyRxOkEvent, &os));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Phy/Tx";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiPhyTxEvent, &os));
}
void 
WifiHelper::EnableAscii (std::ostream &os, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnableAscii (os, dev->GetNode ()->GetId (), dev->GetIfIndex ());
    }
}
void
WifiHelper::EnableAscii (std::ostream &os, NodeContainer n)
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
  EnableAscii (os, devs);
}

void
WifiHelper::EnableAsciiAll (std::ostream &os)
{
  EnableAscii (os, NodeContainer::GetGlobal ());
}

NetDeviceContainer
WifiHelper::Install (NodeContainer c) const
{
  Ptr<WifiChannel> channel = CreateObject<WifiChannel> ();
  channel->SetPropagationDelayModel (CreateObject<ConstantSpeedPropagationDelayModel> ());
  Ptr<LogDistancePropagationLossModel> log = CreateObject<LogDistancePropagationLossModel> ();
  log->SetReferenceModel (CreateObject<FriisPropagationLossModel> ());
  channel->SetPropagationLossModel (log);
  return Install (c, channel);
}
NetDeviceContainer
WifiHelper::Install (NodeContainer c, Ptr<WifiChannel> channel) const
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;
      Ptr<WifiNetDevice> device = CreateObject<WifiNetDevice> ();
      Ptr<WifiRemoteStationManager> manager = m_stationManager.Create<WifiRemoteStationManager> ();
      Ptr<WifiMac> mac = m_mac.Create<WifiMac> ();
      Ptr<WifiPhy> phy = m_phy.Create<WifiPhy> ();
      mac->SetAddress (Mac48Address::Allocate ());
      device->SetMac (mac);
      device->SetPhy (phy);
      device->SetRemoteStationManager (manager);
      device->SetChannel (channel);
      node->AddDevice (device);
      devices.Add (device);
      NS_LOG_DEBUG ("node="<<node<<", mob="<<node->GetObject<MobilityModel> ());
    }
  return devices;
}

} // namespace ns3
