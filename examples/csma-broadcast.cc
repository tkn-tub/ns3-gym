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

//
// Example of the sending of a datagram to a broadcast address
//
// Network topology
//     ==============
//       |          |
//       n0    n1   n2   
//       |     |       
//     ==========
//
//   n0 originates UDP broadcast to 255.255.255.255/discard port, which 
//   is replicated and received on both n1 and n2

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

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

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CsmaBroadcastExample");

int 
main (int argc, char *argv[])
{
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
#if 0
  LogComponentEnable ("CsmaBroadcastExample", LOG_LEVEL_INFO);

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
  // Enable the below logging command to see the packets being received
  LogComponentEnable("PacketSinkApplication", LOG_LEVEL_ALL);
#endif

  // Set up some default values for the simulation.  Use the Bind()

  // Allow the user to override any of the defaults and the above
  // Bind()s at run-time, via command-line arguments
  CommandLine cmd;
  cmd.Parse (argc, argv);

  // Here, we will explicitly create four nodes.  In more sophisticated
  // topologies, we could configure a node factory.
  NS_LOG_INFO ("Create nodes.");
  Ptr<Node> n0 = CreateObject<InternetNode> ();
  Ptr<Node> n1 = CreateObject<InternetNode> (); 
  Ptr<Node> n2 = CreateObject<InternetNode> (); 

  // We create the channels first without any IP addressing information
  NS_LOG_INFO ("Create channels.");
  Ptr<CsmaChannel> channel0 = 
    CsmaTopology::CreateCsmaChannel(
      DataRate(5000000), MilliSeconds(2));

  // We create the channels first without any IP addressing information
  Ptr<CsmaChannel> channel1 = 
    CsmaTopology::CreateCsmaChannel(
      DataRate(5000000), MilliSeconds(2));

  NS_LOG_INFO ("Build Topology.");
  uint32_t n0ifIndex0 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n0, channel0, 
                                         Mac48Address("10:54:23:54:0:50"));
  uint32_t n0ifIndex1 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n0, channel1, 
                                         Mac48Address("10:54:23:54:0:51"));
  uint32_t n1ifIndex = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n1, channel0,
                                         Mac48Address("10:54:23:54:23:51"));
  uint32_t n2ifIndex = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n2, channel1,
                                         Mac48Address("10:54:23:54:23:52"));

  // Later, we add IP addresses.  
  NS_LOG_INFO ("Assign IP Addresses.");
  CsmaIpv4Topology::AddIpv4Address (
      n0, n0ifIndex0, Ipv4Address("10.1.0.1"), Ipv4Mask("255.255.0.0"));

  CsmaIpv4Topology::AddIpv4Address (
      n1, n1ifIndex, Ipv4Address("10.1.0.2"), Ipv4Mask("255.255.0.0"));

  CsmaIpv4Topology::AddIpv4Address (
      n0, n0ifIndex1, Ipv4Address("192.168.1.1"), Ipv4Mask("255.255.255.0"));
  
  CsmaIpv4Topology::AddIpv4Address (
      n2, n2ifIndex, Ipv4Address("192.168.1.2"), Ipv4Mask("255.255.255.0"));

  // RFC 863 discard port ("9") indicates packet should be thrown away
  // by the system.  We allow this silent discard to be overridden
  // by the PacketSink application.
  uint16_t port = 9;

  // Create the OnOff application to send UDP datagrams of size
  // 512 bytes (default) at a rate of 500 Kb/s (default) from n0
  NS_LOG_INFO ("Create Applications.");
  Ptr<OnOffApplication> ooff = 
    CreateObjectWith<OnOffApplication> (
                                        "Node", n0, 
                                        "Remote", Address (InetSocketAddress ("255.255.255.255", port)), 
                                        "Protocol", TypeId::LookupByName ("Udp"),
                                        "OnTime", ConstantVariable(1), 
                                        "OffTime", ConstantVariable(0));
  n0->AddApplication (ooff);
  // Start the application
  ooff->Start(Seconds(1.0));
  ooff->Stop (Seconds(10.0));
  
  // Create an optional packet sink to receive these packets
  Ptr<PacketSink> sink = 
    CreateObjectWith<PacketSink> (
                                  "Node", n1,
                                  "Local", Address (InetSocketAddress (Ipv4Address::GetAny (), port)),
                                  "Protocol", TypeId::LookupByName ("Udp"));
  n1->AddApplication (sink);
  // Start the sink
  sink->Start (Seconds (1.0));
  sink->Stop (Seconds (10.0));

  // Create an optional packet sink to receive these packets
  sink = CreateObjectWith<PacketSink> (
                                       "Node", n2,
                                       "Local", Address (InetSocketAddress (Ipv4Address::GetAny (), port)),
                                       "Protocol", TypeId::LookupByName ("Udp"));
  n2->AddApplication (sink);

  // Start the sink
  sink->Start (Seconds (1.0));
  sink->Stop (Seconds (10.0));

  NS_LOG_INFO ("Configure Tracing.");
  // Configure tracing of all enqueue, dequeue, and NetDevice receive events
  // Trace output will be sent to the csma-broadcast.tr file
  AsciiTrace asciitrace ("csma-broadcast.tr");
  asciitrace.TraceAllNetDeviceRx ();
  asciitrace.TraceAllQueues ();

  // Also configure some tcpdump traces; each interface will be traced
  // The output files will be named 
  // simple-point-to-point.pcap-<nodeId>-<interfaceId>
  // and can be read by the "tcpdump -r" command (use "-tt" option to
  // display timestamps correctly)
  PcapTrace pcaptrace ("csma-broadcast.pcap");
  pcaptrace.TraceAllIp ();

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();    
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
