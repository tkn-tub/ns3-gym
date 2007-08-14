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

NS_DEBUG_COMPONENT_DEFINE ("CsmaMulticast");

int 
main (int argc, char *argv[])
{
//
// Users may find it convenient to turn on explicit debugging
// for selected modules; the below lines suggest how to do this
//
#if 0
  DebugComponentEnable("CsmaMulticast");

  DebugComponentEnable("Object");
  DebugComponentEnable("Queue");
  DebugComponentEnable("DropTailQueue");
  DebugComponentEnable("Channel");
  DebugComponentEnable("CsmaChannel");
  DebugComponentEnable("NetDevice");
  DebugComponentEnable("CsmaNetDevice");
  DebugComponentEnable("Ipv4L3Protocol");
  DebugComponentEnable("OnOffApplication");
  DebugComponentEnable("PacketSocket");
  DebugComponentEnable("UdpSocket");
  DebugComponentEnable("UdpL4Protocol");
  DebugComponentEnable("Ipv4L3Protocol");
  DebugComponentEnable("Ipv4StaticRouting");
  DebugComponentEnable("Ipv4Interface");
  DebugComponentEnable("ArpIpv4Interface");
  DebugComponentEnable("Ipv4LoopbackInterface");
#endif
//
// Set up default values for the simulation.  Use the DefaultValue::Bind()
// technique to tell the system what subclass of Queue to use.  The Bind
// command command tells the queue factory which class to instantiate when the
// queue factory is invoked in the topology code
//
  DefaultValue::Bind ("Queue", "DropTailQueue");
//
// Allow the user to override any of the defaults and the above Bind() at
// run-time, via command-line arguments
//
  CommandLine::Parse (argc, argv);
//
// Explicitly create the nodes required by the topology (shown above).
//
  NS_DEBUG("Create nodes.");
  Ptr<Node> n0 = Create<InternetNode> ();
  Ptr<Node> n1 = Create<InternetNode> (); 
  Ptr<Node> n2 = Create<InternetNode> (); 
  Ptr<Node> n3 = Create<InternetNode> ();
  Ptr<Node> n4 = Create<InternetNode> ();

  NS_DEBUG("Create channels.");
//
// Explicitly create the channels required by the topology (shown above).
//
  Ptr<CsmaChannel> lan0 = 
    CsmaTopology::CreateCsmaChannel(DataRate(5000000), MilliSeconds(2));

  Ptr<CsmaChannel> lan1 = 
    CsmaTopology::CreateCsmaChannel(DataRate(5000000), MilliSeconds(2));

  NS_DEBUG("Build Topology.");
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
    Eui48Address("08:00:2e:00:00:00"));
  uint32_t nd1 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n1, lan0, 
    Eui48Address("08:00:2e:00:00:01"));
  uint32_t nd2Lan0 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n2, lan0, 
    Eui48Address("08:00:2e:00:00:02"));

  uint32_t nd2Lan1 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n2, lan1, 
    Eui48Address("08:00:2e:00:00:00"));
  uint32_t nd3 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n3, lan1, 
    Eui48Address("08:00:2e:00:00:01"));
  uint32_t nd4 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n4, lan1, 
    Eui48Address("08:00:2e:00:00:02"));

  NS_DEBUG ("nd0 = " << nd0);
  NS_DEBUG ("nd1 = " << nd1);
  NS_DEBUG ("nd2Lan0 = " << nd2Lan0);
  NS_DEBUG ("nd2Lan1 = " << nd2Lan1);
  NS_DEBUG ("nd3 = " << nd3);
  NS_DEBUG ("nd4 = " << nd3);
//
// We've got the "hardware" in place.  Now we need to add IP addresses.
//
  NS_DEBUG("Assign IP Addresses.");
//
// XXX BUGBUG
// Need a better way to get the interface index.  The point-to-point topology
// as implemented can't return the index since it creates interfaces on both
// sides (i.e., it does AddIpv4Addresses, not AddIpv4Address).  We need a
// method on Ipv4 to find the interface index corresponding to a given ipv4 
// address.
//
// First, assign IP addresses to the net devices and associated interfaces
// on Lan0.  The AddIpv4Address method returns an Ipv4 interface index.
// Interpret ifIndexNd0 as the interface index to use to reference the
// net device we created on node zero when coming in from the Ipv4 interface.
// Net device numbers and interface indices are distinct.  Interpret
// ifIndexNd2Lan0 as the interface index to use to reference the
// net device we created that connects node two to lan zero.
//
  uint32_t ifIndexNd0 = CsmaIpv4Topology::AddIpv4Address (n0, nd0, 
    Ipv4Address ("10.1.1.1"), Ipv4Mask ("255.255.255.0"));

  uint32_t ifIndexNd1 = CsmaIpv4Topology::AddIpv4Address (n1, nd1, 
    Ipv4Address ("10.1.1.2"), Ipv4Mask ("255.255.255.0"));

  uint32_t ifIndexNd2Lan0 = CsmaIpv4Topology::AddIpv4Address (n2, nd2Lan0,
    Ipv4Address ("10.1.1.3"), Ipv4Mask ("255.255.255.0"));
