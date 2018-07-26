/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005, 2009 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Mirko Banchi <mk.banchi@gmail.com>
 *          Stefano Avallone <stavallo@unina.it>
 */

#include "ns3/simulator.h"
#include "wifi-mac-queue.h"
#include "qos-blocked-destinations.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WifiMacQueue");

NS_OBJECT_ENSURE_REGISTERED (WifiMacQueue);

TypeId
WifiMacQueue::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiMacQueue")
    .SetParent<Queue<WifiMacQueueItem> > ()
    .SetGroupName ("Wifi")
    .AddConstructor<WifiMacQueue> ()
    .AddAttribute ("MaxDelay", "If a packet stays longer than this delay in the queue, it is dropped.",
                   TimeValue (MilliSeconds (500)),
                   MakeTimeAccessor (&WifiMacQueue::SetMaxDelay),
                   MakeTimeChecker ())
    .AddAttribute ("DropPolicy", "Upon enqueue with full queue, drop oldest (DropOldest) or newest (DropNewest) packet",
                   EnumValue (DROP_NEWEST),
                   MakeEnumAccessor (&WifiMacQueue::m_dropPolicy),
                   MakeEnumChecker (WifiMacQueue::DROP_OLDEST, "DropOldest",
                                    WifiMacQueue::DROP_NEWEST, "DropNewest"))
  ;
  return tid;
}

WifiMacQueue::WifiMacQueue ()
  : NS_LOG_TEMPLATE_DEFINE ("WifiMacQueue")
{
}

WifiMacQueue::~WifiMacQueue ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
WifiMacQueue::SetMaxDelay (Time delay)
{
  NS_LOG_FUNCTION (this << delay);
  m_maxDelay = delay;
}

Time
WifiMacQueue::GetMaxDelay (void) const
{
  return m_maxDelay;
}

bool
WifiMacQueue::TtlExceeded (ConstIterator &it)
{
  NS_LOG_FUNCTION (this);

  if (Simulator::Now () > (*it)->GetTimeStamp () + m_maxDelay)
    {
      NS_LOG_DEBUG ("Removing packet that stayed in the queue for too long (" <<
                    Simulator::Now () - (*it)->GetTimeStamp () << ")");
      auto curr = it++;
      DoRemove (curr);
      return true;
    }
  return false;
}

bool
WifiMacQueue::Enqueue (Ptr<WifiMacQueueItem> item)
{
  NS_LOG_FUNCTION (this << item);
  NS_ASSERT_MSG (GetMaxSize ().GetUnit () == QueueSizeUnit::PACKETS,
                 "WifiMacQueues must be in packet mode");

  // if the queue is full, remove the first stale packet (if any) encountered
  // starting from the head of the queue, in order to make room for the new packet.
  if (QueueBase::GetNPackets () == GetMaxSize ().GetValue ())
    {
      auto it = Head ();
      while (it != Tail () && !TtlExceeded (it))
        {
          it++;
        }
    }

  if (QueueBase::GetNPackets () == GetMaxSize ().GetValue () && m_dropPolicy == DROP_OLDEST)
    {
      NS_LOG_DEBUG ("Remove the oldest item in the queue");
      DoRemove (Head ());
    }

  return DoEnqueue (Tail (), item);
}

bool
WifiMacQueue::PushFront (Ptr<WifiMacQueueItem> item)
{
  NS_LOG_FUNCTION (this << item);
  NS_ASSERT_MSG (GetMaxSize ().GetUnit () == QueueSizeUnit::PACKETS,
                 "WifiMacQueues must be in packet mode");

  // if the queue is full, remove the first stale packet (if any) encountered
  // starting from the head of the queue, in order to make room for the new packet.
  if (QueueBase::GetNPackets () == GetMaxSize ().GetValue ())
    {
      auto it = Head ();
      while (it != Tail () && !TtlExceeded (it))
        {
          it++;
        }
    }

  if (QueueBase::GetNPackets () == GetMaxSize ().GetValue () && m_dropPolicy == DROP_OLDEST)
    {
      NS_LOG_DEBUG ("Remove the oldest item in the queue");
      DoRemove (Head ());
    }

  return DoEnqueue (Head (), item);
}

Ptr<WifiMacQueueItem>
WifiMacQueue::Dequeue (void)
{
  NS_LOG_FUNCTION (this);
  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          return DoDequeue (it);
        }
    }
  NS_LOG_DEBUG ("The queue is empty");
  return 0;
}

Ptr<WifiMacQueueItem>
WifiMacQueue::DequeueByAddress (Mac48Address dest)
{
  NS_LOG_FUNCTION (this << dest);

  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          if ((*it)->GetHeader ().IsData () && (*it)->GetDestinationAddress () == dest)
            {
              return DoDequeue (it);
            }

          it++;
        }
    }
  NS_LOG_DEBUG ("The queue is empty");
  return 0;
}

