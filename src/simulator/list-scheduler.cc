/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#include "list-scheduler.h"
#include "event-impl.h"
#include <utility>
#include <string>
#include "ns3/assert.h"

namespace ns3 {


ListScheduler::ListScheduler ()
{}
ListScheduler::~ListScheduler ()
{}

bool 
ListScheduler::IsLower (Scheduler::EventKey const*a, Scheduler::EventKey const*b) const
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

void
ListScheduler::Insert (const EventId &id)
{
  Scheduler::EventKey key;
  // acquire refcount on EventImpl
  EventImpl *event = id.PeekEventImpl ();
  event->Ref ();
  key.m_ts = id.GetTs ();
  key.m_uid = id.GetUid ();
  for (EventsI i = m_events.begin (); i != m_events.end (); i++) 
    {
      if (IsLower (&key, &i->second))
        {
          m_events.insert (i, std::make_pair (event, key));
          return;
        }
    }
  m_events.push_back (std::make_pair (event, key));
}
bool 
ListScheduler::IsEmpty (void) const
{
  return m_events.empty ();
}
EventId
ListScheduler::PeekNext (void) const
{
  std::pair<EventImpl *, EventKey> next = m_events.front ();
  return EventId (next.first, next.second.m_ts, next.second.m_uid);
}

EventId
ListScheduler::RemoveNext (void)
{
  std::pair<EventImpl *, EventKey> next = m_events.front ();
  m_events.pop_front ();
  return EventId (Ptr<EventImpl> (next.first,false), next.second.m_ts, next.second.m_uid);
}

bool
ListScheduler::Remove (const EventId &id)
{
  for (EventsI i = m_events.begin (); i != m_events.end (); i++) 
    {
      if (i->second.m_uid == id.GetUid ())
        {
          NS_ASSERT (id.PeekEventImpl () == i->first);
          // release single acquire ref.
          i->first->Unref ();
          m_events.erase (i);
          return true;
        }
    }
  NS_ASSERT (false);
  return false;
}

}; // namespace ns3
