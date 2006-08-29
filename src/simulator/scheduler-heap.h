/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
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

namespace yans {

class Event;
class EventHolder;

class SchedulerHeap : public Scheduler {
public:
	SchedulerHeap ();
	virtual ~SchedulerHeap ();

	virtual Event insert (Event event, Scheduler::EventKey key);
	virtual bool is_empty (void) const;
	virtual Event peek_next (void) const;
	virtual Scheduler::EventKey peek_next_key (void) const;
	virtual void remove_next (void);
	virtual Scheduler::EventKey remove (Event const ev);
private:
	typedef std::vector<std::pair<Event, Scheduler::EventKey> > BinaryHeap;
	inline void store_in_event (Event ev, uint32_t index) const;
	uint32_t get_from_event (Event const ev) const;

	inline uint32_t parent (uint32_t id) const;
	uint32_t sibling (uint32_t id) const;
	inline uint32_t left_child (uint32_t id) const;
	inline uint32_t right_child (uint32_t id) const;
	inline uint32_t root (void) const;
	uint32_t last (void) const;
	inline bool is_root (uint32_t id) const;
	inline bool is_bottom (uint32_t id) const;
	inline bool is_less (uint32_t a, uint32_t b);
	inline uint32_t smallest (uint32_t a, uint32_t b);

	inline void exch (uint32_t a, uint32_t b);
	void bottom_up (void);
	void top_down (void);

	BinaryHeap m_heap;
};

}; // namespace yans


#endif /* SCHEDULER_HEAP_H */
