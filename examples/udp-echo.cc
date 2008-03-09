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
// - UDP flows from n0 to n1 and back
// - DropTail queues 
// - Tracing of queues and packet receptions to file "udp-echo.tr"

#include "ns3/command-line.h"
#include "ns3/default-value.h"
#include "ns3/ptr.h"
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
#include "ns3/udp-echo-client.h"
#include "ns3/udp-echo-server.h"
#include "ns3/uinteger.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("UdpEchoExample");

int 
main (int argc, char *argv[])
{
//
// Users may find it convenient to turn on explicit debugging
// for selected modules; the below lines suggest how to do this
//
#if 0
  LogComponentEnable ("UdpEchoExample", LOG_LEVEL_INFO);

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

// Allow the user to override any of the defaults and the above Bind() at
// run-time, via command-line arguments
//
  CommandLine cmd;
  cmd.Parse (argc, argv);
//
// Explicitly create the nodes required by the topology (shown above).
//
  NS_LOG_INFO ("Create nodes.");
  Ptr<Node> n0 = CreateObject<InternetNode> ();
  Ptr<Node> n1 = CreateObject<InternetNode> (); 
  Ptr<Node> n2 = CreateObject<InternetNode> (); 
  Ptr<Node> n3 = CreateObject<InternetNode> ();

  NS_LOG_INFO ("Create channels.");
//
// Explicitly create the channels required by the topology (shown above).
//
  Ptr<CsmaChannel> lan = CsmaTopology::CreateCsmaChannel(
    DataRate(5000000), MilliSeconds(2));

  NS_LOG_INFO ("Build Topology.");
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
    Mac48Address("08:00:2e:00:00:00"));

  uint32_t nd1 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n1, lan, 
    Mac48Address("08:00:2e:00:00:01"));

  uint32_t nd2 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n2, lan, 
    Mac48Address("08:00:2e:00:00:02"));

  uint32_t nd3 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n3, lan, 
    Mac48Address("08:00:2e:00:00:03"));
//
// We've got the "hardware" in place.  Now we need to add IP addresses.
//
  NS_LOG_INFO ("Assign IP Addresses.");
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

  NS_LOG_INFO ("Create Applications.");
//
// Create a UdpEchoServer application on node one.
//
  uint16_t port = 9;  // well-known echo port number

  Ptr<UdpEchoServer> server = CreateObjectWith<UdpEchoServer> ("Node", n1, 
                                                               "Port", Uinteger (port));
  n1->AddApplication (server);
//
// Create a UdpEchoClient application to send UDP datagrams from node zero to
// node one.
//
  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 1;
  Time interPacketInterval = Seconds (1.);

  Ptr<UdpEchoClient> client = 
    CreateObjectWith<UdpEchoClient> ("Node", n0, 
                                     "RemoteIpv4", Ipv4Address ("10.1.1.2"),
                                     "RemotePort", Uinteger (port),
                                     "MaxPackets", Uinteger (maxPacketCount), 
                                     "Interval", interPacketInterval, 
                                     "PacketSize", Uinteger (packetSize));
  n0->AddApplication (client);
//
// Tell the applications when to start and stop.
//
  server->Start(Seconds(1.));
  client->Start(Seconds(2.));

  server->Stop (Seconds(10.));
  client->Stop (Seconds(10.));
//
// Configure tracing of all enqueue, dequeue, and NetDevice receive events.
// Trace output will be sent to the file "udp-echo.tr"
//
  NS_LOG_INFO ("Configure Tracing.");
  AsciiTrace asciitrace ("udp-echo.tr");
  asciitrace.TraceAllNetDeviceRx ();
  asciitrace.TraceAllQueues ();
//
// Also configure some tcpdump traces; each interface will be traced.
// The output files will be named:
//     udp-echo.pcap-<nodeId>-<interfaceId>
// and can be read by the "tcpdump -r" command (use "-tt" option to
// display timestamps correctly)
//
  PcapTrace pcaptrace ("udp-echo.pcap");
  pcaptrace.TraceAllIp ();
//
// Now, do the actual simulation.
//
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
