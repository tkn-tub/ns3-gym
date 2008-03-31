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

// This script exercises global routing code in a mixed point-to-point
// and csma/cd environment
//
// Network topology
//
//  n0
//     \ p-p
//      \          (shared csma/cd)
//       n2 -------------------------n3
//      /            |        | 
//     / p-p        n4        n5 ---------- n6
//   n1                             p-p
//
// - CBR/UDP flows from n0 to n6
// - Tracing of queues and packet receptions to file "mixed-global-routing.tr"

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/helper-module.h"
#include "ns3/simulator-module.h"
#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"
#include "ns3/global-route-manager.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("MixedGlobalRoutingExample");

int 
main (int argc, char *argv[])
{

  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
#if 0 
  LogComponentEnable ("MixedGlobalRoutingExample", LOG_LEVEL_INFO);

  LogComponentEnable("Object", LOG_LEVEL_ALL);
  LogComponentEnable("Queue", LOG_LEVEL_ALL);
  LogComponentEnable("DropTailQueue", LOG_LEVEL_ALL);
  LogComponentEnable("Channel", LOG_LEVEL_ALL);
  LogComponentEnable("CsmaChannel", LOG_LEVEL_ALL);
  LogComponentEnable("NetDevice", LOG_LEVEL_ALL);
  LogComponentEnable("CsmaNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable("Ipv4L3Protocol", LOG_LEVEL_ALL);
  LogComponentEnable("PacketSocket", LOG_LEVEL_ALL);
  LogComponentEnable("Socket", LOG_LEVEL_ALL);
  LogComponentEnable("UdpSocket", LOG_LEVEL_ALL);
  LogComponentEnable("UdpL4Protocol", LOG_LEVEL_ALL);
  LogComponentEnable("Ipv4L3Protocol", LOG_LEVEL_ALL);
  LogComponentEnable("Ipv4StaticRouting", LOG_LEVEL_ALL);
  LogComponentEnable("Ipv4Interface", LOG_LEVEL_ALL);
  LogComponentEnable("ArpIpv4Interface", LOG_LEVEL_ALL);
  LogComponentEnable("Ipv4LoopbackInterface", LOG_LEVEL_ALL);
  LogComponentEnable("OnOffApplication", LOG_LEVEL_ALL);
  LogComponentEnable("PacketSinkApplication", LOG_LEVEL_ALL);
  LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_ALL);
  LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_ALL);
#endif
  // Set up some default values for the simulation.  Use the Bind ()


  Config::SetDefault ("ns3::OnOffApplication::PacketSize", Uinteger (210));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", DataRate ("448kb/s"));

  // Allow the user to override any of the defaults and the above
  // Bind ()s at run-time, via command-line arguments
  CommandLine cmd;
  cmd.Parse (argc, argv);

  NS_LOG_INFO ("Create nodes.");
  NodeContainer c;
  c.Create (7);
  NodeContainer n0n2 = NodeContainer (c.Get (0), c.Get (2));
  NodeContainer n1n2 = NodeContainer (c.Get (1), c.Get (2));
  NodeContainer n5n6 = NodeContainer (c.Get (5), c.Get (6));
  NodeContainer n2345 = NodeContainer (c.Get (2), c.Get (3), c.Get (4), c.Get (5));

  InternetStackHelper internet;
  internet.Build (c);

  // We create the channels first without any IP addressing information
  NS_LOG_INFO ("Create channels.");
  PointToPointHelper p2p;
  p2p.SetChannelParameter ("BitRate", DataRate (5000000));
  p2p.SetChannelParameter ("Delay", MilliSeconds (2));
  NetDeviceContainer d0d2 = p2p.Build (n0n2);

  NetDeviceContainer d1d2 = p2p.Build (n1n2);

  p2p.SetChannelParameter ("BitRate", DataRate (1500000));
  p2p.SetChannelParameter ("Delay", MilliSeconds (10));
  NetDeviceContainer d5d6 = p2p.Build (n5n6);

  // We create the channels first without any IP addressing information
  CsmaHelper csma;
  csma.SetChannelParameter ("BitRate", DataRate (5000000));
  csma.SetChannelParameter ("Delay", MilliSeconds (2));
  NetDeviceContainer d2345 = csma.Build (n2345);
  
  // Later, we add IP addresses.  
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  ipv4.Allocate (d0d2);

  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  ipv4.Allocate (d1d2);

  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i5i6 = ipv4.Allocate (d5d6);

  ipv4.SetBase ("10.250.1.0", "255.255.255.0");
  ipv4.Allocate (d2345);

  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  GlobalRouteManager::PopulateRoutingTables ();

  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  NS_LOG_INFO ("Create Applications.");
  uint16_t port = 9;   // Discard port (RFC 863)
  OnOffHelper onoff;
  onoff.SetAppAttribute ("OnTime", ConstantVariable (1));
  onoff.SetAppAttribute ("OffTime", ConstantVariable (0));
  onoff.SetAppAttribute ("DataRate", DataRate("300bps"));
  onoff.SetAppAttribute ("PacketSize", Uinteger (50));
  onoff.SetUdpRemote (i5i6.GetAddress (1), port);
  ApplicationContainer apps = onoff.Build (c.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  std::ofstream ascii;
  ascii.open ("mixed-global-routing.tr");
  PointToPointHelper::EnablePcap ("mixed-global-routing");
  PointToPointHelper::EnableAscii (ascii);
  CsmaHelper::EnablePcap ("mixed-global-routing");
  CsmaHelper::EnableAscii (ascii);


  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
