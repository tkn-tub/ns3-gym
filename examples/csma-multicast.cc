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
//                     Lan1
//                 ===========
//                 |    |    | 
//       n0   n1   n2   n3   n4
//       |    |    |
//       ===========
//           Lan0
//
// - Multicast source is at node n0;
// - Multicast forwarded by node n2 onto LAN1;
// - Nodes n0, n1, n2, n3, and n4 receive the multicast frame.
// - Node n4 listens for the data (actual listener not yet implementted)

#include "ns3/command-line.h"
#include "ns3/default-value.h"
#include "ns3/ptr.h"
#include "ns3/random-variable.h"
#include "ns3/log.h"
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
#include "ns3/mac48-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4.h"
#include "ns3/socket.h"
#include "ns3/ipv4-route.h"
#include "ns3/onoff-application.h"
#include "ns3/packet-sink.h"
#include "ns3/uinteger.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CsmaMulticastExample");

int 
main (int argc, char *argv[])
{
//
// Users may find it convenient to turn on explicit debugging
// for selected modules; the below lines suggest how to do this
//
#if 0
  LogComponentEnable ("CsmaMulticastExample", LOG_LEVEL_INFO);

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
  LogComponentEnable("PacketSinkApplication", LOG_LEVEL_ALL);
#endif
//
// Set up default values for the simulation.  Use the DefaultValue::Bind()

// Allow the user to override any of the defaults and the above Bind() at
// run-time, via command-line arguments
//
  CommandLine::Parse (argc, argv);
//
// Explicitly create the nodes required by the topology (shown above).
//
  NS_LOG_INFO ("Create nodes.");
  Ptr<Node> n0 = CreateObject<InternetNode> ();
  Ptr<Node> n1 = CreateObject<InternetNode> (); 
  Ptr<Node> n2 = CreateObject<InternetNode> (); 
  Ptr<Node> n3 = CreateObject<InternetNode> ();
  Ptr<Node> n4 = CreateObject<InternetNode> ();

  NS_LOG_INFO ("Create channels.");
//
// Explicitly create the channels required by the topology (shown above).
//
  Ptr<CsmaChannel> lan0 = 
    CsmaTopology::CreateCsmaChannel(DataRate(5000000), MilliSeconds(2));

  Ptr<CsmaChannel> lan1 = 
    CsmaTopology::CreateCsmaChannel(DataRate(5000000), MilliSeconds(2));

  NS_LOG_INFO ("Build Topology.");
//
// Now fill out the topology by creating the net devices required to connect
// the nodes to the channels and hooking them up.  AddIpv4CsmaNetDevice will
// create a net device, add a MAC address (in memory of the pink flamingo) and
// connect the net device to a nodes and also to a channel. the 
// AddIpv4CsmaNetDevice method returns a net device index for the net device
// created on the node.  Interpret nd0 as the net device we created for node
// zero.  Interpret nd2Lan0 as the net device we created for node two to
// connect to Lan0. 
//
  uint32_t nd0 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n0, lan0, 
    Mac48Address("08:00:2e:00:00:00"));
  uint32_t nd1 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n1, lan0, 
    Mac48Address("08:00:2e:00:00:01"));
  uint32_t nd2Lan0 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n2, lan0, 
    Mac48Address("08:00:2e:00:00:02"));

  uint32_t nd2Lan1 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n2, lan1, 
    Mac48Address("08:00:2e:00:00:03"));
  uint32_t nd3 __attribute__ ((unused)) = 
    CsmaIpv4Topology::AddIpv4CsmaNetDevice (n3, lan1, 
    Mac48Address("08:00:2e:00:00:04"));
  uint32_t nd4 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n4, lan1, 
    Mac48Address("08:00:2e:00:00:05"));

  NS_LOG_INFO ("nd0 = " << nd0);
  NS_LOG_INFO ("nd1 = " << nd1);
  NS_LOG_INFO ("nd2Lan0 = " << nd2Lan0);
  NS_LOG_INFO ("nd2Lan1 = " << nd2Lan1);
  NS_LOG_INFO ("nd3 = " << nd3);
  NS_LOG_INFO ("nd4 = " << nd3);
