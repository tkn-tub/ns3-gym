/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "timer.h"
#include "simulator.h"
#include "simulation-singleton.h"

namespace ns3 {

Timer::Timer ()
  : m_flags (CHECK_ON_DESTROY),
    m_delay (FemtoSeconds (0)),
    m_event (),
    m_impl (0)
{
}

Timer::Timer (enum DestroyPolicy destroyPolicy)
  : m_flags (destroyPolicy),
    m_delay (FemtoSeconds (0)),
    m_event (),
    m_impl (0)
{
}

Timer::~Timer ()
{
  if (m_flags & CHECK_ON_DESTROY)
    {
      if (m_event.IsRunning ())
        {
          NS_FATAL_ERROR ("Event is still running while destroying.");
        }
    }
  else if (m_flags & CANCEL_ON_DESTROY)
    {
      m_event.Cancel ();
    }
  else if (m_flags & REMOVE_ON_DESTROY)
    {
      Simulator::Remove (m_event);
    }
  delete m_impl;
}

void
Timer::SetDelay (const Time &time)
{
  m_delay = time;
}
Time
Timer::GetDelay (void) const
{
  return m_delay;
}
Time
Timer::GetDelayLeft (void) const
{
  switch (GetState ())
    {
    case Timer::RUNNING:
      return Simulator::GetDelayLeft (m_event);
      break;
    case Timer::EXPIRED:
      return TimeStep (0);
      break;
    case Timer::SUSPENDED:
      return m_delayLeft;
      break;
    default:
      NS_ASSERT (false);
      return TimeStep (0);
      break;
    }
}

void
Timer::Cancel (void)
{
  Simulator::Cancel (m_event);
}
void
Timer::Remove (void)
{
  Simulator::Remove (m_event);
}
bool
Timer::IsExpired (void) const
{
  return !IsSuspended () && m_event.IsExpired ();
}
bool
Timer::IsRunning (void) const
{
  return !IsSuspended () && m_event.IsRunning ();
}
bool
Timer::IsSuspended (void) const
{
  return (m_flags & TIMER_SUSPENDED) == TIMER_SUSPENDED;
}
enum Timer::State
Timer::GetState (void) const
{
  if (IsRunning ())
    {
      return Timer::RUNNING;
    }
  else if (IsExpired ())
    {
      return Timer::EXPIRED;
    }
  else
    {
      NS_ASSERT (IsSuspended ());
      return Timer::SUSPENDED;
    }
}

void
Timer::Schedule (void)
{
  Schedule (m_delay);
}

void
Timer::Schedule (Time delay)
{
  NS_ASSERT (m_impl != 0);
  if (m_event.IsRunning ())
    {
      NS_FATAL_ERROR ("Event is still running while re-scheduling.");
    }
  m_event = m_impl->Schedule (delay);
}

void
Timer::Suspend (void)
{
  NS_ASSERT (IsRunning ());
  m_delayLeft = Simulator::GetDelayLeft (m_event);
  Simulator::Remove (m_event);
  m_flags |= TIMER_SUSPENDED;
}

void
Timer::Resume (void)
{
  NS_ASSERT (m_flags & TIMER_SUSPENDED);
  m_event = m_impl->Schedule (m_delayLeft);
  m_flags &= ~TIMER_SUSPENDED;
}


} // namespace ns3

#include "ns3/test.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"

namespace {
void bari (int)
{
}
void bar2i (int, int)
{
}
void bar3i (int, int, int)
{
}
void bar4i (int, int, int, int)
{
}
void bar5i (int, int, int, int, int)
{
}
void bar6i (int, int, int, int, int, int)
{
}
void barcir (const int &)
{
}
void barir (int &)
{
}
void barip (int *)
{
}
void barcip (const int *)
{
}
} // anonymous namespace

