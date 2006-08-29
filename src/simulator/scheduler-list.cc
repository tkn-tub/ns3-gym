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

#include "scheduler-list.h"
#include "event.h"
#include <utility>
#include <cassert>

namespace yans {

SchedulerList::SchedulerList ()
{}
SchedulerList::~SchedulerList ()
{}

/*  !! WARNING !! 
 * This is a very nasty piece of code but it really should work
 * with pretty much any implementation of a std::list.
 * it relies on the fact that a std::list<>::iterator has a single
 * member variable, a pointer.
 */
void 
SchedulerList::store_in_event (Event ev, EventsI i)
{
	assert (sizeof (i) <= sizeof (Event));
	void *tag;
	strncpy ((char *)&(tag), (char *)&i, sizeof (void *));
	ev.set_tag (tag);
}
SchedulerList::EventsI 
SchedulerList::get_from_event (Event const ev)
{
	SchedulerList::EventsI i;
	assert (sizeof (i) <= sizeof (Event));
	void *tag = ev.get_tag ();
	strncpy ((char *)&i, (char *)&(tag), sizeof (void *));
	return i;
}


Event  
SchedulerList::insert (Event event, Scheduler::EventKey key)
{
	Scheduler::EventKeyCompare compare;
	for (EventsI i = m_events.begin (); i != m_events.end (); i++) {
		if (compare (key, i->second)) {
			m_events.insert (i, std::make_pair (event, key));
			store_in_event (event, i);
			return event;
		}
	}
	m_events.push_back (std::make_pair (event, key));
	store_in_event (event, --(m_events.end ()));
	return event;
}
bool 
SchedulerList::is_empty (void) const
{
	return m_events.empty ();
}
Event 
SchedulerList::peek_next (void) const
{
	assert (!is_empty ());
	return m_events.front ().first;
}
Scheduler::EventKey
SchedulerList::peek_next_key (void) const
{
	assert (!is_empty ());
	return m_events.front ().second;
}

void
SchedulerList::remove_next (void)
{
	m_events.pop_front ();
}

Scheduler::EventKey
SchedulerList::remove (Event const ev)
{
	EventsI i = get_from_event (ev);
	EventKey key = (*i).second;
	m_events.erase (get_from_event (ev));
	return key;
}

}; // namespace yans
