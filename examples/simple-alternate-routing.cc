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

#include "ns3/log.h"

#include "ns3/command-line.h"
#include "ns3/ptr.h"
#include "ns3/random-variable.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"

#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"
#include "ns3/internet-node.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4.h"
#include "ns3/socket.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-route.h"
#include "ns3/point-to-point-topology.h"
#include "ns3/onoff-application.h"
#include "ns3/packet-sink.h"
#include "ns3/global-route-manager.h"

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
  Ptr<Node> n0 = CreateObject<InternetNode> ();
  Ptr<Node> n1 = CreateObject<InternetNode> (); 
  Ptr<Node> n2 = CreateObject<InternetNode> (); 
  Ptr<Node> n3 = CreateObject<InternetNode> ();

  // We create the channels first without any IP addressing information
  NS_LOG_INFO ("Create channels.");
  Ptr<PointToPointChannel> channel0 = 
    PointToPointTopology::AddPointToPointLink (
      n0, n2, DataRate (5000000), MilliSeconds (2));

  Ptr<PointToPointChannel> channel1 = 
    PointToPointTopology::AddPointToPointLink (
      n1, n2, DataRate (5000000), MilliSeconds (2));
  
  Ptr<PointToPointChannel> channel2 = 
    PointToPointTopology::AddPointToPointLink (
      n2, n3, DataRate (1500000), MilliSeconds (10));
  
  Ptr<PointToPointChannel> channel3 = 
    PointToPointTopology::AddPointToPointLink (
      n1, n3, DataRate (1500000), MilliSeconds (100));
  
  // Later, we add IP addresses.  The middle two octets correspond to 
 // the channel number.  
  NS_LOG_INFO ("Assign IP Addresses.");
  PointToPointTopology::AddIpv4Addresses (
      channel0, n0, Ipv4Address ("10.0.0.1"),
      n2, Ipv4Address ("10.0.0.2"));
  
  PointToPointTopology::AddIpv4Addresses (
      channel1, n1, Ipv4Address ("10.1.1.1"),
      n2, Ipv4Address ("10.1.1.2"));
  
  PointToPointTopology::AddIpv4Addresses (
      channel2, n2, Ipv4Address ("10.2.2.1"),
      n3, Ipv4Address ("10.2.2.2"));

  PointToPointTopology::AddIpv4Addresses (
      channel3, n1, Ipv4Address ("10.3.3.1"),
      n3, Ipv4Address ("10.3.3.2"));

  PointToPointTopology::SetIpv4Metric (
      channel3, n1, n3, sampleMetric);

  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  GlobalRouteManager::PopulateRoutingTables ();

  // Create the OnOff application to send UDP datagrams 
  NS_LOG_INFO ("Create Application.");
  uint16_t port = 9;   // Discard port (RFC 863)

  // Create a flow from n3 to n1, starting at time 1.1 seconds
  Ptr<OnOffApplication> ooff = 
    CreateObject<OnOffApplication> ("Remote", Address (InetSocketAddress ("10.1.1.1", port)),
                                    "Protocol", TypeId::LookupByName ("ns3::Udp"),
                                    "OnTime", ConstantVariable (1), 
                                    "OffTime", ConstantVariable (0));
  n3->AddApplication (ooff);
  // Start the application
  ooff->Start (Seconds (1.1));
  ooff->Stop (Seconds (10.0));

  // Create a packet sink to receive these packets
  Ptr<PacketSink> sink = 
    CreateObject<PacketSink> ("Remote", Address (InetSocketAddress (Ipv4Address::GetAny (), port)), 
                              "Protocol", TypeId::LookupByName ("ns3::Udp"));
  n1->AddApplication (sink);
  // Start the sink
  sink->Start (Seconds (1.1));
  sink->Stop (Seconds (10.0));

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
