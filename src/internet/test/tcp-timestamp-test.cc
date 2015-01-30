/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Natale Patriciello <natale.patriciello@gmail.com>
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
 */

#include "ns3/test.h"
#include "ns3/socket-factory.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/simple-channel.h"
#include "ns3/simple-net-device.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/config.h"
#include "ns3/ipv4-static-routing.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/ipv6-static-routing.h"
#include "ns3/ipv6-list-routing.h"
#include "ns3/node.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"
#include "ns3/tcp-socket-base.h"

#include "ns3/ipv4-end-point.h"
#include "ns3/arp-l3-protocol.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv6-l3-protocol.h"
#include "ns3/icmpv4-l4-protocol.h"
#include "ns3/icmpv6-l4-protocol.h"
#include "ns3/udp-l4-protocol.h"
#include "ns3/tcp-l4-protocol.h"

#include "ns3/private/tcp-option-ts.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TimestampTestSuite");

class TimestampTestCase : public TestCase
{
public:
  enum Configuration
  {
    DISABLED,
    ENABLED_CLIENT,
    ENABLED_SERVER,
    ENABLED
  };

  TimestampTestCase (TimestampTestCase::Configuration conf);

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);

  void SetupDefaultSim (void);
  //void SetupDefaultSim6 (void);

  Ptr<Node> CreateInternetNode (void);
  Ptr<Node> CreateInternetNode6 (void);
  Ptr<SimpleNetDevice> AddSimpleNetDevice (Ptr<Node> node, const char* ipaddr, const char* netmask);
  Ptr<SimpleNetDevice> AddSimpleNetDevice6 (Ptr<Node> node, Ipv6Address ipaddr, Ipv6Prefix prefix);
  void ServerHandleConnectionCreated (Ptr<Socket> s, const Address & addr);
  void ServerHandleRecv (Ptr<Socket> sock);
  void ServerHandleSend (Ptr<Socket> sock, uint32_t sendB);
  void SourceHandleSend (Ptr<Socket> sock, uint32_t available);
  void SourceHandleRecv (Ptr<Socket> sock);

  Configuration m_configuration;

  uint32_t m_totalBytes;
  uint32_t m_sourceWriteSize;
  uint32_t m_sourceReadSize;
  uint32_t m_serverWriteSize;
  uint32_t m_serverReadSize;
  uint32_t m_currentSourceTxBytes;
  uint32_t m_currentSourceRxBytes;
  uint32_t m_currentServerRxBytes;
  uint32_t m_currentServerTxBytes;
};

TimestampTestCase::TimestampTestCase (TimestampTestCase::Configuration conf)
  : TestCase ("Testing the TCP Timestamp option")
{
  m_configuration = conf;
  m_totalBytes = 2000;
  m_sourceWriteSize = 500;
  m_sourceReadSize = 500;
  m_serverWriteSize = 500;
  m_serverReadSize = 500;
}

void
TimestampTestCase::DoRun (void)
{
  m_currentSourceTxBytes = 0;
  m_currentSourceRxBytes = 0;
  m_currentServerRxBytes = 0;
  m_currentServerTxBytes = 0;


//  if (m_useIpv6 == true)
//    {
//      SetupDefaultSim6 ();
//    }
//  else
//    {
  SetupDefaultSim ();
//    }

  Simulator::Run ();

  NS_TEST_EXPECT_MSG_EQ (m_currentSourceTxBytes, m_totalBytes, "Source sent all bytes");
  NS_TEST_EXPECT_MSG_EQ (m_currentServerRxBytes, m_totalBytes, "Server received all bytes");
  NS_TEST_EXPECT_MSG_EQ (m_currentSourceRxBytes, m_totalBytes, "Source received all bytes");
}

void
TimestampTestCase::DoTeardown (void)
{
  Simulator::Destroy ();
}

void
TimestampTestCase::ServerHandleConnectionCreated (Ptr<Socket> s, const Address & addr)
{
  s->SetRecvCallback (MakeCallback (&TimestampTestCase::ServerHandleRecv, this));
  s->SetSendCallback (MakeCallback (&TimestampTestCase::ServerHandleSend, this));
}

void
TimestampTestCase::ServerHandleRecv (Ptr<Socket> sock)
{
  while (sock->GetRxAvailable () > 0)
    {
      uint32_t toRead = std::min (m_serverReadSize, sock->GetRxAvailable ());
      Ptr<Packet> p = sock->Recv (toRead, 0);

      if (p == 0 && sock->GetErrno () != Socket::ERROR_NOTERROR)
        {
          NS_FATAL_ERROR ("Server could not read stream at byte " << m_currentServerRxBytes);
        }

      m_currentServerRxBytes += p->GetSize ();

      ServerHandleSend (sock, sock->GetTxAvailable ());
    }
}

