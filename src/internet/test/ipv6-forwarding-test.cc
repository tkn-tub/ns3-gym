/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Universita' di Firenze
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
 * Author: Tommaso Pecorella <tommaso.pecorella@unifi.it>
 */

#include "ns3/test.h"
#include "ns3/socket-factory.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/simple-channel.h"
#include "ns3/simple-net-device.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/socket.h"
#include "ns3/boolean.h"

#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/inet6-socket-address.h"

#include "ns3/ipv6-l3-protocol.h"
#include "ns3/icmpv6-l4-protocol.h"
#include "ns3/udp-l4-protocol.h"
#include "ns3/ipv6-static-routing.h"

#include <string>
#include <limits>

using namespace ns3;

static void
AddInternetStack6 (Ptr<Node> node)
{
  //IPV6
  Ptr<Ipv6L3Protocol> ipv6 = CreateObject<Ipv6L3Protocol> ();
  //Routing for Ipv6
  Ptr<Ipv6StaticRouting> ipv6Routing = CreateObject<Ipv6StaticRouting> ();
  ipv6->SetRoutingProtocol (ipv6Routing);
  node->AggregateObject (ipv6);
  node->AggregateObject (ipv6Routing);
  //ICMP
  Ptr<Icmpv6L4Protocol> icmp = CreateObject<Icmpv6L4Protocol> ();
  node->AggregateObject (icmp);
  //Ipv6 Extensions
  ipv6->RegisterExtensions ();
  ipv6->RegisterOptions ();
  //UDP
  Ptr<UdpL4Protocol> udp = CreateObject<UdpL4Protocol> ();
  node->AggregateObject (udp);
}



class Ipv6ForwardingTest : public TestCase
{
  Ptr<Packet> m_receivedPacket;
  void DoSendData (Ptr<Socket> socket, std::string to);
  void SendData (Ptr<Socket> socket, std::string to);

public:
  virtual void DoRun (void);
  Ipv6ForwardingTest ();

  void ReceivePkt (Ptr<Socket> socket);
};

Ipv6ForwardingTest::Ipv6ForwardingTest ()
  : TestCase ("UDP6 socket implementation")
{
}

void Ipv6ForwardingTest::ReceivePkt (Ptr<Socket> socket)
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
Ipv6ForwardingTest::DoSendData (Ptr<Socket> socket, std::string to)
{
  Address realTo = Inet6SocketAddress (Ipv6Address (to.c_str ()), 1234);
  NS_TEST_EXPECT_MSG_EQ (socket->SendTo (Create<Packet> (123), 0, realTo),
                         123, "100");
}

void
Ipv6ForwardingTest::SendData (Ptr<Socket> socket, std::string to)
{
  m_receivedPacket = Create<Packet> ();
  Simulator::ScheduleWithContext (socket->GetNode ()->GetId (), Seconds (0),
                                  &Ipv6ForwardingTest::DoSendData, this, socket, to);
  Simulator::Run ();
}

