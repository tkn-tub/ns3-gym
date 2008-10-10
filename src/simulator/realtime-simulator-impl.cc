/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
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
 */

#include "simulator.h"
#include "realtime-simulator-impl.h"
#include "wall-clock-synchronizer.h"
#include "scheduler.h"
#include "event-impl.h"
#include "synchronizer.h"

#include "ns3/ptr.h"
#include "ns3/pointer.h"
#include "ns3/assert.h"
#include "ns3/fatal-error.h"
#include "ns3/log.h"
#include "ns3/system-mutex.h"
#include "ns3/boolean.h"
#include "ns3/enum.h"


#include <math.h>

NS_LOG_COMPONENT_DEFINE ("RealtimeSimulatorImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RealtimeSimulatorImpl);

TypeId
RealtimeSimulatorImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RealtimeSimulatorImpl")
    .SetParent<Object> ()
    .AddConstructor<RealtimeSimulatorImpl> ()
    .AddAttribute ("SynchronizationMode", 
                   "What to do if the simulation cannot keep up with real time.",
                   EnumValue (SYNC_BEST_EFFORT),
                   MakeEnumAccessor (&RealtimeSimulatorImpl::SetSynchronizationMode),
                   MakeEnumChecker (SYNC_BEST_EFFORT, "BestEffort",
                                    SYNC_HARD_LIMIT, "HardLimit"))
    .AddAttribute ("HardLimit", 
                   "Maximum acceptable real-time jitter (used in conjunction with SynchronizationMode=HardLimit)",
                   TimeValue (Seconds (0.1)),
                   MakeTimeAccessor (&RealtimeSimulatorImpl::m_hardLimit),
                   MakeTimeChecker ())
    ;
  return tid;
}

void
RealtimeEventLock::Lock (void)
{
  m_eventMutex.Lock ();
}

void
RealtimeEventLock::Unlock (void)
{
  m_eventMutex.Unlock ();
}

RealtimeSimulatorImpl::RealtimeSimulatorImpl ()
{
  NS_LOG_FUNCTION_NOARGS ();

  EventImpl::SetEventLock (&m_eventLock);

  m_stop = false;
  m_stopAt = 0;
  m_running = false;
  // uids are allocated from 4.
  // uid 0 is "invalid" events
  // uid 1 is "now" events
  // uid 2 is "destroy" events
  m_uid = 4; 
  // before ::Run is entered, the m_currentUid will be zero
  m_currentUid = 0;
  m_currentTs = 0;
  m_unscheduledEvents = 0;

  // Be very careful not to do anything that would cause a change or assignment
  // of the underlying reference counts of m_synchronizer or you will be sorry.
  m_synchronizer = CreateObject<WallClockSynchronizer> ();
}

RealtimeSimulatorImpl::~RealtimeSimulatorImpl ()
{
  NS_LOG_FUNCTION_NOARGS ();
  while (m_events->IsEmpty () == false)
    {
      EventId next = m_events->RemoveNext ();
    }
  m_events = 0;
  m_synchronizer = 0;

  EventImpl::SetNoEventLock ();
}

void
RealtimeSimulatorImpl::Destroy ()
{
  NS_LOG_FUNCTION_NOARGS ();

  //
  // This function is only called with the private version "disconnected" from
  // the main simulator functions.  We rely on the user not calling 
  // Simulator::Destroy while there is a chance that a worker thread could be
  // accessing the current instance of the private object.  In practice this
  // means shutting down the workers and doing a Join() before calling the
  // Simulator::Destroy().
  //
  while (m_destroyEvents.empty () == false) 
    {
      Ptr<EventImpl> ev = m_destroyEvents.front ().PeekEventImpl ();
      m_destroyEvents.pop_front ();
      NS_LOG_LOGIC ("handle destroy " << ev);
      if (ev->IsCancelled () == false)
        {
          ev->Invoke ();
        }
    }
}

