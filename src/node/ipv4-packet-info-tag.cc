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
#include "ns3/ipv4-address.h"
#include "ns3/test.h"
#include "ipv4-packet-info-tag.h"

namespace ns3 {

Ipv4PacketInfoTag::Ipv4PacketInfoTag ()  
  : m_addr (Ipv4Address ()),
    m_spec_dst (Ipv4Address ()),
    m_ifindex (0),
    m_ttl (0)
{
}

void 
Ipv4PacketInfoTag::SetAddress (Ipv4Address addr)
{
  m_addr = addr;
}

Ipv4Address
Ipv4PacketInfoTag::GetAddress (void) const
{
  return m_addr;
}

void 
Ipv4PacketInfoTag::SetLocalAddress (Ipv4Address addr)
{
  m_spec_dst = addr;
}

Ipv4Address
Ipv4PacketInfoTag::GetLocalAddress (void) const
{
  return m_spec_dst;
}

void 
Ipv4PacketInfoTag::SetRecvIf (uint32_t ifindex)
{
  m_ifindex = ifindex;
}

uint32_t 
Ipv4PacketInfoTag::GetRecvIf (void) const
{
  return m_ifindex;
}

void 
Ipv4PacketInfoTag::SetTtl (uint8_t ttl)
{
  m_ttl = ttl;
}

uint8_t 
Ipv4PacketInfoTag::GetTtl (void) const
{
  return m_ttl;
}



TypeId
Ipv4PacketInfoTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4PacketInfoTag")
    .SetParent<Tag> ()
    .AddConstructor<Ipv4PacketInfoTag> ()
    ;
  return tid;
}
TypeId
Ipv4PacketInfoTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
Ipv4PacketInfoTag::GetSerializedSize (void) const
{ 
  return 4 + 4 
    + sizeof (uint32_t)
    + sizeof (uint8_t);
}
void 
Ipv4PacketInfoTag::Serialize (TagBuffer i) const
{ 
  uint8_t buf[4];
  m_addr.Serialize (buf);
  i.Write (buf, 4);
  m_spec_dst.Serialize (buf);
  i.Write (buf, 4);
  i.WriteU32 (m_ifindex);
  i.WriteU8 (m_ttl);
}
void 
Ipv4PacketInfoTag::Deserialize (TagBuffer i)
{ 
  uint8_t buf[4];
  i.Read (buf, 4);
  m_addr.Deserialize (buf);
  i.Read (buf, 4);
  m_spec_dst.Deserialize (buf);
  m_ifindex = i.ReadU32 ();
  m_ttl = i.ReadU8 ();
}
void
Ipv4PacketInfoTag::Print (std::ostream &os) const
{
  os << "Ipv4 PKTINFO [DestAddr: " << m_addr;
  os << ", Local Address:" << m_spec_dst;
  os << ", RecvIf:" << (uint32_t) m_ifindex;
  os << ", TTL:" << (uint32_t) m_ttl;
  os << "] ";
}
}//namespace ns3

//-----------------------------------------------------------------------------
// Unit tests
//-----------------------------------------------------------------------------

#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/socket-factory.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/udp-socket.h"
#include "ns3/ipv4-raw-socket-factory.h"
#include "ns3/inet-socket-address.h"
#include "ns3/simulator.h"

namespace ns3 {

class Ipv4PacketInfoTagTest: public TestCase 
{
public:
  Ipv4PacketInfoTagTest ();
private:
  virtual bool DoRun (void);
  void RxCb (Ptr<Socket> socket);
  void DoSendData (Ptr<Socket> socket, std::string to);
};

Ipv4PacketInfoTagTest::Ipv4PacketInfoTagTest ()
  : TestCase ("Ipv4PacketInfoTagTest") 
{}

void
Ipv4PacketInfoTagTest::RxCb (Ptr<Socket> socket)
{
  uint32_t availableData;
  Ptr<Packet> m_receivedPacket;

  availableData = socket->GetRxAvailable ();
  m_receivedPacket = socket->Recv (std::numeric_limits<uint32_t>::max(), 0);
  NS_ASSERT (availableData == m_receivedPacket->GetSize ());

  Ipv4PacketInfoTag tag;
  bool found;
  found = m_receivedPacket->RemovePacketTag (tag);
  NS_ASSERT (found);
  tag.Print (std::cout);
}

void
Ipv4PacketInfoTagTest::DoSendData (Ptr<Socket> socket, std::string to)
{
  Address realTo = InetSocketAddress (Ipv4Address (to.c_str()), 200);
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

bool
Ipv4PacketInfoTagTest::DoRun (void)
{
  NodeContainer n;
  n.Create (2);

  InternetStackHelper internet;
  internet.Install (n);

  CsmaHelper csma;
  NetDeviceContainer d = csma.Install (n);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (d);

  // IPv4 test
  Ptr<SocketFactory> factory = n.Get (0)->GetObject<SocketFactory> (UdpSocketFactory::GetTypeId ());
  Ptr<Socket> socket = factory->CreateSocket ();
  InetSocketAddress local =  InetSocketAddress (Ipv4Address::GetAny (), 200);
  socket->Bind (local);
  socket->SetRecvPktInfo (true);
  socket->SetRecvCallback (MakeCallback (&ns3::Ipv4PacketInfoTagTest::RxCb, this));

  // receive on loopback 
  Simulator::ScheduleWithContext (socket->GetNode ()->GetId (), Seconds (0),
                                  &Ipv4PacketInfoTagTest::DoSendData, this, socket, "127.0.0.1");
  Simulator::Run ();

  // send from node1 and recved via csma
  Ptr<SocketFactory> factory2 = n.Get (1)->GetObject<SocketFactory> (UdpSocketFactory::GetTypeId ());
  Ptr<Socket> socket2 = factory2->CreateSocket ();
  Simulator::ScheduleWithContext (socket2->GetNode ()->GetId (), Seconds (0),
                                  &Ipv4PacketInfoTagTest::DoSendData, this, socket, "10.1.1.1");
  Simulator::Run ();

  // ipv4 w rawsocket
  factory = n.Get (0)->GetObject<SocketFactory> (Ipv4RawSocketFactory::GetTypeId ());
  socket = factory->CreateSocket ();
  local =  InetSocketAddress (Ipv4Address::GetAny (), 0);
  socket->Bind (local);
  socket->SetRecvPktInfo (true);
  socket->SetRecvCallback (MakeCallback (&ns3::Ipv4PacketInfoTagTest::RxCb, this));

  // receive on loopback 
  Simulator::ScheduleWithContext (socket->GetNode ()->GetId (), Seconds (0),
                                  &Ipv4PacketInfoTagTest::DoSendData, this, socket, "127.0.0.1");
  Simulator::Run ();

  // send from node1 and recved via csma
  factory2 = n.Get (1)->GetObject<SocketFactory> (Ipv4RawSocketFactory::GetTypeId ());
  socket2 = factory2->CreateSocket ();
  Simulator::ScheduleWithContext (socket2->GetNode ()->GetId (), Seconds (0),
                                  &Ipv4PacketInfoTagTest::DoSendData, this, socket, "10.1.1.1");
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
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
  AddTestCase (new Ipv4PacketInfoTagTest ());
}


}
