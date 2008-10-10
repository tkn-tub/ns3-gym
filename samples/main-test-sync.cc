/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/simulator.h"
#include "ns3/realtime-simulator.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/system-thread.h"
#include "ns3/string.h"
#include "ns3/config.h"
#include "ns3/global-value.h"

#include <unistd.h>
#include <sys/time.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TestSync");

bool gFirstRun = false;

  void 
inserted_function (void)
{
  NS_ASSERT (gFirstRun);
  NS_LOG_UNCOND ("inserted_function() called at " << 
    Simulator::Now ().GetSeconds () << " s");
}

  void 
background_function (void)
{
  NS_ASSERT (gFirstRun);
  NS_LOG_UNCOND ("background_function() called at " << 
    Simulator::Now ().GetSeconds () << " s");
}

  void 
first_function (void)
{
  NS_LOG_UNCOND ("first_function() called at " << 
    Simulator::Now ().GetSeconds () << " s");
  gFirstRun = true;
}

class FakeNetDevice
{
public:
  FakeNetDevice ();
  void Doit1 (void);
  void Doit2 (void);
  void Doit3 (void);
  void Doit4 (void);
};

FakeNetDevice::FakeNetDevice ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

  void
FakeNetDevice::Doit1 (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  sleep (1);
  for (uint32_t i = 0.001; i < 10000; ++i)
    {
      // 
      // Exercise the relative now path
      //
      Simulator::ScheduleNow (&inserted_function);
      usleep (1000);
    }
}

  void
FakeNetDevice::Doit2 (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  sleep (1);
  for (uint32_t i = 0.001; i < 10000; ++i)
    {
      //
      // Exercise the relative schedule path
      //
      Simulator::Schedule (Seconds (0), &inserted_function);
      usleep (1000);
    }
}

  void
FakeNetDevice::Doit3 (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  sleep (1);
  for (uint32_t i = 0.001; i < 10000; ++i)
    {
      //
      // Exercise the realtime relative now path
      //
      RealtimeSimulatorExtension::ScheduleRealNow (&inserted_function);
      usleep (1000);
    }
}

  void
FakeNetDevice::Doit4 (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  sleep (1);
  for (uint32_t i = 0.001; i < 10000; ++i)
    {
      //
      // Exercise the realtime relative schedule path
      //
      RealtimeSimulatorExtension::ScheduleReal (Seconds (0), &inserted_function);
      usleep (1000);
    }
}

  void 
test (void)
{
  GlobalValue::Bind ("SimulatorImplementationType", 
    StringValue ("ns3::RealtimeSimulatorImpl"));

  FakeNetDevice fnd;

  // 
  // Make sure ScheduleNow works when the system isn't running
  //
  Simulator::ScheduleNow(&first_function);

  // 
  // drive the progression of m_currentTs at a ten millisecond rate
  //
  for (double d = 0.; d < 14.999; d += 0.01)
    {
      Simulator::Schedule (Seconds (d), &background_function);
    }

  Ptr<SystemThread> st1 = Create<SystemThread> (
    MakeCallback (&FakeNetDevice::Doit1, &fnd));
  st1->Start ();

  Ptr<SystemThread> st2 = Create<SystemThread> (
    MakeCallback (&FakeNetDevice::Doit2, &fnd));
  st2->Start ();

  Ptr<SystemThread> st3 = Create<SystemThread> (
    MakeCallback (&FakeNetDevice::Doit3, &fnd));
  st3->Start ();

  Ptr<SystemThread> st4 = Create<SystemThread> (
    MakeCallback (&FakeNetDevice::Doit4, &fnd));
  st4->Start ();

  Simulator::Stop (Seconds (15.0));
  Simulator::Run ();
  st1->Join ();
  st2->Join ();
  st3->Join ();
  st4->Join ();
  Simulator::Destroy ();
}

  int
main (int argc, char *argv[])
{
  for (;;)
    {
      test ();
    }
}

