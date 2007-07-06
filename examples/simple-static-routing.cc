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
 * ns-2 simple.tcl script (ported from ns-2)
 * Originally authored by Steve McCanne, 12/19/1996
 */

// Port of ns-2/tcl/ex/simple.tcl to ns-3
//
// Network topology
//
//  n0
//     \ 5 Mb/s, 2ms
//      \          1.5Mb/s, 10ms
//       n2 -------------------------n3
//      /
//     / 5 Mb/s, 2ms
//   n1
//
// - all links are point-to-point links with indicated one-way BW/delay
// - CBR/UDP flows from n0 to n3, and from n3 to n1
// - FTP/TCP flow from n0 to n3, starting at time 1.2 to time 1.35 sec.
// - UDP packet size of 210 bytes, with per-packet interval 0.00375 sec.
//   (i.e., DataRate of 448,000 bps)
// - DropTail queues 
// - Tracing of queues and packet receptions to file "simple-static-routing.tr"

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

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
#include "ns3/p2p-channel.h"
#include "ns3/p2p-net-device.h"
#include "ns3/mac-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4.h"
#include "ns3/socket.h"
#include "ns3/ipv4-route.h"
#include "ns3/p2p-topology.h"
#include "ns3/onoff-application.h"
#include "ns3/static-route-manager.h"

using namespace ns3;

int main (int argc, char *argv[])
{

  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
#if 0 
  DebugComponentEnable("Object");
  DebugComponentEnable("Queue");
  DebugComponentEnable("DropTailQueue");
  DebugComponentEnable("Channel");
  DebugComponentEnable("PointToPointChannel");
  DebugComponentEnable("PointToPointNetDevice");
#endif

  // Set up some default values for the simulation.  Use the Bind()
  // technique to tell the system what subclass of Queue to use,
  // and what the queue limit is

  // The below Bind command tells the queue factory which class to
  // instantiate, when the queue factory is invoked in the topology code
  Bind ("Queue", "DropTailQueue");

  // This bind tells the system to use global static routing.  It results in
  // a StaticRouter interface being aggregated to the internet nodes and the
  // creation of a Route Manager component to oversee the route generation.
  Bind ("DoStaticRouting", "true");

  Bind ("OnOffApplicationPacketSize", "210");
  Bind ("OnOffApplicationDataRate", "448kb/s");

  //Bind ("DropTailQueue::m_maxPackets", 30);   

  // Allow the user to override any of the defaults and the above
  // Bind()s at run-time, via command-line arguments
  CommandLine::Parse (argc, argv);

  // Here, we will explicitly create four nodes.  In more sophisticated
  // topologies, we could configure a node factory.
  Ptr<Node> n0 = Create<InternetNode> ();
  Ptr<Node> n1 = Create<InternetNode> (); 
  Ptr<Node> n2 = Create<InternetNode> (); 
  Ptr<Node> n3 = Create<InternetNode> ();

  // We create the channels first without any IP addressing information
  Ptr<PointToPointChannel> channel0 = 
    PointToPointTopology::AddPointToPointLink (
      n0, n2, DataRate(5000000), MilliSeconds(2));

  Ptr<PointToPointChannel> channel1 = 
    PointToPointTopology::AddPointToPointLink (
      n1, n2, DataRate(5000000), MilliSeconds(2));
  
  Ptr<PointToPointChannel> channel2 = 
    PointToPointTopology::AddPointToPointLink (
      n2, n3, DataRate(1500000), MilliSeconds(10));
  
  // Later, we add IP addresses.  
  PointToPointTopology::AddIpv4Addresses (
      channel0, n0, Ipv4Address("10.1.1.1"),
      n2, Ipv4Address("10.1.1.2"));
  
  PointToPointTopology::AddIpv4Addresses (
      channel1, n1, Ipv4Address("10.1.2.1"),
      n2, Ipv4Address("10.1.2.2"));
  
  PointToPointTopology::AddIpv4Addresses (
      channel2, n2, Ipv4Address("10.1.3.1"),
      n3, Ipv4Address("10.1.3.2"));

  // Here, we will use the StaticRoutingManager to build routes
  Ptr<StaticRouteManager> route_manager = Create<StaticRouteManager> ();

  // XXX this goes away once static routing is in place
  // Finally, we add static routes.  These three steps (Channel and
  // NetDevice creation, IP Address assignment, and routing) are 
  // separated because there may be a need to postpone IP Address
  // assignment (emulation) or modify to use dynamic routing
  PointToPointTopology::AddIpv4Routes(n0, n2, channel0);
  PointToPointTopology::AddIpv4Routes(n1, n2, channel1);
  PointToPointTopology::AddIpv4Routes(n2, n3, channel2);


  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  Ptr<OnOffApplication> ooff = Create<OnOffApplication> (
    n0, 
    Ipv4Address("10.1.3.2"), 
    80, 
    "Udp",
    ConstantVariable(1), 
    ConstantVariable(0));
  // Start the application
  ooff->Start(Seconds(1.0));
  ooff->Stop (Seconds(10.0));

  // Create a similar flow from n3 to n1, starting at time 1.1 seconds
  ooff = Create<OnOffApplication> (
    n3, 
    Ipv4Address("10.1.2.1"), 
    80, 
    "Udp",
    ConstantVariable(1), 
    ConstantVariable(0));
  // Start the application
  ooff->Start(Seconds(1.1));
  ooff->Stop (Seconds(10.0));

  // Here, finish off packet routing configuration
  // This will likely set by some global StaticRouting object in the future
  Ptr<Ipv4> ipv4;
  ipv4 = n0->QueryInterface<Ipv4> (Ipv4::iid);
  ipv4->SetDefaultRoute (Ipv4Address ("10.1.1.2"), 1);
  ipv4 = n3->QueryInterface<Ipv4> (Ipv4::iid);
  ipv4->SetDefaultRoute (Ipv4Address ("10.1.3.1"), 1);
  
  // Configure tracing of all enqueue, dequeue, and NetDevice receive events
  // Trace output will be sent to the simple-static-routing.tr file
  AsciiTrace asciitrace ("simple-static-routing.tr");
  asciitrace.TraceAllQueues ();
  asciitrace.TraceAllNetDeviceRx ();

  // Also configure some tcpdump traces; each interface will be traced
  // The output files will be named simple-p2p.pcap-<nodeId>-<interfaceId>
  // and can be read by the "tcpdump -r" command (use "-tt" option to
  // display timestamps correctly)
  PcapTrace pcaptrace ("simple-static-routing.pcap");
  pcaptrace.TraceAllIp ();

  Simulator::Run ();
    
  Simulator::Destroy ();
}
