/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Hajime Tazaki
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
 * Authors: Hajime Tazaki <tazaki@sfc.wide.ad.jp>
 */

//-----------------------------------------------------------------------------
// Unit tests
//-----------------------------------------------------------------------------

#include <string>

#include "ns3/test.h"
#include "ns3/ipv4-packet-info-tag.h"
#include "ns3/ipv4-address.h"
#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/attribute.h"
#include "ns3/simple-net-device.h"
#include "ns3/object-factory.h"
#include "ns3/socket-factory.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/udp-socket.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv4-raw-socket-factory.h"
#include "ns3/ipv4-interface.h"
#include "ns3/arp-l3-protocol.h"
#include "ns3/icmpv4-l4-protocol.h"
#include "ns3/ipv4-static-routing.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/udp-l4-protocol.h"
#include "ns3/tcp-l4-protocol.h"
#include "ns3/simulator.h"
#include "ns3/node.h"

using namespace ns3;

namespace {

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
}

}

class Ipv4PacketInfoTagTest : public TestCase
{
public:
  Ipv4PacketInfoTagTest ();
private:
  virtual void DoRun (void);
  void RxCb (Ptr<Socket> socket);
  void DoSendData (Ptr<Socket> socket, std::string to);
};

Ipv4PacketInfoTagTest::Ipv4PacketInfoTagTest ()
  : TestCase ("Ipv4PacketInfoTagTest") 
{
}

void
Ipv4PacketInfoTagTest::RxCb (Ptr<Socket> socket)
{
  uint32_t availableData;
  Ptr<Packet> m_receivedPacket;

  availableData = socket->GetRxAvailable ();
  m_receivedPacket = socket->Recv (std::numeric_limits<uint32_t>::max (), 0);
  NS_TEST_ASSERT_MSG_EQ (availableData, m_receivedPacket->GetSize (), "Did not read expected data");

  Ipv4PacketInfoTag tag;
  bool found;
  found = m_receivedPacket->RemovePacketTag (tag);
  NS_TEST_ASSERT_MSG_EQ (found, true, "Could not find tag");
}

void
Ipv4PacketInfoTagTest::DoSendData (Ptr<Socket> socket, std::string to)
{
  Address realTo = InetSocketAddress (Ipv4Address (to.c_str ()), 200);
  if (DynamicCast<UdpSocket> (socket) != 0)
    {
      NS_TEST_EXPECT_MSG_EQ (socket->SendTo (Create<Packet> (123), 0, realTo),
                             123, "XXX");
    }
  // Should only Ipv4RawSock
  else
    {
      socket->SendTo (Create<Packet> (123), 0, realTo);
    }
}

void
Ipv4PacketInfoTagTest::DoRun (void)
{
  Ptr<Node> node0 = CreateObject<Node> ();
  Ptr<Node> node1 = CreateObject<Node> ();

  Ptr<SimpleNetDevice> device = CreateObject<SimpleNetDevice> ();
  Ptr<SimpleNetDevice> device2 = CreateObject<SimpleNetDevice> ();

  // For Node 0
  node0->AddDevice (device);
  AddInternetStack (node0);
  Ptr<Ipv4> ipv4 = node0->GetObject<Ipv4> ();

  uint32_t index = ipv4->AddInterface (device);
  Ipv4InterfaceAddress ifaceAddr1 = Ipv4InterfaceAddress ("10.1.1.1",
                                                          "255.255.255.0");
  ipv4->AddAddress (index, ifaceAddr1);
  ipv4->SetMetric (index, 1);
  ipv4->SetUp (index);

  // For Node 1
  node1->AddDevice (device2);
  AddInternetStack (node1);
  ipv4 = node1->GetObject<Ipv4> ();

  index = ipv4->AddInterface (device2);
  Ipv4InterfaceAddress ifaceAddr2 = Ipv4InterfaceAddress ("10.1.1.2",
                                                          "255.255.255.0");
  ipv4->AddAddress (index, ifaceAddr2);
  ipv4->SetMetric (index, 1);
  ipv4->SetUp (index);

  // IPv4 test
  Ptr<SocketFactory> factory = node0->GetObject<SocketFactory> (UdpSocketFactory::GetTypeId ());
  Ptr<Socket> socket = factory->CreateSocket ();
  InetSocketAddress local =  InetSocketAddress (Ipv4Address::GetAny (), 200);
  socket->Bind (local);
  socket->SetRecvPktInfo (true);
  socket->SetRecvCallback (MakeCallback (&Ipv4PacketInfoTagTest::RxCb, this));

  // receive on loopback 
  Simulator::ScheduleWithContext (socket->GetNode ()->GetId (), Seconds (0),
                                  &Ipv4PacketInfoTagTest::DoSendData, this, socket, "127.0.0.1");
  Simulator::Run ();

  Ptr<SocketFactory> factory2 = node1->GetObject<SocketFactory> (UdpSocketFactory::GetTypeId ());
  Ptr<Socket> socket2 = factory2->CreateSocket ();
  Simulator::ScheduleWithContext (socket2->GetNode ()->GetId (), Seconds (0),
                                  &Ipv4PacketInfoTagTest::DoSendData, this, socket, "10.1.1.1");
  Simulator::Run ();

  // ipv4 w rawsocket
  factory = node0->GetObject<SocketFactory> (Ipv4RawSocketFactory::GetTypeId ());
  socket = factory->CreateSocket ();
  local =  InetSocketAddress (Ipv4Address::GetAny (), 0);
  socket->Bind (local);
  socket->SetRecvPktInfo (true);
  socket->SetRecvCallback (MakeCallback (&Ipv4PacketInfoTagTest::RxCb, this));

  // receive on loopback 
  Simulator::ScheduleWithContext (socket->GetNode ()->GetId (), Seconds (0),
                                  &Ipv4PacketInfoTagTest::DoSendData, this, socket, "127.0.0.1");
  Simulator::Run ();

  factory2 = node1->GetObject<SocketFactory> (Ipv4RawSocketFactory::GetTypeId ());
  socket2 = factory2->CreateSocket ();
  Simulator::ScheduleWithContext (socket2->GetNode ()->GetId (), Seconds (0),
                                  &Ipv4PacketInfoTagTest::DoSendData, this, socket, "10.1.1.1");
  Simulator::Run ();
  Simulator::Destroy ();
}

static class Ipv4PacketInfoTagTestSuite : public TestSuite
{
public:
  Ipv4PacketInfoTagTestSuite ();
private:
} g_packetinfotagTests;

Ipv4PacketInfoTagTestSuite::Ipv4PacketInfoTagTestSuite ()
  : TestSuite ("ipv4-packet-info-tag", UNIT)
{
  AddTestCase (new Ipv4PacketInfoTagTest (), TestCase::QUICK);
}
