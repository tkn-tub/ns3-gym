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
#include "ns3/udp-echo-client.h"
#include "ns3/udp-echo-server.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/ascii-trace.h"
#include "ns3/pcap-trace.h"
#include "ns3/global-route-manager.h"

#include "point-to-point-ipv4-topology.h"

NS_LOG_COMPONENT_DEFINE ("StarSimulation");

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
  LogComponentEnable ("StarSimulation", LOG_LEVEL_INFO);

  NS_LOG_INFO ("Star Topology Simulation");

  Ptr<Node> n0 = CreateObject<InternetNode> ();
  Ptr<Node> n1 = CreateObject<InternetNode> ();
  Ptr<Node> n2 = CreateObject<InternetNode> ();
  Ptr<Node> n3 = CreateObject<InternetNode> ();
  Ptr<Node> n4 = CreateObject<InternetNode> ();
  Ptr<Node> n5 = CreateObject<InternetNode> ();
  Ptr<Node> n6 = CreateObject<InternetNode> ();

  Ptr<PointToPointChannel> link01 = 
    PointToPointIpv4Topology::CreateChannel (DataRate (38400), 
    MilliSeconds (20));

  uint32_t nd01 = PointToPointIpv4Topology::AddNetDevice (n0,
    link01);

  Ptr<PointToPointChannel> link02 = 
    PointToPointIpv4Topology::CreateChannel (DataRate (38400), 
    MilliSeconds (20));

  uint32_t nd02 = PointToPointIpv4Topology::AddNetDevice (n0,
    link02);

  Ptr<PointToPointChannel> link03 = 
    PointToPointIpv4Topology::CreateChannel (DataRate (38400), 
    MilliSeconds (20));

  uint32_t nd03 = PointToPointIpv4Topology::AddNetDevice (n0,
    link03);

  Ptr<PointToPointChannel> link04 = 
    PointToPointIpv4Topology::CreateChannel (DataRate (38400), 
    MilliSeconds (20));

  uint32_t nd04 = PointToPointIpv4Topology::AddNetDevice (n0, 
    link04);

  Ptr<PointToPointChannel> link05 = 
    PointToPointIpv4Topology::CreateChannel (DataRate (38400), 
    MilliSeconds (20));

  uint32_t nd05 = PointToPointIpv4Topology::AddNetDevice (n0,
    link05);

  Ptr<PointToPointChannel> link06 = 
    PointToPointIpv4Topology::CreateChannel (DataRate (38400), 
    MilliSeconds (20));

  uint32_t nd06 = PointToPointIpv4Topology::AddNetDevice (n0, link06);

  uint32_t nd1 = PointToPointIpv4Topology::AddNetDevice (n1, link01);
  uint32_t nd2 = PointToPointIpv4Topology::AddNetDevice (n2, link02);
  uint32_t nd3 = PointToPointIpv4Topology::AddNetDevice (n3, link03);
  uint32_t nd4 = PointToPointIpv4Topology::AddNetDevice (n4, link04);
  uint32_t nd5 = PointToPointIpv4Topology::AddNetDevice (n5, link05);
  uint32_t nd6 = PointToPointIpv4Topology::AddNetDevice (n6, link06);

  PointToPointIpv4Topology::AddAddress (n0, nd01, "10.1.1.1", 
    "255.255.255.252");

  PointToPointIpv4Topology::AddAddress (n1, nd1, "10.1.1.2", 
    "255.255.255.252");

  PointToPointIpv4Topology::AddAddress (n0, nd02, "10.1.2.1", 
    "255.255.255.252");

  PointToPointIpv4Topology::AddAddress (n2, nd2, "10.1.2.2", 
    "255.255.255.252");

  PointToPointIpv4Topology::AddAddress (n0, nd03, "10.1.3.1", 
    "255.255.255.252");

  PointToPointIpv4Topology::AddAddress (n3, nd3, "10.1.2.2", 
    "255.255.255.252");

  PointToPointIpv4Topology::AddAddress (n0, nd04, "10.1.4.1", 
    "255.255.255.252");

  PointToPointIpv4Topology::AddAddress (n4, nd4, "10.1.4.2", 
    "255.255.255.252");

  PointToPointIpv4Topology::AddAddress (n0, nd05, "10.1.5.1", 
    "255.255.255.252");

  PointToPointIpv4Topology::AddAddress (n5, nd5, "10.1.5.2", 
    "255.255.255.252");

  PointToPointIpv4Topology::AddAddress (n0, nd06, "10.1.6.1", 
    "255.255.255.252");

  PointToPointIpv4Topology::AddAddress (n6, nd6, "10.1.6.2", 
    "255.255.255.252");

  uint16_t port = 7;

  Ptr<UdpEchoClient> client = CreateObject<UdpEchoClient> (n0, "10.1.1.2", port, 
    1, Seconds(1.), 1024);

  Ptr<UdpEchoServer> server = CreateObject<UdpEchoServer> (n1, port);

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
