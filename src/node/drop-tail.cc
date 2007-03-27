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

NS_DEBUG_COMPONENT_DEFINE ("DropTailQueue");

namespace ns3 {

static class QueueStackInitializationClass {
public:
  QueueStackInitializationClass () {
    Queue::Default (DropTailQueue ());
  }
} queue_stack_initialization_class;


DropTailQueue::DropTailQueue () :
  Queue (),
  m_packets (),
  m_maxPackets(DTQ_NPACKETS_MAX_DEFAULT)
{
  NS_DEBUG("DropTailQueue::DropTailQueue ()");
}

DropTailQueue::~DropTailQueue ()
{
  NS_DEBUG("DropTailQueue::~DropTailQueue ()");
}

DropTailQueue* DropTailQueue::Copy() const
{
  return new DropTailQueue(*this);
}

void 
DropTailQueue::SetMaxPackets (uint32_t npackets)
{
  NS_DEBUG("DropTailQueue::SetMaxPackets (" << npackets << ")");

  m_maxPackets = npackets;
}

uint32_t 
DropTailQueue::GetMaxPackets (void)
{
  NS_DEBUG("DropTailQueue::GetMaxPackets () <= " << m_maxPackets);

  return m_maxPackets;
}

bool 
DropTailQueue::DoEnqueue (const Packet& p)
{
  NS_DEBUG("DropTailQueue::DoEnqueue (" << &p << ")");

  if (m_packets.size () >= m_maxPackets)
    {
      NS_DEBUG("DropTailQueue::DoEnqueue (): Queue full -- droppping pkt");
      Drop (p);
      return false;
    }

  m_packets.push(p);
  return true;
}

bool
DropTailQueue::DoDequeue (Packet& p)
{
  NS_DEBUG("DropTailQueue::DoDequeue (" << &p << ")");

  if (m_packets.empty()) 
    {
      NS_DEBUG("DropTailQueue::DoDequeue (): Queue empty");
      return false;
    }

  p = m_packets.front ();
  m_packets.pop ();

  NS_DEBUG("DropTailQueue::DoDequeue (): Popped " << &p << " <= true");

  return true;
}

bool
DropTailQueue::DoPeek (Packet& p)
{
  NS_DEBUG("DropTailQueue::DoPeek (" << &p << ")");

  if (m_packets.empty()) 
    {
      NS_DEBUG("DropTailQueue::DoPeek (): Queue empty");
      return false;
    }

  p = m_packets.front ();

  return true;
}

}; // namespace ns3
