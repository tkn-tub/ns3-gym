/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
 *
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/global-routing-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mesh-module.h"
#include "ns3/mobility-module.h"

#include "ns3/dot11s-helper.h"

using namespace ns3;
using namespace dot11s;

NS_LOG_COMPONENT_DEFINE ("TestMeshScript");

int
main (int argc, char *argv[])
{
  // Creating square topology with nNodes x nNodes grid
  int      xSize       = 2;
  int      ySize       = 2;
  double   step        = 100.0; // Grid with one-hop edge
  double   randomStart = 0.1;   // One beacon interval
  uint32_t nIfaces     = 1;
  bool     chan        = false;
  bool     pcap        = false;
  
  // Command line arguments
  CommandLine cmd;
  cmd.AddValue ("x-size", "Number of nodes in a row grid. [6]", xSize);
  cmd.AddValue ("y-size", "Number of rows in a grid. [6]", ySize);
  cmd.AddValue ("step",   "Size of edge in our grid, meters. [100 m]", step);
  cmd.AddValue ("start",  "Maximum random start delay, seconds. [0.1 s]", randomStart);
  cmd.AddValue ("interfaces", "Number of radio interfaces used by each mesh point. [1]", nIfaces);
  cmd.AddValue ("channels",   "Use different frequency channels for different interfaces. [0]", chan);
  cmd.AddValue ("pcap",   "Enable PCAP traces on interfaces. [0]", pcap);
  
  cmd.Parse (argc, argv);
  NS_LOG_DEBUG ("Grid:" << xSize << "*" << ySize);
  
  // Creating nodes
  NodeContainer nodes;
  nodes.Create (ySize*xSize);

  // Setting channel
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  
  // Install mesh point devices & protocols
  MeshWifiHelper mesh;
  mesh.SetSpreadInterfaceChannels (chan);
  NetDeviceContainer meshDevices = mesh.Install (wifiPhy, nodes, nIfaces);
  
  // Setup mobility
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (step),
                                 "DeltaY", DoubleValue (step),
                                 "GridWidth", UintegerValue (xSize),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);
  
  NS_LOG_UNCOND("start");
  
  // Install internet stack
  InternetStackHelper stack;
  stack.Install (nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (meshDevices);
  
  // Install applications
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));
  UdpEchoClientHelper echoClient (interfaces.GetAddress (0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1000));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.001)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  ApplicationContainer clientApps = echoClient.Install (nodes.Get (1));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
  
  // Enable PCAP trace
  if (pcap)
      wifiPhy.EnablePcapAll (std::string ("mp-") + mesh.GetSsid ().PeekString ());
  
  // Happy end
  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
