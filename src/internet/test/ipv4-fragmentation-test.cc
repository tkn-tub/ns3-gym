/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Universita' di Firenze, Italy
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
/**
 * This is the test code for ipv4-l3protocol.cc (only the fragmentation and reassembly part).
 */

#include "ns3/test.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/socket-factory.h"
#include "ns3/ipv4-raw-socket-factory.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/simulator.h"
#include "error-channel.h"
#include "error-net-device.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"

#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/inet-socket-address.h"
#include "ns3/boolean.h"

#include "ns3/arp-l3-protocol.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/icmpv4-l4-protocol.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/ipv4-static-routing.h"
#include "ns3/udp-l4-protocol.h"

#include <string>
#include <limits>
#include <netinet/in.h>

namespace ns3 {

class UdpSocketImpl;

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
  // //Ipv4Raw
  Ptr<UdpL4Protocol> udp = CreateObject<UdpL4Protocol> ();
  node->AggregateObject(udp);
}


class Ipv4FragmentationTest: public TestCase
{
  Ptr<Packet> m_sentPacketClient;
  Ptr<Packet> m_receivedPacketClient;
  Ptr<Packet> m_receivedPacketServer;


  Ptr<Socket> m_socketServer;
  Ptr<Socket> m_socketClient;
  uint32_t m_dataSize;
  uint8_t *m_data;
  uint32_t m_size;
  uint8_t m_icmpType;

public:
  virtual void DoRun (void);
  Ipv4FragmentationTest ();
  ~Ipv4FragmentationTest ();

  // server part
  void StartServer (Ptr<Node> ServerNode);
  void HandleReadServer (Ptr<Socket> socket);

  // client part
  void StartClient (Ptr<Node> ClientNode);
  void HandleReadClient (Ptr<Socket> socket);
  void HandleReadIcmpClient (Ipv4Address icmpSource, uint8_t icmpTtl, uint8_t icmpType,
                             uint8_t icmpCode,uint32_t icmpInfo);

  void SetFill (uint8_t *fill, uint32_t fillSize, uint32_t dataSize);
  Ptr<Packet> SendClient (void);

};


Ipv4FragmentationTest::Ipv4FragmentationTest ()
  : TestCase ("Verify the IPv4 layer 3 protocol fragmentation and reassembly")
{
  m_socketServer = 0;
  m_data = 0;
  m_dataSize = 0;
}

Ipv4FragmentationTest::~Ipv4FragmentationTest ()
{
  if ( m_data )
    {
      delete[] m_data;
    }
  m_data = 0;
  m_dataSize = 0;
}


void
Ipv4FragmentationTest::StartServer (Ptr<Node> ServerNode)
{

  if (m_socketServer == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socketServer = Socket::CreateSocket (ServerNode, tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 9);
      m_socketServer->Bind (local);
      Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socketServer);
      if (udpSocket)
        {
          // equivalent to setsockopt (MCAST_JOIN_GROUP)
          udpSocket->MulticastJoinGroup (0, Ipv4Address ("10.0.0.1"));
        }
   }

  m_socketServer->SetRecvCallback(MakeCallback(&Ipv4FragmentationTest::HandleReadServer, this));
}

void
Ipv4FragmentationTest::HandleReadServer (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
    {
      if (InetSocketAddress::IsMatchingType (from))
        {
          packet->RemoveAllPacketTags ();
          packet->RemoveAllByteTags ();

          m_receivedPacketServer = packet->Copy();
        }
    }
}

void
Ipv4FragmentationTest::StartClient (Ptr<Node> ClientNode)
{

  if (m_socketClient == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socketClient = Socket::CreateSocket (ClientNode, tid);
      m_socketClient->Bind ();
      m_socketClient->Connect (InetSocketAddress (Ipv4Address ("10.0.0.1"), 9));
      CallbackValue cbValue = MakeCallback(&Ipv4FragmentationTest::HandleReadIcmpClient, this);
      m_socketClient->SetAttribute ("IcmpCallback", cbValue);
    }

  m_socketClient->SetRecvCallback(MakeCallback(&Ipv4FragmentationTest::HandleReadClient, this));
}

