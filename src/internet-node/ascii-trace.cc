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
#include "ns3/trace-root.h"
#include "ns3/simulator.h"
#include "ns3/node.h"
#include "ns3/queue.h"
#include "ns3/node-list.h"

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
  TraceRoot::Connect ("/nodes/*/devices/*/queue/*",
                      MakeCallback (&AsciiTrace::LogDevQueue, this));
}
void
AsciiTrace::TraceAllNetDeviceRx (void)
{
  Packet::EnableMetadata ();
  TraceRoot::Connect ("/nodes/*/devices/*/rx",
                      MakeCallback (&AsciiTrace::LogDevRx, this));
}

void 
AsciiTrace::LogDevQueue (TraceContext const &context, Packet const &packet)
{
  enum Queue::TraceType type;
  context.Get (type);
  switch (type) 
    {
    case Queue::ENQUEUE:
      m_os << "+ ";
      break;
    case Queue::DEQUEUE:
      m_os << "- ";
      break;
    case Queue::DROP:
      m_os << "d ";
      break;
    }
  m_os << Simulator::Now ().GetSeconds () << " ";
  NodeList::NodeIndex nodeIndex;
  context.Get (nodeIndex);
  m_os << "node=" << NodeList::GetNode (nodeIndex)->GetId () << " ";
  Node::NetDeviceIndex deviceIndex;
  context.Get (deviceIndex);
  m_os << "device=" << deviceIndex << " ";
  m_os << "pkt-uid=" << packet.GetUid () << " ";
  packet.Print (m_os);
  m_os << std::endl;
}
void 
AsciiTrace::LogDevRx (TraceContext const &context, Packet &p)
{
  m_os << "r " << Simulator::Now ().GetSeconds () << " ";
  NodeList::NodeIndex nodeIndex;
  context.Get (nodeIndex);
  m_os << "node=" << NodeList::GetNode (nodeIndex)->GetId () << " ";
  Node::NetDeviceIndex deviceIndex;
  context.Get (deviceIndex);
  m_os << "device=" << deviceIndex << " ";
  m_os << "pkt-uid=" << p.GetUid () << " ";
  p.Print (m_os);
  m_os << std::endl;  
}

}//namespace ns3
