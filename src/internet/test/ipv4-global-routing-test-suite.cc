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
#include "ns3/inet-socket-address.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/packet.h"
#include "ns3/simple-net-device-helper.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/test.h"
#include "ns3/uinteger.h"
#include "ns3/ipv4-packet-info-tag.h"
#include "ns3/simple-net-device.h"
#include "ns3/simple-channel.h"
#include "ns3/socket-factory.h"
#include "ns3/udp-socket-factory.h"

using namespace ns3;

class Ipv4DynamicGlobalRoutingTestCase : public TestCase
{
public:
  Ipv4DynamicGlobalRoutingTestCase ();
  virtual ~Ipv4DynamicGlobalRoutingTestCase ();

private:
  void SendData (uint8_t index);
  void ShutDownSock (uint8_t index);
  void HandleRead (Ptr<Socket>);
  virtual void DoRun (void);

  int m_count;
  std::vector<std::pair<Ptr<Socket>, bool> > m_sendSocks;
  DataRate m_dataRate;
  uint16_t m_packetSize;
  std::vector<uint8_t> m_firstInterface;
  std::vector<uint8_t> m_secondInterface;
};

// Add some help text to this case to describe what it is intended to test
Ipv4DynamicGlobalRoutingTestCase::Ipv4DynamicGlobalRoutingTestCase ()
  : TestCase ("Dynamic global routing example"), m_count (0)
{
  m_firstInterface.resize (16);
  m_secondInterface.resize (16);
  m_dataRate = DataRate ("2kbps");
  m_packetSize = 50;
}

Ipv4DynamicGlobalRoutingTestCase::~Ipv4DynamicGlobalRoutingTestCase ()
{
  std::vector<std::pair<Ptr<Socket>, bool> >::iterator iter;

  for (iter = m_sendSocks.begin (); iter != m_sendSocks.end (); iter++)
    {
      if (iter->second)
        {
          iter->second = false;
          iter->first->Close ();
          iter->first = 0;
        }
    }
}

void 
Ipv4DynamicGlobalRoutingTestCase::HandleRead (Ptr<Socket> socket)
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

void
Ipv4DynamicGlobalRoutingTestCase::SendData (uint8_t index)
{
  if (m_sendSocks[index].second == false)
    {
      return;
    }
  Ptr<Packet> packet = Create<Packet> (m_packetSize);
  m_sendSocks[index].first->Send (packet);

  Time tNext (MicroSeconds (m_packetSize * 8 * 1e6 / m_dataRate.GetBitRate ()));
  Simulator::Schedule (tNext, &Ipv4DynamicGlobalRoutingTestCase::SendData, this, index);
}

