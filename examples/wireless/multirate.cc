/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Duy Nguyen <duy@soe.ucsc.edu>
 */

/**
 * Scenarios: 100 nodes, multiple simultaneous flows, multi-hop ad hoc, routing,
 * and mobility
 *
 * QUICK INSTRUCTIONS:
 *
 * To optimize build: 
 * ./waf -d optimized configure
 * ./waf
 *
 * To compile:
 * ./waf --run multirate
 *
 * To compile with command line(useful for varying parameters):
 * ./waf --run "multirate --totalTime=0.3s --rateManager=ns3::MinstrelWifiManager"
 *
 * To turn on NS_LOG:
 * export NS_LOG=multirate=level_all
 * (can only view log if built with ./waf -d debug configure)
 *
 * To debug:
 * ./waf --shell
 * gdb ./build/debug/examples/wireless/multirate
 *
 * To view pcap files:
 * tcpdump -nn -tt -r filename.pcap
 *
 * To monitor the files:
 * tail -f filename.pcap
 *
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/stats-module.h"
#include "ns3/random-variable-stream.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/olsr-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"

#include <iostream>
#include <fstream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("multirate");

class Experiment
{
public:

  Experiment ();
  Experiment (std::string name);
  Gnuplot2dDataset Run (const WifiHelper &wifi, const YansWifiPhyHelper &wifiPhy,
                        const WifiMacHelper &wifiMac, const YansWifiChannelHelper &wifiChannel, const MobilityHelper &mobility);

  bool CommandSetup (int argc, char **argv);
  bool IsRouting () { return (enableRouting == 1) ? 1 : 0; }
  bool IsMobility () { return (enableMobility == 1) ? 1 : 0; }

  uint32_t GetScenario () { return scenario; }

  std::string GetRtsThreshold () { return rtsThreshold; }
  std::string GetOutputFileName () { return outputFileName; }
  std::string GetRateManager () { return rateManager; }

private:

  Ptr<Socket> SetupPacketReceive (Ptr<Node> node);
  NodeContainer GenerateNeighbors (NodeContainer c, uint32_t senderId);

  void ApplicationSetup (Ptr<Node> client, Ptr<Node> server, double start, double stop);
  void AssignNeighbors (NodeContainer c);
  void SelectSrcDest (NodeContainer c);
  void ReceivePacket (Ptr<Socket> socket);
  void CheckThroughput ();
  void SendMultiDestinations (Ptr<Node> sender, NodeContainer c);

  Gnuplot2dDataset m_output;

  double totalTime; 
  double expMean;
  double samplingPeriod;

  uint32_t bytesTotal;
  uint32_t packetSize;
  uint32_t gridSize; 
  uint32_t nodeDistance;
  uint32_t port;
  uint32_t scenario;

  bool enablePcap;
  bool enableTracing;
  bool enableFlowMon;
  bool enableRouting;
  bool enableMobility;

  NodeContainer containerA, containerB, containerC, containerD; 
  std::string rtsThreshold, rateManager, outputFileName;
};

Experiment::Experiment ()
{
}

Experiment::Experiment (std::string name) :
  m_output (name),
  totalTime (0.3),
  expMean (0.1), //flows being exponentially distributed
  samplingPeriod(0.1),
  bytesTotal (0),
  packetSize (2000),
  gridSize (10), //10x10 grid  for a total of 100 nodes
  nodeDistance (30),
  port (5000),
  scenario (4),
  enablePcap (false),
  enableTracing (true),
  enableFlowMon (false),
  enableRouting (false),
  enableMobility (false),
  rtsThreshold ("2200"), //0 for enabling rts/cts
  rateManager ("ns3::MinstrelWifiManager"),
  outputFileName ("minstrel")
{
  m_output.SetStyle (Gnuplot2dDataset::LINES);
}

Ptr<Socket>
Experiment::SetupPacketReceive (Ptr<Node> node)
{
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> sink = Socket::CreateSocket (node, tid);
  InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), port);
  sink->Bind (local);
  sink->SetRecvCallback (MakeCallback (&Experiment::ReceivePacket, this));

  return sink;
}

void
Experiment::ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  while ((packet = socket->Recv ()))
    {
      bytesTotal += packet->GetSize ();
    }
}

void
Experiment::CheckThroughput ()
{
  double mbs = ((bytesTotal * 8.0) /1000000 /samplingPeriod);
  bytesTotal = 0;
  m_output.Add ((Simulator::Now ()).GetSeconds (), mbs);

  //check throughput every samplingPeriod second
  Simulator::Schedule (Seconds (samplingPeriod), &Experiment::CheckThroughput, this);
}

/**
 *
 * Take the grid map, divide it into 4 quadrants
 * Assign all nodes from each quadrant to a specific container 
 * 
 */
