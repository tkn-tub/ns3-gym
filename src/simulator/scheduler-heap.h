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

namespace ns3 {

class EventHolder;

class SchedulerHeap : public Scheduler {
public:
	SchedulerHeap ();
	virtual ~SchedulerHeap ();

private:
	virtual EventId realInsert (EventImpl *event, Scheduler::EventKey key);
	virtual bool realIsEmpty (void) const;
	virtual EventImpl *realPeekNext (void) const;
	virtual Scheduler::EventKey realPeekNextKey (void) const;
	virtual void realRemoveNext (void);
	virtual EventImpl *realRemove (EventId ev, Scheduler::EventKey *key);
	virtual bool realIsValid (EventId id);

	typedef std::vector<std::pair<EventImpl *, Scheduler::EventKey> > BinaryHeap;
	inline void storeInEvent (EventImpl *ev, uint32_t index) const;
	uint32_t getFrom_event (EventImpl *ev) const;

	inline uint32_t parent (uint32_t id) const;
	uint32_t sibling (uint32_t id) const;
	inline uint32_t leftChild (uint32_t id) const;
	inline uint32_t rightChild (uint32_t id) const;
	inline uint32_t root (void) const;
	uint32_t last (void) const;
	inline bool isRoot (uint32_t id) const;
	inline bool isBottom (uint32_t id) const;
	inline bool isLess (uint32_t a, uint32_t b);
	inline uint32_t smallest (uint32_t a, uint32_t b);

	inline void exch (uint32_t a, uint32_t b);
	void bottom_up (void);
	void topDown (void);

	BinaryHeap m_heap;
};

}; // namespace ns3


#endif /* SCHEDULER_HEAP_H */
