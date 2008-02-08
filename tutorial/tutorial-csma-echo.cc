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
#include "ns3/csma-channel.h"
#include "ns3/mac48-address.h"
#include "ns3/csma-net-device.h"
#include "ns3/csma-topology.h"
#include "ns3/csma-ipv4-topology.h"
#include "ns3/udp-echo-client.h"
#include "ns3/udp-echo-server.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"

NS_LOG_COMPONENT_DEFINE ("UdpEchoSimulation");

using namespace ns3;

int 
main (int argc, char *argv[])
{
  LogComponentEnable ("UdpEchoSimulation", LOG_LEVEL_INFO);

  NS_LOG_INFO ("UDP Echo Simulation");

  Ptr<Node> n0 = CreateObject<InternetNode> ();
  Ptr<Node> n1 = CreateObject<InternetNode> ();
  Ptr<Node> n2 = CreateObject<InternetNode> ();
  Ptr<Node> n3 = CreateObject<InternetNode> ();

  Ptr<CsmaChannel> lan = 
    CsmaTopology::CreateCsmaChannel (DataRate (5000000), MilliSeconds (2));

  uint32_t nd0 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n0, lan, 
    "08:00:2e:00:00:00");

  uint32_t nd1 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n1, lan, 
    "08:00:2e:00:00:01");

  uint32_t nd2 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n2, lan, 
    "08:00:2e:00:00:02");

  uint32_t nd3 = CsmaIpv4Topology::AddIpv4CsmaNetDevice (n3, lan, 
    "08:00:2e:00:00:03");

  CsmaIpv4Topology::AddIpv4Address (n0, nd0, "10.1.1.1", "255.255.255.0");
  CsmaIpv4Topology::AddIpv4Address (n1, nd1, "10.1.1.2", "255.255.255.0");
  CsmaIpv4Topology::AddIpv4Address (n2, nd2, "10.1.1.3", "255.255.255.0");
  CsmaIpv4Topology::AddIpv4Address (n3, nd3, "10.1.1.4", "255.255.255.0");

  uint16_t port = 7;

  Ptr<UdpEchoClient> client = CreateObject<UdpEchoClient> (n0, "10.1.1.2",
    port, 1, Seconds(1.), 1024);

  Ptr<UdpEchoServer> server = CreateObject<UdpEchoServer> (n1, port);

  server->Start(Seconds(1.));
  client->Start(Seconds(2.));

  server->Stop (Seconds(10.));
  client->Stop (Seconds(10.));

  Simulator::Run ();
  Simulator::Destroy ();
}
