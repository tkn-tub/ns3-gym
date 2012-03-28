/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: George Riley <riley@ece.gatech.edu>
 */

#include "distributed-simulator-impl.h"
#include "mpi-interface.h"

#include "ns3/simulator.h"
#include "ns3/scheduler.h"
#include "ns3/event-impl.h"
#include "ns3/channel.h"
#include "ns3/node-container.h"
#include "ns3/ptr.h"
#include "ns3/pointer.h"
#include "ns3/assert.h"
#include "ns3/log.h"

#include <math.h>

#ifdef NS3_MPI
#include <mpi.h>
#endif

NS_LOG_COMPONENT_DEFINE ("DistributedSimulatorImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (DistributedSimulatorImpl);

LbtsMessage::~LbtsMessage ()
{
}

Time
LbtsMessage::GetSmallestTime ()
{
  return m_smallestTime;
}

uint32_t
LbtsMessage::GetTxCount ()
{
  return m_txCount;
}

uint32_t
LbtsMessage::GetRxCount ()
{
  return m_rxCount;
}
uint32_t
LbtsMessage::GetMyId ()
{
  return m_myId;
}

Time DistributedSimulatorImpl::m_lookAhead = Seconds (0);

TypeId
DistributedSimulatorImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DistributedSimulatorImpl")
    .SetParent<Object> ()
    .AddConstructor<DistributedSimulatorImpl> ()
  ;
  return tid;
}

DistributedSimulatorImpl::DistributedSimulatorImpl ()
{
#ifdef NS3_MPI
  m_myId = MpiInterface::GetSystemId ();
  m_systemCount = MpiInterface::GetSize ();

  // Allocate the LBTS message buffer
  m_pLBTS = new LbtsMessage[m_systemCount];
  m_grantedTime = Seconds (0);
#else
  NS_FATAL_ERROR ("Can't use distributed simulator without MPI compiled in");
#endif

  m_stop = false;
  // uids are allocated from 4.
  // uid 0 is "invalid" events
  // uid 1 is "now" events
  // uid 2 is "destroy" events
  m_uid = 4;
  // before ::Run is entered, the m_currentUid will be zero
  m_currentUid = 0;
  m_currentTs = 0;
  m_currentContext = 0xffffffff;
  m_unscheduledEvents = 0;
  m_events = 0;
}

DistributedSimulatorImpl::~DistributedSimulatorImpl ()
{
}

void
DistributedSimulatorImpl::DoDispose (void)
{
  while (!m_events->IsEmpty ())
    {
      Scheduler::Event next = m_events->RemoveNext ();
      next.impl->Unref ();
    }
  m_events = 0;
  delete [] m_pLBTS;
  SimulatorImpl::DoDispose ();
}

void
DistributedSimulatorImpl::Destroy ()
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

  MpiInterface::Destroy ();
}


void
DistributedSimulatorImpl::CalculateLookAhead (void)
{
#ifdef NS3_MPI
  if (MpiInterface::GetSize () <= 1)
    {
      DistributedSimulatorImpl::m_lookAhead = Seconds (0);
      m_grantedTime = Seconds (0);
    }
  else
    {
      NodeContainer c = NodeContainer::GetGlobal ();
      for (NodeContainer::Iterator iter = c.Begin (); iter != c.End (); ++iter)
        {
          if ((*iter)->GetSystemId () != MpiInterface::GetSystemId ())
            {
              continue;
            }

          for (uint32_t i = 0; i < (*iter)->GetNDevices (); ++i)
            {
              Ptr<NetDevice> localNetDevice = (*iter)->GetDevice (i);
              // only works for p2p links currently
              if (!localNetDevice->IsPointToPoint ())
                {
                  continue;
                }
              Ptr<Channel> channel = localNetDevice->GetChannel ();
              if (channel == 0)
                {
                  continue;
                }

              // grab the adjacent node
              Ptr<Node> remoteNode;
              if (channel->GetDevice (0) == localNetDevice)
                {
                  remoteNode = (channel->GetDevice (1))->GetNode ();
                }
              else
                {
                  remoteNode = (channel->GetDevice (0))->GetNode ();
                }

              // if it's not remote, don't consider it
              if (remoteNode->GetSystemId () == MpiInterface::GetSystemId ())
                {
                  continue;
                }

              // compare delay on the channel with current value of
              // m_lookAhead.  if delay on channel is smaller, make
              // it the new lookAhead.
              TimeValue delay;
              channel->GetAttribute ("Delay", delay);
              if (DistributedSimulatorImpl::m_lookAhead.IsZero ())
                {
                  DistributedSimulatorImpl::m_lookAhead = delay.Get ();
                  m_grantedTime = delay.Get ();
                }
              if (delay.Get ().GetSeconds () < DistributedSimulatorImpl::m_lookAhead.GetSeconds ())
                {
                  DistributedSimulatorImpl::m_lookAhead = delay.Get ();
                  m_grantedTime = delay.Get ();
                }
            }
        }
    }
#else
  NS_FATAL_ERROR ("Can't use distributed simulator without MPI compiled in");
#endif
}

