/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "ns3/simple-net-device-helper.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/socket.h"

#include "ns3/boolean.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/internet-stack-helper.h"

#include "ns3/arp-l3-protocol.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv6-l3-protocol.h"
#include "ns3/icmpv4-l4-protocol.h"
#include "ns3/icmpv6-l4-protocol.h"
#include "ns3/udp-l4-protocol.h"
#include "ns3/tcp-l4-protocol.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/ipv4-static-routing.h"
#include "ns3/ipv6-list-routing.h"
#include "ns3/ipv6-static-routing.h"
#include "ns3/ipv6-address-helper.h"

#include <string>
#include <limits>

using namespace ns3;


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
}

void
UdpSocketLoopbackTest::DoRun ()
{
  Ptr<Node> rxNode = CreateObject<Node> ();
  InternetStackHelper internet;
  internet.Install (rxNode);

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

class Udp6SocketLoopbackTest : public TestCase
{
public:
  Udp6SocketLoopbackTest ();
  virtual void DoRun (void);

  void ReceivePkt (Ptr<Socket> socket);
  Ptr<Packet> m_receivedPacket;
};

Udp6SocketLoopbackTest::Udp6SocketLoopbackTest ()
  : TestCase ("UDP6 loopback test") 
{
}

void Udp6SocketLoopbackTest::ReceivePkt (Ptr<Socket> socket)
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
Udp6SocketLoopbackTest::DoRun ()
{
  Ptr<Node> rxNode = CreateObject<Node> ();
  InternetStackHelper internet;
  internet.Install (rxNode);

  Ptr<SocketFactory> rxSocketFactory = rxNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> rxSocket = rxSocketFactory->CreateSocket ();
  rxSocket->Bind (Inet6SocketAddress (Ipv6Address::GetAny (), 80));
  rxSocket->SetRecvCallback (MakeCallback (&Udp6SocketLoopbackTest::ReceivePkt, this));

  Ptr<Socket> txSocket = rxSocketFactory->CreateSocket ();
  txSocket->SendTo (Create<Packet> (246), 0, Inet6SocketAddress ("::1", 80));
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
}

void UdpSocketImplTest::ReceivePkt2 (Ptr<Socket> socket)
{
  uint32_t availableData;
  availableData = socket->GetRxAvailable ();
  m_receivedPacket2 = socket->Recv (std::numeric_limits<uint32_t>::max (), 0);
  NS_ASSERT (availableData == m_receivedPacket2->GetSize ());
}

void
UdpSocketImplTest::DoSendData (Ptr<Socket> socket, std::string to)
{
  Address realTo = InetSocketAddress (Ipv4Address (to.c_str ()), 1234);
  NS_TEST_EXPECT_MSG_EQ (socket->SendTo (Create<Packet> (123), 0, realTo),
                         123, "100");
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
  // Sender Node
  Ptr<Node> txNode = CreateObject<Node> ();

  NodeContainer nodes (rxNode, txNode);

  SimpleNetDeviceHelper helperChannel1;
  helperChannel1.SetNetDevicePointToPointMode (true);
  NetDeviceContainer net1 = helperChannel1.Install (nodes);

  SimpleNetDeviceHelper helperChannel2;
  helperChannel2.SetNetDevicePointToPointMode (true);
  NetDeviceContainer net2 = helperChannel2.Install (nodes);

  InternetStackHelper internet;
  internet.Install (nodes);

  Ptr<Ipv4> ipv4;
  uint32_t netdev_idx;
  Ipv4InterfaceAddress ipv4Addr;

  // Receiver Node
  ipv4 = rxNode->GetObject<Ipv4> ();
  netdev_idx = ipv4->AddInterface (net1.Get (0));
  ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.0.1"), Ipv4Mask (0xffff0000U));
  ipv4->AddAddress (netdev_idx, ipv4Addr);
  ipv4->SetUp (netdev_idx);

  netdev_idx = ipv4->AddInterface (net2.Get (0));
  ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.1.1"), Ipv4Mask (0xffff0000U));
  ipv4->AddAddress (netdev_idx, ipv4Addr);
  ipv4->SetUp (netdev_idx);

  // Sender Node
  ipv4 = txNode->GetObject<Ipv4> ();
  netdev_idx = ipv4->AddInterface (net1.Get (1));
  ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.0.2"), Ipv4Mask (0xffff0000U));
  ipv4->AddAddress (netdev_idx, ipv4Addr);
  ipv4->SetUp (netdev_idx);

  netdev_idx = ipv4->AddInterface (net2.Get (1));
  ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.1.2"), Ipv4Mask (0xffff0000U));
  ipv4->AddAddress (netdev_idx, ipv4Addr);
  ipv4->SetUp (netdev_idx);

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

  txSocket->BindToNetDevice (net1.Get (1));
  SendData (txSocket, "224.0.0.9");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket->GetSize (), 0, "first socket should not receive it (it is bound specifically to the second interface's address");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket2->GetSize (), 123, "recv2: 224.0.0.9");

  m_receivedPacket->RemoveAllByteTags ();
  m_receivedPacket2->RemoveAllByteTags ();

  // Simple getpeername tests

  Address peerAddress;
  int err = txSocket->GetPeerName (peerAddress);
  NS_TEST_EXPECT_MSG_EQ (err, -1, "socket GetPeerName() should fail when socket is not connected");
  NS_TEST_EXPECT_MSG_EQ (txSocket->GetErrno (), Socket::ERROR_NOTCONN, "socket error code should be ERROR_NOTCONN");

  InetSocketAddress peer ("10.0.0.1", 1234);
  err = txSocket->Connect (peer);
  NS_TEST_EXPECT_MSG_EQ (err, 0, "socket Connect() should succeed");

  err = txSocket->GetPeerName (peerAddress);
  NS_TEST_EXPECT_MSG_EQ (err, 0, "socket GetPeerName() should succeed when socket is connected");
  NS_TEST_EXPECT_MSG_EQ (peerAddress, peer, "address from socket GetPeerName() should equal the connected address");

  Simulator::Destroy ();

}

