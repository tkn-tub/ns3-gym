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
//               |  CSMA  |     | CSMA | P2P |-----| P2P | CSMA |
//               +--------+     +------+-----+     +-----+------+
//                   |              |                       |         
//                   |              |                       |         
//                   |    n1  n2    |                       |    n5  n6   n7
//                   |     |   |    |                       |     |   |    |
//                   ================                       ================
//                    CSMA LAN 10.1.1                        CSMA LAN 10.1.2

#include <iostream>
#include <fstream>

#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/core-module.h"
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
  // The topology is symmetric around the point-to-point link.  First, create
  // the left side.
  //
  NodeContainer nodesLeft;
  nodesLeft.Create (4);

  CsmaHelper csmaLeft;
  csmaLeft.SetChannelAttribute ("DataRate", DataRateValue (5000000));
  csmaLeft.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));

  NetDeviceContainer devicesLeft = csmaLeft.Install (nodesLeft);

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
  p2p.SetDeviceAttribute ("DataRate", StringValue ("1500kbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("10ms"));

  NodeContainer nodes = NodeContainer (nodesLeft.Get(3), nodesRight.Get (0));
  NetDeviceContainer devices = p2p.Install (nodes);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.2.0", "255.255.255.192");
  Ipv4InterfaceContainer interfaces = ipv4.Assign (devices);

  CsmaHelper::EnablePcapAll ("tap-dumbbell");
  GlobalRouteManager::PopulateRoutingTables ();

  Simulator::Stop (Seconds (60.));
  Simulator::Run ();
  Simulator::Destroy ();
}
