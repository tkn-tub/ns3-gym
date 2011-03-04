/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#include "lte-mac-queue.h"
#include "ns3/packet.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/uinteger.h"
#include "ns3/simulator.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("LteMacQueue");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LteMacQueue);

LteMacQueue::QueueElement::QueueElement (void)
  : m_packet (Create<Packet> ()),
    m_timeStamp (Seconds (0))
{
}


LteMacQueue::QueueElement::QueueElement (Ptr<Packet> packet, Time timeStamp)
  : m_packet (packet),
    m_timeStamp (timeStamp)
{
}


uint32_t
LteMacQueue::QueueElement::GetSize (void) const
{
  NS_LOG_FUNCTION (this);
  // XXX: ADD mac/RLC/CRC OVERHEADs ?!?
  uint32_t size = m_packet->GetSize ();
  return size;
}


TypeId
LteMacQueue::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteMacQueue")
    .SetParent<Object> ()
    .AddAttribute (
      "MaxSize",
      "Maximum size",
      UintegerValue (1024),
      MakeUintegerAccessor (&LteMacQueue::GetMaxSize,
                            &LteMacQueue::SetMaxSize),
      MakeUintegerChecker<uint32_t> ())
    .AddTraceSource ("Enqueue",
                     "Enqueue trace",
                     MakeTraceSourceAccessor (&LteMacQueue::m_traceEnqueue))
    .AddTraceSource ("Dequeue",
                     "Dequeue trace",
                     MakeTraceSourceAccessor (&LteMacQueue::m_traceDequeue))
    .AddTraceSource ("Drop",
                     "Drop trace",
                     MakeTraceSourceAccessor (&LteMacQueue::m_traceDrop))
  ;
  return tid;
}


LteMacQueue::LteMacQueue (void)
  : m_maxSize (0),
    m_bytes (0),
    m_nrDataPackets (0)
{
}


LteMacQueue::LteMacQueue (uint32_t maxSize)
  : m_maxSize (maxSize),
    m_bytes (0),
    m_nrDataPackets (0)
{
}


LteMacQueue::~LteMacQueue (void)
{
  m_queue.clear ();
}


void
LteMacQueue::SetMaxSize (uint32_t maxSize)
{
  NS_LOG_FUNCTION (this);
  m_maxSize = maxSize;
}


uint32_t
LteMacQueue::GetMaxSize (void) const
{
  NS_LOG_FUNCTION (this);
  return m_maxSize;
}


bool
LteMacQueue::Enqueue (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << "queue size: " << m_queue.size ());
  NS_LOG_FUNCTION (this << "packet size: " << packet->GetSize ());
  if (m_queue.size () == m_maxSize)
    {
      m_traceDrop (packet);
      return false;
    }

  m_traceEnqueue (packet);
  QueueElement element (packet, Simulator::Now ());
  m_queue.push_back (element);

  m_nrDataPackets++;
  m_bytes += element.GetSize ();

  NS_LOG_FUNCTION (this << "queue size: " << m_queue.size ());
  return true;
}


Ptr<Packet>
LteMacQueue::Dequeue (void)
{
  NS_LOG_FUNCTION (this);
  if (!IsEmpty ())
    {
      QueueElement element = Front ();
      Pop ();

      m_nrDataPackets--;

      Ptr<Packet> packet = element.m_packet;

      return packet;
    }

  return 0;
}


Ptr<Packet>
LteMacQueue::Dequeue (uint32_t availableByte)
{
  NS_LOG_FUNCTION (this << availableByte);
  /*
   This functiopn can be called when the UM of AM RLC mode are abilited.
   */
  return 0;
}


Ptr<Packet>
LteMacQueue::Peek (void) const
{
  NS_LOG_FUNCTION (this);
  if (!IsEmpty ())
    {
      QueueElement element = m_queue.front ();
      Ptr<Packet> packet = element.m_packet->Copy ();

      return packet;
    }

  return 0;
}


uint32_t
LteMacQueue::GetSize (void) const
{
  NS_LOG_FUNCTION (this);
  return m_queue.size ();
}

uint32_t
LteMacQueue::GetNBytes (void) const
{
  NS_LOG_FUNCTION (this);
  return m_bytes;
}

uint32_t
LteMacQueue::GetQueueLengthWithMACOverhead (void)
{
  NS_LOG_FUNCTION (this);
  uint32_t queueSize = GetNBytes ();
  // Add MAC/RLC/CRC Overhead
  queueSize += GetSize () * 0; // XXX

  return queueSize;
}

LteMacQueue::QueueElement
LteMacQueue::Front (void) const
{
  NS_LOG_FUNCTION (this);
  QueueElement element = m_queue.front ();
  return element;
}

void
LteMacQueue::Pop (void)
{
  NS_LOG_FUNCTION (this);
  m_queue.pop_front ();
}

bool
LteMacQueue::IsEmpty (void) const
{
  NS_LOG_FUNCTION (this);
  return m_queue.empty ();
}


const LteMacQueue::PacketQueue &
LteMacQueue::GetPacketQueue (void) const
{
  NS_LOG_FUNCTION (this);
  return m_queue;
}


} // namespace ns3
