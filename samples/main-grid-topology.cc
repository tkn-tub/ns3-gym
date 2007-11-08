/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/ptr.h"
#include "ns3/grid-topology.h"
#include "ns3/static-mobility-model.h"
#include "ns3/internet-node.h"
#include "ns3/command-line.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  CommandLine::Parse (argc, argv);

  std::vector<Ptr<Object> > nodes;

  // create an array of empty nodes for testing purposes 
  for (uint32_t i = 0; i < 120; i++)
    {
      nodes.push_back (Create<InternetNode> ());
    }

  // setup the grid itself: objects are layed out
  // started from (-100,-100) with 20 objects per row, 
  // the x interval between each object is 5 meters
  // and the y interval between each object is 20 meters
  GridTopology grid (-100, -100, 20, 5, 20);

  // each object will be attached a static position.
  grid.SetMobilityModel (StaticMobilityModel::cid);

  // finalize the setup by attaching to each object
  // in the input array a position and initializing
  // this position with the calculated coordinates.
  grid.LayoutRowFirst (nodes.begin (), nodes.end ());

  // iterate our nodes and print their position.
  for (std::vector<Ptr<Object> >::const_iterator j = nodes.begin ();
       j != nodes.end (); j++)
    {
      Ptr<Object> object = *j;
      Ptr<MobilityModel> position = object->QueryInterface<MobilityModel> (MobilityModel::iid);
      NS_ASSERT (position != 0);
      Position pos = position->GetPosition ();
      std::cout << "x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z << std::endl;
    }

  return 0;
}
