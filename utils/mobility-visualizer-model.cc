/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>

#include "ns3/ptr.h"
#include "ns3/mobility-model.h"
#include "ns3/mobility-model-notifier.h"
#include "ns3/random-topology.h"
#include "ns3/default-value.h"
#include "ns3/command-line.h"
#include "ns3/command-line.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/node.h"
#include "ns3/node-list.h"
#include "ns3/rectangle-default-value.h"

#include "mobility-visualizer.h"

using namespace ns3;

static Time g_sampleInterval = Seconds (SAMPLE_INTERVAL);
static uint32_t g_numNodes = 10;

template <typename T>
static const T* DefaultValueListGet (const std::string &name)
{
  for (DefaultValueList::Iterator iter = DefaultValueList::Begin ();
       iter != DefaultValueList::End (); iter++)
    {
      const DefaultValueBase *value = *iter;
      if (value->GetName () == name)
        {
          return dynamic_cast<const T*> (value);
        }
    }
  return NULL;
}



static void 
Sample ()
{
  
  ViewUpdateData *data = new ViewUpdateData;
  for (NodeList::Iterator nodeIter = NodeList::Begin (); nodeIter != NodeList::End (); nodeIter++)
    {
      Ptr<Node> node = *nodeIter;
      Ptr<MobilityModel> mobility = node->QueryInterface<MobilityModel> (MobilityModel::iid);
      Position pos = mobility->GetPosition ();
      Speed vel = mobility->GetSpeed ();

      NodeUpdate update;
      update.node = PeekPointer<Node> (node);
      update.x = pos.x;
      update.y = pos.y;
      update.vx = vel.dx;
      update.vy = vel.dy;
      data->updateList.push_back (update);
    }
  data->time = Simulator::Now ().GetSeconds ();
  view_update (data);
  Simulator::Schedule (g_sampleInterval, Sample);
}



int model_init (int argc, char *argv[], double *x1, double *y1, double *x2, double *y2)
{
  DefaultValue::Bind ("RandomWalk2dMode", "Time");
  DefaultValue::Bind ("RandomWalk2dTime", "5s");
  DefaultValue::Bind ("RandomWalk2dSpeed", "Constant:20.0");
  DefaultValue::Bind ("RandomDirection2dSpeed", "Uniform:10.0:20.0");
  DefaultValue::Bind ("RandomWalk2dBounds", "0:400:0:300");
  DefaultValue::Bind ("RandomDirection2dArea", "0:400:0:300");
  DefaultValue::Bind ("RandomWaypointSpeed", "Uniform:10:30");

//   DefaultValue::Bind ("RandomDiscPositionX", "100");
//   DefaultValue::Bind ("RandomDiscPositionY", "50");
//   DefaultValue::Bind ("RandomDiscPositionRho", "Uniform:0:30");

  DefaultValue::Bind ("RandomTopologyPositionType", "RandomRectanglePosition");
  DefaultValue::Bind ("RandomTopologyMobilityType", "RandomWalk2dMobilityModel");

//   CommandLine::AddArgValue ("sample-interval","sample interval", g_sampleInterval);
//   CommandLine::AddArgValue ("num-nodes","number of nodes", g_numNodes);

  CommandLine::Parse (argc, argv);

  RandomTopology topology;

  for (uint32_t i = 0; i < g_numNodes; i++)
    {
      Ptr<Node> node = Create<Node> ();
      node->AddInterface (Create<MobilityModelNotifier> ());
    }

  topology.Layout (NodeList::Begin (), NodeList::End ());

  Simulator::Schedule (g_sampleInterval, Sample);

  ClassId mobType = DefaultValueListGet<ClassIdDefaultValue> ("RandomTopologyMobilityType")->GetValue ();
  if (mobType.GetName () == "RandomWalk2dMobilityModel")
    {
      Rectangle bounds = DefaultValueListGet<RectangleDefaultValue> ("RandomWalk2dBounds")->GetValue ();
      *x1 = bounds.xMin;
      *y1 = bounds.yMin;
      *x2 = bounds.xMax;
      *y2 = bounds.yMax;
      std::cout << "RECT " << bounds.xMin << " " << bounds.xMax << " "
                << bounds.yMin << " " << bounds.yMax << std::endl;
    }
  else if (mobType.GetName () == "RandomDirection2dMobilityModel")
    {
      Rectangle bounds = DefaultValueListGet<RectangleDefaultValue> ("RandomDirection2dArea")->GetValue ();
      *x1 = bounds.xMin;
      *y1 = bounds.yMin;
      *x2 = bounds.xMax;
      *y2 = bounds.yMax;
      std::cout << "RECT " << bounds.xMin << " " << bounds.xMax << " "
                << bounds.yMin << " " << bounds.yMax << std::endl;      
    }
  else if (mobType.GetName () == "RandomWaypointMobilityModel")
    {
      std::cerr << "bounds for RandomWaypointMobilityModel not implemented" << std::endl;
      //ClassId posType = DefaultValueList::Get<ClassIdDefaultValue> ("RandomWaypointPosition")->GetValue ();
      std::cout << "?" << std::endl; // too hard to represent an abstract/probabilistic model graphically
    }
  else
    {
      NS_FATAL_ERROR ("mobility type " << mobType.GetName () << " not supported");
    }

  std::cerr << g_sampleInterval << std::endl;
  return 0;
}
