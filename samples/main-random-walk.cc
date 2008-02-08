/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>

#include "ns3/ptr.h"
#include "ns3/mobility-model.h"
#include "ns3/position-allocator.h"
#include "ns3/default-value.h"
#include "ns3/command-line.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/node.h"
#include "ns3/node-list.h"
#include "ns3/mobility-helper.h"

using namespace ns3;

static void 
CourseChange (ns3::TraceContext const&, Ptr<const MobilityModel> mobility)
{
  Vector pos = mobility->GetPosition ();
  Vector vel = mobility->GetVelocity ();
  std::cout << Simulator::Now () << ", model=" << mobility << ", POS: x=" << pos.x << ", y=" << pos.y
            << ", z=" << pos.z << "; VEL:" << vel.x << ", y=" << vel.y
            << ", z=" << vel.z << std::endl;
}

int main (int argc, char *argv[])
{
  DefaultValue::Bind ("RandomWalk2dMode", "Time");
  DefaultValue::Bind ("RandomWalk2dTime", "2s");
  DefaultValue::Bind ("RandomWalk2dSpeed", "Constant:1.0");
  DefaultValue::Bind ("RandomWalk2dBounds", "0:200:0:100");

  DefaultValue::Bind ("RandomTopologyMobilityType", "RandomWalk2dMobilityModel");

  CommandLine::Parse (argc, argv);

  for (uint32_t i = 0; i < 100; i++)
    {
      Ptr<Node> node = CreateObject<Node> ();
    }

  MobilityHelper mobility;
  mobility.EnableNotifier ();
  mobility.SetPositionAllocator ("RandomDiscPositionAllocator",
                                 "X", "100.0",
                                 "Y", "100.0",
                                 "Rho", "Uniform:0:30");
  mobility.SetMobilityModel ("RandomWalk2dMobilityModel",
                             "Mode", "Time",
                             "Time", "2s",
                             "Speed", "Constant:1.0",
                             "Bounds", "0:200:0:100");
  mobility.Layout (NodeList::Begin (), NodeList::End ());
  NodeList::Connect ("/nodes/*/$MobilityModelNotifier/course-change", 
                     MakeCallback (&CourseChange));

  Simulator::StopAt (Seconds (100.0));

  Simulator::Run ();
  
  return 0;
}
