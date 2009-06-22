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
#ifdef RUN_SELF_TESTS

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

#include "arp-l3-protocol.h"
#include "ipv4-l3-protocol.h"
#include "icmpv4-l4-protocol.h"
#include "udp-l4-protocol.h"
#include "tcp-l4-protocol.h"
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
  node->AggregateObject(arp);
  //IPV4
  Ptr<Ipv4L3Protocol> ipv4 = CreateObject<Ipv4L3Protocol> ();
  //Routing for Ipv4
  Ptr<Ipv4ListRouting> ipv4Routing = CreateObject<Ipv4ListRouting> ();
  ipv4->SetRoutingProtocol (ipv4Routing);
  Ptr<Ipv4StaticRouting> ipv4staticRouting = CreateObject<Ipv4StaticRouting> ();
  ipv4Routing->AddRoutingProtocol (ipv4staticRouting, 0);
  node->AggregateObject(ipv4);
  //ICMP
  Ptr<Icmpv4L4Protocol> icmp = CreateObject<Icmpv4L4Protocol> ();
  node->AggregateObject(icmp);
  //UDP
  Ptr<UdpL4Protocol> udp = CreateObject<UdpL4Protocol> ();
  node->AggregateObject(udp); 
  //TCP
  Ptr<TcpL4Protocol> tcp = CreateObject<TcpL4Protocol> ();
  node->AggregateObject(tcp);
}


class UdpSocketImplTest: public Test
{
  Ptr<Packet> m_receivedPacket;
  Ptr<Packet> m_receivedPacket2;

public:
  virtual bool RunTests (void);
  UdpSocketImplTest ();

  void ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from);
  void ReceivePacket2 (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from);
  void ReceivePkt (Ptr<Socket> socket);
  void ReceivePkt2 (Ptr<Socket> socket);
};


UdpSocketImplTest::UdpSocketImplTest ()
  : Test ("UdpSocketImpl") 
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
  m_receivedPacket = socket->Recv (std::numeric_limits<uint32_t>::max(), 0);
  NS_ASSERT (availableData == m_receivedPacket->GetSize ());
}

void UdpSocketImplTest::ReceivePkt2 (Ptr<Socket> socket)
{
  uint32_t availableData;
  availableData = socket->GetRxAvailable ();
  m_receivedPacket2 = socket->Recv (std::numeric_limits<uint32_t>::max(), 0);
  NS_ASSERT (availableData == m_receivedPacket2->GetSize ());
}

bool
UdpSocketImplTest::RunTests (void)
{
  bool result = true;

  // Create topology
  
  // Receiver Node
  Ptr<Node> rxNode = CreateObject<Node> ();
  AddInternetStack (rxNode);
  Ptr<SimpleNetDevice> rxDev1, rxDev2;
  { // first interface
    rxDev1 = CreateObject<SimpleNetDevice> ();
    rxDev1->SetAddress (Mac48Address::Allocate ());
    rxNode->AddDevice (rxDev1);
    Ptr<Ipv4> ipv4 = rxNode->GetObject<Ipv4> ();
    uint32_t netdev_idx = ipv4->AddInterface (rxDev1);
    Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.0.1"), Ipv4Mask (0xffff0000U));
    ipv4->AddAddress (netdev_idx, ipv4Addr);
    ipv4->SetUp (netdev_idx);
  }

  { // second interface
    rxDev2 = CreateObject<SimpleNetDevice> ();
    rxDev2->SetAddress (Mac48Address::Allocate ());
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
    txDev1->SetAddress (Mac48Address::Allocate ());
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
    txDev2->SetAddress (Mac48Address::Allocate ());
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
  NS_TEST_ASSERT_EQUAL (rxSocket->Bind (InetSocketAddress (Ipv4Address ("10.0.0.1"), 1234)), 0);
  rxSocket->SetRecvCallback (MakeCallback (&UdpSocketImplTest::ReceivePkt, this));

  Ptr<Socket> rxSocket2 = rxSocketFactory->CreateSocket ();
  rxSocket2->SetRecvCallback (MakeCallback (&UdpSocketImplTest::ReceivePkt2, this));
  NS_TEST_ASSERT_EQUAL (rxSocket2->Bind (InetSocketAddress (Ipv4Address ("10.0.1.1"), 1234)), 0);

  Ptr<SocketFactory> txSocketFactory = txNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> txSocket = txSocketFactory->CreateSocket ();

  // ------ Now the tests ------------

  // Unicast test
  m_receivedPacket = Create<Packet> ();
  m_receivedPacket2 = Create<Packet> ();
  NS_TEST_ASSERT_EQUAL (txSocket->SendTo ( Create<Packet> (123), 0, 
    InetSocketAddress (Ipv4Address("10.0.0.1"), 1234)), 123);
  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_receivedPacket->GetSize (), 123);
  NS_TEST_ASSERT_EQUAL (m_receivedPacket2->GetSize (), 0); // second interface should receive it

  m_receivedPacket->RemoveAllByteTags ();
  m_receivedPacket2->RemoveAllByteTags ();

  // Simple broadcast test

  m_receivedPacket = Create<Packet> ();
  m_receivedPacket2 = Create<Packet> ();
  NS_TEST_ASSERT_EQUAL (txSocket->SendTo ( Create<Packet> (123), 0, 
    InetSocketAddress (Ipv4Address("255.255.255.255"), 1234)), 123);
  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_receivedPacket->GetSize (), 123);
  // second socket should not receive it (it is bound specifically to the second interface's address
  NS_TEST_ASSERT_EQUAL (m_receivedPacket2->GetSize (), 0);

  m_receivedPacket->RemoveAllByteTags ();
  m_receivedPacket2->RemoveAllByteTags ();

  // Broadcast test with multiple receiving sockets

  // When receiving broadcast packets, all sockets sockets bound to
  // the address/port should receive a copy of the same packet -- if
  // the socket address matches.
  rxSocket2->Dispose ();
  rxSocket2 = rxSocketFactory->CreateSocket ();
  rxSocket2->SetRecvCallback (MakeCallback (&UdpSocketImplTest::ReceivePkt2, this));
  NS_TEST_ASSERT_EQUAL (rxSocket2->Bind (InetSocketAddress (Ipv4Address ("0.0.0.0"), 1234)), 0);

  m_receivedPacket = Create<Packet> ();
  m_receivedPacket2 = Create<Packet> ();
  NS_TEST_ASSERT_EQUAL (txSocket->SendTo (Create<Packet> (123), 0,
InetSocketAddress (Ipv4Address("255.255.255.255"), 1234)), 123);
  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_receivedPacket->GetSize (), 123);
  NS_TEST_ASSERT_EQUAL (m_receivedPacket2->GetSize (), 123);

  m_receivedPacket = 0;
  m_receivedPacket2 = 0;

  Simulator::Destroy ();

  return result;
}

static UdpSocketImplTest gUdpSocketImplTest;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */
