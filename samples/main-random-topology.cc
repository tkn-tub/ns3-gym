/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>

#include "ns3/ptr.h"
#include "ns3/mobility-model.h"
#include "ns3/default-value.h"
#include "ns3/command-line.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/node.h"
#include "ns3/mobility-helper.h"
#include "ns3/node-list.h"
#include "ns3/string.h"
#include "ns3/config.h"

using namespace ns3;

static void 
CourseChange (std::string context, Ptr<const MobilityModel> position)
{
  Vector pos = position->GetPosition ();
  std::cout << Simulator::Now () << ", pos=" << position << ", x=" << pos.x << ", y=" << pos.y
            << ", z=" << pos.z << std::endl;
}

int main (int argc, char *argv[])
{

  CommandLine::Parse (argc, argv);


  std::vector<Ptr<Object> > objects;
  for (uint32_t i = 0; i < 10000; i++)
    {
      objects.push_back (CreateObject<Node> ());
    }

  MobilityHelper mobility;
  mobility.EnableNotifier ();
  mobility.SetPositionAllocator ("RandomDiscPositionAllocator",
                                 "X", String ("100.0"),
                                 "Y", String ("100.0"),
                                 "Rho", String ("Uniform:0:30"));
  mobility.SetMobilityModel ("StaticMobilityModel");
  mobility.Layout (objects.begin (), objects.end ());

  Config::ConnectWithContext ("/NodeList/*/$MobilityModelNotifier/CourseChange",
                              MakeCallback (&CourseChange));
  
  Simulator::StopAt (Seconds (100.0));

  Simulator::Run ();
  
  return 0;
}
