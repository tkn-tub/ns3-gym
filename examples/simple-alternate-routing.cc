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
//  n0
//     \ 5 Mb/s, 2ms
//      \          1.5Mb/s, 10ms
//       n2 ------------------------n3
//      /                          /
//     / 5 Mb/s, 2ms              /
//   n1--------------------------
//          1.5 Mb/s, 100ms
//
// this is a modification of simple-global-routing to allow for
// a single hop but higher-cost path between n1 and n3
// 
// - Tracing of queues and packet receptions to file "simple-rerouting.tr"

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/helper-module.h"
#include "ns3/global-route-manager.h"
#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SimpleAlternateRoutingExample");

int 
main (int argc, char *argv[])
{
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
#if 0 
  LogComponentEnable("GlobalRouteManager", LOG_LOGIC);
  LogComponentEnable("GlobalRouter", LOG_LOGIC);
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
  // Set up some default values for the simulation.  Use the 
  // DefaultValue::Bind () technique to tell the system what subclass of 
  // Queue to use, and what the queue limit is


  Config::SetDefault ("ns3::OnOffApplication::PacketSize", Uinteger (210));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", DataRate ("300b/s"));

  // The below metric, if set to 3 or higher, will cause packets between
  // n1 and n3 to take the 2-hop route through n2

  CommandLine cmd;
  // 
  // Additionally, we plumb this metric into the default value / command 
  // line argument system as well, for exemplary purposes.  This means 
  // that it can be resettable at the command-line to the program, 
  // rather than recompiling
  // e.g. waf --run "simple-alternate-routing --AlternateCost=5"
  uint16_t sampleMetric = 1;
  cmd.AddValue ("AlternateCost",
                "This metric is used in the example script between n3 and n1 ", 
                sampleMetric);

  // Allow the user to override any of the defaults and the above
  // DefaultValue::Bind ()s at run-time, via command-line arguments
  cmd.Parse (argc, argv);

  // Here, we will explicitly create four nodes.  In more sophisticated
  // topologies, we could configure a node factory.
  NS_LOG_INFO ("Create nodes.");
  NodeContainer c;
  c.Create (4);
  NodeContainer n0n2 = NodeContainer (c.Get (0), c.Get (2));
  NodeContainer n1n2 = NodeContainer (c.Get (1), c.Get (2));
  NodeContainer n3n2 = NodeContainer (c.Get (3), c.Get (2));
  NodeContainer n1n3 = NodeContainer (c.Get (1), c.Get (3));

  // We create the channels first without any IP addressing information
  NS_LOG_INFO ("Create channels.");
  PointToPointHelper p2p;
  p2p.SetChannelParameter ("BitRate", DataRate (5000000));
  p2p.SetChannelParameter ("Delay", MilliSeconds (2));
  NetDeviceContainer d0d2 = p2p.Build (n0n2);

  NetDeviceContainer d1d2 = p2p.Build (n1n2);

  p2p.SetChannelParameter ("BitRate", DataRate(1500000));
  p2p.SetChannelParameter ("Delay", MilliSeconds (10));
  NetDeviceContainer d3d2 = p2p.Build (n3n2);

  p2p.SetChannelParameter ("Delay", MilliSeconds (100));
  NetDeviceContainer d1d3 = p2p.Build (n1n3);

  InternetStackHelper internet;
  internet.Build (c);
  
  // Later, we add IP addresses.  The middle two octets correspond to 
 // the channel number.  
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");
  ipv4.Allocate (d0d2);
  
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4.Allocate (d1d2);
  
  ipv4.SetBase ("10.2.2.0", "255.255.255.0");
  ipv4.Allocate (d3d2);

  ipv4.SetBase ("10.3.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i3 = ipv4.Allocate (d1d3);

  i1i3.SetMetric (0, sampleMetric);
  i1i3.SetMetric (1, sampleMetric);

  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  GlobalRouteManager::PopulateRoutingTables ();

  // Create the OnOff application to send UDP datagrams 
  NS_LOG_INFO ("Create Application.");
  uint16_t port = 9;   // Discard port (RFC 863)

  // Create a flow from n3 to n1, starting at time 1.1 seconds
  OnOffHelper onoff;
  onoff.SetAppAttribute ("OnTime", ConstantVariable (1));
  onoff.SetAppAttribute ("OffTime", ConstantVariable (0));
  onoff.SetUdpRemote (i1i2.GetAddress (0), port);
  ApplicationContainer apps = onoff.Build (c.Get (3));
  apps.Start (Seconds (1.1));
  apps.Start (Seconds (10.0));

  // Create a packet sink to receive these packets
  PacketSinkHelper sink;
  sink.SetupUdp (Ipv4Address::GetAny (), port);
  apps = sink.Build (c.Get (1));
  apps.Start (Seconds (1.1));
  apps.Stop (Seconds (10.0));

  // Configure tracing of all enqueue, dequeue, and NetDevice receive events
  // Trace output will be sent to the simple-alternate-routing.tr file
  NS_LOG_INFO ("Configure Tracing.");
  AsciiTrace asciitrace ("simple-alternate-routing.tr");
  asciitrace.TraceAllQueues ();
  asciitrace.TraceAllNetDeviceRx ();

  // Also configure some tcpdump traces; each interface will be traced
  // The output files will be named simple-p2p.pcap-<nodeId>-<interfaceId>
  // and can be read by the "tcpdump -r" command (use "-tt" option to
  // display timestamps correctly)
  PcapTrace pcaptrace ("simple-alternate-routing.pcap");
  pcaptrace.TraceAllIp ();

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  return 0;
}
