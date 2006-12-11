/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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


EventId
SchedulerMap::RealInsert (EventImpl *event, Scheduler::EventKey key)
{
  std::pair<EventMapI,bool> result = m_list.insert (std::make_pair (key, event));
  assert (result.second);
  return EventId (event, key.m_ns, key.m_uid);
}

bool
SchedulerMap::RealIsEmpty (void) const
{
  return m_list.empty ();
}

EventImpl *
SchedulerMap::RealPeekNext (void) const
{
  EventMapCI i = m_list.begin ();
  assert (i != m_list.end ());
  return (*i).second;
}
Scheduler::EventKey
SchedulerMap::RealPeekNextKey (void) const
{
  EventMapCI i = m_list.begin ();
  assert (i != m_list.end ());
  return (*i).first;
}
void
SchedulerMap::RealRemoveNext (void)
{
  m_list.erase (m_list.begin ());
}

EventImpl *
SchedulerMap::RealRemove (EventId id, Scheduler::EventKey *key)
{
  key->m_ns = id.GetNs ();
  key->m_uid = id.GetUid ();
  EventMapI i = m_list.find (*key);
  EventImpl *retval = i->second;
  m_list.erase (i);
  return retval;
}

bool
SchedulerMap::RealIsValid (EventId id)
{
  return true;
}


}; // namespace ns3
