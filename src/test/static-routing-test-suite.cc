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

// End-to-end tests for Ipv4 static routing

#include "ns3/boolean.h"
#include "ns3/config.h"
#include "ns3/csma-helper.h"
#include "ns3/csma-net-device.h"
#include "ns3/inet-socket-address.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/on-off-helper.h"
#include "ns3/packet.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/packet-socket-address.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/pointer.h"
#include "ns3/random-variable.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/test.h"
#include "ns3/uinteger.h"

using namespace ns3;

class StaticRoutingSlash32TestCase : public TestCase
{
public:
  StaticRoutingSlash32TestCase ();
  virtual ~StaticRoutingSlash32TestCase ();

private:
  virtual void DoRun (void);
};

// Add some help text to this case to describe what it is intended to test
StaticRoutingSlash32TestCase::StaticRoutingSlash32TestCase ()
  : TestCase ("Slash 32 static routing example")
{
}

StaticRoutingSlash32TestCase::~StaticRoutingSlash32TestCase ()
{
}

// Test program for this 3-router scenario, using static routing
//
// (a.a.a.a/32)A<--x.x.x.0/30-->B<--y.y.y.0/30-->C(c.c.c.c/32)
//
void
StaticRoutingSlash32TestCase::DoRun (void)
{
  Ptr<Node> nA = CreateObject<Node> ();
  Ptr<Node> nB = CreateObject<Node> ();
  Ptr<Node> nC = CreateObject<Node> ();

  NodeContainer c = NodeContainer (nA, nB, nC);

  InternetStackHelper internet;
  internet.Install (c);

  // Point-to-point links
  NodeContainer nAnB = NodeContainer (nA, nB);
  NodeContainer nBnC = NodeContainer (nB, nC);

  // We create the channels first without any IP addressing information
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer dAdB = p2p.Install (nAnB);

  NetDeviceContainer dBdC = p2p.Install (nBnC);;

  Ptr<CsmaNetDevice> deviceA = CreateObject<CsmaNetDevice> ();
  deviceA->SetAddress (Mac48Address::Allocate ());
  nA->AddDevice (deviceA);

  Ptr<CsmaNetDevice> deviceC = CreateObject<CsmaNetDevice> ();
  deviceC->SetAddress (Mac48Address::Allocate ());
  nC->AddDevice (deviceC);

  // Later, we add IP addresses.
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.252");
  Ipv4InterfaceContainer iAiB = ipv4.Assign (dAdB);

  ipv4.SetBase ("10.1.1.4", "255.255.255.252");
  Ipv4InterfaceContainer iBiC = ipv4.Assign (dBdC);

  Ptr<Ipv4> ipv4A = nA->GetObject<Ipv4> ();
  Ptr<Ipv4> ipv4B = nB->GetObject<Ipv4> ();
  Ptr<Ipv4> ipv4C = nC->GetObject<Ipv4> ();

  int32_t ifIndexA = ipv4A->AddInterface (deviceA);
  int32_t ifIndexC = ipv4C->AddInterface (deviceC);

  Ipv4InterfaceAddress ifInAddrA = Ipv4InterfaceAddress (Ipv4Address ("172.16.1.1"), Ipv4Mask ("/32"));
  ipv4A->AddAddress (ifIndexA, ifInAddrA);
  ipv4A->SetMetric (ifIndexA, 1);
  ipv4A->SetUp (ifIndexA);

  Ipv4InterfaceAddress ifInAddrC = Ipv4InterfaceAddress (Ipv4Address ("192.168.1.1"), Ipv4Mask ("/32"));
  ipv4C->AddAddress (ifIndexC, ifInAddrC);
  ipv4C->SetMetric (ifIndexC, 1);
  ipv4C->SetUp (ifIndexC);
 
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  // Create static routes from A to C
  Ptr<Ipv4StaticRouting> staticRoutingA = ipv4RoutingHelper.GetStaticRouting (ipv4A);
  // The ifIndex for this outbound route is 1; the first p2p link added
  staticRoutingA->AddHostRouteTo (Ipv4Address ("192.168.1.1"), Ipv4Address ("10.1.1.2"), 1);
  Ptr<Ipv4StaticRouting> staticRoutingB = ipv4RoutingHelper.GetStaticRouting (ipv4B);
  // The ifIndex we want on node B is 2; 0 corresponds to loopback, and 1 to the first point to point link
  staticRoutingB->AddHostRouteTo (Ipv4Address ("192.168.1.1"), Ipv4Address ("10.1.1.6"), 2);
  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  uint16_t port = 9;   // Discard port (RFC 863)
  OnOffHelper onoff ("ns3::UdpSocketFactory", 
                     Address (InetSocketAddress (ifInAddrC.GetLocal (), port)));
  onoff.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
  onoff.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));
  onoff.SetAttribute ("DataRate", DataRateValue (DataRate (6000)));
  ApplicationContainer apps = onoff.Install (nA);
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  // Create a packet sink to receive these packets
  PacketSinkHelper sink ("ns3::UdpSocketFactory",
                         Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
  apps = sink.Install (nC);
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  Simulator::Run ();
  // Check that we received 13 * 512 = 6656 bytes
  Ptr<PacketSink> sinkPtr = DynamicCast <PacketSink> (apps.Get (0));
  NS_TEST_ASSERT_MSG_EQ (sinkPtr->GetTotalRx (), 6656, "Static routing with /32 did not deliver all packets");
  Simulator::Destroy ();
}

class StaticRoutingTestSuite : public TestSuite
{
public:
  StaticRoutingTestSuite ();
};

StaticRoutingTestSuite::StaticRoutingTestSuite ()
  : TestSuite ("static-routing", UNIT)
{
  AddTestCase (new StaticRoutingSlash32TestCase);
}

// Do not forget to allocate an instance of this TestSuite
static StaticRoutingTestSuite staticRoutingTestSuite;
