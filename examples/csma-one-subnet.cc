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

#include <iostream>
#include <fstream>

#include "ns3/simulator-module.h"
#include "ns3/core-module.h"
#include "ns3/helper-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CsmaOneSubnetExample");

int 
main (int argc, char *argv[])
{
//
// Users may find it convenient to turn on explicit debugging
// for selected modules; the below lines suggest how to do this
//
#if 0 
  LogComponentEnable ("CsmaOneSubnetExample", LOG_LEVEL_INFO);
#endif
//
// Make the random number generators generate reproducible results.
//
  RandomVariable::UseGlobalSeed (1, 1, 2, 3, 5, 8);
//
// Allow the user to override any of the defaults and the above Bind() at
// run-time, via command-line arguments
//
  CommandLine cmd;
  cmd.Parse (argc, argv);
//
// Explicitly create the nodes required by the topology (shown above).
//
  NS_LOG_INFO ("Create nodes.");
  NodeContainer c;
  c.Create (4);

  NS_LOG_INFO ("Build Topology");
  CsmaHelper csma;
  csma.SetChannelParameter ("BitRate", DataRate (5000000));
  csma.SetChannelParameter ("Delay", MilliSeconds (2));
//
// Now fill out the topology by creating the net devices required to connect
// the nodes to the channels and hooking them up.  AddIpv4CsmaNetDevice will
// create a net device, add a MAC address (in memory of the pink flamingo) and
// connect the net device to a nodes and also to a channel. the 
// AddIpv4CsmaNetDevice method returns a net device index for the net device
// created on the node.  Interpret nd0 as the net device we created for node
// zero.
//
  NetDeviceContainer nd0 = csma.Install (c);

  InternetStackHelper internet;
  internet.Install (c);

// We've got the "hardware" in place.  Now we need to add IP addresses.
//
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  ipv4.Assign (nd0);

//
// Create an OnOff application to send UDP datagrams from node zero to node 1.
//
  NS_LOG_INFO ("Create Applications.");
  uint16_t port = 9;   // Discard port (RFC 863)

  OnOffHelper onoff;
  onoff.SetUdpRemote (Ipv4Address ("10.1.1.2"), port);
  onoff.SetAppAttribute ("OnTime", ConstantVariable (1));
  onoff.SetAppAttribute ("OffTime", ConstantVariable (0));

  ApplicationContainer app = onoff.Install (c.Get (0));
  // Start the application
  app.Start (Seconds (1.0));
  app.Stop (Seconds (10.0));

  // Create an optional packet sink to receive these packets
  PacketSinkHelper sink;
  sink.SetUdpLocal (Ipv4Address::GetAny (), port);
  sink.Install (c.Get (1));

// 
// Create a similar flow from n3 to n0, starting at time 1.1 seconds
//
  onoff.SetUdpRemote (Ipv4Address("10.1.1.1"), port);
  ApplicationContainer app2 = onoff.Install (c.Get (3));

  sink.Install (c.Get (0));

  app2.Start(Seconds (1.1));
  app2.Stop (Seconds (10.0));
//
// Configure tracing of all enqueue, dequeue, and NetDevice receive events.
// Trace output will be sent to the file "csma-one-subnet.tr"
//
  NS_LOG_INFO ("Configure Tracing.");
  std::ofstream ascii;
  ascii.open ("csma-one-subnet.tr");
  CsmaHelper::EnableAscii (ascii);
//
// Also configure some tcpdump traces; each interface will be traced.
// The output files will be named:
//     csma-one-subnet.pcap-<nodeId>-<interfaceId>
// and can be read by the "tcpdump -r" command (use "-tt" option to
// display timestamps correctly)
//
  CsmaHelper::EnablePcap ("csma-one-subnet");
//
// Now, do the actual simulation.
//
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
