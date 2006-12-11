/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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


EventId
SchedulerList::RealInsert (EventImpl *event, Scheduler::EventKey key)
{
  Scheduler::EventKeyCompare compare;
  for (EventsI i = m_events.begin (); i != m_events.end (); i++) 
    {
      if (compare (key, i->second)) 
        {
          m_events.insert (i, std::make_pair (event, key));
          return EventId (event, key.m_ns, key.m_uid);
        }
    }
  m_events.push_back (std::make_pair (event, key));
  return EventId (event, key.m_ns, key.m_uid);
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
  for (EventsI i = m_events.begin (); i != m_events.end (); i++) 
    {
      if (i->second.m_uid == id.GetUid ())
        {
          EventImpl *retval = i->first;
          assert (id.GetEventImpl () == retval);
          key->m_ns = id.GetNs ();
          key->m_uid = id.GetUid ();
          m_events.erase (i);
          return retval;
        }
    }
  assert (false);
  return 0;
}

bool 
SchedulerList::RealIsValid (EventId id)
{
  return true;
}

}; // namespace ns3
