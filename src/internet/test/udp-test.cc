/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
 * 
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
 *
 * Author: Raj Bhattacharjea <raj.b@gatech.edu>
 */
/**
 * This is the test code for udp-socket-impl.cc, it was moved out of udp-socket-impl.cc to 
 * be in an independent file for clarity purposes.
 */

#include "ns3/test.h"
#include "ns3/socket-factory.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/simple-channel.h"
#include "ns3/simple-net-device.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/socket.h"

#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/inet-socket-address.h"

#include "ns3/arp-l3-protocol.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/icmpv4-l4-protocol.h"
#include "ns3/udp-l4-protocol.h"
#include "ns3/tcp-l4-protocol.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/ipv4-static-routing.h"

#include <string>
#include <limits>
namespace ns3 {

static void
AddInternetStack (Ptr<Node> node)
{
  //ARP
  Ptr<ArpL3Protocol> arp = CreateObject<ArpL3Protocol> ();
  node->AggregateObject (arp);
  //IPV4
  Ptr<Ipv4L3Protocol> ipv4 = CreateObject<Ipv4L3Protocol> ();
  //Routing for Ipv4
  Ptr<Ipv4ListRouting> ipv4Routing = CreateObject<Ipv4ListRouting> ();
  ipv4->SetRoutingProtocol (ipv4Routing);
  Ptr<Ipv4StaticRouting> ipv4staticRouting = CreateObject<Ipv4StaticRouting> ();
  ipv4Routing->AddRoutingProtocol (ipv4staticRouting, 0);
  node->AggregateObject (ipv4);
  //ICMP
  Ptr<Icmpv4L4Protocol> icmp = CreateObject<Icmpv4L4Protocol> ();
  node->AggregateObject (icmp);
  //UDP
  Ptr<UdpL4Protocol> udp = CreateObject<UdpL4Protocol> ();
  node->AggregateObject (udp);
  //TCP
  Ptr<TcpL4Protocol> tcp = CreateObject<TcpL4Protocol> ();
  node->AggregateObject (tcp);
}


class UdpSocketLoopbackTest : public TestCase
{
public:
  UdpSocketLoopbackTest ();
  virtual void DoRun (void);

  void ReceivePkt (Ptr<Socket> socket);
  Ptr<Packet> m_receivedPacket;
};

UdpSocketLoopbackTest::UdpSocketLoopbackTest ()
  : TestCase ("UDP loopback test") 
{
}

void UdpSocketLoopbackTest::ReceivePkt (Ptr<Socket> socket)
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
UdpSocketLoopbackTest::DoRun ()
{
  Ptr<Node> rxNode = CreateObject<Node> ();
  AddInternetStack (rxNode);

  Ptr<SocketFactory> rxSocketFactory = rxNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> rxSocket = rxSocketFactory->CreateSocket ();
  rxSocket->Bind (InetSocketAddress (Ipv4Address::GetAny (), 80));
  rxSocket->SetRecvCallback (MakeCallback (&UdpSocketLoopbackTest::ReceivePkt, this));

  Ptr<Socket> txSocket = rxSocketFactory->CreateSocket ();
  txSocket->SendTo (Create<Packet> (246), 0, InetSocketAddress ("127.0.0.1", 80));
  Simulator::Run ();
  Simulator::Destroy ();
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket->GetSize (), 246, "first socket should not receive it (it is bound specifically to the second interface's address");
}

class UdpSocketImplTest : public TestCase
{
  Ptr<Packet> m_receivedPacket;
  Ptr<Packet> m_receivedPacket2;
  void DoSendData (Ptr<Socket> socket, std::string to);
  void SendData (Ptr<Socket> socket, std::string to);

public:
  virtual void DoRun (void);
  UdpSocketImplTest ();

  void ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from);
  void ReceivePacket2 (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from);
  void ReceivePkt (Ptr<Socket> socket);
  void ReceivePkt2 (Ptr<Socket> socket);
};

UdpSocketImplTest::UdpSocketImplTest ()
  : TestCase ("UDP socket implementation") 
{
}

void UdpSocketImplTest::ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from)
{
  m_receivedPacket = packet;
}

