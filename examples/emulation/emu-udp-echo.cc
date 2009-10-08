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
// Normally, the use case for emulated net devices is in collections of
// small simulations that connect to the outside world through specific 
// interfaces.  For example, one could construct a number of virtual
// machines and connect them via a host-only network.  To use the emulated
// net device, you would need to set all of the host-only interfaces in
// promiscuous mode and provide an appropriate device name (search for "eth1"
// below).  One could also use the emulated net device in a testbed situation
// where the host on which the simulation is running has a specific interface
// of interested.  You would also need to set this specific interface into
// promiscuous mode and provide an appropriate device name.
//
// This philosophy carries over to this simple example.
//
// We don't assume any special configuration and all of the ns-3 emulated net
// devices will actually talk to the same underlying OS device.  We rely on 
// the fact that the OS will deliver copies of our packets to the other ns-3
// net devices since we operate in promiscuous mode.  
//
// Packets will be sent out over the device, but we use MAC spoofing.  The
// MAC addresses will be generated using the Organizationally Unique Identifier
// (OUI) 00:00:00 as a base.  This vendor code is not assigned to any 
// organization and so should not conflict with any real hardware.  We'll use 
// the first n of these addresses, where n is the number of nodes, in this
// simualtion.  It is up to you to determine that using these MAC addresses is
// okay on your network and won't conflict with anything else (including another
// simulation using emu devices) on your network.  Once you have made this 
// determination, you need to put the interface you chose into promiscuous mode.
// We don't do it for you since you need to think about it first.
//
// This simulation uses the real-time simulator and so will consume ten seconds
// of real time.
//
// By default, we create the following topology
//
//       n0    n1   n2   n3
//       |     |    |    |
//       -----------------
//             "eth1"
//
// - UDP flows from n0 to n1 and back
// - DropTail queues 
// - Tracing of queues and packet receptions to file "udp-echo.tr"
// - pcap tracing on all devices
//

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/helper-module.h"
#include "ns3/emu-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("EmulatedUdpEchoExample");

int 
main (int argc, char *argv[])
{
  std::string deviceName ("eth1");
  uint32_t nNodes = 4;

  //
  // Allow the user to override any of the defaults at run-time, via command-line
  // arguments
  //
  CommandLine cmd;
  cmd.AddValue("deviceName", "device name", deviceName);
  cmd.AddValue("nNodes", "number of nodes to create (>= 2)", nNodes);
  cmd.Parse (argc, argv);

  GlobalValue::Bind ("SimulatorImplementationType", 
    StringValue ("ns3::RealtimeSimulatorImpl"));

  //
  // need at least two nodes
  //
  nNodes = nNodes < 2 ? 2 : nNodes;

  //
  // Explicitly create the nodes required by the topology (shown above).
  //
  NS_LOG_INFO ("Create nodes.");
  NodeContainer n;
  n.Create (nNodes);

  InternetStackHelper internet;
  internet.Install (n);

  //
  // Explicitly create the channels required by the topology (shown above).
  //
  NS_LOG_INFO ("Create channels.");
  EmuHelper emu;
  emu.SetAttribute ("DeviceName", StringValue (deviceName));
  NetDeviceContainer d = emu.Install (n);

  //
  // We've got the "hardware" in place.  Now we need to add IP addresses.
  //
  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (d);

  //
  // Create a UdpEchoServer application on node one.
  //
  NS_LOG_INFO ("Create Applications.");
  UdpEchoServerHelper server (9);
  ApplicationContainer apps = server.Install (n.Get(1));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  //
  // Create a UdpEchoClient application to send UDP datagrams from node zero to node one.
  //
  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 1;
  Time interPacketInterval = Seconds (1.);
  UdpEchoClientHelper client (i.GetAddress (1), 9);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  apps = client.Install (n.Get (0));
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (10.0));

  std::ofstream ascii;
  ascii.open ("emu-udp-echo.tr");
  EmuHelper::EnablePcapAll ("emu-udp-echo", true);

  //
  // Now, do the actual simulation.
  //
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
