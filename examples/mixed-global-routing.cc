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

#include "ns3/debug.h"

#include "ns3/command-line.h"
#include "ns3/default-value.h"
#include "ns3/ptr.h"
#include "ns3/random-variable.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"

#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"
#include "ns3/internet-node.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/csma-channel.h"
#include "ns3/csma-net-device.h"
#include "ns3/csma-topology.h"
#include "ns3/csma-ipv4-topology.h"
#include "ns3/eui48-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4.h"
#include "ns3/socket.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-route.h"
#include "ns3/point-to-point-topology.h"
#include "ns3/onoff-application.h"
#include "ns3/global-route-manager.h"

using namespace ns3;

int main (int argc, char *argv[])
{

  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
#if 0 
  DebugComponentEnable ("Object");
  DebugComponentEnable ("Queue");
  DebugComponentEnable ("DropTailQueue");
  DebugComponentEnable ("Channel");
  DebugComponentEnable ("PointToPointChannel");
  DebugComponentEnable ("PointToPointNetDevice");
  DebugComponentEnable ("GlobalRouter");
  DebugComponentEnable ("GlobalRouteManager");
#endif

  // Set up some default values for the simulation.  Use the Bind ()
  // technique to tell the system what subclass of Queue to use,
  // and what the queue limit is

  // The below DefaultValue::Bind command tells the queue factory which 
  // class to instantiate, when the queue factory is invoked in the 
  // topology code
  DefaultValue::Bind ("Queue", "DropTailQueue");

  DefaultValue::Bind ("OnOffApplicationPacketSize", "210");
  DefaultValue::Bind ("OnOffApplicationDataRate", "448kb/s");

  //DefaultValue::Bind ("DropTailQueue::m_maxPackets", 30);   

  // Allow the user to override any of the defaults and the above
  // Bind ()s at run-time, via command-line arguments
  CommandLine::Parse (argc, argv);

  Ptr<Node> n0 = Create<InternetNode> ();
  Ptr<Node> n1 = Create<InternetNode> (); 
  Ptr<Node> n2 = Create<InternetNode> (); 
  Ptr<Node> n3 = Create<InternetNode> ();
  Ptr<Node> n4 = Create<InternetNode> (); 
  Ptr<Node> n5 = Create<InternetNode> (); 
  Ptr<Node> n6 = Create<InternetNode> ();

  // We create the channels first without any IP addressing information
  Ptr<PointToPointChannel> channel0 = 
    PointToPointTopology::AddPointToPointLink (
      n0, n2, DataRate (5000000), MilliSeconds (2));

  Ptr<PointToPointChannel> channel1 = 
    PointToPointTopology::AddPointToPointLink (
      n1, n2, DataRate (5000000), MilliSeconds (2));
  
  Ptr<PointToPointChannel> channel2 = 
    PointToPointTopology::AddPointToPointLink (
      n5, n6, DataRate (1500000), MilliSeconds (10));

  // We create the channels first without any IP addressing information
  Ptr<CsmaChannel> channelc0 = 
    CsmaTopology::CreateCsmaChannel(
      DataRate(5000000), MilliSeconds(2));

  uint32_t n2ifIndex = CsmaIpv4Topology::AddIpv4CsmaNode (n2, channelc0,
                                         Eui48Address("10:54:23:54:23:50"));
  uint32_t n3ifIndex = CsmaIpv4Topology::AddIpv4CsmaNode (n3, channelc0,
                                         Eui48Address("10:54:23:54:23:51"));
  uint32_t n4ifIndex = CsmaIpv4Topology::AddIpv4CsmaNode (n4, channelc0,
                                         Eui48Address("10:54:23:54:23:52"));
  uint32_t n5ifIndex = CsmaIpv4Topology::AddIpv4CsmaNode (n5, channelc0,
                                         Eui48Address("10:54:23:54:23:53"));

  // Later, we add IP addresses.  
  PointToPointTopology::AddIpv4Addresses (
      channel0, n0, Ipv4Address ("10.1.1.1"),
      n2, Ipv4Address ("10.1.1.2"));
  
  PointToPointTopology::AddIpv4Addresses (
      channel1, n1, Ipv4Address ("10.1.2.1"),
      n2, Ipv4Address ("10.1.2.2"));
  
  PointToPointTopology::AddIpv4Addresses (
      channel2, n5, Ipv4Address ("10.1.3.1"),
      n6, Ipv4Address ("10.1.3.2"));

  CsmaIpv4Topology::AddIpv4Address (
      n2, n2ifIndex, Ipv4Address("10.250.1.1"), Ipv4Mask("255.255.255.0"));

  CsmaIpv4Topology::AddIpv4Address (
      n3, n3ifIndex, Ipv4Address("10.250.1.2"), Ipv4Mask("255.255.255.0"));
  
  CsmaIpv4Topology::AddIpv4Address (
      n4, n4ifIndex, Ipv4Address("10.250.1.3"), Ipv4Mask("255.255.255.0"));
  
  CsmaIpv4Topology::AddIpv4Address (
      n5, n5ifIndex, Ipv4Address("10.250.1.4"), Ipv4Mask("255.255.255.0"));

  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  GlobalRouteManager::PopulateRoutingTables ();

  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  Ptr<OnOffApplication> ooff = Create<OnOffApplication> (
    n0, 
    InetSocketAddress ("10.1.3.2", 80), 
    "Udp",
    ConstantVariable (1), 
    ConstantVariable (0),
    DataRate("300bps"),
    50);
  // Start the application
  ooff->Start (Seconds (1.0));
  ooff->Stop (Seconds (10.0));

  // Configure tracing of all enqueue, dequeue, and NetDevice receive events
  // Trace output will be sent to the simple-global-routing.tr file
  AsciiTrace asciitrace ("mixed-global-routing.tr");
  asciitrace.TraceAllQueues ();
  asciitrace.TraceAllNetDeviceRx ();

  // Also configure some tcpdump traces; each interface will be traced
  // The output files will be named simple-p2p.pcap-<nodeId>-<interfaceId>
  // and can be read by the "tcpdump -r" command (use "-tt" option to
  // display timestamps correctly)
  PcapTrace pcaptrace ("mixed-global-routing.pcap");
  pcaptrace.TraceAllIp ();

  Simulator::Run ();
    
  Simulator::Destroy ();
}
