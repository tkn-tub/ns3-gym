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


#include <stdint.h>
#include "ns3/ipv6-address.h"
#include "ns3/test.h"
#include "ipv6-packet-info-tag.h"

namespace ns3 {

Ipv6PacketInfoTag::Ipv6PacketInfoTag ()  
  : m_addr (Ipv6Address ()),
    m_ifindex (0),
    m_hoplimit (0),
    m_tclass (0)
{
}

void 
Ipv6PacketInfoTag::SetAddress (Ipv6Address addr)
{
  m_addr = addr;
}

Ipv6Address
Ipv6PacketInfoTag::GetAddress (void) const
{
  return m_addr;
}

void 
Ipv6PacketInfoTag::SetRecvIf (uint32_t ifindex)
{
  m_ifindex = ifindex;
}

uint32_t 
Ipv6PacketInfoTag::GetRecvIf (void) const
{
  return m_ifindex;
}

void 
Ipv6PacketInfoTag::SetHoplimit (uint8_t ttl)
{
  m_hoplimit = ttl;
}

uint8_t 
Ipv6PacketInfoTag::GetHoplimit (void) const
{
  return m_hoplimit;
}

void 
Ipv6PacketInfoTag::SetTrafficClass (uint8_t tclass)
{
  m_tclass = tclass;
}

uint8_t 
Ipv6PacketInfoTag::GetTrafficClass (void) const
{
  return m_tclass;
}


TypeId
Ipv6PacketInfoTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv6PacketInfoTag")
    .SetParent<Tag> ()
    .AddConstructor<Ipv6PacketInfoTag> ()
    ;
  return tid;
}
TypeId
Ipv6PacketInfoTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
Ipv6PacketInfoTag::GetSerializedSize (void) const
{ 
  return 16
    + sizeof (uint8_t)
    + sizeof (uint8_t)
    + sizeof (uint8_t);
}
void 
Ipv6PacketInfoTag::Serialize (TagBuffer i) const
{ 
  uint8_t buf[16];
  m_addr.Serialize (buf);
  i.Write (buf, 16);
  i.WriteU8 (m_ifindex);
  i.WriteU8 (m_hoplimit);
  i.WriteU8 (m_tclass);
}
void 
Ipv6PacketInfoTag::Deserialize (TagBuffer i)
{ 
  uint8_t buf[16];
  i.Read (buf, 16);
  m_addr.Deserialize (buf);
  m_ifindex = i.ReadU8 ();
  m_hoplimit = i.ReadU8 ();
  m_tclass = i.ReadU8 ();
}
void
Ipv6PacketInfoTag::Print (std::ostream &os) const
{
  os << "Ipv6 PKTINFO [DestAddr: " << m_addr;
  os << ", RecvIf:" << (uint32_t) m_ifindex;
  os << ", TTL:" << (uint32_t) m_hoplimit;
  os << ", TClass:" << (uint32_t) m_tclass;
  os << "] ";
}
}//namespace ns3

//-----------------------------------------------------------------------------
// Unit tests
//-----------------------------------------------------------------------------

#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/attribute.h"
#include "ns3/simple-net-device.h"
#include "ns3/object-factory.h"
#include "ns3/socket-factory.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/udp-socket.h"
#include "ns3/ipv6-l3-protocol.h"
#include "ns3/ipv6-raw-socket-factory.h"
#include "ns3/ipv6-interface.h"
#include "ns3/icmpv6-l4-protocol.h"
#include "ns3/ipv6-static-routing.h"
#include "ns3/ipv6-list-routing.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/node.h"

