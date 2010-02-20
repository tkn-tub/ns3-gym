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

//
// This is an illustration of how one could use virtualization techniques to
// allow running applications on virtual machines talking over simulated
// networks.
//
// The actual steps required to configure the virtual machines can be rather
// involved, so we don't go into that here.  Please have a look at one of
// our HOWTOs on the nsnam wiki for more details about how to get the 
// system confgured.  For an example, have a look at "HOWTO Use Linux 
// Containers to set up virtual networks" which uses this code as an 
// example.
//
// The configuration you are after is explained in great detail in the 
// HOWTO, but looks like the following:
//
//  +----------+                           +----------+
//  | virtual  |                           | virtual  |
//  |  Linux   |                           |  Linux   |
//  |   Host   |                           |   Host   |
//  |          |                           |          |
//  |   eth0   |                           |   eth0   |
//  +----------+                           +----------+
//       |                                      |
//  +----------+                           +----------+
//  |  Linux   |                           |  Linux   |
//  |  Bridge  |                           |  Bridge  |
//  +----------+                           +----------+
//       |                                      |
//  +------------+                       +-------------+
//  | "tap-left" |                       | "tap-right" |
//  +------------+                       +-------------+
//       |           n0            n1           |
//       |       +--------+    +--------+       |
//       +-------|  tap   |    |  tap   |-------+
//               | bridge |    | bridge |
//               +--------+    +--------+
//               |  wifi  |    |  wifi  |
//               +--------+    +--------+
//                   |             |     
//                 ((*))         ((*))
//
//                   Wifi LAN 10.0.0
//
//                        ((*))
//                          |
//                     +--------+
//                     |  wifi  |
//                     +--------+
//                     | access |
//                     |  point |
//                     +--------+
//
// The wifi device on node zero is:  10.0.0.1
// The wifi device on node one is:   10.0.0.2
// The wifi device (AP) is:          10.0.0.3
//
#include <iostream>
#include <fstream>

#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/core-module.h"
#include "ns3/wifi-module.h"
#include "ns3/helper-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TapWifiVirtualMachineExample");

int 
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  //
  // We are interacting with the outside, real, world.  This means we have to 
  // interact in real-time and therefore means we have to use the real-time
  // simulator and take the time to calculate checksums.
  //
  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

  //
  // Create three ghost nodes.  The first will represent the virtual machine host
  // on the left side of the network; and the second will represent the VM on 
  // the right side.  The third node will be the wifi access point.
  //
  NodeContainer nodes;
  nodes.Create (3);

  //
  // Use the YANS helpers to get the PHY layer set up.  We'll just work with
  // the defaults here.
  //
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());

  //
  // Pick a reasonable sounding service set id for this network.
  //
  Ssid ssid = Ssid ("demo");

  //
  // Use the Wifi helper to get a basic wifi setup.  We select station managers
  // using the auto rate fallback (ARF) protocol.
  //
  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::ArfWifiManager");

  //
  // Use a non-quality-of-service MAC for the access point (AP).
  //
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifiMac.SetType ("ns3::NqapWifiMac", 
                   "Ssid", SsidValue (ssid), 
                   "BeaconGeneration", BooleanValue (true), 
                   "BeaconInterval", TimeValue (Seconds (2.5)));

  //
  // Install the Access point onto its ghost node.
  //
  wifi.Install (wifiPhy, wifiMac, nodes.Get (2));

  //
  // the remaining nodes are going to be station (STA) nodes eventually 
  // connecting to "tap-left" and "tap-right".
  //
  wifiMac.SetType ("ns3::NqstaWifiMac", 
                   "Ssid", SsidValue (ssid), 
                   "ActiveProbing", BooleanValue (false));

  NetDeviceContainer devices =  wifi.Install (wifiPhy, wifiMac, NodeContainer (nodes.Get (0), nodes.Get (1)));

  //
  // We need location information since we are talking about wifi, so add mobility
  // models to all the nodes.
  MobilityHelper mobility;
  mobility.Install (nodes);

  //
  // Use the TapBridgeHelper to connect to the pre-configured tap devices for 
  // the left side.  We go with "UseLocal" mode since the wifi devices do not
  // support promiscuous mode (because of their natures0.  This is a special
  // case mode that allows us to extend a linux bridge into ns-3 IFF we will
  // only see traffic from one other device on that bridge.  That is the case
  // for this configuration.
  //
  TapBridgeHelper tapBridge;
  tapBridge.SetAttribute ("Mode", StringValue ("UseLocal"));
  tapBridge.SetAttribute ("DeviceName", StringValue ("tap-left"));
  tapBridge.Install (nodes.Get (0), devices.Get (0));

  //
  // Connect the right side tap to the right side wifi device on the right-side
  // ghost node.
  //
  tapBridge.SetAttribute ("DeviceName", StringValue ("tap-right"));
  tapBridge.Install (nodes.Get (1), devices.Get (1));

  wifiPhy.EnablePcapAll ("tap-wifi-virtual-machine");

  //
  // Run the simulation for ten minutes to give the user time to play around
  //
  Simulator::Stop (Seconds (60.));
  Simulator::Run ();
  Simulator::Destroy ();
}
