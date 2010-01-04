/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
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
 */

#include <string>

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/object-factory.h"
#include "ns3/names.h"
#include "ns3/queue.h"
#include "ns3/emu-net-device.h"
#include "ns3/pcap-writer.h"
#include "ns3/ascii-writer.h"
#include "ns3/config.h"
#include "ns3/packet.h"

#include "pcap-helper.h"
#include "emu-helper.h"

NS_LOG_COMPONENT_DEFINE ("EmuHelper");

namespace ns3 {

EmuHelper::EmuHelper ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_queueFactory.SetTypeId ("ns3::DropTailQueue");
  m_deviceFactory.SetTypeId ("ns3::EmuNetDevice");
}

void 
EmuHelper::SetQueue (
  std::string type,
  std::string n1, const AttributeValue &v1,
  std::string n2, const AttributeValue &v2,
  std::string n3, const AttributeValue &v3,
  std::string n4, const AttributeValue &v4)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_queueFactory.SetTypeId (type);
  m_queueFactory.Set (n1, v1);
  m_queueFactory.Set (n2, v2);
  m_queueFactory.Set (n3, v3);
  m_queueFactory.Set (n4, v4);
}

void 
EmuHelper::SetAttribute (std::string n1, const AttributeValue &v1)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_deviceFactory.Set (n1, v1);
}

void 
EmuHelper::EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous)
{
  //
  // All of the Pcap enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.  We can only deal with devices of type EmuNetDevice.
  //
  Ptr<EmuNetDevice> device = nd->GetObject<EmuNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("EmuHelper::EnablePcapInternal(): Device " << device << " not of type ns3::EmuNetDevice");
      return;
    }

  PcapHelper pcapHelper;
  std::string filename = pcapHelper.GetFilename (prefix, device);
  Ptr<PcapFileObject> file = pcapHelper.CreateFile (filename, "w", PcapHelper::DLT_EN10MB);
  if (promiscuous)
    {
      pcapHelper.HookDefaultSink<EmuNetDevice> (device, "PromiscSniffer", file);
    }
  else
    {
      pcapHelper.HookDefaultSink<EmuNetDevice> (device, "Sniffer", file);
    }
}

void 
EmuHelper::EnableAscii (std::ostream &os, uint32_t nodeid, uint32_t deviceid)
{
  NS_LOG_FUNCTION (&os << nodeid << deviceid);
  Ptr<AsciiWriter> writer = AsciiWriter::Get (os);
  Packet::EnablePrinting ();
  std::ostringstream oss;

  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::EmuNetDevice/MacRx";
  Config::Connect (oss.str (), MakeBoundCallback (&EmuHelper::AsciiRxEvent, writer));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::EmuNetDevice/TxQueue/Enqueue";
  Config::Connect (oss.str (), MakeBoundCallback (&EmuHelper::AsciiEnqueueEvent, writer));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::EmuNetDevice/TxQueue/Dequeue";
  Config::Connect (oss.str (), MakeBoundCallback (&EmuHelper::AsciiDequeueEvent, writer));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::EmuNetDevice/TxQueue/Drop";
  Config::Connect (oss.str (), MakeBoundCallback (&EmuHelper::AsciiDropEvent, writer));
}

void 
EmuHelper::EnableAscii (std::ostream &os, NetDeviceContainer d)
{
  NS_LOG_FUNCTION (&os << &d);
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnableAscii (os, dev->GetNode ()->GetId (), dev->GetIfIndex ());
    }
}

void
EmuHelper::EnableAscii (std::ostream &os, NodeContainer n)
{
  NS_LOG_FUNCTION (&os << &n);
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
EmuHelper::EnableAsciiAll (std::ostream &os)
{
  NS_LOG_FUNCTION (&os);
  EnableAscii (os, NodeContainer::GetGlobal ());
}

NetDeviceContainer
EmuHelper::Install (Ptr<Node> node) const
{
  return NetDeviceContainer (InstallPriv (node));
}

NetDeviceContainer
EmuHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return NetDeviceContainer (InstallPriv (node));
}

NetDeviceContainer 
EmuHelper::Install (const NodeContainer &c) const
{
  NetDeviceContainer devs;

  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      devs.Add (InstallPriv (*i));
    }

  return devs;
}

Ptr<NetDevice>
EmuHelper::InstallPriv (Ptr<Node> node) const
{
  Ptr<EmuNetDevice> device = m_deviceFactory.Create<EmuNetDevice> ();
  device->SetAddress (Mac48Address::Allocate ());
  node->AddDevice (device);
  Ptr<Queue> queue = m_queueFactory.Create<Queue> ();
  device->SetQueue (queue);

  return device;
}

void
EmuHelper::AsciiEnqueueEvent (Ptr<AsciiWriter> writer,
                              std::string path,
                              Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (writer << path << packet);
  writer->WritePacket (AsciiWriter::ENQUEUE, path, packet);
}

void
EmuHelper::AsciiDequeueEvent (Ptr<AsciiWriter> writer,
                              std::string path,
                              Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (writer << path << packet);
  writer->WritePacket (AsciiWriter::DEQUEUE, path, packet);
}

void
EmuHelper::AsciiDropEvent (Ptr<AsciiWriter> writer,
                           std::string path,
                           Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (writer << path << packet);
  writer->WritePacket (AsciiWriter::DROP, path, packet);
}

void
EmuHelper::AsciiRxEvent (Ptr<AsciiWriter> writer,
                         std::string path,
                         Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (writer << path << packet);
  writer->WritePacket (AsciiWriter::RX, path, packet);
}

} // namespace ns3
