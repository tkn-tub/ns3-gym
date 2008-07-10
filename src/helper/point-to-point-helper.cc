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
#include "point-to-point-helper.h"
#include "ns3/simulator.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/queue.h"
#include "ns3/pcap-writer.h"
#include "ns3/config.h"
#include "ns3/packet.h"


namespace ns3 {


PointToPointHelper::PointToPointHelper ()
{
  m_queueFactory.SetTypeId ("ns3::DropTailQueue");
  m_deviceFactory.SetTypeId ("ns3::PointToPointNetDevice");
  m_channelFactory.SetTypeId ("ns3::PointToPointChannel");
}

void 
PointToPointHelper::SetQueue (std::string type,
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
PointToPointHelper::SetDeviceAttribute (std::string n1, const AttributeValue &v1)
{
  m_deviceFactory.Set (n1, v1);
}

void 
PointToPointHelper::SetChannelAttribute (std::string n1, const AttributeValue &v1)
{
  m_channelFactory.Set (n1, v1);
}

void 
PointToPointHelper::SetDeviceParameter (std::string name, const AttributeValue &value)
{
  SetDeviceAttribute (name, value);
}
void 
PointToPointHelper::SetChannelParameter (std::string name, const AttributeValue &value)
{
  SetChannelAttribute (name, value);
}

void 
PointToPointHelper::EnablePcap (std::string filename, uint32_t nodeid, uint32_t deviceid)
{
  std::ostringstream oss;
  oss << filename << "-" << nodeid << "-" << deviceid << ".pcap";
  Ptr<PcapWriter> pcap = Create<PcapWriter> ();
  pcap->Open (oss.str ());
  pcap->WritePppHeader ();
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::PointToPointNetDevice/Rx";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&PointToPointHelper::RxEvent, pcap));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::PointToPointNetDevice/TxQueue/Enqueue";
  Config::ConnectWithoutContext (oss.str (), MakeBoundCallback (&PointToPointHelper::EnqueueEvent, pcap));
}
void 
PointToPointHelper::EnablePcap (std::string filename, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnablePcap (filename, dev->GetNode ()->GetId (), dev->GetIfIndex ());
    }
}
void
PointToPointHelper::EnablePcap (std::string filename, NodeContainer n)
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
PointToPointHelper::EnablePcapAll (std::string filename)
{
  EnablePcap (filename, NodeContainer::GetGlobal ());
}

void 
PointToPointHelper::EnableAscii (std::ostream &os, uint32_t nodeid, uint32_t deviceid)
{
  Packet::EnableMetadata ();
  std::ostringstream oss;
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::PointToPointNetDevice/Rx";
  Config::Connect (oss.str (), MakeBoundCallback (&PointToPointHelper::AsciiRxEvent, &os));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::PointToPointNetDevice/TxQueue/Enqueue";
  Config::Connect (oss.str (), MakeBoundCallback (&PointToPointHelper::AsciiEnqueueEvent, &os));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::PointToPointNetDevice/TxQueue/Dequeue";
  Config::Connect (oss.str (), MakeBoundCallback (&PointToPointHelper::AsciiDequeueEvent, &os));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::PointToPointNetDevice/TxQueue/Drop";
  Config::Connect (oss.str (), MakeBoundCallback (&PointToPointHelper::AsciiDropEvent, &os));
}
void 
PointToPointHelper::EnableAscii (std::ostream &os, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnableAscii (os, dev->GetNode ()->GetId (), dev->GetIfIndex ());
    }
}
void
PointToPointHelper::EnableAscii (std::ostream &os, NodeContainer n)
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
PointToPointHelper::EnableAsciiAll (std::ostream &os)
{
  EnableAscii (os, NodeContainer::GetGlobal ());
}

NetDeviceContainer 
PointToPointHelper::Install (NodeContainer c)
{
  NS_ASSERT (c.GetN () == 2);
  return Install (c.Get (0), c.Get (1));
}
NetDeviceContainer 
PointToPointHelper::Install (Ptr<Node> a, Ptr<Node> b)
{
  NetDeviceContainer container;

  Ptr<PointToPointNetDevice> devA = m_deviceFactory.Create<PointToPointNetDevice> ();
  devA->SetAddress (Mac48Address::Allocate ());
  a->AddDevice (devA);
  Ptr<Queue> queueA = m_queueFactory.Create<Queue> ();
  devA->SetQueue (queueA);
  Ptr<PointToPointNetDevice> devB = m_deviceFactory.Create<PointToPointNetDevice> ();
  devB->SetAddress (Mac48Address::Allocate ());
  b->AddDevice (devB);
  Ptr<Queue> queueB = m_queueFactory.Create<Queue> ();
  devB->SetQueue (queueB);
  Ptr<PointToPointChannel> channel = m_channelFactory.Create<PointToPointChannel> ();
  devA->Attach (channel);
  devB->Attach (channel);
  container.Add (devA);
  container.Add (devB);

  return container;
}

void 
PointToPointHelper::EnqueueEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet)
{
  writer->WritePacket (packet);
}
void 
PointToPointHelper::RxEvent (Ptr<PcapWriter> writer, Ptr<const Packet> packet)
{
  writer->WritePacket (packet);
}
void 
PointToPointHelper::AsciiEnqueueEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << "+ " << Simulator::Now ().GetSeconds () << " ";
  *os << path << " " << *packet << std::endl;
}
void 
PointToPointHelper::AsciiDequeueEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << "- " << Simulator::Now ().GetSeconds () << " ";
  *os << path << " " << *packet << std::endl;
}
void 
PointToPointHelper::AsciiDropEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << "d " << Simulator::Now ().GetSeconds () << " ";
  *os << path << " " << *packet << std::endl;
}
void 
PointToPointHelper::AsciiRxEvent (std::ostream *os, std::string path, Ptr<const Packet> packet)
{
  *os << "r " << Simulator::Now ().GetSeconds () << " ";
  *os << path << " " << *packet << std::endl;
}


} // namespace ns3
