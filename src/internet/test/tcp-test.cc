/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
 * Copyright (c) 2009 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Raj Bhattacharjea <raj.b@gatech.edu>
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

#include "ns3/ipv4-end-point.h"
#include "ns3/arp-l3-protocol.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv6-l3-protocol.h"
#include "ns3/icmpv4-l4-protocol.h"
#include "ns3/icmpv6-l4-protocol.h"
#include "ns3/udp-l4-protocol.h"
#include "ns3/tcp-l4-protocol.h"

#include <string>

NS_LOG_COMPONENT_DEFINE ("TcpTestSuite");

using namespace ns3;

class TcpTestCase : public TestCase
{
public:
  TcpTestCase (uint32_t totalStreamSize,
               uint32_t sourceWriteSize,
               uint32_t sourceReadSize,
               uint32_t serverWriteSize,
               uint32_t serverReadSize,
               bool useIpv6);
private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
  void SetupDefaultSim (void);
  void SetupDefaultSim6 (void);

  Ptr<Node> CreateInternetNode (void);
  Ptr<Node> CreateInternetNode6 (void);
  Ptr<SimpleNetDevice> AddSimpleNetDevice (Ptr<Node> node, const char* ipaddr, const char* netmask);
  Ptr<SimpleNetDevice> AddSimpleNetDevice6 (Ptr<Node> node, Ipv6Address ipaddr, Ipv6Prefix prefix);
  void ServerHandleConnectionCreated (Ptr<Socket> s, const Address & addr);
  void ServerHandleRecv (Ptr<Socket> sock);
  void ServerHandleSend (Ptr<Socket> sock, uint32_t available);
  void SourceHandleSend (Ptr<Socket> sock, uint32_t available);
  void SourceHandleRecv (Ptr<Socket> sock);

  uint32_t m_totalBytes;
  uint32_t m_sourceWriteSize;
  uint32_t m_sourceReadSize;
  uint32_t m_serverWriteSize;
  uint32_t m_serverReadSize;
  uint32_t m_currentSourceTxBytes;
  uint32_t m_currentSourceRxBytes;
  uint32_t m_currentServerRxBytes;
  uint32_t m_currentServerTxBytes;
  uint8_t *m_sourceTxPayload;
  uint8_t *m_sourceRxPayload;
  uint8_t* m_serverRxPayload;

  bool m_useIpv6;
};

static std::string Name (std::string str, uint32_t totalStreamSize,
                         uint32_t sourceWriteSize,
                         uint32_t serverReadSize,
                         uint32_t serverWriteSize,
                         uint32_t sourceReadSize,
                         bool useIpv6)
{
  std::ostringstream oss;
  oss << str << " total=" << totalStreamSize << " sourceWrite=" << sourceWriteSize 
      << " sourceRead=" << sourceReadSize << " serverRead=" << serverReadSize
      << " serverWrite=" << serverWriteSize << " useIpv6=" << useIpv6;
  return oss.str ();
}

#ifdef NS3_LOG_ENABLE
static std::string GetString (Ptr<Packet> p)
{
  std::ostringstream oss;
  p->CopyData (&oss, p->GetSize ());
  return oss.str ();
}
#endif /* NS3_LOG_ENABLE */

TcpTestCase::TcpTestCase (uint32_t totalStreamSize,
                          uint32_t sourceWriteSize,
                          uint32_t sourceReadSize,
                          uint32_t serverWriteSize,
                          uint32_t serverReadSize,
                          bool useIpv6)
  : TestCase (Name ("Send string data from client to server and back", 
                    totalStreamSize, 
                    sourceWriteSize,
                    serverReadSize,
                    serverWriteSize,
                    sourceReadSize,
                    useIpv6)),
    m_totalBytes (totalStreamSize),
    m_sourceWriteSize (sourceWriteSize),
    m_sourceReadSize (sourceReadSize),
    m_serverWriteSize (serverWriteSize),
    m_serverReadSize (serverReadSize),
    m_useIpv6 (useIpv6)
{
}

void
TcpTestCase::DoRun (void)
{
  m_currentSourceTxBytes = 0;
  m_currentSourceRxBytes = 0;
  m_currentServerRxBytes = 0;
  m_currentServerTxBytes = 0;
  m_sourceTxPayload = new uint8_t [m_totalBytes];
  m_sourceRxPayload = new uint8_t [m_totalBytes];
  m_serverRxPayload = new uint8_t [m_totalBytes];
  for(uint32_t i = 0; i < m_totalBytes; ++i)
    {
      uint8_t m = (uint8_t)(97 + (i % 26));
      m_sourceTxPayload[i] = m;
    }
  memset (m_sourceRxPayload, 0, m_totalBytes);
  memset (m_serverRxPayload, 0, m_totalBytes);

  if (m_useIpv6 == true)
    {
      SetupDefaultSim6 ();
    }
  else
    {
      SetupDefaultSim ();
    }

  Simulator::Run ();

  NS_TEST_EXPECT_MSG_EQ (m_currentSourceTxBytes, m_totalBytes, "Source sent all bytes");
  NS_TEST_EXPECT_MSG_EQ (m_currentServerRxBytes, m_totalBytes, "Server received all bytes");
  NS_TEST_EXPECT_MSG_EQ (m_currentSourceRxBytes, m_totalBytes, "Source received all bytes");
  NS_TEST_EXPECT_MSG_EQ (memcmp (m_sourceTxPayload, m_serverRxPayload, m_totalBytes), 0, 
                         "Server received expected data buffers");
  NS_TEST_EXPECT_MSG_EQ (memcmp (m_sourceTxPayload, m_sourceRxPayload, m_totalBytes), 0, 
                         "Source received back expected data buffers");
}
void
TcpTestCase::DoTeardown (void)
{
  delete [] m_sourceTxPayload;
  delete [] m_sourceRxPayload;
  delete [] m_serverRxPayload;
  Simulator::Destroy ();
}

