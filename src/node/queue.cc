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
#include "ns3/composite-trace-resolver.h"
#include "ns3/default-value.h"
#include "ns3/component-manager.h"
#include "queue.h"

NS_LOG_COMPONENT_DEFINE ("Queue");

namespace ns3 {

const InterfaceId Queue::iid = MakeInterfaceId ("Queue", Object::iid);
static ClassIdDefaultValue g_classIdDefaultValue ("Queue", "Packet Queue",
                                                  Queue::iid, "DropTailQueue");


std::string 
QueueTraceType::GetTypeName (void) const
{
  NS_LOG_FUNCTION;
  return "ns3::QueueTraceType";
}

uint16_t 
QueueTraceType::GetUid (void)
{
  NS_LOG_FUNCTION;
  static uint16_t uid = AllocateUid<QueueTraceType> ("QueueTraceType");
  return uid;
}

QueueTraceType::QueueTraceType ()
  : m_type (QueueTraceType::ENQUEUE)
{
  NS_LOG_FUNCTION;
}

QueueTraceType::QueueTraceType (enum Type type)
  : m_type (type)
{
  NS_LOG_FUNCTION;
}

bool 
QueueTraceType::IsEnqueue (void) const
{
  NS_LOG_FUNCTION;
  return m_type == ENQUEUE;
}

bool 
QueueTraceType::IsDequeue (void) const
{
  NS_LOG_FUNCTION;
  return m_type == DEQUEUE;
}

bool 
QueueTraceType::IsDrop (void) const
{
  NS_LOG_FUNCTION;
  return m_type == DROP;
}

void 
QueueTraceType::Print (std::ostream &os) const
{
  os << "queue-";
  switch (m_type) {
  case QueueTraceType::ENQUEUE:
    os << "enqueue";
    break;
  case QueueTraceType::DEQUEUE:
    os << "dequeue";
    break;
  case QueueTraceType::DROP:
    os << "drop";
    break;
  }
}

Queue::Queue() : 
  m_nBytes(0), 
  m_nTotalReceivedBytes(0),
  m_nPackets(0), 
  m_nTotalReceivedPackets(0),
  m_nTotalDroppedBytes(0),
  m_nTotalDroppedPackets(0)
{
  NS_LOG_FUNCTION;
  SetInterfaceId (Queue::iid);
}

Queue::~Queue()
{
  NS_LOG_FUNCTION;
}

Ptr<TraceResolver>
Queue::GetTraceResolver (void) const
{
  NS_LOG_FUNCTION;
  Ptr<CompositeTraceResolver> resolver = Create<CompositeTraceResolver> ();
  resolver->AddSource ("enqueue", 
                       TraceDoc ("store packet in queue",
                                 "const Packet &", "packet queued"),
                       m_traceEnqueue, QueueTraceType (QueueTraceType::ENQUEUE));
  resolver->AddSource ("dequeue", 
                       TraceDoc ("remove packet from queue",
                                 "const Packet &", "packet dequeued"),
                       m_traceDequeue, QueueTraceType (QueueTraceType::DEQUEUE));
  resolver->AddSource ("drop", 
                       TraceDoc ("drop packet from queue", 
                                 "const Packet &", "packet dropped"),
                       m_traceDrop, QueueTraceType (QueueTraceType::DROP));
  resolver->SetParentResolver (Object::GetTraceResolver ());
  return resolver;
}

bool 
Queue::Enqueue (const Packet& p)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << &p);
  NS_LOG_LOGIC ("m_traceEnqueue (p)");

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
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << &p);

  bool retval = DoDequeue (p);

  if (retval)
    {
      m_nBytes -= p.GetSize ();
      m_nPackets--;

      NS_ASSERT (m_nBytes >= 0);
      NS_ASSERT (m_nPackets >= 0);

      NS_LOG_LOGIC("m_traceDequeue (p)");

      const Packet packet = p;
      m_traceDequeue (packet);
    }

  return retval;
}

void
Queue::DequeueAll (void)
{
  NS_LOG_FUNCTION;
  NS_ASSERT_MSG (0, "Don't know what to do with dequeued packets!");
}

bool
Queue::Peek (Packet &p)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << &p);
  return DoPeek (p);
}


uint32_t 
Queue::GetNPackets (void)
{
  NS_LOG_FUNCTION;
  NS_LOG_LOGIC ("returns " << m_nPackets);
  return m_nPackets;
}

uint32_t
Queue::GetNBytes (void)
{
  NS_LOG_FUNCTION;
  NS_LOG_LOGIC (" returns " << m_nBytes);
  return m_nBytes;
}

bool
Queue::IsEmpty (void)
{
  NS_LOG_FUNCTION;
  NS_LOG_LOGIC ("returns " << (m_nPackets == 0));
  return m_nPackets == 0;
}

uint32_t
Queue::GetTotalReceivedBytes (void)
{
  NS_LOG_FUNCTION;
  NS_LOG_LOGIC("returns " << m_nTotalReceivedBytes);
  return m_nTotalReceivedBytes;
}

uint32_t
Queue::GetTotalReceivedPackets (void)
{
  NS_LOG_FUNCTION;
  NS_LOG_LOGIC ("returns " << m_nTotalReceivedPackets);
  return m_nTotalReceivedPackets;
}

uint32_t
Queue:: GetTotalDroppedBytes (void)
{
  NS_LOG_FUNCTION;
  NS_LOG_LOGIC ("returns " << m_nTotalDroppedBytes);
  return m_nTotalDroppedBytes;
}

uint32_t
Queue::GetTotalDroppedPackets (void)
{
  NS_LOG_FUNCTION;
  NS_LOG_LOGIC("returns " << m_nTotalDroppedPackets);
  return m_nTotalDroppedPackets;
}

void 
Queue::ResetStatistics (void)
{
  NS_LOG_FUNCTION;
  m_nTotalReceivedBytes = 0;
  m_nTotalReceivedPackets = 0;
  m_nTotalDroppedBytes = 0;
  m_nTotalDroppedPackets = 0;
}

void
Queue::Drop (const Packet& p)
{
  NS_LOG_FUNCTION;
  NS_LOG_PARAMS (this << &p);

  m_nTotalDroppedPackets++;
  m_nTotalDroppedBytes += p.GetSize ();

  NS_LOG_LOGIC ("m_traceDrop (p)");
  m_traceDrop (p);
}

Ptr<Queue>
Queue::CreateDefault (void)
{
  NS_LOG_FUNCTION;
  ClassId classId = g_classIdDefaultValue.GetValue ();
  Ptr<Queue> queue = ComponentManager::Create<Queue> (classId, Queue::iid);
  return queue;
}

}; // namespace ns3
