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
  inline bool IsLowerStrictly (Scheduler::EventKey const*a, Scheduler::EventKey const*b) const;
  inline bool IsLessStrictly (uint32_t a, uint32_t b) const;
  inline uint32_t Smallest (uint32_t a, uint32_t b) const;

  inline void Exch (uint32_t a, uint32_t b);
  void BottomUp (void);
  void TopDown (uint32_t start);

  BinaryHeap m_heap;
};

}; // namespace ns3


#endif /* SCHEDULER_HEAP_H */
