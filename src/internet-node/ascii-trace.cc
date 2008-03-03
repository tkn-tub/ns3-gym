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

#include "ns3/config.h"
#include "ns3/simulator.h"
#include "ns3/node.h"
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
  Config::ConnectWithContext ("/NodeList/*/DeviceList/*/TxQueue/Enqueue",
                              MakeCallback (&AsciiTrace::LogDevQueueEnqueue, this));
  Config::ConnectWithContext ("/NodeList/*/DeviceList/*/TxQueue/Dequeue",
                              MakeCallback (&AsciiTrace::LogDevQueueDequeue, this));
  Config::ConnectWithContext ("/NodeList/*/DeviceList/*/TxQueue/Drop",
                              MakeCallback (&AsciiTrace::LogDevQueueDrop, this));
}
void
AsciiTrace::TraceAllNetDeviceRx (void)
{
  Packet::EnableMetadata ();
  Config::ConnectWithContext ("/NodeList/*/DeviceList/*/Rx",
                              MakeCallback (&AsciiTrace::LogDevRx, this));
}

void 
AsciiTrace::LogDevQueueEnqueue (std::string context,
                                Ptr<const Packet> packet)
{
  m_os << "+ ";
  m_os << Simulator::Now ().GetSeconds () << " ";
  m_os << context;
  m_os << " pkt-uid=" << packet->GetUid () << " ";
  packet->Print (m_os);
  m_os << std::endl;
}

void 
AsciiTrace::LogDevQueueDequeue (std::string context,
                                Ptr<const Packet> packet)
{
  m_os << "- ";
  m_os << Simulator::Now ().GetSeconds () << " ";
  m_os << context;
  m_os << " pkt-uid=" << packet->GetUid () << " ";
  packet->Print (m_os);
  m_os << std::endl;
}

void 
AsciiTrace::LogDevQueueDrop (std::string context,
                             Ptr<const Packet> packet)
{
  m_os << "d ";
  m_os << Simulator::Now ().GetSeconds () << " ";
  m_os << context;
  m_os << " pkt-uid=" << packet->GetUid () << " ";
  packet->Print (m_os);
  m_os << std::endl;
}
void 
AsciiTrace::LogDevRx (std::string context,
                      Ptr<const Packet> p)
{
  m_os << "r " << Simulator::Now ().GetSeconds () << " ";
  m_os << context;
  m_os << " pkt-uid=" << p->GetUid () << " ";
  p->Print (m_os);
  m_os << std::endl;  
}

}//namespace ns3
