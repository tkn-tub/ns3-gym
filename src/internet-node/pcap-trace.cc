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

#include "ns3/node-list.h"
#include "ns3/trace-context.h"
#include "ns3/callback.h"
#include "ns3/pcap-writer.h"
#include "ns3/node-list.h"
#include "ns3/node.h"

#include "ipv4-l3-protocol.h"


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
  NodeList::Connect ("/nodes/*/ipv4/(tx|rx)",
                     MakeCallback (&PcapTrace::LogIp, this));
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

void 
PcapTrace::LogIp (TraceContext const &context, Packet const &p, uint32_t interfaceIndex)
{
  NodeListIndex nodeIndex;
  context.GetElement (nodeIndex);
  PcapWriter *writer = GetStream (nodeIndex.Get (), interfaceIndex);
  writer->WritePacket (p);
}


}//namespace ns3
