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
#include "queue.h"

namespace ns3 {

namespace {
  int qDebug = 1;
}

Queue::Queue() : 
  m_nBytes(0), 
  m_nTotalReceivedBytes(0),
  m_nPackets(0), 
  m_nTotalReceivedPackets(0),
  m_nTotalDroppedBytes(0),
  m_nTotalDroppedPackets(0)
{
  NS3_TRACE(qDebug, "Queue::Queue ()")
}

Queue::~Queue()
{
  NS3_TRACE(qDebug, "Queue::~Queue ()")
}

  bool 
Queue::Enque (const Packet& p)
{
  NS3_TRACE(qDebug, "Queue::Enque (" << &p << ")")

  NS3_TRACE(qDebug, "Queue::Enque (): m_traceEnque (p)")
  m_traceEnque ("+ <timestamp> ", p);

  bool retval = DoEnque (p);
  if (retval)
    {
      m_nBytes += p.GetSize ();
      m_nPackets++;
    }
  return retval;
}

  bool
Queue::Deque (Packet &p)
{
  NS3_TRACE(qDebug, "Queue::Deque (" << &p << ")")

  bool retval = DoDeque (p);

  if (retval)
    {
      m_nBytes -= p.GetSize ();
      m_nPackets--;

      assert(m_nBytes >= 0);
      assert(m_nPackets >= 0);

      NS3_TRACE(qDebug, "Queue::Deque (): m_traceDeque (p)")
      m_traceDeque ("+ <timestamp> ", static_cast<const Packet &>(p));
    }

  return retval;
}

  void
Queue::DequeAll (void)
{
  NS3_TRACE(qDebug, "Queue::DequeAll ()")

  assert (!"Don't know what to do with dequeued packets!");
}

  uint32_t 
Queue::GetNPackets (void)
{
  NS3_TRACE(qDebug, "Queue::GetNPackets () <= " << m_nPackets)

  return m_nPackets;
}

  uint32_t
Queue::GetNBytes (void)
{
  NS3_TRACE(qDebug, "Queue::GetNBytes () <= " << m_nBytes)

  return m_nBytes;
}


  bool
Queue::IsEmpty (void)
{
  NS3_TRACE(qDebug, "Queue::IsEmpty () <= " << (m_nPackets == 0))
  return m_nPackets == 0;
}

  void
Queue::RegisterTraces (TraceContainer &container)
{
  NS3_TRACE(qDebug, "Queue::RegisterTraces (" << &container << ")")

  container.RegisterCallback ("Queue::Enque", &m_traceEnque);
  container.RegisterCallback ("Queue::Deque", &m_traceDeque);
  container.RegisterCallback ("Queue::Drop", &m_traceDrop);
}

  uint32_t
Queue::GetTotalReceivedBytes (void)
{
  NS3_TRACE(qDebug, 
    "Queue::GetTotalReceivedBytes () <= " << m_nTotalReceivedBytes)

  return m_nTotalReceivedBytes;
}

  uint32_t
Queue::GetTotalReceivedPackets (void)
{
  NS3_TRACE(qDebug, 
    "Queue::GetTotalReceivedPackets () <= " << m_nTotalReceivedPackets)

  return m_nTotalReceivedPackets;
}

  uint32_t
Queue:: GetTotalDroppedBytes (void)
{
  NS3_TRACE(qDebug, 
    "Queue::GetTotalDroppedBytes () <= " << m_nTotalDroppedBytes
)
  return m_nTotalDroppedBytes;
}

  uint32_t
Queue::GetTotalDroppedPackets (void)
{
  NS3_TRACE(qDebug, 
    "Queue::GetTotalDroppedPackets () <= " << m_nTotalDroppedPackets)

  return m_nTotalDroppedPackets;
}

  void 
Queue::ResetStatistics (void)
{
  NS3_TRACE(qDebug, "Queue::ResetStatistics ()")

  m_nTotalReceivedBytes = 0;
  m_nTotalReceivedPackets = 0;
  m_nTotalDroppedBytes = 0;
  m_nTotalDroppedPackets = 0;
}

  void
Queue::Drop (const Packet& p)
{
  NS3_TRACE(qDebug, "Queue::Drop (" << &p << ")")

  m_nTotalDroppedPackets++;
  m_nTotalDroppedBytes += p.GetSize ();

  NS3_TRACE(qDebug, "Queue::Drop (): m_traceDrop (p)")
  m_traceEnque ("d <timestamp> ", p);
}

}; // namespace ns3