class Udp6SocketImplTest : public TestCase
{
  Ptr<Packet> m_receivedPacket;
  Ptr<Packet> m_receivedPacket2;
  void DoSendData (Ptr<Socket> socket, std::string to);
  void SendData (Ptr<Socket> socket, std::string to);

public:
  virtual void DoRun (void);
  Udp6SocketImplTest ();

  void ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from);
  void ReceivePacket2 (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from);
  void ReceivePkt (Ptr<Socket> socket);
  void ReceivePkt2 (Ptr<Socket> socket);
};

Udp6SocketImplTest::Udp6SocketImplTest ()
  : TestCase ("UDP6 socket implementation")
{
}

void Udp6SocketImplTest::ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from)
{
  m_receivedPacket = packet;
}

void Udp6SocketImplTest::ReceivePacket2 (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from)
{
  m_receivedPacket2 = packet;
}

void Udp6SocketImplTest::ReceivePkt (Ptr<Socket> socket)
{
  uint32_t availableData;
  availableData = socket->GetRxAvailable ();
  m_receivedPacket = socket->Recv (std::numeric_limits<uint32_t>::max (), 0);
  NS_ASSERT (availableData == m_receivedPacket->GetSize ());
  //cast availableData to void, to suppress 'availableData' set but not used
  //compiler warning
  (void) availableData;
}

void Udp6SocketImplTest::ReceivePkt2 (Ptr<Socket> socket)
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
Udp6SocketImplTest::DoSendData (Ptr<Socket> socket, std::string to)
{
  Address realTo = Inet6SocketAddress (Ipv6Address (to.c_str ()), 1234);
  NS_TEST_EXPECT_MSG_EQ (socket->SendTo (Create<Packet> (123), 0, realTo),
                         123, "200");
}

void
Udp6SocketImplTest::SendData (Ptr<Socket> socket, std::string to)
{
  m_receivedPacket = Create<Packet> ();
  m_receivedPacket2 = Create<Packet> ();
  Simulator::ScheduleWithContext (socket->GetNode ()->GetId (), Seconds (0),
                                  &Udp6SocketImplTest::DoSendData, this, socket, to);
  Simulator::Run ();
}

