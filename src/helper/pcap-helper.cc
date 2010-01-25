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
#include <stdint.h>
#include <string>

#include "ns3/abort.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/names.h"
#include "ns3/net-device.h"
#include "ns3/pcap-file-object.h"

#include "pcap-helper.h"

NS_LOG_COMPONENT_DEFINE("PcapHelper");

namespace ns3 {

PcapHelper::PcapHelper ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

PcapHelper::~PcapHelper ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Ptr<PcapFileObject>
PcapHelper::CreateFile (
  std::string filename, 
  std::string filemode,  
  uint32_t    dataLinkType, 
  uint32_t    snapLen, 
  int32_t     tzCorrection)
{
  NS_LOG_FUNCTION (filename << filemode << dataLinkType << snapLen << tzCorrection);

  Ptr<PcapFileObject> file = CreateObject<PcapFileObject> ();
  bool err = file->Open (filename, filemode);
  NS_ABORT_MSG_IF (err, "Unable to Open " << filename << " for mode " << filemode);

  err = file->Init (dataLinkType, snapLen, tzCorrection);
  NS_ABORT_MSG_IF (err, "Unable to Init " << filename);

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
PcapHelper::GetFilenameFromInterfacePair (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface, bool useObjectNames)
{
  NS_LOG_FUNCTION (prefix << ipv4 << interface << useObjectNames);
  NS_ABORT_MSG_UNLESS (prefix.size (), "Empty prefix string");

  std::ostringstream oss;
  oss << prefix << "-";

  std::string ipv4name;
  std::string nodename;

  Ptr<Node> node = ipv4->GetObject<Node> ();

  if (useObjectNames)
    {
      ipv4name = Names::FindName (ipv4);
      nodename = Names::FindName (node);
    }

  if (ipv4name.size ())
    {
      oss << ipv4name;
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
PcapHelper::DefaultSink (Ptr<PcapFileObject> file, Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (file << p);
  file->Write(Simulator::Now(), p);
}

void 
PcapUserHelperForDevice::EnablePcap (std::string prefix, Ptr<NetDevice> nd, bool promiscuous)
{
  EnablePcapInternal (prefix, nd, promiscuous);
}

void 
PcapUserHelperForDevice::EnablePcap (std::string prefix, std::string ndName, bool promiscuous)
{
  Ptr<NetDevice> nd = Names::Find<NetDevice> (ndName);
  EnablePcap (prefix, nd, promiscuous);
}

void 
PcapUserHelperForDevice::EnablePcap (std::string prefix, NetDeviceContainer d, bool promiscuous)
{
  for (NetDeviceContainer::Iterator i = d.Begin (); i != d.End (); ++i)
    {
      Ptr<NetDevice> dev = *i;
      EnablePcap (prefix, dev, promiscuous);
    }
}

void
PcapUserHelperForDevice::EnablePcap (std::string prefix, NodeContainer n, bool promiscuous)
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
PcapUserHelperForDevice::EnablePcapAll (std::string prefix, bool promiscuous)
{
  EnablePcap (prefix, NodeContainer::GetGlobal (), promiscuous);
}

void 
PcapUserHelperForDevice::EnablePcap (std::string prefix, uint32_t nodeid, uint32_t deviceid, bool promiscuous)
{
  NodeContainer n = NodeContainer::GetGlobal ();

  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetId () != nodeid) 
        {
          continue;
        }
      
      NS_ABORT_MSG_IF (deviceid >= node->GetNDevices (), "PcapUserHelperForDevice::EnablePcap(): Unknown deviceid = " 
                       << deviceid);
      Ptr<NetDevice> nd = node->GetDevice (deviceid);
      EnablePcap (prefix, nd, promiscuous);
      return;
    }
}

void 
PcapUserHelperForIpv4::EnablePcap (std::string prefix, Ptr<Ipv4> ipv4, uint32_t interface)
{
  EnablePcapInternal (prefix, ipv4, interface);
}

void 
PcapUserHelperForIpv4::EnablePcap (std::string prefix, std::string ipv4Name, uint32_t interface)
{
  Ptr<Ipv4> ipv4 = Names::Find<Ipv4> (ipv4Name);
  EnablePcap (prefix, ipv4, interface);
}

void 
PcapUserHelperForIpv4::EnablePcap (std::string prefix, Ipv4InterfaceContainer c)
{
  for (Ipv4InterfaceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      std::pair<Ptr<Ipv4>, uint32_t> pair = *i;
      EnablePcap (prefix, pair.first, pair.second);
    }
}

void
PcapUserHelperForIpv4::EnablePcap (std::string prefix, NodeContainer n)
{
  for (NodeContainer::Iterator i = n.Begin (); i != n.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      if (ipv4)
        {
          for (uint32_t j = 0; j < ipv4->GetNInterfaces (); ++j)
            {
              EnablePcap (prefix, ipv4, j);
            }
        }
    }
}

void
PcapUserHelperForIpv4::EnablePcapAll (std::string prefix)
{
  EnablePcap (prefix, NodeContainer::GetGlobal ());
}

void 
PcapUserHelperForIpv4::EnablePcap (std::string prefix, uint32_t nodeid, uint32_t interface)
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
          EnablePcap (prefix, ipv4, interface);
        }
      return;
    }
}

} // namespace ns3

