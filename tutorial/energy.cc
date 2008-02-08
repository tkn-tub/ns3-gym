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

#include "energy-model.h"

NS_LOG_COMPONENT_DEFINE ("EnergyModelInterfaceExample");

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
  LogComponentEnable ("EnergyModelInterfaceExample", LOG_LEVEL_ALL);
  //  LogComponentEnable ("EnergyModel", LOG_LEVEL_ALL);

  NS_LOG_INFO ("Energy Model Interface Example");

  NS_LOG_INFO ("Creating Nodes");
  Ptr<Node> n0 = CreateObject<InternetNode> ();
  Ptr<Node> n1 = CreateObject<InternetNode> ();

  NS_LOG_INFO ("Creating Channel");
  Ptr<PointToPointChannel> link = PointToPointTopology::AddPointToPointLink (
    n0, n1, DataRate (38400), MilliSeconds (20));

  PointToPointTopology::AddIpv4Addresses (link, n0, "10.1.1.1", 
    n1, "10.1.1.2");

  NS_LOG_INFO ("Creating Applications");
  uint16_t port = 7;

  Ptr<UdpEchoClient> client = CreateObject<UdpEchoClient> (n0, "10.1.1.2", 
    port, 1, Seconds(1.), 1024);

  Ptr<UdpEchoServer> server = CreateObject<UdpEchoServer> (n1, port);

  server->Start(Seconds(1.));
  client->Start(Seconds(2.));

  server->Stop (Seconds(10.));
  client->Stop (Seconds(10.));

  NS_LOG_INFO ("Initializing Tracing");
  AsciiTrace asciitrace ("energy.tr");
  asciitrace.TraceAllQueues ();
  asciitrace.TraceAllNetDeviceRx ();
//  
// Pick a battery out of the air and use some somewhat real numbers found on
// data sheets on the web.
//
// 2 AAA battery (900 mAh * 2, with imaginary wireless device that uses
// 0.350 W at idle power, 0.025 W additional during receive, and 0.2 W 
// additional power during transmission (10 mW TX power).
//
  NS_LOG_INFO ("Initializing Energy Models");
  Ptr<EnergyModel> e0 = CreateObject<EnergyModel> (1.8, 1.5, 0.35, 0.025, 0.2);
  n0->AddInterface (e0);

  Ptr<EnergyModel> e1 = CreateObject<EnergyModel> (1.8, 1.5, 0.35, 0.025, 0.2);
  n1->AddInterface (e1);

#if 0
//
// As simulation progresses, the battereis draw idle power.  Down in the 
// net device, we will want to call DrawTransmitPower and DrawReceivePower
// as required.
//
// This is just some example code showing how to draw power and check 
// consumption.
//
  NS_LOG_INFO("Node zero energy: " << e0->GetCapacity (Seconds (0.)) << 
    " watt-seconds");
  NS_LOG_INFO("Node one energy: " << e1->GetCapacity (Seconds (0.)) << 
    " watt-seconds");

  e0->DrawTransmitPower (Seconds (0.1));
  e1->DrawReceivePower (Seconds (0.1));
  e1->DrawTransmitPower (Seconds (0.1));
  e0->DrawReceivePower (Seconds (0.1));

  NS_LOG_INFO("Node zero energy: " << e0->GetCapacity (Seconds (10.)) << 
    " watt-seconds");
  NS_LOG_INFO("Node one energy: " << e1->GetCapacity (Seconds (10.)) << 
    " watt-seconds");
#endif

  Simulator::Run ();
  Simulator::Destroy ();
}
