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

// Port of ns-2/tcl/ex/simple.tcl to ns-3
//
// Network topology
//
//       n0    n1   n2   n3
//       |     |    |    |
//     =====================
//
// - CBR/UDP flows from n0 to n1, and from n3 to n0
// - UDP packet size of 210 bytes, with per-packet interval 0.00375 sec.
//   (i.e., DataRate of 448,000 bps)
// - DropTail queues 
// - Tracing of queues and packet receptions to file "csma-cd-one-subnet.tr"

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/command-line.h"
#include "ns3/default-value.h"
#include "ns3/ptr.h"
#include "ns3/random-variable.h"
#include "ns3/debug.h"

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"

#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"
#include "ns3/internet-node.h"
#include "ns3/csma-cd-channel.h"
#include "ns3/csma-cd-net-device.h"
#include "ns3/csma-cd-topology.h"
#include "ns3/csma-cd-ipv4-topology.h"
#include "ns3/eui48-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4.h"
#include "ns3/socket.h"
#include "ns3/ipv4-route.h"
#include "ns3/onoff-application.h"


using namespace ns3;


int main (int argc, char *argv[])
{

  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
#if 0 
  DebugComponentEnable("CsmaCdNetDevice");
  DebugComponentEnable("Ipv4L3Protocol");
  DebugComponentEnable("NetDevice");
  DebugComponentEnable("Channel");
  DebugComponentEnable("CsmaCdChannel");
  DebugComponentEnable("PacketSocket");
#endif

  // Set up some default values for the simulation.  Use the Bind()
  // technique to tell the system what subclass of Queue to use,
  // and what the queue limit is

  // The below Bind command tells the queue factory which class to
  // instantiate, when the queue factory is invoked in the topology code
  Bind ("Queue", "DropTailQueue");

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
  Ptr<CsmaCdChannel> channel0 = 
    CsmaCdTopology::CreateCsmaCdChannel(
      DataRate(5000000), MilliSeconds(2));

  uint32_t n0ifIndex = CsmaCdIpv4Topology::AddIpv4CsmaCdNode (n0, channel0, 
                                         Eui48Address("10:54:23:54:23:50"));
  uint32_t n1ifIndex = CsmaCdIpv4Topology::AddIpv4CsmaCdNode (n1, channel0,
                                         Eui48Address("10:54:23:54:23:51"));
  uint32_t n2ifIndex = CsmaCdIpv4Topology::AddIpv4CsmaCdNode (n2, channel0,
                                         Eui48Address("10:54:23:54:23:52"));
  uint32_t n3ifIndex = CsmaCdIpv4Topology::AddIpv4CsmaCdNode (n3, channel0,
                                         Eui48Address("10:54:23:54:23:53"));

  // Later, we add IP addresses.  
  CsmaCdIpv4Topology::AddIpv4Address (
      n0, n0ifIndex, Ipv4Address("10.1.1.1"), Ipv4Mask("255.255.255.0"));

  CsmaCdIpv4Topology::AddIpv4Address (
      n1, n1ifIndex, Ipv4Address("10.1.1.2"), Ipv4Mask("255.255.255.0"));

  CsmaCdIpv4Topology::AddIpv4Address (
      n2, n2ifIndex, Ipv4Address("10.1.1.3"), Ipv4Mask("255.255.255.0"));
  
  CsmaCdIpv4Topology::AddIpv4Address (
      n3, n3ifIndex, Ipv4Address("10.1.1.4"), Ipv4Mask("255.255.255.0"));

  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  // from n0 to n1
  Ptr<OnOffApplication> ooff = Create<OnOffApplication> (
    n0, 
    InetSocketAddress ("10.1.1.2", 80), 
    "Udp",
    ConstantVariable(1), 
    ConstantVariable(0));
  // Start the application
  ooff->Start(Seconds(1.0));
  ooff->Stop (Seconds(10.0));

  // Create a similar flow from n3 to n0, starting at time 1.1 seconds
  ooff = Create<OnOffApplication> (
    n3, 
    InetSocketAddress ("10.1.1.1", 80), 
    "Udp",
    ConstantVariable(1), 
    ConstantVariable(0));
  // Start the application
  ooff->Start(Seconds(1.1));
  ooff->Stop (Seconds(10.0));
 
  // Configure tracing of all enqueue, dequeue, and NetDevice receive events
  // Trace output will be sent to the csma-cd-one-subnet.tr file
  AsciiTrace asciitrace ("csma-cd-one-subnet.tr");
  asciitrace.TraceAllNetDeviceRx ();
  asciitrace.TraceAllQueues ();

  // Also configure some tcpdump traces; each interface will be traced
  // The output files will be named 
  // simple-point-to-point.pcap-<nodeId>-<interfaceId>
  // and can be read by the "tcpdump -r" command (use "-tt" option to
  // display timestamps correctly)
  PcapTrace pcaptrace ("csma-cd-one-subnet.pcap");
  pcaptrace.TraceAllIp ();

  Simulator::Run ();
    
  Simulator::Destroy ();
}
