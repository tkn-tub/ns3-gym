/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>

#include "ns3/ptr.h"
#include "ns3/mobility-model.h"
#include "ns3/position-allocator.h"
#include "ns3/command-line.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/node.h"
#include "ns3/node-list.h"
#include "ns3/mobility-helper.h"
#include "ns3/string.h"
#include "ns3/config.h"

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
  Config::SetDefault ("RandomWalk2dMobilityModel::Mode", String ("Time"));
  Config::SetDefault ("RandomWalk2dMobilityModel::Time", String ("2s"));
  Config::SetDefault ("RandomWalk2dMobilityModel::Speed", String ("Constant:1.0"));
  Config::SetDefault ("RandomWalk2dMobilityModel::Bounds", String ("0:200:0:100"));

  CommandLine cmd;
  cmd.Parse (argc, argv);

  for (uint32_t i = 0; i < 100; i++)
    {
      Ptr<Node> node = CreateObject<Node> ();
    }

  MobilityHelper mobility;
  mobility.EnableNotifier ();
  mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", String ("100.0"),
                                 "Y", String ("100.0"),
                                 "Rho", String ("Uniform:0:30"));
  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Mode", String ("Time"),
                             "Time", String ("2s"),
                             "Speed", String ("Constant:1.0"),
                             "Bounds", String ("0:200:0:100"));
  mobility.Layout (NodeList::Begin (), NodeList::End ());
  Config::Connect ("/NodeList/*/$MobilityModelNotifier/CourseChange",
                              MakeCallback (&CourseChange));

  Simulator::StopAt (Seconds (100.0));

  Simulator::Run ();
  
  return 0;
}
