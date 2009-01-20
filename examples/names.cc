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

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/helper-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("NamesExample");

void 
RxEvent (std::string context, Ptr<const Packet> packet)
{
  NS_LOG_INFO (context << " packet " << packet);
}

int 
main (int argc, char *argv[])
{
  //
  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
  //
#if 1
  LogComponentEnable ("NamesExample", LOG_LEVEL_INFO);
#endif

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
  NodeContainer n;
  n.Create (4);

  //
  // We're going to use the zeroth node in the container as the client, and
  // the first node as the server.  Add some "human readable" names for these
  // nodes.  The first parameter specifies the root of the "/Names" name space
  // as the destination, so these will go into the name system as "/Names/client"
  // and "/Names/server".  
  //
  Names::Add ("/Names", "client", n.Get (0));
  Names::Add ("/Names", "server", n.Get (1));

  InternetStackHelper internet;
  internet.Install (n);

  NS_LOG_INFO ("Create devices.");
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate(5000000)));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  csma.SetDeviceAttribute ("Mtu", UintegerValue (1400));
  NetDeviceContainer d = csma.Install (n);

  //
  // Add some human readable names for the devices we'll be interested in.
  // We add the names to the name space "under" the nodes we created above.
  // This has the effect of making "/Names/client/eth0" and "/Names/server/eth0"
  // Note that the first parameter must reference a previously named object,
  // and we have, in fact, already named objects "/Names/client" and
  // "/Names/server"
  //
  Names::Add ("/Names/client", "eth0", d.Get (0));
  Names::Add ("/Names/server", "eth0", d.Get (1));

  Ipv4AddressHelper ipv4;

  //
  // We've got the "hardware" in place.  Now we need to add IP addresses.
  //
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (d);

  NS_LOG_INFO ("Create Applications.");

  //
  // Create a UdpEchoServer application on the server node.  Note that we 
  // reference the server node by name in the Install method below.
  //
  uint16_t port = 9;  // well-known echo port number
  UdpEchoServerHelper server (port);
  ApplicationContainer apps = server.Install (Names::Find<Node> ("/Names/server"));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  //
  // Create a UdpEchoClient application to send UDP datagrams from node zero to
  // node one.  Notice that we reference the client node by name in the Install
  // method below.
  //
  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 1;
  Time interPacketInterval = Seconds (1.);
  UdpEchoClientHelper client (i.GetAddress (1), port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  apps = client.Install (Names::Find<Node> ("/Names/client"));
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (10.0));

  Config::Connect ("/Names/client/eth0/Rx", MakeCallback (&RxEvent));

  //
  // Now, do the actual simulation.
  //
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
