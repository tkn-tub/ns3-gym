/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "calendar-scheduler.h"
#include "event-impl.h"
#include <utility>
#include <string>
#include <list>
#include "ns3/assert.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CalendarScheduler");

class Calendar 
{
public:
  Calendar (uint32_t nBuckets, 
            uint64_t newWidth, uint64_t startPrio);
  ~Calendar ();

  void Insert (const Scheduler::Event &ev);
  Scheduler::Event PeekNext (void) const;
  Scheduler::Event RemoveNext (void);
  void Remove (const Scheduler::Event &ev);
  uint32_t GetNBuckets (void) const;

private:
  Calendar &operator = (const Calendar &o);
  Calendar (const Calendar &o);
  void UpdateSearch (uint32_t bucket, uint64_t ts) const;

  typedef std::list<Scheduler::Event> Bucket;
  Bucket *m_buckets;
  uint32_t m_nBuckets;
  // duration of a year
  uint64_t m_width;
  // bucket index from which the last event was dequeued
  uint32_t m_lastBucket;
  // priority at the top of the bucket from which last event was dequeued
  uint64_t m_bucketTop;
  // priority of last event dequeued
  uint64_t m_lastPrio;
};


Calendar::Calendar (uint32_t nBuckets, 
                    uint64_t width, 
                    uint64_t startPrio)
{
  m_buckets = new Bucket [nBuckets];
  m_nBuckets = nBuckets;
  m_width = width;
  m_lastPrio = startPrio;
  uint32_t n = startPrio / width;
  m_lastBucket = n % nBuckets;
  m_bucketTop = (n + 1) * width;
}
Calendar::~Calendar ()
{
  delete [] m_buckets;
  m_buckets = 0;
}
uint32_t 
Calendar::GetNBuckets (void) const
{
  return m_nBuckets;
}
void
Calendar::Insert (const Scheduler::Event &ev)
{
  // calculate bucket index.
  uint32_t bucket = (ev.key.m_ts / m_width) % m_nBuckets;

  // insert in bucket list.
  Bucket::iterator end = m_buckets[bucket].end ();
  for (Bucket::iterator i = m_buckets[bucket].begin (); i != end; ++i) 
    {
      if (ev.key < i->key)
        {
          m_buckets[bucket].insert (i, ev);
          return;
        }
    }
  m_buckets[bucket].push_back (ev);
}
void 
Calendar::UpdateSearch (uint32_t bucket, uint64_t ts) const
{
  // we update these variables to be able to find the
  // 'next' event faster next time.
  Calendar *self = const_cast<Calendar *> (this);
  self->m_lastBucket = bucket;
  self->m_bucketTop = ts;
}
Scheduler::Event
Calendar::PeekNext (void) const
{
  uint32_t i = m_lastBucket;
  uint64_t bucketTop = m_bucketTop;
  do {
    if (!m_buckets[i].empty ())
      {
        Scheduler::Event next = m_buckets[i].front ();
        if (next.key.m_ts < bucketTop)
          {
            UpdateSearch (i, bucketTop);
            return next;
          }
      }
    i++;
    i %= m_nBuckets;
    bucketTop += m_width;
  } while (i != m_lastBucket);

  // We did not find a low-priority event within next year.
  // So, iterate again to search the absolute minimum priority event.
  i = m_lastBucket;
  bucketTop = m_bucketTop;
  Scheduler::Event minEvent = {0, {~0, ~0}};
  uint32_t minBucket;
  uint64_t minBucketTop;
  do {
    if (!m_buckets[i].empty ())
      {
        Scheduler::Event event = m_buckets[i].front ();
        if (event.key < minEvent.key)
          {
            minEvent = event;
            minBucket = i;
            minBucketTop = bucketTop;
          }
      }
    i++;
    i %= m_nBuckets;
    bucketTop += m_width;
  } while (i != m_lastBucket);

  UpdateSearch (minBucket, minBucketTop);

  return minEvent;
}

