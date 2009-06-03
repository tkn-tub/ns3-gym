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
#include "yans-wifi-helper.h"
#include "ns3/error-rate-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-phy.h"
#include "ns3/wifi-net-device.h"
#include "ns3/pcap-writer.h"
#include "ns3/simulator.h"
#include "ns3/config.h"
#include "ns3/names.h"

namespace ns3 {

static void PcapSniffTxEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet, uint16_t channelFreqMhz,  uint32_t rate, bool isShortPreamble)
{
  const double unusedValue = 0;
  writer->WriteWifiMonitorPacket(packet, channelFreqMhz, rate, isShortPreamble, true, unusedValue, unusedValue); 
}


static void PcapSniffRxEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet, uint16_t channelFreqMhz,  uint32_t rate, bool isShortPreamble, double signalDbm, double noiseDbm)
{
  writer->WriteWifiMonitorPacket(packet, channelFreqMhz, rate, isShortPreamble, false, signalDbm, noiseDbm); 
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


YansWifiChannelHelper::YansWifiChannelHelper ()
{}

YansWifiChannelHelper 
YansWifiChannelHelper::Default (void)
{
  YansWifiChannelHelper helper;
  helper.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  helper.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");
  return helper;
}

void 
YansWifiChannelHelper::AddPropagationLoss (std::string type,
					   std::string n0, const AttributeValue &v0,
					   std::string n1, const AttributeValue &v1,
					   std::string n2, const AttributeValue &v2,
					   std::string n3, const AttributeValue &v3,
					   std::string n4, const AttributeValue &v4,
					   std::string n5, const AttributeValue &v5,
					   std::string n6, const AttributeValue &v6,
					   std::string n7, const AttributeValue &v7)
{
  ObjectFactory factory;
  factory.SetTypeId (type);
  factory.Set (n0, v0);
  factory.Set (n1, v1);
  factory.Set (n2, v2);
  factory.Set (n3, v3);
  factory.Set (n4, v4);
  factory.Set (n5, v5);
  factory.Set (n6, v6);
  factory.Set (n7, v7);
  m_propagationLoss.push_back (factory);
}

void 
YansWifiChannelHelper::SetPropagationDelay (std::string type,
					    std::string n0, const AttributeValue &v0,
					    std::string n1, const AttributeValue &v1,
					    std::string n2, const AttributeValue &v2,
					    std::string n3, const AttributeValue &v3,
					    std::string n4, const AttributeValue &v4,
					    std::string n5, const AttributeValue &v5,
					    std::string n6, const AttributeValue &v6,
					    std::string n7, const AttributeValue &v7)
{
  ObjectFactory factory;
  factory.SetTypeId (type);
  factory.Set (n0, v0);
  factory.Set (n1, v1);
  factory.Set (n2, v2);
  factory.Set (n3, v3);
  factory.Set (n4, v4);
  factory.Set (n5, v5);
  factory.Set (n6, v6);
  factory.Set (n7, v7);
  m_propagationDelay = factory;
}

Ptr<YansWifiChannel> 
YansWifiChannelHelper::Create (void) const
{
  Ptr<YansWifiChannel> channel = CreateObject<YansWifiChannel> ();
  Ptr<PropagationLossModel> prev = 0;
  for (std::vector<ObjectFactory>::const_iterator i = m_propagationLoss.begin (); i != m_propagationLoss.end (); ++i)
    {
      Ptr<PropagationLossModel> cur = (*i).Create<PropagationLossModel> ();
      if (prev != 0)
	{
	  prev->SetNext (cur);
	}
      if (m_propagationLoss.begin () == i)
	{
	  channel->SetPropagationLossModel (cur);
	}
      prev = cur;
    }
  Ptr<PropagationDelayModel> delay = m_propagationDelay.Create<PropagationDelayModel> ();
  channel->SetPropagationDelayModel (delay);
  return channel;
}


YansWifiPhyHelper::YansWifiPhyHelper ()
  : m_channel (0),
    m_pcapFormat(PCAP_FORMAT_80211)
    
{
  m_phy.SetTypeId ("ns3::YansWifiPhy");
}

YansWifiPhyHelper 
YansWifiPhyHelper::Default (void)
{
  YansWifiPhyHelper helper;
  helper.SetErrorRateModel ("ns3::YansErrorRateModel");
  return helper;
}

void 
YansWifiPhyHelper::SetChannel (Ptr<YansWifiChannel> channel)
{
  m_channel = channel;
}
void 
YansWifiPhyHelper::SetChannel (std::string channelName)
{
  Ptr<YansWifiChannel> channel = Names::Find<YansWifiChannel> (channelName);
  m_channel = channel;
}
void 
YansWifiPhyHelper::Set (std::string name, const AttributeValue &v)
{
  m_phy.Set (name, v);
}

void 
YansWifiPhyHelper::SetErrorRateModel (std::string name,
                                     std::string n0, const AttributeValue &v0,
                                     std::string n1, const AttributeValue &v1,
                                     std::string n2, const AttributeValue &v2,
                                     std::string n3, const AttributeValue &v3,
                                     std::string n4, const AttributeValue &v4,
                                     std::string n5, const AttributeValue &v5,
                                     std::string n6, const AttributeValue &v6,
                                     std::string n7, const AttributeValue &v7)
{
  m_errorRateModel = ObjectFactory ();
  m_errorRateModel.SetTypeId (name);
  m_errorRateModel.Set (n0, v0);
  m_errorRateModel.Set (n1, v1);
  m_errorRateModel.Set (n2, v2);
  m_errorRateModel.Set (n3, v3);
  m_errorRateModel.Set (n4, v4);
  m_errorRateModel.Set (n5, v5);
  m_errorRateModel.Set (n6, v6);
  m_errorRateModel.Set (n7, v7);
}

Ptr<WifiPhy> 
YansWifiPhyHelper::Create (Ptr<Node> node, Ptr<WifiNetDevice> device) const
{
  Ptr<YansWifiPhy> phy = m_phy.Create<YansWifiPhy> ();
  Ptr<ErrorRateModel> error = m_errorRateModel.Create<ErrorRateModel> ();
  phy->SetErrorRateModel (error);
  phy->SetChannel (m_channel);
  phy->SetMobility (node);
  phy->SetDevice (device);
  return phy;
}


void 
YansWifiPhyHelper::SetPcapFormat (enum PcapFormat format)
{
  m_pcapFormat = format;
}


void 
YansWifiPhyHelper::EnablePcap (std::string filename, uint32_t nodeid, uint32_t deviceid)
{
  std::ostringstream oss;
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Phy/";
  Config::MatchContainer matches = Config::LookupMatches (oss.str ());
  if (matches.GetN () == 0)
    {
      return;
    }
  oss.str ("");
  oss << filename << "-" << nodeid << "-" << deviceid << ".pcap";
  // we must fully-qualify the call to Create below because it conflicts
  // with the locally-defined WifiPhyHelper::Create method.
  Ptr<PcapWriter> pcap = ::ns3::Create<PcapWriter> ();
  pcap->Open (oss.str ());

  switch (m_pcapFormat) {
  case PCAP_FORMAT_80211:
    pcap->WriteWifiHeader ();  
    break;
  case PCAP_FORMAT_80211_RADIOTAP:
    pcap->WriteWifiRadiotapHeader ();  
    break;
  case PCAP_FORMAT_80211_PRISM:
    pcap->WriteWifiPrismHeader ();  
    break;
  }
  
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid;
  oss << "/$ns3::WifiNetDevice/Phy/PromiscSnifferTx";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&PcapSniffTxEvent, pcap));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid;
  oss << "/$ns3::WifiNetDevice/Phy/PromiscSnifferRx";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&PcapSniffRxEvent, pcap));  
}

