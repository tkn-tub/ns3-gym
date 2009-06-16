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
 * This is the test code for tcp-socket-impl.cc, it was moved out of tcp-socket-impl.cc 
 * to be in an independent file for clarity purposes.
 */
#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include "ns3/socket-factory.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/simple-channel.h"
#include "ns3/simple-net-device.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/config.h"
#include "ipv4-end-point.h"
#include "arp-l3-protocol.h"
#include "ipv4-l3-protocol.h"
#include "icmpv4-l4-protocol.h"
#include "udp-l4-protocol.h"
#include "tcp-l4-protocol.h"
#include "ipv4-static-routing-impl.h"
#include "ipv4-list-routing-impl.h"

#include "ns3/node.h"
#include "ns3/inet-socket-address.h"

#include <string>

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
  //Routing for Ipv4
  Ptr<Ipv4ListRoutingImpl> ipv4RoutingImpl = CreateObject<Ipv4ListRoutingImpl> ();
  ipv4->SetRoutingProtocol (ipv4RoutingImpl);
  ipv4RoutingImpl->SetNode (node);
  Ptr<Ipv4StaticRoutingImpl> ipv4staticRoutingImpl = CreateObject<Ipv4StaticRoutingImpl> ();
  ipv4staticRoutingImpl->SetNode (node);
  ipv4RoutingImpl->AddRoutingProtocol (ipv4staticRoutingImpl, 0);
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

class TcpSocketImplTest: public Test
{
  public:
  TcpSocketImplTest ();
  virtual bool RunTests (void);
  private:
  //test 1, which sends string "Hello world" server->client
  void Test1 (void);
  void Test1_HandleConnectionCreated (Ptr<Socket>, const Address &);
  void Test1_HandleRecv (Ptr<Socket> sock);

  //test 2, which sends a number of bytes server->client
  void Test2 (uint32_t payloadSize);
  void Test2_HandleConnectionCreated (Ptr<Socket>, const Address &);
  void Test2_HandleRecv (Ptr<Socket> sock);
  uint32_t test2_payloadSize;

  //test 3, which makes sure the rx buffer is finite
  void Test3 (uint32_t payloadSize);
  void Test3_HandleConnectionCreated (Ptr<Socket>, const Address &);
  void Test3_HandleRecv (Ptr<Socket> sock);
  uint32_t test3_payloadSize;

  //helpers to make topology construction easier
  Ptr<Node> CreateInternetNode ();
  Ptr<SimpleNetDevice> AddSimpleNetDevice (Ptr<Node>,const char*,const char*);
  void SetupDefaultSim ();

  //reset all of the below state for another run
  void Reset ();

  //all of the state this class needs; basically both ends of the connection,
  //and this test kind of acts as an single application running on both nodes
  //simultaneously
  Ptr<Node> node0;
  Ptr<Node> node1;
  Ptr<SimpleNetDevice> dev0;
  Ptr<SimpleNetDevice> dev1;
  Ptr<SimpleChannel> channel;
  Ptr<Socket> listeningSock;
  Ptr<Socket> sock0;
  Ptr<Socket> sock1;
  uint32_t rxBytes0;
  uint32_t rxBytes1;

  uint8_t* rxPayload;

  bool result;
};

TcpSocketImplTest::TcpSocketImplTest ()
  : Test ("TcpSocketImpl"), 
    rxBytes0 (0),
    rxBytes1 (0),
    rxPayload (0),
    result (true)
{
}

bool
TcpSocketImplTest::RunTests (void)
{
  Test1();
  if (!result) return false;
  Test2(600);
  if (!result) return false;
  Test3(20000);
  return result;
}

//-----------------------------------------------------------------------------
//test 1-----------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
TcpSocketImplTest::Test1 ()
{
  SetupDefaultSim ();
  listeningSock->SetAcceptCallback 
      (MakeNullCallback<bool, Ptr< Socket >, const Address &> (),
       MakeCallback(&TcpSocketImplTest::Test1_HandleConnectionCreated,this));
  sock1->SetRecvCallback (MakeCallback(&TcpSocketImplTest::Test1_HandleRecv, this));

  Simulator::Run ();
  Simulator::Destroy ();

  result = result && (rxBytes1 == 13);
  result = result && (strcmp((const char*) rxPayload,"Hello World!") == 0);

  Reset ();
}

void
TcpSocketImplTest::Test1_HandleConnectionCreated (Ptr<Socket> s, const Address & addr)
{
  NS_ASSERT(s != listeningSock);
  NS_ASSERT(sock0 == 0);
  sock0 = s;
  const uint8_t* hello = (uint8_t*)"Hello World!";
  Ptr<Packet> p = Create<Packet> (hello, 13);
  sock0->Send(p);
  
  sock0->SetRecvCallback (MakeCallback(&TcpSocketImplTest::Test1_HandleRecv, this));
}

void
TcpSocketImplTest::Test1_HandleRecv (Ptr<Socket> sock)
{
  NS_ASSERT (sock == sock0 || sock == sock1);
  Ptr<Packet> p = sock->Recv();
  uint32_t sz = p->GetSize();
  if (sock == sock1)
  {
    rxBytes1 += sz;
    rxPayload = new uint8_t[sz];
    p->CopyData (rxPayload, sz);
  }
  else
  {
    NS_FATAL_ERROR ("Recv from unknown socket "<<sock);
  }
}

