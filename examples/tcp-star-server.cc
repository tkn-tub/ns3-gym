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
 */


// Default Network topology, 9 nodes in a star
/*
          n2 n3 n4
           \ | /
            \|/
       n1---n0---n5
            /|\
           / | \
          n8 n7 n6
*/
// - CBR Traffic goes from the star "arms" to the "hub"
// - Tracing of queues and packet receptions to file
//   "tcp-star-server.tr"
// - pcap traces also generated in the following files
//   "tcp-star-server-$n-$i.pcap" where n and i represent node and interface
//   numbers respectively
// Usage examples for things you might want to tweak:
//       ./waf --run="tcp-star-server"
//       ./waf --run="tcp-star-server --nNodes=25"
//       ./waf --run="tcp-star-server --ns3::OnOffApplication::DataRate=10000"
//       ./waf --run="tcp-star-server --ns3::OnOffApplication::PacketSize=500"
// See the ns-3 tutorial for more info on the command line: 
// http://www.nsnam.org/tutorials.html




#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/global-route-manager.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpServer");

int 
main (int argc, char *argv[])
{
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this

  //LogComponentEnable ("TcpServer", LOG_LEVEL_INFO);
  //LogComponentEnable ("TcpL4Protocol", LOG_LEVEL_ALL);
  //LogComponentEnable ("TcpSocketImpl", LOG_LEVEL_ALL);
  //LogComponentEnable ("PacketSink", LOG_LEVEL_ALL);
  //
  // Make the random number generators generate reproducible results.
  //
  uint32_t seed[6] = {1, 1, 2, 3, 5, 8};
  SeedManager::SetSeed(seed);

  // Set up some default values for the simulation.
  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (250));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("5kb/s"));
  uint32_t N = 9; //number of nodes in the star

  // Allow the user to override any of the defaults and the above
  // Config::SetDefault()s at run-time, via command-line arguments
  CommandLine cmd;
  cmd.AddValue("nNodes", "Number of nodes to place in the star", N);
  cmd.Parse (argc, argv);

  // Here, we will create N nodes in a star.
  NS_LOG_INFO ("Create nodes.");
  NodeContainer serverNode;
  NodeContainer clientNodes;
  serverNode.Create(1);
  clientNodes.Create(N-1);
  NodeContainer allNodes = NodeContainer(serverNode, clientNodes);

  // Install network stacks on the nodes
  InternetStackHelper internet;
  internet.Install (allNodes);

  //Collect an adjacency list of nodes for the p2p topology
  std::vector<NodeContainer> nodeAdjacencyList(N-1);
  for(uint32_t i=0; i<nodeAdjacencyList.size(); ++i)
  {
    nodeAdjacencyList[i] = NodeContainer (serverNode, clientNodes.Get (i));
  }

  // We create the channels first without any IP addressing information
  NS_LOG_INFO ("Create channels.");
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  std::vector<NetDeviceContainer> deviceAdjacencyList(N-1);
  for(uint32_t i=0; i<deviceAdjacencyList.size(); ++i)
  {
    deviceAdjacencyList[i] = p2p.Install (nodeAdjacencyList[i]);
  }

  // Later, we add IP addresses.  
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  std::vector<Ipv4InterfaceContainer> interfaceAdjacencyList(N-1);
  for(uint32_t i=0; i<interfaceAdjacencyList.size(); ++i)
  {
    std::ostringstream subnet;
    subnet<<"10.1."<<i+1<<".0";
    ipv4.SetBase (subnet.str().c_str(), "255.255.255.0");
    interfaceAdjacencyList[i] = ipv4.Assign (deviceAdjacencyList[i]);
  }

  //Turn on global static routing
  GlobalRouteManager::PopulateRoutingTables ();

  // Create a packet sink on the star "hub" to receive these packets
  uint16_t port = 50000;
  Address sinkLocalAddress(InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);
  ApplicationContainer sinkApp = sinkHelper.Install (serverNode);
  sinkApp.Start (Seconds (1.0));
  sinkApp.Stop (Seconds (10.0));

  // Create the OnOff applications to send TCP to the server
  OnOffHelper clientHelper ("ns3::TcpSocketFactory", Address ());
  clientHelper.SetAttribute 
      ("OnTime", RandomVariableValue (ConstantVariable (1)));
  clientHelper.SetAttribute 
      ("OffTime", RandomVariableValue (ConstantVariable (0)));
  //normally wouldn't need a loop here but the server IP address is different
  //on each p2p subnet
  ApplicationContainer clientApps;
  for(uint32_t i=0; i<clientNodes.GetN(); ++i)
  {
    AddressValue remoteAddress
        (InetSocketAddress (interfaceAdjacencyList[i].GetAddress (0), port));
    clientHelper.SetAttribute ("Remote", remoteAddress);
    clientApps.Add(clientHelper.Install (clientNodes.Get(i)));
  }
  clientApps.Start (Seconds (1.0));
  clientApps.Stop (Seconds (10.0));


  //configure tracing
  std::ofstream ascii;
  ascii.open ("tcp-star-server.tr");
  PointToPointHelper::EnablePcapAll ("tcp-star-server");
  PointToPointHelper::EnableAsciiAll (ascii);

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  return 0;
}
