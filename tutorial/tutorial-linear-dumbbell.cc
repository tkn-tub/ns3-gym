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

#include <fstream>

#include "ns3/core-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/simulator-module.h"
#include "ns3/global-route-manager.h"


NS_LOG_COMPONENT_DEFINE ("DumbbellSimulation");

using namespace ns3;

// Network topology
//
//                       point to point
//                      +--------------+
//                      |              |
//       n0   n1   n2   n3             n4   n5   n6   n7
//       |    |    |    |              |    |    |    |
//       ================              ================
//             lan1                          lan2
//
int 
main (int argc, char *argv[])
{
  LogComponentEnable ("DumbbellSimulation", LOG_LEVEL_INFO);

  NS_LOG_INFO ("Dumbbell Topology Simulation");
//
// Create the lan on the left side of the dumbbell.
//
  NodeContainer lan1;
  lan1.Create (4);

  InternetStackHelper internet;
  internet.Install (lan1);

  CsmaHelper csma;
  csma.SetChannelParameter ("BitRate", StringValue ("10Mbps"));
  csma.SetChannelParameter ("Delay", StringValue ("2ms"));
  NetDeviceContainer dev1 = csma.Install (lan1);
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i1 = ipv4.Assign (dev1);


//
// Create the lan on the right side of the dumbbell.
//
  NodeContainer lan2;
  lan2.Create (4);
  internet.Install (lan2);

  NetDeviceContainer dev2 = csma.Install (lan2);
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i2 = ipv4.Assign (dev2);


//
// Create the point-to-point link to connect the two lans.
//
  NodeContainer backbone = NodeContainer (lan1.Get (3), lan2.Get (0));
  PointToPointHelper p2p;
  p2p.SetChannelParameter ("BitRate", StringValue ("38400bps"));
  p2p.SetChannelParameter ("Delay", StringValue ("20ms"));
  NetDeviceContainer dev3 = p2p.Install (backbone);
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  ipv4.Assign (dev3);

//
// Create data flows across the link:
//   n0 ==> n4 ==> n0
//   n1 ==> n5 ==> n1
//   n2 ==> n6 ==> n2
//   n3 ==> n7 ==> n3
//
  uint16_t port = 7;

  UdpEchoClientHelper client;
  client.SetRemote (i2.GetAddress (0), port);
  client.SetAppAttribute ("MaxPackets", UintegerValue (100));
  client.SetAppAttribute ("Interval", StringValue ("10ms"));
  client.SetAppAttribute ("PacketSize", UintegerValue (1024));
  ApplicationContainer apps = client.Install (lan1.Get (0));
  apps.Start (Seconds(2.));
  apps.Stop (Seconds (10.0));

  client.SetRemote (i2.GetAddress (1), port);
  apps = client.Install (lan1.Get (1));
  apps.Start (Seconds(2.1));
  apps.Stop (Seconds (10.0));
  
  client.SetRemote (i2.GetAddress (2), port);
  apps = client.Install (lan1.Get (2));
  apps.Start (Seconds(2.2));
  apps.Stop (Seconds (10.0));

  client.SetRemote (i2.GetAddress (3), port);
  apps = client.Install (lan1.Get (3));
  apps.Start (Seconds(2.3));
  apps.Stop (Seconds (10.0));


  UdpEchoServerHelper server;
  server.SetPort (port);
  apps = server.Install (lan2.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));
  apps = server.Install (lan2.Get (1));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));
  apps = server.Install (lan2.Get (2));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));
  apps = server.Install (lan2.Get (3));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  GlobalRouteManager::PopulateRoutingTables ();

  std::ofstream os;
  os.open ("tutorial.tr");
  PointToPointHelper::EnableAsciiAll (os);
  CsmaHelper::EnableAsciiAll (os);

  PointToPointHelper::EnablePcapAll ("tutorial");
  CsmaHelper::EnablePcapAll ("tutorial");

  Simulator::Run ();
  Simulator::Destroy ();
}