void
Ipv4DynamicGlobalRoutingTestCase::ShutDownSock (uint8_t index)
{
  m_sendSocks[index].second = false;
  m_sendSocks[index].first->Close ();
  m_sendSocks[index].first = 0;
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
Ipv4DynamicGlobalRoutingTestCase::DoRun (void)
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
  SimpleNetDeviceHelper devHelper;

  devHelper.SetNetDevicePointToPointMode (true);
  NetDeviceContainer d0d2 = devHelper.Install (n0n2);
  devHelper.SetNetDevicePointToPointMode (false);

  NetDeviceContainer d1d6 = devHelper.Install (n1n6);
  NetDeviceContainer d1d2 = devHelper.Install (n1n2);
  NetDeviceContainer d5d6 = devHelper.Install (n5n6);
  NetDeviceContainer d2345 = devHelper.Install (n2345);

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

  // Create the applications to send UDP datagrams of size
  // 50 bytes at a rate of 2 Kb/s
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  uint16_t port = 9;   // Discard port (RFC 863)

  std::pair<Ptr<Socket>, bool>  sendSockA;
  sendSockA.first = Socket::CreateSocket (c.Get (1), tid);
  sendSockA.first->Bind ();
  sendSockA.first->Connect (InetSocketAddress (i5i6.GetAddress (1), port));
  sendSockA.second = true;
  m_sendSocks.push_back (sendSockA);
  Simulator::Schedule (Seconds (1.0), &Ipv4DynamicGlobalRoutingTestCase::SendData, this, 0);
  Simulator::Schedule (Seconds (10.0), &Ipv4DynamicGlobalRoutingTestCase::ShutDownSock, this, 0);

  std::pair<Ptr<Socket>, bool>  sendSockB;
  sendSockB.first = Socket::CreateSocket (c.Get (1), tid);
  sendSockB.first->Bind ();
  sendSockB.first->Connect (InetSocketAddress (i1i6.GetAddress (1), port));
  sendSockB.second = true;
  m_sendSocks.push_back (sendSockB);
  Simulator::Schedule (Seconds (11.0), &Ipv4DynamicGlobalRoutingTestCase::SendData, this, 1);
  Simulator::Schedule (Seconds (16.0), &Ipv4DynamicGlobalRoutingTestCase::ShutDownSock, this, 1);


  // Create an optional packet sink to receive these packets
  Ptr<Socket> sink2 = Socket::CreateSocket (c.Get (6), tid);
  sink2->Bind (Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
  sink2->Listen ();
  sink2->ShutdownSend ();

  sink2->SetRecvPktInfo (true);
  sink2->SetRecvCallback (MakeCallback (&Ipv4DynamicGlobalRoutingTestCase::HandleRead, this));

  Ptr<Node> n1 = c.Get (1);
  Ptr<Ipv4> ipv41 = n1->GetObject<Ipv4> ();
  // The first ifIndex is 0 for loopback, then the first p2p is numbered 1,
  // then the next p2p is numbered 2
  uint32_t ipv4ifIndex1 = 2;

  Simulator::Schedule (Seconds (2), &Ipv4::SetDown,ipv41, ipv4ifIndex1);
  Simulator::Schedule (Seconds (4), &Ipv4::SetUp,ipv41, ipv4ifIndex1);

  Ptr<Node> n6 = c.Get (6);
  Ptr<Ipv4> ipv46 = n6->GetObject<Ipv4> ();
  // The first ifIndex is 0 for loopback, then the first p2p is numbered 1,
  // then the next p2p is numbered 2
  uint32_t ipv4ifIndex6 = 2;
  Simulator::Schedule (Seconds (6), &Ipv4::SetDown,ipv46, ipv4ifIndex6);
  Simulator::Schedule (Seconds (8), &Ipv4::SetUp,ipv46, ipv4ifIndex6);

  Simulator::Schedule (Seconds (12), &Ipv4::SetDown,ipv41, ipv4ifIndex1);
  Simulator::Schedule (Seconds (14), &Ipv4::SetUp,ipv41, ipv4ifIndex1);

  Simulator::Run ();

  NS_TEST_ASSERT_MSG_EQ (m_count, 70, "Dynamic global routing did not deliver all packets");
// Test that for node n6, the interface facing n5 receives packets at
// times (1-2), (4-6), (8-10), (11-12), (14-16) and the interface
// facing n1 receives packets at times (2-4), (6-8), (12-13)
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[1], 5, "Dynamic global routing did not deliver all packets " << int(m_firstInterface[1]));
  NS_TEST_ASSERT_MSG_EQ (m_secondInterface[2], 5, "Dynamic global routing did not deliver all packets " << int(m_secondInterface[2]));
  NS_TEST_ASSERT_MSG_EQ (m_secondInterface[3], 5, "Dynamic global routing did not deliver all packets " << int(m_secondInterface[3]));
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[4], 5, "Dynamic global routing did not deliver all packets " << int(m_firstInterface[4]));
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[5], 5, "Dynamic global routing did not deliver all packets " << int(m_firstInterface[5]));
  NS_TEST_ASSERT_MSG_EQ (m_secondInterface[6], 5, "Dynamic global routing did not deliver all packets " << int(m_secondInterface[6]));
  NS_TEST_ASSERT_MSG_EQ (m_secondInterface[7], 5, "Dynamic global routing did not deliver all packets " << int(m_secondInterface[7]));
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[8], 5, "Dynamic global routing did not deliver all packets " << int(m_firstInterface[8]));
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[9], 5, "Dynamic global routing did not deliver all packets " << int(m_firstInterface[9]));
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[10], 0, "Dynamic global routing did not deliver all packets " << int(m_firstInterface[10]));
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[11], 5, "Dynamic global routing did not deliver all packets " << int(m_firstInterface[11]));
  NS_TEST_ASSERT_MSG_EQ (m_secondInterface[12], 5, "Dynamic global routing did not deliver all packets " << int(m_secondInterface[12]));
  NS_TEST_ASSERT_MSG_EQ (m_secondInterface[13], 5, "Dynamic global routing did not deliver all packets " << int(m_secondInterface[13]));
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[14], 5, "Dynamic global routing did not deliver all packets " << int(m_firstInterface[14]));
  NS_TEST_ASSERT_MSG_EQ (m_firstInterface[15], 5, "Dynamic global routing did not deliver all packets " << int(m_firstInterface[15]));
  Simulator::Destroy ();
}

class Ipv4GlobalRoutingSlash32TestCase : public TestCase
{
public:
  Ipv4GlobalRoutingSlash32TestCase ();
  virtual ~Ipv4GlobalRoutingSlash32TestCase ();