Scheduler::Event
Calendar::RemoveNext (void)
{
  uint32_t i = m_lastBucket;
  uint64_t bucketTop = m_bucketTop;
  do {
    if (!m_buckets[i].empty ())
      {
        Scheduler::Event next = m_buckets[i].front ();
        if (next.key.m_ts < bucketTop)
          {
            m_lastBucket = i;
            m_bucketTop = bucketTop;
            m_lastPrio = next.key.m_ts;
            m_buckets[i].pop_front ();
            return next;
          }
      }
    i++;
    i %= m_nBuckets;
    bucketTop += m_width;
  } while (i != m_lastBucket);

  // We did not find a low-priority event within next year.
  // So, iterate again to search the absolute minimum priority event.
  i = m_lastBucket;
  bucketTop = m_bucketTop;
  Scheduler::Event minEvent = {0, {~0, ~0}};
  uint32_t minBucket;
  uint64_t minBucketTop;
  do {
    if (!m_buckets[i].empty ())
      {
        Scheduler::Event event = m_buckets[i].front ();
        if (event.key < minEvent.key)
          {
            minEvent = event;
            minBucket = i;
            minBucketTop = bucketTop;
          }
      }
    i++;
    i %= m_nBuckets;
    bucketTop += m_width;
  } while (i != m_lastBucket);

  m_lastBucket = minBucket;
  m_bucketTop = minBucketTop;
  m_lastPrio = minEvent.key.m_ts;
  m_buckets[minBucket].pop_front ();

  return minEvent;
}

void
Calendar::Remove (const Scheduler::Event &ev)
{
  // bucket index of event
  uint32_t bucket = (ev.key.m_ts / m_width) % m_nBuckets;

  Bucket::iterator end = m_buckets[bucket].end ();
  for (Bucket::iterator i = m_buckets[bucket].begin (); i != end; ++i)
    {
      if (i->key.m_uid == ev.key.m_uid)
        {
          NS_ASSERT (ev.impl == i->impl);
          m_buckets[bucket].erase (i);
          return;
        }
    }
  NS_ASSERT (false);
}





CalendarScheduler::CalendarScheduler ()
{
  NS_LOG_FUNCTION (this);
  m_calendar = new Calendar (2, 1, 0);
  m_qSize = 0;
}
CalendarScheduler::~CalendarScheduler ()
{
  NS_LOG_FUNCTION (this);
  delete m_calendar;
  m_calendar = 0;
}

void
CalendarScheduler::Insert (const Event &ev)
{
  NS_LOG_FUNCTION (this << ev.impl << ev.key.m_ts << ev.key.m_uid);
  m_calendar->Insert (ev);
  m_qSize++;
  ResizeUp ();
}
bool 
CalendarScheduler::IsEmpty (void) const
{
  return m_qSize == 0;
}
Scheduler::Event
CalendarScheduler::PeekNext (void) const
{
  NS_LOG_FUNCTION (this);
  return m_calendar->PeekNext ();
}

Scheduler::Event
CalendarScheduler::RemoveNext (void)
{
  NS_LOG_FUNCTION (this);

  Scheduler::Event ev = m_calendar->RemoveNext ();
  m_qSize--;
  ResizeDown ();

  return ev;
}

void
CalendarScheduler::Remove (const Event &ev)
{
  NS_LOG_FUNCTION (this << ev.impl << ev.key.m_ts << ev.key.m_uid);

  m_calendar->Remove (ev);
  m_qSize--;
  ResizeDown ();
}

void 
CalendarScheduler::ResizeUp (void)
{
  if (m_qSize > m_calendar->GetNBuckets () * 2)
    {
      Resize (m_calendar->GetNBuckets () * 2);
    }
}
void 
CalendarScheduler::ResizeDown (void)
{
  if (m_qSize < ((m_calendar->GetNBuckets () / 2) - 2))
    {
      Resize (m_calendar->GetNBuckets () / 2);
    }
}
#if 0
uint32_t
CalendarScheduler::CalculateNewWidth (void) const
{
  // We don't implement the heuristic from the calendar queue paper. Instead,
  // we implement the same heuristic found in the ns-2 calendar scheduler.
  
}
#endif
void 
CalendarScheduler::Resize (uint32_t newSize)
{
  NS_LOG_FUNCTION (this << newSize);

  
}

} // namespace ns3
