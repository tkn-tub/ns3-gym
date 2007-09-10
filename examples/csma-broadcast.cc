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
//   n0 originates UDP broadcast to 255.255.255.255, which is replicated 
//   and received on both n1 and n2

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


int main (int argc, char *argv[])
{

  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
#if 0 
  DebugComponentEnable("CsmaNetDevice");
  DebugComponentEnable("Ipv4L3Protocol");
  DebugComponentEnable("NetDevice");
  DebugComponentEnable("Channel");
  DebugComponentEnable("CsmaChannel");
  DebugComponentEnable("PacketSocket");
#endif

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
  Ptr<Node> n0 = Create<InternetNode> ();
  Ptr<Node> n1 = Create<InternetNode> (); 
  Ptr<Node> n2 = Create<InternetNode> (); 

  // We create the channels first without any IP addressing information
  Ptr<CsmaChannel> channel0 = 
    CsmaTopology::CreateCsmaChannel(
      DataRate(5000000), MilliSeconds(2));

  // We create the channels first without any IP addressing information
  Ptr<CsmaChannel> channel1 = 
    CsmaTopology::CreateCsmaChannel(
      DataRate(5000000), MilliSeconds(2));

  uint32_t n0ifIndex0 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n0, channel0, 
                                         Eui48Address("10:54:23:54:0:50"));
  uint32_t n0ifIndex1 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n0, channel1, 
                                         Eui48Address("10:54:23:54:0:51"));
  uint32_t n1ifIndex = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n1, channel0,
                                         Eui48Address("10:54:23:54:23:51"));
  uint32_t n2ifIndex = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n2, channel1,
                                         Eui48Address("10:54:23:54:23:52"));

  // Later, we add IP addresses.  
  CsmaIpv4Topology::AddIpv4Address (
      n0, n0ifIndex0, Ipv4Address("10.1.0.1"), Ipv4Mask("255.255.0.0"));

  CsmaIpv4Topology::AddIpv4Address (
      n1, n1ifIndex, Ipv4Address("10.1.0.2"), Ipv4Mask("255.255.0.0"));

  CsmaIpv4Topology::AddIpv4Address (
      n0, n0ifIndex1, Ipv4Address("192.168.1.1"), Ipv4Mask("255.255.255.0"));
  
  CsmaIpv4Topology::AddIpv4Address (
      n2, n2ifIndex, Ipv4Address("192.168.1.2"), Ipv4Mask("255.255.255.0"));

  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  // from n0 to n1
  Ptr<OnOffApplication> ooff = Create<OnOffApplication> (
    n0, 
    InetSocketAddress ("255.255.255.255", 80), 
    "Udp",
    ConstantVariable(1), 
    ConstantVariable(0));
  // Start the application
  ooff->Start(Seconds(1.0));
  ooff->Stop (Seconds(10.0));

 
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

  Simulator::Run ();
    
  Simulator::Destroy ();
}
