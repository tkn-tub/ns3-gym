/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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

#include "map-scheduler.h"
#include "event-impl.h"
#include "ns3/assert.h"
#include <string>

#define noTRACE_MAP 1

#ifdef TRACE_MAP
#include <iostream>
# define TRACE(x) \
std::cout << "MAP TRACE " << x << std::endl;
#else /* TRACE_MAP */
# define TRACE(format,...)
#endif /* TRACE_MAP */


namespace ns3 {

MapScheduler::MapScheduler ()
{}
MapScheduler::~MapScheduler ()
{}

void
MapScheduler::Insert (const EventId &id)
{
  // acquire a single ref
  EventImpl *event = id.PeekEventImpl ();
  event->Ref ();
  Scheduler::EventKey key;
  key.m_ts = id.GetTs ();
  key.m_uid = id.GetUid ();
  std::pair<EventMapI,bool> result;
  result = m_list.insert (std::make_pair (key, event));
  NS_ASSERT (result.second);
}

bool
MapScheduler::IsEmpty (void) const
{
  return m_list.empty ();
}

EventId
MapScheduler::PeekNext (void) const
{
  EventMapCI i = m_list.begin ();
  NS_ASSERT (i != m_list.end ());
  
  return EventId (i->second, i->first.m_ts, i->first.m_uid);
}
EventId
MapScheduler::RemoveNext (void)
{
  EventMapI i = m_list.begin ();
  std::pair<Scheduler::EventKey, EventImpl*> next = *i;
  m_list.erase (i);
  return EventId (Ptr<EventImpl> (next.second, false), next.first.m_ts, next.first.m_uid);
}

bool
MapScheduler::Remove (const EventId &id)
{
  Scheduler::EventKey key;
  key.m_ts = id.GetTs ();
  key.m_uid = id.GetUid ();
  EventMapI i = m_list.find (key);
  NS_ASSERT (i->second == id.PeekEventImpl ());
  // release single ref.
  i->second->Unref ();
  m_list.erase (i);
  return true;
}

}; // namespace ns3