//-----------------------------------------------------------------------------
//test 2-----------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
TcpSocketImplTest::Test2 (uint32_t payloadSize)
{
  test2_payloadSize = payloadSize;
  SetupDefaultSim ();
  listeningSock->SetAcceptCallback 
      (MakeNullCallback<bool, Ptr< Socket >, const Address &> (),
       MakeCallback(&TcpSocketImplTest::Test2_HandleConnectionCreated,this));
  sock1->SetRecvCallback (MakeCallback(&TcpSocketImplTest::Test2_HandleRecv, this));

  Simulator::Run ();
  Simulator::Destroy ();

  result = result && (rxBytes1 == test2_payloadSize);

  Reset ();
}

void
TcpSocketImplTest::Test2_HandleConnectionCreated (Ptr<Socket> s, const Address & addr)
{
  NS_ASSERT(s != listeningSock);
  NS_ASSERT(sock0 == 0);
  sock0 = s;
  Ptr<Packet> p = Create<Packet> (test2_payloadSize);
  sock0->Send(p);
  
  sock0->SetRecvCallback (MakeCallback(&TcpSocketImplTest::Test2_HandleRecv, this));
}

void
TcpSocketImplTest::Test2_HandleRecv (Ptr<Socket> sock)
{
  NS_ASSERT (sock == sock0 || sock == sock1);
  Ptr<Packet> p = sock->Recv();
  uint32_t sz = p->GetSize();
  if (sock == sock1)
  {
    rxBytes1 += sz;
  }
  else
  {
    NS_FATAL_ERROR ("Not supposed to be back traffic in test 2..."<<sock);
  }
}

//-----------------------------------------------------------------------------
//test 3-----------------------------------------------------------------------
//-----------------------------------------------------------------------------
void
TcpSocketImplTest::Test3 (uint32_t payloadSize)
{
  Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue (10000));
  test3_payloadSize = payloadSize;
  SetupDefaultSim ();
  listeningSock->SetAcceptCallback 
      (MakeNullCallback<bool, Ptr< Socket >, const Address &> (),
       MakeCallback(&TcpSocketImplTest::Test3_HandleConnectionCreated,this));
  sock1->SetRecvCallback (MakeCallback(&TcpSocketImplTest::Test3_HandleRecv, this));

  Simulator::Run ();
  Simulator::Destroy ();

  result = result && (rxBytes1 == test3_payloadSize);

  Reset();
}
void
TcpSocketImplTest::Test3_HandleConnectionCreated (Ptr<Socket> s, const Address &)
{
  NS_ASSERT(s != listeningSock);
  NS_ASSERT(sock0 == 0);
  sock0 = s;
  Ptr<Packet> p = Create<Packet> (test3_payloadSize);
  sock0->Send(p);
}
void
TcpSocketImplTest::Test3_HandleRecv (Ptr<Socket> sock)
{
  NS_ASSERT_MSG (sock == sock1, "Not supposed to be back traffic in test 3... ");
  if(sock->GetRxAvailable() >= 10000 ) //perform batch reads every 10000 bytes
  {
    Ptr<Packet> p = sock->Recv();
    uint32_t sz = p->GetSize();
    rxBytes1 += sz;
  }
}

//-----------------------------------------------------------------------------
//helpers----------------------------------------------------------------------
//-----------------------------------------------------------------------------
Ptr<Node>
TcpSocketImplTest::CreateInternetNode ()
{
  Ptr<Node> node = CreateObject<Node> ();
  AddInternetStack (node);
  return node;
}

Ptr<SimpleNetDevice>
TcpSocketImplTest::AddSimpleNetDevice (Ptr<Node> node, const char* ipaddr, const char* netmask)
{
  Ptr<SimpleNetDevice> dev = CreateObject<SimpleNetDevice> ();
  dev->SetAddress (Mac48Address::Allocate ());
  node->AddDevice (dev);
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  uint32_t ndid = ipv4->AddInterface (dev);
  Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address (ipaddr), Ipv4Mask (netmask));
  ipv4->AddAddress (ndid, ipv4Addr);
  ipv4->SetUp (ndid);
  return dev;
}

void 
TcpSocketImplTest::SetupDefaultSim ()
{
  const char* netmask = "255.255.255.0";
  const char* ipaddr0 = "192.168.1.1";
  const char* ipaddr1 = "192.168.1.2";
  node0 = CreateInternetNode ();
  node1 = CreateInternetNode ();
  dev0 = AddSimpleNetDevice (node0, ipaddr0, netmask);
  dev1 = AddSimpleNetDevice (node1, ipaddr1, netmask);

  channel = CreateObject<SimpleChannel> ();
  dev0->SetChannel (channel);
  dev1->SetChannel (channel);

  Ptr<SocketFactory> sockFactory0 = node0->GetObject<TcpSocketFactory> ();
  Ptr<SocketFactory> sockFactory1 = node1->GetObject<TcpSocketFactory> ();

  listeningSock = sockFactory0->CreateSocket();
  sock1 = sockFactory1->CreateSocket();

  uint16_t port = 50000;
  InetSocketAddress serverlocaladdr (Ipv4Address::GetAny(), port);
  InetSocketAddress serverremoteaddr (Ipv4Address(ipaddr0), port);

  listeningSock->Bind(serverlocaladdr);
  listeningSock->Listen ();

  sock1->Connect(serverremoteaddr);
}

void
TcpSocketImplTest::Reset ()
{
  node0 = 0;
  node1 = 0;
  dev0 = 0;
  dev1 = 0;
  channel = 0;
  listeningSock = 0;
  sock0 = 0;
  sock1 = 0;
  rxBytes0 = 0;
  rxBytes1 = 0;
  delete[] rxPayload;
  rxPayload = 0;
}

static TcpSocketImplTest gTcpSocketImplTest;

}//namespace ns3

#endif /* RUN_SELF_TESTS */