void
RealtimeSimulatorImpl::SetScheduler (Ptr<Scheduler> scheduler)
{
  NS_LOG_FUNCTION_NOARGS ();

  { 
    CriticalSection cs (m_mutex);

    if (m_events != 0)
      {
        while (m_events->IsEmpty () == false)
          {
            EventId next = m_events->RemoveNext ();
            scheduler->Insert (next);
          }
      }
    m_events = scheduler;
  }
}

Ptr<Scheduler>
RealtimeSimulatorImpl::GetScheduler (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_events;
}

void
RealtimeSimulatorImpl::ProcessOneEvent (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  //
  // The idea here is to wait until the next event comes due.  In the case of
  // a realtime simulation, we want real time to be consumed between events.  
  // It is the realtime synchronizer that causes real time to be consumed by
  // doing some kind of a wait.
  //
  // We need to be able to have external events (such as a packet reception event)
  // cause us to re-evaluate our state.  The way this works is that the synchronizer
  // gets interrupted and returs.  So, there is a possibility that things may change
  // out from under us dynamically.  In this case, we need to re-evaluate how long to 
  // wait in a for-loop until we have waited sucessfully (until a timeout) for the 
  // event at the head of the event list.
  //
  // m_synchronizer->Synchronize will return true if the wait was completed without 
  // interruption, otherwise it will return false indicating that something has changed
  // out from under us.  If we sit in the for-loop trying to synchronize until 
  // Synchronize() returns true, we will have successfully synchronized the execution 
  // time of the next event with the wall clock time of the synchronizer.
  //

  for (;;) 
    {
      uint64_t tsDelay = 0;
      uint64_t tsNext = 0;

      //
      // It is important to understand that m_currentTs is interpreted only as the 
      // timestamp  of the last event we executed.  Current time can a bit of a 
      // slippery concept in realtime mode.  What we have here is a discrete event 
      // simulator, so the last event is, by defintion, executed entirely at a single
      //  discrete time.  This is the definition of m_currentTs.  It really has 
      // nothing to do with the current real time, except that we are trying to arrange
      // that at the instant of the beginning of event execution, the current real time
      // and m_currentTs coincide.
      //
      // We use tsNow as the indication of the current real time.
      //
      uint64_t tsNow;

      { 
        CriticalSection cs (m_mutex);
        //
        // Since we are in realtime mode, the time to delay has got to be the 
        // difference between the current realtime and the timestamp of the next 
        // event.  Since m_currentTs is actually the timestamp of the last event we 
        // executed, it's not particularly meaningful for us here since real time has
        // certainly elapsed since it was last updated.
        //
        // It is possible that the current realtime has drifted past the next event
        // time so we need to be careful about that and not delay in that case.
        //
        NS_ASSERT_MSG (m_synchronizer->Realtime (), 
          "RealtimeSimulatorImpl::ProcessOneEvent (): Synchronizer reports not Realtime ()");

        //
        // tsNow is set to the normalized current real time.  When the simulation was
        // started, the current real time was effectively set to zero; so tsNow is
        // the current "real" simulation time.
        //
        // tsNext is the simulation time of the next event we want to execute.
        //
        tsNow = m_synchronizer->GetCurrentRealtime ();
        tsNext = NextTs ();

        //
        // tsDelay is therefore the real time we need to delay in order to bring the
        // real time in sync with the simulation time.  If we wait for this amount of
        // real time, we will accomplish moving the simulation time at the same rate
        // as the real time.  This is typically called "pacing" the simulation time.
        //
        // We do have to be careful if we are falling behind.  If so, tsDelay must be
        // zero.  If we're late, don't dawdle.
        //
        if (tsNext <= tsNow)
          {
            tsDelay = 0;
          }
        else
          {
            tsDelay = tsNext - tsNow;
          }
      
        //
        // We've figured out how long we need to delay in order to pace the 
        // simulation time with the real time.  We're going to sleep, but need
        // to work with the synchronizer to make sure we're awakened if something 
        // external happens (like a packet is received).  This next line resets
        // the synchronizer so that any future event will cause it to interrupt.
        //
        m_synchronizer->SetCondition (false);
      }

      //
      // We have a time to delay.  This time may actually not be valid anymore
      // since we released the critical section immediately above, and a real-time
      // ScheduleReal or ScheduleRealNow may have snuck in, well, between the 
      // closing brace above and this comment so to speak.  If this is the case, 
      // that schedule operation will have done a synchronizer Signal() that 
      // will set the condition variable to true and cause the Synchronize call 
      // below to return immediately.
      //
      // It's easiest to understand if you just consider a short tsDelay that only
      // requires a SpinWait down in the synchronizer.  What will happen is that 
      // whan Synchronize calls SpinWait, SpinWait will look directly at its 
      // condition variable.  Note that we set this condition variable to false 
      // inside the critical section above. 
      //
      // SpinWait will go into a forever loop until either the time has expired or
      // until the condition variable becomes true.  A true condition indicates that
      // the wait should stop.  The condition is set to true by one of the Schedule
      // methods of the simulator; so if we are in a wait down in Synchronize, and
      // a Simulator::ScheduleReal is done, the wait down in Synchronize will exit and
      // Synchronize will return false.  This means we have not actually synchronized
      // to the event expiration time.  If no real-time schedule operation is done
      // while down in Synchronize, the wait will time out and Synchronize will return 
      // true.  This indicates that we have synchronized to the event time.
      //
      // So we need to stay in this for loop, looking for the next event timestamp and 
      // attempting to sleep until its due.  If we've slept until the timestamp is due, 
      // Synchronize returns true and we break out of the sync loop.  If an external
      // event happens that requires a re-schedule, Synchronize returns false and
      // we re-evaluate our timing by continuing in the loop.  
      //
      // It is expected that tsDelay become shorter as external events interrupt our
      // waits.
      //
      if (m_synchronizer->Synchronize (tsNow, tsDelay))
        {
          NS_LOG_LOGIC ("Interrupted ...");
          break;
        }
 
      //
      // If we get to this point, we have been interrupted during a wait by a real-time
      // schedule operation.  This means all bets are off regarding tsDelay and we need
      // to re-evaluate what it is we want to do.  We'll loop back around in the 
      // for-loop and start again from scratch.
      //
    }

  //
  // If we break out of the for-loop above, we have waited until the time specified
  // by the event that was at the head of the event list when we started the process.
  // Since there is a bunch of code that was executed outside a critical section (the
  // Synchronize call) we cannot be sure that the event at the head of the event list
  // is the one we think it is.  What we can be sure of is that it is time to execute
  // whatever event is at the head of this list if the list is in time order.
  //
  EventId next;

  { 
    CriticalSection cs (m_mutex);

    // 
    // We do know we're waiting for an event, so there had better be an event on the 
    // event queue.  Let's pull it off.  When we release the critical section, the
    // event we're working on won't be on the list and so subsequent operations won't
    // mess with us.
    //
    NS_ASSERT_MSG (m_events->IsEmpty () == false, 
      "RealtimeSimulatorImpl::ProcessOneEvent(): event queue is empty");
    next = m_events->RemoveNext ();
    --m_unscheduledEvents;

    //
    // We cannot make any assumption that "next" is the same event we originally waited 
    // for.  We can only assume that only that it must be due and cannot cause time 
    // to move backward.
    //
    NS_ASSERT_MSG (next.GetTs () >= m_currentTs,
                   "RealtimeSimulatorImpl::ProcessOneEvent(): "
                   "next.GetTs() earlier than m_currentTs (list order error)");
    NS_LOG_LOGIC ("handle " << next.GetTs ());

    // 
    // Update the current simulation time to be the timestamp of the event we're 
    // executing.  From the rest of the simulation's point of view, simulation time
    // is frozen until the next event is executed.
    //
    m_currentTs = next.GetTs ();
    m_currentUid = next.GetUid ();

    // 
    // We're about to run the event and we've done our best to synchronize this
    // event execution time to real time.  Now, if we're in SYNC_HARD_LIMIT mode
    // we have to decide if we've done a good enough job and if we haven't, we've
    // been asked to commit ritual suicide.
    //
    // We check the simulation time against the current real time to make this
    // judgement.
    //
    if (m_synchronizationMode == SYNC_HARD_LIMIT)
      {
        uint64_t tsFinal = m_synchronizer->GetCurrentRealtime ();
        uint64_t tsJitter;

        if (tsFinal >= m_currentTs)
          {
            tsJitter = tsFinal - m_currentTs;
          }
        else
          {
            tsJitter = m_currentTs - tsFinal;
          }

        if (tsJitter > static_cast<uint64_t>(m_hardLimit.GetTimeStep ()))
          {
            NS_FATAL_ERROR ("RealtimeSimulatorImpl::ProcessOneEvent (): "
                            "Hard real-time limit exceeded (jitter = " << tsJitter << ")");
          }
      }
  }

  //
  // We have got the event we're about to execute completely disentangled from the 
  // event list so we can execute it outside a critical section without fear of someone
  // changing things out from under us.

  EventImpl *event = next.PeekEventImpl ();
  m_synchronizer->EventStart ();
  event->Invoke ();
  m_synchronizer->EventEnd ();
}

