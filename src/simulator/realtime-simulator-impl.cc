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
  // a simulation not locked to realtime, the next event is due immediately and
  // we jump time forward -- there is no real time consumed between events.  In
  // the case of a realtime simulation, we do want real time to be consumed 
  // between events.  It is the synchronizer that causes real time to be 
  // consumed.
  //
  // Now, there is a possibility that a wait down in the call to the synchronizer
  // will be interrupted.  In this case, we need to re-evaluate how long to wait 
  // until the next event since the interruption may have been caused by an
  // event inserted before the one that was on the head of the list when we 
  // started.
  //
  // m_synchronizer->Synchronize will return true if the wait was completed
  // without interruption, otherwise it will return false.  So our goal is to
  // sit in a for loop trying to synchronize until it returns true.  If this 
  // happens we will have successfully synchronized the execution time of the
  // next event with the wall clock time of the synchronizer.
  //

  for (;;) 
    {
      uint64_t tsDelay = 0;
      uint64_t tsNow = m_currentTs;
      uint64_t tsNext = 0;
      //
      // NextTs peeks into the event list and returns the time stamp of the first 
      // event in the list.  We need to protect this kind of access with a critical
      // section.
      // 
      { 
        CriticalSection cs (m_mutex);

        NS_ASSERT_MSG (NextTs () >= m_currentTs, 
          "RealtimeSimulatorImpl::ProcessOneEvent (): "
          "Next event time earlier than m_currentTs (list order error)");
        //
        // Since we are in realtime mode, the time to delay has got to be the 
        // difference between the current realtime and the timestamp of the next 
        // event.  Since m_currentTs is actually the timestamp of the last event we 
        // executed, it's not particularly meaningful for us here since real time has
        // since elapsed.  
        //
        // It is possible that the current realtime has drifted past the next event
        // time so we need to be careful about that and not delay in that case.
        //
        NS_ASSERT_MSG (m_synchronizer->Realtime (), 
          "RealtimeSimulatorImpl::ProcessOneEvent (): Synchronizer reports not Realtime ()");

        tsNow = m_synchronizer->GetCurrentRealtime ();
        tsNext = NextTs ();

        if (tsNext <= tsNow)
          {
            tsDelay = 0;
          }
        else
          {
            tsDelay = tsNext - tsNow;
          }
      
        m_synchronizer->SetCondition (false);
      }

      //
      // So, we have a time to delay.  This time may actually not be valid anymore
      // since we released the critical section and a Schedule may have snuck in just
      // after the closing brace above.
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
      // a Simulator::Schedule is done, the wait down in Synchronize will exit and
      // Synchronize will return (false).
      //
      // So, we set this condition to false in a critical section.  The code that
      // sets the condition (Simulator::Schedule) also runs protected by the same
      // critical section mutex -- there is no race.  We call Synchronize -- if no
      // Simulator::Schedule is done, the waits (sleep- and spin-wait) will complete
      // and Synchronize will return true.  If a Schedule is done before we get to 
      // Synchronize, the Synchronize code will check the condition before going to
      // sleep.  If a Schedule happens while we are sleeping, we let the kernel wake
      // us up.
      //
      // So the bottom line is that we just stay in this for loop, looking for the
      // next timestamp and attempting to sleep until its due.  If we've slept until
      // the timestamp is due, Synchronize() returns true and we break out of the 
      //sync loop.  If we're interrupted we continue in the loop.  We may find that
      // the next timestamp is actually earlier than we thought, but we continue 
      // around the loop and reevaluate and wait for that one correctly.
      //
      if (m_synchronizer->Synchronize (tsNow, tsDelay))
        {
          NS_LOG_LOGIC ("Interrupted ...");
          break;
        }
    }

  //
  // Okay, now more words.  We have slept without interruption until the 
  // timestamp we found at the head of the event list when we started the sleep.
  // We are now outside a critical section, so another schedule operation can
  // sneak in.  What does this mean?  The only thing that can "go wrong" is if
  // the new event was moved in ahead of the timestamp for which we waited.
  //
  // If you think about it, this is not a problem, since the best we can 
  // possibly do is to execute the event as soon as we can execute it.  We'll
  // be a little late, but we were late anyway.
  //
  // So we just go ahead and pull the first event off of the list, even though
  // it may be the case that it's not actually the one we waited for.
  //
  // One final tidbit is, "what the heck time is it anyway"?  The simulator is
  // going to want to get a timestamp from the next event and wait until the
  // wall clock time is equal to the timestamp.  At the point when those times
  // are the same (roughly) we get to this point and set the m_currentTs below.
  // That's straightforward here, but you might ask, how does the next event get
  // the time when it is inserted from an external source?
  //
  // The method RealtimeSimulatorImpl::ScheduleNow takes makes an event and 
  // needs to schedule that event for immediate execution.  If the simulator is 
  // not locked to a realtime source, the current time is m_currentTs.  If it is
  // locked to a realtime source, we need to use the real current real time.
  // This real time is then used to set the event execution time and will be 
  // read by the next.GetTs below and will set the current simulation time.
  //
  EventId next;

  { 
    CriticalSection cs (m_mutex);

    NS_ASSERT_MSG (m_events->IsEmpty () == false, 
      "RealtimeSimulatorImpl::ProcessOneEvent(): event queue is empty");

    next = m_events->RemoveNext ();
    --m_unscheduledEvents;
  }

  NS_ASSERT_MSG (next.GetTs () >= m_currentTs,
    "RealtimeSimulatorImpl::ProcessOneEvent(): "
    "next.GetTs() earlier than m_currentTs (list order error)");
  NS_LOG_LOGIC ("handle " << next.GetTs ());
  m_currentTs = next.GetTs ();
  m_currentUid = next.GetUid ();

  // 
  // We're about to run the event and we've done our best to synchronize this
  // event execution time to real time.  Now, if we're in SYNC_HARD_LIMIT mode
  // we have to decide if we've done a good enough job and if we haven't, we've
  // been asked to commit ritual suicide.
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

