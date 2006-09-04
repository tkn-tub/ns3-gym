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

#ifndef SCHEDULER_LIST_H
#define SCHEDULER_LIST_H

#include "scheduler.h"
#include "event-id.h"
#include <list>
#include <utility>
#include <stdint.h>

namespace ns3 {

class EventImpl;

class SchedulerList : public Scheduler {
 public:
	SchedulerList ();
	virtual ~SchedulerList ();

 private:
	virtual EventId real_insert (EventImpl *event, EventKey key);
	virtual bool real_is_empty (void) const;
	virtual EventImpl *real_peek_next (void) const;
	virtual Scheduler::EventKey real_peek_next_key (void) const;
	virtual void real_remove_next (void);
	virtual EventImpl *real_remove (EventId ev, Scheduler::EventKey *key);
	virtual bool real_is_valid (EventId id);

	typedef std::list<std::pair<EventImpl*, EventKey> > Events;
	typedef std::list<std::pair<EventImpl*, EventKey> >::iterator EventsI;
	EventId get_event_id (Scheduler::EventKey key, EventsI i);
	EventsI get_iterator (EventId id);
	Events m_events;
};

}; // namespace ns3


#endif /* SCHEDULER_LIST_H */
