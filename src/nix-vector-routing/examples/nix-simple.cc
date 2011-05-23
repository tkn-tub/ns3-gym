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

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/ipv4-nix-vector-helper.h"

/*
 *  Simple point to point links:
 *
 *  n0 -- n1 -- n2 -- n3
 *
 *  n0 has UdpEchoClient 
 *  n3 has UdpEchoServer
 *
 *  n0 IP: 10.1.1.1
 *  n1 IP: 10.1.1.2, 10.1.2.1
 *  n2 IP: 10.1.2.2, 10.1.3.1
 *  n3 IP: 10.1.3.2
 *
 */

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("NixSimpleExample");

int
main (int argc, char *argv[])
{
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes12;
  nodes12.Create (2);

  NodeContainer nodes23;
  nodes23.Add (nodes12.Get (1));
  nodes23.Create (1);

  NodeContainer nodes34;
  nodes34.Add (nodes23.Get (1));
  nodes34.Create (1);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NodeContainer allNodes = NodeContainer (nodes12, nodes23.Get (1), nodes34.Get (1));

  // NixHelper to install nix-vector routing
  // on all nodes
  Ipv4NixVectorHelper nixRouting;
  Ipv4StaticRoutingHelper staticRouting;

  Ipv4ListRoutingHelper list;
  list.Add (staticRouting, 0);
  list.Add (nixRouting, 10);

  InternetStackHelper stack;
  stack.SetRoutingHelper (list); // has effect on the next Install ()
  stack.Install (allNodes);

  NetDeviceContainer devices12;
  NetDeviceContainer devices23;
  NetDeviceContainer devices34;
  devices12 = pointToPoint.Install (nodes12);
  devices23 = pointToPoint.Install (nodes23);
  devices34 = pointToPoint.Install (nodes34);

  Ipv4AddressHelper address1;
  address1.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4AddressHelper address2;
  address2.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4AddressHelper address3;
  address3.SetBase ("10.1.3.0", "255.255.255.0");

  address1.Assign (devices12);
  address2.Assign (devices23);
  Ipv4InterfaceContainer interfaces = address3.Assign (devices34);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes34.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes12.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  // Trace routing tables
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("nix-simple.routes", std::ios::out);
  nixRouting.PrintRoutingTableAllAt (Seconds (8), routingStream);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
