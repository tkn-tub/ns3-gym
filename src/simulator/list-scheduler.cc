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

void
ListScheduler::Insert (const Event &ev)
{
  // acquire refcount on EventImpl
  ev.impl->Ref ();
  for (EventsI i = m_events.begin (); i != m_events.end (); i++) 
    {
      if (ev.key < i->key)
        {
          m_events.insert (i, ev);
          return;
        }
    }
  m_events.push_back (ev);
}
bool 
ListScheduler::IsEmpty (void) const
{
  return m_events.empty ();
}
EventId
ListScheduler::PeekNext (void) const
{
  Event next = m_events.front ();
  return EventId (next.impl, next.key.m_ts, next.key.m_uid);
}

EventId
ListScheduler::RemoveNext (void)
{
  Event next = m_events.front ();
  m_events.pop_front ();
  return EventId (Ptr<EventImpl> (next.impl,false), next.key.m_ts, next.key.m_uid);
}

bool
ListScheduler::Remove (const EventId &id)
{
  for (EventsI i = m_events.begin (); i != m_events.end (); i++) 
    {
      if (i->key.m_uid == id.GetUid ())
        {
          NS_ASSERT (id.PeekEventImpl () == i->impl);
          // release single acquire ref.
          i->impl->Unref ();
          m_events.erase (i);
          return true;
        }
    }
  NS_ASSERT (false);
  return false;
}

} // namespace ns3
