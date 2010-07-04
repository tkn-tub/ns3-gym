/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 University of Washington
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

#include <stdint.h>
#include <string>
#include <fstream>

#include "ns3/abort.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/names.h"
#include "ns3/net-device.h"
#include "ns3/pcap-file-wrapper.h"

#include "trace-helper.h"

NS_LOG_COMPONENT_DEFINE("TraceHelper");

namespace ns3 {

PcapHelper::PcapHelper ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

PcapHelper::~PcapHelper ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ptr<PcapFileWrapper>
PcapHelper::CreateFile (
  std::string filename, 
  std::ios::openmode filemode,
  uint32_t    dataLinkType, 
  uint32_t    snapLen, 
  int32_t     tzCorrection)
{
  NS_LOG_FUNCTION (filename << filemode << dataLinkType << snapLen << tzCorrection);

  Ptr<PcapFileWrapper> file = CreateObject<PcapFileWrapper> ();
  file->Open (filename, filemode);
  NS_ABORT_MSG_IF (file->Fail (), "Unable to Open " << filename << " for mode " << filemode);

  file->Init (dataLinkType, snapLen, tzCorrection);
  NS_ABORT_MSG_IF (file->Fail (), "Unable to Init " << filename);

  //
  // Note that the pcap helper promptly forgets all about the pcap file.  We
  // rely on the reference count of the file object which will soon be owned
  // by the caller to keep the object alive.  If the caller uses the file 
  // object to hook a trace source, ownership of the file object will be
  // implicitly transferred to the callback which keeps the object alive.
  // When the callback is destroyed (when either the trace is disconnected or
  // the object with the trace source is deleted) the callback will be destroyed
  // and the file object will be destroyed, releasing the pointer and closing
  // the file.
  //
  return file;
}

std::string
PcapHelper::GetFilenameFromDevice (std::string prefix, Ptr<NetDevice> device, bool useObjectNames)
{
  NS_LOG_FUNCTION (prefix << device << useObjectNames);
  NS_ABORT_MSG_UNLESS (prefix.size (), "Empty prefix string");

  std::ostringstream oss;
  oss << prefix << "-";

  std::string nodename;
  std::string devicename;

  Ptr<Node> node = device->GetNode ();

  if (useObjectNames)
    {
      nodename = Names::FindName (node);
      devicename = Names::FindName (device);
    }

  if (nodename.size ())
    {
      oss << nodename;
    }
  else
    {
      oss << node->GetId ();
    }

  oss << "-";

  if (devicename.size ())
    {
      oss << devicename;
    }
  else
    {
      oss << device->GetIfIndex ();
    }

  oss << ".pcap";

  return oss.str ();
}

std::string
PcapHelper::GetFilenameFromInterfacePair (std::string prefix, Ptr<Object> object, uint32_t interface, bool useObjectNames)
{
  NS_LOG_FUNCTION (prefix << object << interface << useObjectNames);
  NS_ABORT_MSG_UNLESS (prefix.size (), "Empty prefix string");

  std::ostringstream oss;
  oss << prefix << "-";

  std::string objname;
  std::string nodename;

  Ptr<Node> node = object->GetObject<Node> ();

  if (useObjectNames)
    {
      objname = Names::FindName (object);
      nodename = Names::FindName (node);
    }

  if (objname.size ())
    {
      oss << objname;
    }
  else if (nodename.size ())
    {
      oss << nodename;
    }
  else
    {
      oss << "n" << node->GetId ();
    }

  oss << "-i" << interface << ".pcap";

  return oss.str ();
}

//
// The basic default trace sink.  This one just writes the packet to the pcap
// file which is good enough for most kinds of captures.
//
void
PcapHelper::DefaultSink (Ptr<PcapFileWrapper> file, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (file << p);
  file->Write(Simulator::Now(), p);
}

AsciiTraceHelper::AsciiTraceHelper ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

AsciiTraceHelper::~AsciiTraceHelper ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ptr<OutputStreamWrapper>
AsciiTraceHelper::CreateFileStream (std::string filename, std::ios::openmode filemode)
{
  NS_LOG_FUNCTION (filename << filemode);

  Ptr<OutputStreamWrapper> StreamWrapper = Create<OutputStreamWrapper> (filename, filemode);

  //
  // Note that the ascii trace helper promptly forgets all about the trace file.
  // We rely on the reference count of the file object which will soon be owned
  // by the caller to keep the object alive.  If the caller uses the stream 
  // object to hook a trace source, ownership of the stream object will be
  // implicitly transferred to the callback which keeps the object alive.
  // When the callback is destroyed (when either the trace is disconnected or
  // the object with the trace source is deleted) the callback will be destroyed
  // and the stream object will be destroyed, releasing the pointer and closing
  // the underlying file.
  //
  return StreamWrapper;
}

std::string
AsciiTraceHelper::GetFilenameFromDevice (std::string prefix, Ptr<NetDevice> device, bool useObjectNames)
{
  NS_LOG_FUNCTION (prefix << device << useObjectNames);
  NS_ABORT_MSG_UNLESS (prefix.size (), "Empty prefix string");

  std::ostringstream oss;
  oss << prefix << "-";

  std::string nodename;
  std::string devicename;

  Ptr<Node> node = device->GetNode ();

  if (useObjectNames)
    {
      nodename = Names::FindName (node);
      devicename = Names::FindName (device);
    }

  if (nodename.size ())
    {
      oss << nodename;
    }
  else
    {
      oss << node->GetId ();
    }

  oss << "-";

  if (devicename.size ())
    {
      oss << devicename;
    }
  else
    {
      oss << device->GetIfIndex ();
    }

  oss << ".tr";

  return oss.str ();
}

std::string
AsciiTraceHelper::GetFilenameFromInterfacePair (
  std::string prefix, 
  Ptr<Object> object, 
  uint32_t interface, 
  bool useObjectNames)
{
  NS_LOG_FUNCTION (prefix << object << interface << useObjectNames);
  NS_ABORT_MSG_UNLESS (prefix.size (), "Empty prefix string");

  std::ostringstream oss;
  oss << prefix << "-";

  std::string objname;
  std::string nodename;

  Ptr<Node> node = object->GetObject<Node> ();

  if (useObjectNames)
    {
      objname = Names::FindName (object);
      nodename = Names::FindName (node);
    }

  if (objname.size ())
    {
      oss << objname;
    }
  else if (nodename.size ())
    {
      oss << nodename;
    }
  else
    {
      oss << "n" << node->GetId ();
    }

  oss << "-i" << interface << ".tr";

  return oss.str ();
}

//
// One of the basic default trace sink sets.  Enqueue:
//
//   When a packet has been sent to a device for transmission, the device is
//   expected to place the packet onto a transmit queue even if it does not
//   have to delay the packet at all, if only to trigger this event.  This 
//   event will eventually translate into a '+' operation in the trace file.
//
//   This is typically implemented by hooking the "TxQueue/Enqueue" trace hook
//   in the device (actually the Queue in the device).
//
void
AsciiTraceHelper::DefaultEnqueueSinkWithoutContext (Ptr<OutputStreamWrapper> stream, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (stream << p);
  *stream->GetStream () << "+ " << Simulator::Now ().GetSeconds () << " " << *p << std::endl;
}

void
AsciiTraceHelper::DefaultEnqueueSinkWithContext (Ptr<OutputStreamWrapper> stream, std::string context, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (stream << p);
  *stream->GetStream () << "+ " << Simulator::Now ().GetSeconds () << " " << context << " " << *p << std::endl;
}

//
// One of the basic default trace sink sets.  Drop:
//
//   When a packet has been sent to a device for transmission, the device is
//   expected to place the packet onto a transmit queue.  If this queue is 
//   full the packet will be dropped.  The device is expected to trigger an
//   event to indicate that an outbound packet is being dropped.  This event
//   will eventually translate into a 'd' operation in the trace file.
//
//   This is typically implemented by hooking the "TxQueue/Drop" trace hook
//   in the device (actually the Queue in the device).
//
void
AsciiTraceHelper::DefaultDropSinkWithoutContext (Ptr<OutputStreamWrapper> stream, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (stream << p);
  *stream->GetStream () << "d " << Simulator::Now ().GetSeconds () << " " << *p << std::endl;
}

void
AsciiTraceHelper::DefaultDropSinkWithContext (Ptr<OutputStreamWrapper> stream, std::string context, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (stream << p);
  *stream->GetStream () << "d " << Simulator::Now ().GetSeconds () << " " << context << " " << *p << std::endl;
}

//
// One of the basic default trace sink sets.  Dequeue:
//
//   When a packet has been sent to a device for transmission, the device is
//   expected to place the packet onto a transmit queue even if it does not
//   have to delay the packet at all.  The device removes the packet from the
//   transmit queue when the packet is ready to send, and this dequeue will 
//   fire a corresponding event.  This event will eventually translate into a 
//   '-' operation in the trace file.
//
//   This is typically implemented by hooking the "TxQueue/Dequeue" trace hook
//   in the device (actually the Queue in the device).
//
void
AsciiTraceHelper::DefaultDequeueSinkWithoutContext (Ptr<OutputStreamWrapper> stream, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (stream << p);
  *stream->GetStream () << "- " << Simulator::Now ().GetSeconds () << " " << *p << std::endl;
}

void
AsciiTraceHelper::DefaultDequeueSinkWithContext (Ptr<OutputStreamWrapper> stream, std::string context, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (stream << p);
  *stream->GetStream () << "- " << Simulator::Now ().GetSeconds () << " " << context << " " << *p << std::endl;
}

//
// One of the basic default trace sink sets.  Receive:
//
//   When a packet is received by a device for transmission, the device is
//   expected to trigger this event to indicate the reception has occurred.
//   This event will eventually translate into an 'r' operation in the trace 
//   file.
//
//   This is typically implemented by hooking the "MacRx" trace hook in the
//   device.
void
AsciiTraceHelper::DefaultReceiveSinkWithoutContext (Ptr<OutputStreamWrapper> stream, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (stream << p);
  *stream->GetStream () << "r " << Simulator::Now ().GetSeconds () << " " << *p << std::endl;
}

void
AsciiTraceHelper::DefaultReceiveSinkWithContext (Ptr<OutputStreamWrapper> stream, std::string context, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (stream << p);
  *stream->GetStream () << "r " << Simulator::Now ().GetSeconds () << " " << context << " " << *p << std::endl;
}

void 
PcapHelperForDevice::EnablePcap (std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename)
{
  EnablePcapInternal (prefix, nd, promiscuous, explicitFilename);
}

void 
PcapHelperForDevice::EnablePcap (std::string prefix, std::string ndName, bool promiscuous, bool explicitFilename)
{
  Ptr<NetDevice> nd = Names::Find<NetDevice> (ndName);
  EnablePcap (prefix, nd, promiscuous, explicitFilename);
}

void 
PcapHelperForDevice::EnablePcap (std::string prefix, NetDeviceContainer d, bool promiscuous)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnablePcap (prefix, dev, promiscuous);
    }
}