void UdpSocketImplTest::ReceivePacket2 (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from)
{
  m_receivedPacket2 = packet;
}

void UdpSocketImplTest::ReceivePkt (Ptr<Socket> socket)
{
  uint32_t availableData;
  availableData = socket->GetRxAvailable ();
  m_receivedPacket = socket->Recv (std::numeric_limits<uint32_t>::max (), 0);
  NS_ASSERT (availableData == m_receivedPacket->GetSize ());
  //cast availableData to void, to suppress 'availableData' set but not used
  //compiler warning
  (void) availableData;
}

void UdpSocketImplTest::ReceivePkt2 (Ptr<Socket> socket)
{
  uint32_t availableData;
  availableData = socket->GetRxAvailable ();
  m_receivedPacket2 = socket->Recv (std::numeric_limits<uint32_t>::max (), 0);
  NS_ASSERT (availableData == m_receivedPacket2->GetSize ());
  //cast availableData to void, to suppress 'availableData' set but not used
  //compiler warning
  (void) availableData;
}

void
UdpSocketImplTest::DoSendData (Ptr<Socket> socket, std::string to)
{
  Address realTo = InetSocketAddress (Ipv4Address (to.c_str ()), 1234);
  NS_TEST_EXPECT_MSG_EQ (socket->SendTo (Create<Packet> (123), 0, realTo),
                         123, "XXX");
}

void
UdpSocketImplTest::SendData (Ptr<Socket> socket, std::string to)
{
  m_receivedPacket = Create<Packet> ();
  m_receivedPacket2 = Create<Packet> ();
  Simulator::ScheduleWithContext (socket->GetNode ()->GetId (), Seconds (0),
                                  &UdpSocketImplTest::DoSendData, this, socket, to);
  Simulator::Run ();
}

