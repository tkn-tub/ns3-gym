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
#if 1
  LogComponentEnable ("NamesExample", LOG_LEVEL_INFO);
#endif

  CommandLine cmd;
  cmd.Parse (argc, argv);

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
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (d);

  uint16_t port = 9;
  UdpEchoServerHelper server (port);
  //
  // Install the UdpEchoServer application on the server node using its name
  // directly.
  //
  ApplicationContainer apps = server.Install ("/Names/server");
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 1;
  Time interPacketInterval = Seconds (1.);
  UdpEchoClientHelper client (i.GetAddress (1), port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  //
  // Install the UdpEchoClient application on the server node using its name
  // directly.
  //
  apps = client.Install ("/Names/client");
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (10.0));

  //
  // Use the config system to connect a trace source using the object name
  // system to specify the path.
  //
  Config::Connect ("/Names/client/eth0/Rx", MakeCallback (&RxEvent));

  Simulator::Run ();
  Simulator::Destroy ();
}
