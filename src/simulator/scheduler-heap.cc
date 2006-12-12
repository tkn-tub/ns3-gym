/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 * Copyright (c) 2005 Mathieu Lacage
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
 *
 * This code started as a c++ translation of a java-based code written in 2005
 * to implement a heap sort. Which explains the Copyright Mathieu Lacage at the
 * top of this file.
 *
 * What is smart about this code ?
 *  - it does not use the index 0 in the array to avoid having to convert
 *    C-style array indexes (which start at zero) and heap-style indexes
 *    (which start at 1). This is why _all_ indexes start at 1, and that
 *    the index of the root is 1.
 *  - It uses a slightly non-standard while loop for top-down heapify
 *    to move one if statement out of the loop.
 */

#include "scheduler-heap.h"
#include "event-impl.h"
#include <cassert>

#define noTRACE_HEAP 1

#ifdef TRACE_HEAP
#include <iostream>
# define TRACE(x) \
std::cout << "HEAP TRACE " << x << std::endl;
#else /* TRACE_HEAP */
# define TRACE(format,...)
#endif /* TRACE_HEAP */




namespace ns3 {

SchedulerHeap::SchedulerHeap ()
{
  // we purposedly waste an item at the start of
  // the array to make sure the indexes in the
  // array start at one.
  Scheduler::EventKey emptyKey = {0,0};
  m_heap.push_back (std::make_pair (static_cast<EventImpl *>(0), emptyKey));
}

SchedulerHeap::~SchedulerHeap ()
{}

uint32_t 
SchedulerHeap::Parent (uint32_t id) const
{
  return id / 2;
}
uint32_t 
SchedulerHeap::Sibling (uint32_t id) const
{
  return id + 1;
}
uint32_t 
SchedulerHeap::LeftChild (uint32_t id) const
{
  return id * 2;
}
uint32_t 
SchedulerHeap::RightChild (uint32_t id) const
{
  return id * 2 + 1;
}

uint32_t
SchedulerHeap::Root (void) const
{
  return 1;
}

bool
SchedulerHeap::IsRoot (uint32_t id) const
{
  return (id == Root ())?true:false;
}

uint32_t
SchedulerHeap::Last (void) const
{
  return m_heap.size () - 1;
}


bool
SchedulerHeap::IsBottom (uint32_t id) const
{
  return (id >= m_heap.size ())?true:false;
}

void
SchedulerHeap::Exch (uint32_t a, uint32_t b) 
{
  assert (b < m_heap.size () && a < m_heap.size ());
  TRACE ("Exch " << a << ", " << b);
  std::pair<EventImpl*, Scheduler::EventKey> tmp (m_heap[a]);
  m_heap[a] = m_heap[b];
  m_heap[b] = tmp;
}

bool
SchedulerHeap::IsLower (Scheduler::EventKey const*a, Scheduler::EventKey const*b) const
{
  if (a->m_ns < b->m_ns)
    {
      return true;
    }
  else if (a->m_ns == b->m_ns &&
           a->m_uid < b->m_uid)
    {
      return true;
    }
  else
    {
      return false;
    }
}

bool
SchedulerHeap::IsLess (uint32_t a, uint32_t b) const
{
  return IsLower (&m_heap[a].second, &m_heap[b].second);
}

uint32_t 
SchedulerHeap::Smallest (uint32_t a, uint32_t b) const
{
  return IsLess (a,b)?a:b;
}

bool
SchedulerHeap::RealIsEmpty (void) const
{
  return (m_heap.size () == 1)?true:false;
}

void
SchedulerHeap::BottomUp (void)
{
  uint32_t index = Last ();
  while (!IsRoot (index) && 
         IsLess (index, Parent (index))) 
    { 
      Exch(index, Parent (index)); 
      index = Parent (index); 
    }
}

void
SchedulerHeap::TopDown (void)
{
  uint32_t index = Root ();
  uint32_t right = RightChild (index);
  while (!IsBottom (right)) 
    {
      uint32_t left = LeftChild (index);
      uint32_t tmp = Smallest (left, right);
      if (IsLess (index, tmp)) 
        {
          return;
        }
      Exch (index, tmp);
      index = tmp;
      right = RightChild (index);
    }
  if (IsBottom (index)) 
    {
      return;
    }
  assert (!IsBottom (index));
  uint32_t left = LeftChild (index);
  if (IsBottom (left)) 
    {
      return;
    }
  if (IsLess (index, left)) 
    {
      return;
    }
  Exch (index, left);
}


EventId
SchedulerHeap::RealInsert (EventImpl *event, Scheduler::EventKey key)
{
  m_heap.push_back (std::make_pair (event, key));
  BottomUp ();
  return EventId (event, key.m_ns, key.m_uid);
}

EventImpl *
SchedulerHeap::RealPeekNext (void) const
{
  return m_heap[Root ()].first;
}
Scheduler::EventKey
SchedulerHeap::RealPeekNextKey (void) const
{
  return m_heap[Root ()].second;
}
void     
SchedulerHeap::RealRemoveNext (void)
{
  Exch (Root (), Last ());
  m_heap.pop_back ();
  TopDown ();
}


EventImpl *
SchedulerHeap::RealRemove (EventId id, Scheduler::EventKey *key)
{
  uint32_t top;
  uint32_t bottom;
  bottom = 1;
  top = Last ();
  key->m_ns = id.GetNs ();
  key->m_uid = id.GetUid ();
  while (top != bottom)
    {
      uint32_t i = bottom + (top - bottom) / 2;
      if (IsLower (key, &m_heap[i].second))
        {
          top = i;
        }
      else
        {
          bottom = i;
        }
    }
  assert (top == bottom);
  assert (m_heap[top].second.m_uid == id.GetUid ());
  EventImpl *retval = m_heap[top].first;
  Exch (top, Last ());
  m_heap.pop_back ();
  TopDown ();
  return retval;
}

bool 
SchedulerHeap::RealIsValid (EventId id)
{
  return true;
}
}; // namespace ns3

#ifdef RUN_SELF_TESTS

#include "ns3/test.h"

namespace ns3 {

class SchedulerHeapTest : public Test 
{
public:
  SchedulerHeapTest ();
  virtual ~SchedulerHeapTest ();
  virtual bool RunTests (void);
};

SchedulerHeapTest::SchedulerHeapTest ()
  : Test ("SchedulerHeap")
{}
SchedulerHeapTest::~SchedulerHeapTest ()
{}

bool 
SchedulerHeapTest::RunTests (void)
{
  bool ok = true;
  return ok;
}

static SchedulerHeapTest g_schedulerHeapTest;

}; // namespace ns3


#endif /* RUN_SELF_TESTS */
