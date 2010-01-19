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

#include "ns3/abort.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/object-factory.h"
#include "ns3/queue.h"
#include "ns3/csma-net-device.h"
#include "ns3/csma-channel.h"
#include "ns3/config.h"
#include "ns3/packet.h"
#include "ns3/names.h"
#include "ns3/pcap-writer.h"
#include "ns3/ascii-writer.h"

#include "pcap-helper.h"
#include "ascii-trace-helper.h"
#include "csma-helper.h"

#include <string>

NS_LOG_COMPONENT_DEFINE ("CsmaHelper");

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
CsmaHelper::EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous)
{
  //
  // All of the Pcap enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.  We can only deal with devices of type CsmaNetDevice.
  //
  Ptr<CsmaNetDevice> device = nd->GetObject<CsmaNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("CsmaHelper::EnablePcapInternal(): Device " << device << " not of type ns3::CsmaNetDevice");
      return;
    }

  PcapHelper pcapHelper;
  std::string filename = pcapHelper.GetFilename (prefix, device);
  Ptr<PcapFileObject> file = pcapHelper.CreateFile (filename, "w", PcapHelper::DLT_EN10MB);
  if (promiscuous)
    {
      pcapHelper.HookDefaultSink<CsmaNetDevice> (device, "PromiscSniffer", file);
    }
  else
    {
      pcapHelper.HookDefaultSink<CsmaNetDevice> (device, "Sniffer", file);
    }
}

void 
CsmaHelper::EnableAsciiInternal (std::string prefix, Ptr<NetDevice> nd)
{
  //
  // All of the ascii enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.  We can only deal with devices of type CsmaNetDevice.
  //
  Ptr<CsmaNetDevice> device = nd->GetObject<CsmaNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("CsmaHelper::EnableAsciiInternal(): Device " << device << " not of type ns3::CsmaNetDevice");
      return;
    }

  //
  // Set up an output stream object to deal with private ofstream copy 
  // constructor and lifetime issues.  Let the helper decide the actual
  // name of the file given the prefix.
  //
  AsciiTraceHelper asciiTraceHelper;
  std::string filename = asciiTraceHelper.GetFilename (prefix, device);
  Ptr<OutputStreamObject> stream = asciiTraceHelper.CreateFileStream (filename, "w");

  //
  // The MacRx trace source provides our "r" event.
  //
  asciiTraceHelper.HookDefaultReceiveSink<CsmaNetDevice> (device, "MacRx", stream);

  //
  // The "+", '-', and 'd' events are driven by trace sources actually in the
  // transmit queue.
  //
  Ptr<Queue> queue = device->GetQueue ();
  asciiTraceHelper.HookDefaultEnqueueSink<Queue> (queue, "Enqueue", stream);
  asciiTraceHelper.HookDefaultDropSink<Queue> (queue, "Drop", stream);
  asciiTraceHelper.HookDefaultDequeueSink<Queue> (queue, "Dequeue", stream);
}

void 
CsmaHelper::EnableAscii (std::ostream &os, uint32_t nodeid, uint32_t deviceid)
{
  Ptr<AsciiWriter> writer = AsciiWriter::Get (os);
  Packet::EnablePrinting ();
  std::ostringstream oss;
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CsmaNetDevice/MacRx";
  Config::Connect (oss.str (), MakeBoundCallback (&CsmaHelper::AsciiRxEvent, writer));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CsmaNetDevice/TxQueue/Enqueue";
  Config::Connect (oss.str (), MakeBoundCallback (&CsmaHelper::AsciiEnqueueEvent, writer));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CsmaNetDevice/TxQueue/Dequeue";
  Config::Connect (oss.str (), MakeBoundCallback (&CsmaHelper::AsciiDequeueEvent, writer));
  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CsmaNetDevice/TxQueue/Drop";
  Config::Connect (oss.str (), MakeBoundCallback (&CsmaHelper::AsciiDropEvent, writer));
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
CsmaHelper::Install (Ptr<Node> node) const
{
  Ptr<CsmaChannel> channel = m_channelFactory.Create ()->GetObject<CsmaChannel> ();
  return Install (node, channel);
}

NetDeviceContainer
CsmaHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return Install (node);
}

NetDeviceContainer
CsmaHelper::Install (Ptr<Node> node, Ptr<CsmaChannel> channel) const
{
  return NetDeviceContainer (InstallPriv (node, channel));
}

NetDeviceContainer
CsmaHelper::Install (Ptr<Node> node, std::string channelName) const
{
  Ptr<CsmaChannel> channel = Names::Find<CsmaChannel> (channelName);
  return NetDeviceContainer (InstallPriv (node, channel));
}

NetDeviceContainer
CsmaHelper::Install (std::string nodeName, Ptr<CsmaChannel> channel) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return NetDeviceContainer (InstallPriv (node, channel));
}

NetDeviceContainer
CsmaHelper::Install (std::string nodeName, std::string channelName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  Ptr<CsmaChannel> channel = Names::Find<CsmaChannel> (channelName);
  return NetDeviceContainer (InstallPriv (node, channel));
}

NetDeviceContainer 
CsmaHelper::Install (const NodeContainer &c) const
{
  Ptr<CsmaChannel> channel = m_channelFactory.Create ()->GetObject<CsmaChannel> ();

  return Install (c, channel);
}

NetDeviceContainer 
CsmaHelper::Install (const NodeContainer &c, Ptr<CsmaChannel> channel) const
{
  NetDeviceContainer devs;

  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      devs.Add (InstallPriv (*i, channel));
    }

  return devs;
}

NetDeviceContainer 
CsmaHelper::Install (const NodeContainer &c, std::string channelName) const
{
  Ptr<CsmaChannel> channel = Names::Find<CsmaChannel> (channelName);
  return Install (c, channel);
}

Ptr<NetDevice>
CsmaHelper::InstallPriv (Ptr<Node> node, Ptr<CsmaChannel> channel) const
{
  Ptr<CsmaNetDevice> device = m_deviceFactory.Create<CsmaNetDevice> ();
  device->SetAddress (Mac48Address::Allocate ());
  node->AddDevice (device);
  Ptr<Queue> queue = m_queueFactory.Create<Queue> ();
  device->SetQueue (queue);
  device->Attach (channel);

  return device;
}

void 
CsmaHelper::AsciiEnqueueEvent (Ptr<AsciiWriter> writer, std::string path, Ptr<const Packet> packet)
{
  writer->WritePacket (AsciiWriter::ENQUEUE, path, packet);
}

void 
CsmaHelper::AsciiDequeueEvent (Ptr<AsciiWriter> writer, std::string path, Ptr<const Packet> packet)
{
  writer->WritePacket (AsciiWriter::DEQUEUE, path, packet);
}

void 
CsmaHelper::AsciiDropEvent (Ptr<AsciiWriter> writer, std::string path, Ptr<const Packet> packet)
{
  writer->WritePacket (AsciiWriter::DROP, path, packet);
}

void 
CsmaHelper::AsciiRxEvent (Ptr<AsciiWriter> writer, std::string path, Ptr<const Packet> packet)
{
  writer->WritePacket (AsciiWriter::RX, path, packet);
}

} // namespace ns3