void
PcapHelperForDevice::EnablePcap (std::string prefix, NodeContainer n, bool promiscuous)
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
  EnablePcap (prefix, devs, promiscuous);
}

void
PcapHelperForDevice::EnablePcapAll (std::string prefix, bool promiscuous)
{
  EnablePcap (prefix, NodeContainer::GetGlobal (), promiscuous);
}

void 
PcapHelperForDevice::EnablePcap (std::string prefix, uint32_t nodeid, uint32_t deviceid, bool promiscuous)
{
  NodeContainer n = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetId () != nodeid) 
        {
          continue;
        }
      
      NS_ABORT_MSG_IF (deviceid >= node->GetNDevices (), "PcapHelperForDevice::EnablePcap(): Unknown deviceid = " 
                       << deviceid);
      Ptr<NetDevice> nd = node->GetDevice (deviceid);
      EnablePcap (prefix, nd, promiscuous);
      return;
    }
}

//
// Public API
//
void 
AsciiTraceHelperForDevice::EnableAscii (std::string prefix, Ptr<NetDevice> nd, bool explicitFilename)
{
  EnableAsciiInternal (Ptr<OutputStreamWrapper> (), prefix, nd, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForDevice::EnableAscii (Ptr<OutputStreamWrapper> stream, Ptr<NetDevice> nd)
{
  EnableAsciiInternal (stream, std::string (), nd, false);
}

//
// Public API
//
void 
AsciiTraceHelperForDevice::EnableAscii (std::string prefix, std::string ndName, bool explicitFilename)
{
  EnableAsciiImpl (Ptr<OutputStreamWrapper> (), prefix, ndName, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForDevice::EnableAscii (Ptr<OutputStreamWrapper> stream, std::string ndName)
{
  EnableAsciiImpl (stream, std::string (), ndName, false);
}

//
// Private API
//
void 
AsciiTraceHelperForDevice::EnableAsciiImpl (
  Ptr<OutputStreamWrapper> stream, 
  std::string prefix, 
  std::string ndName,
  bool explicitFilename)
{
  Ptr<NetDevice> nd = Names::Find<NetDevice> (ndName);
  EnableAsciiInternal (stream, prefix, nd, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForDevice::EnableAscii (std::string prefix, NetDeviceContainer d)
{
  EnableAsciiImpl (Ptr<OutputStreamWrapper> (), prefix, d);
}

//
// Public API
//
void 
AsciiTraceHelperForDevice::EnableAscii (Ptr<OutputStreamWrapper> stream, NetDeviceContainer d)
{
  EnableAsciiImpl (stream, std::string (), d);
}

//
// Private API
//
void 
AsciiTraceHelperForDevice::EnableAsciiImpl (Ptr<OutputStreamWrapper> stream, std::string prefix, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnableAsciiInternal (stream, prefix, dev, false);
    }
}

//
// Public API
//
void
AsciiTraceHelperForDevice::EnableAscii (std::string prefix, NodeContainer n)
{
  EnableAsciiImpl (Ptr<OutputStreamWrapper> (), prefix, n);
}

//
// Public API
//
void
AsciiTraceHelperForDevice::EnableAscii (Ptr<OutputStreamWrapper> stream, NodeContainer n)
{
  EnableAsciiImpl (stream, std::string (), n);
}

//
// Private API
//
void
AsciiTraceHelperForDevice::EnableAsciiImpl (Ptr<OutputStreamWrapper> stream, std::string prefix, NodeContainer n)
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
  EnableAsciiImpl (stream, prefix, devs);
}

//
// Public API
//
void
AsciiTraceHelperForDevice::EnableAsciiAll (std::string prefix)
{
  EnableAsciiImpl (Ptr<OutputStreamWrapper> (), prefix, NodeContainer::GetGlobal ());
}

//
// Public API
//
void
AsciiTraceHelperForDevice::EnableAsciiAll (Ptr<OutputStreamWrapper> stream)
{
  EnableAsciiImpl (stream, std::string (), NodeContainer::GetGlobal ());
}

//
// Public API
//
void 
AsciiTraceHelperForDevice::EnableAscii (Ptr<OutputStreamWrapper> stream, uint32_t nodeid, uint32_t deviceid)
{
  EnableAsciiImpl (stream, std::string (), nodeid, deviceid, false);
}

//
// Public API
//
void 
AsciiTraceHelperForDevice::EnableAscii (
  std::string prefix, 
  uint32_t nodeid, 
  uint32_t deviceid,
  bool explicitFilename)
{
  EnableAsciiImpl (Ptr<OutputStreamWrapper> (), prefix, nodeid, deviceid, explicitFilename);
}

//
// Private API
//
void 
AsciiTraceHelperForDevice::EnableAsciiImpl (
  Ptr<OutputStreamWrapper> stream, 
  std::string prefix, 
  uint32_t nodeid, 
  uint32_t deviceid,
  bool explicitFilename)
{
  NodeContainer n = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetId () != nodeid) 
        {
          continue;
        }
      
      NS_ABORT_MSG_IF (deviceid >= node->GetNDevices (), 
                       "AsciiTraceHelperForDevice::EnableAscii(): Unknown deviceid = " << deviceid);

      Ptr<NetDevice> nd = node->GetDevice (deviceid);

      EnableAsciiInternal (stream, prefix, nd, explicitFilename);
      return;
    }
}

void 
PcapHelperForIpv4::EnablePcapIpv4 (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface, bool explicitFilename)
{
  EnablePcapIpv4Internal (prefix, ipv4, interface, explicitFilename);
}

void 
PcapHelperForIpv4::EnablePcapIpv4 (std::string prefix, std::string ipv4Name, uint32_t interface, bool explicitFilename)
{
  Ptr<Ipv4> ipv4 = Names::Find<Ipv4> (ipv4Name);
  EnablePcapIpv4 (prefix, ipv4, interface, explicitFilename);
}

void 
PcapHelperForIpv4::EnablePcapIpv4 (std::string prefix, Ipv4InterfaceContainer c)
{
  for (Ipv4InterfaceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      std::pair<Ptr<Ipv4>, uint32_t> pair = *i;
      EnablePcapIpv4 (prefix, pair.first, pair.second, false);
    }
}

void
PcapHelperForIpv4::EnablePcapIpv4 (std::string prefix, NodeContainer n)
{
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      if (ipv4)
        {
          for (uint32_t j = 0; j < ipv4->GetNInterfaces (); ++j)
            {
              EnablePcapIpv4 (prefix, ipv4, j, false);
            }
        }
    }
}

void
PcapHelperForIpv4::EnablePcapIpv4All (std::string prefix)
{
  EnablePcapIpv4 (prefix, NodeContainer::GetGlobal ());
}

void 
PcapHelperForIpv4::EnablePcapIpv4 (std::string prefix, uint32_t nodeid, uint32_t interface, bool explicitFilename)
{
  NodeContainer n = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetId () != nodeid) 
        {
          continue;
        }
      
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      if (ipv4)
        {
          EnablePcapIpv4 (prefix, ipv4, interface, explicitFilename);
        }
      return;
    }
}

