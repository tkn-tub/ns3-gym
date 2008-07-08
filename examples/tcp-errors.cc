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

//
// Network topology
//
//           10Mb/s, 10ms       10Mb/s, 10ms
//       n0-----------------n1-----------------n2
//
//
// - CBR traffic for 1000 seconds
// - Tracing of queues and packet receptions to file 
//   "tcp-large-transfer.tr"
// - pcap traces also generated in the following files
//   "tcp-large-transfer-$n-$i.pcap" where n and i represent node and interface
// numbers respectively
//  Usage (e.g.): ./waf --run tcp-large-transfer


#include <ctype.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/common-module.h"
#include "ns3/helper-module.h"
#include "ns3/node-module.h"
#include "ns3/global-route-manager.h"
#include "ns3/simulator-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpErrors");

int main (int argc, char *argv[])
{

  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
  //  LogComponentEnable("TcpL4Protocol", LOG_LEVEL_ALL);
  //  LogComponentEnable("TcpSocketImpl", LOG_LEVEL_ALL);
  //  LogComponentEnable("PacketSink", LOG_LEVEL_ALL);
  //  LogComponentEnable("TcpErrors", LOG_LEVEL_ALL);

  //
  // Make the random number generators generate reproducible results.
  //
  RandomVariable::UseGlobalSeed (1, 1, 2, 3, 5, 8);

  // Allow the user to override any of the defaults and the above
  // Bind()s at run-time, via command-line arguments
  CommandLine cmd;
  cmd.Parse (argc, argv);

  // Here, we will explicitly create three nodes.  The first container contains
  // nodes 0 and 1 from the diagram above, and the second one contains nodes
  // 1 and 2.  This reflects the channel connectivity, and will be used to
  // install the network interfaces and connect them with a channel.
  NodeContainer n0n1;
  n0n1.Create (2);

  NodeContainer n1n2;
  n1n2.Add (n0n1.Get (1));
  n1n2.Create (1);

  // We create the channels first without any IP addressing information
  // First make and configure the helper, so that it will put the appropriate
  // parameters on the network interfaces and channels we are about to install.
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", DataRateValue (DataRate(10000000)));
  p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds(10)));

  // And then install devices and channels connecting our topology.
  NetDeviceContainer d0d1 = p2p.Install (n0n1);
  NetDeviceContainer d1d2 = p2p.Install (n1n2);

  // Now add ip/tcp stack to all nodes.
  NodeContainer allNodes = NodeContainer (n0n1, n1n2.Get (1));
  InternetStackHelper internet;
  internet.Install (allNodes);

  // Later, we add IP addresses.
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4.Assign (d1d2);

  // and setup ip routing tables to get total ip-level connectivity.
  GlobalRouteManager::PopulateRoutingTables ();

  // Set up the sending CBR application
  uint16_t servPort = 50000;
  Address remoteAddress(InetSocketAddress(i1i2.GetAddress (1), servPort));
  OnOffHelper clientHelper ("ns3::TcpSocketFactory", remoteAddress);
  clientHelper.SetAttribute 
      ("OnTime", RandomVariableValue (ConstantVariable (1)));
  clientHelper.SetAttribute 
      ("OffTime", RandomVariableValue (ConstantVariable (0)));
  ApplicationContainer clientApp = clientHelper.Install(n0n1.Get(0));
  clientApp.Start (Seconds (1.0));
  clientApp.Stop (Seconds (10.0));

  // Create a packet sink to receive at n2
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), servPort));
  ApplicationContainer sinkApp = sinkHelper.Install (n1n2.Get(1));
  sinkApp.Start (Seconds (1.0));
  sinkApp.Stop (Seconds (10.0));

  // We're going to model a lossy channel
  RandomVariableValue u01(UniformVariable (0.0, 1.0));
  DoubleValue rate(0.001);
  Ptr<RateErrorModel> em1 = 
      CreateObject<RateErrorModel> ("RanVar", u01, "ErrorRate", rate);
  Ptr<RateErrorModel> em2 = 
      CreateObject<RateErrorModel> ("RanVar", u01, "ErrorRate", rate);
  //put error models on both netdevices of the router nodes so that there is
  //loss of both data and acks
  d0d1.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue (em1));
  d1d2.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue (em2));

  //Ask for ASCII and pcap traces of network traffic
  std::ofstream ascii;
  ascii.open ("tcp-errors.tr");
  PointToPointHelper::EnableAsciiAll (ascii);

  PointToPointHelper::EnablePcapAll ("tcp-errors");

  // Finally, set up the simulator to run for 1000 seconds.
  Simulator::Stop (Seconds(1000));
  Simulator::Run ();
  Simulator::Destroy ();
}
