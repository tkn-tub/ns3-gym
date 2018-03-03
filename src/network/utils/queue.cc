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

#include "ns3/abort.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "queue.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Queue");

NS_OBJECT_ENSURE_REGISTERED (QueueBase);
NS_OBJECT_TEMPLATE_CLASS_DEFINE (Queue,Packet);

TypeId
QueueBase::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::QueueBase")
    .SetParent<Object> ()
    .SetGroupName ("Network")
    .AddAttribute ("Mode",
                   "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.",
                   EnumValue (QUEUE_MODE_PACKETS),
                   MakeEnumAccessor (&QueueBase::SetMode,
                                     &QueueBase::GetMode),
                   MakeEnumChecker (QUEUE_MODE_BYTES, "QUEUE_MODE_BYTES",
                                    QUEUE_MODE_PACKETS, "QUEUE_MODE_PACKETS"),
                   TypeId::DEPRECATED,
                   "Use the MaxSize attribute instead")
    .AddAttribute ("MaxPackets",
                   "The maximum number of packets accepted by this queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&QueueBase::SetMaxPackets,
                                         &QueueBase::GetMaxPackets),
                   MakeUintegerChecker<uint32_t> (),
                   TypeId::DEPRECATED,
                   "Use the MaxSize attribute instead")
    .AddAttribute ("MaxBytes",
                   "The maximum number of bytes accepted by this queue.",
                   UintegerValue (100 * 65535),
                   MakeUintegerAccessor (&QueueBase::SetMaxBytes,
                                         &QueueBase::GetMaxBytes),
                   MakeUintegerChecker<uint32_t> (),
                   TypeId::DEPRECATED,
                   "Use the MaxSize attribute instead")
    .AddAttribute ("MaxSize",
                   "The max queue size",
                   QueueSizeValue (QueueSize ("0p")),
                   MakeQueueSizeAccessor (&QueueBase::SetMaxSize,
                                          &QueueBase::GetMaxSize),
                   MakeQueueSizeChecker ())
    .AddTraceSource ("PacketsInQueue",
                     "Number of packets currently stored in the queue",
                     MakeTraceSourceAccessor (&QueueBase::m_nPackets),
                     "ns3::TracedValueCallback::Uint32")
    .AddTraceSource ("BytesInQueue",
                     "Number of bytes currently stored in the queue",
                     MakeTraceSourceAccessor (&QueueBase::m_nBytes),
                     "ns3::TracedValueCallback::Uint32")
  ;
  return tid;
}

QueueBase::QueueBase () :
  m_nBytes (0),
  m_nTotalReceivedBytes (0),
  m_nPackets (0),
  m_nTotalReceivedPackets (0),
  m_nTotalDroppedBytes (0),
  m_nTotalDroppedBytesBeforeEnqueue (0),
  m_nTotalDroppedBytesAfterDequeue (0),
  m_nTotalDroppedPackets (0),
  m_nTotalDroppedPacketsBeforeEnqueue (0),
  m_nTotalDroppedPacketsAfterDequeue (0),
  m_maxPackets (1),         // to avoid that setting the mode at construction time is ignored
  m_maxBytes (1)            // to avoid that setting the mode at construction time is ignored
{
  NS_LOG_FUNCTION (this);
}

QueueBase::~QueueBase ()
{
  NS_LOG_FUNCTION (this);
}

void
QueueBase::AppendItemTypeIfNotPresent (std::string& typeId, const std::string& itemType)
{
  if (typeId.back () != '>')
    {
      typeId.append ("<" + itemType + ">");
    }
}

bool
QueueBase::IsEmpty (void) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("returns " << (m_nPackets.Get () == 0));
  return m_nPackets.Get () == 0;
}

uint32_t
QueueBase::GetNPackets (void) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("returns " << m_nPackets);
  return m_nPackets;
}

uint32_t
QueueBase::GetNBytes (void) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC (" returns " << m_nBytes);
  return m_nBytes;
}

QueueSize
QueueBase::GetCurrentSize (void) const
{
  NS_LOG_FUNCTION (this);

  if (m_maxSize.GetUnit () == QueueSizeUnit::PACKETS)
    {
      return QueueSize (QueueSizeUnit::PACKETS, m_nPackets);
    }
  if (m_maxSize.GetUnit () == QueueSizeUnit::BYTES)
    {
      return QueueSize (QueueSizeUnit::BYTES, m_nBytes);
    }
  NS_ABORT_MSG ("Unknown queue size unit");
}

uint32_t
QueueBase::GetTotalReceivedBytes (void) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("returns " << m_nTotalReceivedBytes);
  return m_nTotalReceivedBytes;
}