bool 
RealtimeSimulatorImpl::IsFinished (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  bool rc;
  {
    CriticalSection cs (m_mutex);
    rc = m_events->IsEmpty ();
  }

  return rc;
}

//
// Peeks into event list.  Should be called with critical section locked.
//
uint64_t
RealtimeSimulatorImpl::NextTs (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT_MSG (m_events->IsEmpty () == false, 
    "RealtimeSimulatorImpl::NextTs(): event queue is empty");
  EventId id = m_events->PeekNext ();

  return id.GetTs ();
}

//
// Calls NextTs().  Should be called with critical section locked.
//
Time
RealtimeSimulatorImpl::Next (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return TimeStep (NextTs ());
}

void
RealtimeSimulatorImpl::Run (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_running = true;
  NS_ASSERT_MSG (m_currentTs == 0,
    "RealtimeSimulatorImpl::Run(): The beginning of time is not zero");
  m_synchronizer->SetOrigin (m_currentTs);

  for (;;) 
    {
      bool done = false;

      {
        CriticalSection cs (m_mutex);
        //
        // In all cases we stop when the event list is empty.  If you are doing a 
        // realtime simulation and you want it to extend out for some time, you must
        // call StopAt.  In the realtime case, this will stick a placeholder event out
        // at the end of time.
        //
        if (m_stop || m_events->IsEmpty ())
          {
            done = true;
          }
        //
        // We also want to stop the simulator at some time even if there are events 
        // that have been scheduled out in the future.  If we're in realtime mode, we 
        // actually have time passing, so we must look at the realtime clock to see if 
        // we're past the end time.
        //
        if (m_stopAt && m_stopAt <= m_synchronizer->GetCurrentRealtime ())
          {
            done = true;
          }
      }

      if (done)
        {
          break;
        }

      ProcessOneEvent ();
    }

  //
  // If the simulator stopped naturally by lack of events, make a
  // consistency test to check that we didn't lose any events along the way.
  //
  {
    CriticalSection cs (m_mutex);

    NS_ASSERT_MSG (m_events->IsEmpty () == false || m_unscheduledEvents == 0,
      "RealtimeSimulatorImpl::Run(): Empty queue and unprocessed events");
  }

  m_running = false;
}