//
// Public API
//
void 
AsciiTraceHelperForIpv4::EnableAsciiIpv4 (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface, bool explicitFilename)
{
  EnableAsciiIpv4Internal (Ptr<OutputStreamWrapper> (), prefix, ipv4, interface, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForIpv4::EnableAsciiIpv4 (Ptr<OutputStreamWrapper> stream, Ptr<Ipv4> ipv4, uint32_t interface)
{
  EnableAsciiIpv4Internal (stream, std::string (), ipv4, interface, false);
}

//
// Public API
//
void 
AsciiTraceHelperForIpv4::EnableAsciiIpv4 (
  std::string prefix, 
  std::string ipv4Name, 
  uint32_t interface,
  bool explicitFilename)
{
  EnableAsciiIpv4Impl (Ptr<OutputStreamWrapper> (), prefix, ipv4Name, interface, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForIpv4::EnableAsciiIpv4 (Ptr<OutputStreamWrapper> stream, std::string ipv4Name, uint32_t interface)
{
  EnableAsciiIpv4Impl (stream, std::string (), ipv4Name, interface, false);
}

//
// Private API
//
void 
AsciiTraceHelperForIpv4::EnableAsciiIpv4Impl (
  Ptr<OutputStreamWrapper> stream, 
  std::string prefix, 
  std::string ipv4Name, 
  uint32_t interface,
  bool explicitFilename)
{
  Ptr<Ipv4> ipv4 = Names::Find<Ipv4> (ipv4Name);
  EnableAsciiIpv4Internal (stream, prefix, ipv4, interface, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForIpv4::EnableAsciiIpv4 (std::string prefix, Ipv4InterfaceContainer c)
{
  EnableAsciiIpv4Impl (Ptr<OutputStreamWrapper> (), prefix, c);
}

//
// Public API
//
void 
AsciiTraceHelperForIpv4::EnableAsciiIpv4 (Ptr<OutputStreamWrapper> stream, Ipv4InterfaceContainer c)
{
  EnableAsciiIpv4Impl (stream, std::string (), c);
}

//
// Private API
//
void 
AsciiTraceHelperForIpv4::EnableAsciiIpv4Impl (Ptr<OutputStreamWrapper> stream, std::string prefix, Ipv4InterfaceContainer c)
{
  for (Ipv4InterfaceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      std::pair<Ptr<Ipv4>, uint32_t> pair = *i;
      EnableAsciiIpv4Internal (stream, prefix, pair.first, pair.second, false);
    }
}

//
// Public API
//
void
AsciiTraceHelperForIpv4::EnableAsciiIpv4 (std::string prefix, NodeContainer n)
{
  EnableAsciiIpv4Impl (Ptr<OutputStreamWrapper> (), prefix, n);
}

//
// Public API
//
void
AsciiTraceHelperForIpv4::EnableAsciiIpv4 (Ptr<OutputStreamWrapper> stream, NodeContainer n)
{
  EnableAsciiIpv4Impl (stream, std::string (), n);
}

//
// Private API
//
void
AsciiTraceHelperForIpv4::EnableAsciiIpv4Impl (Ptr<OutputStreamWrapper> stream, std::string prefix, NodeContainer n)
{
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      if (ipv4)
        {
          for (uint32_t j = 0; j < ipv4->GetNInterfaces (); ++j)
            {
              EnableAsciiIpv4Internal (stream, prefix, ipv4, j, false);
            }
        }
    }
}

//
// Public API
//
void
AsciiTraceHelperForIpv4::EnableAsciiIpv4All (std::string prefix)
{
  EnableAsciiIpv4Impl (Ptr<OutputStreamWrapper> (), prefix, NodeContainer::GetGlobal ());
}

//
// Public API
//
void
AsciiTraceHelperForIpv4::EnableAsciiIpv4All (Ptr<OutputStreamWrapper> stream)
{
  EnableAsciiIpv4Impl (stream, std::string (), NodeContainer::GetGlobal ());
}

//
// Public API
//
void 
AsciiTraceHelperForIpv4::EnableAsciiIpv4 (
  Ptr<OutputStreamWrapper> stream, 
  uint32_t nodeid, 
  uint32_t interface,
  bool explicitFilename)
{
  EnableAsciiIpv4Impl (stream, std::string (), nodeid, interface, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForIpv4::EnableAsciiIpv4 (std::string prefix, uint32_t nodeid, uint32_t interface, bool explicitFilename)
{
  EnableAsciiIpv4Impl (Ptr<OutputStreamWrapper> (), prefix, nodeid, interface, explicitFilename);
}

//
// Private API
//
void 
AsciiTraceHelperForIpv4::EnableAsciiIpv4Impl (
  Ptr<OutputStreamWrapper> stream, 
  std::string prefix, 
  uint32_t nodeid, 
  uint32_t interface,
  bool explicitFilename)
{
  NodeContainer n = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetId () != nodeid) 
        {
          continue;
        }

      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      if (ipv4)
        {
          EnableAsciiIpv4Internal (stream, prefix, ipv4, interface, explicitFilename);
        }

      return;
    }
}

void 
PcapHelperForIpv6::EnablePcapIpv6 (std::string prefix, Ptr<Ipv6> ipv6, uint32_t interface, bool explicitFilename)
{
  EnablePcapIpv6Internal (prefix, ipv6, interface, explicitFilename);
}

void 
PcapHelperForIpv6::EnablePcapIpv6 (std::string prefix, std::string ipv6Name, uint32_t interface, bool explicitFilename)
{
  Ptr<Ipv6> ipv6 = Names::Find<Ipv6> (ipv6Name);
  EnablePcapIpv6 (prefix, ipv6, interface, explicitFilename);
}

void 
PcapHelperForIpv6::EnablePcapIpv6 (std::string prefix, Ipv6InterfaceContainer c)
{
  for (Ipv6InterfaceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      std::pair<Ptr<Ipv6>, uint32_t> pair = *i;
      EnablePcapIpv6 (prefix, pair.first, pair.second, false);
    }
}

void
PcapHelperForIpv6::EnablePcapIpv6 (std::string prefix, NodeContainer n)
{
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
      if (ipv6)
        {
          for (uint32_t j = 0; j < ipv6->GetNInterfaces (); ++j)
            {
              EnablePcapIpv6 (prefix, ipv6, j, false);
            }
        }
    }
}

void
PcapHelperForIpv6::EnablePcapIpv6All (std::string prefix)
{
  EnablePcapIpv6 (prefix, NodeContainer::GetGlobal ());
}

void 
PcapHelperForIpv6::EnablePcapIpv6 (std::string prefix, uint32_t nodeid, uint32_t interface, bool explicitFilename)
{
  NodeContainer n = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetId () != nodeid) 
        {
          continue;
        }
      
      Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
      if (ipv6)
        {
          EnablePcapIpv6 (prefix, ipv6, interface, explicitFilename);
        }
      return;
    }
}

//
// Public API
//
void 
AsciiTraceHelperForIpv6::EnableAsciiIpv6 (std::string prefix, Ptr<Ipv6> ipv6, uint32_t interface, bool explicitFilename)
{
  EnableAsciiIpv6Internal (Ptr<OutputStreamWrapper> (), prefix, ipv6, interface, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForIpv6::EnableAsciiIpv6 (Ptr<OutputStreamWrapper> stream, Ptr<Ipv6> ipv6, uint32_t interface)
{
  EnableAsciiIpv6Internal (stream, std::string (), ipv6, interface, false);
}

//
// Public API
//
void 
AsciiTraceHelperForIpv6::EnableAsciiIpv6 (std::string prefix, std::string ipv6Name, uint32_t interface, bool explicitFilename)
{
  EnableAsciiIpv6Impl (Ptr<OutputStreamWrapper> (), prefix, ipv6Name, interface, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForIpv6::EnableAsciiIpv6 (Ptr<OutputStreamWrapper> stream, std::string ipv6Name, uint32_t interface)
{
  EnableAsciiIpv6Impl (stream, std::string (), ipv6Name, interface, false);
}

//
// Private API
//
void 
AsciiTraceHelperForIpv6::EnableAsciiIpv6Impl (
  Ptr<OutputStreamWrapper> stream, 
  std::string prefix, 
  std::string ipv6Name, 
  uint32_t interface,
  bool explicitFilename)
{
  Ptr<Ipv6> ipv6 = Names::Find<Ipv6> (ipv6Name);
  EnableAsciiIpv6Internal (stream, prefix, ipv6, interface, explicitFilename);
}

//
// Public API
//
void 
AsciiTraceHelperForIpv6::EnableAsciiIpv6 (std::string prefix, Ipv6InterfaceContainer c)
{
  EnableAsciiIpv6Impl (Ptr<OutputStreamWrapper> (), prefix, c);
}

//
// Public API
//
void 
AsciiTraceHelperForIpv6::EnableAsciiIpv6 (Ptr<OutputStreamWrapper> stream, Ipv6InterfaceContainer c)
{
  EnableAsciiIpv6Impl (stream, std::string (), c);
}

//
// Private API
//
void 
AsciiTraceHelperForIpv6::EnableAsciiIpv6Impl (Ptr<OutputStreamWrapper> stream, std::string prefix, Ipv6InterfaceContainer c)
{
  for (Ipv6InterfaceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      std::pair<Ptr<Ipv6>, uint32_t> pair = *i;
      EnableAsciiIpv6Internal (stream, prefix, pair.first, pair.second, false);
    }
}

//
// Public API
//
void
AsciiTraceHelperForIpv6::EnableAsciiIpv6 (std::string prefix, NodeContainer n)
{
  EnableAsciiIpv6Impl (Ptr<OutputStreamWrapper> (), prefix, n);
}

//
// Public API
//
void
AsciiTraceHelperForIpv6::EnableAsciiIpv6 (Ptr<OutputStreamWrapper> stream, NodeContainer n)
{
  EnableAsciiIpv6Impl (stream, std::string (), n);
}

//
// Private API
//
void
AsciiTraceHelperForIpv6::EnableAsciiIpv6Impl (Ptr<OutputStreamWrapper> stream, std::string prefix, NodeContainer n)
{
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
      if (ipv6)
        {
          for (uint32_t j = 0; j < ipv6->GetNInterfaces (); ++j)
            {
              EnableAsciiIpv6Internal (stream, prefix, ipv6, j, false);
            }
        }
    }
}

//
// Public API
//
void
AsciiTraceHelperForIpv6::EnableAsciiIpv6All (std::string prefix)
{
  EnableAsciiIpv6Impl (Ptr<OutputStreamWrapper> (), prefix, NodeContainer::GetGlobal ());
}

//
// Public API
//
void
AsciiTraceHelperForIpv6::EnableAsciiIpv6All (Ptr<OutputStreamWrapper> stream)
{
  EnableAsciiIpv6Impl (stream, std::string (), NodeContainer::GetGlobal ());
}

//
// Public API
//
void 
AsciiTraceHelperForIpv6::EnableAsciiIpv6 (Ptr<OutputStreamWrapper> stream, uint32_t nodeid, uint32_t interface)
{
  EnableAsciiIpv6Impl (stream, std::string (), nodeid, interface, false);
}

//
// Public API
//
void 
AsciiTraceHelperForIpv6::EnableAsciiIpv6 (std::string prefix, uint32_t nodeid, uint32_t interface, bool explicitFilename)
{
  EnableAsciiIpv6Impl (Ptr<OutputStreamWrapper> (), prefix, nodeid, interface, explicitFilename);
}

//
// Private API
//
void 
AsciiTraceHelperForIpv6::EnableAsciiIpv6Impl (
  Ptr<OutputStreamWrapper> stream, 
  std::string prefix, 
  uint32_t nodeid, 
  uint32_t interface,
  bool explicitFilename)
{
  NodeContainer n = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetId () != nodeid) 
        {
          continue;
        }

      Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
      if (ipv6)
        {
          EnableAsciiIpv6Internal (stream, prefix, ipv6, interface, explicitFilename);
        }

      return;
    }
}

} // namespace ns3

