/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:f -*- */
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
SchedulerMap::storeInEvent (EventImpl *ev, EventMapI i) const
{
    void *tag;
    memcpy (&(tag), &i, sizeof (tag));
    ev->setInternalIterator (tag);
}
SchedulerMap::EventMapI
SchedulerMap::getFrom_event (EventImpl *ev) const
{
    EventMapI i;
    void *tag = ev->getInternalIterator ();
    memcpy (&i, &(tag), sizeof (i));
     return i;
}

EventId
SchedulerMap::realInsert (EventImpl *event, Scheduler::EventKey key)
{
    std::pair<EventMapI,bool> result = m_list.insert (std::make_pair (key, event));
    assert (result.second);
    storeInEvent (event, result.first);
    return EventId (event, key.m_ns, key.m_uid);
}

bool
SchedulerMap::realIsEmpty (void) const
{
    return m_list.empty ();
}

EventImpl *
SchedulerMap::realPeekNext (void) const
{
    EventMapCI i = m_list.begin ();
    assert (i != m_list.end ());
    return (*i).second;
}
Scheduler::EventKey
SchedulerMap::realPeekNextKey (void) const
{
    EventMapCI i = m_list.begin ();
    assert (i != m_list.end ());
    return (*i).first;
}
void
SchedulerMap::realRemoveNext (void)
{
    m_list.erase (m_list.begin ());
}

EventImpl *
SchedulerMap::realRemove (EventId id, Scheduler::EventKey *key)
{
    EventMapI i = getFrom_event (id.getEventImpl ());
    *key = i->first;
    m_list.erase (i);
    return i->second;
}

bool
SchedulerMap::realIsValid (EventId id)
{
    EventMapI i = getFrom_event (id.getEventImpl ());
    Scheduler::EventKey key = i->first;
    return (key.m_ns == id.getNs () &&
        key.m_uid == id.getUid ());
}


}; // namespace ns3
