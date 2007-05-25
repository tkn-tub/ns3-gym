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
#include "scheduler-factory.h"
#include "event-impl.h"
#include <utility>
#include <string>
#include "ns3/assert.h"

namespace ns3 {

static class SchedulerListFactory : public SchedulerFactory 
{
public:
  SchedulerListFactory ()
  {
    SchedulerFactory::AddDefault (this, "List");
  }
private:
  virtual Scheduler *DoCreate (void) const
  {
    return new SchedulerList ();
  }
} g_schedulerListFactory;


SchedulerList::SchedulerList ()
{}
SchedulerList::~SchedulerList ()
{}

bool 
SchedulerList::IsLower (Scheduler::EventKey const*a, Scheduler::EventKey const*b) const
{
  if (a->m_ts < b->m_ts)
    {
      return true;
    }
  else if (a->m_ts == b->m_ts &&
           a->m_uid < b->m_uid)
    {
      return true;
    }
  else
    {
      return false;
    }
}

EventId
SchedulerList::RealInsert (EventImpl *event, Scheduler::EventKey key)
{
  for (EventsI i = m_events.begin (); i != m_events.end (); i++) 
    {
      if (IsLower (&key, &i->second))
        {
          m_events.insert (i, std::make_pair (event, key));
          return EventId (event, key.m_ts, key.m_uid);
        }
    }
  m_events.push_back (std::make_pair (event, key));
  return EventId (event, key.m_ts, key.m_uid);
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
          NS_ASSERT (id.GetEventImpl () == retval);
          key->m_ts = id.GetTs ();
          key->m_uid = id.GetUid ();
          m_events.erase (i);
          return retval;
        }
    }
  NS_ASSERT (false);
  return 0;
}

bool 
SchedulerList::RealIsValid (EventId id)
{
  return true;
}

}; // namespace ns3
