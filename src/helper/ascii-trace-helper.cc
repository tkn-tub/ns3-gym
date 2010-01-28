/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
#include <fstream>
#include <string>

#include "ns3/abort.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/names.h"
#include "ns3/net-device.h"
#include "ns3/simulator.h"

#include "ascii-trace-helper.h"

NS_LOG_COMPONENT_DEFINE("AsciiTraceHelper");

namespace ns3 {

AsciiTraceHelper::AsciiTraceHelper ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

AsciiTraceHelper::~AsciiTraceHelper ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ptr<OutputStreamObject>
AsciiTraceHelper::CreateFileStream (std::string filename, std::string filemode)
{
  NS_LOG_FUNCTION (filename << filemode);

  std::ofstream *ofstream = new std::ofstream;
  std::ios_base::openmode mode;

  if (filemode == "a")
    {
      mode = std::ios_base::out | std::ios_base::app;
    }
  else if (filemode == "w")
    {
      mode = std::ios_base::out | std::ios_base::trunc;
    }
  else
    {
      NS_ABORT_MSG ("AsciiTraceHelper::CreateFileStream(): Unexpected file mode");
    }

  ofstream->open (filename.c_str (), mode);
  NS_ABORT_MSG_UNLESS (ofstream->is_open (), "AsciiTraceHelper::CreateFileStream():  Unable to Open " << 
                       filename << " for mode " << filemode);
  
  Ptr<OutputStreamObject> streamObject = CreateObject<OutputStreamObject> ();
  streamObject->SetStream (ofstream);

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
  return streamObject;
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
AsciiTraceHelper::DefaultEnqueueSinkWithoutContext (Ptr<OutputStreamObject> file, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (file << p);
  *file->GetStream () << "+ " << Simulator::Now ().GetSeconds () << " " << *p << std::endl;
}

void
AsciiTraceHelper::DefaultEnqueueSinkWithContext (Ptr<OutputStreamObject> file, std::string context, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (file << p);
  *file->GetStream () << "+ " << Simulator::Now ().GetSeconds () << " " << context << " " << *p << std::endl;
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
AsciiTraceHelper::DefaultDropSinkWithoutContext (Ptr<OutputStreamObject> file, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (file << p);
  *file->GetStream () << "d " << Simulator::Now ().GetSeconds () << " " << *p << std::endl;
}

void
AsciiTraceHelper::DefaultDropSinkWithContext (Ptr<OutputStreamObject> file, std::string context, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (file << p);
  *file->GetStream () << "d " << Simulator::Now ().GetSeconds () << " " << context << " " << *p << std::endl;
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
AsciiTraceHelper::DefaultDequeueSinkWithoutContext (Ptr<OutputStreamObject> file, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (file << p);
  *file->GetStream () << "- " << Simulator::Now ().GetSeconds () << " " << *p << std::endl;
}

void
AsciiTraceHelper::DefaultDequeueSinkWithContext (Ptr<OutputStreamObject> file, std::string context, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (file << p);
  *file->GetStream () << "- " << Simulator::Now ().GetSeconds () << " " << context << " " << *p << std::endl;
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
AsciiTraceHelper::DefaultReceiveSinkWithoutContext (Ptr<OutputStreamObject> file, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (file << p);
  *file->GetStream () << "r " << Simulator::Now ().GetSeconds () << " " << *p << std::endl;
}

void
AsciiTraceHelper::DefaultReceiveSinkWithContext (Ptr<OutputStreamObject> file, std::string context, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (file << p);
  *file->GetStream () << "r " << Simulator::Now ().GetSeconds () << " " << context << " " << *p << std::endl;
}

//
// Public API
//
void 
AsciiTraceUserHelperForDevice::EnableAscii (std::string prefix, Ptr<NetDevice> nd)
{
  EnableAsciiInternal (Ptr<OutputStreamObject> (), prefix, nd);
}

//
// Public API
//
void 
AsciiTraceUserHelperForDevice::EnableAscii (Ptr<OutputStreamObject> stream, Ptr<NetDevice> nd)
{
  EnableAsciiInternal (stream, std::string (), nd);
}

//
// Public API
//
void 
AsciiTraceUserHelperForDevice::EnableAscii (std::string prefix, std::string ndName)
{
  EnableAsciiImpl (Ptr<OutputStreamObject> (), prefix, ndName);
}

//
// Public API
//
void 
AsciiTraceUserHelperForDevice::EnableAscii (Ptr<OutputStreamObject> stream, std::string ndName)
{
  EnableAsciiImpl (stream, std::string (), ndName);
}

//
// Private API
//
void 
AsciiTraceUserHelperForDevice::EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, std::string ndName)
{
  Ptr<NetDevice> nd = Names::Find<NetDevice> (ndName);
  EnableAsciiInternal (stream, prefix, nd);
}

//
// Public API
//
void 
AsciiTraceUserHelperForDevice::EnableAscii (std::string prefix, NetDeviceContainer d)
{
  EnableAsciiImpl (Ptr<OutputStreamObject> (), prefix, d);
}

//
// Public API
//
void 
AsciiTraceUserHelperForDevice::EnableAscii (Ptr<OutputStreamObject> stream, NetDeviceContainer d)
{
  EnableAsciiImpl (stream, std::string (), d);
}

//
// Private API
//
void 
AsciiTraceUserHelperForDevice::EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, NetDeviceContainer d)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnableAsciiInternal (stream, prefix, dev);
    }
}

//
// Public API
//
void
AsciiTraceUserHelperForDevice::EnableAscii (std::string prefix, NodeContainer n)
{
  EnableAsciiImpl (Ptr<OutputStreamObject> (), prefix, n);
}

//
// Public API
//
void
AsciiTraceUserHelperForDevice::EnableAscii (Ptr<OutputStreamObject> stream, NodeContainer n)
{
  EnableAsciiImpl (stream, std::string (), n);
}

//
// Private API
//
void
AsciiTraceUserHelperForDevice::EnableAsciiImpl (Ptr<OutputStreamObject> stream, std::string prefix, NodeContainer n)
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
AsciiTraceUserHelperForDevice::EnableAsciiAll (std::string prefix)
{
  EnableAsciiImpl (Ptr<OutputStreamObject> (), prefix, NodeContainer::GetGlobal ());
}

//
// Public API
//
void
AsciiTraceUserHelperForDevice::EnableAsciiAll (Ptr<OutputStreamObject> stream)
{
  EnableAsciiImpl (stream, std::string (), NodeContainer::GetGlobal ());
}

//
// Public API
//
void 
AsciiTraceUserHelperForDevice::EnableAscii (Ptr<OutputStreamObject> stream, uint32_t nodeid, uint32_t deviceid)
{
  EnableAsciiImpl (stream, std::string (), nodeid, deviceid);
}

//
// Public API
//
void 
AsciiTraceUserHelperForDevice::EnableAscii (std::string prefix, uint32_t nodeid, uint32_t deviceid)
{
  EnableAsciiImpl (Ptr<OutputStreamObject> (), prefix, nodeid, deviceid);
}

//
// Private API
//
void 
AsciiTraceUserHelperForDevice::EnableAsciiImpl (
  Ptr<OutputStreamObject> stream, 
  std::string prefix, 
  uint32_t nodeid, 
  uint32_t deviceid)
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
                       "AsciiTraceUserHelperForDevice::EnableAscii(): Unknown deviceid = " << deviceid);

      Ptr<NetDevice> nd = node->GetDevice (deviceid);

      EnableAsciiInternal (stream, prefix, nd);
      return;
    }
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4 (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface)
{
  EnableAsciiIpv4Internal (Ptr<OutputStreamObject> (), prefix, ipv4, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4 (Ptr<OutputStreamObject> stream, Ptr<Ipv4> ipv4, uint32_t interface)
{
  EnableAsciiIpv4Internal (stream, std::string (), ipv4, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4 (std::string prefix, std::string ipv4Name, uint32_t interface)
{
  EnableAsciiIpv4Impl (Ptr<OutputStreamObject> (), prefix, ipv4Name, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4 (Ptr<OutputStreamObject> stream, std::string ipv4Name, uint32_t interface)
{
  EnableAsciiIpv4Impl (stream, std::string (), ipv4Name, interface);
}

//
// Private API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4Impl (
  Ptr<OutputStreamObject> stream, 
  std::string prefix, 
  std::string ipv4Name, 
  uint32_t interface)
{
  Ptr<Ipv4> ipv4 = Names::Find<Ipv4> (ipv4Name);
  EnableAsciiIpv4Internal (stream, prefix, ipv4, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4 (std::string prefix, Ipv4InterfaceContainer c)
{
  EnableAsciiIpv4Impl (Ptr<OutputStreamObject> (), prefix, c);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4 (Ptr<OutputStreamObject> stream, Ipv4InterfaceContainer c)
{
  EnableAsciiIpv4Impl (stream, std::string (), c);
}

//
// Private API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4Impl (Ptr<OutputStreamObject> stream, std::string prefix, Ipv4InterfaceContainer c)
{
  for (Ipv4InterfaceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      std::pair<Ptr<Ipv4>, uint32_t> pair = *i;
      EnableAsciiIpv4Internal (stream, prefix, pair.first, pair.second);
    }
}

//
// Public API
//
void
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4 (std::string prefix, NodeContainer n)
{
  EnableAsciiIpv4Impl (Ptr<OutputStreamObject> (), prefix, n);
}

//
// Public API
//
void
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4 (Ptr<OutputStreamObject> stream, NodeContainer n)
{
  EnableAsciiIpv4Impl (stream, std::string (), n);
}

//
// Private API
//
void
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4Impl (Ptr<OutputStreamObject> stream, std::string prefix, NodeContainer n)
{
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      if (ipv4)
        {
          for (uint32_t j = 0; j < ipv4->GetNInterfaces (); ++j)
            {
              EnableAsciiIpv4Internal (stream, prefix, ipv4, j);
            }
        }
    }
}

//
// Public API
//
void
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4All (std::string prefix)
{
  EnableAsciiIpv4Impl (Ptr<OutputStreamObject> (), prefix, NodeContainer::GetGlobal ());
}

//
// Public API
//
void
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4All (Ptr<OutputStreamObject> stream)
{
  EnableAsciiIpv4Impl (stream, std::string (), NodeContainer::GetGlobal ());
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4 (Ptr<OutputStreamObject> stream, uint32_t nodeid, uint32_t interface)
{
  EnableAsciiIpv4Impl (stream, std::string (), nodeid, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4 (std::string prefix, uint32_t nodeid, uint32_t interface)
{
  EnableAsciiIpv4Impl (Ptr<OutputStreamObject> (), prefix, nodeid, interface);
}

//
// Private API
//
void 
AsciiTraceUserHelperForIpv4::EnableAsciiIpv4Impl (
  Ptr<OutputStreamObject> stream, 
  std::string prefix, 
  uint32_t nodeid, 
  uint32_t interface)
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
          EnableAsciiIpv4Internal (stream, prefix, ipv4, interface);
        }

      return;
    }
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6 (std::string prefix, Ptr<Ipv6> ipv6, uint32_t interface)
{
  EnableAsciiIpv6Internal (Ptr<OutputStreamObject> (), prefix, ipv6, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6 (Ptr<OutputStreamObject> stream, Ptr<Ipv6> ipv6, uint32_t interface)
{
  EnableAsciiIpv6Internal (stream, std::string (), ipv6, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6 (std::string prefix, std::string ipv6Name, uint32_t interface)
{
  EnableAsciiIpv6Impl (Ptr<OutputStreamObject> (), prefix, ipv6Name, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6 (Ptr<OutputStreamObject> stream, std::string ipv6Name, uint32_t interface)
{
  EnableAsciiIpv6Impl (stream, std::string (), ipv6Name, interface);
}

//
// Private API
//
void 
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6Impl (
  Ptr<OutputStreamObject> stream, 
  std::string prefix, 
  std::string ipv6Name, 
  uint32_t interface)
{
  Ptr<Ipv6> ipv6 = Names::Find<Ipv6> (ipv6Name);
  EnableAsciiIpv6Internal (stream, prefix, ipv6, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6 (std::string prefix, Ipv6InterfaceContainer c)
{
  EnableAsciiIpv6Impl (Ptr<OutputStreamObject> (), prefix, c);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6 (Ptr<OutputStreamObject> stream, Ipv6InterfaceContainer c)
{
  EnableAsciiIpv6Impl (stream, std::string (), c);
}

//
// Private API
//
void 
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6Impl (Ptr<OutputStreamObject> stream, std::string prefix, Ipv6InterfaceContainer c)
{
  for (Ipv6InterfaceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      std::pair<Ptr<Ipv6>, uint32_t> pair = *i;
      EnableAsciiIpv6Internal (stream, prefix, pair.first, pair.second);
    }
}

//
// Public API
//
void
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6 (std::string prefix, NodeContainer n)
{
  EnableAsciiIpv6Impl (Ptr<OutputStreamObject> (), prefix, n);
}

//
// Public API
//
void
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6 (Ptr<OutputStreamObject> stream, NodeContainer n)
{
  EnableAsciiIpv6Impl (stream, std::string (), n);
}

//
// Private API
//
void
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6Impl (Ptr<OutputStreamObject> stream, std::string prefix, NodeContainer n)
{
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
      if (ipv6)
        {
          for (uint32_t j = 0; j < ipv6->GetNInterfaces (); ++j)
            {
              EnableAsciiIpv6Internal (stream, prefix, ipv6, j);
            }
        }
    }
}

//
// Public API
//
void
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6All (std::string prefix)
{
  EnableAsciiIpv6Impl (Ptr<OutputStreamObject> (), prefix, NodeContainer::GetGlobal ());
}

//
// Public API
//
void
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6All (Ptr<OutputStreamObject> stream)
{
  EnableAsciiIpv6Impl (stream, std::string (), NodeContainer::GetGlobal ());
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6 (Ptr<OutputStreamObject> stream, uint32_t nodeid, uint32_t interface)
{
  EnableAsciiIpv6Impl (stream, std::string (), nodeid, interface);
}

//
// Public API
//
void 
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6 (std::string prefix, uint32_t nodeid, uint32_t interface)
{
  EnableAsciiIpv6Impl (Ptr<OutputStreamObject> (), prefix, nodeid, interface);
}

//
// Private API
//
void 
AsciiTraceUserHelperForIpv6::EnableAsciiIpv6Impl (
  Ptr<OutputStreamObject> stream, 
  std::string prefix, 
  uint32_t nodeid, 
  uint32_t interface)
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
          EnableAsciiIpv6Internal (stream, prefix, ipv6, interface);
        }

      return;
    }
}

} // namespace ns3

