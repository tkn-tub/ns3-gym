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
#include "scheduler.h"
#include "event-impl.h"

#include "ns3/ptr.h"
#include "ns3/pointer.h"
#include "ns3/assert.h"


#include <math.h>
#include <fstream>
#include <list>
#include <vector>
#include <iostream>

#define noTRACE_SIMU 1

#ifdef TRACE_SIMU
#include <iostream>
# define TRACE(x) \
std::cout << "SIMU TRACE " << Simulator::Now () << " " << x << std::endl;
# define TRACE_S(x) \
std::cout << "SIMU TRACE " << x << std::endl;
#else /* TRACE_SIMU */
# define TRACE(format,...)
# define TRACE_S(format,...)
#endif /* TRACE_SIMU */


namespace ns3 {


/**
 * \brief private implementation detail of the Simulator API.
 */
class SimulatorPrivate : public Object
{
public:
  static TypeId GetTypeId (void);

  SimulatorPrivate ();
  ~SimulatorPrivate ();

  void Destroy ();

  void EnableLogTo (char const *filename);

  bool IsFinished (void) const;
  Time Next (void) const;
  void Stop (void);
  void StopAt (Time const &time);
  EventId Schedule (Time const &time, const Ptr<EventImpl> &event);
  EventId ScheduleNow (const Ptr<EventImpl> &event);
  EventId ScheduleDestroy (const Ptr<EventImpl> &event);
  void Remove (const EventId &ev);
  void Cancel (const EventId &ev);
  bool IsExpired (const EventId &ev) const;
  void Run (void);
  Time Now (void) const;
  Time GetDelayLeft (const EventId &id) const;
  Time GetMaximumSimulationTime (void) const;

  void SetScheduler (Ptr<Scheduler> scheduler);
  Ptr<Scheduler> GetScheduler (void) const;

private:
  void ProcessOneEvent (void);
  uint64_t NextTs (void) const;

  typedef std::list<EventId> DestroyEvents;
  DestroyEvents m_destroyEvents;
  uint64_t m_stopAt;
  bool m_stop;
  Ptr<Scheduler> m_events;
  uint32_t m_uid;
  uint32_t m_currentUid;
  uint64_t m_currentTs;
  std::ofstream m_log;
  std::ifstream m_inputLog;
  bool m_logEnable;
  // number of events that have been inserted but not yet scheduled,
  // not counting the "destroy" events; this is used for validation
  int m_unscheduledEvents;
};

NS_OBJECT_ENSURE_REGISTERED (SimulatorPrivate);


TypeId
SimulatorPrivate::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SimulatorPrivate")
    .SetParent<Object> ()
    .AddConstructor<SimulatorPrivate> ()
    .AddAttribute ("Scheduler",
                   "XXX",
                   Pointer (),
                   // XXX: allow getting the scheduler too.
                   MakePointerAccessor (&SimulatorPrivate::SetScheduler),
                   MakePointerChecker<Scheduler> ())
    ;
  return tid;
}

SimulatorPrivate::SimulatorPrivate ()
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

SimulatorPrivate::~SimulatorPrivate ()
{
  while (!m_events->IsEmpty ())
    {
      EventId next = m_events->RemoveNext ();
    }
  m_events = 0;
}
void
SimulatorPrivate::Destroy ()
{
  while (!m_destroyEvents.empty ()) 
    {
      Ptr<EventImpl> ev = m_destroyEvents.front ().PeekEventImpl ();
      m_destroyEvents.pop_front ();
      TRACE ("handle destroy " << ev);
      if (!ev->IsCancelled ())
        {
          ev->Invoke ();
        }
    }
}

void
SimulatorPrivate::SetScheduler (Ptr<Scheduler> scheduler)
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
SimulatorPrivate::GetScheduler (void) const
{
  return m_events;
}

void
SimulatorPrivate::EnableLogTo (char const *filename)
{
  m_log.open (filename);
  m_logEnable = true;
}

