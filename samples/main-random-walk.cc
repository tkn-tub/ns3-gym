/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/ptr.h"
#include "ns3/position.h"
#include "ns3/mobility-model-notifier.h"
#include "ns3/random-walk-position.h"
#include "ns3/default-value.h"
#include "ns3/command-line.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"

using namespace ns3;

static void 
CourseChange (Ptr<const Position> position)
{
  double x, y, z;
  position->Get (x, y, z);
  std::cout << Simulator::Now () << ", pos=" << position << ", x=" << x << ", y=" << y << ", z=" << z << std::endl;
}

int main (int argc, char *argv[])
{
  Bind ("RandomWalkMinSpeed", "2");
  Bind ("RandomWalkMaxSpeed", "3");
  Bind ("RandomWalkMode", "Time");
  Bind ("RandomWalkModeDistance", "20");
  Bind ("RandomWalkModeTime", "2s");

  CommandLine::Parse (argc, argv);

  Ptr<MobilityModelNotifier> notifier = Create<MobilityModelNotifier> ();
  Ptr<RandomWalkPosition> position = Create<RandomWalkPosition> ();
  position->AddInterface (notifier);
  notifier->RegisterListener (MakeCallback (&CourseChange));

  Simulator::StopAt (Seconds (20.0));

  Simulator::Run ();
  
  return 0;
}