//
// We've got the "hardware" in place.  Now we need to add IP addresses.
//
  NS_LOG_INFO ("Assign IP Addresses.");
  CsmaIpv4Topology::AddIpv4Address (n0, nd0, Ipv4Address ("10.1.1.1"), 
    Ipv4Mask ("255.255.255.0"));

  CsmaIpv4Topology::AddIpv4Address (n1, nd1, Ipv4Address ("10.1.1.2"), 
    Ipv4Mask ("255.255.255.0"));

//
// We'll need these addresses later
//
  Ipv4Address n2Lan0Addr ("10.1.1.3");
  Ipv4Address n2Lan1Addr ("10.1.2.1");

  CsmaIpv4Topology::AddIpv4Address (n2, nd2Lan0, n2Lan0Addr, 
    Ipv4Mask ("255.255.255.0"));
//
// Assign IP addresses to the net devices and associated interfaces on Lan1
//
  CsmaIpv4Topology::AddIpv4Address (n2, nd2Lan1, n2Lan1Addr, 
    Ipv4Mask ("255.255.255.0"));

  CsmaIpv4Topology::AddIpv4Address (n3, nd1, Ipv4Address ("10.1.2.2"), 
    Ipv4Mask ("255.255.255.0"));

  CsmaIpv4Topology::AddIpv4Address (n4, nd4, Ipv4Address ("10.1.2.3"), 
    Ipv4Mask ("255.255.255.0"));

  NS_LOG_INFO ("Configure multicasting.");
//
// Now we can configure multicasting.  As described above, the multicast 
// source is at node zero, which we assigned the IP address of 10.1.1.1 
// earlier.  We need to define a multicast group to send packets to.  This
// can be any multicast address from 224.0.0.0 through 239.255.255.255
// (avoiding the reserved routing protocol addresses).  We just pick a
// convenient number (225.0.0.0) and or in some bits to let us verify that
// correct Ethernet multicast addresses are constructed down in the system. 
//
  Ipv4Address multicastSource ("10.1.1.1");
  Ipv4Address multicastGroup ("225.1.2.4");
//
// We are going to manually configure multicast routing.  This means telling
// node two that it should expect multicast data coming from IP address 
// 10.1.1.1 originally.  It should expect these data coming in over its IP 
// interface connected to Lan0.  When node two receives these packets, they
// should be forwarded out the interface that connects it to Lan1.
//
// We're going to need the interface indices on node two corresponding to 
// these interfaces, which we call ifIndexLan0 and ifIndexLan1.  The most
// general way to get these interfaces is to look them up by IP address.
// Looking back to the topology creation calls above, we saved the addresses
// assigned to the interface connecting node two to Lan0 and Lan1.  Now is
// a fine time to find the interface indices on node two.
//
  Ptr<Ipv4> ipv4;
  ipv4 = n2->GetObject<Ipv4> ();

  uint32_t ifIndexLan0 = ipv4->FindInterfaceForAddr (n2Lan0Addr);
  uint32_t ifIndexLan1 = ipv4->FindInterfaceForAddr (n2Lan1Addr);
//
// Now, we need to do is to call the AddMulticastRoute () method on node 
// two's Ipv4 interface and tell it that whenever it receives a packet on
// the interface from Lan0, with the packet from the multicast source, 
// destined for the multicast group, it should forward these packets down
// the interface connecting it to Lan1.  (Note: the vector of output
// interfaces is in case there are multiple net devices on a node -- not
// true in this case).
//
  std::vector<uint32_t> outputInterfaces (1);
  outputInterfaces[0] = ifIndexLan1;

  ipv4->AddMulticastRoute (multicastSource, multicastGroup, ifIndexLan0,
    outputInterfaces);
