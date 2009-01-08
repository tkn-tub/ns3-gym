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
            uint64_t newWidth);
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
  // duration of a bucket
  uint64_t m_width;
  // bucket index from which the last event was dequeued
  uint32_t m_lastBucket;
  // priority at the top of the bucket from which last event was dequeued
  uint64_t m_bucketTop;
};


Calendar::Calendar (uint32_t nBuckets, 
                    uint64_t width)
{
  NS_ASSERT (width > 0);
  m_buckets = new Bucket [nBuckets];
  m_nBuckets = nBuckets;
  m_width = width;
  m_lastBucket = 0;
  m_bucketTop = m_width;
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
  Scheduler::Event minEvent = {0, {~0, ~0}};
  uint32_t minBucket = 0; // quiet compiler
  uint64_t minBucketTop = 0; // quiet compiler
  do {
    if (!m_buckets[i].empty ())
      {
        Scheduler::Event next = m_buckets[i].front ();
        if (next.key.m_ts < bucketTop)
          {
            UpdateSearch (i, bucketTop);
            return next;
          }
        if (next.key < minEvent.key)
          {
            minEvent = next;
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
  Scheduler::Event minEvent = {0, {~0, ~0}};
  uint32_t minBucket = 0; // quiet compiler
  uint64_t minBucketTop = 0; // quiet compiler
  do {
    if (!m_buckets[i].empty ())
      {
        Scheduler::Event next = m_buckets[i].front ();
        if (next.key.m_ts < bucketTop)
          {
            m_lastBucket = i;
            m_bucketTop = bucketTop;
            m_buckets[i].pop_front ();
            return next;
          }
        if (next.key < minEvent.key)
          {
            minEvent = next;
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
  m_calendar = new Calendar (2, 1);
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
  NS_ASSERT (!IsEmpty ());
  return m_calendar->PeekNext ();
}

Scheduler::Event
CalendarScheduler::RemoveNext (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (!IsEmpty ());
  Scheduler::Event ev = m_calendar->RemoveNext ();
  m_qSize--;
  ResizeDown ();

  return ev;
}

void
CalendarScheduler::Remove (const Event &ev)
{
  NS_LOG_FUNCTION (this << ev.impl << ev.key.m_ts << ev.key.m_uid);
  NS_ASSERT (!IsEmpty ());
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
  if (m_qSize < m_calendar->GetNBuckets () / 2)
    {
      Resize (m_calendar->GetNBuckets () / 2);
    }
}

uint32_t
CalendarScheduler::CalculateNewWidth (std::list<Scheduler::Event> *dequeued)
{
  if (m_qSize < 2) 
    {
      return 1;
    }
  uint32_t nSamples;
  if (m_qSize <= 5)
    {
      nSamples = m_qSize;
    }
  else
    {
      nSamples = 5 + m_qSize / 10;
    }
  if (nSamples > 25)
    {
      nSamples = 25;
    }

  for (uint32_t i = 0; i < nSamples; i++)
    {
      dequeued->push_back (m_calendar->RemoveNext ());
    }
  m_qSize -= nSamples;
  
  uint64_t totalSeparation = 0;
  std::list<Scheduler::Event>::const_iterator end = dequeued->end ();
  std::list<Scheduler::Event>::const_iterator cur = dequeued->begin ();
  std::list<Scheduler::Event>::const_iterator next = cur;
  next++;
  while (next != end)
    {
      totalSeparation += next->key.m_ts - cur->key.m_ts;
      cur++;
      next++;
    }
  uint64_t twiceAvg = totalSeparation / (nSamples - 1) * 2;
  totalSeparation = 0;
  cur = dequeued->begin ();
  next = cur;
  next++;
  while (next != end)
    {
      uint64_t diff = next->key.m_ts - cur->key.m_ts;
      if (diff <= twiceAvg)
        {
          totalSeparation += diff;
        }
      cur++;
      next++;
    }

  totalSeparation *= 3;
  totalSeparation = std::max (totalSeparation, (uint64_t)1);
  return totalSeparation;
}

void 
CalendarScheduler::Resize (uint32_t newSize)
{
  NS_LOG_FUNCTION (this << newSize);

  std::list<Scheduler::Event> dequeued;
  uint32_t newWidth = CalculateNewWidth (&dequeued);
  
  Calendar *calendar = new Calendar (newSize, newWidth);

  for (uint32_t i = 0; i < m_qSize; i++)
    {
      Scheduler::Event ev = m_calendar->RemoveNext ();
      calendar->Insert (ev);
    }
  std::list<Scheduler::Event>::const_iterator end = dequeued.end ();
  for (std::list<Scheduler::Event>::const_iterator i = dequeued.begin ();
       i != end; ++i)
    {
      calendar->Insert (*i);
    }
  m_qSize += dequeued.size ();

  delete m_calendar;
  m_calendar = calendar;
}

} // namespace ns3