void
TimestampTestCase::ServerHandleSend (Ptr<Socket> sock, uint32_t sendB)
{
  while (sock->GetTxAvailable () > 0 && m_currentServerTxBytes < m_currentServerRxBytes)
    {
      uint32_t left = m_currentServerRxBytes - m_currentServerTxBytes;
      uint32_t toSend = std::min (left, sock->GetTxAvailable ());
      toSend = std::min (toSend, m_serverWriteSize);

      Ptr<Packet> p = Create<Packet> (toSend);

      int sent = sock->Send (p);

      NS_TEST_EXPECT_MSG_EQ ((sent != -1), true, "Server error during send ?");

      m_currentServerTxBytes += sent;
    }

  if (m_currentServerTxBytes == m_totalBytes)
    {
      sock->Close ();
    }
}

void
TimestampTestCase::SourceHandleSend (Ptr<Socket> sock, uint32_t available)
{
  while (sock->GetTxAvailable () > 0 && m_currentSourceTxBytes < m_totalBytes)
    {
      uint32_t left = m_totalBytes - m_currentSourceTxBytes;
      uint32_t toSend = std::min (left, sock->GetTxAvailable ());
      toSend = std::min (toSend, m_sourceWriteSize);

      Ptr<Packet> p = Create<Packet> (toSend);

      int sent = sock->Send (p);
      NS_TEST_EXPECT_MSG_EQ ((sent != -1), true, "Error during send ?");
      m_currentSourceTxBytes += sent;
    }
}

void
TimestampTestCase::SourceHandleRecv (Ptr<Socket> sock)
{
  while (sock->GetRxAvailable () > 0 && m_currentSourceRxBytes < m_totalBytes)
    {
      uint32_t toRead = std::min (m_sourceReadSize, sock->GetRxAvailable ());
      Ptr<Packet> p = sock->Recv (toRead, 0);

      if (p == 0 && sock->GetErrno () != Socket::ERROR_NOTERROR)
        {
          NS_FATAL_ERROR ("Source could not read stream at byte " << m_currentSourceRxBytes);
        }

      m_currentSourceRxBytes += p->GetSize ();
    }

  if (m_currentSourceRxBytes == m_totalBytes)
    {
      sock->Close ();
    }
}

Ptr<Node>
TimestampTestCase::CreateInternetNode ()
{
  Ptr<Node> node = CreateObject<Node> ();
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
  return node;
}

Ptr<SimpleNetDevice>
TimestampTestCase::AddSimpleNetDevice (Ptr<Node> node, const char* ipaddr, const char* netmask)
{
  Ptr<SimpleNetDevice> dev = CreateObject<SimpleNetDevice> ();
  dev->SetAddress (Mac48Address::ConvertFrom (Mac48Address::Allocate ()));
  node->AddDevice (dev);
  Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
  uint32_t ndid = ipv4->AddInterface (dev);
  Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address (ipaddr), Ipv4Mask (netmask));
  ipv4->AddAddress (ndid, ipv4Addr);
  ipv4->SetUp (ndid);
  return dev;
}

