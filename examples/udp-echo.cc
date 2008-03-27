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

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/helper-module.h"
#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"

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
  NodeContainer n;
  n.Create (4);

  InternetStackHelper internet;
  internet.Build (n);

  NS_LOG_INFO ("Create channels.");
//
// Explicitly create the channels required by the topology (shown above).
//
  CsmaHelper csma;
  csma.SetChannelParameter ("BitRate", DataRate(5000000));
  csma.SetChannelParameter ("Delay", MilliSeconds (2));
  NetDeviceContainer d = csma.Build (n);

  Ipv4AddressHelper ipv4;
//
// We've got the "hardware" in place.  Now we need to add IP addresses.
//
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Allocate (d);

  NS_LOG_INFO ("Create Applications.");
//
// Create a UdpEchoServer application on node one.
//
  uint16_t port = 9;  // well-known echo port number
  UdpEchoServerHelper server;
  server.SetPort (port);
  ApplicationContainer apps = server.Build (n.Get(1));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

//
// Create a UdpEchoClient application to send UDP datagrams from node zero to
// node one.
//
  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 1;
  Time interPacketInterval = Seconds (1.);
  UdpEchoClientHelper client;
  client.SetRemote (i.GetAddress (1), port);
  client.SetAppAttribute ("MaxPackets", Uinteger (maxPacketCount));
  client.SetAppAttribute ("Interval", interPacketInterval);
  client.SetAppAttribute ("PacketSize", Uinteger (packetSize));
  apps = client.Build (n.Get (0));
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (10.0));

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
