/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 INRIA
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

NS_OBJECT_ENSURE_REGISTERED (CalendarScheduler);

TypeId 
CalendarScheduler::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CalendarScheduler")
    .SetParent<Scheduler> ()
    .AddConstructor<CalendarScheduler> ()
    ;
  return tid;
}

CalendarScheduler::CalendarScheduler ()
{
  NS_LOG_FUNCTION (this);
  Init (2, 1, 0);
  m_qSize = 0;
}
CalendarScheduler::~CalendarScheduler ()
{
  NS_LOG_FUNCTION (this);
  delete [] m_buckets;
  m_buckets = 0;
}
void
CalendarScheduler::Init (uint32_t nBuckets, 
                         uint64_t width,
                         uint64_t startPrio)
{
  NS_LOG_FUNCTION (this << nBuckets << width << startPrio);
  m_buckets = new Bucket [nBuckets];
  m_nBuckets = nBuckets;
  m_width = width;
  m_lastPrio = startPrio;
  m_lastBucket = Hash (startPrio);
  m_bucketTop = (startPrio / width + 1) * width;
}
void
CalendarScheduler::PrintInfo (void)
{
  std::cout << "nBuckets=" << m_nBuckets << ", width=" << m_width<< std::endl;
  std::cout << "Bucket Distribution ";
  for (uint32_t i = 0; i < m_nBuckets; i++)
    {
      std::cout << m_buckets[i].size () << " ";
    }
  std::cout << std::endl;
}
uint32_t
CalendarScheduler::Hash (uint64_t ts) const
{
  uint32_t bucket = (ts / m_width) % m_nBuckets;
  return bucket;
}

void
CalendarScheduler::DoInsert (const Event &ev)
{
  NS_LOG_FUNCTION (this << ev.key.m_ts << ev.key.m_uid);
  // calculate bucket index.
  uint32_t bucket = Hash (ev.key.m_ts);
  NS_LOG_LOGIC ("insert in bucket=" << bucket);

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
CalendarScheduler::Insert (const Event &ev)
{
  DoInsert (ev);
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
  NS_LOG_FUNCTION (this << m_lastBucket << m_bucketTop);
  NS_ASSERT (!IsEmpty ());
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
CalendarScheduler::DoRemoveNext (void)
{
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

  return minEvent;
}

Scheduler::Event
CalendarScheduler::RemoveNext (void)
{
  NS_LOG_FUNCTION (this << m_lastBucket << m_bucketTop);
  NS_ASSERT (!IsEmpty ());

  Scheduler::Event ev = DoRemoveNext ();
  NS_LOG_LOGIC ("remove ts=" << ev.key.m_ts << 
                ", key=" << ev.key.m_uid << 
                ", from bucket=" << m_lastBucket);
  m_qSize--;
  ResizeDown ();
  return ev;
}

void
CalendarScheduler::Remove (const Event &ev)
{
  NS_ASSERT (!IsEmpty ());
  // bucket index of event
  uint32_t bucket = Hash (ev.key.m_ts);

  Bucket::iterator end = m_buckets[bucket].end ();
  for (Bucket::iterator i = m_buckets[bucket].begin (); i != end; ++i)
    {
      if (i->key.m_uid == ev.key.m_uid)
        {
          NS_ASSERT (ev.impl == i->impl);
          m_buckets[bucket].erase (i);

          m_qSize--;
          ResizeDown ();
          return;
        }
    }
  NS_ASSERT (false);
}

void 
CalendarScheduler::ResizeUp (void)
{
  if (m_qSize > m_nBuckets * 2 && 
      m_nBuckets < 32768)
    {
      Resize (m_nBuckets * 2);
    }
}
void 
CalendarScheduler::ResizeDown (void)
{
  if (m_qSize < m_nBuckets / 2)
    {
      Resize (m_nBuckets / 2);
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

  // we gather the first nSamples from the queue
  std::list<Scheduler::Event> samples;
  // save state
  uint32_t lastBucket = m_lastBucket;
  uint64_t bucketTop = m_bucketTop;
  uint64_t lastPrio = m_lastPrio;
 
  // gather requested events
  for (uint32_t i = 0; i < nSamples; i++)
    {
      samples.push_back (DoRemoveNext ());
    }
  // put them back
  for (std::list<Scheduler::Event>::const_iterator i = samples.begin (); 
       i != samples.end (); ++i)
    {
      DoInsert (*i);
    }

  // restore state.
  m_lastBucket = lastBucket;
  m_bucketTop = bucketTop;
  m_lastPrio = lastPrio;

  // finally calculate inter-time average over samples.
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
CalendarScheduler::DoResize (uint32_t newSize, uint32_t newWidth)
{
  Bucket *oldBuckets = m_buckets;
  uint32_t oldNBuckets = m_nBuckets;
  Init (newSize, newWidth, m_lastPrio);

  for (uint32_t i = 0; i < oldNBuckets; i++)
    {
      Bucket::iterator end = oldBuckets[i].end ();
      for (Bucket::iterator j = oldBuckets[i].begin (); j != end; ++j) 
        {
          DoInsert (*j);
        }
    }
  delete [] oldBuckets;
}
void 
CalendarScheduler::Resize (uint32_t newSize)
{
  NS_LOG_FUNCTION (this << newSize);

  //PrintInfo ();
  uint32_t newWidth = CalculateNewWidth ();  
  DoResize (newSize, newWidth);
}

} // namespace ns3