  Ptr<Packet> m_receivedPacket;
  void ReceivePkt (Ptr<Socket> socket);
  void DoSendData (Ptr<Socket> socket, std::string to);
  void SendData (Ptr<Socket> socket, std::string to);

private:
  virtual void DoRun (void);
};

// Add some help text to this case to describe what it is intended to test
Ipv4GlobalRoutingSlash32TestCase::Ipv4GlobalRoutingSlash32TestCase ()
  : TestCase ("Slash 32 global routing example")
{
}

Ipv4GlobalRoutingSlash32TestCase::~Ipv4GlobalRoutingSlash32TestCase ()
{
}

void
Ipv4GlobalRoutingSlash32TestCase::ReceivePkt (Ptr<Socket> socket)
{
  uint32_t availableData;
  availableData = socket->GetRxAvailable ();
  m_receivedPacket = socket->Recv (std::numeric_limits<uint32_t>::max (), 0);
  NS_ASSERT (availableData == m_receivedPacket->GetSize ());
  //cast availableData to void, to suppress 'availableData' set but not used
  //compiler warning
  (void) availableData;
}

void
Ipv4GlobalRoutingSlash32TestCase::DoSendData (Ptr<Socket> socket, std::string to)
{
  Address realTo = InetSocketAddress (Ipv4Address (to.c_str ()), 1234);
  NS_TEST_EXPECT_MSG_EQ (socket->SendTo (Create<Packet> (123), 0, realTo),
                         123, "100");
}

void
Ipv4GlobalRoutingSlash32TestCase::SendData (Ptr<Socket> socket, std::string to)
{
  m_receivedPacket = Create<Packet> ();
  Simulator::ScheduleWithContext (socket->GetNode ()->GetId (), Seconds (60),
                                  &Ipv4GlobalRoutingSlash32TestCase::DoSendData, this, socket, to);
  Simulator::Stop (Seconds (66));
  Simulator::Run ();
}

// Test program for this 3-router scenario, using global routing
//
// (a.a.a.a/32)A<--x.x.x.0/30-->B<--y.y.y.0/30-->C(c.c.c.c/32)
//
void
Ipv4GlobalRoutingSlash32TestCase::DoRun (void)
{
  Ptr<Node> nA = CreateObject<Node> ();
  Ptr<Node> nB = CreateObject<Node> ();
  Ptr<Node> nC = CreateObject<Node> ();

  NodeContainer c = NodeContainer (nA, nB, nC);

  InternetStackHelper internet;
  internet.Install (c);

  // simple links
  NodeContainer nAnB = NodeContainer (nA, nB);
  NodeContainer nBnC = NodeContainer (nB, nC);

  SimpleNetDeviceHelper devHelper;

  Ptr<SimpleNetDevice> deviceA = CreateObject<SimpleNetDevice> ();
  deviceA->SetAddress (Mac48Address::Allocate ());
  nA->AddDevice (deviceA);

  NetDeviceContainer dAdB = devHelper.Install (nAnB);
  NetDeviceContainer dBdC = devHelper.Install (nBnC);

  Ptr<SimpleNetDevice> deviceC = CreateObject<SimpleNetDevice> ();
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

  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // Create the UDP sockets
  Ptr<SocketFactory> rxSocketFactory = nC->GetObject<UdpSocketFactory> ();
  Ptr<Socket> rxSocket = rxSocketFactory->CreateSocket ();
  NS_TEST_EXPECT_MSG_EQ (rxSocket->Bind (InetSocketAddress (Ipv4Address ("192.168.1.1"), 1234)), 0, "trivial");
  rxSocket->SetRecvCallback (MakeCallback (&Ipv4GlobalRoutingSlash32TestCase::ReceivePkt, this));

  Ptr<SocketFactory> txSocketFactory = nA->GetObject<UdpSocketFactory> ();
  Ptr<Socket> txSocket = txSocketFactory->CreateSocket ();
  txSocket->SetAllowBroadcast (true);

  // ------ Now the tests ------------

  // Unicast test
  SendData (txSocket, "192.168.1.1");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket->GetSize (), 123, "Static routing with /32 did not deliver all packets.");

  Simulator::Destroy ();
}


class Ipv4GlobalRoutingTestSuite : public TestSuite
{
public:
  Ipv4GlobalRoutingTestSuite ();
};

Ipv4GlobalRoutingTestSuite::Ipv4GlobalRoutingTestSuite ()
  : TestSuite ("ipv4-global-routing", UNIT)
{
  AddTestCase (new Ipv4DynamicGlobalRoutingTestCase, TestCase::QUICK);
  AddTestCase (new Ipv4GlobalRoutingSlash32TestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static Ipv4GlobalRoutingTestSuite globalRoutingTestSuite;
