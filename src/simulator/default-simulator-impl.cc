/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

#include "simulator.h"
#include "default-simulator-impl.h"
#include "scheduler.h"
#include "event-impl.h"

#include "ns3/ptr.h"
#include "ns3/pointer.h"
#include "ns3/assert.h"
#include "ns3/log.h"

#include <math.h>

NS_LOG_COMPONENT_DEFINE ("DefaultSimulatorImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (DefaultSimulatorImpl);

TypeId
DefaultSimulatorImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DefaultSimulatorImpl")
    .SetParent<Object> ()
    .AddConstructor<DefaultSimulatorImpl> ()
    ;
  return tid;
}

DefaultSimulatorImpl::DefaultSimulatorImpl ()
{
  m_stop = false;
  // uids are allocated from 4.
  // uid 0 is "invalid" events
  // uid 1 is "now" events
  // uid 2 is "destroy" events
  m_uid = 4; 
  // before ::Run is entered, the m_currentUid will be zero
  m_currentUid = 0;
  m_currentTs = 0;
  m_unscheduledEvents = 0;
}

DefaultSimulatorImpl::~DefaultSimulatorImpl ()
{
  while (!m_events->IsEmpty ())
    {
      Scheduler::Event next = m_events->RemoveNext ();
      next.impl->Unref ();
    }
  m_events = 0;
}

void
DefaultSimulatorImpl::Destroy ()
{
  while (!m_destroyEvents.empty ()) 
    {
      Ptr<EventImpl> ev = m_destroyEvents.front ().PeekEventImpl ();
      m_destroyEvents.pop_front ();
      NS_LOG_LOGIC ("handle destroy " << ev);
      if (!ev->IsCancelled ())
        {
          ev->Invoke ();
        }
    }
}

void
DefaultSimulatorImpl::SetScheduler (Ptr<Scheduler> scheduler)
{
  if (m_events != 0)
    {
      while (!m_events->IsEmpty ())
        {
          Scheduler::Event next = m_events->RemoveNext ();
          scheduler->Insert (next);
        }
    }
  m_events = scheduler;
}

void
DefaultSimulatorImpl::ProcessOneEvent (void)
{
  Scheduler::Event next = m_events->RemoveNext ();

  NS_ASSERT (next.key.m_ts >= m_currentTs);
  --m_unscheduledEvents;

  NS_LOG_LOGIC ("handle " << next.key.m_ts);
  m_currentTs = next.key.m_ts;
  m_currentUid = next.key.m_uid;
  next.impl->Invoke ();
  next.impl->Unref ();
}

bool 
DefaultSimulatorImpl::IsFinished (void) const
{
  return m_events->IsEmpty () || m_stop;
}

uint64_t
DefaultSimulatorImpl::NextTs (void) const
{
  NS_ASSERT (!m_events->IsEmpty ());
  Scheduler::Event ev = m_events->PeekNext ();
  return ev.key.m_ts;
}

Time
DefaultSimulatorImpl::Next (void) const
{
  return TimeStep (NextTs ());
}

void
DefaultSimulatorImpl::Run (void)
{
  m_stop = false;
  while (!m_events->IsEmpty () && !m_stop) 
    {
      ProcessOneEvent ();
    }

  // If the simulator stopped naturally by lack of events, make a
  // consistency test to check that we didn't lose any events along the way.
  NS_ASSERT(!m_events->IsEmpty () || m_unscheduledEvents == 0);
}

void
DefaultSimulatorImpl::RunOneEvent (void)
{
  ProcessOneEvent ();
}

void 
DefaultSimulatorImpl::Stop (void)
{
  m_stop = true;
}


//
// Schedule an event for a _relative_ time in the future.
//
EventId
DefaultSimulatorImpl::Schedule (Time const &time, EventImpl *event)
{
  Time tAbsolute = time + Now();

  NS_ASSERT (tAbsolute.IsPositive ());
  NS_ASSERT (tAbsolute >= TimeStep (m_currentTs));
  Scheduler::Event ev;
  ev.impl = event;
  ev.key.m_ts = (uint64_t) tAbsolute.GetTimeStep ();
  ev.key.m_uid = m_uid;
  m_uid++;
  ++m_unscheduledEvents;
  m_events->Insert (ev);
  return EventId (event, ev.key.m_ts, ev.key.m_uid);
}

EventId
DefaultSimulatorImpl::ScheduleNow (EventImpl *event)
{
  Scheduler::Event ev;
  ev.impl = event;
  ev.key.m_ts = m_currentTs;
  ev.key.m_uid = m_uid;
  m_uid++;
  ++m_unscheduledEvents;
  m_events->Insert (ev);
  return EventId (event, ev.key.m_ts, ev.key.m_uid);
}

EventId
DefaultSimulatorImpl::ScheduleDestroy (EventImpl *event)
{
  EventId id (Ptr<EventImpl> (event, false), m_currentTs, 2);
  m_destroyEvents.push_back (id);
  m_uid++;
  return id;
}

Time
DefaultSimulatorImpl::Now (void) const
{
  return TimeStep (m_currentTs);
}

Time 
DefaultSimulatorImpl::GetDelayLeft (const EventId &id) const
{
  if (IsExpired (id))
    {
      return TimeStep (0);
    }
  else
    {
      return TimeStep (id.GetTs () - m_currentTs);
    }
}

void
DefaultSimulatorImpl::Remove (const EventId &id)
{
  if (id.GetUid () == 2)
    {
      // destroy events.
      for (DestroyEvents::iterator i = m_destroyEvents.begin (); i != m_destroyEvents.end (); i++)
        {
          if (*i == id)
            {
              m_destroyEvents.erase (i);
              break;
            }
         }
      return;
    }
  if (IsExpired (id))
    {
      return;
    }
  Scheduler::Event event;
  event.impl = id.PeekEventImpl ();
  event.key.m_ts = id.GetTs ();
  event.key.m_uid = id.GetUid ();
  m_events->Remove (event);
  event.impl->Cancel ();
  // whenever we remove an event from the event list, we have to unref it.
  event.impl->Unref ();

  --m_unscheduledEvents;
}

void
DefaultSimulatorImpl::Cancel (const EventId &id)
{
  if (!IsExpired (id))
    {
      id.PeekEventImpl ()->Cancel ();
    }
}

bool
DefaultSimulatorImpl::IsExpired (const EventId &ev) const
{
  if (ev.GetUid () == 2)
    {
      // destroy events.
      for (DestroyEvents::const_iterator i = m_destroyEvents.begin (); i != m_destroyEvents.end (); i++)
        {
          if (*i == ev)
            {
              return false;
            }
         }
      return true;
    }
  if (ev.PeekEventImpl () == 0 ||
      ev.GetTs () < m_currentTs ||
      (ev.GetTs () == m_currentTs &&
       ev.GetUid () <= m_currentUid) ||
      ev.PeekEventImpl ()->IsCancelled ()) 
    {
      return true;
    }
  else
    {
      return false;
    }
}

Time 
DefaultSimulatorImpl::GetMaximumSimulationTime (void) const
{
  // XXX: I am fairly certain other compilers use other non-standard
  // post-fixes to indicate 64 bit constants.
  return TimeStep (0x7fffffffffffffffLL);
}

} // namespace ns3


