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

#ifndef SCHEDULER_HEAP_H
#define SCHEDULER_HEAP_H

#include "scheduler.h"
#include <stdint.h>
#include <vector>

namespace ns3 {

class EventHolder;

/**
 * \ingroup scheduler
 * \brief a binary heap event scheduler
 * 
 * This code started as a c++ translation of a java-based code written in 2005
 * to implement a heap sort. So, this binary heap is really a pretty 
 * straightforward implementation of the classic data structure. Not much to say
 * about it.
 *
 * What is smart about this code ?
 *  - it does not use the index 0 in the array to avoid having to convert
 *    C-style array indexes (which start at zero) and heap-style indexes
 *    (which start at 1). This is why _all_ indexes start at 1, and that
 *    the index of the root is 1.
 *  - It uses a slightly non-standard while loop for top-down heapify
 *    to move one if statement out of the loop.
 */
class HeapScheduler : public Scheduler {
public:
  HeapScheduler ();
  virtual ~HeapScheduler ();

  virtual void Insert (const EventId &id);
  virtual bool IsEmpty (void) const;
  virtual EventId PeekNext (void) const;
  virtual EventId RemoveNext (void);
  virtual bool Remove (const EventId &ev);

private:
  typedef std::vector<std::pair<EventImpl *, Scheduler::EventKey> > BinaryHeap;

  inline uint32_t Parent (uint32_t id) const;
  uint32_t Sibling (uint32_t id) const;
  inline uint32_t LeftChild (uint32_t id) const;
  inline uint32_t RightChild (uint32_t id) const;
  inline uint32_t Root (void) const;
  /* Return the position in the array of the last element included in it. */
  uint32_t Last (void) const;
  inline bool IsRoot (uint32_t id) const;
  inline bool IsBottom (uint32_t id) const;
  inline bool IsLessStrictly (uint32_t a, uint32_t b) const;
  inline uint32_t Smallest (uint32_t a, uint32_t b) const;

  inline void Exch (uint32_t a, uint32_t b);
  void BottomUp (void);
  void TopDown (uint32_t start);

  BinaryHeap m_heap;
};

}; // namespace ns3


#endif /* SCHEDULER_HEAP_H */
