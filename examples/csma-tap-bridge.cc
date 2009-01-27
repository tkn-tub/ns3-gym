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
 */

// Network topology
//
//  +----------+                           +----------+  
//  | external |                           | external |  
//  |  Linux   |                           |  Linux   |  
//  |   Host   |                           |   Host   |  
//  +----------+                           +----------+
//       |           n0             n3           |
//       |       +--------+     +--------+       |
//       +-------|  tap   |     |  tap   |-------+
//               | bridge | ... | bridge |
//               +--------+     +--------+
//               |  CSMA  |     |  CSMA  |
//               +--------+     +--------+
//                   |              |
//                   |              |
//                   |    n1  n2    |
//                   |     |   |    |
//                   ================
//                          LAN

#include <iostream>
#include <fstream>

#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/core-module.h"
#include "ns3/helper-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CsmaTapBridgeExample");

int 
main (int argc, char *argv[])
{
  //
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
  //
#if 0 
  LogComponentEnable ("CsmaOneSubnetExample", LOG_LEVEL_INFO);
#endif

  //
  // Make the random number generators generate reproducible results.
  //
  RandomVariable::UseGlobalSeed (1, 1, 2, 3, 5, 8);

  //
  // Allow the user to override any of the defaults and the above Bind() at
  // run-time, via command-line arguments
  //
  CommandLine cmd;
  cmd.Parse (argc, argv);

  //
  // Create the nodes required by the topology (shown above).
  //
  NS_LOG_INFO ("Create nodes.");
  NodeContainer nodes;
  nodes.Create (4);

  //
  // Create and install the network.
  //
  NS_LOG_INFO ("Build Topology");
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));

  NetDeviceContainer devices = csma.Install (nodes);

  InternetStackHelper internet;
  internet.Install (nodes);

  //
  // Add the tap bridges to nodes zero and one to enable external Linux 
  // processes to talk to the CSMA devices.
  //
  TapBridgeHelper bridge;
  NetDeviceContainer bridgeDevices;
  bridgeDevices.Add (bridge.Install (nodes.Get (0), devices.Get (0)));
  bridgeDevices.Add (bridge.Install (nodes.Get (3), devices.Get (3)));

  //
  // We've got the "hardware" in place.  Now add IP addresses.  We mjust not
  // add IP addresses to the devices that we bridged using the TapBridgeHelper
  // above.  The IP addresses are added to the bridge itself and are propagated
  // to the tap device on the host.  We do need to add IP addresses to the CSMA
  // devices that are attached to the nodes that are entirely contained within
  // the simulation (not connected to any other external host).
  //
  NS_LOG_INFO ("Assign IP Addresses.");
  NetDeviceContainer ndc;
  ndc.Add (bridgeDevices.Get (0));
  ndc.Add (devices.Get (1));
  ndc.Add (devices.Get (2));
  ndc.Add (bridgeDevices.Get (0));
          
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = ipv4.Assign (ndc);

#if 1
  //
  // Testing only -- send a packet from an internal node to an external node
  //
  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 100;
  Time interPacketInterval = Seconds (1.);
  UdpEchoClientHelper client (interfaces.GetAddress (0), 9);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer apps = client.Install (nodes.Get (1));
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (10.0));
#endif

  //
  // Configure tracing of all enqueue, dequeue, and NetDevice receive events.
  //
#if 0
  NS_LOG_INFO ("Configure Tracing.");
  std::ofstream ascii;
  ascii.open ("csma-tap-bridge.tr");
  CsmaHelper::EnableAsciiAll (ascii);
  CsmaHelper::EnablePcapAll ("csma-tap-bridge");
#endif

  //
  // Now, do the actual simulation.  Run for a few minutes to allow the user a chance
  // to run some applications on the Linux hosts.
  //
  Simulator::Stop (Seconds (3. * 60.));
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
