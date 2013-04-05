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

#include <vector>
#include "ns3/boolean.h"
#include "ns3/config.h"
#include "ns3/csma-helper.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/inet-socket-address.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/on-off-helper.h"
#include "ns3/packet.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/packet-socket-address.h"
#include "ns3/csma-net-device.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/pointer.h"
#include "ns3/simple-channel.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/test.h"
#include "ns3/uinteger.h"
#include "ns3/ipv4-packet-info-tag.h"

using namespace ns3;

class DynamicGlobalRoutingTestCase : public TestCase
{
public:
  DynamicGlobalRoutingTestCase ();
  virtual ~DynamicGlobalRoutingTestCase ();

private:
  void SinkRx (std::string path, Ptr<const Packet> p, const Address &address);
  void HandleRead (Ptr<Socket>);
  virtual void DoRun (void);
  int m_count;
  std::vector<uint8_t> m_firstInterface;
  std::vector<uint8_t> m_secondInterface;
};

// Add some help text to this case to describe what it is intended to test
DynamicGlobalRoutingTestCase::DynamicGlobalRoutingTestCase ()
  : TestCase ("Dynamic global routing example"), m_count (0)
{
  m_firstInterface.resize (16);
  m_secondInterface.resize (16);
}

DynamicGlobalRoutingTestCase::~DynamicGlobalRoutingTestCase ()
{
}

void
DynamicGlobalRoutingTestCase::SinkRx (std::string path, Ptr<const Packet> p, const Address& address)
{
  Ipv4PacketInfoTag tag;
  bool found;
  found = p->PeekPacketTag (tag);
  uint8_t now = static_cast<uint8_t> (Simulator::Now ().GetSeconds ());
  if (found)
    {
      ;
    }
  m_firstInterface[now]++;
  m_count++;
}

void 
DynamicGlobalRoutingTestCase::HandleRead (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
    {
      if (packet->GetSize () == 0)
        { //EOF
          break;
        }
      Ipv4PacketInfoTag tag;
      bool found;
      found = packet->PeekPacketTag (tag);
      uint8_t now = static_cast<uint8_t> (Simulator::Now ().GetSeconds ());
      if (found)
        {
          if (tag.GetRecvIf () == 1)
            {
              m_firstInterface[now]++;
            }
          if (tag.GetRecvIf () == 2)
            {
              m_secondInterface[now]++;
            }
          m_count++;
        }
    }
}

// Test derived from examples/routing/dynamic-global-routing.cc
//
// Network topology
//
//  n0
//     \ p-p
//      \          (shared csma/cd)
//       n2 -------------------------n3
//      /            |        | 
//     / p-p        n4        n5 ---------- n6
//   n1                             p-p
//   |                                      |
//   ----------------------------------------
//                p-p
//
// Test that for node n6, the interface facing n5 receives packets at
// times (1-2), (4-6), (8-10), (11-12), (14-16) and the interface
// facing n1 receives packets at times (2-4), (6-8), (12-13)
//
void
DynamicGlobalRoutingTestCase::DoRun (void)
{
  // The below value configures the default behavior of global routing.
  // By default, it is disabled.  To respond to interface events, set to true
  Config::SetDefault ("ns3::Ipv4GlobalRouting::RespondToInterfaceEvents", BooleanValue (true));

  NodeContainer c;
  c.Create (7);
  NodeContainer n0n2 = NodeContainer (c.Get (0), c.Get (2));
  NodeContainer n1n2 = NodeContainer (c.Get (1), c.Get (2));
  NodeContainer n5n6 = NodeContainer (c.Get (5), c.Get (6));
  NodeContainer n1n6 = NodeContainer (c.Get (1), c.Get (6));
  NodeContainer n2345 = NodeContainer (c.Get (2), c.Get (3), c.Get (4), c.Get (5));

  InternetStackHelper internet;
  internet.Install (c);

  // We create the channels first without any IP addressing information
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer d0d2 = p2p.Install (n0n2);
  NetDeviceContainer d1d6 = p2p.Install (n1n6);

  NetDeviceContainer d1d2 = p2p.Install (n1n2);

  p2p.SetDeviceAttribute ("DataRate", StringValue ("1500kbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("10ms"));
  NetDeviceContainer d5d6 = p2p.Install (n5n6);

  // We create the channels first without any IP addressing information
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("5Mbps"));
  csma.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer d2345 = csma.Install (n2345);

  // Later, we add IP addresses.
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  ipv4.Assign (d0d2);

  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  ipv4.Assign (d1d2);

  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i5i6 = ipv4.Assign (d5d6);

  ipv4.SetBase ("10.250.1.0", "255.255.255.0");
  ipv4.Assign (d2345);

  ipv4.SetBase ("172.16.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i6 = ipv4.Assign (d1d6);

  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  uint16_t port = 9;   // Discard port (RFC 863)
  OnOffHelper onoff ("ns3::UdpSocketFactory",
                     InetSocketAddress (i5i6.GetAddress (1), port));
  onoff.SetConstantRate (DataRate ("2kbps"));
  onoff.SetAttribute ("PacketSize", UintegerValue (50));

  ApplicationContainer apps = onoff.Install (c.Get (1));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  // Create a second OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  OnOffHelper onoff2 ("ns3::UdpSocketFactory",
                      InetSocketAddress (i1i6.GetAddress (1), port));
  onoff2.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff2.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  onoff2.SetAttribute ("DataRate", StringValue ("2kbps"));
  onoff2.SetAttribute ("PacketSize", UintegerValue (50));

  ApplicationContainer apps2 = onoff2.Install (c.Get (1));
  apps2.Start (Seconds (11.0));
  apps2.Stop (Seconds (16.0));

  // Create an optional packet sink to receive these packets
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> sink2 = Socket::CreateSocket (c.Get (6), tid);
  sink2->Bind (Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
  sink2->Listen ();
  sink2->ShutdownSend ();

  sink2->SetRecvPktInfo (true);
  sink2->SetRecvCallback (MakeCallback (&DynamicGlobalRoutingTestCase::HandleRead, this));

  Ptr<Node> n1 = c.Get (1);
  Ptr<Ipv4> ipv41 = n1->GetObject<Ipv4> ();
  // The first ifIndex is 0 for loopback, then the first p2p is numbered 1,
  // then the next p2p is numbered 2
  uint32_t ipv4ifIndex1 = 2;

  // Trace receptions
  Config::Connect ("/NodeList/6/ApplicationList/*/$ns3::PacketSink/Rx",
                   MakeCallback (&DynamicGlobalRoutingTestCase::SinkRx, this));

  Simulator::Schedule (Seconds (2),&Ipv4::SetDown,ipv41, ipv4ifIndex1);
  Simulator::Schedule (Seconds (4),&Ipv4::SetUp,ipv41, ipv4ifIndex1);

  Ptr<Node> n6 = c.Get (6);
  Ptr<Ipv4> ipv46 = n6->GetObject<Ipv4> ();
  // The first ifIndex is 0 for loopback, then the first p2p is numbered 1,
  // then the next p2p is numbered 2
  uint32_t ipv4ifIndex6 = 2;
  Simulator::Schedule (Seconds (6),&Ipv4::SetDown,ipv46, ipv4ifIndex6);
  Simulator::Schedule (Seconds (8),&Ipv4::SetUp,ipv46, ipv4ifIndex6);

  Simulator::Schedule (Seconds (12),&Ipv4::SetDown,ipv41, ipv4ifIndex1);
  Simulator::Schedule (Seconds (14),&Ipv4::SetUp,ipv41, ipv4ifIndex1);

  Simulator::Run ();

  NS_TEST_ASSERT_MSG_EQ (m_count, 68, "Dynamic global routing did not deliver all packets");
// Test that for node n6, the interface facing n5 receives packets at
// times (1-2), (4-6), (8-10), (11-12), (14-16) and the interface
// facing n1 receives packets at times (2-4), (6-8), (12-13)
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[1], 4, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_secondInterface[2], 5, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_secondInterface[3], 5, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[4], 5, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[5], 5, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_secondInterface[6], 5, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_secondInterface[7], 5, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[8], 5, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[9], 5, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[10], 0, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[11], 4, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_secondInterface[12], 5, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_secondInterface[13], 5, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[14], 5, "Dynamic global routing did not deliver all packets");
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[15], 5, "Dynamic global routing did not deliver all packets");
  Simulator::Destroy ();
}

