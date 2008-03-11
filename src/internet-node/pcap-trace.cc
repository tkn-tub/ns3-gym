/* -*-	Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
#include "pcap-trace.h"

#include <sstream>

#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/pcap-writer.h"
#include "ns3/node-list.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/log.h"

#include "ipv4-l3-protocol.h"

NS_LOG_COMPONENT_DEFINE ("PcapTrace");

namespace ns3 {


PcapTrace::PcapTrace (std::string filename)
  : m_filename (filename)
{}
PcapTrace::~PcapTrace ()
{
  for (std::vector<Trace>::iterator i = m_traces.begin ();
       i != m_traces.end (); i++)
    {
      delete i->writer;
    }
}

void 
PcapTrace::TraceAllIp (void)
{
  Config::Connect ("/NodeList/*/$Ipv4L3Protocol/Tx",
                              MakeCallback (&PcapTrace::LogTxIp, this));
  Config::Connect ("/NodeList/*/$Ipv4L3Protocol/Rx",
                              MakeCallback (&PcapTrace::LogRxIp, this));
}

PcapWriter *
PcapTrace::GetStream (uint32_t nodeId, uint32_t interfaceId)
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
  PcapTrace::Trace trace;
  trace.nodeId = nodeId;
  trace.interfaceId = interfaceId;
  trace.writer = new PcapWriter ();
  std::ostringstream oss;
  oss << m_filename << "-" << nodeId << "-" << interfaceId;
  std::string filename = oss.str ();
  trace.writer->Open (filename);
  trace.writer->WriteIpHeader ();
  m_traces.push_back (trace);
  return trace.writer;
}

uint32_t
PcapTrace::GetNodeIndex (std::string context) const
{
  std::string::size_type pos;
  pos = context.find ("/NodeList/");
  NS_ASSERT (pos == 0);
  std::string::size_type afterNodeIndex = context.find ("/", 11);
  NS_ASSERT (afterNodeIndex != std::string::npos);
  std::string index = context.substr (10, afterNodeIndex - 10);
  NS_LOG_DEBUG ("index="<<index);
  std::istringstream iss;
  iss.str (index);
  uint32_t nodeIndex;
  iss >> nodeIndex;
  return nodeIndex;
}  

void 
PcapTrace::LogTxIp (std::string context, Ptr<const Packet> p, uint32_t interfaceIndex)
{
  PcapWriter *writer = GetStream (GetNodeIndex (context), interfaceIndex);
  writer->WritePacket (p);
}

void 
PcapTrace::LogRxIp (std::string context, Ptr<const Packet> p, uint32_t interfaceIndex)
{
  PcapWriter *writer = GetStream (GetNodeIndex (context), interfaceIndex);
  writer->WritePacket (p);
}


}//namespace ns3
