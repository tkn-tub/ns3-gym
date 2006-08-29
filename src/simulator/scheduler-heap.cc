/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
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
#include "event.h"
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
	Scheduler::EventKey empty_key = {0,0};
	m_heap.push_back (std::make_pair ((Event )0, empty_key));
}

SchedulerHeap::~SchedulerHeap ()
{}

 
void 
SchedulerHeap::store_in_event (Event ev, uint32_t index) const
{
	ev.set_tag ((void *)index);
}
uint32_t 
SchedulerHeap::get_from_event (Event const ev) const
{
 	return (uint32_t)ev.get_tag ();
}


uint32_t 
SchedulerHeap::parent (uint32_t id) const
{
	return id / 2;
}
uint32_t 
SchedulerHeap::sibling (uint32_t id) const
{
	return id + 1;
}
uint32_t 
SchedulerHeap::left_child (uint32_t id) const
{
	return id * 2;
}
uint32_t 
SchedulerHeap::right_child (uint32_t id) const
{
	return id * 2 + 1;
}

uint32_t
SchedulerHeap::root (void) const
{
	return 1;
}

bool
SchedulerHeap::is_root (uint32_t id) const
{
	return (id == root ())?true:false;
}

uint32_t
SchedulerHeap::last (void) const
{
	return m_heap.size () - 1;
}


bool
SchedulerHeap::is_bottom (uint32_t id) const
{
	return (id >= m_heap.size ())?true:false;
}

void
SchedulerHeap::exch (uint32_t a, uint32_t b) 
{
	assert (b < m_heap.size () && a < m_heap.size ());
	TRACE ("exch " << a << ", " << b);
#if 1
	std::swap (m_heap[a].second, m_heap[b].second);
	std::swap (m_heap[a].first.m_impl, m_heap[b].first.m_impl);
#else
	std::pair<Event , Scheduler::EventKey> tmp (m_heap[a]);
	m_heap[a] = m_heap[b];
	m_heap[b] = tmp;
#endif
	store_in_event (m_heap[a].first, a);
	store_in_event (m_heap[b].first, b);
}

bool
SchedulerHeap::is_less (uint32_t a, uint32_t b)
{
	Scheduler::EventKeyCompare compare;
	return compare (m_heap[a].second, m_heap[b].second);
}

uint32_t 
SchedulerHeap::smallest (uint32_t a, uint32_t b)
{
	return is_less (a,b)?a:b;
}

bool
SchedulerHeap::is_empty (void) const
{
	return (m_heap.size () == 1)?true:false;
}

void
SchedulerHeap::bottom_up (void)
{
	uint32_t index = last ();
	while (!is_root (index) && 
	       is_less (index, parent (index))) { 
		exch(index, parent (index)); 
		index = parent (index); 
	}
}

void
SchedulerHeap::top_down (void)
{
	uint32_t index = root ();
	uint32_t right = right_child (index);
	while (!is_bottom (right)) {
		uint32_t left = left_child (index);
		uint32_t tmp = smallest (left, right);
		if (is_less (index, tmp)) {
			return;
		}
		exch (index, tmp);
		index = tmp;
		right = right_child (index);
	}
	if (is_bottom (index)) {
		return;
	}
	assert (!is_bottom (index));
	uint32_t left = left_child (index);
	if (is_bottom (left)) {
		return;
	}
	if (is_less (index, left)) {
		return;
	}
	exch (index, left);
}


Event  
SchedulerHeap::insert (Event event, Scheduler::EventKey key)
{
	m_heap.push_back (std::make_pair (event, key));
	store_in_event (event, last ());
	bottom_up ();
	return event;
}

Event
SchedulerHeap::peek_next (void) const
{
	assert (!is_empty ());
	return m_heap[root ()].first;
}
Scheduler::EventKey
SchedulerHeap::peek_next_key (void) const
{
	assert (!is_empty ());
	return m_heap[root ()].second;
}
void     
SchedulerHeap::remove_next (void)
{
	assert (!is_empty ());
	exch (root (), last ());
	m_heap.pop_back ();
	top_down ();
}

Scheduler::EventKey
SchedulerHeap::remove (Event const ev)
{
	uint32_t i = get_from_event (ev);
	EventKey key = m_heap[i].second;
	exch (i, last ());
	m_heap.pop_back ();
	top_down ();
	return key;
}

}; // namespace ns3
