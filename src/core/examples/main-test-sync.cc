/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/simulator.h"
#include "ns3/realtime-simulator-impl.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/system-thread.h"
#include "ns3/string.h"
#include "ns3/config.h"
#include "ns3/global-value.h"
#include "ns3/ptr.h"

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
  void Doit3 (void);
};

FakeNetDevice::FakeNetDevice ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
FakeNetDevice::Doit3 (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  sleep (1);
  for (uint32_t i = 0; i < 10000; ++i)
    {
      //
      // Exercise the realtime relative now path
      //
      Simulator::ScheduleWithContext(0xffffffff, Seconds(0.0), MakeEvent (&inserted_function));
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
  Simulator::ScheduleWithContext(0xffffffff, Seconds(0.0), MakeEvent (&first_function));

  // 
  // drive the progression of m_currentTs at a ten millisecond rate from the main thread
  //
  for (double d = 0.; d < 14.999; d += 0.01)
    {
      Simulator::Schedule (Seconds (d), &background_function);
    }

  Ptr<SystemThread> st3 = Create<SystemThread> (
      MakeCallback (&FakeNetDevice::Doit3, &fnd));
  st3->Start ();

  Simulator::Stop (Seconds (15.0));
  Simulator::Run ();
  st3->Join ();
  Simulator::Destroy ();
}

int
main (int argc, char *argv[])
{
  while (true)
    {
      test ();
    }
}

