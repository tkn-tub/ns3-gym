/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 */

#include "simulator.h"
#include "scheduler.h"
#include "event-impl.h"

#include "ns3/assert.h"
#include "ns3/default-value.h"


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


class SimulatorPrivate {
public:
  SimulatorPrivate (Scheduler *events);
  ~SimulatorPrivate ();

  void EnableLogTo (char const *filename);

  bool IsFinished (void) const;
  Time Next (void) const;
  void Stop (void);
  void StopAt (Time const &time);
  EventId Schedule (Time const &time, EventImpl *event);
  void ScheduleNow (EventImpl *event);
  void ScheduleDestroy (EventImpl *event);
  void Remove (EventId ev);
  void Cancel (EventId &ev);
  bool IsExpired (EventId ev);
  void Run (void);
  Time Now (void) const;

private:
  void ProcessOneEvent (void);
  uint64_t NextTs (void) const;

  typedef std::list<std::pair<EventImpl *,uint32_t> > Events;
  Events m_destroy;
  uint64_t m_stopAt;
  bool m_stop;
  Scheduler *m_events;
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




SimulatorPrivate::SimulatorPrivate (Scheduler *events)
{
  m_stop = false;
  m_stopAt = 0;
  m_events = events;
  // uids are allocated from 1.
  m_uid = 1; 
  // before ::Run is entered, the m_currentUid will be zero
  m_currentUid = 0;
  m_logEnable = false;
  m_currentTs = 0;
  m_unscheduledEvents = 0;
}

SimulatorPrivate::~SimulatorPrivate ()
{
  while (!m_destroy.empty ()) 
    {
      EventImpl *ev = m_destroy.front ().first;
      m_destroy.pop_front ();
      TRACE ("handle destroy " << ev);
      ev->Invoke ();
      delete ev;
    }
  delete m_events;
  m_events = (Scheduler *)0xdeadbeaf;
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
  EventImpl *nextEv = m_events->PeekNext ();
  Scheduler::EventKey nextKey = m_events->PeekNextKey ();
  m_events->RemoveNext ();

  NS_ASSERT (nextKey.m_ts >= m_currentTs);
  --m_unscheduledEvents;

  TRACE ("handle " << nextEv);
  m_currentTs = nextKey.m_ts;
  m_currentUid = nextKey.m_uid;
  if (m_logEnable) 
    {
      m_log << "e "<<nextKey.m_uid << " " << nextKey.m_ts << std::endl;
    }
  nextEv->Invoke ();
  delete nextEv;
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
  Scheduler::EventKey nextKey = m_events->PeekNextKey ();
  return nextKey.m_ts;
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
SimulatorPrivate::Schedule (Time const &time, EventImpl *event)
{
  NS_ASSERT (time.IsPositive ());
  NS_ASSERT (time >= TimeStep (m_currentTs));
  uint64_t ts = (uint64_t) time.GetTimeStep ();
  Scheduler::EventKey key = {ts, m_uid};
  if (m_logEnable) 
    {
      m_log << "i "<<m_currentUid<<" "<<m_currentTs<<" "
            <<m_uid<<" "<<time.GetTimeStep () << std::endl;
    }
  m_uid++;
  ++m_unscheduledEvents;
  return m_events->Insert (event, key);
}
void 
SimulatorPrivate::ScheduleNow (EventImpl *event)
{
  uint64_t ts = m_currentTs;
  Scheduler::EventKey key = {ts, m_uid};
  if (m_logEnable) 
    {
      m_log << "i "<<m_currentUid<<" "<<m_currentTs<<" "
            <<m_uid<<" "<<ts << std::endl;
    }
  m_uid++;
  ++m_unscheduledEvents;
  m_events->Insert (event, key);
}
void 
SimulatorPrivate::ScheduleDestroy (EventImpl *event)
{
  m_destroy.push_back (std::make_pair (event, m_uid));  
  if (m_logEnable) 
  {
    m_log << "id " << m_currentUid << " " << Now ().GetTimeStep () << " "
          << m_uid << std::endl;
  }
  m_uid++;
}

Time
SimulatorPrivate::Now (void) const
{
  return TimeStep (m_currentTs);
}

void
SimulatorPrivate::Remove (EventId ev)
{
  Scheduler::EventKey key;
  EventImpl *impl = m_events->Remove (ev, &key);
  delete impl;
  if (m_logEnable) 
    {
      m_log << "r " << m_currentUid << " " << m_currentTs << " "
            << key.m_uid << " " << key.m_ts << std::endl;
    }
  --m_unscheduledEvents;
}

void
SimulatorPrivate::Cancel (EventId &id)
{
  id.Cancel ();
}

bool
SimulatorPrivate::IsExpired (EventId ev)
{
  if (ev.GetEventImpl () == 0 ||
      ev.GetTs () < m_currentTs ||
      (ev.GetTs () == m_currentTs &&
       ev.GetUid () <= m_currentUid) ||
      ev.GetEventImpl ()->IsCancelled ()) 
    {
      return true;
    }
  else
    {
      return false;
    }
}


}; // namespace ns3


#include "scheduler-list.h"
#include "scheduler-heap.h"
#include "scheduler-map.h"
#include "scheduler-factory.h"


namespace ns3 {

SimulatorPrivate *Simulator::m_priv = 0;

void Simulator::SetLinkedList (void)
{
  Bind ("Scheduler", "List");
}
void Simulator::SetBinaryHeap (void)
{
  Bind ("Scheduler", "BinaryHeap");
}
void Simulator::SetStdMap (void)
{
  Bind ("Scheduler", "Map");
}
void 
Simulator::SetExternal (const std::string &external)
{
  Bind ("Scheduler", external);
}
void Simulator::EnableLogTo (char const *filename)
{
  GetPriv ()->EnableLogTo (filename);
}


SimulatorPrivate *
Simulator::GetPriv (void)
{
  if (m_priv == 0) 
    {
      Scheduler *events = SchedulerFactory::CreateDefault ();
      m_priv = new SimulatorPrivate (events);
    }
  TRACE_S ("priv " << m_priv);
  return m_priv;
}

void
Simulator::Destroy (void)
{
  delete m_priv;
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

EventImpl *
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
  return ev;
}
EventId
Simulator::Schedule (Time const &time, EventImpl *ev)
{
  return GetPriv ()->Schedule (Now () + time, ev);
}
void
Simulator::ScheduleNow (EventImpl *ev)
{
  GetPriv ()->ScheduleNow (ev);
}
void
Simulator::ScheduleDestroy (EventImpl *ev)
{
  GetPriv ()->ScheduleDestroy (ev);
}  
EventId
Simulator::Schedule (Time const &time, void (*f) (void))
{
  return Schedule (time, MakeEvent (f));
}
void
Simulator::ScheduleNow (void (*f) (void))
{
  return ScheduleNow (MakeEvent (f));
}
void
Simulator::ScheduleDestroy (void (*f) (void))
{
  return ScheduleDestroy (MakeEvent (f));
}


void
Simulator::Remove (EventId ev)
{
  return GetPriv ()->Remove (ev);
}

void
Simulator::Cancel (EventId &ev)
{
  return GetPriv ()->Cancel (ev);
}
bool 
Simulator::IsExpired (EventId id)
{
  return GetPriv ()->IsExpired (id);
}

Time Now (void)
{
  return Time (Simulator::Now ());
}

}; // namespace ns3


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
  

class SimulatorTests : public Test {
public:
  SimulatorTests ();
  // only here for testing of Ptr<>
  void Ref (void);
  void Unref (void);
  virtual ~SimulatorTests ();
  virtual bool RunTests (void);
private:
  uint64_t NowUs ();
  bool RunOneTest (void);
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
  