bool
RealtimeSimulatorImpl::Running (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_running;
}

bool
RealtimeSimulatorImpl::Realtime (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_synchronizer->Realtime ();
}

void
RealtimeSimulatorImpl::RunOneEvent (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_FATAL_ERROR ("DefaultSimulatorImpl::RunOneEvent(): Not allowed in realtime simulations");
}

void 
RealtimeSimulatorImpl::Stop (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_stop = true;
}

static void Placeholder (void) {}

//
// Schedule a stop for a _relative_ time in the future.  If the simulation
// hasn't started yet, this will effectively be an absolute time.
//
void 
RealtimeSimulatorImpl::Stop (Time const &time)
{
  NS_LOG_FUNCTION (time);

  Time tAbsolute = Simulator::Now () + time;
  NS_ASSERT (tAbsolute.IsPositive ());
  NS_ASSERT (tAbsolute >= TimeStep (m_currentTs));
  m_stopAt = tAbsolute.GetTimeStep ();
  //
  // For the realtime case, we need a real event sitting out at the end of time
  // to keep the simulator running (sleeping) while there are no other events 
  // present.  If an "external" device in another thread decides to schedule an
  // event, the sleeping synchronizer will be awakened and the new event will
  // be run.
  //
  // The easiest thing to do is to call back up into the simulator to take 
  // advantage of all of the nice event wrappers.  This will call back down into
  // RealtimeSimulatorImpl::Schedule to do the work.  This path interprets the 
  // time as relative, so pass the relative time.
  //
  Simulator::Schedule (time, &Placeholder);
}

