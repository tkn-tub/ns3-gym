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

#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/internet-node.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/mac48-address.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-topology.h"
#include "ns3/udp-echo-client.h"
#include "ns3/udp-echo-server.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"
#include "ns3/global-route-manager.h"

NS_LOG_COMPONENT_DEFINE ("PointToPointSimulation");

using namespace ns3;

// Network topology
//
//                       point to point
//                      +--------------+
//                      |              |
//                     n0             n1
//
int 
main (int argc, char *argv[])
{
  LogComponentEnable ("PointToPointSimulation", LOG_LEVEL_INFO);

  NS_LOG_INFO ("Point to Point Topology Simulation");

  Ptr<Node> n0 = Create<InternetNode> ();
  Ptr<Node> n1 = Create<InternetNode> ();

  Ptr<PointToPointChannel> link = PointToPointTopology::AddPointToPointLink (
    n0, n1, DataRate (38400), MilliSeconds (20));

  PointToPointTopology::AddIpv4Addresses (link, n0, "10.1.1.1", 
    n1, "10.1.1.2");

  uint16_t port = 7;

  Ptr<UdpEchoClient> client = Create<UdpEchoClient> (n0, "10.1.1.2", port, 
    1, Seconds(1.), 1024);

  Ptr<UdpEchoServer> server = Create<UdpEchoServer> (n1, port);

  server->Start(Seconds(1.));
  client->Start(Seconds(2.));

  server->Stop (Seconds(10.));
  client->Stop (Seconds(10.));

  AsciiTrace asciitrace ("tutorial.tr");
  asciitrace.TraceAllQueues ();
  asciitrace.TraceAllNetDeviceRx ();

  Simulator::Run ();
  Simulator::Destroy ();
}
