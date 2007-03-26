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
#include "ns3/composite-trace-resolver.h"
#include "queue.h"

NS_DEBUG_COMPONENT_DEFINE ("Queue");

namespace ns3 {

Queue* Queue::defaultQueue = 0;

Queue::Queue() : 
  m_nBytes(0), 
  m_nTotalReceivedBytes(0),
  m_nPackets(0), 
  m_nTotalReceivedPackets(0),
  m_nTotalDroppedBytes(0),
  m_nTotalDroppedPackets(0)
{
  NS_DEBUG("Queue::Queue ()");
}

Queue::~Queue()
{
  NS_DEBUG("Queue::~Queue ()");
}

TraceResolver *
Queue::CreateTraceResolver (TraceContext const &context)
{
  CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
  resolver->Add ("enqueue", m_traceEnqueue, Queue::ENQUEUE);
  resolver->Add ("dequeue", m_traceDequeue, Queue::DEQUEUE);
  resolver->Add ("drop", m_traceDrop, Queue::DROP);
  return resolver;
}

bool 
Queue::Enqueue (const Packet& p)
{
  NS_DEBUG("Queue::Enqueue (" << &p << ")");

  NS_DEBUG("Queue::Enqueue (): m_traceEnqueue (p)");

  m_traceEnqueue (p);

  bool retval = DoEnqueue (p);
  if (retval)
    {
      m_nBytes += p.GetSize ();
      m_nPackets++;
    }
  return retval;
}

bool
Queue::Dequeue (Packet &p)
{
  NS_DEBUG("Queue::Dequeue (" << &p << ")");

  bool retval = DoDequeue (p);

  if (retval)
    {
      m_nBytes -= p.GetSize ();
      m_nPackets--;

      NS_ASSERT (m_nBytes >= 0);
      NS_ASSERT (m_nPackets >= 0);

      NS_DEBUG("Queue::Dequeue (): m_traceDequeue (p)");

      const Packet packet = p;
      m_traceDequeue (packet);
    }

  return retval;
}

void
Queue::DequeueAll (void)
{
  NS_DEBUG("Queue::DequeueAll ()");

  NS_ASSERT (!"Don't know what to do with dequeued packets!");
}

uint32_t 
Queue::GetNPackets (void)
{
  NS_DEBUG("Queue::GetNPackets () <= " << m_nPackets);

  return m_nPackets;
}

uint32_t
Queue::GetNBytes (void)
{
  NS_DEBUG("Queue::GetNBytes () <= " << m_nBytes);

  return m_nBytes;
}


bool
Queue::IsEmpty (void)
{
  NS_DEBUG("Queue::IsEmpty () <= " << (m_nPackets == 0));
  return m_nPackets == 0;
}

uint32_t
Queue::GetTotalReceivedBytes (void)
{
  NS_DEBUG("Queue::GetTotalReceivedBytes () <= " << m_nTotalReceivedBytes);

  return m_nTotalReceivedBytes;
}

uint32_t
Queue::GetTotalReceivedPackets (void)
{
  NS_DEBUG("Queue::GetTotalReceivedPackets () <= " << m_nTotalReceivedPackets);

  return m_nTotalReceivedPackets;
}

uint32_t
Queue:: GetTotalDroppedBytes (void)
{
  NS_DEBUG(
    "Queue::GetTotalDroppedBytes () <= " << m_nTotalDroppedBytes
    );
  return m_nTotalDroppedBytes;
}

uint32_t
Queue::GetTotalDroppedPackets (void)
{
  NS_DEBUG(
           "Queue::GetTotalDroppedPackets () <= " << m_nTotalDroppedPackets);

  return m_nTotalDroppedPackets;
}

void 
Queue::ResetStatistics (void)
{
  NS_DEBUG("Queue::ResetStatistics ()");

  m_nTotalReceivedBytes = 0;
  m_nTotalReceivedPackets = 0;
  m_nTotalDroppedBytes = 0;
  m_nTotalDroppedPackets = 0;
}

void
Queue::Drop (const Packet& p)
{
  NS_DEBUG("Queue::Drop (" << &p << ")");

  m_nTotalDroppedPackets++;
  m_nTotalDroppedBytes += p.GetSize ();

  NS_DEBUG("Queue::Drop (): m_traceDrop (p)");
  m_traceDrop (p);
}

// Static methods for managing default queue

// Set new default
void Queue::Default(const Queue& q)
{
  delete defaultQueue;      // delete previous (if any)
  defaultQueue = q.Copy();  // set new default
}

// Get current default
Queue& Queue::Default()
{
  // ! Need to schedule an "at end" event to delete the default
  return *defaultQueue;
}

}; // namespace ns3