void
Experiment::AssignNeighbors (NodeContainer c)
{
  uint32_t totalNodes = c.GetN ();
  for (uint32_t i=0; i< totalNodes; i++)
    {
      if ( (i % gridSize) <= (gridSize/2 - 1))
        {
          //lower left quadrant
          if ( i < totalNodes/2 )
            {
              containerA.Add (c.Get (i));
            }

          //upper left quadrant
          if ( i >= (uint32_t)(4*totalNodes)/10 )
            {
              containerC.Add (c.Get (i));
            }
        }
      if ( (i % gridSize) >= (gridSize/2 - 1))
        {
          //lower right quadrant
          if ( i < totalNodes/2 )
            {
              containerB.Add (c.Get (i));
            }

          //upper right quadrant
          if ( i >= (uint32_t)(4*totalNodes)/10  )
            {
              containerD.Add (c.Get (i));
            }
        }
    }
}

/**
 * Generate 1-hop and 2-hop neighbors of a node in grid topology
 *
 */
NodeContainer
Experiment::GenerateNeighbors (NodeContainer c, uint32_t senderId)
{
  NodeContainer nc;
  uint32_t limit = senderId + 2;
  for (uint32_t i= senderId - 2; i <= limit; i++)
    {
      //must ensure the boundaries for other topologies
      nc.Add (c.Get (i));
      nc.Add (c.Get (i + 10));
      nc.Add (c.Get (i + 20));
      nc.Add (c.Get (i - 10));
      nc.Add (c.Get (i - 20));
    }
  return nc;
}

/**
 * Sources and destinations are randomly selected such that a node 
 * may be the source for multiple destinations and a node maybe a destination 
 * for multiple sources. 
 */
void
Experiment::SelectSrcDest (NodeContainer c)
{
  uint32_t totalNodes = c.GetN ();
  Ptr<UniformRandomVariable> uvSrc = CreateObject<UniformRandomVariable> ();
  uvSrc->SetAttribute ("Min", DoubleValue (0));
  uvSrc->SetAttribute ("Max", DoubleValue (totalNodes/2 -1));
  Ptr<UniformRandomVariable> uvDest = CreateObject<UniformRandomVariable> ();
  uvDest->SetAttribute ("Min", DoubleValue (totalNodes/2));
  uvDest->SetAttribute ("Max", DoubleValue (totalNodes));

  for (uint32_t i=0; i < totalNodes/3; i++)
    {
      ApplicationSetup (c.Get (uvSrc->GetInteger ()), c.Get (uvDest->GetInteger ()),  0, totalTime);
    }
}

/**
 *
 * A sender node will  set up a flow to each of the its neighbors
 * in its quadrant randomly.  All the flows are exponentially distributed
 *
 */
void
Experiment::SendMultiDestinations (Ptr<Node> sender, NodeContainer c)
{

  // UniformRandomVariable params: (Xrange, Yrange)
  Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
  uv->SetAttribute ("Min", DoubleValue (0));
  uv->SetAttribute ("Max", DoubleValue (c.GetN ()));

  // ExponentialRandomVariable params: (mean, upperbound)
  Ptr<ExponentialRandomVariable> ev = CreateObject<ExponentialRandomVariable> ();
  ev->SetAttribute ("Mean", DoubleValue (expMean));
  ev->SetAttribute ("Bound", DoubleValue (totalTime));

  double start=0.0, stop=totalTime;
  uint32_t destIndex; 

  for (uint32_t i=0; i < c.GetN (); i++)
    {
      stop = start + ev->GetValue ();
      NS_LOG_DEBUG ("Start=" << start << " Stop=" << stop);

      do {
          destIndex = (uint32_t) uv->GetValue ();
        } while ( (c.Get (destIndex))->GetId () == sender->GetId ());

      ApplicationSetup (sender, c.Get (destIndex),  start, stop);

      start = stop;

      if(start > totalTime) 
        {
          break;
        }
    }
}

static inline Vector
GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition ();
}

static inline std::string
PrintPosition (Ptr<Node> client, Ptr<Node> server)
{
  Vector serverPos = GetPosition (server);
  Vector clientPos = GetPosition (client);

  Ptr<Ipv4> ipv4Server = server->GetObject<Ipv4>();
  Ptr<Ipv4> ipv4Client = client->GetObject<Ipv4>();

  Ipv4InterfaceAddress iaddrServer = ipv4Server->GetAddress (1,0);
  Ipv4InterfaceAddress iaddrClient = ipv4Client->GetAddress (1,0);

  Ipv4Address ipv4AddrServer = iaddrServer.GetLocal ();
  Ipv4Address ipv4AddrClient = iaddrClient.GetLocal ();

  std::ostringstream oss;
  oss << "Set up Server Device " <<  (server->GetDevice (0))->GetAddress ()
                                 << " with ip " << ipv4AddrServer
                                 << " position (" << serverPos.x << "," << serverPos.y << "," << serverPos.z << ")";

  oss << "Set up Client Device " <<  (client->GetDevice (0))->GetAddress ()
                                 << " with ip " << ipv4AddrClient
                                 << " position (" << clientPos.x << "," << clientPos.y << "," << clientPos.z << ")"
                                 << "\n";
  return oss.str ();
}

