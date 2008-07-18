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
#include "simulator-impl.h"
#include "scheduler.h"
#include "event-impl.h"

#include "ns3/ptr.h"
#include "ns3/pointer.h"
#include "ns3/assert.h"
#include "ns3/log.h"

#include <math.h>

NS_LOG_COMPONENT_DEFINE ("SimulatorImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SimulatorImpl);

TypeId
SimulatorImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SimulatorImpl")
    .SetParent<Object> ()
    .AddConstructor<SimulatorImpl> ()
    ;
  return tid;
}

SimulatorImpl::SimulatorImpl ()
{
  m_stop = false;
  m_stopAt = 0;
  // uids are allocated from 4.
  // uid 0 is "invalid" events
  // uid 1 is "now" events
  // uid 2 is "destroy" events
  m_uid = 4; 
  // before ::Run is entered, the m_currentUid will be zero
  m_currentUid = 0;
  m_logEnable = false;
  m_currentTs = 0;
  m_unscheduledEvents = 0;
}

SimulatorImpl::~SimulatorImpl ()
{
  while (!m_events->IsEmpty ())
    {
      EventId next = m_events->RemoveNext ();
    }
  m_events = 0;
}

void
SimulatorImpl::Destroy ()
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
SimulatorImpl::SetScheduler (Ptr<Scheduler> scheduler)
{
  if (m_events != 0)
    {
      while (!m_events->IsEmpty ())
        {
          EventId next = m_events->RemoveNext ();
          scheduler->Insert (next);
        }
    }
  m_events = scheduler;
}

Ptr<Scheduler>
SimulatorImpl::GetScheduler (void) const
{
  return m_events;
}

void
SimulatorImpl::EnableLogTo (char const *filename)
{
  m_log.open (filename);
  m_logEnable = true;
}

void
SimulatorImpl::ProcessOneEvent (void)
{
  EventId next = m_events->RemoveNext ();

  NS_ASSERT (next.GetTs () >= m_currentTs);
  --m_unscheduledEvents;

  NS_LOG_LOGIC ("handle " << next.GetTs ());
  m_currentTs = next.GetTs ();
  m_currentUid = next.GetUid ();
  if (m_logEnable) 
    {
      m_log << "e "<<next.GetUid () << " " << next.GetTs () << std::endl;
    }
  EventImpl *event = next.PeekEventImpl ();
  event->Invoke ();
}

bool 
SimulatorImpl::IsFinished (void) const
{
  return m_events->IsEmpty ();
}

uint64_t
SimulatorImpl::NextTs (void) const
{
  NS_ASSERT (!m_events->IsEmpty ());
  EventId id = m_events->PeekNext ();
  return id.GetTs ();
}

Time
SimulatorImpl::Next (void) const
{
  return TimeStep (NextTs ());
}

void
SimulatorImpl::Run (void)
{

  while (!m_events->IsEmpty () && !m_stop && 
         (m_stopAt == 0 || m_stopAt > NextTs ())) 
    {
      ProcessOneEvent ();
    }

  // If the simulator stopped naturally by lack of events, make a
  // consistency test to check that we didn't lose any events along the way.
  NS_ASSERT(!m_events->IsEmpty () || m_unscheduledEvents == 0);

  m_log.close ();
}

void 
SimulatorImpl::Stop (void)
{
  m_stop = true;
}

void 
SimulatorImpl::Stop (Time const &time)
{
  NS_ASSERT (time.IsPositive ());
  Time absolute = Simulator::Now () + time;
  m_stopAt = absolute.GetTimeStep ();
}

EventId
SimulatorImpl::Schedule (Time const &time, const Ptr<EventImpl> &event)
{
  NS_ASSERT (time.IsPositive ());
  NS_ASSERT (time >= TimeStep (m_currentTs));
  uint64_t ts = (uint64_t) time.GetTimeStep ();
  EventId id (event, ts, m_uid);
  if (m_logEnable) 
    {
      m_log << "i "<<m_currentUid<<" "<<m_currentTs<<" "
            <<m_uid<<" "<<time.GetTimeStep () << std::endl;
    }
  m_uid++;
  ++m_unscheduledEvents;
  m_events->Insert (id);
  return id;
}

EventId
SimulatorImpl::ScheduleNow (const Ptr<EventImpl> &event)
{
  EventId id (event, m_currentTs, m_uid);
  if (m_logEnable) 
    {
      m_log << "i "<<m_currentUid<<" "<<m_currentTs<<" "
            <<m_uid<<" "<<m_currentTs << std::endl;
    }
  m_uid++;
  ++m_unscheduledEvents;
  m_events->Insert (id);
  return id;
}

EventId
SimulatorImpl::ScheduleDestroy (const Ptr<EventImpl> &event)
{
  EventId id (event, m_currentTs, 2);
  m_destroyEvents.push_back (id);
  if (m_logEnable) 
  {
    m_log << "id " << m_currentUid << " " << Now ().GetTimeStep () << " "
          << m_uid << std::endl;
  }
  m_uid++;
  return id;
}

Time
SimulatorImpl::Now (void) const
{
  return TimeStep (m_currentTs);
}

Time 
SimulatorImpl::GetDelayLeft (const EventId &id) const
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
SimulatorImpl::Remove (const EventId &ev)
{
  if (ev.GetUid () == 2)
    {
      // destroy events.
      for (DestroyEvents::iterator i = m_destroyEvents.begin (); i != m_destroyEvents.end (); i++)
        {
          if (*i == ev)
            {
              m_destroyEvents.erase (i);
              break;
            }
         }
      return;
    }
  if (IsExpired (ev))
    {
      return;
    }
  m_events->Remove (ev);
  Cancel (ev);

  if (m_logEnable) 
    {
      m_log << "r " << m_currentUid << " " << m_currentTs << " "
            << ev.GetUid () << " " << ev.GetTs () << std::endl;
    }
  --m_unscheduledEvents;
}

void
SimulatorImpl::Cancel (const EventId &id)
{
  if (!IsExpired (id))
    {
      id.PeekEventImpl ()->Cancel ();
    }
}

bool
SimulatorImpl::IsExpired (const EventId &ev) const
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
SimulatorImpl::GetMaximumSimulationTime (void) const
{
  // XXX: I am fairly certain other compilers use other non-standard
  // post-fixes to indicate 64 bit constants.
  return TimeStep (0x7fffffffffffffffLL);
}

}; // namespace ns3