void
Ipv6ForwardingTest::DoRun (void)
{
  // Create topology

  // Receiver Node
  Ptr<Node> rxNode = CreateObject<Node> ();
  AddInternetStack6 (rxNode);
  Ptr<SimpleNetDevice> rxDev;
  { // first interface
    rxDev = CreateObject<SimpleNetDevice> ();
    rxDev->SetAddress (Mac48Address::ConvertFrom (Mac48Address::Allocate ()));
    rxNode->AddDevice (rxDev);
    Ptr<Ipv6> ipv6 = rxNode->GetObject<Ipv6> ();
    uint32_t netdev_idx = ipv6->AddInterface (rxDev);
    Ipv6InterfaceAddress ipv6Addr = Ipv6InterfaceAddress (Ipv6Address ("2001:1::2"), Ipv6Prefix (64));
    ipv6->AddAddress (netdev_idx, ipv6Addr);
    ipv6->SetUp (netdev_idx);
  }

  // Forwarding Node
  Ptr<Node> fwNode = CreateObject<Node> ();
  AddInternetStack6 (fwNode);
  Ptr<SimpleNetDevice> fwDev1, fwDev2;
  { // first interface
    fwDev1 = CreateObject<SimpleNetDevice> ();
    fwDev1->SetAddress (Mac48Address::ConvertFrom (Mac48Address::Allocate ()));
    fwNode->AddDevice (fwDev1);
    Ptr<Ipv6> ipv6 = fwNode->GetObject<Ipv6> ();
    uint32_t netdev_idx = ipv6->AddInterface (fwDev1);
    Ipv6InterfaceAddress ipv6Addr = Ipv6InterfaceAddress (Ipv6Address ("2001:1::1"), Ipv6Prefix (64));
    ipv6->AddAddress (netdev_idx, ipv6Addr);
    ipv6->SetUp (netdev_idx);
  }

  Ipv6Address nextHop;
  { // second interface
    fwDev2 = CreateObject<SimpleNetDevice> ();
    fwDev2->SetAddress (Mac48Address::ConvertFrom (Mac48Address::Allocate ()));
    fwNode->AddDevice (fwDev2);
    Ptr<Ipv6> ipv6 = fwNode->GetObject<Ipv6> ();
    uint32_t netdev_idx = ipv6->AddInterface (fwDev2);
    Ipv6InterfaceAddress ipv6Addr = Ipv6InterfaceAddress (Ipv6Address ("2001:2::1"), Ipv6Prefix (64));
    nextHop = ipv6->GetAddress(netdev_idx, 0).GetAddress();
    ipv6->AddAddress (netdev_idx, ipv6Addr);
    ipv6->SetUp (netdev_idx);
  }

  // Sender Node
  Ptr<Node> txNode = CreateObject<Node> ();
  AddInternetStack6 (txNode);
  Ptr<SimpleNetDevice> txDev;
  {
    txDev = CreateObject<SimpleNetDevice> ();
    txDev->SetAddress (Mac48Address::ConvertFrom (Mac48Address::Allocate ()));
    txNode->AddDevice (txDev);
    Ptr<Ipv6> ipv6 = txNode->GetObject<Ipv6> ();
    uint32_t netdev_idx = ipv6->AddInterface (txDev);
    Ipv6InterfaceAddress ipv6Addr = Ipv6InterfaceAddress (Ipv6Address ("2001:2::2"), Ipv6Prefix (64));
    ipv6->AddAddress (netdev_idx, ipv6Addr);
    ipv6->SetUp (netdev_idx);
    Ptr<Ipv6StaticRouting> ipv6StaticRouting = txNode->GetObject<Ipv6StaticRouting> ();
    ipv6StaticRouting->SetDefaultRoute(nextHop, netdev_idx);
  }

  // link the two nodes
  Ptr<SimpleChannel> channel1 = CreateObject<SimpleChannel> ();
  rxDev->SetChannel (channel1);
  fwDev1->SetChannel (channel1);

  Ptr<SimpleChannel> channel2 = CreateObject<SimpleChannel> ();
  fwDev2->SetChannel (channel2);
  txDev->SetChannel (channel2);

  // Create the UDP sockets
  Ptr<SocketFactory> rxSocketFactory = rxNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> rxSocket = rxSocketFactory->CreateSocket ();
  NS_TEST_EXPECT_MSG_EQ (rxSocket->Bind (Inet6SocketAddress (Ipv6Address ("2001:1::2"), 1234)), 0, "trivial");
  rxSocket->SetRecvCallback (MakeCallback (&Ipv6ForwardingTest::ReceivePkt, this));

  Ptr<SocketFactory> txSocketFactory = txNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> txSocket = txSocketFactory->CreateSocket ();
  txSocket->SetAllowBroadcast (true);

  // ------ Now the tests ------------

  // Unicast test
  SendData (txSocket, "2001:1::2");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket->GetSize (), 0, "IPv6 Forwarding off");

  m_receivedPacket->RemoveAllByteTags ();
  m_receivedPacket = 0;

  Ptr<Ipv6> ipv6 = fwNode->GetObject<Ipv6> ();
  ipv6->SetAttribute("IpForward", BooleanValue (true));
  SendData (txSocket, "2001:1::2");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket->GetSize (), 123, "IPv6 Forwarding on");

  m_receivedPacket->RemoveAllByteTags ();

  Simulator::Destroy ();

}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class Ipv6ForwardingTestSuite : public TestSuite
{
public:
  Ipv6ForwardingTestSuite () : TestSuite ("ipv6-forwarding", UNIT)
  {
    AddTestCase (new Ipv6ForwardingTest, TestCase::QUICK);
  }
} g_ipv6forwardingTestSuite;
