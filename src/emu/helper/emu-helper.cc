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
#include "ns3/config.h"
#include "ns3/packet.h"

#include "ns3/trace-helper.h"
#include "emu-helper.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EmuHelper");

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
EmuHelper::EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename)
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

  std::string filename;
  if (explicitFilename)
    {
      filename = prefix;
    }
  else
    {
      filename = pcapHelper.GetFilenameFromDevice (prefix, device);
    }

  Ptr<PcapFileWrapper> file = pcapHelper.CreateFile (filename, std::ios::out, PcapHelper::DLT_EN10MB);
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
EmuHelper::EnableAsciiInternal (
  Ptr<OutputStreamWrapper> stream, 
  std::string prefix, 
  Ptr<NetDevice> nd,
  bool explicitFilename)
{
  //
  // All of the ascii enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.  We can only deal with devices of type EmuNetDevice.
  //
  Ptr<EmuNetDevice> device = nd->GetObject<EmuNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("EmuHelper::EnableAsciiInternal(): Device " << device << " not of type ns3::EmuNetDevice");
      return;
    }

  //
  // Our default trace sinks are going to use packet printing, so we have to 
  // make sure that is turned on.
  //
  Packet::EnablePrinting ();

  //
  // If we are not provided an OutputStreamWrapper, we are expected to create 
  // one using the usual trace filename conventions and do a Hook*WithoutContext
  // since there will be one file per context and therefore the context would
  // be redundant.
  //
  if (stream == 0)
    {
      //
      // Set up an output stream object to deal with private ofstream copy 
      // constructor and lifetime issues.  Let the helper decide the actual
      // name of the file given the prefix.
      //
      AsciiTraceHelper asciiTraceHelper;

      std::string filename;
      if (explicitFilename)
        {
          filename = prefix;
        }
      else
        {
          filename = asciiTraceHelper.GetFilenameFromDevice (prefix, device);
        }

      Ptr<OutputStreamWrapper> theStream = asciiTraceHelper.CreateFileStream (filename);

      //
      // The MacRx trace source provides our "r" event.
      //
      asciiTraceHelper.HookDefaultReceiveSinkWithoutContext<EmuNetDevice> (device, "MacRx", theStream);

      //
      // The "+", '-', and 'd' events are driven by trace sources actually in the
      // transmit queue.
      //
      Ptr<Queue> queue = device->GetQueue ();
      asciiTraceHelper.HookDefaultEnqueueSinkWithoutContext<Queue> (queue, "Enqueue", theStream);
      asciiTraceHelper.HookDefaultDropSinkWithoutContext<Queue> (queue, "Drop", theStream);
      asciiTraceHelper.HookDefaultDequeueSinkWithoutContext<Queue> (queue, "Dequeue", theStream);

      return;
    }

  //
  // If we are provided an OutputStreamWrapper, we are expected to use it, and
  // to providd a context.  We are free to come up with our own context if we
  // want, and use the AsciiTraceHelper Hook*WithContext functions, but for 
  // compatibility and simplicity, we just use Config::Connect and let it deal
  // with the context.
  //
  // Note that we are going to use the default trace sinks provided by the 
  // ascii trace helper.  There is actually no AsciiTraceHelper in sight here,
  // but the default trace sinks are actually publicly available static 
  // functions that are always there waiting for just such a case.
  //
  uint32_t nodeid = nd->GetNode ()->GetId ();
  uint32_t deviceid = nd->GetIfIndex ();
  std::ostringstream oss;

  oss << "/NodeList/" << nd->GetNode ()->GetId () << "/DeviceList/" << deviceid << "/$ns3::EmuNetDevice/MacRx";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultReceiveSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::EmuNetDevice/TxQueue/Enqueue";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultEnqueueSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::EmuNetDevice/TxQueue/Dequeue";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultDequeueSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::EmuNetDevice/TxQueue/Drop";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultDropSinkWithContext, stream));
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

} // namespace ns3
