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
//  +----------+                                                                  
//  | external |                                                                  
//  |  Linux   |                                                                  
//  |   Host   |                                                                  
//  |  "left"  |                                                                  
//  +----------+                                                                  
//       |           n0                n3                n4
//       |       +--------+     +------------+     +------------+
//       +-------|  tap   |     |            |     |            |
//               | bridge | ... |            |     |            |
//               +--------+     +------------+     +------------+
//               |  Wifi  |     | Wifi | P2P |-----| P2P | CSMA |
//               +--------+     +------+-----+     +-----+------+
//                   |              |           ^           |
//                 ((*))          ((*))         |           |         
//                                          P2P 10.1.2      |         
//                 ((*))          ((*))                     |    n5  n6   n7
//                   |              |                       |     |   |    |
//                  n1             n2                       ================
//                     Wifi 10.1.1                           CSMA LAN 10.1.3
//
// The Wifi device on node zero is:  10.1.1.1
// The Wifi device on node one is:   10.1.1.2
// The Wifi device on node two is:   10.1.1.3
// The Wifi device on node three is: 10.1.1.4
// The P2P device on node three is:  10.1.2.1
// The P2P device on node four is:   10.1.2.2
// The CSMA device on node four is:  10.1.3.1
// The CSMA device on node five is:  10.1.3.2
// The CSMA device on node six is:   10.1.3.3
// The CSMA device on node seven is: 10.1.3.4
//
// Some simple things to do:
//
// 1) Ping one of the simulated nodes on the left side of the topology.
//
//    ./waf --run tap-wifi-dumbbell&
//    ping 10.1.1.3
//
// 2) Configure a route in the linux host and ping once of the nodes on the 
//    right, across the point-to-point link.  You will see relatively large
//    delays due to CBR background traffic on the point-to-point (see next
//    item).
//
//    ./waf --run tap-wifi-dumbbell&
//    sudo route add -net 10.1.3.0 netmask 255.255.255.0 dev left gw 10.1.1.2
//    ping 10.1.3.4
//
//    Take a look at the pcap traces and note that the timing reflects the 
//    addition of the significant delay and low bandwidth configured on the 
//    point-to-point link along with the high traffic.
//
// 3) Fiddle with the background CBR traffic across the point-to-point 
//    link and watch the ping timing change.  The OnOffApplication "DataRate"
//    attribute defaults to 500kb/s and the "PacketSize" Attribute defaults
//    to 512.  The point-to-point "DataRate" is set to 512kb/s in the script,
//    so in the default case, the link is pretty full.  This should be 
//    reflected in large delays seen by ping.  You can crank down the CBR 
//    traffic data rate and watch the ping timing change dramatically.
//
//    ./waf --run "tap-wifi-dumbbell --ns3::OnOffApplication::DataRate=100kb/s"&
//    sudo route add -net 10.1.3.0 netmask 255.255.255.0 dev left gw 10.1.1.2
//    ping 10.1.3.4
//

#include <iostream>
#include <fstream>

#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/core-module.h"
#include "ns3/wifi-module.h"
#include "ns3/helper-module.h"
#include "ns3/global-routing-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TapDumbbellExample");

int 
main (int argc, char *argv[])
{
  RandomVariable::UseGlobalSeed (1, 1, 2, 3, 5, 8);

  CommandLine cmd;
  cmd.Parse (argc, argv);

  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));

  Config::SetDefault ("ns3::Ipv4L3Protocol::CalcChecksum", BooleanValue (true)); 
  Config::SetDefault ("ns3::Icmpv4L4Protocol::CalcChecksum", BooleanValue (true)); 
  Config::SetDefault ("ns3::TcpL4Protocol::CalcChecksum", BooleanValue (true)); 
  Config::SetDefault ("ns3::UdpL4Protocol::CalcChecksum", BooleanValue (true)); 

  //
  // The topology has a Wifi network of four nodes on the left side.  We'll make
  // node zero the AP and have the other three will be the STAs.
  //
  NodeContainer nodesLeft;
  nodesLeft.Create (4);

  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());

  Ssid ssid = Ssid ("left");
  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::ArfWifiManager");

  wifi.SetMac ("ns3::NqapWifiMac", 
               "Ssid", SsidValue (ssid), 
               "BeaconGeneration", BooleanValue (true), 
               "BeaconInterval", TimeValue (Seconds (2.5)));
  NetDeviceContainer devicesLeft = wifi.Install (wifiPhy, nodesLeft.Get (0));


  wifi.SetMac ("ns3::NqstaWifiMac", 
               "Ssid", SsidValue (ssid), 
               "ActiveProbing", BooleanValue (false));
  devicesLeft.Add (wifi.Install (wifiPhy, NodeContainer (nodesLeft.Get (1), nodesLeft.Get (2), nodesLeft.Get (3))));

  MobilityHelper mobility;
  mobility.Install (nodesLeft);

  InternetStackHelper internetLeft;
  internetLeft.Install (nodesLeft);

  Ipv4AddressHelper ipv4Left;
  ipv4Left.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfacesLeft = ipv4Left.Assign (devicesLeft);

  TapBridgeHelper bridgeLeft (interfacesLeft.GetAddress (1));
  bridgeLeft.SetAttribute ("DeviceName", StringValue ("left"));
  bridgeLeft.Install (nodesLeft.Get (0), devicesLeft.Get (0));

  //
  // Now, create the right side.
  //
  NodeContainer nodesRight;
  nodesRight.Create (4);

  CsmaHelper csmaRight;
  csmaRight.SetChannelAttribute ("DataRate", DataRateValue (5000000));
  csmaRight.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));

  NetDeviceContainer devicesRight = csmaRight.Install (nodesRight);

  InternetStackHelper internetRight;
  internetRight.Install (nodesRight);

  Ipv4AddressHelper ipv4Right;
  ipv4Right.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer interfacesRight = ipv4Right.Assign (devicesRight);

  //
  // Stick in the point-to-point line between the sides.
  //
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("512kbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("10ms"));

  NodeContainer nodes = NodeContainer (nodesLeft.Get(3), nodesRight.Get (0));
  NetDeviceContainer devices = p2p.Install (nodes);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.2.0", "255.255.255.192");
  Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);

  //
  // Simulate some CBR traffic over the point-to-point link
  //
  uint16_t port = 9;   // Discard port (RFC 863)
  OnOffHelper onoff ("ns3::UdpSocketFactory", InetSocketAddress (interfaces.GetAddress (1), port));
  onoff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
  onoff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));

  ApplicationContainer apps = onoff.Install (nodesLeft.Get (3));
  apps.Start (Seconds (1.0));

  // Create a packet sink to receive these packets
  PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));

  apps = sink.Install (nodesRight.Get (0));
  apps.Start (Seconds (1.0));

  CsmaHelper::EnablePcapAll ("tap-dumbbell");
  GlobalRouteManager::PopulateRoutingTables ();

  Simulator::Stop (Seconds (60.));
  Simulator::Run ();
  Simulator::Destroy ();
}
