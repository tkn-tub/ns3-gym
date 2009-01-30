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
//  |  "left"  |                           | "right"  |  
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

  GlobalValue::Bind ("SimulatorImplementationType", 
    StringValue ("ns3::RealtimeSimulatorImpl"));

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
  // We've got the "hardware" in place.  Now add IP addresses.
  //
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);

  //
  // The Tap bridge is going to use the address information we just created
  // when it makes a Tap device on the Linux host.  This actually happens
  // when the simulation is started, so there is no dependence on ordering
  // of the IP and MAC address assignment in the initialization phase of
  // the simulation.
  //
  // The Tap bridge will suck the MAC address out of the bridged device, and 
  // the IP address and net mask out of the Ipv4Interface which is associated 
  // with the bridged device.  It will use these found values unless we 
  // configure Attribute to provide alternate values.  There are two 
  // configuration Attributes we always need to pay some attention to.
  //
  // The "Gateway" Attribute is the IP address of the default gateway that 
  // will be set on the newly created Tap devices on the Linux hosts.  We
  // can't derive this address, so it must be set.  Following standard 
  // practice in helpers, this Attribute is a construction parameter for
  // the Helper.  Here, we pick the interface corresponding to the 
  // CSMA device on node one as the default gateway.  You can change this
  // at a later time by setting the "Gateway" Attribute in the helper.
  //
  TapBridgeHelper bridge (interfaces.GetAddress (1));

  //
  // The "DeviceName" is the name of the Tap device that will be created on
  // the Linux host.  If we leave this Attribute set to the default value,
  // the Linux system will create on of the name /dev/tapx where x will be 
  // a number from 0 to 255.  In a simulation where you have some number of
  // Tap devices, it is convenient to assign a name.  Referring back to the 
  // topology illustration, we assign the name "left" to the tap device on
  // the host to the left of the diagram, and "right" to the host on the right.
  //
  // Create a tap-bridge on node zero, create a Tap device called "left" on the
  // Linux host and bridge that Linux device to the CSMA device on node zero.
  //
  bridge.SetAttribute ("DeviceName", StringValue ("left"));
  bridge.Install (nodes.Get (0), devices.Get (0));

  //
  // Create a tap-bridge on node three, create a Tap device called "right" on the
  // Linux host and bridge that Linux device to the CSMA device on node three.
  //
  bridge.SetAttribute ("DeviceName", StringValue ("right"));
  bridge.Install (nodes.Get (3), devices.Get (3));

  //
  // Configure tracing of all enqueue, dequeue, and NetDevice receive events.
  //
  NS_LOG_INFO ("Configure Tracing.");
  CsmaHelper::EnablePcapAll ("csma-tap-bridge");

  //
  // Now, do the actual simulation.  Run for a few minutes to allow the user a chance
  // to run some applications on the Linux hosts.
  //
  Simulator::Stop (Seconds (60.));
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
