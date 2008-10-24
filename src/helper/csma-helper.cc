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
#include "csma-helper.h"
#include "ns3/simulator.h"
#include "ns3/object-factory.h"
#include "ns3/queue.h"
#include "ns3/csma-net-device.h"
#include "ns3/csma-channel.h"
#include "ns3/pcap-writer.h"
#include "ns3/config.h"
#include "ns3/packet.h"
#include <string>

namespace ns3 {

CsmaHelper::CsmaHelper ()
{
  m_queueFactory.SetTypeId ("ns3::DropTailQueue");
  m_deviceFactory.SetTypeId ("ns3::CsmaNetDevice");
  m_channelFactory.SetTypeId ("ns3::CsmaChannel");
}

void 
CsmaHelper::SetQueue (std::string type,
                      std::string n1, const AttributeValue &v1,
                      std::string n2, const AttributeValue &v2,
                      std::string n3, const AttributeValue &v3,
                      std::string n4, const AttributeValue &v4)
{
  m_queueFactory.SetTypeId (type);
  m_queueFactory.Set (n1, v1);
  m_queueFactory.Set (n2, v2);
  m_queueFactory.Set (n3, v3);
  m_queueFactory.Set (n4, v4);
}

void 
CsmaHelper::SetDeviceAttribute (std::string n1, const AttributeValue &v1)
{
  m_deviceFactory.Set (n1, v1);
}

void 
CsmaHelper::SetChannelAttribute (std::string n1, const AttributeValue &v1)
{
  m_channelFactory.Set (n1, v1);
}

void 
CsmaHelper::SetDeviceParameter (std::string n1, const AttributeValue &v1)
{
  SetDeviceAttribute (n1, v1);
}
void 
CsmaHelper::SetChannelParameter (std::string n1, const AttributeValue &v1)
{
  SetChannelAttribute (n1, v1);
}

void 
CsmaHelper::EnablePcap (std::string filename, uint32_t nodeid, uint32_t deviceid)
{
  std::ostringstream oss;
  oss << filename << "-" << nodeid << "-" << deviceid << ".pcap";
  Ptr<PcapWriter> pcap = Create<PcapWriter> ();
  pcap->Open (oss.str ());
  pcap->WriteEthernetHeader ();
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CsmaNetDevice/Rx";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&CsmaHelper::RxEvent, pcap));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CsmaNetDevice/TxQueue/Enqueue";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&CsmaHelper::EnqueueEvent, pcap));
}
void 
CsmaHelper::EnablePcap (std::string filename, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnablePcap (filename, dev->GetNode ()->GetId (), dev->GetIfIndex ());
    }
}
void
CsmaHelper::EnablePcap (std::string filename, NodeContainer n)
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
CsmaHelper::EnablePcapAll (std::string filename)
{
  EnablePcap (filename, NodeContainer::GetGlobal ());
}

void 
CsmaHelper::EnableAscii (std::ostream &os, uint32_t nodeid, uint32_t deviceid)
{
  Packet::EnablePrinting ();
  std::ostringstream oss;
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CsmaNetDevice/Rx";
  Config::Connect (oss.str (), MakeBoundCallback (&CsmaHelper::AsciiRxEvent, &os));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CsmaNetDevice/TxQueue/Enqueue";
  Config::Connect (oss.str (), MakeBoundCallback (&CsmaHelper::AsciiEnqueueEvent, &os));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CsmaNetDevice/TxQueue/Dequeue";
  Config::Connect (oss.str (), MakeBoundCallback (&CsmaHelper::AsciiDequeueEvent, &os));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CsmaNetDevice/TxQueue/Drop";
  Config::Connect (oss.str (), MakeBoundCallback (&CsmaHelper::AsciiDropEvent, &os));
}
void 
CsmaHelper::EnableAscii (std::ostream &os, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnableAscii (os, dev->GetNode ()->GetId (), dev->GetIfIndex ());
    }
}
void
CsmaHelper::EnableAscii (std::ostream &os, NodeContainer n)
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
CsmaHelper::EnableAsciiAll (std::ostream &os)
{
  EnableAscii (os, NodeContainer::GetGlobal ());
}


NetDeviceContainer 
CsmaHelper::Install (const NodeContainer &c)
{
  Ptr<CsmaChannel> channel = m_channelFactory.Create ()->GetObject<CsmaChannel> ();
  return Install (c, channel);
}

NetDeviceContainer 
CsmaHelper::Install (const NodeContainer &c, Ptr<CsmaChannel> channel)
{
  NetDeviceContainer container;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;
      Ptr<CsmaNetDevice> device = m_deviceFactory.Create<CsmaNetDevice> ();
      device->SetAddress (Mac48Address::Allocate ());
      node->AddDevice (device);
      Ptr<Queue> queue = m_queueFactory.Create<Queue> ();
      device->SetQueue (queue);
      device->Attach (channel);
      container.Add (device);
    }
  return container;
}

void 
CsmaHelper::EnqueueEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet)
{
  writer->WritePacket (packet);
}
void 
CsmaHelper::RxEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet)
{
  writer->WritePacket (packet);
}
void 
CsmaHelper::AsciiEnqueueEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << "+ " << Simulator::Now ().GetSeconds () << " ";
  *os << path << " " << *packet << std::endl;
}

void 
CsmaHelper::AsciiDequeueEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << "- " << Simulator::Now ().GetSeconds () << " ";
  *os << path << " " << *packet << std::endl;
}

void 
CsmaHelper::AsciiDropEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << "d " << Simulator::Now ().GetSeconds () << " ";
  *os << path << " " << *packet << std::endl;
}

void 
CsmaHelper::AsciiRxEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << "r " << Simulator::Now ().GetSeconds () << " ";
  *os << path << " " << *packet << std::endl;
}

} // namespace ns3