namespace ns3 {

static void
AddInternetStack (Ptr<Node> node)
{
  Ptr<Ipv6L3Protocol> ipv6 = CreateObject<Ipv6L3Protocol> ();
  Ptr<Icmpv6L4Protocol> icmpv6 = CreateObject<Icmpv6L4Protocol> ();
  node->AggregateObject (ipv6);
  node->AggregateObject (icmpv6);
  ipv6->Insert (icmpv6);
  icmpv6->SetAttribute ("DAD", BooleanValue (false));

  //Routing for Ipv6
  Ptr<Ipv6ListRouting> ipv6Routing = CreateObject<Ipv6ListRouting> ();
  ipv6->SetRoutingProtocol (ipv6Routing);
  Ptr<Ipv6StaticRouting> ipv6staticRouting = CreateObject<Ipv6StaticRouting> ();
  ipv6Routing->AddRoutingProtocol (ipv6staticRouting, 0);

  /* register IPv6 extensions and options */
  ipv6->RegisterExtensions ();
  ipv6->RegisterOptions ();
}

class Ipv6PacketInfoTagTest: public TestCase 
{
public:
  Ipv6PacketInfoTagTest ();
private:
  virtual void DoRun (void);
  void RxCb (Ptr<Socket> socket);
  void DoSendData (Ptr<Socket> socket, std::string to);
};

Ipv6PacketInfoTagTest::Ipv6PacketInfoTagTest ()
  : TestCase ("Ipv6PacketInfoTagTest") 
{}

void
Ipv6PacketInfoTagTest::RxCb (Ptr<Socket> socket)
{
  uint32_t availableData;
  Ptr<Packet> m_receivedPacket;

  availableData = socket->GetRxAvailable ();
  m_receivedPacket = socket->Recv (std::numeric_limits<uint32_t>::max(), 0);
  NS_ASSERT (availableData == m_receivedPacket->GetSize ());

  Ipv6PacketInfoTag tag;
  bool found;
  found = m_receivedPacket->RemovePacketTag (tag);
  NS_ASSERT (found);
  tag.Print (std::cout);
  std::cout << std::endl;
}

void
Ipv6PacketInfoTagTest::DoSendData (Ptr<Socket> socket, std::string to)
{
  Address realTo = Inet6SocketAddress (Ipv6Address (to.c_str()), 200);
  if (DynamicCast<UdpSocket> (socket) != 0)
    {
      NS_TEST_EXPECT_MSG_EQ (socket->SendTo (Create<Packet> (123), 0, realTo),
                             123, "XXX");
    }
  // Should only Ipv6RawSock
  else
    {
      socket->SendTo (Create<Packet> (123), 0, realTo);
    }
}

void
Ipv6PacketInfoTagTest::DoRun (void)
{
  Ptr<Node> node0 = CreateObject<Node> ();
  Ptr<Node> node1 = CreateObject<Node> ();

  Ptr<SimpleNetDevice> device = CreateObject<SimpleNetDevice> ();
  Ptr<SimpleNetDevice> device2 = CreateObject<SimpleNetDevice> ();

  // For Node 0
  node0->AddDevice (device);
  AddInternetStack (node0);
  Ptr<Ipv6> ipv6 = node0->GetObject<Ipv6> ();

  uint32_t index = ipv6->AddInterface (device);
  Ipv6InterfaceAddress ifaceAddr1 = Ipv6InterfaceAddress (Ipv6Address("2000:1000:0:2000::1"), 
                                                          Ipv6Prefix(64));
  ipv6->AddAddress (index, ifaceAddr1);
  ipv6->SetMetric (index, 1);
  ipv6->SetUp (index);

  // For Node 1
  node1->AddDevice (device2);
  AddInternetStack (node1);
  ipv6 = node1->GetObject<Ipv6> ();

  index = ipv6->AddInterface (device2);
  Ipv6InterfaceAddress ifaceAddr2 = Ipv6InterfaceAddress (Ipv6Address("2000:1000:0:2000::2"), 
                                                          Ipv6Prefix(64));
  ipv6->AddAddress (index, ifaceAddr2);
  ipv6->SetMetric (index, 1);
  ipv6->SetUp (index);

  // ipv6 w rawsocket
  Ptr<SocketFactory> factory = node0->GetObject<SocketFactory> (Ipv6RawSocketFactory::GetTypeId ());
  Ptr<Socket> socket = factory->CreateSocket ();
  Inet6SocketAddress local =  Inet6SocketAddress (Ipv6Address::GetAny (), 0);
  socket->SetAttribute ("Protocol", UintegerValue (Ipv6Header::IPV6_ICMPV6));
  socket->Bind (local);
  socket->SetRecvPktInfo (true);
  socket->SetRecvCallback (MakeCallback (&ns3::Ipv6PacketInfoTagTest::RxCb, this));

  // receive on loopback 
  Simulator::ScheduleWithContext (socket->GetNode ()->GetId (), Seconds (0),
                                  &Ipv6PacketInfoTagTest::DoSendData, this, socket, "::1");
  Simulator::Run ();

  Ptr<SocketFactory> factory2 = node1->GetObject<SocketFactory> (Ipv6RawSocketFactory::GetTypeId ());
  Ptr<Socket> socket2 = factory2->CreateSocket ();
  std::stringstream dst;
  dst << ifaceAddr1.GetAddress ();
  Simulator::ScheduleWithContext (socket2->GetNode ()->GetId (), Seconds (0),
                                  &Ipv6PacketInfoTagTest::DoSendData, this, socket, 
                                  dst.str ());
  Simulator::Run ();

#ifdef UDP6_SUPPORTED
  // IPv6 test
  factory = node0->GetObject<SocketFactory> (UdpSocketFactory::GetTypeId ());
  socket = factory->CreateSocket ();
  local =  Inet6SocketAddress (Ipv6Address::GetAny (), 200);
  socket->Bind (local);
  socket->SetRecvPktInfo (true);
  socket->SetRecvCallback (MakeCallback (&ns3::Ipv6PacketInfoTagTest::RxCb, this));

  // receive on loopback 
  Simulator::ScheduleWithContext (socket->GetNode ()->GetId (), Seconds (0),
                                  &Ipv6PacketInfoTagTest::DoSendData, this, socket, "::1");
  Simulator::Run ();

  factory2 = node1->GetObject<SocketFactory> (UdpSocketFactory::GetTypeId ());
  socket2 = factory2->CreateSocket ();
  Simulator::ScheduleWithContext (socket2->GetNode ()->GetId (), Seconds (0),
                                  &Ipv6PacketInfoTagTest::DoSendData, this, socket, "10.1.1.1");
  Simulator::Run ();

#endif  // UDP6_SUPPORTED

  Simulator::Destroy ();
  // IPv6 test
}

static class Ipv6PacketInfoTagTestSuite : public TestSuite
{
public:
  Ipv6PacketInfoTagTestSuite ();
private:
} g_packetinfotagTests;

Ipv6PacketInfoTagTestSuite::Ipv6PacketInfoTagTestSuite ()
  : TestSuite ("ipv6-packet-info-tag", UNIT)
{
  AddTestCase (new Ipv6PacketInfoTagTest ());
}


}

