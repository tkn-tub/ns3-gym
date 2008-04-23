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

#include <fstream>

#include "ns3/core-module.h"
#include "ns3/node-module.h"
#include "ns3/helper-module.h"
#include "ns3/simulator-module.h"

NS_LOG_COMPONENT_DEFINE ("StarRoutingSimulation");

using namespace ns3;

// Network topology
//
//                  n3    n2
//                   |   /
//                    | /
//              n4 --- n0 --- n1
//                    /  |
//                   /    |
//                  n5    n6

int 
main (int argc, char *argv[])
{
  LogComponentEnable ("StarRoutingSimulation", LOG_LEVEL_INFO);

  NS_LOG_INFO ("Star Topology with Routing Simulation");

  NodeContainer n;
  n.Create (7);
  NodeContainer n01 = NodeContainer (n.Get (0), n.Get (1));
  NodeContainer n02 = NodeContainer (n.Get (0), n.Get (2));
  NodeContainer n03 = NodeContainer (n.Get (0), n.Get (3));
  NodeContainer n04 = NodeContainer (n.Get (0), n.Get (4));
  NodeContainer n05 = NodeContainer (n.Get (0), n.Get (5));
  NodeContainer n06 = NodeContainer (n.Get (0), n.Get (6));

  InternetStackHelper internet;
  internet.Install (n);

  PointToPointHelper p2p;
  p2p.SetChannelParameter ("BitRate", DataRate (38400));
  p2p.SetChannelParameter ("Delay", MilliSeconds (20));

  NetDeviceContainer d01 = p2p.Install (n01);
  NetDeviceContainer d02 = p2p.Install (n02);
  NetDeviceContainer d03 = p2p.Install (n03);
  NetDeviceContainer d04 = p2p.Install (n04);
  NetDeviceContainer d05 = p2p.Install (n05);
  NetDeviceContainer d06 = p2p.Install (n06);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.252");
  Ipv4InterfaceContainer i01 = ipv4.Assign (d01);
  ipv4.SetBase ("10.1.2.0", "255.255.255.252");
  Ipv4InterfaceContainer i02 = ipv4.Assign (d02);
  ipv4.SetBase ("10.1.3.0", "255.255.255.252");
  Ipv4InterfaceContainer i03 = ipv4.Assign (d03);
  ipv4.SetBase ("10.1.4.0", "255.255.255.252");
  Ipv4InterfaceContainer i04 = ipv4.Assign (d04);
  ipv4.SetBase ("10.1.5.0", "255.255.255.252");
  Ipv4InterfaceContainer i05 = ipv4.Assign (d05);
  ipv4.SetBase ("10.1.6.0", "255.255.255.252");
  Ipv4InterfaceContainer i06 = ipv4.Assign (d06);

  uint16_t port = 7;

  UdpEchoServerHelper server;
  server.SetPort (port);
  ApplicationContainer apps = server.Install (n.Get (1));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  UdpEchoClientHelper client;
  client.SetRemote (i01.GetAddress (1), port);
  client.SetAppAttribute ("MaxPackets", Uinteger (1));
  client.SetAppAttribute ("Interval", Seconds (1.0));
  client.SetAppAttribute ("PacketSize", Uinteger (1024));
  apps = client.Install (n.Get (0));
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (10.0));

  std::ofstream ascii;
  ascii.open ("tutorial.tr");
  PointToPointHelper::EnableAsciiAll (ascii);

  Simulator::Run ();
  Simulator::Destroy ();
}