//
// Schedule an event for a _relative_ time in the future.
//
EventId
RealtimeSimulatorImpl::Schedule (Time const &time, const Ptr<EventImpl> &event)
{
  NS_LOG_FUNCTION (time << event);

  //
  // This is a little tricky.  We get a Ptr<EventImpl> passed down to us in some
  // thread context.  This Ptr<EventImpl> is not yet shared in any way.  It is 
  // possible however that the calling context is not the context of the main 
  // scheduler thread (e.g. it is in the context of a separate device thread).  
  // It would be bad (TM) if we naively wrapped the EventImpl up in an EventId 
  // that would be accessible from multiple threads without considering thread 
  // safety.
  //
  // It's clear that we cannot have a situation where the EventImpl is "owned" by
  // multiple threads.  The calling thread is free to hold the EventId as long as
  // it wants and manage the reference counts to the underlying EventImpl all it
  // wants.  The scheduler is free to do the same; and will eventually release
  // the reference in the context of thread running ProcessOneEvent().  It is 
  // "a bad thing" (TM) if these two threads decide to release the underlying
  // EventImpl "at the same time" since the result is sure to be multiple frees,
  // memory leaks or bus errors.
  //
  // The answer is to make the reference counting of the EventImpl thread safe; 
  // which we do.  We don't want to force the event implementation to carry around
  // a mutex, so we "lend" it one using a RealtimeEventLock object (m_eventLock)
  // in the constructor of the event and take it back in the destructor.  See the
  // event code for details.
  //
  EventId id;
  {
    CriticalSection cs (m_mutex);
    //
    // This is the reason we had to bring the absolute time calcualtion in from the
    // simulator.h into the implementation.  Since the implementations may be 
    // multi-threaded, we need this calculation to be atomic.  You can see it is
    // here since we are running in a CriticalSection.
    //
    Time tAbsolute = Simulator::Now () + time;
    NS_ASSERT_MSG (tAbsolute.IsPositive (), "RealtimeSimulatorImpl::Schedule(): Negative time");
    NS_ASSERT_MSG (tAbsolute >= TimeStep (m_currentTs), "RealtimeSimulatorImpl::Schedule(): time < m_currentTs");
    uint64_t ts = (uint64_t) tAbsolute.GetTimeStep ();
    id = EventId (event, ts, m_uid);
    m_uid++;
    ++m_unscheduledEvents;
    m_events->Insert (id);
    m_synchronizer->Signal ();
  }

  return id;
}

EventId
RealtimeSimulatorImpl::ScheduleNow (const Ptr<EventImpl> &event)
{
  NS_LOG_FUNCTION_NOARGS ();
  EventId id;
  {
    CriticalSection cs (m_mutex);

    id = EventId (event, m_currentTs, m_uid);
    m_uid++;
    ++m_unscheduledEvents;
    m_events->Insert (id);
    m_synchronizer->Signal ();
  }

  return id;
}

Time
RealtimeSimulatorImpl::Now (void) const
{
  return TimeStep (m_currentTs);
}

//
// Schedule an event for a _relative_ time in the future.
//
EventId
RealtimeSimulatorImpl::ScheduleReal (Time const &time, const Ptr<EventImpl> &event)
{
  NS_LOG_FUNCTION (time << event);
  NS_ASSERT (false);
  EventId id;
  return id;
}

EventId
RealtimeSimulatorImpl::ScheduleRealNow (const Ptr<EventImpl> &event)
{
  NS_LOG_FUNCTION (event);
  NS_ASSERT (false);
  EventId id;
  return id;
}

Time
RealtimeSimulatorImpl::RealNow (void) const
{
  NS_ASSERT (false);
  return TimeStep (m_currentTs);
}


EventId
RealtimeSimulatorImpl::ScheduleDestroy (const Ptr<EventImpl> &event)
{
  NS_LOG_FUNCTION_NOARGS ();
  EventId id;

  {
    CriticalSection cs (m_mutex);

    //
    // Time doesn't really matter here (especially in realtime mode).  It is 
    // overridden by the uid of 2 which identifies this as an event to be 
    // executed at Simulator::Destroy time.
    //
    id = EventId (event, m_currentTs, 2);
    m_destroyEvents.push_back (id);
    m_uid++;
  }

  return id;
}

Time 
RealtimeSimulatorImpl::GetDelayLeft (const EventId &id) const
{
  //
  // If the event has expired, there is no delay until it runs.  It is not the
  // case that there is a negative time until it runs.
  //
  if (IsExpired (id))
    {
      return TimeStep (0);
    }

  return TimeStep (id.GetTs () - m_currentTs);
}

void
RealtimeSimulatorImpl::Remove (const EventId &ev)
{
  if (ev.GetUid () == 2)
    {
      // destroy events.
      for (DestroyEvents::iterator i = m_destroyEvents.begin (); 
           i != m_destroyEvents.end (); 
           i++)
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

  {
    CriticalSection cs (m_mutex);

    m_events->Remove (ev);
    --m_unscheduledEvents;

    Cancel (ev);

  }
}

void
RealtimeSimulatorImpl::Cancel (const EventId &id)
{
  if (IsExpired (id) == false)
    {
      id.PeekEventImpl ()->Cancel ();
    }
}

bool
RealtimeSimulatorImpl::IsExpired (const EventId &ev) const
{
  if (ev.GetUid () == 2)
    {
      // destroy events.
      for (DestroyEvents::const_iterator i = m_destroyEvents.begin (); 
           i != m_destroyEvents.end (); i++)
        {
          if (*i == ev)
            {
              return false;
            }
         }
      return true;
    }

  //
  // If the time of the event is less than the current timestamp of the 
  // simulator, the simulator has gone past the invocation time of the 
  // event, so the statement ev.GetTs () < m_currentTs does mean that 
  // the event has been fired even in realtime mode.
  //
  // The same is true for the next line involving the m_currentUid.
  //
  if (ev.PeekEventImpl () == 0 ||
      ev.GetTs () < m_currentTs ||
      (ev.GetTs () == m_currentTs && ev.GetUid () <= m_currentUid) ||
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
RealtimeSimulatorImpl::GetMaximumSimulationTime (void) const
{
  // XXX: I am fairly certain other compilers use other non-standard
  // post-fixes to indicate 64 bit constants.
  return TimeStep (0x7fffffffffffffffLL);
}

void 
RealtimeSimulatorImpl::SetSynchronizationMode (enum SynchronizationMode mode)
{
  NS_LOG_FUNCTION (mode);
  m_synchronizationMode = mode;
}

RealtimeSimulatorImpl::SynchronizationMode
RealtimeSimulatorImpl::GetSynchronizationMode (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_synchronizationMode;
}
  
void 
RealtimeSimulatorImpl::SetHardLimit (Time limit)
{
  NS_LOG_FUNCTION (limit);
  m_hardLimit = limit;
}

Time
RealtimeSimulatorImpl::GetHardLimit (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_hardLimit;
}
  
}; // namespace ns3