namespace ns3 {

class TimerStateTestCase : public TestCase
{
public:
  TimerStateTestCase ();
  virtual bool DoRun (void);
};

TimerStateTestCase::TimerStateTestCase ()
  : TestCase ("Check correct state transitions")
{
}
bool
TimerStateTestCase::DoRun (void)
{
  Timer timer = Timer (Timer::CANCEL_ON_DESTROY);

  timer.SetFunction (&bari);
  timer.SetArguments (1);
  timer.SetDelay (Seconds (10.0));
  NS_TEST_ASSERT_MSG_EQ (!timer.IsRunning (), true, "");
  NS_TEST_ASSERT_MSG_EQ (timer.IsExpired (), true, "");
  NS_TEST_ASSERT_MSG_EQ (!timer.IsSuspended (), true, "");
  NS_TEST_ASSERT_MSG_EQ (timer.GetState (), Timer::EXPIRED, "");
  timer.Schedule ();
  NS_TEST_ASSERT_MSG_EQ (timer.IsRunning (), true, "");
  NS_TEST_ASSERT_MSG_EQ (!timer.IsExpired (), true, "");
  NS_TEST_ASSERT_MSG_EQ (!timer.IsSuspended (), true, "");
  NS_TEST_ASSERT_MSG_EQ (timer.GetState (), Timer::RUNNING, "");
  timer.Suspend ();
  NS_TEST_ASSERT_MSG_EQ (!timer.IsRunning (), true, "");
  NS_TEST_ASSERT_MSG_EQ (!timer.IsExpired (), true, "");
  NS_TEST_ASSERT_MSG_EQ (timer.IsSuspended (), true, "");
  NS_TEST_ASSERT_MSG_EQ (timer.GetState (), Timer::SUSPENDED, "");
  timer.Resume ();
  NS_TEST_ASSERT_MSG_EQ (timer.IsRunning (), true, "");
  NS_TEST_ASSERT_MSG_EQ (!timer.IsExpired (), true, "");
  NS_TEST_ASSERT_MSG_EQ (!timer.IsSuspended (), true, "");
  NS_TEST_ASSERT_MSG_EQ (timer.GetState (), Timer::RUNNING, "");
  timer.Cancel ();
  NS_TEST_ASSERT_MSG_EQ (!timer.IsRunning (), true, "");
  NS_TEST_ASSERT_MSG_EQ (timer.IsExpired (), true, "");
  NS_TEST_ASSERT_MSG_EQ (!timer.IsSuspended (), true, "");
  NS_TEST_ASSERT_MSG_EQ (timer.GetState (), Timer::EXPIRED, "");
  return false;
}

class TimerTemplateTestCase : public TestCase
{
public:
  TimerTemplateTestCase ();
  virtual bool DoRun (void);
  virtual void DoTeardown (void);
  void bazi (int)
  {
  }
  void baz2i (int, int)
  {
  }
  void baz3i (int, int, int)
  {
  }
  void baz4i (int, int, int, int)
  {
  }
  void baz5i (int, int, int, int, int)
  {
  }
  void baz6i (int, int, int, int, int, int)
  {
  }
  void bazcir (const int&)
  {
  }
  void bazir (int&)
  {
  }
  void bazip (int *)
  {
  }
  void bazcip (const int *)
  {
  }
};

TimerTemplateTestCase::TimerTemplateTestCase ()
  : TestCase ("Check that template magic is working")
{
}

bool
TimerTemplateTestCase::DoRun (void)
{
  Timer timer = Timer (Timer::CANCEL_ON_DESTROY);

  int a = 0;
  int &b = a;
  const int &c = a;

  timer.SetFunction (&bari);
  timer.SetArguments (2);
  timer.SetArguments (a);
  timer.SetArguments (b);
  timer.SetArguments (c);
  timer.SetFunction (&barir);
  timer.SetArguments (2);
  timer.SetArguments (a);
  timer.SetArguments (b);
  timer.SetArguments (c);
  timer.SetFunction (&barcir);
  timer.SetArguments (2);
  timer.SetArguments (a);
  timer.SetArguments (b);
  timer.SetArguments (c);
  // the following call cannot possibly work and is flagged by
  // a runtime error.
  // timer.SetArguments (0.0);
  timer.SetDelay (Seconds (1.0));
  timer.Schedule ();

  timer.SetFunction (&TimerTemplateTestCase::bazi, this);
  timer.SetArguments (3);
  timer.SetFunction (&TimerTemplateTestCase::bazir, this);
  timer.SetArguments (3);
  timer.SetFunction (&TimerTemplateTestCase::bazcir, this);
  timer.SetArguments (3);

  timer.SetFunction (&bar2i);
  timer.SetArguments (1, 1);
  timer.SetFunction (&bar3i);
  timer.SetArguments (1, 1, 1);
  timer.SetFunction (&bar4i);
  timer.SetArguments (1, 1, 1, 1);
  timer.SetFunction (&bar5i);
  timer.SetArguments (1, 1, 1, 1, 1);
  // unsupported in simulator class
  // timer.SetFunction (&bar6i);
  // timer.SetArguments (1, 1, 1, 1, 1, 1);

  timer.SetFunction (&TimerTemplateTestCase::baz2i, this);
  timer.SetArguments (1, 1);
  timer.SetFunction (&TimerTemplateTestCase::baz3i, this);
  timer.SetArguments (1, 1, 1);
  timer.SetFunction (&TimerTemplateTestCase::baz4i, this);
  timer.SetArguments (1, 1, 1, 1);
  timer.SetFunction (&TimerTemplateTestCase::baz5i, this);
  timer.SetArguments (1, 1, 1, 1, 1);
  // unsupported in simulator class
  // timer.SetFunction (&TimerTemplateTestCase::baz6i, this);
  // timer.SetArguments (1, 1, 1, 1, 1, 1);

  Simulator::Run ();
  Simulator::Destroy ();

  return false;
}

void
TimerTemplateTestCase::DoTeardown (void)
{
  Simulator::Run ();
  Simulator::Destroy ();
}

static class TimerTestSuite : public TestSuite
{
public:
  TimerTestSuite ()
    : TestSuite ("timer", UNIT)
  {
    AddTestCase (new TimerStateTestCase ());
    AddTestCase (new TimerTemplateTestCase ());
  }
} g_timerTestSuite;

} // namespace ns3