void
SimulatorPrivate::ProcessOneEvent (void)
{
  EventId next = m_events->RemoveNext ();

  NS_ASSERT (next.GetTs () >= m_currentTs);
  --m_unscheduledEvents;

  TRACE ("handle " << nextEv);
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
SimulatorPrivate::IsFinished (void) const
{
  return m_events->IsEmpty ();
}
uint64_t
SimulatorPrivate::NextTs (void) const
{
  NS_ASSERT (!m_events->IsEmpty ());
  EventId id = m_events->PeekNext ();
  return id.GetTs ();
}
Time
SimulatorPrivate::Next (void) const
{
  return TimeStep (NextTs ());
}

void
SimulatorPrivate::Run (void)
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
SimulatorPrivate::Stop (void)
{
  m_stop = true;
}
void 
SimulatorPrivate::StopAt (Time const &at)
{
  NS_ASSERT (at.IsPositive ());
  m_stopAt = at.GetTimeStep ();
}
EventId
SimulatorPrivate::Schedule (Time const &time, const Ptr<EventImpl> &event)
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
SimulatorPrivate::ScheduleNow (const Ptr<EventImpl> &event)
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
SimulatorPrivate::ScheduleDestroy (const Ptr<EventImpl> &event)
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
SimulatorPrivate::Now (void) const
{
  return TimeStep (m_currentTs);
}
Time 
SimulatorPrivate::GetDelayLeft (const EventId &id) const
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
SimulatorPrivate::Remove (const EventId &ev)
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
SimulatorPrivate::Cancel (const EventId &id)
{
  if (!IsExpired (id))
    {
      id.PeekEventImpl ()->Cancel ();
    }
}

bool
SimulatorPrivate::IsExpired (const EventId &ev) const
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
SimulatorPrivate::GetMaximumSimulationTime (void) const
{
  // XXX: I am fairly certain other compilers use other non-standard
  // post-fixes to indicate 64 bit constants.
  return TimeStep (0x7fffffffffffffffLL);
}



}; // namespace ns3


#include "scheduler-list.h"
#include "scheduler-heap.h"
#include "scheduler-map.h"


namespace ns3 {

Ptr<SimulatorPrivate> Simulator::m_priv = 0;

void Simulator::SetScheduler (Ptr<Scheduler> scheduler)
{
  GetPriv ()->SetScheduler (scheduler);
}
void Simulator::EnableLogTo (char const *filename)
{
  GetPriv ()->EnableLogTo (filename);
}


Ptr<SimulatorPrivate>
Simulator::GetPriv (void)
{
  if (m_priv == 0) 
    {
      m_priv = CreateObject<SimulatorPrivate> ();
      Ptr<Scheduler> scheduler = CreateObject<SchedulerMap> ();
      m_priv->SetScheduler (scheduler);
    }
  TRACE_S ("priv " << m_priv);
  return m_priv;
}

void
Simulator::Destroy (void)
{
  if (m_priv == 0)
    {
      return;
    }
  m_priv->Destroy ();
  m_priv = 0;
}


bool 
Simulator::IsFinished (void)
{
  return GetPriv ()->IsFinished ();
}
Time
Simulator::Next (void)
{
  return GetPriv ()->Next ();
}


void 
Simulator::Run (void)
{
  GetPriv ()->Run ();
}
void 
Simulator::Stop (void)
{
  TRACE ("stop");
  GetPriv ()->Stop ();
}
void 
Simulator::StopAt (Time const &at)
{
  GetPriv ()->StopAt (at);
}
Time
Simulator::Now (void)
{
  return GetPriv ()->Now ();
}
Time
Simulator::GetDelayLeft (const EventId &id)
{
  return GetPriv ()->GetDelayLeft (id);
}

Ptr<EventImpl>
Simulator::MakeEvent (void (*f) (void))
{
    // zero arg version
  class EventFunctionImpl0 : public EventImpl {
  public:
    typedef void (*F)(void);
      
    EventFunctionImpl0 (F function) 
      : m_function (function)
    {}
    virtual ~EventFunctionImpl0 () {}
  protected:
    virtual void Notify (void) { 
      (*m_function) (); 
    }
  private:
  	F m_function;
  } *ev = new EventFunctionImpl0 (f);
  return Ptr<EventImpl> (ev, false);
}
EventId
Simulator::Schedule (Time const &time, const Ptr<EventImpl> &ev)
{
  return GetPriv ()->Schedule (Now () + time, ev);
}
EventId
Simulator::ScheduleNow (const Ptr<EventImpl> &ev)
{
  return GetPriv ()->ScheduleNow (ev);
}
EventId
Simulator::ScheduleDestroy (const Ptr<EventImpl> &ev)
{
  return GetPriv ()->ScheduleDestroy (ev);
}  
EventId
Simulator::Schedule (Time const &time, void (*f) (void))
{
  return Schedule (time, MakeEvent (f));
}
EventId
Simulator::ScheduleNow (void (*f) (void))
{
  return ScheduleNow (MakeEvent (f));
}
EventId
Simulator::ScheduleDestroy (void (*f) (void))
{
  return ScheduleDestroy (MakeEvent (f));
}


void
Simulator::Remove (const EventId &ev)
{
  return GetPriv ()->Remove (ev);
}

void
Simulator::Cancel (const EventId &ev)
{
  return GetPriv ()->Cancel (ev);
}
bool 
Simulator::IsExpired (const EventId &id)
{
  return GetPriv ()->IsExpired (id);
}

Time Now (void)
{
  return Time (Simulator::Now ());
}

Time 
Simulator::GetMaximumSimulationTime (void)
{
  return GetPriv ()->GetMaximumSimulationTime ();
}

} // namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include "ns3/ptr.h"

namespace ns3 {

static void foo0 (void)
{}
static void foo1 (int)
{}
static void foo2 (int, int)
{}
static void foo3 (int, int, int)
{}
static void foo4 (int, int, int, int)
{}
static void foo5 (int, int, int, int, int)
{}

#if 1
static void ber1 (int &)
{}
static void ber2 (int &, int &)
{}
static void ber3 (int &, int &, int &)
{}
static void ber4 (int &, int &, int &, int &)
{}
static void ber5 (int &, int &, int &, int &, int &)
{}
#endif

static void cber1 (const int &)
{}
static void cber2 (const int &, const int &)
{}
static void cber3 (const int &, const int &, const int &)
{}
static void cber4 (const int &, const int &, const int &, const int &)
{}
static void cber5 (const int &, const int &, const int &, const int &, const int &)
{}
  

class SimulatorTests : public Test
{
public:
  SimulatorTests ();
  // only here for testing of Ptr<>
  void Ref (void) const;
  void Unref (void) const;
  virtual ~SimulatorTests ();
  virtual bool RunTests (void);
private:
  uint64_t NowUs ();
  bool RunOneTest (void);
  void RunTestsConst (void) const;
  void A (int a);
  void B (int b);
  void C (int c);
  void D (int d);
  void bar0 (void);
  void bar1 (int);
  void bar2 (int, int);
  void bar3 (int, int, int);
  void bar4 (int, int, int, int);
  void bar5 (int, int, int, int, int);
  void baz1 (int &);
  void baz2 (int &, int &);
  void baz3 (int &, int &, int &);
  void baz4 (int &, int &, int &, int &);
  void baz5 (int &, int &, int &, int &, int &);
  void cbaz1 (const int &);
  void cbaz2 (const int &, const int &);
  void cbaz3 (const int &, const int &, const int &);
  void cbaz4 (const int &, const int &, const int &, const int &);
  void cbaz5 (const int &, const int &, const int &, const int &, const int &);

  void bar0c (void) const;
  void bar1c (int) const;
  void bar2c (int, int) const;
  void bar3c (int, int, int) const;
  void bar4c (int, int, int, int) const;
  void bar5c (int, int, int, int, int) const;
  void baz1c (int &) const;
  void baz2c (int &, int &) const;
  void baz3c (int &, int &, int &) const;
  void baz4c (int &, int &, int &, int &) const;
  void baz5c (int &, int &, int &, int &, int &) const;
  void cbaz1c (const int &) const;
  void cbaz2c (const int &, const int &) const;
  void cbaz3c (const int &, const int &, const int &) const;
  void cbaz4c (const int &, const int &, const int &, const int &) const;
  void cbaz5c (const int &, const int &, const int &, const int &, const int &) const;

  void destroy (void);
  
