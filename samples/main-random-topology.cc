/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>

#include "ns3/ptr.h"
#include "ns3/mobility-model.h"
#include "ns3/mobility-model-notifier.h"
#include "ns3/static-mobility-model.h"
#include "ns3/random-topology.h"
#include "ns3/default-value.h"
#include "ns3/command-line.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"

using namespace ns3;

static void 
CourseChange (const TraceContext &context, Ptr<const MobilityModel> position)
{
  Vector pos = position->GetPosition ();
  std::cout << Simulator::Now () << ", pos=" << position << ", x=" << pos.x << ", y=" << pos.y
            << ", z=" << pos.z << std::endl;
}

int main (int argc, char *argv[])
{
  DefaultValue::Bind ("RandomDiscPositionX", "100");
  DefaultValue::Bind ("RandomDiscPositionY", "50");
  DefaultValue::Bind ("RandomDiscPositionRho", "Uniform:0:30");

  DefaultValue::Bind ("RandomTopologyPositionType", "RandomDiscPosition");
  DefaultValue::Bind ("RandomTopologyMobilityType", "StaticMobilityModel");

  CommandLine::Parse (argc, argv);

  RandomTopology topology;

  std::vector<Ptr<Object> > objects;
  for (uint32_t i = 0; i < 10000; i++)
    {
      Ptr<MobilityModelNotifier> notifier = CreateObject<MobilityModelNotifier> ();
      notifier->TraceConnect ("/course-change", MakeCallback (&CourseChange));
      objects.push_back (notifier);
    }

  topology.Layout (objects.begin (), objects.end ());

  Simulator::StopAt (Seconds (100.0));

  Simulator::Run ();
  
  return 0;
}
