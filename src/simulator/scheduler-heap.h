/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
 * All rights reserved.
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

class SchedulerHeap : public Scheduler {
public:
  SchedulerHeap ();
  virtual ~SchedulerHeap ();

private:
  virtual EventId RealInsert (EventImpl *event, Scheduler::EventKey key);
  virtual bool RealIsEmpty (void) const;
  virtual EventImpl *RealPeekNext (void) const;
  virtual Scheduler::EventKey RealPeekNextKey (void) const;
  virtual void RealRemoveNext (void);
  virtual EventImpl *RealRemove (EventId ev, Scheduler::EventKey *key);
  virtual bool RealIsValid (EventId id);

  typedef std::vector<std::pair<EventImpl *, Scheduler::EventKey> > BinaryHeap;
  inline void StoreInEvent (EventImpl *ev, uint32_t index) const;
  uint32_t GetFromEvent (EventImpl *ev) const;

  inline uint32_t Parent (uint32_t id) const;
  uint32_t Sibling (uint32_t id) const;
  inline uint32_t LeftChild (uint32_t id) const;
  inline uint32_t RightChild (uint32_t id) const;
  inline uint32_t Root (void) const;
  uint32_t Last (void) const;
  inline bool IsRoot (uint32_t id) const;
  inline bool IsBottom (uint32_t id) const;
  inline bool IsLess (uint32_t a, uint32_t b);
  inline uint32_t Smallest (uint32_t a, uint32_t b);

  inline void Exch (uint32_t a, uint32_t b);
  void BottomUp (void);
  void TopDown (void);

  BinaryHeap m_heap;
};

}; // namespace ns3


#endif /* SCHEDULER_HEAP_H */
