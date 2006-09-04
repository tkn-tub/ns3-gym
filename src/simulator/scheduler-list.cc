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
#include "event-impl.h"
#include <utility>
#include <cassert>

namespace ns3 {

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
EventId
SchedulerList::get_event_id (Scheduler::EventKey key, EventsI i)
{
	assert (sizeof (i) <= sizeof (void *));
	void *internal_iterator;
	memcpy ((char *)&(internal_iterator), (char *)&i, sizeof (void *));
	EventImpl *ev = i->first;
	ev->set_internal_iterator (internal_iterator);
	return EventId (ev, key.m_ns, key.m_uid);
}
SchedulerList::EventsI 
SchedulerList::get_iterator (EventId id)
{
	SchedulerList::EventsI i;
	assert (sizeof (i) <= sizeof (void *));
	EventImpl *ev = id.get_event_impl ();
	void *internal_iterator = ev->get_internal_iterator ();
	memcpy ((char *)&i, (char *)&(internal_iterator), sizeof (void *));
	return i;
}


EventId
SchedulerList::insert (EventImpl *event, Scheduler::EventKey key)
{
	Scheduler::EventKeyCompare compare;
	for (EventsI i = m_events.begin (); i != m_events.end (); i++) {
		if (compare (key, i->second)) {
			m_events.insert (i, std::make_pair (event, key));
			return get_event_id (key, i);
		}
	}
	m_events.push_back (std::make_pair (event, key));
	return get_event_id (key, --(m_events.end ()));
}
bool 
SchedulerList::is_empty (void) const
{
	return m_events.empty ();
}
EventImpl *
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

EventImpl *
SchedulerList::remove (EventId id, Scheduler::EventKey *key)
{
	EventsI i = get_iterator (id);
	*key = i->second;
	assert (key->m_ns == id.get_ns () &&
		key->m_uid == id.get_uid ());
	EventImpl *ev = i->first;
	m_events.erase (i);
	return ev;
}

bool 
SchedulerList::is_valid (EventId id)
{
	EventsI i = get_iterator (id);
	Scheduler::EventKey key = i->second;
	return (key.m_ns == id.get_ns () &&
		key.m_uid == id.get_uid ());
	
}

}; // namespace ns3
