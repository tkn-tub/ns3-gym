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

Timer::Timer (enum SchedulePolicy schedulePolicy, 
	      enum DestroyPolicy destroyPolicy)
  : m_flags (schedulePolicy | destroyPolicy),
    m_delay (FemtoSeconds (0)),
    m_event (),
    m_impl (0)
{}

Timer::Timer (enum GarbageCollectPolicy policy)
  : m_flags (GARBAGE_COLLECT),
    m_delay (FemtoSeconds (0)),
    m_event (),
    m_impl (0)
{}

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
  bool result = true;

  int a = 0;
  int &b = a;
  const int &c = a;
  Timer timer;

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
  //timer.SetArguments (0.0);
  timer.SetDelay (Seconds (1.0));
  timer.Schedule ();

  timer.SetFunction (&TimerTests::bazi, this);
  timer.SetArguments (3);
  timer.SetFunction (&TimerTests::bazir, this);
  timer.SetArguments (3);
  timer.SetFunction (&TimerTests::bazcir, this);
  timer.SetArguments (3);

  Simulator::Run ();
  Simulator::Destroy ();
  return result;
}

TimerTests g_tests;

} // namespace ns3

#endif /* RUN_SELF_TESTS */
