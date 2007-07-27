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
#include "scheduler-factory.h"
#include "event-impl.h"
#include "ns3/assert.h"

#include <string>
#define noTRACE_HEAP 1

#ifdef TRACE_HEAP
#include <iostream>
# define TRACE(x) \
std::cout << "HEAP TRACE " << x << std::endl;
#else /* TRACE_HEAP */
# define TRACE(format,...)
#endif /* TRACE_HEAP */




namespace ns3 {

static class SchedulerHeapFactory : public SchedulerFactory 
{
public:
  SchedulerHeapFactory ()
  {
    SchedulerFactory::Add (this, "BinaryHeap");
  }
private:
  virtual Scheduler *DoCreate (void) const
  {
    return new SchedulerHeap ();
  }
} g_schedulerHeapFactory;


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
  NS_ASSERT (b < m_heap.size () && a < m_heap.size ());
  TRACE ("Exch " << a << ", " << b);
  std::pair<EventImpl*, Scheduler::EventKey> tmp (m_heap[a]);
  m_heap[a] = m_heap[b];
  m_heap[b] = tmp;
}

bool
SchedulerHeap::IsLowerStrictly (Scheduler::EventKey const*a, Scheduler::EventKey const*b) const
{
  if (a->m_ts < b->m_ts)
    {
      return true;
    }
  else if (a->m_ts > b->m_ts)
    {
      return false;
    } 
  else if (a->m_uid < b->m_uid)
    {
      return true;
    }
  else
    {
      return false;
    }
}

bool
SchedulerHeap::IsLessStrictly (uint32_t a, uint32_t b) const
{
  return IsLowerStrictly (&m_heap[a].second, &m_heap[b].second);
}

uint32_t 
SchedulerHeap::Smallest (uint32_t a, uint32_t b) const
{
  return IsLessStrictly (a,b)?a:b;
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
         IsLessStrictly (index, Parent (index))) 
    { 
      Exch(index, Parent (index)); 
      index = Parent (index); 
    }
}

void
SchedulerHeap::TopDown (uint32_t start)
{
  uint32_t index = start;
  uint32_t right = RightChild (index);
  while (!IsBottom (right)) 
    {
      uint32_t left = LeftChild (index);
      uint32_t tmp = Smallest (left, right);
      if (IsLessStrictly (index, tmp)) 
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
  NS_ASSERT (!IsBottom (index));
  uint32_t left = LeftChild (index);
  if (IsBottom (left)) 
    {
      return;
    }
  if (IsLessStrictly (index, left)) 
    {
      return;
    }
  Exch (index, left);
}


void
SchedulerHeap::RealInsert (EventId id)
{
  EventImpl *event = id.PeekEventImpl ();
  Scheduler::EventKey key;
  key.m_ts = id.GetTs ();
  key.m_uid = id.GetUid ();
  m_heap.push_back (std::make_pair (event, key));
  BottomUp ();
}

EventId
SchedulerHeap::RealPeekNext (void) const
{
  std::pair<EventImpl *,Scheduler::EventKey> next = m_heap[Root ()];
  return EventId (next.first, next.second.m_ts, next.second.m_uid);
}
void     
SchedulerHeap::RealRemoveNext (void)
{
  Exch (Root (), Last ());
  m_heap.pop_back ();
  TopDown (Root ());
}


bool
SchedulerHeap::RealRemove (EventId id)
{
  uint32_t uid = id.GetUid ();
  for (uint32_t i = 1; i < m_heap.size (); i++)
    {
      if (uid == m_heap[i].second.m_uid)
        {
          NS_ASSERT (m_heap[i].first == id.PeekEventImpl ());
          Exch (i, Last ());
          m_heap.pop_back ();
          TopDown (i);
          return true;
        }
    }
  NS_ASSERT (false);
  // quiet compiler
  return false;
}

}; // namespace ns3