void 
YansWifiPhyHelper::EnablePcap (std::string filename, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnablePcap (filename, dev->GetNode ()->GetId (), dev->GetIfIndex ());
    }
}

void 
YansWifiPhyHelper::EnablePcap (std::string filename, Ptr<NetDevice> nd)
{
  EnablePcap (filename, nd->GetNode ()->GetId (), nd->GetIfIndex ());
}

void 
YansWifiPhyHelper::EnablePcap (std::string filename, std::string ndName)
{
  Ptr<NetDevice> nd = Names::Find<NetDevice> (ndName);
  EnablePcap (filename, nd->GetNode ()->GetId (), nd->GetIfIndex ());
}

void
YansWifiPhyHelper::EnablePcap (std::string filename, NodeContainer n)
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
YansWifiPhyHelper::EnablePcapAll (std::string filename)
{
  EnablePcap (filename, NodeContainer::GetGlobal ());
}

void 
YansWifiPhyHelper::EnableAscii (std::ostream &os, uint32_t nodeid, uint32_t deviceid)
{
  Packet::EnablePrinting ();
  std::ostringstream oss;
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Phy/State/RxOk";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiPhyRxOkEvent, &os));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::WifiNetDevice/Phy/State/Tx";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiPhyTxEvent, &os));
}
void 
YansWifiPhyHelper::EnableAscii (std::ostream &os, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnableAscii (os, dev->GetNode ()->GetId (), dev->GetIfIndex ());
    }
}
void
YansWifiPhyHelper::EnableAscii (std::ostream &os, NodeContainer n)
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
YansWifiPhyHelper::EnableAsciiAll (std::ostream &os)
{
  EnableAscii (os, NodeContainer::GetGlobal ());
}

} // namespace ns3