void
Ipv4FragmentationTest::HandleReadClient (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
    {
      if (InetSocketAddress::IsMatchingType (from))
        {
          m_receivedPacketClient = packet->Copy();
        }
    }
}

void
Ipv4FragmentationTest::HandleReadIcmpClient (Ipv4Address icmpSource,
                                             uint8_t icmpTtl, uint8_t icmpType,
                                             uint8_t icmpCode, uint32_t icmpInfo)
{
  m_icmpType = icmpType;
}

void
Ipv4FragmentationTest::SetFill (uint8_t *fill, uint32_t fillSize, uint32_t dataSize)
{
  if (dataSize != m_dataSize)
    {
      delete [] m_data;
      m_data = new uint8_t [dataSize];
      m_dataSize = dataSize;
    }

  if (fillSize >= dataSize)
    {
      memcpy (m_data, fill, dataSize);
      return;
    }

  uint32_t filled = 0;
  while (filled + fillSize < dataSize)
    {
      memcpy (&m_data[filled], fill, fillSize);
      filled += fillSize;
    }

  memcpy(&m_data[filled], fill, dataSize - filled);

  m_size = dataSize;
}

Ptr<Packet> Ipv4FragmentationTest::SendClient (void)
{
  Ptr<Packet> p;
  if (m_dataSize)
    {
      p = Create<Packet> (m_data, m_dataSize);
    }
  else
    {
      p = Create<Packet> (m_size);
    }
  m_socketClient->Send (p);

  return p;
}

