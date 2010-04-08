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
#include "ns3/core-config.h"
#include "simulator.h"
#include "simulator-impl.h"
#include "scheduler.h"
#include "map-scheduler.h"
#include "event-impl.h"

#include "ns3/ptr.h"
#include "ns3/string.h"
#include "ns3/object-factory.h"
#include "ns3/global-value.h"
#include "ns3/assert.h"
#include "ns3/log.h"

#include <math.h>
#include <fstream>
#include <list>
#include <vector>
#include <iostream>

NS_LOG_COMPONENT_DEFINE ("Simulator");

namespace ns3 {

GlobalValue g_simTypeImpl = GlobalValue ("SimulatorImplementationType", 
  "The object class to use as the simulator implementation",
  StringValue ("ns3::DefaultSimulatorImpl"),
  MakeStringChecker ());

GlobalValue g_schedTypeImpl = GlobalValue ("SchedulerType", 
  "The object class to use as the scheduler implementation",
  TypeIdValue (MapScheduler::GetTypeId ()),
  MakeTypeIdChecker ());


#ifdef NS3_LOG_ENABLE

//
// Note:  Calls that take TimePrinter as a parameter are defined as nothing
// in the logging module if NS3_LOG_ENABLE is not defined.
// 

static void
TimePrinter (std::ostream &os)
{
  os << Simulator::Now ().GetSeconds () << "s";
}

static void
NodePrinter (std::ostream &os)
{
  if (Simulator::GetContext () == 0xffffffff)
    {
      os << "-1";
    }
  else
    {
      os << Simulator::GetContext ();
    }
}

#endif /* NS3_LOG_ENABLE */

static SimulatorImpl **PeekImpl (void)
{
  static SimulatorImpl *impl = 0;
  return &impl;
}

static SimulatorImpl * GetImpl (void)
{
  SimulatorImpl **pimpl = PeekImpl ();
  /* Please, don't include any calls to logging macros in this function
   * or pay the price, that is, stack explosions.
   */
  if (*pimpl == 0)
    {
      {
        ObjectFactory factory;
        StringValue s;
        
        g_simTypeImpl.GetValue (s);
        factory.SetTypeId (s.Get ());
        *pimpl = GetPointer (factory.Create<SimulatorImpl> ());
      }
      {
        ObjectFactory factory;
        StringValue s;
        g_schedTypeImpl.GetValue (s);
        factory.SetTypeId (s.Get ());
        (*pimpl)->SetScheduler (factory);
      }

//
// Note: we call LogSetTimePrinter _after_ creating the implementation
// object because the act of creation can trigger calls to the logging 
// framework which would call the TimePrinter function which would call 
// Simulator::Now which would call Simulator::GetImpl, and, thus, get us 
// in an infinite recursion until the stack explodes.
//
      LogSetTimePrinter (&TimePrinter);
      LogSetNodePrinter (&NodePrinter);
    }
  return *pimpl;
}

void
Simulator::Destroy (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  SimulatorImpl **pimpl = PeekImpl (); 
  if (*pimpl == 0)
    {
      return;
    }
  /* Note: we have to call LogSetTimePrinter (0) below because if we do not do
   * this, and restart a simulation after this call to Destroy, (which is 
   * legal), Simulator::GetImpl will trigger again an infinite recursion until
   * the stack explodes.
   */
  LogSetTimePrinter (0);
  LogSetNodePrinter (0);
  (*pimpl)->Destroy ();
  (*pimpl)->Unref ();
  *pimpl = 0;
}

void
Simulator::SetScheduler (ObjectFactory schedulerFactory)
{
  NS_LOG_FUNCTION (schedulerFactory);
  GetImpl ()->SetScheduler (schedulerFactory);
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
Simulator::RunOneEvent (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  GetImpl ()->RunOneEvent ();
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
  /* Please, don't include any calls to logging macros in this function
   * or pay the price, that is, stack explosions.
   */
  return GetImpl ()->Now ();
}

Time
Simulator::GetDelayLeft (const EventId &id)
{
  NS_LOG_FUNCTION (&id);
  return GetImpl ()->GetDelayLeft (id);
}

EventId
Simulator::Schedule (Time const &time, const Ptr<EventImpl> &ev)
{
  NS_LOG_FUNCTION (time << ev);
  return DoSchedule (time, GetPointer (ev));
}

EventId
Simulator::ScheduleNow (const Ptr<EventImpl> &ev)
{
  NS_LOG_FUNCTION (ev);
  return DoScheduleNow (GetPointer (ev));
}
void
Simulator::ScheduleWithContext (uint32_t context, const Time &time, EventImpl *impl)
{
  return GetImpl ()->ScheduleWithContext (context, time, impl);
}
EventId
Simulator::ScheduleDestroy (const Ptr<EventImpl> &ev)
{
  NS_LOG_FUNCTION (ev);
  return DoScheduleDestroy (GetPointer (ev));
}
EventId 
Simulator::DoSchedule (Time const &time, EventImpl *impl)
{
  return GetImpl ()->Schedule (time, impl);
}
EventId 
Simulator::DoScheduleNow (EventImpl *impl)
{
  return GetImpl ()->ScheduleNow (impl);
}
EventId 
Simulator::DoScheduleDestroy (EventImpl *impl)
{
  return GetImpl ()->ScheduleDestroy (impl);
}


EventId
Simulator::Schedule (Time const &time, void (*f) (void))
{
  NS_LOG_FUNCTION (time << f);
  return DoSchedule (time, MakeEvent (f));
}

void
Simulator::ScheduleWithContext (uint32_t context, Time const &time, void (*f) (void))
{
  NS_LOG_FUNCTION (time << context << f);
  return ScheduleWithContext (context, time, MakeEvent (f));
}

EventId
Simulator::ScheduleNow (void (*f) (void))
{
  NS_LOG_FUNCTION (f);
  return DoScheduleNow (MakeEvent (f));
}

EventId
Simulator::ScheduleDestroy (void (*f) (void))
{
  NS_LOG_FUNCTION (f);
  return DoScheduleDestroy (MakeEvent (f));
}

void
Simulator::Remove (const EventId &ev)
{
  NS_LOG_FUNCTION (&ev);
  if (*PeekImpl () == 0)
    {
      return;
    }
  return GetImpl ()->Remove (ev);
}

void
Simulator::Cancel (const EventId &ev)
{
  NS_LOG_FUNCTION (&ev);
  if (*PeekImpl () == 0)
    {
      return;
    }
  return GetImpl ()->Cancel (ev);
}

bool 
Simulator::IsExpired (const EventId &id)
{
  NS_LOG_FUNCTION (&id);
  if (*PeekImpl () == 0)
    {
      return true;
    }
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

uint32_t
Simulator::GetContext (void)
{
  return GetImpl ()->GetContext ();
}

uint32_t
Simulator::GetSystemId (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (*PeekImpl () != 0)
    {
      return GetImpl ()->GetSystemId ();
    }
  else
    {
      return 0;
    }
}

void
Simulator::SetImplementation (Ptr<SimulatorImpl> impl)
{
  if (*PeekImpl () != 0)
    {
      NS_FATAL_ERROR ("It is not possible to set the implementation after calling any Simulator:: function. Call Simulator::SetImplementation earlier or after Simulator::Destroy.");
    }
  *PeekImpl () = GetPointer (impl);
  // Set the default scheduler
  ObjectFactory factory;
  StringValue s;
  g_schedTypeImpl.GetValue (s);
  factory.SetTypeId (s.Get ());
  impl->SetScheduler (factory);
//
// Note: we call LogSetTimePrinter _after_ creating the implementation
// object because the act of creation can trigger calls to the logging 
// framework which would call the TimePrinter function which would call 
// Simulator::Now which would call Simulator::GetImpl, and, thus, get us 
// in an infinite recursion until the stack explodes.
//
  LogSetTimePrinter (&TimePrinter);
  LogSetNodePrinter (&NodePrinter);
}
Ptr<SimulatorImpl>
Simulator::GetImplementation (void)
{
  return GetImpl ();
}



} // namespace ns3

#include "ns3/test.h"
#include "list-scheduler.h"
#include "heap-scheduler.h"
#include "map-scheduler.h"
#include "calendar-scheduler.h"
#include "ns2-calendar-scheduler.h"

namespace ns3 {

class SimulatorEventsTestCase : public TestCase
{
public:
  SimulatorEventsTestCase (ObjectFactory schedulerFactory);
  virtual bool DoRun (void);
  void A (int a);
  void B (int b);
  void C (int c);
  void D (int d);
  void foo0 (void);
  uint64_t NowUs (void);
  void destroy(void);
  bool m_b;
  bool m_a;
  bool m_c;
  bool m_d;
  EventId m_idC;
  bool m_destroy;
  EventId m_destroyId;
  ObjectFactory m_schedulerFactory;
};

SimulatorEventsTestCase::SimulatorEventsTestCase (ObjectFactory schedulerFactory)
  : TestCase ("Check that basic event handling is working with " + 
              schedulerFactory.GetTypeId ().GetName ()),
    m_schedulerFactory (schedulerFactory)
{}
uint64_t
SimulatorEventsTestCase::NowUs (void)
{
  uint64_t ns = Now ().GetNanoSeconds ();
  return ns / 1000;
}  

void
SimulatorEventsTestCase::A (int a)
{
  m_a = false;
}

void
SimulatorEventsTestCase::B (int b)
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
  Simulator::Schedule (MicroSeconds (10), &SimulatorEventsTestCase::D, this, 4);
}

void
SimulatorEventsTestCase::C (int c)
{
  m_c = false;
}

void
SimulatorEventsTestCase::D (int d)
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
SimulatorEventsTestCase::foo0(void)
{}

void
SimulatorEventsTestCase::destroy (void)
{
  if (m_destroyId.IsExpired ())
    {
      m_destroy = true; 
    }
}
bool 
SimulatorEventsTestCase::DoRun (void)
{
  m_a = true;
  m_b = false;
  m_c = true;
  m_d = false;

  Simulator::SetScheduler (m_schedulerFactory);

  EventId a = Simulator::Schedule (MicroSeconds (10), &SimulatorEventsTestCase::A, this, 1);
  Simulator::Schedule (MicroSeconds (11), &SimulatorEventsTestCase::B, this, 2);
  m_idC = Simulator::Schedule (MicroSeconds (12), &SimulatorEventsTestCase::C, this, 3);

  NS_TEST_EXPECT_MSG_EQ (!m_idC.IsExpired (), true, "");
  NS_TEST_EXPECT_MSG_EQ (!a.IsExpired (), true, "");
  Simulator::Cancel (a);
  NS_TEST_EXPECT_MSG_EQ (a.IsExpired (), true, "");
  Simulator::Run ();
  NS_TEST_EXPECT_MSG_EQ (m_a, true, "Event A did not run ?");
  NS_TEST_EXPECT_MSG_EQ (m_b, true, "Event B did not run ?");
  NS_TEST_EXPECT_MSG_EQ (m_c, true, "Event C did not run ?");
  NS_TEST_EXPECT_MSG_EQ (m_d, true, "Event D did not run ?");

  EventId anId = Simulator::ScheduleNow (&SimulatorEventsTestCase::foo0, this);
  EventId anotherId = anId;
  NS_TEST_EXPECT_MSG_EQ (!(anId.IsExpired () || anotherId.IsExpired ()), true, "Event should not have expired yet.");

  Simulator::Remove (anId);
  NS_TEST_EXPECT_MSG_EQ (anId.IsExpired (), true, "Event was removed: it is now expired");
  NS_TEST_EXPECT_MSG_EQ (anotherId.IsExpired (), true, "Event was removed: it is now expired");

  m_destroy = false;
  m_destroyId = Simulator::ScheduleDestroy (&SimulatorEventsTestCase::destroy, this);
  NS_TEST_EXPECT_MSG_EQ (!m_destroyId.IsExpired (), true, "Event should not have expired yet");
  m_destroyId.Cancel ();
  NS_TEST_EXPECT_MSG_EQ (m_destroyId.IsExpired (), true, "Event was canceled: should have expired now");

  m_destroyId = Simulator::ScheduleDestroy (&SimulatorEventsTestCase::destroy, this);
  NS_TEST_EXPECT_MSG_EQ (!m_destroyId.IsExpired (), true, "Event should not have expired yet");
  Simulator::Remove (m_destroyId);
  NS_TEST_EXPECT_MSG_EQ (m_destroyId.IsExpired (), true, "Event was canceled: should have expired now");

  m_destroyId = Simulator::ScheduleDestroy (&SimulatorEventsTestCase::destroy, this);
  NS_TEST_EXPECT_MSG_EQ (!m_destroyId.IsExpired (), true, "Event should not have expired yet");

  Simulator::Run ();
  NS_TEST_EXPECT_MSG_EQ (!m_destroyId.IsExpired (), true, "Event should not have expired yet");
  NS_TEST_EXPECT_MSG_EQ (!m_destroy, true, "Event should not have run");

  Simulator::Destroy ();
  NS_TEST_EXPECT_MSG_EQ (m_destroyId.IsExpired (), true, "Event should have expired now");
  NS_TEST_EXPECT_MSG_EQ (m_destroy, true, "Event should have run");

  return false;
}

class SimulatorTemplateTestCase : public TestCase
{
public:
  SimulatorTemplateTestCase ();
  // only here for testing of Ptr<>
  void Ref (void) const {}
  void Unref (void) const {}
private:
  virtual bool DoRun (void);

  void bar0 (void) {}
  void bar1 (int) {}
  void bar2 (int, int) {}
  void bar3 (int, int, int) {}
  void bar4 (int, int, int, int) {}
  void bar5 (int, int, int, int, int) {}
  void baz1 (int &) {}
  void baz2 (int &, int &) {}
  void baz3 (int &, int &, int &) {}
  void baz4 (int &, int &, int &, int &) {}
  void baz5 (int &, int &, int &, int &, int &) {}
  void cbaz1 (const int &) {}
  void cbaz2 (const int &, const int &) {}
  void cbaz3 (const int &, const int &, const int &) {}
  void cbaz4 (const int &, const int &, const int &, const int &) {}
  void cbaz5 (const int &, const int &, const int &, const int &, const int &) {}

  void bar0c (void) const {}
  void bar1c (int) const {}
  void bar2c (int, int) const {}
  void bar3c (int, int, int) const {}
  void bar4c (int, int, int, int) const {}
  void bar5c (int, int, int, int, int) const {}
  void baz1c (int &) const {}
  void baz2c (int &, int &) const {}
  void baz3c (int &, int &, int &) const {}
  void baz4c (int &, int &, int &, int &) const {}
  void baz5c (int &, int &, int &, int &, int &) const {}
  void cbaz1c (const int &) const {}
  void cbaz2c (const int &, const int &) const {}
  void cbaz3c (const int &, const int &, const int &) const {}
  void cbaz4c (const int &, const int &, const int &, const int &) const {}
  void cbaz5c (const int &, const int &, const int &, const int &, const int &) const {}

};

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

SimulatorTemplateTestCase::SimulatorTemplateTestCase ()
  : TestCase ("Check that all templates are instanciated correctly. This is a compilation test, it cannot fail at runtime.")
{}
bool 
SimulatorTemplateTestCase::DoRun (void)
{
  // Test schedule of const methods
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar0c, this);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar1c, this, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar2c, this, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar3c, this, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar4c, this, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar5c, this, 0, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::cbaz1c, this, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::cbaz2c, this, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::cbaz3c, this, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::cbaz4c, this, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::cbaz5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar0c, this);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar1c, this, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar2c, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar3c, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar4c, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::cbaz1c, this, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::cbaz2c, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::cbaz3c, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::cbaz4c, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::cbaz5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar0c, this);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar1c, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar2c, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar3c, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar4c, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::cbaz1c, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::cbaz2c, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::cbaz3c, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::cbaz4c, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::cbaz5c, this, 0, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::baz1c, this, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::baz2c, this, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::baz3c, this, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::baz4c, this, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::baz5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::baz1c, this, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::baz2c, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::baz3c, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::baz4c, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::baz5c, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::baz1c, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::baz2c, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::baz3c, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::baz4c, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::baz5c, this, 0, 0, 0, 0, 0);

  // Test of schedule const methods with Ptr<> pointers
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar0c, Ptr<const SimulatorTemplateTestCase> (this));
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar1c, Ptr<const SimulatorTemplateTestCase> (this), 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar2c, Ptr<const SimulatorTemplateTestCase> (this), 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar3c, Ptr<const SimulatorTemplateTestCase> (this), 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar4c, Ptr<const SimulatorTemplateTestCase> (this), 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar5c, Ptr<const SimulatorTemplateTestCase> (this), 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar0c, Ptr<const SimulatorTemplateTestCase> (this));
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar1c, Ptr<const SimulatorTemplateTestCase> (this), 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar2c, Ptr<const SimulatorTemplateTestCase> (this), 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar3c, Ptr<const SimulatorTemplateTestCase> (this), 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar4c, Ptr<const SimulatorTemplateTestCase> (this), 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar5c, Ptr<const SimulatorTemplateTestCase> (this), 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar0c, Ptr<const SimulatorTemplateTestCase> (this));
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar1c, Ptr<const SimulatorTemplateTestCase> (this), 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar2c, Ptr<const SimulatorTemplateTestCase> (this), 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar3c, Ptr<const SimulatorTemplateTestCase> (this), 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar4c, Ptr<const SimulatorTemplateTestCase> (this), 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar5c, Ptr<const SimulatorTemplateTestCase> (this), 0, 0, 0, 0, 0);


  // Test schedule of raw functions
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


  // Test schedule of normal member methods
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar0, this);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar1, this, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar2, this, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar3, this, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar4, this, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar5, this, 0, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::cbaz1, this, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::cbaz2, this, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::cbaz3, this, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::cbaz4, this, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::cbaz5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar0, this);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar1, this, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar2, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar3, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar4, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::cbaz1, this, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::cbaz2, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::cbaz3, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::cbaz4, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::cbaz5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar0, this);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar1, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar2, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar3, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar4, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::cbaz1, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::cbaz2, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::cbaz3, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::cbaz4, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::cbaz5, this, 0, 0, 0, 0, 0);


  // test schedule of normal methods with Ptr<> pointers
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar0, Ptr<SimulatorTemplateTestCase> (this));
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar1, Ptr<SimulatorTemplateTestCase> (this), 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar2, Ptr<SimulatorTemplateTestCase> (this), 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar3, Ptr<SimulatorTemplateTestCase> (this), 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar4, Ptr<SimulatorTemplateTestCase> (this), 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::bar5, Ptr<SimulatorTemplateTestCase> (this), 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar0, Ptr<SimulatorTemplateTestCase> (this));
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar1, Ptr<SimulatorTemplateTestCase> (this), 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar2, Ptr<SimulatorTemplateTestCase> (this), 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar3, Ptr<SimulatorTemplateTestCase> (this), 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar4, Ptr<SimulatorTemplateTestCase> (this), 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::bar5, Ptr<SimulatorTemplateTestCase> (this), 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar0, Ptr<SimulatorTemplateTestCase> (this));
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar1, Ptr<SimulatorTemplateTestCase> (this), 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar2, Ptr<SimulatorTemplateTestCase> (this), 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar3, Ptr<SimulatorTemplateTestCase> (this), 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar4, Ptr<SimulatorTemplateTestCase> (this), 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::bar5, Ptr<SimulatorTemplateTestCase> (this), 0, 0, 0, 0, 0);


  // the code below does not compile, as expected.
  //Simulator::Schedule (Seconds (0.0), &cber1, 0.0);


  // This code appears to be duplicate test code.
  Simulator::Schedule (Seconds (0.0), &ber1, 0);
  Simulator::Schedule (Seconds (0.0), &ber2, 0, 0);
  Simulator::Schedule (Seconds (0.0), &ber3, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &ber4, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &ber5, 0, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::baz1, this, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::baz2, this, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::baz3, this, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::baz4, this, 0, 0, 0, 0);
  Simulator::Schedule (Seconds (0.0), &SimulatorTemplateTestCase::baz5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&ber1, 0);
  Simulator::ScheduleNow (&ber2, 0, 0);
  Simulator::ScheduleNow (&ber3, 0, 0, 0);
  Simulator::ScheduleNow (&ber4, 0, 0, 0, 0);
  Simulator::ScheduleNow (&ber5, 0, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::baz1, this, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::baz2, this, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::baz3, this, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::baz4, this, 0, 0, 0, 0);
  Simulator::ScheduleNow (&SimulatorTemplateTestCase::baz5, this, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&ber1, 0);
  Simulator::ScheduleDestroy (&ber2, 0, 0);
  Simulator::ScheduleDestroy (&ber3, 0, 0, 0);
  Simulator::ScheduleDestroy (&ber4, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&ber5, 0, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::baz1, this, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::baz2, this, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::baz3, this, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::baz4, this, 0, 0, 0, 0);
  Simulator::ScheduleDestroy (&SimulatorTemplateTestCase::baz5, this, 0, 0, 0, 0, 0);


  Simulator::Run ();
  Simulator::Destroy ();

  return false;
}

class SimulatorTestSuite : public TestSuite
{
public:
  SimulatorTestSuite ()
    : TestSuite ("simulator")
  {
    ObjectFactory factory;
    factory.SetTypeId (ListScheduler::GetTypeId ());

    AddTestCase (new SimulatorEventsTestCase (factory));
    factory.SetTypeId (MapScheduler::GetTypeId ());
    AddTestCase (new SimulatorEventsTestCase (factory));
    factory.SetTypeId (HeapScheduler::GetTypeId ());
    AddTestCase (new SimulatorEventsTestCase (factory));
    factory.SetTypeId (CalendarScheduler::GetTypeId ());
    AddTestCase (new SimulatorEventsTestCase (factory));
    factory.SetTypeId (Ns2CalendarScheduler::GetTypeId ());
    AddTestCase (new SimulatorEventsTestCase (factory));
  }
} g_simulatorTestSuite;

} // namespace ns3
