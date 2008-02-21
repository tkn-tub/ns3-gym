/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/command-line.h"
#include "ns3/mobility-model.h"
#include "ns3/mobility-helper.h"
#include "ns3/uinteger.h"
#include "ns3/fp-value.h"
#include "ns3/enum-value.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  CommandLine::Parse (argc, argv);

  std::vector<Ptr<Object> > nodes;

  // create an array of empty nodes for testing purposes 
  for (uint32_t i = 0; i < 120; i++)
    {
      nodes.push_back (CreateObject<Node> ());
    }

  MobilityHelper mobility;
  // setup the grid itself: objects are layed out
  // started from (-100,-100) with 20 objects per row, 
  // the x interval between each object is 5 meters
  // and the y interval between each object is 20 meters
  mobility.SetPositionAllocator ("GridPositionAllocator",
                                 "MinX", Double (-100.0),
                                 "MinY", Double (-100.0),
                                 "DeltaX", Double (5.0),
                                 "DeltaY", Double (20.0),
                                 "GridWidth", Uinteger (20),
                                 "LayoutType", "RowFirst");
  // each object will be attached a static position.
  // i.e., once set by the "position allocator", the
  // position will never change.
  mobility.SetMobilityModel ("StaticMobilityModel");

  // finalize the setup by attaching to each object
  // in the input array a position and initializing
  // this position with the calculated coordinates.
  mobility.Layout (nodes.begin (), nodes.end ());

  // iterate our nodes and print their position.
  for (std::vector<Ptr<Object> >::const_iterator j = nodes.begin ();
       j != nodes.end (); j++)
    {
      Ptr<Object> object = *j;
      Ptr<MobilityModel> position = object->GetObject<MobilityModel> ();
      NS_ASSERT (position != 0);
      Vector pos = position->GetPosition ();
      std::cout << "x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z << std::endl;
    }

  return 0;
}