uint32_t
QueueBase::GetTotalReceivedPackets (void) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("returns " << m_nTotalReceivedPackets);
  return m_nTotalReceivedPackets;
}

uint32_t
QueueBase:: GetTotalDroppedBytes (void) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("returns " << m_nTotalDroppedBytes);
  return m_nTotalDroppedBytes;
}

uint32_t
QueueBase:: GetTotalDroppedBytesBeforeEnqueue (void) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("returns " << m_nTotalDroppedBytesBeforeEnqueue);
  return m_nTotalDroppedBytesBeforeEnqueue;
}

uint32_t
QueueBase:: GetTotalDroppedBytesAfterDequeue (void) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("returns " << m_nTotalDroppedBytesAfterDequeue);
  return m_nTotalDroppedBytesAfterDequeue;
}

uint32_t
QueueBase::GetTotalDroppedPackets (void) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("returns " << m_nTotalDroppedPackets);
  return m_nTotalDroppedPackets;
}

uint32_t
QueueBase::GetTotalDroppedPacketsBeforeEnqueue (void) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("returns " << m_nTotalDroppedPacketsBeforeEnqueue);
  return m_nTotalDroppedPacketsBeforeEnqueue;
}

uint32_t
QueueBase::GetTotalDroppedPacketsAfterDequeue (void) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("returns " << m_nTotalDroppedPacketsAfterDequeue);
  return m_nTotalDroppedPacketsAfterDequeue;
}

void
QueueBase::ResetStatistics (void)
{
  NS_LOG_FUNCTION (this);
  m_nTotalReceivedBytes = 0;
  m_nTotalReceivedPackets = 0;
  m_nTotalDroppedBytes = 0;
  m_nTotalDroppedBytesBeforeEnqueue = 0;
  m_nTotalDroppedBytesAfterDequeue = 0;
  m_nTotalDroppedPackets = 0;
  m_nTotalDroppedPacketsBeforeEnqueue = 0;
  m_nTotalDroppedPacketsAfterDequeue = 0;
}

void
QueueBase::SetMode (QueueBase::QueueMode mode)
{
  NS_LOG_FUNCTION (this << mode);

  if (mode == QUEUE_MODE_BYTES)
    {
      SetMaxSize (QueueSize (QueueSizeUnit::BYTES, m_maxBytes));
    }
  else if (mode == QUEUE_MODE_PACKETS)
    {
      SetMaxSize (QueueSize (QueueSizeUnit::PACKETS, m_maxPackets));
    }
  else
    {
      NS_ABORT_MSG ("Unknown queue size unit");
    }
}

QueueBase::QueueMode
QueueBase::GetMode (void) const
{
  NS_LOG_FUNCTION (this);
  return (m_maxSize.GetUnit () == QueueSizeUnit::PACKETS ? QUEUE_MODE_PACKETS : QUEUE_MODE_BYTES);
}

void
QueueBase::SetMaxPackets (uint32_t maxPackets)
{
  NS_LOG_FUNCTION (this << maxPackets);

  m_maxPackets = maxPackets;

  if (m_maxSize.GetUnit () == QueueSizeUnit::PACKETS)
    {
      SetMaxSize (QueueSize (QueueSizeUnit::PACKETS, m_maxPackets));
    }
}

uint32_t
QueueBase::GetMaxPackets (void) const
{
  NS_LOG_FUNCTION (this);
  return m_maxPackets;
}

void
QueueBase::SetMaxBytes (uint32_t maxBytes)
{
  NS_LOG_FUNCTION (this << maxBytes);

  m_maxBytes = maxBytes;

  if (m_maxSize.GetUnit () == QueueSizeUnit::BYTES)
    {
      SetMaxSize (QueueSize (QueueSizeUnit::BYTES, m_maxBytes));
    }
}

uint32_t
QueueBase::GetMaxBytes (void) const
{
  NS_LOG_FUNCTION (this);
  return m_maxBytes;
}

void
QueueBase::SetMaxSize (QueueSize size)
{
  NS_LOG_FUNCTION (this << size);

  // do nothing if the size is null
  if (!size.GetValue ())
    {
      return;
    }

  m_maxSize = size;

  NS_ABORT_MSG_IF (size < GetCurrentSize (),
                   "The new maximum queue size cannot be less than the current size");

  if (size.GetUnit () == QueueSizeUnit::PACKETS)
    {
      m_maxPackets = size.GetValue ();
    }
  else if (size.GetUnit () == QueueSizeUnit::BYTES)
    {
      m_maxBytes = size.GetValue ();
    }
}

QueueSize
QueueBase::GetMaxSize (void) const
{
  NS_LOG_FUNCTION (this);
  return m_maxSize;
}

} // namespace ns3
