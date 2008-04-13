/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/helper-module.h"
#include "ns3/mobility-module.h"
#include "ns3/simulator-module.h"

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
  Config::SetDefault ("ns3::RandomWalk2dMobilityModel::Mode", String ("Time"));
  Config::SetDefault ("ns3::RandomWalk2dMobilityModel::Time", String ("2s"));
  Config::SetDefault ("ns3::RandomWalk2dMobilityModel::Speed", String ("Constant:1.0"));
  Config::SetDefault ("ns3::RandomWalk2dMobilityModel::Bounds", String ("0:200:0:100"));

  CommandLine cmd;
  cmd.Parse (argc, argv);

  NodeContainer c;
  c.Create (100);

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
  mobility.InstallAll ();
  Config::Connect ("/NodeList/*/$ns3::MobilityModelNotifier/CourseChange",
                              MakeCallback (&CourseChange));

  Simulator::StopAt (Seconds (100.0));

  Simulator::Run ();
  
  return 0;
}