void
DistributedSimulatorImpl::SetScheduler (ObjectFactory schedulerFactory)
{
  Ptr<Scheduler> scheduler = schedulerFactory.Create<Scheduler> ();

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
DistributedSimulatorImpl::ProcessOneEvent (void)
{
  Scheduler::Event next = m_events->RemoveNext ();

  NS_ASSERT (next.key.m_ts >= m_currentTs);
  m_unscheduledEvents--;

  NS_LOG_LOGIC ("handle " << next.key.m_ts);
  m_currentTs = next.key.m_ts;
  m_currentContext = next.key.m_context;
  m_currentUid = next.key.m_uid;
  next.impl->Invoke ();
  next.impl->Unref ();
}

bool
DistributedSimulatorImpl::IsFinished (void) const
{
  return m_events->IsEmpty () || m_stop;
}

uint64_t
DistributedSimulatorImpl::NextTs (void) const
{
  NS_ASSERT (!m_events->IsEmpty ());
  Scheduler::Event ev = m_events->PeekNext ();
  return ev.key.m_ts;
}

Time
DistributedSimulatorImpl::Next (void) const
{
  return TimeStep (NextTs ());
}

void
DistributedSimulatorImpl::Run (void)
{
#ifdef NS3_MPI
  CalculateLookAhead ();
  m_stop = false;
  while (!m_events->IsEmpty () && !m_stop)
    {
      Time nextTime = Next ();
      if (nextTime > m_grantedTime)
        { // Can't process, calculate a new LBTS
          // First receive any pending messages
          MpiInterface::ReceiveMessages ();
          // reset next time
          nextTime = Next ();
          // And check for send completes
          MpiInterface::TestSendComplete ();
          // Finally calculate the lbts
          LbtsMessage lMsg (MpiInterface::GetRxCount (), MpiInterface::GetTxCount (), m_myId, nextTime);
          m_pLBTS[m_myId] = lMsg;
          MPI_Allgather (&lMsg, sizeof (LbtsMessage), MPI_BYTE, m_pLBTS,
                         sizeof (LbtsMessage), MPI_BYTE, MPI_COMM_WORLD);
          Time smallestTime = m_pLBTS[0].GetSmallestTime ();
          // The totRx and totTx counts insure there are no transient
          // messages;  If totRx != totTx, there are transients,
          // so we don't update the granted time.
          uint32_t totRx = m_pLBTS[0].GetRxCount ();
          uint32_t totTx = m_pLBTS[0].GetTxCount ();

          for (uint32_t i = 1; i < m_systemCount; ++i)
            {
              if (m_pLBTS[i].GetSmallestTime () < smallestTime)
                {
                  smallestTime = m_pLBTS[i].GetSmallestTime ();
                }
              totRx += m_pLBTS[i].GetRxCount ();
              totTx += m_pLBTS[i].GetTxCount ();

            }
          if (totRx == totTx)
            {
              m_grantedTime = smallestTime + DistributedSimulatorImpl::m_lookAhead;
            }
        }
      if (nextTime <= m_grantedTime)
        { // Save to process
          ProcessOneEvent ();
        }
    }

  // If the simulator stopped naturally by lack of events, make a
  // consistency test to check that we didn't lose any events along the way.
  NS_ASSERT (!m_events->IsEmpty () || m_unscheduledEvents == 0);
#else
  NS_FATAL_ERROR ("Can't use distributed simulator without MPI compiled in");
#endif
}

uint32_t DistributedSimulatorImpl::GetSystemId () const
{
  return m_myId;
}

void
DistributedSimulatorImpl::Stop (void)
{
  m_stop = true;
}

void
DistributedSimulatorImpl::Stop (Time const &time)
{
  Simulator::Schedule (time, &Simulator::Stop);
}

//
// Schedule an event for a _relative_ time in the future.
//
EventId
DistributedSimulatorImpl::Schedule (Time const &time, EventImpl *event)
{
  Time tAbsolute = time + TimeStep (m_currentTs);

  NS_ASSERT (tAbsolute.IsPositive ());
  NS_ASSERT (tAbsolute >= TimeStep (m_currentTs));
  Scheduler::Event ev;
  ev.impl = event;
  ev.key.m_ts = static_cast<uint64_t> (tAbsolute.GetTimeStep ());
  ev.key.m_context = GetContext ();
  ev.key.m_uid = m_uid;
  m_uid++;
  m_unscheduledEvents++;
  m_events->Insert (ev);
  return EventId (event, ev.key.m_ts, ev.key.m_context, ev.key.m_uid);
}

void
DistributedSimulatorImpl::ScheduleWithContext (uint32_t context, Time const &time, EventImpl *event)
{
  NS_LOG_FUNCTION (this << context << time.GetTimeStep () << m_currentTs << event);

  Scheduler::Event ev;
  ev.impl = event;
  ev.key.m_ts = m_currentTs + time.GetTimeStep ();
  ev.key.m_context = context;
  ev.key.m_uid = m_uid;
  m_uid++;
  m_unscheduledEvents++;
  m_events->Insert (ev);
}

EventId
DistributedSimulatorImpl::ScheduleNow (EventImpl *event)
{
  Scheduler::Event ev;
  ev.impl = event;
  ev.key.m_ts = m_currentTs;
  ev.key.m_context = GetContext ();
  ev.key.m_uid = m_uid;
  m_uid++;
  m_unscheduledEvents++;
  m_events->Insert (ev);
  return EventId (event, ev.key.m_ts, ev.key.m_context, ev.key.m_uid);
}

EventId
DistributedSimulatorImpl::ScheduleDestroy (EventImpl *event)
{
  EventId id (Ptr<EventImpl> (event, false), m_currentTs, 0xffffffff, 2);
  m_destroyEvents.push_back (id);
  m_uid++;
  return id;
}

Time
DistributedSimulatorImpl::Now (void) const
{
  return TimeStep (m_currentTs);
}

Time
DistributedSimulatorImpl::GetDelayLeft (const EventId &id) const
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
DistributedSimulatorImpl::Remove (const EventId &id)
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
  event.key.m_context = id.GetContext ();
  event.key.m_uid = id.GetUid ();
  m_events->Remove (event);
  event.impl->Cancel ();
  // whenever we remove an event from the event list, we have to unref it.
  event.impl->Unref ();

  m_unscheduledEvents--;
}

void
DistributedSimulatorImpl::Cancel (const EventId &id)
{
  if (!IsExpired (id))
    {
      id.PeekEventImpl ()->Cancel ();
    }
}

bool
DistributedSimulatorImpl::IsExpired (const EventId &ev) const
{
  if (ev.GetUid () == 2)
    {
      if (ev.PeekEventImpl () == 0
          || ev.PeekEventImpl ()->IsCancelled ())
        {
          return true;
        }
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
  if (ev.PeekEventImpl () == 0
      || ev.GetTs () < m_currentTs
      || (ev.GetTs () == m_currentTs
          && ev.GetUid () <= m_currentUid)
      || ev.PeekEventImpl ()->IsCancelled ())
    {
      return true;
    }
  else
    {
      return false;
    }
}

Time
DistributedSimulatorImpl::GetMaximumSimulationTime (void) const
{
  // XXX: I am fairly certain other compilers use other non-standard
  // post-fixes to indicate 64 bit constants.
  return TimeStep (0x7fffffffffffffffLL);
}

uint32_t
DistributedSimulatorImpl::GetContext (void) const
{
  return m_currentContext;
}

} // namespace ns3