  bool m_b;
  bool m_a;
  bool m_c;
  bool m_d;
  EventId m_idC;
  bool m_destroy;
  EventId m_destroyId;
};

SimulatorTests::SimulatorTests ()
  : Test ("Simulator")
{}
SimulatorTests::~SimulatorTests ()
{}
void 
SimulatorTests::Ref (void) const
{}
void 
SimulatorTests::Unref (void) const
{}
uint64_t
SimulatorTests::NowUs (void)
{
  uint64_t ns = Now ().GetNanoSeconds ();
  return ns / 1000;
}  
void
SimulatorTests::A (int a)
{
  m_a = false;
}
void
SimulatorTests::B (int b)
{
  if (b != 2 || NowUs () != 11) 
    {
      m_b = false;
    } 
  else 
    {
      m_b = true;
    }
  Simulator::Remove (m_idC);
  Simulator::Schedule (MicroSeconds (10), &SimulatorTests::D, this, 4);
}
void
SimulatorTests::C (int c)
{
  m_c = false;
}
void
SimulatorTests::D (int d)
{
  if (d != 4 || NowUs () != (11+10)) 
    {
      m_d = false;
    } 
  else 
    {
      m_d = true;
    }
}
void
SimulatorTests::destroy (void)
{
  if (m_destroyId.IsExpired ())
    {
      m_destroy = true; 
    }
}
void 
SimulatorTests::bar0 (void)
{}
void 
SimulatorTests::bar1 (int)
{}
void 
SimulatorTests::bar2 (int, int)
{}
void 
SimulatorTests::bar3 (int, int, int)
{}
void 
SimulatorTests::bar4 (int, int, int, int)
{}
void 
SimulatorTests::bar5 (int, int, int, int, int)
{}

void
SimulatorTests::baz1 (int &)
{}
void
SimulatorTests::baz2 (int &, int &)
{}
void
SimulatorTests::baz3 (int &, int &, int &)
{}
void 
SimulatorTests::baz4 (int &, int &, int &, int &)
{}
void 
SimulatorTests::baz5 (int &, int &, int &, int &, int &)
{}

void
SimulatorTests::cbaz1 (const int &)
{}
void
SimulatorTests::cbaz2 (const int &, const int &)
{}
void
SimulatorTests::cbaz3 (const int &, const int &, const int &)
{}
void 
SimulatorTests::cbaz4 (const int &, const int &, const int &, const int &)
{}
void 
SimulatorTests::cbaz5 (const int &, const int &, const int &, const int &, const int &)
{}

void 
SimulatorTests::bar0c (void) const
{}
void 
SimulatorTests::bar1c (int) const
{}
void 
SimulatorTests::bar2c (int, int) const
{}
void 
SimulatorTests::bar3c (int, int, int) const
{}
void 
SimulatorTests::bar4c (int, int, int, int) const
{}
void 
SimulatorTests::bar5c (int, int, int, int, int) const
{}

void
SimulatorTests::baz1c (int &) const
{}
void
SimulatorTests::baz2c (int &, int &) const
{}
void
SimulatorTests::baz3c (int &, int &, int &) const
{}
void 
SimulatorTests::baz4c (int &, int &, int &, int &) const
{}
void 
SimulatorTests::baz5c (int &, int &, int &, int &, int &) const
{}

void
SimulatorTests::cbaz1c (const int &) const
{}
void
SimulatorTests::cbaz2c (const int &, const int &) const
{}
void
SimulatorTests::cbaz3c (const int &, const int &, const int &) const
{}
void 
SimulatorTests::cbaz4c (const int &, const int &, const int &, const int &) const
{}
void 
SimulatorTests::cbaz5c (const int &, const int &, const int &, const int &, const int &) const
{}

bool
SimulatorTests::RunOneTest (void)
{
  bool result = true;
  m_a = true;
  m_b = false;
  m_c = true;
  m_d = false;

  EventId a = Simulator::Schedule (MicroSeconds (10), &SimulatorTests::A, this, 1);
  Simulator::Schedule (MicroSeconds (11), &SimulatorTests::B, this, 2);
  m_idC = Simulator::Schedule (MicroSeconds (12), &SimulatorTests::C, this, 3);

  NS_TEST_ASSERT (!m_idC.IsExpired ());
  NS_TEST_ASSERT (!a.IsExpired ());
  Simulator::Cancel (a);
  NS_TEST_ASSERT (a.IsExpired ());
  Simulator::Run ();
  NS_TEST_ASSERT (m_a);
  NS_TEST_ASSERT (m_b);
  NS_TEST_ASSERT (m_c);
  NS_TEST_ASSERT (m_d);
  return result;
}
void
SimulatorTests::RunTestsConst (void) const
{
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar0c, this);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar1c, this, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar2c, this, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar3c, this, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar4c, this, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar5c, this, 0, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar0c, Ptr<const SimulatorTests> (this));
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar1c, Ptr<const SimulatorTests> (this), 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar2c, Ptr<const SimulatorTests> (this), 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar3c, Ptr<const SimulatorTests> (this), 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar4c, Ptr<const SimulatorTests> (this), 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar5c, Ptr<const SimulatorTests> (this), 0, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::cbaz1c, this, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::cbaz2c, this, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::cbaz3c, this, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::cbaz4c, this, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::cbaz5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar0c, this);
  Simulator::ScheduleNow (&SimulatorTests::bar1c, this, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar2c, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar3c, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar4c, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::cbaz1c, this, 0);
  Simulator::ScheduleNow (&SimulatorTests::cbaz2c, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::cbaz3c, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::cbaz4c, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::cbaz5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar0c, Ptr<const SimulatorTests> (this));
  Simulator::ScheduleNow (&SimulatorTests::bar1c, Ptr<const SimulatorTests> (this), 0);
  Simulator::ScheduleNow (&SimulatorTests::bar2c, Ptr<const SimulatorTests> (this), 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar3c, Ptr<const SimulatorTests> (this), 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar4c, Ptr<const SimulatorTests> (this), 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar5c, Ptr<const SimulatorTests> (this), 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar0c, this);
  Simulator::ScheduleDestroy (&SimulatorTests::bar1c, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar2c, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar3c, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar4c, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::cbaz1c, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::cbaz2c, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::cbaz3c, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::cbaz4c, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::cbaz5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar0c, Ptr<const SimulatorTests> (this));
  Simulator::ScheduleDestroy (&SimulatorTests::bar1c, Ptr<const SimulatorTests> (this), 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar2c, Ptr<const SimulatorTests> (this), 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar3c, Ptr<const SimulatorTests> (this), 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar4c, Ptr<const SimulatorTests> (this), 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar5c, Ptr<const SimulatorTests> (this), 0, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::baz1c, this, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::baz2c, this, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::baz3c, this, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::baz4c, this, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::baz5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::baz1c, this, 0);
  Simulator::ScheduleNow (&SimulatorTests::baz2c, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::baz3c, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::baz4c, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::baz5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::baz1c, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::baz2c, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::baz3c, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::baz4c, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::baz5c, this, 0, 0, 0, 0, 0);

  Simulator::Run ();
  Simulator::Destroy ();
}