void
Udp6SocketImplTest::DoRun (void)
{
  // Create topology

  // Receiver Node
  Ptr<Node> rxNode = CreateObject<Node> ();
  // Sender Node
  Ptr<Node> txNode = CreateObject<Node> ();

  NodeContainer nodes (rxNode, txNode);

  SimpleNetDeviceHelper helperChannel1;
  helperChannel1.SetNetDevicePointToPointMode (true);
  NetDeviceContainer net1 = helperChannel1.Install (nodes);

  SimpleNetDeviceHelper helperChannel2;
  helperChannel2.SetNetDevicePointToPointMode (true);
  NetDeviceContainer net2 = helperChannel2.Install (nodes);

  InternetStackHelper internetv6;
  internetv6.Install (nodes);

  txNode->GetObject<Icmpv6L4Protocol> ()->SetAttribute ("DAD", BooleanValue (false));
  rxNode->GetObject<Icmpv6L4Protocol> ()->SetAttribute ("DAD", BooleanValue (false));

  Ipv6AddressHelper ipv6helper;
  Ipv6InterfaceContainer iic1 = ipv6helper.AssignWithoutAddress (net1);
  Ipv6InterfaceContainer iic2 = ipv6helper.AssignWithoutAddress (net2);

  Ptr<NetDevice> device;
  Ptr<Ipv6> ipv6;
  int32_t ifIndex;
  Ipv6InterfaceAddress ipv6Addr;

  ipv6 = rxNode->GetObject<Ipv6> ();
  device = net1.Get (0);
  ifIndex = ipv6->GetInterfaceForDevice (device);
  ipv6Addr = Ipv6InterfaceAddress (Ipv6Address ("2001:0100::1"), Ipv6Prefix (64));
  ipv6->AddAddress (ifIndex, ipv6Addr);
  ipv6->SetUp (ifIndex);

  device = net2.Get (0);
  ifIndex = ipv6->GetInterfaceForDevice (device);
  ipv6Addr = Ipv6InterfaceAddress (Ipv6Address ("2001:0100:1::1"), Ipv6Prefix (64));
  ipv6->AddAddress (ifIndex, ipv6Addr);
  ipv6->SetUp (ifIndex);

  ipv6 = txNode->GetObject<Ipv6> ();
  device = net1.Get (1);
  ifIndex = ipv6->GetInterfaceForDevice (device);
  ipv6Addr = Ipv6InterfaceAddress (Ipv6Address ("2001:0100::2"), Ipv6Prefix (64));
  ipv6->AddAddress (ifIndex, ipv6Addr);
  ipv6->SetUp (ifIndex);

  device = net2.Get (1);
  ifIndex = ipv6->GetInterfaceForDevice (device);
  ipv6Addr = Ipv6InterfaceAddress (Ipv6Address ("2001:0100:1::2"), Ipv6Prefix (64));
  ipv6->AddAddress (ifIndex, ipv6Addr);
  ipv6->SetUp (ifIndex);

  // Create the UDP sockets
  Ptr<SocketFactory> rxSocketFactory = rxNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> rxSocket = rxSocketFactory->CreateSocket ();
  NS_TEST_EXPECT_MSG_EQ (rxSocket->Bind (Inet6SocketAddress (Ipv6Address ("2001:0100::1"), 1234)), 0, "trivial");
  rxSocket->SetRecvCallback (MakeCallback (&Udp6SocketImplTest::ReceivePkt, this));

  Ptr<Socket> rxSocket2 = rxSocketFactory->CreateSocket ();
  rxSocket2->SetRecvCallback (MakeCallback (&Udp6SocketImplTest::ReceivePkt2, this));
  NS_TEST_EXPECT_MSG_EQ (rxSocket2->Bind (Inet6SocketAddress (Ipv6Address ("2001:0100:1::1"), 1234)), 0, "trivial");

  Ptr<SocketFactory> txSocketFactory = txNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> txSocket = txSocketFactory->CreateSocket ();
  txSocket->SetAllowBroadcast (true);
  // ------ Now the tests ------------

  // Unicast test
  SendData (txSocket, "2001:0100::1");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket->GetSize (), 123, "trivial");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket2->GetSize (), 0, "second interface should receive it");

  m_receivedPacket->RemoveAllByteTags ();
  m_receivedPacket2->RemoveAllByteTags ();

  // Simple Link-local multicast test

  // When receiving broadcast packets, all sockets sockets bound to
  // the address/port should receive a copy of the same packet -- if
  // the socket address matches.
  rxSocket2->Dispose ();
  rxSocket2 = rxSocketFactory->CreateSocket ();
  rxSocket2->SetRecvCallback (MakeCallback (&Udp6SocketImplTest::ReceivePkt2, this));
  NS_TEST_EXPECT_MSG_EQ (rxSocket2->Bind (Inet6SocketAddress (Ipv6Address ("::"), 1234)), 0, "trivial");

  txSocket->BindToNetDevice (net1.Get (1));
  SendData (txSocket, "ff02::1");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket->GetSize (), 0, "first socket should not receive it (it is bound specifically to the second interface's address");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket2->GetSize (), 123, "recv2: ff02::1");

  m_receivedPacket->RemoveAllByteTags ();
  m_receivedPacket2->RemoveAllByteTags ();

  // Simple getpeername tests
  Address peerAddress;
  int err = txSocket->GetPeerName (peerAddress);
  NS_TEST_EXPECT_MSG_EQ (err, -1, "socket GetPeerName() should fail when socket is not connected");
  NS_TEST_EXPECT_MSG_EQ (txSocket->GetErrno (), Socket::ERROR_NOTCONN, "socket error code should be ERROR_NOTCONN");

  Inet6SocketAddress peer ("2001:0100::1", 1234);
  err = txSocket->Connect (peer);
  NS_TEST_EXPECT_MSG_EQ (err, 0, "socket Connect() should succeed");

  err = txSocket->GetPeerName (peerAddress);
  NS_TEST_EXPECT_MSG_EQ (err, 0, "socket GetPeerName() should succeed when socket is connected");
  NS_TEST_EXPECT_MSG_EQ (peerAddress, peer, "address from socket GetPeerName() should equal the connected address");

  Simulator::Destroy ();

}


//-----------------------------------------------------------------------------
class UdpTestSuite : public TestSuite
{
public:
  UdpTestSuite () : TestSuite ("udp", UNIT)
  {
    AddTestCase (new UdpSocketImplTest, TestCase::QUICK);
    AddTestCase (new UdpSocketLoopbackTest, TestCase::QUICK);
    AddTestCase (new Udp6SocketImplTest, TestCase::QUICK);
    AddTestCase (new Udp6SocketLoopbackTest, TestCase::QUICK);
  }
} g_udpTestSuite;
