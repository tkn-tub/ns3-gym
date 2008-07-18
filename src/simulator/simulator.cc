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
#include "map-scheduler.h"
#include "event-impl.h"

#include "ns3/ptr.h"
#include "ns3/pointer.h"
#include "ns3/assert.h"
#include "ns3/log.h"

#include <math.h>
#include <fstream>
#include <list>
#include <vector>
#include <iostream>

NS_LOG_COMPONENT_DEFINE ("Simulator");

namespace ns3 {

#ifdef NS3_LOG_ENABLE
static void
TimePrinter (std::ostream &os)
{
  os << Simulator::Now ();
}
#endif /* NS3_LOG_ENABLE */

Ptr<SimulatorImpl> Simulator::m_impl = 0;

Ptr<SimulatorImpl>
Simulator::GetImpl (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (m_impl == 0) 
    {
      /* Note: we call LogSetTimePrinter below _after_ calling CreateObject 
       * because CreateObject can trigger calls to the logging framework which
       * would call the TimePrinter function above which would call 
       * Simulator::Now which would call Simulator::GetImpl, and, thus, get 
       * us in an infinite recursion until the stack explodes.
       */
      m_impl = CreateObject<SimulatorImpl> ();

      Ptr<Scheduler> scheduler = CreateObject<MapScheduler> ();
      m_impl->SetScheduler (scheduler);

      LogSetTimePrinter (&TimePrinter);
    }
  NS_LOG_LOGIC ("priv " << m_impl);
  return m_impl;
}

void
Simulator::Destroy (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (m_impl == 0)
    {
      return;
    }
  /* Note: we have to call LogSetTimePrinter (0) below because if we do not do
   * this, and restart a simulation after this call to Destroy, (which is 
   * legal), Simulator::GetImpl will trigger again an infinite recursion until
   * the stack explodes.
   */
  LogSetTimePrinter (0);
  m_impl->Destroy ();
  m_impl = 0;
}

void
Simulator::SetScheduler (Ptr<Scheduler> scheduler)
{
  NS_LOG_FUNCTION (scheduler);
  GetImpl ()->SetScheduler (scheduler);
}

void 
Simulator::EnableLogTo (char const *filename)
{
  NS_LOG_FUNCTION (filename);
  GetImpl ()->EnableLogTo (filename);
}

bool 
Simulator::IsFinished (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return GetImpl ()->IsFinished ();
}

Time
Simulator::Next (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return GetImpl ()->Next ();
}

void 
Simulator::Run (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  GetImpl ()->Run ();
}

void 
Simulator::Stop (void)
{
  NS_LOG_LOGIC ("stop");
  GetImpl ()->Stop ();
}

void 
Simulator::Stop (Time const &time)
{
  NS_LOG_FUNCTION (time);
  GetImpl ()->Stop (time);
}

Time
Simulator::Now (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return GetImpl ()->Now ();
}

Time
Simulator::GetDelayLeft (const EventId &id)
{
  NS_LOG_FUNCTION (&id);
  return GetImpl ()->GetDelayLeft (id);
}

Ptr<EventImpl>
Simulator::MakeEvent (void (*f) (void))
{
  NS_LOG_FUNCTION (f);
  // zero arg version
  class EventFunctionImpl0 : public EventImpl
  {
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
  NS_LOG_FUNCTION (time << ev);
  return GetImpl ()->Schedule (Now () + time, ev);
}

EventId
Simulator::ScheduleNow (const Ptr<EventImpl> &ev)
{
  NS_LOG_FUNCTION (ev);
  return GetImpl ()->ScheduleNow (ev);
}

EventId
Simulator::ScheduleDestroy (const Ptr<EventImpl> &ev)
{
  NS_LOG_FUNCTION (ev);
  return GetImpl ()->ScheduleDestroy (ev);
}  

EventId
Simulator::Schedule (Time const &time, void (*f) (void))
{
  NS_LOG_FUNCTION (time << f);
  return Schedule (time, MakeEvent (f));
}

EventId
Simulator::ScheduleNow (void (*f) (void))
{
  NS_LOG_FUNCTION (f);
  return ScheduleNow (MakeEvent (f));
}

EventId
Simulator::ScheduleDestroy (void (*f) (void))
{
  NS_LOG_FUNCTION (f);
  return ScheduleDestroy (MakeEvent (f));
}

void
Simulator::Remove (const EventId &ev)
{
  NS_LOG_FUNCTION (&ev);
  return GetImpl ()->Remove (ev);
}

void
Simulator::Cancel (const EventId &ev)
{
  NS_LOG_FUNCTION (&ev);
  return GetImpl ()->Cancel (ev);
}

bool 
Simulator::IsExpired (const EventId &id)
{
  NS_LOG_FUNCTION (&id);
  return GetImpl ()->IsExpired (id);
}

Time Now (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return Time (Simulator::Now ());
}

Time 
Simulator::GetMaximumSimulationTime (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return GetImpl ()->GetMaximumSimulationTime ();
}

} // namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include "ns3/ptr.h"
#include "list-scheduler.h"
#include "heap-scheduler.h"

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
  Simulator::SetScheduler (CreateObject<ListScheduler> ());
  if (!RunOneTest ()) 
    {
      result = false;
    }
  Simulator::Destroy ();
  Simulator::SetScheduler (CreateObject<HeapScheduler> ());
  if (!RunOneTest ()) 
    {
      result = false;
    }
  Simulator::Destroy ();
  Simulator::SetScheduler (CreateObject<MapScheduler> ());
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
  Simulator::Stop (Seconds (1.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return result;
}

SimulatorTests gSimulatorTests;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */



