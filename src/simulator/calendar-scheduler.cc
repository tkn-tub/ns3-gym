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
  Calendar ();
  ~Calendar ();

  Calendar *CreateResized (uint32_t nBuckets, 
                           uint64_t newWidth) const;

  void InsertAll (const Calendar *calendar);
  void Insert (const Scheduler::Event &ev);
  Scheduler::Event PeekNext (void) const;
  Scheduler::Event RemoveNext (void);
  void Remove (const Scheduler::Event &ev);
  uint32_t GetNBuckets (void) const;
  std::list<Scheduler::Event> PeekNext (uint32_t n) const;

  void PrintBucketDistribution (void);

private:
  Calendar &operator = (const Calendar &o);
  Calendar (const Calendar &o);
  Calendar (uint32_t nBuckets, 
            uint64_t width,
            uint64_t startPrio);
  void Init (uint32_t nBuckets, 
             uint64_t width,
             uint64_t startPrio);
  inline uint32_t Hash (uint64_t key) const;

  typedef std::list<Scheduler::Event> Bucket;
  Bucket *m_buckets;
  uint32_t m_nBuckets;
  // duration of a bucket
  uint64_t m_width;
  // bucket index from which the last event was dequeued
  uint32_t m_lastBucket;
  // priority at the top of the bucket from which last event was dequeued
  uint64_t m_bucketTop;
  // the priority of the last event removed.
  uint64_t m_lastPrio;
};

Calendar::Calendar ()
{
  Init (2, 1, 0);
}
Calendar::Calendar (uint32_t nBuckets, 
                    uint64_t width,
                    uint64_t startPrio)
{
  NS_ASSERT (width > 0);
  Init (nBuckets, width, startPrio);
}
void
Calendar::Init (uint32_t nBuckets, 
                uint64_t width,
                uint64_t startPrio)
{
  m_buckets = new Bucket [nBuckets];
  m_nBuckets = nBuckets;
  m_width = width;
  m_lastPrio = startPrio;
  m_lastBucket = Hash (startPrio);
  m_bucketTop = (startPrio / width + 1) * width;
}
Calendar::~Calendar ()
{
  delete [] m_buckets;
  m_buckets = 0;
}
Calendar *
Calendar::CreateResized (uint32_t nBuckets, 
                         uint64_t newWidth) const
{
  Calendar *calendar = new Calendar (nBuckets, newWidth, m_lastPrio);
  for (uint32_t i = 0; i < m_nBuckets; i++)
    {
      Bucket::iterator end = m_buckets[i].end ();
      for (Bucket::iterator j = m_buckets[i].begin (); j != end; ++j) 
        {
          calendar->Insert (*j);
        }
    }
  return calendar;
}

void
Calendar::PrintBucketDistribution (void)
{
  for (uint32_t i = 0; i < m_nBuckets; i++)
    {
      std::cout << m_buckets[i].size () << " ";
    }
  std::cout << std::endl;
}
uint32_t 
Calendar::GetNBuckets (void) const
{
  return m_nBuckets;
}
uint32_t
Calendar::Hash (uint64_t ts) const
{
  uint32_t bucket = (ts / m_width) % m_nBuckets;
  return bucket;
}
void
Calendar::Insert (const Scheduler::Event &ev)
{
  NS_LOG_FUNCTION (this << ev.key.m_ts << ev.key.m_uid);
  // calculate bucket index.
  uint32_t bucket = Hash (ev.key.m_ts);

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
  NS_LOG_LOGIC ("insert in bucket=" << bucket);
}
std::list<Scheduler::Event>
Calendar::PeekNext (uint32_t n) const
{
  // save state
  uint32_t lastBucket = m_lastBucket;
  uint64_t bucketTop = m_bucketTop;
  uint64_t lastPrio = m_lastPrio;
 
  Calendar *self = const_cast<Calendar *> (this); 
  std::list<Scheduler::Event> events;
  // gather requested events
  for (uint32_t i = 0; i < n; i++)
    {
      events.push_back (self->RemoveNext ());
    }
  // put them back
  for (std::list<Scheduler::Event>::const_iterator i = events.begin (); i != events.end (); ++i)
    {
      self->Insert (*i);
    }

  // restore state.
  self->m_lastBucket = lastBucket;
  self->m_bucketTop = bucketTop;
  self->m_lastPrio = lastPrio;

  // return copy of requested events.
  return events;
}

Scheduler::Event
Calendar::PeekNext (void) const
{
  NS_LOG_FUNCTION (this << m_lastBucket << m_bucketTop);
  uint32_t i = m_lastBucket;
  uint64_t bucketTop = m_bucketTop;
  Scheduler::Event minEvent = {0, {~0, ~0}};
  do {
    if (!m_buckets[i].empty ())
      {
        Scheduler::Event next = m_buckets[i].front ();
        if (next.key.m_ts < bucketTop)
          {
            return next;
          }
        if (next.key < minEvent.key)
          {
            minEvent = next;
          }
      }
    i++;
    i %= m_nBuckets;
    bucketTop += m_width;
  } while (i != m_lastBucket);

  return minEvent;
}

Scheduler::Event
Calendar::RemoveNext (void)
{
  NS_LOG_FUNCTION (this << m_lastBucket << m_bucketTop);
  uint32_t i = m_lastBucket;
  uint64_t bucketTop = m_bucketTop;
  Scheduler::Event minEvent = {0, {~0, ~0}};
  do {
    if (!m_buckets[i].empty ())
      {
        Scheduler::Event next = m_buckets[i].front ();
        if (next.key.m_ts < bucketTop)
          {
            m_lastBucket = i;
            m_lastPrio = next.key.m_ts;
            m_bucketTop = bucketTop;
            m_buckets[i].pop_front ();
            NS_LOG_LOGIC ("remove ts=" << next.key.m_ts << ", key=" << next.key.m_uid << ", from bucket=" << i);
            return next;
          }
        if (next.key < minEvent.key)
          {
            minEvent = next;
          }
      }
    i++;
    i %= m_nBuckets;
    bucketTop += m_width;
  } while (i != m_lastBucket);

  m_lastPrio = minEvent.key.m_ts;
  m_lastBucket = Hash (minEvent.key.m_ts);
  m_bucketTop = (minEvent.key.m_ts / m_width + 1) * m_width;
  m_buckets[m_lastBucket].pop_front ();

  NS_LOG_LOGIC ("remove ts=" << minEvent.key.m_ts << ", key=" << minEvent.key.m_uid << ", from bucket=" << m_lastBucket);

  return minEvent;
}

void
Calendar::Remove (const Scheduler::Event &ev)
{
  // bucket index of event
  uint32_t bucket = Hash (ev.key.m_ts);

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
  m_calendar = new Calendar ();
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
  NS_ASSERT (!IsEmpty ());
  return m_calendar->PeekNext ();
}

Scheduler::Event
CalendarScheduler::RemoveNext (void)
{
  NS_ASSERT (!IsEmpty ());
  Scheduler::Event ev = m_calendar->RemoveNext ();
  m_qSize--;
  ResizeDown ();

  return ev;
}

void
CalendarScheduler::Remove (const Event &ev)
{
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
CalendarScheduler::CalculateNewWidth (void)
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

  std::list<Scheduler::Event> samples = m_calendar->PeekNext (nSamples);
  
  uint64_t totalSeparation = 0;
  std::list<Scheduler::Event>::const_iterator end = samples.end ();
  std::list<Scheduler::Event>::const_iterator cur = samples.begin ();
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
  cur = samples.begin ();
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

  m_calendar->PrintBucketDistribution ();

  uint32_t newWidth = CalculateNewWidth ();  
  Calendar *calendar = m_calendar->CreateResized (newSize, newWidth);

  delete m_calendar;
  m_calendar = calendar;
}

} // namespace ns3
