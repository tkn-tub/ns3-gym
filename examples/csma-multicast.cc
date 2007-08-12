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
//       n0    n1   n2   n3
//       |     |    |    |
//     =====================
//
// - CBR/UDP flows from n0 to n1, and from n3 to n0
// - UDP packet size of 210 bytes, with per-packet interval 0.00375 sec.
//   (i.e., DataRate of 448,000 bps)
// - DropTail queues 
// - Tracing of queues and packet receptions to file "csma-one-subnet.tr"

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
#include "ns3/csma-channel.h"
#include "ns3/csma-net-device.h"
#include "ns3/csma-topology.h"
#include "ns3/csma-ipv4-topology.h"
#include "ns3/eui48-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4.h"
#include "ns3/socket.h"
#include "ns3/ipv4-route.h"
#include "ns3/onoff-application.h"

using namespace ns3;

NS_DEBUG_COMPONENT_DEFINE ("Me");

int 
main (int argc, char *argv[])
{
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
#if 0 
  DebugComponentEnable("Me");
  DebugComponentEnable("CsmaNetDevice");
  DebugComponentEnable("Ipv4L3Protocol");
  DebugComponentEnable("NetDevice");
  DebugComponentEnable("Channel");
  DebugComponentEnable("CsmaChannel");
  DebugComponentEnable("PacketSocket");
  DebugComponentEnable("OnOffApplication");
  DebugComponentEnable("UdpSocket");
  DebugComponentEnable("UdpL4Protocol");
  DebugComponentEnable("Ipv4L3Protocol");
  DebugComponentEnable("Ipv4StaticRouting");
#endif

  DebugComponentEnable("Me");
  DebugComponentEnable("OnOffApplication");
  DebugComponentEnable("UdpSocket");
  DebugComponentEnable("UdpL4Protocol");
  DebugComponentEnable("Ipv4L3Protocol");
  DebugComponentEnable("Ipv4StaticRouting");
  DebugComponentEnable("CsmaNetDevice");
  DebugComponentEnable("CsmaChannel");

  // Set up some default values for the simulation.  Use the Bind()
  // technique to tell the system what subclass of Queue to use,
  // and what the queue limit is

  // The below Bind command tells the queue factory which class to
  // instantiate, when the queue factory is invoked in the topology code
  DefaultValue::Bind ("Queue", "DropTailQueue");

  // Allow the user to override any of the defaults and the above
  // Bind()s at run-time, via command-line arguments
  CommandLine::Parse (argc, argv);

  // Here, we will explicitly create four nodes.  In more sophisticated
  // topologies, we could configure a node factory.
  NS_DEBUG("Create nodes.");
  Ptr<Node> n0 = Create<InternetNode> ();
  Ptr<Node> n1 = Create<InternetNode> (); 
  Ptr<Node> n2 = Create<InternetNode> (); 
  Ptr<Node> n3 = Create<InternetNode> ();

  NS_DEBUG("Create channels.");
  // We create the channels first without any IP addressing information
  Ptr<CsmaChannel> channel0 = 
    CsmaTopology::CreateCsmaChannel(
      DataRate(5000000), MilliSeconds(2));

  NS_DEBUG("Build Topology.");
  uint32_t n0ifIndex = CsmaIpv4Topology::AddIpv4CsmaNode (n0, channel0, 
                                         Eui48Address("10:54:23:54:23:50"));
  uint32_t n1ifIndex = CsmaIpv4Topology::AddIpv4CsmaNode (n1, channel0,
                                         Eui48Address("10:54:23:54:23:51"));
  uint32_t n2ifIndex = CsmaIpv4Topology::AddIpv4CsmaNode (n2, channel0,
                                         Eui48Address("10:54:23:54:23:52"));
  uint32_t n3ifIndex = CsmaIpv4Topology::AddIpv4CsmaNode (n3, channel0,
                                         Eui48Address("10:54:23:54:23:53"));

  // Later, we add IP addresses.  
  NS_DEBUG("Assign IP Addresses.");

  CsmaIpv4Topology::AddIpv4Address (
      n0, n0ifIndex, Ipv4Address("10.1.1.1"), Ipv4Mask("255.255.255.0"));

  CsmaIpv4Topology::AddIpv4Address (
      n1, n1ifIndex, Ipv4Address("10.1.1.2"), Ipv4Mask("255.255.255.0"));

  CsmaIpv4Topology::AddIpv4Address (
      n2, n2ifIndex, Ipv4Address("10.1.1.3"), Ipv4Mask("255.255.255.0"));
  
  CsmaIpv4Topology::AddIpv4Address (
      n3, n3ifIndex, Ipv4Address("10.1.1.4"), Ipv4Mask("255.255.255.0"));

  // Configure multicasting
  NS_DEBUG("Configure multicasting.");
  Ipv4Address multicastSource ("10.1.1.1");
  Ipv4Address multicastGroup ("225.0.0.0");

  Ptr<Ipv4> ipv4;
  ipv4 = n0->QueryInterface<Ipv4> (Ipv4::iid);

  std::vector<uint32_t> outputInterfaces (1);
  outputInterfaces[0] = n0ifIndex;

  ipv4->AddMulticastRoute (multicastSource, multicastGroup, 0, 
    outputInterfaces);

  ipv4 = n1->QueryInterface<Ipv4> (Ipv4::iid);
  ipv4->JoinMulticastGroup (multicastSource, multicastGroup);

  ipv4 = n2->QueryInterface<Ipv4> (Ipv4::iid);
  ipv4->JoinMulticastGroup (multicastSource, multicastGroup);

  ipv4 = n3->QueryInterface<Ipv4> (Ipv4::iid);
  ipv4->JoinMulticastGroup (multicastSource, multicastGroup);

  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  // from n0 to the multicast group
  NS_DEBUG("Create Applications.");
  Ptr<OnOffApplication> ooff = Create<OnOffApplication> (
    n0, 
    InetSocketAddress (multicastGroup, 80), 
    "Udp",
    ConstantVariable(1), 
    ConstantVariable(0));
  // Start the application
  ooff->Start(Seconds(1.0));
  ooff->Stop (Seconds(10.0));

  // Configure tracing of all enqueue, dequeue, and NetDevice receive events
  // Trace output will be sent to the csma-one-subnet.tr file
  NS_DEBUG("Configure Tracing.");
  AsciiTrace asciitrace ("csma-multicast.tr");
  asciitrace.TraceAllNetDeviceRx ();
  asciitrace.TraceAllQueues ();

  // Also configure some tcpdump traces; each interface will be traced
  // The output files will be named 
  // simple-point-to-point.pcap-<nodeId>-<interfaceId>
  // and can be read by the "tcpdump -r" command (use "-tt" option to
  // display timestamps correctly)
  PcapTrace pcaptrace ("csma-multicast.pcap");
  pcaptrace.TraceAllIp ();

  NS_DEBUG("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_DEBUG("Done.");
}