void
Experiment::ApplicationSetup (Ptr<Node> client, Ptr<Node> server, double start, double stop)
{
  Ptr<Ipv4> ipv4Server = server->GetObject<Ipv4> ();

  Ipv4InterfaceAddress iaddrServer = ipv4Server->GetAddress (1,0);
  Ipv4Address ipv4AddrServer = iaddrServer.GetLocal ();

  NS_LOG_DEBUG (PrintPosition (client, server));

  // Equipping the source  node with OnOff Application used for sending 
  OnOffHelper onoff ("ns3::UdpSocketFactory", Address (InetSocketAddress (Ipv4Address ("10.0.0.1"), port)));
  onoff.SetConstantRate (DataRate (60000000));
  onoff.SetAttribute ("PacketSize", UintegerValue (packetSize));
  onoff.SetAttribute ("Remote", AddressValue (InetSocketAddress (ipv4AddrServer, port)));

  ApplicationContainer apps = onoff.Install (client);
  apps.Start (Seconds (start));
  apps.Stop (Seconds (stop));

  Ptr<Socket> sink = SetupPacketReceive (server);

}

Gnuplot2dDataset
Experiment::Run (const WifiHelper &wifi, const YansWifiPhyHelper &wifiPhy,
                 const WifiMacHelper &wifiMac, const YansWifiChannelHelper &wifiChannel, const MobilityHelper &mobility)
{


  uint32_t nodeSize = gridSize*gridSize;
  NodeContainer c;
  c.Create (nodeSize);

  YansWifiPhyHelper phy = wifiPhy;
  phy.SetChannel (wifiChannel.Create ());

  WifiMacHelper mac = wifiMac;
  NetDeviceContainer devices = wifi.Install (phy, mac, c);


  OlsrHelper olsr;
  Ipv4StaticRoutingHelper staticRouting;

  Ipv4ListRoutingHelper list;

  if (enableRouting)
    {
      list.Add (staticRouting, 0);
      list.Add (olsr, 10);
    }

  InternetStackHelper internet;

  if (enableRouting)
    {
      internet.SetRoutingHelper (list);  // has effect on the next Install ()
    }
  internet.Install (c);


  Ipv4AddressHelper address;
  address.SetBase ("10.0.0.0", "255.255.255.0");

  Ipv4InterfaceContainer ipInterfaces;
  ipInterfaces = address.Assign (devices);

  MobilityHelper mobil= mobility;
  mobil.SetPositionAllocator ("ns3::GridPositionAllocator",
                              "MinX", DoubleValue (0.0),
                              "MinY", DoubleValue (0.0),
                              "DeltaX", DoubleValue (nodeDistance),
                              "DeltaY", DoubleValue (nodeDistance),
                              "GridWidth", UintegerValue (gridSize),
                              "LayoutType", StringValue ("RowFirst"));

  mobil.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  if (enableMobility && enableRouting)
    {
      //Rectangle (xMin, xMax, yMin, yMax)
      mobil.SetMobilityModel ("ns3::RandomDirection2dMobilityModel",
                              "Bounds", RectangleValue (Rectangle (0, 500, 0, 500)),
                              "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=10]"),
                              "Pause", StringValue ("ns3::ConstantRandomVariable[Constant=0.2]"));
    }
  mobil.Install (c);