void
TcpTestCase::ServerHandleConnectionCreated (Ptr<Socket> s, const Address & addr)
{
  s->SetRecvCallback (MakeCallback (&TcpTestCase::ServerHandleRecv, this));
  s->SetSendCallback (MakeCallback (&TcpTestCase::ServerHandleSend, this));
}

void
TcpTestCase::ServerHandleRecv (Ptr<Socket> sock)
{
  while (sock->GetRxAvailable () > 0)
    {
      uint32_t toRead = std::min (m_serverReadSize, sock->GetRxAvailable ());
      Ptr<Packet> p = sock->Recv (toRead, 0);
      if (p == 0 && sock->GetErrno () != Socket::ERROR_NOTERROR)
        {
          NS_FATAL_ERROR ("Server could not read stream at byte " << m_currentServerRxBytes);
        }
      NS_TEST_EXPECT_MSG_EQ ((m_currentServerRxBytes + p->GetSize () <= m_totalBytes), true, 
                             "Server received too many bytes");
      NS_LOG_DEBUG ("Server recv data=\"" << GetString (p) << "\"");
      p->CopyData (&m_serverRxPayload[m_currentServerRxBytes], p->GetSize ());
      m_currentServerRxBytes += p->GetSize ();
      ServerHandleSend (sock, sock->GetTxAvailable ());
    }
}

