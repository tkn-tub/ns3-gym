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
#include <list>
#include <utility>
#include <stdint.h>

namespace ns3 {

class Event;

class SchedulerList : public Scheduler {
 public:
	SchedulerList ();
	virtual ~SchedulerList ();

	virtual Event insert (Event event, EventKey key);
	virtual bool is_empty (void) const;
	virtual Event peek_next (void) const;
	virtual Scheduler::EventKey peek_next_key (void) const;
	virtual void remove_next (void);
	virtual Scheduler::EventKey remove (Event const ev);

 private:
	typedef std::list<std::pair<Event, EventKey> > Events;
	typedef std::list<std::pair<Event, EventKey> >::iterator EventsI;
	void store_in_event (Event ev, EventsI i);
	EventsI get_from_event (Event const ev);
	Events m_events;
};

}; // namespace ns3


#endif /* SCHEDULER_LIST_H */
