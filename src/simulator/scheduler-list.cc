/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
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
SchedulerList::GetEventId (Scheduler::EventKey key, EventsI i)
{
    assert (sizeof (i) <= sizeof (void *));
    void *internalIterator;
    memcpy ((char *)&(internalIterator), (char *)&i, sizeof (void *));
    EventImpl *ev = i->first;
    ev->SetInternalIterator (internalIterator);
    return EventId (ev, key.m_ns, key.m_uid);
}
SchedulerList::EventsI 
SchedulerList::GetIterator (EventId id)
{
    SchedulerList::EventsI i;
    assert (sizeof (i) <= sizeof (void *));
    EventImpl *ev = id.GetEventImpl ();
    void *internalIterator = ev->GetInternalIterator ();
    memcpy ((char *)&i, (char *)&(internalIterator), sizeof (void *));
    return i;
}


EventId
SchedulerList::RealInsert (EventImpl *event, Scheduler::EventKey key)
{
    Scheduler::EventKeyCompare compare;
    for (EventsI i = m_events.begin (); i != m_events.end (); i++) {
        if (compare (key, i->second)) {
            m_events.insert (i, std::make_pair (event, key));
            return GetEventId (key, i);
        }
    }
    m_events.push_back (std::make_pair (event, key));
    return GetEventId (key, --(m_events.end ()));
}
bool 
SchedulerList::RealIsEmpty (void) const
{
    return m_events.empty ();
}
EventImpl *
SchedulerList::RealPeekNext (void) const
{
    return m_events.front ().first;
}
Scheduler::EventKey
SchedulerList::RealPeekNextKey (void) const
{
    return m_events.front ().second;
}

void
SchedulerList::RealRemoveNext (void)
{
    m_events.pop_front ();
}

EventImpl *
SchedulerList::RealRemove (EventId id, Scheduler::EventKey *key)
{
    EventsI i = GetIterator (id);
    *key = i->second;
    assert (key->m_ns == id.GetNs () &&
            key->m_uid == id.GetUid ());
    EventImpl *ev = i->first;
    m_events.erase (i);
    return ev;
}

bool 
SchedulerList::RealIsValid (EventId id)
{
    EventsI i = GetIterator (id);
    Scheduler::EventKey key = i->second;
    return (key.m_ns == id.GetNs () &&
            key.m_uid == id.GetUid ());
    
}

}; // namespace ns3
