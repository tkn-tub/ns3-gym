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

#ifndef CALENDAR_SCHEDULER_H
#define CALENDAR_SCHEDULER_H

#include "scheduler.h"
#include <stdint.h>
#include <list>

/**
 * \file
 * \ingroup scheduler
 * ns3::CalendarScheduler class declaration.
 */

namespace ns3 {

class EventImpl;

/**
 * \ingroup scheduler
 * \brief a calendar queue event scheduler
 *
 * This event scheduler is a direct implementation of the algorithm
 * known as a calendar queue, first published in 1988 in
 * "Calendar Queues: A Fast O(1) Priority Queue Implementation for
 * the Simulation Event Set Problem" by Randy Brown. There are many
 * refinements published later but this class implements
 * the original algorithm (to the best of my knowledge).
 *
 * \note
 * This queue is much slower than I expected (much slower than the
 * std::map queue) and this seems to be because the original resizing policy
 * is horribly bad.  This is most likely the reason why there have been
 * so many variations published which all slightly tweak the resizing
 * heuristics to obtain a better distribution of events across buckets.
 */
class CalendarScheduler : public Scheduler
{
public:
  /**
   *  Register this type.
   *  \return The object TypeId.
   */
  static TypeId GetTypeId (void);

  /** Constructor. */
  CalendarScheduler ();
  /** Destructor. */
  virtual ~CalendarScheduler ();

  // Inherited
  virtual void Insert (const Scheduler::Event &ev);
  virtual bool IsEmpty (void) const;
  virtual Scheduler::Event PeekNext (void) const;
  virtual Scheduler::Event RemoveNext (void);
  virtual void Remove (const Scheduler::Event &ev);

private:
  /** Double the number of buckets if necessary. */
  void ResizeUp (void);
  /** Halve the number of buckets if necessary. */
  void ResizeDown (void);
  /**
   * Resize to a new number of buckets, with automatically computed width.
   *
   * \param [in] newSize The new number of buckets.
   */
  void Resize (uint32_t newSize);
  /**
   * Compute the new bucket size, based on up to the first 25 entries.
   *
   * \returns The new width.
   */
  uint32_t CalculateNewWidth (void);
  /**
   * Initialize the calendar queue.
   *
   * \param [in] nBuckets The number of buckets.
   * \param [in] width The bucket size, in dimensionless time units.
   * \param [in] startPrio The starting time.
   */
  void Init (uint32_t nBuckets,
             uint64_t width,
             uint64_t startPrio);
  /**
   * Hash the dimensionless time to a bucket.
   *
   * \param [in] key The dimensionless time.
   * \returns The bucket index.
   */
  inline uint32_t Hash (uint64_t key) const;
  /** Print the configuration and bucket size distribution. */
  void PrintInfo (void);
  /**
   * Resize the number of buckets and width.
   *
   * \param [in] newSize The number of buckets.
   * \param [in] newWidth The size of the new buckets.
   */
  void DoResize (uint32_t newSize, uint32_t newWidth);
  /**
   * Remove the earliest event.
   *
   * \returns The earliest event.
   */
  Scheduler::Event DoRemoveNext (void);
  /**
   * Insert a new event in to the correct bucket.
   *
   * \param [in] ev The new Event.
   */
  void DoInsert (const Scheduler::Event &ev);

  /** Calendar bucket type: a list of Events. */
  typedef std::list<Scheduler::Event> Bucket;
  
  /** Array of buckets. */
  Bucket *m_buckets;
  /** Number of buckets in the array. */
  uint32_t m_nBuckets;
  /** Duration of a bucket, in dimensionless time units. */
  uint64_t m_width;
  /** Bucket index from which the last event was dequeued. */
  uint32_t m_lastBucket;
  /** Priority at the top of the bucket from which last event was dequeued. */
  uint64_t m_bucketTop;
  /** The priority of the last event removed. */
  uint64_t m_lastPrio;
  /** Number of events in queue. */
  uint32_t m_qSize;
};

} // namespace ns3

#endif /* CALENDAR_SCHEDULER_H */