void
UdpSocketImplTest::DoRun (void)
{
  // Create topology

  // Receiver Node
  Ptr<Node> rxNode = CreateObject<Node> ();
  AddInternetStack (rxNode);
  Ptr<SimpleNetDevice> rxDev1, rxDev2;
  { // first interface
    rxDev1 = CreateObject<SimpleNetDevice> ();
    rxDev1->SetAddress (Mac48Address::ConvertFrom (Mac48Address::Allocate ()));
    rxNode->AddDevice (rxDev1);
    Ptr<Ipv4> ipv4 = rxNode->GetObject<Ipv4> ();
    uint32_t netdev_idx = ipv4->AddInterface (rxDev1);
    Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.0.1"), Ipv4Mask (0xffff0000U));
    ipv4->AddAddress (netdev_idx, ipv4Addr);
    ipv4->SetUp (netdev_idx);
  }

  { // second interface
    rxDev2 = CreateObject<SimpleNetDevice> ();
    rxDev2->SetAddress (Mac48Address::ConvertFrom (Mac48Address::Allocate ()));
    rxNode->AddDevice (rxDev2);
    Ptr<Ipv4> ipv4 = rxNode->GetObject<Ipv4> ();
    uint32_t netdev_idx = ipv4->AddInterface (rxDev2);
    Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.1.1"), Ipv4Mask (0xffff0000U));
    ipv4->AddAddress (netdev_idx, ipv4Addr);
    ipv4->SetUp (netdev_idx);
  }

  // Sender Node
  Ptr<Node> txNode = CreateObject<Node> ();
  AddInternetStack (txNode);
  Ptr<SimpleNetDevice> txDev1;
  {
    txDev1 = CreateObject<SimpleNetDevice> ();
    txDev1->SetAddress (Mac48Address::ConvertFrom (Mac48Address::Allocate ()));
    txNode->AddDevice (txDev1);
    Ptr<Ipv4> ipv4 = txNode->GetObject<Ipv4> ();
    uint32_t netdev_idx = ipv4->AddInterface (txDev1);
    Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.0.2"), Ipv4Mask (0xffff0000U));
    ipv4->AddAddress (netdev_idx, ipv4Addr);
    ipv4->SetUp (netdev_idx);
  }
  Ptr<SimpleNetDevice> txDev2;
  {
    txDev2 = CreateObject<SimpleNetDevice> ();
    txDev2->SetAddress (Mac48Address::ConvertFrom (Mac48Address::Allocate ()));
    txNode->AddDevice (txDev2);
    Ptr<Ipv4> ipv4 = txNode->GetObject<Ipv4> ();
    uint32_t netdev_idx = ipv4->AddInterface (txDev2);
    Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.1.2"), Ipv4Mask (0xffff0000U));
    ipv4->AddAddress (netdev_idx, ipv4Addr);
    ipv4->SetUp (netdev_idx);
  }

  // link the two nodes
  Ptr<SimpleChannel> channel1 = CreateObject<SimpleChannel> ();
  rxDev1->SetChannel (channel1);
  txDev1->SetChannel (channel1);

  Ptr<SimpleChannel> channel2 = CreateObject<SimpleChannel> ();
  rxDev2->SetChannel (channel2);
  txDev2->SetChannel (channel2);


  // Create the UDP sockets
  Ptr<SocketFactory> rxSocketFactory = rxNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> rxSocket = rxSocketFactory->CreateSocket ();
  NS_TEST_EXPECT_MSG_EQ (rxSocket->Bind (InetSocketAddress (Ipv4Address ("10.0.0.1"), 1234)), 0, "trivial");
  rxSocket->SetRecvCallback (MakeCallback (&UdpSocketImplTest::ReceivePkt, this));

  Ptr<Socket> rxSocket2 = rxSocketFactory->CreateSocket ();
  rxSocket2->SetRecvCallback (MakeCallback (&UdpSocketImplTest::ReceivePkt2, this));
  NS_TEST_EXPECT_MSG_EQ (rxSocket2->Bind (InetSocketAddress (Ipv4Address ("10.0.1.1"), 1234)), 0, "trivial");

  Ptr<SocketFactory> txSocketFactory = txNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> txSocket = txSocketFactory->CreateSocket ();
  txSocket->SetAllowBroadcast (true);

  // ------ Now the tests ------------

  // Unicast test
  SendData (txSocket, "10.0.0.1");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket->GetSize (), 123, "trivial");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket2->GetSize (), 0, "second interface should receive it");

  m_receivedPacket->RemoveAllByteTags ();
  m_receivedPacket2->RemoveAllByteTags ();

  // Simple broadcast test

  SendData (txSocket, "255.255.255.255");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket->GetSize (), 123, "trivial");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket2->GetSize (), 0, "second socket should not receive it (it is bound specifically to the second interface's address");

  m_receivedPacket->RemoveAllByteTags ();
  m_receivedPacket2->RemoveAllByteTags ();

  // Broadcast test with multiple receiving sockets

  // When receiving broadcast packets, all sockets sockets bound to
  // the address/port should receive a copy of the same packet -- if
  // the socket address matches.
  rxSocket2->Dispose ();
  rxSocket2 = rxSocketFactory->CreateSocket ();
  rxSocket2->SetRecvCallback (MakeCallback (&UdpSocketImplTest::ReceivePkt2, this));
  NS_TEST_EXPECT_MSG_EQ (rxSocket2->Bind (InetSocketAddress (Ipv4Address ("0.0.0.0"), 1234)), 0, "trivial");

  SendData (txSocket, "255.255.255.255");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket->GetSize (), 123, "trivial");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket2->GetSize (), 123, "trivial");

  m_receivedPacket = 0;
  m_receivedPacket2 = 0;

  // Simple Link-local multicast test

  txSocket->BindToNetDevice (txDev1);
  SendData (txSocket, "224.0.0.9");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket->GetSize (), 0, "first socket should not receive it (it is bound specifically to the second interface's address");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket2->GetSize (), 123, "recv2: 224.0.0.9");

  m_receivedPacket->RemoveAllByteTags ();
  m_receivedPacket2->RemoveAllByteTags ();

  Simulator::Destroy ();

}

//-----------------------------------------------------------------------------
class UdpTestSuite : public TestSuite
{
public:
  UdpTestSuite () : TestSuite ("udp", UNIT)
  {
    AddTestCase (new UdpSocketImplTest);
    AddTestCase (new UdpSocketLoopbackTest);
  }
} g_udpTestSuite;

}; // namespace ns3
