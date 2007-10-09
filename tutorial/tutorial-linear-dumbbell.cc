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
#include "ns3/csma-channel.h"
#include "ns3/mac48-address.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/csma-net-device.h"
#include "ns3/point-to-point-topology.h"
#include "ns3/csma-topology.h"
#include "ns3/csma-ipv4-topology.h"
#include "ns3/udp-echo-client.h"
#include "ns3/udp-echo-server.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"
#include "ns3/global-route-manager.h"

NS_LOG_COMPONENT_DEFINE ("DumbbellSimulation");

using namespace ns3;

// Network topology
//
//                       point to point
//                      +--------------+
//                      |              |
//       n0   n1   n2   n3             n4   n5   n6   n7
//       |    |    |    |              |    |    |    |
//       ================              ================
//             lan1                          lan2
//
int 
main (int argc, char *argv[])
{
  LogComponentEnable ("DumbbellSimulation", LOG_LEVEL_INFO);
//  LogComponentEnableAll (LOG_LEVEL_ALL, LOG_DECORATE_ALL);

  NS_LOG_INFO ("Dumbbell Topology Simulation");

  Ptr<Node> n0 = Create<InternetNode> ();
  Ptr<Node> n1 = Create<InternetNode> ();
  Ptr<Node> n2 = Create<InternetNode> ();
  Ptr<Node> n3 = Create<InternetNode> ();

  Ptr<CsmaChannel> lan1 = 
    CsmaTopology::CreateCsmaChannel (DataRate (10000000), MilliSeconds (2));

  uint32_t nd0 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n0, lan1, 
    "08:00:2e:00:00:00");

  uint32_t nd1 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n1, lan1, 
    "08:00:2e:00:00:01");

  uint32_t nd2 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n2, lan1, 
    "08:00:2e:00:00:02");

  uint32_t nd3 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n3, lan1, 
    "08:00:2e:00:00:03");

  CsmaIpv4Topology::AddIpv4Address (n0, nd0, "10.1.1.1", "255.255.255.0");
  CsmaIpv4Topology::AddIpv4Address (n1, nd1, "10.1.1.2", "255.255.255.0");
  CsmaIpv4Topology::AddIpv4Address (n2, nd2, "10.1.1.3", "255.255.255.0");
  CsmaIpv4Topology::AddIpv4Address (n3, nd3, "10.1.1.4", "255.255.255.0");

  Ptr<Node> n4 = Create<InternetNode> ();
  Ptr<Node> n5 = Create<InternetNode> ();
  Ptr<Node> n6 = Create<InternetNode> ();
  Ptr<Node> n7 = Create<InternetNode> ();

  Ptr<CsmaChannel> lan2 = 
    CsmaTopology::CreateCsmaChannel (DataRate (10000000), MilliSeconds (2));

  uint32_t nd4 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n4, lan2, 
    "08:00:2e:00:00:04");

  uint32_t nd5 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n5, lan2, 
    "08:00:2e:00:00:05");

  uint32_t nd6 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n6, lan2, 
    "08:00:2e:00:00:06");

  uint32_t nd7 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n7, lan2, 
    "08:00:2e:00:00:07");

  CsmaIpv4Topology::AddIpv4Address (n4, nd4, "10.1.2.1", "255.255.255.0");
  CsmaIpv4Topology::AddIpv4Address (n5, nd5, "10.1.2.2", "255.255.255.0");
  CsmaIpv4Topology::AddIpv4Address (n6, nd6, "10.1.2.3", "255.255.255.0");
  CsmaIpv4Topology::AddIpv4Address (n7, nd7, "10.1.2.4", "255.255.255.0");

  Ptr<PointToPointChannel> link = PointToPointTopology::AddPointToPointLink (
    n3, n4, DataRate (38400), MilliSeconds (20));

  PointToPointTopology::AddIpv4Addresses (link, n3, "10.1.3.1", 
    n4, "10.1.3.2");

  uint16_t port = 7;

  Ptr<UdpEchoClient> client = Create<UdpEchoClient> (n0, "10.1.2.4", port, 
    1, Seconds(1.), 1024);

  Ptr<UdpEchoServer> server = Create<UdpEchoServer> (n7, port);

  server->Start(Seconds(1.));
  client->Start(Seconds(2.));

  server->Stop (Seconds(10.));
  client->Stop (Seconds(10.));

  AsciiTrace asciitrace ("tutorial-4.tr");
  asciitrace.TraceAllQueues ();
  asciitrace.TraceAllNetDeviceRx ();

  PcapTrace pcaptrace ("tutorial-4.pcap");
  pcaptrace.TraceAllIp ();

  GlobalRouteManager::PopulateRoutingTables ();

  Simulator::Run ();
  Simulator::Destroy ();
}