//
// We need to specify how the source node handles multicasting.  There are a
// number of ways we can deal with this, we just need to pick one.  The first
// method is to add an explicit route out of the source node, just as we did
// for the forwarding node.  Use this method when you want to send packets out
// multiple interfaces or send packets out different interfaces based on the
// differing multicast groups.  Since the source is local, there will be no 
// input interface over which packets are received, so use  
// Ipv4RoutingProtocol::IF_INDEX_ANY as a wildcard.
//
// A second way is to specify a multicast route using wildcards.  If you 
// want to send multicasts out differing sets of interfaces based on the 
// multicast group, you can use AddMulticastRoute () but specify the origin 
// as a wildcard.  If you want all multicasts to go out a single set of 
// interfaces, you can make both the origin and group a wildcard.
//
// If you have a simple system, where the source has a single interface, this
// can be done via the SetDefaultMulticastRoute () method on the Ipv4 
// interface.  This tells the system to send all multicasts out a single
// specified network interface index.
//
// A last way is to specify a (or use an existing) default unicast route.  The
// multicast routing code uses the unicast default route as a multicast "route
// of last resort."  this method for is also on Ipv4 and is called 
// SetDefaultRoute ().
//
// Since this is a simple multicast example, we use the 
// SetDefaultMulticastRoute () approach.  We are going to first need the 
// Ipv4 interface for node 0 which is the multicast source.  We use this
// interface to find the output interface index, and tell node zero to send
// its multicast traffic out that interface.
//
  ipv4 = n0->GetObject<Ipv4> ();
  uint32_t ifIndexSrc = ipv4->FindInterfaceForAddr (multicastSource);
  ipv4->SetDefaultMulticastRoute (ifIndexSrc);
//
// As described above, node four will be the only node listening for the
// multicast data.  To enable forwarding bits up the protocol stack, we need
// to tell the stack to join the multicast group.
//
  ipv4 = n4->GetObject<Ipv4> ();
  ipv4->JoinMulticastGroup (multicastSource, multicastGroup);
//
// Create an OnOff application to send UDP datagrams from node zero to the
// multicast group (node four will be listening).
//
  NS_LOG_INFO ("Create Applications.");

  uint16_t port = 9;   // Discard port (RFC 863)

  // Configure a multicast packet generator that generates a packet
  // every few seconds
  Ptr<OnOffApplication> ooff = 
    CreateObjectWith<OnOffApplication> (
                                        "Node", n0, 
                                        "Remote", Address (InetSocketAddress (multicastGroup, port)), 
                                        "Protocol", TypeId::LookupByName ("Udp"),
                                        "OnTime", ConstantVariable(1), 
                                        "OffTime", ConstantVariable(0),
                                        "DataRate", DataRate ("255b/s"),
                                        "PacketSize", Uinteger (128));
  n0->AddApplication (ooff);
//
// Tell the application when to start and stop.
//
  ooff->Start(Seconds(1.));
  ooff->Stop (Seconds(10.));

  // Create an optional packet sink to receive these packets
  // If you enable logging on this (above) it will print a log statement
  // for every packet received
  Ptr<PacketSink> sink = 
    CreateObjectWith<PacketSink> (
                                  "Node", n4,
                                  "Local", Address (InetSocketAddress (Ipv4Address::GetAny (), port)),
                                  "Protocol", TypeId::LookupByName ("Udp"));
  n4->AddApplication (sink);
  // Start the sink
  sink->Start (Seconds (1.0));
  sink->Stop (Seconds (10.0));

//
// Configure tracing of all enqueue, dequeue, and NetDevice receive events.
// Trace output will be sent to the file "csma-multicast.tr"
//
  NS_LOG_INFO ("Configure Tracing.");
  AsciiTrace asciitrace ("csma-multicast.tr");
  asciitrace.TraceAllNetDeviceRx ();
  asciitrace.TraceAllQueues ();
//
// Also configure some tcpdump traces; each interface will be traced.
// The output files will be named:
//     csma-multicast.pcap-<nodeId>-<interfaceId>
// and can be read by the "tcpdump -r" command (use "-tt" option to
// display timestamps correctly)
//
  PcapTrace pcaptrace ("csma-multicast.pcap");
  pcaptrace.TraceAllIp ();
//
// Now, do the actual simulation.
//
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