class GlobalRoutingSlash32TestCase : public TestCase
{
public:
  GlobalRoutingSlash32TestCase ();
  virtual ~GlobalRoutingSlash32TestCase ();

private:
  virtual void DoRun (void);
};

// Add some help text to this case to describe what it is intended to test
GlobalRoutingSlash32TestCase::GlobalRoutingSlash32TestCase ()
  : TestCase ("Slash 32 global routing example")
{
}

GlobalRoutingSlash32TestCase::~GlobalRoutingSlash32TestCase ()
{
}

// Test program for this 3-router scenario, using global routing
//
// (a.a.a.a/32)A<--x.x.x.0/30-->B<--y.y.y.0/30-->C(c.c.c.c/32)
//
void
GlobalRoutingSlash32TestCase::DoRun (void)
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
  Ptr<Ipv4> ipv4C = nC->GetObject<Ipv4> ();

  int32_t ifIndexA = ipv4A->AddInterface (deviceA);
  int32_t ifIndexC = ipv4C->AddInterface (deviceC);

  Ipv4InterfaceAddress ifInAddrA = Ipv4InterfaceAddress (Ipv4Address ("172.16.1.1"), Ipv4Mask ("255.255.255.255"));
  ipv4A->AddAddress (ifIndexA, ifInAddrA);
  ipv4A->SetMetric (ifIndexA, 1);
  ipv4A->SetUp (ifIndexA);

  Ipv4InterfaceAddress ifInAddrC = Ipv4InterfaceAddress (Ipv4Address ("192.168.1.1"), Ipv4Mask ("255.255.255.255"));
  ipv4C->AddAddress (ifIndexC, ifInAddrC);
  ipv4C->SetMetric (ifIndexC, 1);
  ipv4C->SetUp (ifIndexC);

  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  uint16_t port = 9;   // Discard port (RFC 863)
  OnOffHelper onoff ("ns3::UdpSocketFactory",
                     Address (InetSocketAddress (ifInAddrC.GetLocal (), port)));
  onoff.SetConstantRate (DataRate (6000));
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


class GlobalRoutingTestSuite : public TestSuite
{
public:
  GlobalRoutingTestSuite ();
};

GlobalRoutingTestSuite::GlobalRoutingTestSuite ()
  : TestSuite ("global-routing", UNIT)
{
  AddTestCase (new DynamicGlobalRoutingTestCase, TestCase::QUICK);
  AddTestCase (new GlobalRoutingSlash32TestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static GlobalRoutingTestSuite globalRoutingTestSuite;