void
TimestampTestCase::SetupDefaultSim (void)
{
  const char* netmask = "255.255.255.0";
  const char* ipaddr0 = "192.168.1.1";
  const char* ipaddr1 = "192.168.1.2";
  Ptr<Node> node0 = CreateInternetNode ();
  Ptr<Node> node1 = CreateInternetNode ();
  Ptr<SimpleNetDevice> dev0 = AddSimpleNetDevice (node0, ipaddr0, netmask);
  Ptr<SimpleNetDevice> dev1 = AddSimpleNetDevice (node1, ipaddr1, netmask);

  Ptr<SimpleChannel> channel = CreateObject<SimpleChannel> ();
  dev0->SetChannel (channel);
  dev1->SetChannel (channel);

  Ptr<SocketFactory> sockFactory0 = node0->GetObject<TcpSocketFactory> ();
  Ptr<SocketFactory> sockFactory1 = node1->GetObject<TcpSocketFactory> ();

  Ptr<TcpSocketBase> server = DynamicCast<TcpSocketBase> (sockFactory0->CreateSocket ());
  Ptr<TcpSocketBase> source = DynamicCast<TcpSocketBase> (sockFactory1->CreateSocket ());

  NS_ASSERT (server != 0);
  NS_ASSERT (source != 0);

  switch (m_configuration)
    {
    case DISABLED:
      server->SetAttribute ("Timestamp", BooleanValue (false));
      source->SetAttribute ("Timestamp", BooleanValue (false));
      break;

    case ENABLED_CLIENT:
      server->SetAttribute ("Timestamp", BooleanValue (false));
      source->SetAttribute ("Timestamp", BooleanValue (true));
      break;

    case ENABLED_SERVER:
      server->SetAttribute ("Timestamp", BooleanValue (true));
      source->SetAttribute ("Timestamp", BooleanValue (false));
      break;

    case ENABLED:
      server->SetAttribute ("Timestamp", BooleanValue (true));
      source->SetAttribute ("Timestamp", BooleanValue (true));
      break;
    }

  uint16_t port = 50000;
  InetSocketAddress serverlocaladdr (Ipv4Address::GetAny (), port);
  InetSocketAddress serverremoteaddr (Ipv4Address (ipaddr0), port);

  server->Bind (serverlocaladdr);
  server->Listen ();
  server->SetAcceptCallback (MakeNullCallback<bool, Ptr< Socket >, const Address &> (),
                             MakeCallback (&TimestampTestCase::ServerHandleConnectionCreated,this));

  source->SetRecvCallback (MakeCallback (&TimestampTestCase::SourceHandleRecv, this));
  source->SetSendCallback (MakeCallback (&TimestampTestCase::SourceHandleSend, this));

  source->Connect (serverremoteaddr);
}

class TimestampValueTestCase : public TestCase
{
public:
  TimestampValueTestCase (double startTime, double timeToWait,
                          std::string name);

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);

  void Check ();
  void Init ();

  double m_startTime;
  double m_timeToWait;
  double m_initValue;
};

TimestampValueTestCase::TimestampValueTestCase (double startTime,
                                                double timeToWait,
                                                std::string name)
  : TestCase (name)
{
  m_startTime = startTime;
  m_timeToWait = timeToWait;
}

void
TimestampValueTestCase::DoRun ()
{
  Simulator::Schedule (Seconds (m_startTime + m_timeToWait),
                       &TimestampValueTestCase::Check, this);
  Simulator::Schedule (Seconds (m_startTime),
                       &TimestampValueTestCase::Init, this);

  Simulator::Run ();
}

void
TimestampValueTestCase::DoTeardown ()
{
  Simulator::Destroy ();
}

void
TimestampValueTestCase::Init ()
{
  m_initValue = TcpOptionTS::NowToTsValue ();
}

void
TimestampValueTestCase::Check ()
{
  uint32_t lastValue = TcpOptionTS::NowToTsValue ();

  NS_TEST_ASSERT_MSG_EQ_TOL (MilliSeconds (lastValue - m_initValue), Seconds (m_timeToWait),
                             MilliSeconds (1), "Different TS values");

  NS_TEST_ASSERT_MSG_EQ_TOL (TcpOptionTS::ElapsedTimeFromTsValue (m_initValue), Seconds (m_timeToWait),
                             MilliSeconds (1), "Estimating Wrong RTT");
}

static class TcpTimestampTestSuite : public TestSuite
{
public:
  TcpTimestampTestSuite ()
    : TestSuite ("tcp-timestamp", UNIT)
  {
    AddTestCase (new TimestampTestCase (TimestampTestCase::DISABLED), TestCase::QUICK);
    AddTestCase (new TimestampTestCase (TimestampTestCase::ENABLED_CLIENT), TestCase::QUICK);
    AddTestCase (new TimestampTestCase (TimestampTestCase::ENABLED_SERVER), TestCase::QUICK);
    AddTestCase (new TimestampTestCase (TimestampTestCase::ENABLED), TestCase::QUICK);
    AddTestCase (new TimestampValueTestCase (0.0, 0.01, "Value Check"), TestCase::QUICK);
    AddTestCase (new TimestampValueTestCase (3.0, 0.5,  "Value Check"), TestCase::QUICK);
    AddTestCase (new TimestampValueTestCase (5.5, 1.0,  "Value Check"), TestCase::QUICK);
    AddTestCase (new TimestampValueTestCase (6.0, 2.0,  "Value Check"), TestCase::QUICK);
    AddTestCase (new TimestampValueTestCase (2.4, 0.7,  "Value Check"), TestCase::QUICK);

  }

} g_tcpTimestampTestSuite;

} // namespace ns3
