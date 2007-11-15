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

#include "ns3/log.h"
#include "drop-tail-queue.h"

NS_LOG_COMPONENT_DEFINE ("DropTailQueue");

namespace ns3 {

const ClassId DropTailQueue::cid = 
  MakeClassId<DropTailQueue> ("DropTailQueue", Queue::iid);


DropTailQueue::DropTailQueue () :
  Queue (),
  m_packets (),
  m_maxPackets(DTQ_NPACKETS_MAX_DEFAULT)
{
  NS_LOG_FUNCTION;
}

DropTailQueue::~DropTailQueue ()
{
  NS_LOG_FUNCTION;
}

void 
DropTailQueue::SetMaxPackets (uint32_t npackets)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << npackets);
  m_maxPackets = npackets;
}

uint32_t 
DropTailQueue::GetMaxPackets (void)
{
  NS_LOG_FUNCTION;
  NS_LOG_LOGIC ("returns " << m_maxPackets);
  return m_maxPackets;
}

bool 
DropTailQueue::DoEnqueue (const Packet& p)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << &p);

  if (m_packets.size () >= m_maxPackets)
    {
      NS_LOG_LOGIC ("Queue full -- droppping pkt");
      Drop (p);
      return false;
    }

  m_packets.push(p);
  return true;
}

bool
DropTailQueue::DoDequeue (Packet& p)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << &p);

  if (m_packets.empty()) 
    {
      NS_LOG_LOGIC ("Queue empty");
      return false;
    }

  p = m_packets.front ();
  m_packets.pop ();

  NS_LOG_LOGIC ("Popped " << &p);

  return true;
}

bool
DropTailQueue::DoPeek (Packet& p)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << &p);

  if (m_packets.empty()) 
    {
      NS_LOG_LOGIC ("Queue empty");
      return false;
    }

  p = m_packets.front ();

  return true;
}

}; // namespace ns3
