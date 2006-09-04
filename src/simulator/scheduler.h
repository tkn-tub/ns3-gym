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

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "event-id.h"

namespace ns3 {

class EventImpl;

class Scheduler {
 public:
	struct EventKey {
		uint64_t m_ns;
		uint32_t m_uid;
	};
	class EventKeyCompare {
	public:
		bool operator () (struct EventKey a, struct EventKey b);
	};

	virtual ~Scheduler () = 0;

	EventId insert (EventImpl *event, EventKey key);
	bool is_empty (void) const;
	EventImpl *peek_next (void) const;
	Scheduler::EventKey peek_next_key (void) const ;
	void remove_next (void);
	EventImpl *remove (EventId id, EventKey *key);
	bool is_valid (EventId id);

private:
	virtual EventId real_insert (EventImpl *event, EventKey key) = 0;
	virtual bool real_is_empty (void) const = 0;
	virtual EventImpl *real_peek_next (void) const = 0;
	virtual Scheduler::EventKey real_peek_next_key (void) const = 0;
	virtual void real_remove_next (void) = 0;
	virtual EventImpl *real_remove (EventId id, EventKey *key) = 0;
	virtual bool real_is_valid (EventId id) = 0;
};

}; // namespace ns3


#endif /* SCHEDULER_H */
