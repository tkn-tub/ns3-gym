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
//       =================
//              LAN
//
// - CBR/UDP flows from n0 to n1 and from n3 to n0
// - DropTail queues 
// - Tracing of queues and packet receptions to file "csma-one-subnet.tr"

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

NS_DEBUG_COMPONENT_DEFINE ("CsmaOneSubnet");

int 
main (int argc, char *argv[])
{
//
// Users may find it convenient to turn on explicit debugging
// for selected modules; the below lines suggest how to do this
//
#if 0 
  DebugComponentEnable("CsmaOneSubnet");

  DebugComponentEnable("Object");
  DebugComponentEnable("Queue");
  DebugComponentEnable("DropTailQueue");
  DebugComponentEnable("Channel");
  DebugComponentEnable("CsmaChannel");
  DebugComponentEnable("CsmaNetDevice");
  DebugComponentEnable("Ipv4L3Protocol");
  DebugComponentEnable("NetDevice");
  DebugComponentEnable("PacketSocket");
  DebugComponentEnable("OnOffApplication");
  DebugComponentEnable("Socket");
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

  NS_DEBUG("Create channels.");
//
// Explicitly create the channels required by the topology (shown above).
//
  Ptr<CsmaChannel> lan = CsmaTopology::CreateCsmaChannel(
    DataRate(5000000), MilliSeconds(2));

  NS_DEBUG("Build Topology.");
//
// Now fill out the topology by creating the net devices required to connect
// the nodes to the channels and hooking them up.  AddIpv4CsmaNetDevice will
// create a net device, add a MAC address (in memory of the pink flamingo) and
// connect the net device to a nodes and also to a channel. the 
// AddIpv4CsmaNetDevice method returns a net device index for the net device
// created on the node.  Interpret nd0 as the net device we created for node
// zero.
//
  uint32_t nd0 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n0, lan, 
    Eui48Address("08:00:2e:00:00:00"));

  uint32_t nd1 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n1, lan, 
    Eui48Address("08:00:2e:00:00:01"));

  uint32_t nd2 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n2, lan, 
    Eui48Address("08:00:2e:00:00:02"));

  uint32_t nd3 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n3, lan, 
    Eui48Address("08:00:2e:00:00:03"));

  NS_DEBUG ("nd0 = " << nd0);
  NS_DEBUG ("nd1 = " << nd1);
  NS_DEBUG ("nd2 = " << nd2);
  NS_DEBUG ("nd3 = " << nd3);
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
// Assign IP addresses to the net devices and associated interfaces
// on the lan.  The AddIpv4Address method returns an Ipv4 interface index
// which we do not need here.
//
  CsmaIpv4Topology::AddIpv4Address (n0, nd0, Ipv4Address("10.1.1.1"), 
    Ipv4Mask("255.255.255.0"));

  CsmaIpv4Topology::AddIpv4Address (n1, nd1, Ipv4Address("10.1.1.2"), 
    Ipv4Mask("255.255.255.0"));

  CsmaIpv4Topology::AddIpv4Address (n2, nd2, Ipv4Address("10.1.1.3"), 
    Ipv4Mask("255.255.255.0"));
  
  CsmaIpv4Topology::AddIpv4Address (n3, nd3, Ipv4Address("10.1.1.4"), 
    Ipv4Mask("255.255.255.0"));
//
// Create an OnOff application to send UDP datagrams from node zero to node 1.
//
  NS_DEBUG("Create Applications.");
  Ptr<OnOffApplication> ooff = Create<OnOffApplication> (
    n0, 
    InetSocketAddress ("10.1.1.2", 80), 
    "Udp",
    ConstantVariable(1), 
    ConstantVariable(0));
//
// Tell the application when to start and stop.
//
  ooff->Start(Seconds(1.0));
  ooff->Stop (Seconds(10.0));
// 
// Create a similar flow from n3 to n0, starting at time 1.1 seconds
//
  ooff = Create<OnOffApplication> (
    n3, 
    InetSocketAddress ("10.1.1.1", 80), 
    "Udp",
    ConstantVariable(1), 
    ConstantVariable(0));

  ooff->Start(Seconds(1.1));
  ooff->Stop (Seconds(10.0));
//
// Configure tracing of all enqueue, dequeue, and NetDevice receive events.
// Trace output will be sent to the file "csma-one-subnet.tr"
//
   NS_DEBUG("Configure Tracing.");
  AsciiTrace asciitrace ("csma-one-subnet.tr");
  asciitrace.TraceAllNetDeviceRx ();
  asciitrace.TraceAllQueues ();
//
// Also configure some tcpdump traces; each interface will be traced.
// The output files will be named:
//     csma-one-subnet.pcap-<nodeId>-<interfaceId>
// and can be read by the "tcpdump -r" command (use "-tt" option to
// display timestamps correctly)
//
  PcapTrace pcaptrace ("csma-one-subnet.pcap");
  pcaptrace.TraceAllIp ();
//
// Now, do the actual simulation.
//
  NS_DEBUG("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_DEBUG("Done.");
}
