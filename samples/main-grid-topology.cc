/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/ptr.h"
#include "ns3/grid-topology.h"
#include "ns3/random-walk-position.h"
#include "ns3/static-position.h"
#include "ns3/internet-node.h"

using namespace ns3;

int main (int argc, char *argv[])
{
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
  grid.SetPositionModel (StaticPosition::cid);
  //grid.SetPositionModel (RandomWalkPosition::cid);

  // finalize the setup by attaching to each object
  // in the input array a position and initializing
  // this position with the calculated coordinates.
  grid.ArrangeHorizontally (nodes);


  // iterate our nodes and print their position.
  for (std::vector<Ptr<Object> >::const_iterator j = nodes.begin ();
       j != nodes.end (); j++)
    {
      Ptr<Object> object = *j;
      Ptr<Position> position = object->QueryInterface<Position> (Position::iid);
      double x, y, z;
      position->Get (x,y,z);
      std::cout << "x=" << x << ", y=" << y << ", z=" << z << std::endl;
    }

  return 0;
}