void
TcpTestCase::ServerHandleSend (Ptr<Socket> sock, uint32_t available)
{
  while (sock->GetTxAvailable () > 0 && m_currentServerTxBytes < m_currentServerRxBytes)
    {
      uint32_t left = m_currentServerRxBytes - m_currentServerTxBytes;
      uint32_t toSend = std::min (left, sock->GetTxAvailable ());
      toSend = std::min (toSend, m_serverWriteSize);
      Ptr<Packet> p = Create<Packet> (&m_serverRxPayload[m_currentServerTxBytes], toSend);
      NS_LOG_DEBUG ("Server send data=\"" << GetString (p) << "\"");
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
TcpTestCase::SourceHandleSend (Ptr<Socket> sock, uint32_t available)
{
  while (sock->GetTxAvailable () > 0 && m_currentSourceTxBytes < m_totalBytes)
    {
      uint32_t left = m_totalBytes - m_currentSourceTxBytes;
      uint32_t toSend = std::min (left, sock->GetTxAvailable ());
      toSend = std::min (toSend, m_sourceWriteSize);
      Ptr<Packet> p = Create<Packet> (&m_sourceTxPayload[m_currentSourceTxBytes], toSend);
      NS_LOG_DEBUG ("Source send data=\"" << GetString (p) << "\"");
      int sent = sock->Send (p);
      NS_TEST_EXPECT_MSG_EQ ((sent != -1), true, "Error during send ?");
      m_currentSourceTxBytes += sent;
    }
}

void
TcpTestCase::SourceHandleRecv (Ptr<Socket> sock)
{
  while (sock->GetRxAvailable () > 0 && m_currentSourceRxBytes < m_totalBytes)
    {
      uint32_t toRead = std::min (m_sourceReadSize, sock->GetRxAvailable ());
      Ptr<Packet> p = sock->Recv (toRead, 0);
      if (p == 0 && sock->GetErrno () != Socket::ERROR_NOTERROR)
        {
          NS_FATAL_ERROR ("Source could not read stream at byte " << m_currentSourceRxBytes);
        }
      NS_TEST_EXPECT_MSG_EQ ((m_currentSourceRxBytes + p->GetSize () <= m_totalBytes), true, 
                             "Source received too many bytes");
      NS_LOG_DEBUG ("Source recv data=\"" << GetString (p) << "\"");
      p->CopyData (&m_sourceRxPayload[m_currentSourceRxBytes], p->GetSize ());
      m_currentSourceRxBytes += p->GetSize ();
    }
  if (m_currentSourceRxBytes == m_totalBytes)
    {
      sock->Close ();
    }
}

Ptr<Node>
TcpTestCase::CreateInternetNode ()
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
TcpTestCase::AddSimpleNetDevice (Ptr<Node> node, const char* ipaddr, const char* netmask)
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
TcpTestCase::SetupDefaultSim (void)
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

  Ptr<Socket> server = sockFactory0->CreateSocket ();
  Ptr<Socket> source = sockFactory1->CreateSocket ();

  uint16_t port = 50000;
  InetSocketAddress serverlocaladdr (Ipv4Address::GetAny (), port);
  InetSocketAddress serverremoteaddr (Ipv4Address (ipaddr0), port);

  server->Bind (serverlocaladdr);
  server->Listen ();
  server->SetAcceptCallback (MakeNullCallback<bool, Ptr< Socket >, const Address &> (),
                             MakeCallback (&TcpTestCase::ServerHandleConnectionCreated,this));

  source->SetRecvCallback (MakeCallback (&TcpTestCase::SourceHandleRecv, this));
  source->SetSendCallback (MakeCallback (&TcpTestCase::SourceHandleSend, this));

  source->Connect (serverremoteaddr);
}

void
TcpTestCase::SetupDefaultSim6 (void)
{
  Ipv6Prefix prefix = Ipv6Prefix(64);
  Ipv6Address ipaddr0 = Ipv6Address("2001:0100:f00d:cafe::1");
  Ipv6Address ipaddr1 = Ipv6Address("2001:0100:f00d:cafe::2");
  Ptr<Node> node0 = CreateInternetNode6 ();
  Ptr<Node> node1 = CreateInternetNode6 ();
  Ptr<SimpleNetDevice> dev0 = AddSimpleNetDevice6 (node0, ipaddr0, prefix);
  Ptr<SimpleNetDevice> dev1 = AddSimpleNetDevice6 (node1, ipaddr1, prefix);

  Ptr<SimpleChannel> channel = CreateObject<SimpleChannel> ();
  dev0->SetChannel (channel);
  dev1->SetChannel (channel);

  Ptr<SocketFactory> sockFactory0 = node0->GetObject<TcpSocketFactory> ();
  Ptr<SocketFactory> sockFactory1 = node1->GetObject<TcpSocketFactory> ();

  Ptr<Socket> server = sockFactory0->CreateSocket ();
  Ptr<Socket> source = sockFactory1->CreateSocket ();

  uint16_t port = 50000;
  Inet6SocketAddress serverlocaladdr (Ipv6Address::GetAny (), port);
  Inet6SocketAddress serverremoteaddr (ipaddr0, port);

  server->Bind (serverlocaladdr);
  server->Listen ();
  server->SetAcceptCallback (MakeNullCallback<bool, Ptr< Socket >, const Address &> (),
                             MakeCallback (&TcpTestCase::ServerHandleConnectionCreated,this));

  source->SetRecvCallback (MakeCallback (&TcpTestCase::SourceHandleRecv, this));
  source->SetSendCallback (MakeCallback (&TcpTestCase::SourceHandleSend, this));

  source->Connect (serverremoteaddr);
}

Ptr<Node>
TcpTestCase::CreateInternetNode6 ()
{
  Ptr<Node> node = CreateObject<Node> ();
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
  //TCP
  Ptr<TcpL4Protocol> tcp = CreateObject<TcpL4Protocol> ();
  node->AggregateObject (tcp);
  return node;
}

Ptr<SimpleNetDevice>
TcpTestCase::AddSimpleNetDevice6 (Ptr<Node> node, Ipv6Address ipaddr, Ipv6Prefix prefix)
{
  Ptr<SimpleNetDevice> dev = CreateObject<SimpleNetDevice> ();
  dev->SetAddress (Mac48Address::ConvertFrom (Mac48Address::Allocate ()));
  node->AddDevice (dev);
  Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
  uint32_t ndid = ipv6->AddInterface (dev);
  Ipv6InterfaceAddress ipv6Addr = Ipv6InterfaceAddress (ipaddr, prefix);
  ipv6->AddAddress (ndid, ipv6Addr);
  ipv6->SetUp (ndid);
  return dev;
}

static class TcpTestSuite : public TestSuite
{
public:
  TcpTestSuite ()
    : TestSuite ("tcp", UNIT)
  {
    // Arguments to these test cases are 1) totalStreamSize,
    // 2) source write size, 3) source read size
    // 4) server write size, and 5) server read size
    // with units of bytes
    AddTestCase (new TcpTestCase (13, 200, 200, 200, 200, false), TestCase::QUICK);
    AddTestCase (new TcpTestCase (13, 1, 1, 1, 1, false), TestCase::QUICK);
    AddTestCase (new TcpTestCase (100000, 100, 50, 100, 20, false), TestCase::QUICK);

    AddTestCase (new TcpTestCase (13, 200, 200, 200, 200, true), TestCase::QUICK);
    AddTestCase (new TcpTestCase (13, 1, 1, 1, 1, true), TestCase::QUICK);
    AddTestCase (new TcpTestCase (100000, 100, 50, 100, 20, true), TestCase::QUICK);
  }

} g_tcpTestSuite;