//    NS_LOG_INFO ("Enabling global routing on all nodes");
//    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  if ( scenario == 1 && enableRouting)
    {
      SelectSrcDest (c);
    }
  else if ( scenario == 2)
    {
      //All flows begin at the same time
      for (uint32_t i = 0; i < nodeSize - 1; i = i+2)
        {
          ApplicationSetup (c.Get (i), c.Get (i+1),  0, totalTime);
        }
    }
  else if ( scenario == 3)
    {
      AssignNeighbors (c);
      //Note: these senders are hand-picked in order to ensure good coverage
      //for 10x10 grid, basically one sender for each quadrant
      //you might have to change these values for other grids 
      NS_LOG_DEBUG (">>>>>>>>>region A<<<<<<<<<");
      SendMultiDestinations (c.Get (22), containerA);

      NS_LOG_DEBUG (">>>>>>>>>region B<<<<<<<<<");
      SendMultiDestinations (c.Get (26), containerB);

      NS_LOG_DEBUG (">>>>>>>>>region C<<<<<<<<<");
      SendMultiDestinations (c.Get (72), containerC);

      NS_LOG_DEBUG (">>>>>>>>>region D<<<<<<<<<");
      SendMultiDestinations (c.Get (76), containerD);
    }
  else if ( scenario == 4)
    {
      //GenerateNeighbors(NodeContainer, uint32_t sender)
      //Note: these senders are hand-picked in order to ensure good coverage
      //you might have to change these values for other grids 
      NodeContainer c1, c2, c3, c4, c5, c6, c7, c8, c9;

      c1 = GenerateNeighbors (c, 22);
      c2 = GenerateNeighbors (c, 24);;
      c3 = GenerateNeighbors (c, 26);;
      c4 = GenerateNeighbors (c, 42);;
      c5 = GenerateNeighbors (c, 44);;
      c6 = GenerateNeighbors (c, 46);;
      c7 = GenerateNeighbors (c, 62);;
      c8 = GenerateNeighbors (c, 64);;
      c9 = GenerateNeighbors (c, 66);;

      SendMultiDestinations (c.Get (22), c1);
      SendMultiDestinations (c.Get (24), c2);
      SendMultiDestinations (c.Get (26), c3);
      SendMultiDestinations (c.Get (42), c4);
      SendMultiDestinations (c.Get (44), c5);
      SendMultiDestinations (c.Get (46), c6);
      SendMultiDestinations (c.Get (62), c7);
      SendMultiDestinations (c.Get (64), c8);
      SendMultiDestinations (c.Get (66), c9);
    }

  CheckThroughput ();

  if (enablePcap)
    {
      phy.EnablePcapAll (GetOutputFileName ());
    }

  if (enableTracing)
    {
      AsciiTraceHelper ascii;
      phy.EnableAsciiAll (ascii.CreateFileStream (GetOutputFileName () + ".tr"));
    }

  FlowMonitorHelper flowmonHelper;

  if (enableFlowMon)
    {
      flowmonHelper.InstallAll ();
    }

  Simulator::Stop (Seconds (totalTime));
  Simulator::Run ();

  if (enableFlowMon)
    {
      flowmonHelper.SerializeToXmlFile ((GetOutputFileName () + ".flomon"), false, false);
    }

  Simulator::Destroy ();

  return m_output;
}

bool
Experiment::CommandSetup (int argc, char **argv)
{
  // for commandline input
  CommandLine cmd;
  cmd.AddValue ("packetSize", "packet size", packetSize);
  cmd.AddValue ("totalTime", "simulation time", totalTime);
  // according to totalTime, select an appropriate samplingPeriod automatically.
  if (totalTime < 1.0)
    {
	  samplingPeriod = 0.1;
    }
  else
    {
	  samplingPeriod = 1.0;
    }
  // or user selects a samplingPeriod.
  cmd.AddValue ("samplingPeriod", "sampling period", samplingPeriod);
  cmd.AddValue ("rtsThreshold", "rts threshold", rtsThreshold);
  cmd.AddValue ("rateManager", "type of rate", rateManager);
  cmd.AddValue ("outputFileName", "output filename", outputFileName);
  cmd.AddValue ("enableRouting", "enable Routing", enableRouting);
  cmd.AddValue ("enableMobility", "enable Mobility", enableMobility);
  cmd.AddValue ("scenario", "scenario ", scenario);

  cmd.Parse (argc, argv);
  return true;
}

int main (int argc, char *argv[])
{

  Experiment experiment;
  experiment = Experiment ("multirate");

  //for commandline input
  experiment.CommandSetup (argc, argv);

  // set value to 0 for enabling fragmentation
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue (experiment.GetRtsThreshold ()));

  std::ofstream outfile ((experiment.GetOutputFileName ()+ ".plt").c_str ());

  MobilityHelper mobility;
  Gnuplot gnuplot;
  Gnuplot2dDataset dataset;

  WifiHelper wifi;
  WifiMacHelper wifiMac;
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  Ssid ssid = Ssid ("Testbed");

  wifiMac.SetType ("ns3::AdhocWifiMac",
                   "Ssid", SsidValue (ssid));
  wifi.SetStandard (WIFI_PHY_STANDARD_holland);
  wifi.SetRemoteStationManager (experiment.GetRateManager ());

  NS_LOG_INFO ("Scenario: " << experiment.GetScenario ());
  NS_LOG_INFO ("Rts Threshold: " << experiment.GetRtsThreshold ());
  NS_LOG_INFO ("Name:  " << experiment.GetOutputFileName ());
  NS_LOG_INFO ("Rate:  " << experiment.GetRateManager ());
  NS_LOG_INFO ("Routing: " << experiment.IsRouting ());
  NS_LOG_INFO ("Mobility: " << experiment.IsMobility ());

  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel, mobility);

  gnuplot.AddDataset (dataset);
  gnuplot.GenerateOutput (outfile);

  return 0;
}