bool 
SimulatorTests::RunTests (void)
{
  bool result = true;

  Simulator::Destroy ();
  Simulator::SetScheduler (CreateObject<SchedulerList> ());
  if (!RunOneTest ()) 
    {
      result = false;
    }
  Simulator::Destroy ();
  Simulator::SetScheduler (CreateObject<SchedulerHeap> ());
  if (!RunOneTest ()) 
    {
      result = false;
    }
  Simulator::Destroy ();
  Simulator::SetScheduler (CreateObject<SchedulerMap> ());
  if (!RunOneTest ()) 
    {
      result = false;
    }
  Simulator::Destroy ();


  Simulator::Schedule (Seconds (0.0), &foo0);
  Simulator::Schedule (Seconds (0.0), &foo1, 0);
  Simulator::Schedule (Seconds (0.0), &foo2, 0, 0);
  Simulator::Schedule (Seconds (0.0), &foo3, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &foo4, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &foo5, 0, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &cber1, 0);
  Simulator::Schedule (Seconds (0.0), &cber2, 0, 0);
  Simulator::Schedule (Seconds (0.0), &cber3, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &cber4, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &cber5, 0, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar0, this);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar1, this, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar2, this, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar3, this, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar4, this, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar5, this, 0, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar0, Ptr<SimulatorTests> (this));
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar1, Ptr<SimulatorTests> (this), 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar2, Ptr<SimulatorTests> (this), 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar3, Ptr<SimulatorTests> (this), 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar4, Ptr<SimulatorTests> (this), 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::bar5, Ptr<SimulatorTests> (this), 0, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::cbaz1, this, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::cbaz2, this, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::cbaz3, this, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::cbaz4, this, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::cbaz5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&foo0);
  Simulator::ScheduleNow (&foo1, 0);
  Simulator::ScheduleNow (&foo2, 0, 0);
  Simulator::ScheduleNow (&foo3, 0, 0, 0);
  Simulator::ScheduleNow (&foo4, 0, 0, 0, 0);
  Simulator::ScheduleNow (&foo5, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&cber1, 0);
  Simulator::ScheduleNow (&cber2, 0, 0);
  Simulator::ScheduleNow (&cber3, 0, 0, 0);
  Simulator::ScheduleNow (&cber4, 0, 0, 0, 0);
  Simulator::ScheduleNow (&cber5, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar0, this);
  Simulator::ScheduleNow (&SimulatorTests::bar1, this, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar2, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar3, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar4, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::cbaz1, this, 0);
  Simulator::ScheduleNow (&SimulatorTests::cbaz2, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::cbaz3, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::cbaz4, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::cbaz5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar0, Ptr<SimulatorTests> (this));
  Simulator::ScheduleNow (&SimulatorTests::bar1, Ptr<SimulatorTests> (this), 0);
  Simulator::ScheduleNow (&SimulatorTests::bar2, Ptr<SimulatorTests> (this), 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar3, Ptr<SimulatorTests> (this), 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar4, Ptr<SimulatorTests> (this), 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar5, Ptr<SimulatorTests> (this), 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&foo0);
  Simulator::ScheduleDestroy (&foo1, 0);
  Simulator::ScheduleDestroy (&foo2, 0, 0);
  Simulator::ScheduleDestroy (&foo3, 0, 0, 0);
  Simulator::ScheduleDestroy (&foo4, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&foo5, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&cber1, 0);
  Simulator::ScheduleDestroy (&cber2, 0, 0);
  Simulator::ScheduleDestroy (&cber3, 0, 0, 0);
  Simulator::ScheduleDestroy (&cber4, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&cber5, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar0, this);
  Simulator::ScheduleDestroy (&SimulatorTests::bar1, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar2, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar3, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar4, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::cbaz1, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::cbaz2, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::cbaz3, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::cbaz4, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::cbaz5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar0, Ptr<SimulatorTests> (this));
  Simulator::ScheduleDestroy (&SimulatorTests::bar1, Ptr<SimulatorTests> (this), 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar2, Ptr<SimulatorTests> (this), 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar3, Ptr<SimulatorTests> (this), 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar4, Ptr<SimulatorTests> (this), 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar5, Ptr<SimulatorTests> (this), 0, 0, 0, 0, 0);


  // the code below does not compile, as expected.
  //Simulator::Schedule (Seconds (0.0), &cber1, 0.0);

