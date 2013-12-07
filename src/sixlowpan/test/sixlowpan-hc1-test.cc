/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Universita' di Firenze, Italy
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
#include "ns3/ipv6-list-routing.h"
#include "ns3/ipv6-static-routing.h"

#include "ns3/sixlowpan-net-device.h"

#include <string>
#include <limits>

using namespace ns3;

static void
AddInternetStack6 (Ptr<Node> node)
{
  //IPV6
  Ptr<Ipv6L3Protocol> ipv6 = CreateObject<Ipv6L3Protocol> ();
  //Routing for Ipv6
  Ptr<Ipv6ListRouting> ipv6Routing = CreateObject<Ipv6ListRouting> ();
  ipv6->SetRoutingProtocol (ipv6Routing);
  Ptr<Ipv6StaticRouting> ipv6staticRouting = CreateObject<Ipv6StaticRouting> ();
  ipv6Routing->AddRoutingProtocol (ipv6staticRouting, 0);
  node->AggregateObject (ipv6);
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


class SixlowpanHc1ImplTest : public TestCase
{
  Ptr<Packet> m_receivedPacket;
  void DoSendData (Ptr<Socket> socket, std::string to);
  void SendData (Ptr<Socket> socket, std::string to);

public:
  virtual void DoRun (void);
  SixlowpanHc1ImplTest ();

  void ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from);
  void ReceivePkt (Ptr<Socket> socket);
};

SixlowpanHc1ImplTest::SixlowpanHc1ImplTest ()
  : TestCase ("Sixlowpan implementation")
{
}

void SixlowpanHc1ImplTest::ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from)
{
  m_receivedPacket = packet;
}

void SixlowpanHc1ImplTest::ReceivePkt (Ptr<Socket> socket)
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
SixlowpanHc1ImplTest::DoSendData (Ptr<Socket> socket, std::string to)
{
  Address realTo = Inet6SocketAddress (Ipv6Address (to.c_str ()), 1234);
  uint8_t buffer [] = "\"Can you tell me where my country lies?\" \\ said the unifaun to his true love's eyes. \\ \"It lies with me!\" cried the Queen of Maybe \\ - for her merchandise, he traded in his prize.";

  Ptr<Packet> packet = Create<Packet> (buffer, 180);
  NS_TEST_EXPECT_MSG_EQ (socket->SendTo (packet, 0, realTo),
                         180, "200");
}

void
SixlowpanHc1ImplTest::SendData (Ptr<Socket> socket, std::string to)
{
  m_receivedPacket = Create<Packet> ();
  Simulator::ScheduleWithContext (socket->GetNode ()->GetId (), Seconds (0),
                                  &SixlowpanHc1ImplTest::DoSendData, this, socket, to);
  Simulator::Run ();
}

void
SixlowpanHc1ImplTest::DoRun (void)
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

    Ptr<SixLowPanNetDevice> rxSix = CreateObject<SixLowPanNetDevice> ();
    rxSix->SetAttribute ("ForceEtherType", BooleanValue (true) );
    rxSix->SetAttribute ("Rfc6282", BooleanValue (false) );
    rxNode->AddDevice (rxSix);
    rxSix->SetNetDevice (rxDev);

    Ptr<Ipv6> ipv6 = rxNode->GetObject<Ipv6> ();
    ipv6->AddInterface (rxDev);
    uint32_t netdev_idx = ipv6->AddInterface (rxSix);
    Ipv6InterfaceAddress ipv6Addr = Ipv6InterfaceAddress (Ipv6Address ("2001:0100::1"), Ipv6Prefix (64));
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

    Ptr<SixLowPanNetDevice> txSix = CreateObject<SixLowPanNetDevice> ();
    txSix->SetAttribute ("ForceEtherType", BooleanValue (true) );
    txSix->SetAttribute ("Rfc6282", BooleanValue (false) );
    txNode->AddDevice (txSix);
    txSix->SetNetDevice (txDev);

    Ptr<Ipv6> ipv6 = txNode->GetObject<Ipv6> ();
    ipv6->AddInterface (txDev);
    uint32_t netdev_idx = ipv6->AddInterface (txSix);
    Ipv6InterfaceAddress ipv6Addr = Ipv6InterfaceAddress (Ipv6Address ("2001:0100::2"), Ipv6Prefix (64));
    ipv6->AddAddress (netdev_idx, ipv6Addr);
    ipv6->SetUp (netdev_idx);
  }

  // link the two nodes
  Ptr<SimpleChannel> channel1 = CreateObject<SimpleChannel> ();
  rxDev->SetChannel (channel1);
  txDev->SetChannel (channel1);

  // Create the UDP sockets
  Ptr<SocketFactory> rxSocketFactory = rxNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> rxSocket = rxSocketFactory->CreateSocket ();
  NS_TEST_EXPECT_MSG_EQ (rxSocket->Bind (Inet6SocketAddress (Ipv6Address ("2001:0100::1"), 1234)), 0, "trivial");
  rxSocket->SetRecvCallback (MakeCallback (&SixlowpanHc1ImplTest::ReceivePkt, this));

  Ptr<SocketFactory> txSocketFactory = txNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> txSocket = txSocketFactory->CreateSocket ();
  txSocket->SetAllowBroadcast (true);
  // ------ Now the tests ------------

  // Unicast test
  SendData (txSocket, "2001:0100::1");
  NS_TEST_EXPECT_MSG_EQ (m_receivedPacket->GetSize (), 180, "trivial");
  uint8_t rxBuffer [180];
  uint8_t txBuffer [180] = "\"Can you tell me where my country lies?\" \\ said the unifaun to his true love's eyes. \\ \"It lies with me!\" cried the Queen of Maybe \\ - for her merchandise, he traded in his prize.";
  m_receivedPacket->CopyData (rxBuffer, 180);
  NS_TEST_EXPECT_MSG_EQ (memcmp (rxBuffer, txBuffer, 180), 0, "trivial");

  m_receivedPacket->RemoveAllByteTags ();

  Simulator::Destroy ();

}


//-----------------------------------------------------------------------------
class SixlowpanHc1TestSuite : public TestSuite
{
public:
  SixlowpanHc1TestSuite () : TestSuite ("sixlowpan-hc1", UNIT)
  {
    AddTestCase (new SixlowpanHc1ImplTest, TestCase::QUICK);
  }
} g_sixlowpanHc1TestSuite;