void 
RealtimeSimulatorImpl::Stop (Time const &time)
{
  NS_LOG_FUNCTION (time);
  NS_ASSERT_MSG (time.IsPositive (),
    "RealtimeSimulatorImpl::Stop(): Negative time");

  Time absolute = Simulator::Now () + time;
  m_stopAt = absolute.GetTimeStep ();
  //
  // For the realtime case, we need a real event sitting out at the end of time
  // to keep the simulator running (sleeping) while there are no other events 
  // present.  If an "external" device in another thread decides to schedule an
  // event, the sleeping synchronizer will be awakened and the new event will
  // be run.
  //
  // The easiest thing to do is to call back up into the simulator to take 
  // advantage of all of the nice event wrappers.  This will call back down into
  // RealtimeSimulatorImpl::Schedule to do the work.
  //
  Simulator::Schedule (absolute, &Placeholder);
}

EventId
RealtimeSimulatorImpl::Schedule (Time const &time, const Ptr<EventImpl> &event)
{
  NS_LOG_FUNCTION (time << event);

  //
  // This is a little tricky.  We get a Ptr<EventImpl> passed down to us in some
  // thread context.  This Ptr<EventImpl> is not yet shared in any way.  It is 
  // possible however that the calling context is not the context of the main 
  // scheduler thread (eg. it is in the context of a separate device thread).  
  // It would be bad (TM) if we naively wrapped the EventImpl up in an EventId 
  // that would be accessible from multiple threads without considering thread 
  // safety.
  //
  // Having multiple EventId containing Ptr<EventImpl> in different thread
  // contexts creates a situation where atomic reference count decrements
  // would be required (think about an event being scheduled with the calling
  // yielding immediately afterward.  The scheduler could become ready and 
  // fire the event which would eventually want to release the EventImpl.  If
  // the calling thread were readied and executed in mid-release, it would also
  // want to release the EventImpl "at the same time."  If we are careless about
  // this, multiple deletes are sure to eventually happen as the threads 
  // separately play with the EventImpl reference counts.
  // 
  // The way this all works is that we created an EventImpl in the template 
  // function that called us, which may be in the context of a thread separate
  // from the simulator thread.  We are managing the lifetime of the EventImpl
  // with an intrusive reference count.  The count was initialized to one when
  // it was created and is still one right now.  We're going to "wrap" this
  // EventImpl in an EventId in this method.  There's an assignment of our 
  // Ptr<EventImpl> into another Ptr<EventImpl> inside the EventId which will 
  // bump the ref count to two.  We're going to return this EventId to the 
  // caller so the calling thread will hold a reference to the underlying 
  // EventImpl.  This is what causes the first half of the multithreading issue.
  //
  // We are going to call Insert() on the EventId to put the event into the 
  // scheduler.  Down there, it will do a PeekEventImpl to get the pointer to
  // the EventImpl and manually increment the reference count (to three).  From 
  // there, the sheduler works with the EventImpl pointer.  When the EventId 
  // below goes out of scope, the Ptr<EventImpl> destructor is run and the ref
  // count is decremented to two.  When this function returns to the calling
  // template function, the Ptr<EventImpl> there will go out of scope and we'll
  // decrement the EventImpl ref count leaving it to be the one held by our
  // scheduler directly.
  //
  // The scheduler removes the EventImpl in Remove() or RemoveNext().  In the
  // case of Remove(), the scheduler is provided an Event reference and locates
  // the corresponding EventImpl in the event list.  It gets the raw pointer to
  // the EventImpl, erases the pointer in the list, and manually calls Unref()
  // on the pointer.  In RemoveNext(), it gets the raw pointer from the list and
  // assigns it to a Ptr<EventImpl> without bumping the reference count, thereby
  // transferring ownership to a containing EventId.  This is the second half of
  // the multithreading issue.
  //
  // It's clear that we cannot have a situation where the EventImpl is "owned" by
  // multiple threads.  The calling thread is free to hold the EventId as long as
  // it wants and manage the reference counts to the underlying EventImpl all it
  // wants.  The scheduler is free to do the same; and will eventually release
  // the reference in the context of thread running ProcessOneEvent().  It is 
  // "a bad thing" (TM) if these two threads decide to release the underlying
  // EventImpl "at the same time."
  //
  // The answer is to make the reference counting of the EventImpl thread safe; 
  // which we do.  We don't want to force the event implementation to carry around
  // a mutex, so we "lend" it one using a RealtimeEventLock object (m_eventLock)
  // in the constructor and take it back in the destructor.
  //
  EventId id;

  {
    CriticalSection cs (m_mutex);

    NS_ASSERT_MSG (time.IsPositive (),
      "RealtimeSimulatorImpl::Schedule(): Negative time");
    NS_ASSERT_MSG (
      time >= TimeStep (m_currentTs),
      "RealtimeSimulatorImpl::Schedule(): time < m_currentTs");

    uint64_t ts = (uint64_t) time.GetTimeStep ();

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

    id = EventId (event, m_synchronizer->GetCurrentRealtime (), m_uid);
    
    m_uid++;
    ++m_unscheduledEvents;
    m_events->Insert (id);
    m_synchronizer->Signal ();
  }

  return id;
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
RealtimeSimulatorImpl::Now (void) const
{
  return TimeStep (m_currentTs);
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