//
// Assign IP addresses to the net devices and associated interfaces on Lan1
//
  uint32_t ifIndexNd2Lan1 = CsmaIpv4Topology::AddIpv4Address (n2, nd2Lan1, 
    Ipv4Address ("10.1.2.1"), Ipv4Mask ("255.255.255.0"));

  uint32_t ifIndexNd3 = CsmaIpv4Topology::AddIpv4Address (n3, nd1, 
    Ipv4Address ("10.1.2.2"), Ipv4Mask ("255.255.255.0"));

  uint32_t ifIndexNd4 = CsmaIpv4Topology::AddIpv4Address (n4, nd4,
    Ipv4Address ("10.1.2.3"), Ipv4Mask ("255.255.255.0"));

  NS_DEBUG ("ifIndexNd0 = " << ifIndexNd0);
  NS_DEBUG ("ifIndexNd1 = " << ifIndexNd1);
  NS_DEBUG ("ifIndexNd2Lan0 = " << ifIndexNd2Lan0);
  NS_DEBUG ("ifIndexNd2Lan1 = " << ifIndexNd2Lan1);
  NS_DEBUG ("ifIndexNd3 = " << ifIndexNd3);
  NS_DEBUG ("ifIndexNd4 = " << ifIndexNd4);
  NS_DEBUG("Configure multicasting.");
//
// Now we can configure multicasting.  As described above, the multicast 
// source is at node zero, which we assigned the IP address of 10.1.1.1 
// earlier.  We need to define a multicast group to send packets to.  This
// can be any multicast address from 224.0.0.0 through 239.255.255.255
// (avoiding the reserved routing protocol addresses).  We just pick a
// convenient number.
//
  Ipv4Address multicastSource ("10.1.1.1");
  Ipv4Address multicastGroup ("225.0.0.0");
//
// We are going to manually configure multicast routing.  This means telling
// node two that it should expect multicast data coming from IP address 
// 10.1.1.1 over its IP interface connected to Lan0.  These are called
// multicastSource and ifIndexNd2Lan0 respectively.  When node two receives
// these packets, they should be forwarded out the interface that connects it
// to Lan1 which is called ifIndexNd2Lan1.  All we need to do is to call the
// AddMulticastRoute method on node two's Ipv4 interface and provide this
// information.  (Note: the vector of output interfaces is in case there are
// multiple net devices on a node).
//
  Ptr<Ipv4> ipv4;
  ipv4 = n2->QueryInterface<Ipv4> (Ipv4::iid);

  std::vector<uint32_t> outputInterfaces (1);
  outputInterfaces[0] = ifIndexNd2Lan1;

  ipv4->AddMulticastRoute (multicastSource, multicastGroup, ifIndexNd2Lan0,
    outputInterfaces);
//
// We also need to explain to the node zero forwarding code that when it sees
// a packet destined for the multicast group it needs to send it out its
// one and only interface.  The 0xffffffff in the call means that the input
// interface qualification is not applicable in this case (the packet has
// not been received over an interface, it has been created locally).
//
  ipv4 = n0->QueryInterface<Ipv4> (Ipv4::iid);

  outputInterfaces[0] = ifIndexNd0;;

  ipv4->AddMulticastRoute (multicastSource, multicastGroup, 0xffffffff,
    outputInterfaces);
//
// As described above, node four will be the only node listening for the
// multicast data.  To enable forwarding bits up the protocol stack, we need
// to tell the stack to join the multicast group.
//
  ipv4 = n4->QueryInterface<Ipv4> (Ipv4::iid);
  ipv4->JoinMulticastGroup (multicastSource, multicastGroup);
//
// Create an OnOff application to send UDP datagrams from node zero to the
// multicast group (node four will be listening).
//
  NS_DEBUG("Create Applications.");
  Ptr<OnOffApplication> ooff = Create<OnOffApplication> (
    n0, 
    InetSocketAddress (multicastGroup, 80), 
    "Udp",
    ConstantVariable(1), 
    ConstantVariable(0),
    DataRate ("255b/s"),
    128);
//
// Tell the application when to start and stop.
//
  ooff->Start(Seconds(1.));
  ooff->Stop (Seconds(10.));
//
// Configure tracing of all enqueue, dequeue, and NetDevice receive events.
// Trace output will be sent to the file "csma-multicast.tr"
//
  NS_DEBUG("Configure Tracing.");
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
  NS_DEBUG("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_DEBUG("Done.");
}