  bool m_b;
  bool m_a;
  bool m_c;
  bool m_d;
  EventId m_idC;
};

SimulatorTests::SimulatorTests ()
  : Test ("Simulator")
{}
SimulatorTests::~SimulatorTests ()
{}
void 
SimulatorTests::Ref (void)
{}
void 
SimulatorTests::Unref (void)
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

bool
SimulatorTests::RunOneTest (void)
{
  bool ok = true;
  m_a = true;
  m_b = false;
  m_c = true;
  m_d = false;

  EventId a = Simulator::Schedule (MicroSeconds (10), &SimulatorTests::A, this, 1);
  Simulator::Schedule (MicroSeconds (11), &SimulatorTests::B, this, 2);
  m_idC = Simulator::Schedule (MicroSeconds (12), &SimulatorTests::C, this, 3);

  if (m_idC.IsExpired ()) 
    {
      ok = false;
    }
  if (a.IsExpired ())
    {
      ok = false;
    }
  Simulator::Cancel (a);
  if (!a.IsExpired ())
    {
      ok = false;
    }
  Simulator::Run ();

  if (!m_a || !m_b || !m_c || !m_d) 
    {
      ok = false;
    }
  return ok;
}
bool 
SimulatorTests::RunTests (void)
{
  bool ok = true;

  Simulator::SetLinkedList ();
  if (!RunOneTest ()) 
    {
      ok = false;
    }
  Simulator::Destroy ();
  Simulator::SetBinaryHeap ();
  if (!RunOneTest ()) 
    {
      ok = false;
    }
  Simulator::Destroy ();
  Simulator::SetStdMap ();
  if (!RunOneTest ()) 
    {
      ok = false;
    }
  Simulator::Destroy ();


  Simulator::Schedule (Seconds (0.0), &foo0);
  Simulator::Schedule (Seconds (0.0), &foo1, 0);
  Simulator::Schedule (Seconds (0.0), &foo2, 0, 0);
  Simulator::Schedule (Seconds (0.0), &foo3, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &foo4, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &foo5, 0, 0, 0, 0, 0);
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
  Simulator::ScheduleNow (&foo0);
  Simulator::ScheduleNow (&foo1, 0);
  Simulator::ScheduleNow (&foo2, 0, 0);
  Simulator::ScheduleNow (&foo3, 0, 0, 0);
  Simulator::ScheduleNow (&foo4, 0, 0, 0, 0);
  Simulator::ScheduleNow (&foo5, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar0, this);
  Simulator::ScheduleNow (&SimulatorTests::bar1, this, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar2, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar3, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar4, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTests::bar5, this, 0, 0, 0, 0, 0);
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
  Simulator::ScheduleDestroy (&SimulatorTests::bar0, this);
  Simulator::ScheduleDestroy (&SimulatorTests::bar1, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar2, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar3, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar4, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar0, Ptr<SimulatorTests> (this));
  Simulator::ScheduleDestroy (&SimulatorTests::bar1, Ptr<SimulatorTests> (this), 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar2, Ptr<SimulatorTests> (this), 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar3, Ptr<SimulatorTests> (this), 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar4, Ptr<SimulatorTests> (this), 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTests::bar5, Ptr<SimulatorTests> (this), 0, 0, 0, 0, 0);

  Simulator::Run ();
  Simulator::Destroy ();

  return ok;
}

SimulatorTests gSimulatorTests;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */



