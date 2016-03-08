/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DropTailQueue");

NS_OBJECT_ENSURE_REGISTERED (DropTailQueue);

TypeId DropTailQueue::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DropTailQueue")
    .SetParent<Queue> ()
    .SetGroupName ("Network")
    .AddConstructor<DropTailQueue> ()
  ;
  return tid;
}

DropTailQueue::DropTailQueue () :
  Queue (),
  m_packets ()
{
  NS_LOG_FUNCTION (this);
}

DropTailQueue::~DropTailQueue ()
{
  NS_LOG_FUNCTION (this);
}

bool 
DropTailQueue::DoEnqueue (Ptr<QueueItem> item)
{
  NS_LOG_FUNCTION (this << item);
  NS_ASSERT (m_packets.size () == GetNPackets ());

  m_packets.push (item);

  return true;
}

Ptr<QueueItem>
DropTailQueue::DoDequeue (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_packets.size () == GetNPackets ());

  Ptr<QueueItem> item = m_packets.front ();
  m_packets.pop ();

  NS_LOG_LOGIC ("Popped " << item);

  return item;
}

Ptr<const QueueItem>
DropTailQueue::DoPeek (void) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_packets.size () == GetNPackets ());

  return m_packets.front ();
}

} // namespace ns3

