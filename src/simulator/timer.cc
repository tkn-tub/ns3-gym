#include "timer.h"
#include "simulator.h"
#include "simulation-singleton.h"
#include "event-garbage-collector.h"

namespace ns3 {

Timer::Timer ()
  : m_flags (0),
    m_delay (FemtoSeconds (0)),
    m_event (),
    m_impl (0)
{}

Timer::Timer (int flags)
  : m_flags (flags),
    m_delay (FemtoSeconds (0)),
    m_event (),
    m_impl (0)
{
  if (flags & GARBAGE_COLLECT)
    {
      if (flags != GARBAGE_COLLECT)
	{
	  NS_FATAL_ERROR ("You cannot specify another Timer flag with the GARBAGE_COLLECT flag.");
	}
    }
  else
    {
      int onSchedule = 0;
      if (m_flags & CHECK_ON_SCHEDULE)
	{
	  onSchedule++;
	}
      if (m_flags & CANCEL_ON_SCHEDULE)
	{
	  onSchedule++;
	}
      if (m_flags & REMOVE_ON_SCHEDULE)
	{
	  onSchedule++;
	}
      int onDestroy = 0;
      if (m_flags & CHECK_ON_DESTROY)
	{
	  onDestroy++;
	}
      if (m_flags & CANCEL_ON_DESTROY)
	{
	  onDestroy++;
	}
      if (m_flags & REMOVE_ON_DESTROY)
	{
	  onDestroy++;
	}
      if (onSchedule > 1)
	{
	  NS_FATAL_ERROR ("You cannot specify more than one ON_SCHEDULE flag on a Timer.");
	}
      if (onDestroy > 1)
	{
	  NS_FATAL_ERROR ("You cannot specify more than one ON_DESTROY flag on a Timer.");
	}
    }
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
Timer::SetFunction (void (*fn) (void))
{
  struct FnTimerImplZero : public TimerImpl
  {
    typedef void (*FN) (void);
    FnTimerImplZero (FN fn) 
      : m_fn (fn) {}
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_fn);
    }
    FN m_fn;
  } *function = new FnTimerImplZero (fn);
  delete m_impl;
  m_impl = function;
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
  return m_event.IsExpired ();
}
bool 
Timer::IsRunning (void) const
{
  return m_event.IsRunning ();
}


void 
Timer::Schedule (void)
{
  NS_ASSERT (m_impl != 0);
  if (m_flags & CHECK_ON_SCHEDULE)
    {
      if (m_event.IsRunning ())
	{
	  NS_FATAL_ERROR ("Event is still running while re-scheduling.");
	}
    }
  else if (m_flags & CANCEL_ON_SCHEDULE)
    {
      m_event.Cancel ();
    }
  else if (m_flags & REMOVE_ON_SCHEDULE)
    {
      Simulator::Remove (m_event);
    }
  m_event = m_impl->Schedule (m_delay);
  if (m_flags & GARBAGE_COLLECT)
    {
      SimulationSingleton<EventGarbageCollector>::Get ()->Track (m_event);
    }
}


} // namespace ns3


#ifdef RUN_SELF_TESTS
#include "ns3/test.h"

namespace {
void bari (int)
{}
void barcir (const int &)
{}
void barir (int &)
{}
void barip (int *)
{}
void barcip (const int *)
{}
}

namespace ns3 {

class TimerTests : public Test
{
public:
  TimerTests ();
  virtual bool RunTests (void);
  void bazi (int) {}
  void bazcir (const int&) {}
  void bazir (int&) {}
  void bazip (int *) {}
  void bazcip (const int *) {}
};

TimerTests::TimerTests ()
  : Test ("Timer")
{}

bool
TimerTests::RunTests (void)
{
  bool ok = true;

  int a = 0;
  int &b = a;
  const int &c = a;
  Timer timer = Timer (0);

  timer.SetFunction (&bari, a);
  timer.SetArguments (2);
  timer.SetArguments (a);
  timer.SetArguments (b);
  timer.SetArguments (c);
  timer.SetFunction (&barir, a);
  timer.SetArguments (2);
  timer.SetArguments (a);
  timer.SetArguments (b);
  timer.SetArguments (c);
  timer.SetFunction (&barcir, a);
  timer.SetArguments (2);
  timer.SetArguments (a);
  timer.SetArguments (b);
  timer.SetArguments (c);
  // the following call cannot possibly work and is flagged by
  // a runtime error.
  //timer.SetArguments (0.0);
  timer.SetDelay (Seconds (1.0));
  timer.Schedule ();

  timer.SetFunction (&TimerTests::bazi, this, 1);
  timer.SetFunction (&TimerTests::bazir, this, 1);
  timer.SetFunction (&TimerTests::bazcir, this, 1);

  Simulator::Run ();
  Simulator::Destroy ();
  return ok;
}

TimerTests g_tests;

} // namespace ns3

#endif /* RUN_SELF_TESTS */
