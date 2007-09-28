/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>

#include "ns3/ptr.h"
#include "ns3/mobility-model.h"
#include "ns3/mobility-model-notifier.h"
#include "ns3/random-topology.h"
#include "ns3/default-value.h"
#include "ns3/command-line.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/node.h"
#include "ns3/node-list.h"

using namespace ns3;

static void 
CourseChange (ns3::TraceContext const&, Ptr<const MobilityModel> position)
{
  Position pos = position->Get ();
  std::cout << Simulator::Now () << ", pos=" << position << ", x=" << pos.x << ", y=" << pos.y
            << ", z=" << pos.z << std::endl;
}

int main (int argc, char *argv[])
{
  DefaultValue::Bind ("RandomWalk2dMode", "Time");
  DefaultValue::Bind ("RandomWalk2dTime", "2s");
  DefaultValue::Bind ("RandomWalk2dSpeed", "Constant:1.0");
  DefaultValue::Bind ("RandomWalk2dBounds", "0:200:0:100");

  DefaultValue::Bind ("RandomDiscPositionX", "100");
  DefaultValue::Bind ("RandomDiscPositionY", "50");
  DefaultValue::Bind ("RandomDiscPositionRho", "Uniform:0:30");

  DefaultValue::Bind ("RandomTopologyPositionType", "RandomDiscPosition");
  DefaultValue::Bind ("RandomTopologyMobilityType", "RandomWalk2dMobilityModel");

  CommandLine::Parse (argc, argv);

  RandomTopology topology;

  for (uint32_t i = 0; i < 100; i++)
    {
      Ptr<Node> node = Create<Node> ();
      node->AddInterface (Create<MobilityModelNotifier> ());
    }

  topology.Layout (NodeList::Begin (), NodeList::End ());
  NodeList::Connect ("/nodes/*/$MobilityModelNotifier/course-change", 
                     MakeCallback (&CourseChange));

  Simulator::StopAt (Seconds (100.0));

  Simulator::Run ();
  
  return 0;
}