Ptr<WifiMacQueueItem>
WifiMacQueue::DequeueByTidAndAddress (uint8_t tid, Mac48Address dest)
{
  NS_LOG_FUNCTION (this << dest);
  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          if ((*it)->GetHeader ().IsQosData () && (*it)->GetDestinationAddress () == dest
              && (*it)->GetHeader ().GetQosTid () == tid)
            {
              return DoDequeue (it);
            }

          it++;
        }
    }
  NS_LOG_DEBUG ("The queue is empty");
  return 0;
}

Ptr<WifiMacQueueItem>
WifiMacQueue::DequeueFirstAvailable (const Ptr<QosBlockedDestinations> blockedPackets)
{
  NS_LOG_FUNCTION (this);
  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          if (!(*it)->GetHeader ().IsQosData ()
              || !blockedPackets->IsBlocked ((*it)->GetHeader ().GetAddr1 (), (*it)->GetHeader ().GetQosTid ()))
            {
              return DoDequeue (it);
            }

          it++;
        }
    }
  NS_LOG_DEBUG ("The queue is empty");
  return 0;
}

Ptr<const WifiMacQueueItem>
WifiMacQueue::Peek (void) const
{
  NS_LOG_FUNCTION (this);
  for (auto it = Head (); it != Tail (); it++)
    {
      // skip packets that stayed in the queue for too long. They will be
      // actually removed from the queue by the next call to a non-const method
      if (Simulator::Now () <= (*it)->GetTimeStamp () + m_maxDelay)
        {
          return DoPeek (it);
        }
    }
  NS_LOG_DEBUG ("The queue is empty");
  return 0;
}

Ptr<const WifiMacQueueItem>
WifiMacQueue::PeekByTidAndAddress (uint8_t tid, Mac48Address dest)
{
  NS_LOG_FUNCTION (this << dest);
  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          if ((*it)->GetHeader ().IsQosData () && (*it)->GetDestinationAddress () == dest
              && (*it)->GetHeader ().GetQosTid () == tid)
            {
              return DoPeek (it);
            }

          it++;
        }
    }
  NS_LOG_DEBUG ("The queue is empty");
  return 0;
}

Ptr<const WifiMacQueueItem>
WifiMacQueue::PeekFirstAvailable (const Ptr<QosBlockedDestinations> blockedPackets)
{
  NS_LOG_FUNCTION (this);
  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          if (!(*it)->GetHeader ().IsQosData ()
              || !blockedPackets->IsBlocked ((*it)->GetHeader ().GetAddr1 (), (*it)->GetHeader ().GetQosTid ()))
            {
              return DoPeek (it);
            }

          it++;
        }
    }
  NS_LOG_DEBUG ("The queue is empty");
  return 0;
}

Ptr<WifiMacQueueItem>
WifiMacQueue::Remove (void)
{
  NS_LOG_FUNCTION (this);

  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          return DoRemove (it);
        }
    }
  NS_LOG_DEBUG ("The queue is empty");
  return 0;
}

bool
WifiMacQueue::Remove (Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);
  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          if ((*it)->GetPacket () == packet)
            {
              DoRemove (it);
              return true;
            }

          it++;
        }
    }
  NS_LOG_DEBUG ("Packet " << packet << " not found in the queue");
  return false;
}

uint32_t
WifiMacQueue::GetNPacketsByAddress (Mac48Address dest)
{
  NS_LOG_FUNCTION (this << dest);

  uint32_t nPackets = 0;

  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          if ((*it)->GetHeader ().IsData () && (*it)->GetDestinationAddress () == dest)
            {
              nPackets++;
            }

          it++;
        }
    }
  NS_LOG_DEBUG ("returns " << nPackets);
  return nPackets;
}

uint32_t
WifiMacQueue::GetNPacketsByTidAndAddress (uint8_t tid, Mac48Address dest)
{
  NS_LOG_FUNCTION (this << dest);
  uint32_t nPackets = 0;
  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          if ((*it)->GetHeader ().IsQosData () && (*it)->GetDestinationAddress () == dest
              && (*it)->GetHeader ().GetQosTid () == tid)
            {
              nPackets++;
            }

          it++;
        }
    }
  NS_LOG_DEBUG ("returns " << nPackets);
  return nPackets;
}

bool
WifiMacQueue::IsEmpty (void)
{
  NS_LOG_FUNCTION (this);
  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          NS_LOG_DEBUG ("returns false");
          return false;
        }
    }
  NS_LOG_DEBUG ("returns true");
  return true;
}

uint32_t
WifiMacQueue::GetNPackets (void)
{
  NS_LOG_FUNCTION (this);
  // remove packets that stayed in the queue for too long
  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          it++;
        }
    }
  return QueueBase::GetNPackets ();
}

uint32_t
WifiMacQueue::GetNBytes (void)
{
  NS_LOG_FUNCTION (this);
  // remove packets that stayed in the queue for too long
  for (auto it = Head (); it != Tail (); )
    {
      if (!TtlExceeded (it))
        {
          it++;
        }
    }
  return QueueBase::GetNBytes ();
}

} //namespace ns3