void
Ipv4FragmentationTest::DoRun (void)
{
  // set the arp cache to something quite high
  // we shouldn't need because the NetDevice used doesn't need arp, but still
  Config::SetDefault ("ns3::ArpCache::PendingQueueSize", UintegerValue (100));

  // Create topology
  
  // Receiver Node
  Ptr<Node> serverNode = CreateObject<Node> ();
  AddInternetStack (serverNode);
  Ptr<ErrorNetDevice> serverDev;
  Ptr<BinaryErrorModel> serverDevErrorModel = CreateObject<BinaryErrorModel> ();
  {
    serverDev = CreateObject<ErrorNetDevice> ();
    serverDev->SetAddress (Mac48Address::ConvertFrom (Mac48Address::Allocate ()));
    serverDev->SetMtu(1500);
    serverDev->SetReceiveErrorModel(serverDevErrorModel);
    serverDevErrorModel->Disable();
    serverNode->AddDevice (serverDev);
    Ptr<Ipv4> ipv4 = serverNode->GetObject<Ipv4> ();
    uint32_t netdev_idx = ipv4->AddInterface (serverDev);
    Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.0.1"), Ipv4Mask (0xffff0000U));
    ipv4->AddAddress (netdev_idx, ipv4Addr);
    ipv4->SetUp (netdev_idx);
  }
  StartServer(serverNode);

  // Sender Node
  Ptr<Node> clientNode = CreateObject<Node> ();
  AddInternetStack (clientNode);
  Ptr<ErrorNetDevice> clientDev;
  Ptr<BinaryErrorModel> clientDevErrorModel = CreateObject<BinaryErrorModel> ();
  {
    clientDev = CreateObject<ErrorNetDevice> ();
    clientDev->SetAddress (Mac48Address::ConvertFrom (Mac48Address::Allocate ()));
    clientDev->SetMtu(1000);
    clientDev->SetReceiveErrorModel(clientDevErrorModel);
    clientDevErrorModel->Disable();
    clientNode->AddDevice (clientDev);
    Ptr<Ipv4> ipv4 = clientNode->GetObject<Ipv4> ();
    uint32_t netdev_idx = ipv4->AddInterface (clientDev);
    Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.0.2"), Ipv4Mask (0xffff0000U));
    ipv4->AddAddress (netdev_idx, ipv4Addr);
    ipv4->SetUp (netdev_idx);
  }
  StartClient(clientNode);

  // link the two nodes
  Ptr<ErrorChannel> channel = CreateObject<ErrorChannel> ();
  serverDev->SetChannel (channel);
  clientDev->SetChannel (channel);
  channel->SetJumpingTime(Seconds(0.5));


  // some small packets, some rather big ones
  uint32_t packetSizes[5] = {1000, 2000, 5000, 10000, 65000};

  // using the alphabet
  uint8_t fillData[78];
  for ( uint32_t k=48; k<=125; k++ )
    {
      fillData[k-48] = k;
    }

  // First test: normal channel, no errors, no delays
  for( int i= 0; i<5; i++)
    {
      uint32_t packetSize = packetSizes[i];

      SetFill (fillData, 78, packetSize);

      m_receivedPacketServer = Create<Packet> ();
      Simulator::ScheduleWithContext (m_socketClient->GetNode ()->GetId (), Seconds (0),
                                      &Ipv4FragmentationTest::SendClient, this);
      Simulator::Run ();

      uint8_t recvBuffer[65000];

      uint16_t recvSize = m_receivedPacketServer->GetSize ();

      NS_TEST_EXPECT_MSG_EQ (recvSize, packetSizes[i], "Packet size not correct");

      m_receivedPacketServer->CopyData(recvBuffer, 65000);
      NS_TEST_EXPECT_MSG_EQ (memcmp(m_data, recvBuffer, m_receivedPacketServer->GetSize ()),
                             0, "Packet content differs");
    }

  // Second test: normal channel, no errors, delays each 2 packets.
  // Each other fragment will arrive out-of-order.
  // The packets should be received correctly since reassembly will reorder the fragments.
  channel->SetJumpingMode(true);
  for( int i= 0; i<5; i++)
    {
      uint32_t packetSize = packetSizes[i];

      SetFill (fillData, 78, packetSize);

      m_receivedPacketServer = Create<Packet> ();
      Simulator::ScheduleWithContext (m_socketClient->GetNode ()->GetId (), Seconds (0),
                                      &Ipv4FragmentationTest::SendClient, this);
      Simulator::Run ();

      uint8_t recvBuffer[65000];

      uint16_t recvSize = m_receivedPacketServer->GetSize ();

      NS_TEST_EXPECT_MSG_EQ (recvSize, packetSizes[i], "Packet size not correct");

      m_receivedPacketServer->CopyData(recvBuffer, 65000);
      NS_TEST_EXPECT_MSG_EQ (memcmp(m_data, recvBuffer, m_receivedPacketServer->GetSize ()),
                             0, "Packet content differs");
    }
  channel->SetJumpingMode(false);

  // Third test: normal channel, some errors, no delays.
  // The reassembly procedure should fire a timeout after 30 seconds (as specified in the RFCs).
  // Upon the timeout, the fragments received so far are discarded and an ICMP should be sent back
  // to the sender (if the first fragment has been received).
  // In this test case the first fragment is received, so we do expect an ICMP.
  // Client -> Server : errors enabled
  // Server -> Client : errors disabled (we want to have back the ICMP)
  clientDevErrorModel->Disable();
  serverDevErrorModel->Enable();
  for( int i= 1; i<5; i++)
    {
      uint32_t packetSize = packetSizes[i];

      SetFill (fillData, 78, packetSize);

      // reset the model, we want to receive the very first fragment.
      serverDevErrorModel->Reset();

      m_receivedPacketServer = Create<Packet> ();
      m_icmpType = 0;
      Simulator::ScheduleWithContext (m_socketClient->GetNode ()->GetId (), Seconds (0),
                                      &Ipv4FragmentationTest::SendClient, this);
      Simulator::Run ();

      uint16_t recvSize = m_receivedPacketServer->GetSize ();

      NS_TEST_EXPECT_MSG_EQ ((recvSize == 0), true, "Server got a packet, something wrong");
      NS_TEST_EXPECT_MSG_EQ ((m_icmpType == 11), true, "Client did not receive ICMP::TIME_EXCEEDED");
    }


  Simulator::Destroy ();
}
//-----------------------------------------------------------------------------
class Ipv4FragmentationTestSuite : public TestSuite
{
public:
  Ipv4FragmentationTestSuite () : TestSuite ("ipv4-fragmentation", UNIT)
  {
    AddTestCase (new Ipv4FragmentationTest);
  }
} g_ipv4fragmentationTestSuite;

}; // namespace ns3
