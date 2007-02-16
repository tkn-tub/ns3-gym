/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 */

#include "ns3/debug.h"
#include "drop-tail.h"

namespace ns3 {

namespace {
  int dtqDebug = 0;
}

DropTailQueue::DropTailQueue () :
    m_packets (),
    m_maxPackets(DTQ_NPACKETS_MAX_DEFAULT)
{
  NS3_TRACE(dtqDebug, 
    "DropTailQueue::DropTailQueue ()")
}

DropTailQueue::DropTailQueue (TraceContainer &traceContainer) :
    m_packets(),
    m_maxPackets(DTQ_NPACKETS_MAX_DEFAULT)
{
  NS3_TRACE(dtqDebug, 
    "DropTailQueue::DropTailQueue (" << &traceContainer << ")")

  RegisterTraces(traceContainer);
}

DropTailQueue::~DropTailQueue ()
{
  NS3_TRACE(dtqDebug, 
    "DropTailQueue::~DropTailQueue ()")
}

  void 
DropTailQueue::SetMaxPackets (uint32_t npackets)
{
  NS3_TRACE(dtqDebug, 
    "DropTailQueue::SetMaxPackets (" << npackets << ")")

  m_maxPackets = npackets;
}

  uint32_t 
DropTailQueue::GetMaxPackets (void)
{
  NS3_TRACE(dtqDebug, 
    "DropTailQueue::GetMaxPackets () <= " << m_maxPackets)

  return m_maxPackets;
}

  bool 
DropTailQueue::DoEnque (const Packet& p)
{
  NS3_TRACE(dtqDebug, 
    "DropTailQueue::DoEnque (" << &p << ")")

  if (m_nPackets >= m_maxPackets)
    {
      NS3_TRACE(dtqDebug, 
        "DropTailQueue::DoEnque (): Queue full -- droppping pkt")
      Drop (p);
      return false;
    }

  m_packets.push(p);
  return true;
}

  bool
DropTailQueue::DoDeque (Packet& p)
{
  NS3_TRACE(dtqDebug, 
    "DropTailQueue::DoDeque (" << &p << ")")

  if (m_packets.empty()) 
    {
      NS3_TRACE(dtqDebug, 
        "DropTailQueue::DoDeque (): Queue empty")
      return false;
    }

  p = m_packets.front ();
  m_packets.pop ();

  NS3_TRACE(dtqDebug, 
    "DropTailQueue::DoDeque (): Popped " << &p << " <= true")

  return true;
}

}; // namespace ns3