#if 1
  Simulator::Schedule (Seconds (0.0), &ber1, 0);
  Simulator::Schedule (Seconds (0.0), &ber2, 0, 0);
  Simulator::Schedule (Seconds (0.0), &ber3, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &ber4, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &ber5, 0, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::baz1, this, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::baz2, this, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::baz3, this, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::baz4, this, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTests::baz5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&ber1, 0);
  Simulator::ScheduleNow (&ber2, 0, 0);
  Simulator::ScheduleNow (&ber3, 0, 0, 0);
  Simulator::ScheduleNow (&ber4, 0, 0, 0, 0);
  Simulator::ScheduleNow (&ber5, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::baz1, this, 0);
  Simulator::ScheduleNow (&SimulatorTests::baz2, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::baz3, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::baz4, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::baz5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&ber1, 0);
  Simulator::ScheduleDestroy (&ber2, 0, 0);
  Simulator::ScheduleDestroy (&ber3, 0, 0, 0);
  Simulator::ScheduleDestroy (&ber4, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&ber5, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::baz1, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::baz2, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::baz3, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::baz4, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::baz5, this, 0, 0, 0, 0, 0);
#endif

  RunTestsConst ();

  EventId nowId = Simulator::ScheduleNow (&foo0);
  m_destroyId = Simulator::ScheduleDestroy (&SimulatorTests::destroy, this);
  NS_TEST_ASSERT (!m_destroyId.IsExpired ());

  Simulator::Run ();
  m_destroy = false;
  Simulator::Destroy ();
  NS_TEST_ASSERT (m_destroy);

  EventId anId = Simulator::ScheduleNow (&foo0);
  EventId anotherId = anId;
  NS_TEST_ASSERT (!(anId.IsExpired () || anotherId.IsExpired ()));

  Simulator::Remove (anId);
  NS_TEST_ASSERT (anId.IsExpired ());
  NS_TEST_ASSERT (anotherId.IsExpired ());

  Simulator::Run ();
  Simulator::Destroy ();

  Simulator::Schedule (Seconds (10.0), &SimulatorTests::baz1, this, 0);
  Simulator::StopAt (Seconds (1.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return result;
}

SimulatorTests gSimulatorTests;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */



