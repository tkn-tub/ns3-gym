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
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/ipv4.h"
#include "internet-stack-helper.h"
#include "ns3/internet-stack.h"
#include "ns3/packet-socket-factory.h"
#include "ns3/config.h"

namespace ns3 {

std::vector<InternetStackHelper::Trace> InternetStackHelper::m_traces;
std::string InternetStackHelper::m_pcapBaseFilename;

void 
InternetStackHelper::Install (NodeContainer c)
{
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetObject<Ipv4> () != 0)
        {
          NS_FATAL_ERROR ("InternetStackHelper::Install(): Aggregating " 
             "an InternetStack to a node with an existing Ipv4 object");
          return;
        } 
      AddInternetStack (node);
      Ptr<PacketSocketFactory> factory = CreateObject<PacketSocketFactory> ();
      node->AggregateObject (factory);
    }
}

void
InternetStackHelper::EnablePcap (std::string filename)
{
  InternetStackHelper::m_pcapBaseFilename = filename;
  Config::Connect ("/NodeList/*/$ns3::Ipv4L3Protocol/Tx",
                              MakeCallback (&InternetStackHelper::LogTxIp));
  Config::Connect ("/NodeList/*/$ns3::Ipv4L3Protocol/Rx",
                              MakeCallback (&InternetStackHelper::LogRxIp));
}

uint32_t
InternetStackHelper::GetNodeIndex (std::string context)
{
  std::string::size_type pos;
  pos = context.find ("/NodeList/");
  NS_ASSERT (pos == 0);
  std::string::size_type afterNodeIndex = context.find ("/", 11);
  NS_ASSERT (afterNodeIndex != std::string::npos);
  std::string index = context.substr (10, afterNodeIndex - 10);
  std::istringstream iss;
  iss.str (index);
  uint32_t nodeIndex;
  iss >> nodeIndex;
  return nodeIndex;
}

void
InternetStackHelper::LogTxIp (std::string context, Ptr<const Packet> packet, uint32_t interfaceIndex)
{
  Ptr<PcapWriter> writer = InternetStackHelper::GetStream (GetNodeIndex (context), interfaceIndex);
  writer->WritePacket (packet);
}

void
InternetStackHelper::LogRxIp (std::string context, Ptr<const Packet> packet, uint32_t interfaceIndex)
{
  Ptr<PcapWriter> writer = InternetStackHelper::GetStream (GetNodeIndex (context), interfaceIndex);
  writer->WritePacket (packet);
}

Ptr<PcapWriter>
InternetStackHelper::GetStream (uint32_t nodeId, uint32_t interfaceId)
{
  for (std::vector<Trace>::iterator i = m_traces.begin ();
       i != m_traces.end (); i++)
  {
    if (i->nodeId == nodeId &&
        i->interfaceId == interfaceId)
    {
      return i->writer;
    }
  }
  InternetStackHelper::Trace trace;
  trace.nodeId = nodeId;
  trace.interfaceId = interfaceId;
  trace.writer = Create<PcapWriter> ();
  std::ostringstream oss;
  oss << m_pcapBaseFilename << "-" << nodeId << "-" << interfaceId << ".pcap";
  trace.writer->Open (oss.str ());
  trace.writer->WriteIpHeader ();
  m_traces.push_back (trace);
  return trace.writer;
}

} // namespace ns3
