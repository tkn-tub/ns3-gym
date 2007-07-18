/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>

#include "ns3/ptr.h"
#include "ns3/mobility-model.h"
#include "ns3/mobility-model-notifier.h"
#include "ns3/random-walk-2d-mobility-model.h"
#include "ns3/random-topology.h"
#include "ns3/default-value.h"
#include "ns3/command-line.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"

using namespace ns3;

static void 
CourseChange (Ptr<const MobilityModel> position)
{
  Position pos = position->Get ();
  std::cout << Simulator::Now () << ", pos=" << position << ", x=" << pos.x << ", y=" << pos.y
            << ", z=" << pos.z << std::endl;
}

int main (int argc, char *argv[])
{
  Bind ("RandomWalk2dMode", "Time");
  Bind ("RandomWalk2dTime", "2s");
  Bind ("RandomWalk2dSpeed", "Constant:1.0");
  Bind ("RandomWalk2dBounds", "0:200:0:100");

  Bind ("RandomDiscPositionX", "100");
  Bind ("RandomDiscPositionY", "50");
  Bind ("RandomDiscPositionRho", "Uniform:0:30");

  Bind ("RandomTopologyPositionType", "RandomDiscPosition");
  Bind ("RandomTopologyMobilityType", "RandomWalk2dMobilityModel");

  CommandLine::Parse (argc, argv);

  RandomTopology topology;

  std::vector<Ptr<Object> > objects;
  for (uint32_t i = 0; i < 100; i++)
    {
      Ptr<MobilityModelNotifier> notifier = Create<MobilityModelNotifier> ();
      notifier->RegisterListener (MakeCallback (&CourseChange));
      objects.push_back (notifier);
    }

  topology.Layout (objects.begin (), objects.end ());

  Simulator::StopAt (Seconds (100.0));

  Simulator::Run ();
  
  return 0;
}
