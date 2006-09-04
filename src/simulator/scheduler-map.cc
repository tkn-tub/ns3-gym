/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2006 INRIA
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
 * The idea to use a std c++ map came from GTNetS
 */

#include "scheduler-map.h"
#include "event-impl.h"
#include <cassert>

#define noTRACE_MAP 1

#ifdef TRACE_MAP
#include <iostream>
# define TRACE(x) \
std::cout << "MAP TRACE " << x << std::endl;
#else /* TRACE_MAP */
# define TRACE(format,...)
#endif /* TRACE_MAP */


namespace ns3 {


SchedulerMap::SchedulerMap ()
{}
SchedulerMap::~SchedulerMap ()
{}


void 
SchedulerMap::store_in_event (EventImpl *ev, EventMapI i) const
{
	void *tag;
	memcpy (&(tag), &i, sizeof (tag));
	ev->set_internal_iterator (tag);
}
SchedulerMap::EventMapI
SchedulerMap::get_from_event (EventImpl *ev) const
{
	EventMapI i;
	void *tag = ev->get_internal_iterator ();
	memcpy (&i, &(tag), sizeof (i));
 	return i;
}

EventId
SchedulerMap::insert (EventImpl *event, Scheduler::EventKey key)
{
	std::pair<EventMapI,bool> result = m_list.insert (std::make_pair (key, event));
	assert (result.second);
	store_in_event (event, result.first);
	return EventId (event, key.m_ns, key.m_uid);
}

bool
SchedulerMap::is_empty (void) const
{
	return m_list.empty ();
}

EventImpl *
SchedulerMap::peek_next (void) const
{
	assert (!is_empty ());
	EventMapCI i = m_list.begin ();
	assert (i != m_list.end ());
	return (*i).second;
}
Scheduler::EventKey
SchedulerMap::peek_next_key (void) const
{
	assert (!is_empty ());
	EventMapCI i = m_list.begin ();
	assert (i != m_list.end ());
	return (*i).first;
}
void
SchedulerMap::remove_next (void)
{
	assert (!is_empty ());
	m_list.erase (m_list.begin ());
}

EventImpl *
SchedulerMap::remove (EventId id, Scheduler::EventKey *key)
{
	assert (!is_empty ());
	EventMapI i = get_from_event (id.get_event_impl ());
	*key = i->first;
	m_list.erase (i);
	return i->second;
}

bool
SchedulerMap::is_valid (EventId id)
{
	EventMapI i = get_from_event (id.get_event_impl ());
	Scheduler::EventKey key = i->first;
	return (key.m_ns == id.get_ns () &&
		key.m_uid == id.get_uid ());
}


}; // namespace ns3
