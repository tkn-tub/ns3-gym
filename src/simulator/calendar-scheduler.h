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

#ifndef CALENDAR_SCHEDULER_H
#define CALENDAR_SCHEDULER_H

#include "scheduler.h"
#include <stdint.h>
#include <list>

namespace ns3 {

class EventImpl;

/**
 * \ingroup scheduler
 */
class CalendarScheduler : public Scheduler 
{
public:
  CalendarScheduler ();
  virtual ~CalendarScheduler ();

  virtual void Insert (const Event &ev);
  virtual bool IsEmpty (void) const;
  virtual Event PeekNext (void) const;
  virtual Event RemoveNext (void);
  virtual void Remove (const Event &ev);

private:
  void ResizeUp (void);
  void ResizeDown (void);
  void Resize (uint32_t newSize);
  uint32_t CalculateNewWidth (void);
  void Init (uint32_t nBuckets, 
             uint64_t width,
             uint64_t startPrio);
  inline uint32_t Hash (uint64_t key) const;
  void PrintInfo (void);
  void DoResize (uint32_t newSize, uint32_t newWidth);
  Scheduler::Event DoRemoveNext (void);
  void DoInsert (const Event &ev);

  typedef std::list<Scheduler::Event> Bucket;
  Bucket *m_buckets;
  // number of buckets in array
  uint32_t m_nBuckets;
  // duration of a bucket
  uint64_t m_width;
  // bucket index from which the last event was dequeued
  uint32_t m_lastBucket;
  // priority at the top of the bucket from which last event was dequeued
  uint64_t m_bucketTop;
  // the priority of the last event removed
  uint64_t m_lastPrio;
  // number of events in queue
  uint32_t m_qSize;
};

} // namespace ns3

#endif /* CALENDAR_SCHEDULER_H */
