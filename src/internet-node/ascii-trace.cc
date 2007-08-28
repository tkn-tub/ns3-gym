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
#include "ascii-trace.h"

#include "ns3/trace-context.h"
#include "ns3/simulator.h"
#include "ns3/node.h"
#include "ns3/node-list.h"
#include "ns3/packet.h"
#include "ns3/queue.h"

namespace ns3 {

AsciiTrace::AsciiTrace (std::string filename)
{
  m_os.open (filename.c_str ());
}
AsciiTrace::~AsciiTrace ()
{
  m_os.close ();
}
void
AsciiTrace::TraceAllQueues (void)
{
  Packet::EnableMetadata ();
  NodeList::Connect ("/nodes/*/devices/*/queue/enqueue",
                      MakeCallback (&AsciiTrace::LogDevQueueEnqueue, this));
  NodeList::Connect ("/nodes/*/devices/*/queue/dequeue",
                      MakeCallback (&AsciiTrace::LogDevQueueDequeue, this));
  NodeList::Connect ("/nodes/*/devices/*/queue/drop",
                      MakeCallback (&AsciiTrace::LogDevQueueDrop, this));
}
void
AsciiTrace::TraceAllNetDeviceRx (void)
{
  Packet::EnableMetadata ();
  NodeList::Connect ("/nodes/*/devices/*/rx",
                     MakeCallback (&AsciiTrace::LogDevRx, this));
}

void 
AsciiTrace::LogDevQueueEnqueue (TraceContext const &context, 
  Packet const &packet)
{
  m_os << "+ ";
  m_os << Simulator::Now ().GetSeconds () << " ";
  context.Print (m_os);
  m_os << " pkt-uid=" << packet.GetUid () << " ";
  packet.Print (m_os);
  m_os << std::endl;
}

void 
AsciiTrace::LogDevQueueDequeue (TraceContext const &context, 
  Packet const &packet)
{
  m_os << "- ";
  m_os << Simulator::Now ().GetSeconds () << " ";
  context.Print (m_os);
  m_os << " pkt-uid=" << packet.GetUid () << " ";
  packet.Print (m_os);
  m_os << std::endl;
}

void 
AsciiTrace::LogDevQueueDrop (TraceContext const &context, 
  Packet const &packet)
{
  m_os << "d ";
  m_os << Simulator::Now ().GetSeconds () << " ";
  context.Print (m_os);
  m_os << " pkt-uid=" << packet.GetUid () << " ";
  packet.Print (m_os);
  m_os << std::endl;
}
void 
AsciiTrace::LogDevRx (TraceContext const &context, Packet &p)
{
  m_os << "r " << Simulator::Now ().GetSeconds () << " ";
  context.Print (m_os);
  m_os << " pkt-uid=" << p.GetUid () << " ";
  p.Print (m_os);
  m_os << std::endl;  
}

}//namespace ns3
