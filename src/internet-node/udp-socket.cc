/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-route.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4.h"
#include "ns3/udp.h"
#include "ns3/socket-defaults.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/uinteger.h"
#include "udp-socket.h"
#include "udp-l4-protocol.h"
#include "ipv4-end-point.h"
#include "ipv4-l4-demux.h"

NS_LOG_COMPONENT_DEFINE ("UdpSocket");

namespace ns3 {

TypeId
UdpSocket::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpSocket")
    .SetParent<Socket> ()
    .AddConstructor<UdpSocket> ()
    .AddTraceSource ("Drop", "Drop UDP packet due to receive buffer overflow",
                     MakeTraceSourceAccessor (&UdpSocket::m_dropTrace))
    ;
  return tid;
}

UdpSocket::UdpSocket ()
  : m_endPoint (0),
    m_node (0),
    m_udp (0),
    m_errno (ERROR_NOTERROR),
    m_shutdownSend (false),
    m_shutdownRecv (false),
    m_connected (false),
    m_rxAvailable (0),
    m_sndBufLimit (0),
    m_rcvBufLimit (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

UdpSocket::~UdpSocket ()
{
  NS_LOG_FUNCTION_NOARGS ();

  m_node = 0;
  if (m_endPoint != 0)
    {
      NS_ASSERT (m_udp != 0);
      /**
       * Note that this piece of code is a bit tricky:
       * when DeAllocate is called, it will call into
       * Ipv4EndPointDemux::Deallocate which triggers
       * a delete of the associated endPoint which triggers
       * in turn a call to the method ::Destroy below
       * will will zero the m_endPoint field.
       */
      NS_ASSERT (m_endPoint != 0);
      m_udp->DeAllocate (m_endPoint);
      NS_ASSERT (m_endPoint == 0);
    }
  m_udp = 0;
}

void 
UdpSocket::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION_NOARGS ();
  // Pull default values for socket options from SocketDefaults
  // object that was aggregated to the node 
  m_node = node;
  Ptr<SocketDefaults> sd = node->GetObject<SocketDefaults> ();
  NS_ASSERT (sd != 0);
  UintegerValue uiv;
  sd->GetAttribute ("DefaultRcvBufLimit", uiv);
  m_rcvBufLimit =  uiv.Get();

}
void 
UdpSocket::SetUdp (Ptr<UdpL4Protocol> udp)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_udp = udp;
}


enum Socket::SocketErrno
UdpSocket::GetErrno (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_errno;
}

Ptr<Node>
UdpSocket::GetNode (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_node;
}

void 
UdpSocket::Destroy (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_endPoint = 0;
  m_udp = 0;
}

int
UdpSocket::FinishBind (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_endPoint == 0)
    {
      return -1;
    }
  m_endPoint->SetRxCallback (MakeCallback (&UdpSocket::ForwardUp, this));
  m_endPoint->SetDestroyCallback (MakeCallback (&UdpSocket::Destroy, this));
  return 0;
}

int
UdpSocket::Bind (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_endPoint = m_udp->Allocate ();
  return FinishBind ();
}

int 
UdpSocket::Bind (const Address &address)
{
  NS_LOG_FUNCTION (this << address);

  if (!InetSocketAddress::IsMatchingType (address))
    {
      NS_LOG_ERROR ("Not IsMatchingType");
      return ERROR_INVAL;
    }
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  Ipv4Address ipv4 = transport.GetIpv4 ();
  uint16_t port = transport.GetPort ();
  if (ipv4 == Ipv4Address::GetAny () && port == 0)
    {
      m_endPoint = m_udp->Allocate ();
    }
  else if (ipv4 == Ipv4Address::GetAny () && port != 0)
    {
      m_endPoint = m_udp->Allocate (port);
    }
  else if (ipv4 != Ipv4Address::GetAny () && port == 0)
    {
      m_endPoint = m_udp->Allocate (ipv4);
    }
  else if (ipv4 != Ipv4Address::GetAny () && port != 0)
    {
      m_endPoint = m_udp->Allocate (ipv4, port);
    }

  return FinishBind ();
}

int 
UdpSocket::ShutdownSend (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_shutdownSend = true;
  return 0;
}

int 
UdpSocket::ShutdownRecv (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_shutdownRecv = false;
  return 0;
}

int
UdpSocket::Close(void)
{
  NS_LOG_FUNCTION_NOARGS ();
  NotifyCloseCompleted ();
  return 0;
}

int
UdpSocket::Connect(const Address & address)
{
  NS_LOG_FUNCTION (this << address);
  Ipv4Route routeToDest;
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  m_defaultAddress = transport.GetIpv4 ();
  m_defaultPort = transport.GetPort ();
  NotifyConnectionSucceeded ();
  m_connected = true;

  return 0;
}

int 
UdpSocket::Send (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);

  if (!m_connected)
    {
      m_errno = ERROR_NOTCONN;
      return -1;
    }
  return DoSend (p);
}

int 
UdpSocket::DoSend (Ptr<Packet> p)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_endPoint == 0)
    {
      if (Bind () == -1)
       {
          NS_ASSERT (m_endPoint == 0);
         return -1;
       }
      NS_ASSERT (m_endPoint != 0);
    }
  if (m_shutdownSend)
    {
      m_errno = ERROR_SHUTDOWN;
      return -1;
    } 
  
  return DoSendTo (p, m_defaultAddress, m_defaultPort);
}

int
UdpSocket::DoSendTo (Ptr<Packet> p, const Address &address)
{
  NS_LOG_FUNCTION (this << p << address);

  if (!m_connected)
    {
      NS_LOG_LOGIC ("Not connected");
      InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
      Ipv4Address ipv4 = transport.GetIpv4 ();
      uint16_t port = transport.GetPort ();
      return DoSendTo (p, ipv4, port);
    }
  else
    {
      // connected UDP socket must use default addresses
      NS_LOG_LOGIC ("Connected");
      return DoSendTo (p, m_defaultAddress, m_defaultPort);
    }
}

int
UdpSocket::DoSendTo (Ptr<Packet> p, Ipv4Address dest, uint16_t port)
{
  NS_LOG_FUNCTION (this << p << dest << port);

  Ipv4Route routeToDest;

  if (m_endPoint == 0)
    {
      if (Bind () == -1)
	{
          NS_ASSERT (m_endPoint == 0);
	  return -1;
	}
      NS_ASSERT (m_endPoint != 0);
    }
  if (m_shutdownSend)
    {
      m_errno = ERROR_SHUTDOWN;
      return -1;
    }

  uint32_t localIfIndex;
  Ptr<Ipv4> ipv4 = m_node->GetObject<Ipv4> ();

  //
  // If dest is sent to the limited broadcast address (all ones),
  // convert it to send a copy of the packet out of every interface
  //
  if (dest.IsBroadcast ())
    {
      NS_LOG_LOGIC ("Limited broadcast start.");
      for (uint32_t i = 0; i < ipv4->GetNInterfaces (); i++ )
        {
          Ipv4Address addri = ipv4->GetAddress (i);
          Ipv4Mask maski = ipv4->GetNetworkMask (i);
          Ipv4Address bcast = addri.GetSubnetDirectedBroadcast (maski);
          NS_LOG_LOGIC ("Sending one copy from " << addri << " to " << bcast
                        << " (mask is " << maski << ")");
          m_udp->Send (p->Copy (), addri, bcast,
                       m_endPoint->GetLocalPort (), port);
          NotifyDataSent (p->GetSize ());
        }
      NS_LOG_LOGIC ("Limited broadcast end.");
      return p->GetSize();
    }
  else if (ipv4->GetIfIndexForDestination(dest, localIfIndex))
    {
      NS_LOG_LOGIC ("Route exists");
      m_udp->Send (p->Copy (), ipv4->GetAddress (localIfIndex), dest,
		   m_endPoint->GetLocalPort (), port);
      NotifyDataSent (p->GetSize ());
      return p->GetSize();;
    }
  else
   {
      NS_LOG_ERROR ("ERROR_NOROUTETOHOST");
      m_errno = ERROR_NOROUTETOHOST;
      return -1;
   }

  return 0;
}

uint32_t
UdpSocket::GetTxAvailable (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  // No finite send buffer is modelled
  return std::numeric_limits<uint32_t>::max ();
}

int 
UdpSocket::SendTo (const Address &address, Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << address << p);
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  Ipv4Address ipv4 = transport.GetIpv4 ();
  uint16_t port = transport.GetPort ();
  return DoSendTo (p, ipv4, port);
}

Ptr<Packet>
UdpSocket::Recv (uint32_t maxSize, uint32_t flags)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_deliveryQueue.empty() )
    {
      return 0;
    }
  Ptr<Packet> p = m_deliveryQueue.front ();
  if (p->GetSize () <= maxSize) 
    {
      m_deliveryQueue.pop ();
      m_rxAvailable -= p->GetSize ();
    }
  else
    {
      p = 0; 
    }
  return p;
}

uint32_t
UdpSocket::GetRxAvailable (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  // We separately maintain this state to avoid walking the queue 
  // every time this might be called
  return m_rxAvailable;
}

void 
UdpSocket::ForwardUp (Ptr<Packet> packet, Ipv4Address ipv4, uint16_t port)
{
  NS_LOG_FUNCTION (this << packet << ipv4 << port);

  if (m_shutdownRecv)
    {
      return;
    }
  if ((m_rxAvailable + packet->GetSize ()) <= m_rcvBufLimit)
    {
      Address address = InetSocketAddress (ipv4, port);
      SocketRxAddressTag tag;
      tag.SetAddress (address);
      packet->AddTag (tag);
      m_deliveryQueue.push (packet);
      m_rxAvailable += packet->GetSize ();
      NotifyDataRecv ();
    }
  else
    {
      // In general, this case should not occur unless the
      // receiving application reads data from this socket slowly
      // in comparison to the arrival rate
      //
      // drop and trace packet
      NS_LOG_WARN ("No receive buffer space available.  Drop.");
      m_dropTrace (packet);
    }
}

void 
UdpSocket::SetSndBuf (uint32_t size)
{
  NS_LOG_FUNCTION_NOARGS ();
  // return EINVAL since we are not modelling a finite send buffer
  // Enforcing buffer size should be added if we ever start to model
  // non-zero processing delay in the UDP/IP stack
  NS_LOG_WARN ("UdpSocket has infinite send buffer");
  m_sndBufLimit = size; 
}

uint32_t 
UdpSocket::GetSndBuf (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_sndBufLimit;
}

void 
UdpSocket::SetRcvBuf (uint32_t size)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_rcvBufLimit = size;
}

uint32_t 
UdpSocket::GetRcvBuf (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_rcvBufLimit;
}


} //namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include "ns3/socket-factory.h"
#include "ns3/udp.h"
#include "ns3/simulator.h"
#include "ns3/simple-channel.h"
#include "ns3/simple-net-device.h"
#include "ns3/drop-tail-queue.h"
#include "internet-stack.h"
#include <string>

namespace ns3 {

class UdpSocketTest: public Test
{
  Ptr<Packet> m_receivedPacket;
  Ptr<Packet> m_receivedPacket2;

public:
  virtual bool RunTests (void);
  UdpSocketTest ();

  void ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from);
  void ReceivePacket2 (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from);
  void ReceivePkt (Ptr<Socket> socket);
  void ReceivePkt2 (Ptr<Socket> socket);
};


UdpSocketTest::UdpSocketTest ()
  : Test ("UdpSocket") {}


void UdpSocketTest::ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from)
{
  m_receivedPacket = packet;
}

void UdpSocketTest::ReceivePacket2 (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from)
{
  m_receivedPacket2 = packet;
}

void UdpSocketTest::ReceivePkt (Ptr<Socket> socket)
{
  uint32_t availableData = socket->GetRxAvailable ();
  m_receivedPacket = socket->Recv (std::numeric_limits<uint32_t>::max(), 0);
  NS_ASSERT (availableData == m_receivedPacket->GetSize ());
}

void UdpSocketTest::ReceivePkt2 (Ptr<Socket> socket)
{
  uint32_t availableData = socket->GetRxAvailable ();
  m_receivedPacket2 = socket->Recv (std::numeric_limits<uint32_t>::max(), 0);
  NS_ASSERT (availableData == m_receivedPacket2->GetSize ());
}

bool
UdpSocketTest::RunTests (void)
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
    ipv4->SetAddress (netdev_idx, Ipv4Address ("10.0.0.1"));
    ipv4->SetNetworkMask (netdev_idx, Ipv4Mask (0xffff0000U));
    ipv4->SetUp (netdev_idx);
  }

  { // second interface
    rxDev2 = CreateObject<SimpleNetDevice> ();
    rxDev2->SetAddress (Mac48Address::Allocate ());
    rxNode->AddDevice (rxDev2);
    Ptr<Ipv4> ipv4 = rxNode->GetObject<Ipv4> ();
    uint32_t netdev_idx = ipv4->AddInterface (rxDev2);
    ipv4->SetAddress (netdev_idx, Ipv4Address ("10.0.1.1"));
    ipv4->SetNetworkMask (netdev_idx, Ipv4Mask (0xffff0000U));
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
    ipv4->SetAddress (netdev_idx, Ipv4Address ("10.0.0.2"));
    ipv4->SetNetworkMask (netdev_idx, Ipv4Mask (0xffff0000U));
    ipv4->SetUp (netdev_idx);
  }
  Ptr<SimpleNetDevice> txDev2;
  {
    txDev2 = CreateObject<SimpleNetDevice> ();
    txDev2->SetAddress (Mac48Address::Allocate ());
    txNode->AddDevice (txDev2);
    Ptr<Ipv4> ipv4 = txNode->GetObject<Ipv4> ();
    uint32_t netdev_idx = ipv4->AddInterface (txDev2);
    ipv4->SetAddress (netdev_idx, Ipv4Address ("10.0.1.2"));
    ipv4->SetNetworkMask (netdev_idx, Ipv4Mask (0xffff0000U));
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
  Ptr<SocketFactory> rxSocketFactory = rxNode->GetObject<Udp> ();
  Ptr<Socket> rxSocket = rxSocketFactory->CreateSocket ();
  NS_TEST_ASSERT_EQUAL (rxSocket->Bind (InetSocketAddress (Ipv4Address ("10.0.0.1"), 1234)), 0);
  rxSocket->SetRecvCallback (MakeCallback (&UdpSocketTest::ReceivePkt, this));

  Ptr<Socket> rxSocket2 = rxSocketFactory->CreateSocket ();
  rxSocket2->SetRecvCallback (MakeCallback (&UdpSocketTest::ReceivePkt2, this));
  NS_TEST_ASSERT_EQUAL (rxSocket2->Bind (InetSocketAddress (Ipv4Address ("10.0.1.1"), 1234)), 0);

  Ptr<SocketFactory> txSocketFactory = txNode->GetObject<Udp> ();
  Ptr<Socket> txSocket = txSocketFactory->CreateSocket ();

  // ------ Now the tests ------------

  // Unicast test
  m_receivedPacket = Create<Packet> ();
  m_receivedPacket2 = Create<Packet> ();
  NS_TEST_ASSERT_EQUAL (txSocket->SendTo (InetSocketAddress (Ipv4Address("10.0.0.1"), 1234),
                                          Create<Packet> (123)), 123);
  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_receivedPacket->GetSize (), 123);
  NS_TEST_ASSERT_EQUAL (m_receivedPacket2->GetSize (), 0); // second interface should receive it

  m_receivedPacket->RemoveAllTags ();
  m_receivedPacket2->RemoveAllTags ();

  // Simple broadcast test

  m_receivedPacket = Create<Packet> ();
  m_receivedPacket2 = Create<Packet> ();
  NS_TEST_ASSERT_EQUAL (txSocket->SendTo (InetSocketAddress (Ipv4Address("255.255.255.255"), 1234),
                                          Create<Packet> (123)), 123);
  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_receivedPacket->GetSize (), 123);
  // second socket should not receive it (it is bound specifically to the second interface's address
  NS_TEST_ASSERT_EQUAL (m_receivedPacket2->GetSize (), 0);

  m_receivedPacket->RemoveAllTags ();
  m_receivedPacket2->RemoveAllTags ();

  // Broadcast test with multiple receiving sockets

  // When receiving broadcast packets, all sockets sockets bound to
  // the address/port should receive a copy of the same packet -- if
  // the socket address matches.
  rxSocket2->Dispose ();
  rxSocket2 = rxSocketFactory->CreateSocket ();
  rxSocket2->SetRecvCallback (MakeCallback (&UdpSocketTest::ReceivePkt2, this));
  NS_TEST_ASSERT_EQUAL (rxSocket2->Bind (InetSocketAddress (Ipv4Address ("0.0.0.0"), 1234)), 0);

  m_receivedPacket = Create<Packet> ();
  m_receivedPacket2 = Create<Packet> ();
  NS_TEST_ASSERT_EQUAL (txSocket->SendTo (InetSocketAddress (Ipv4Address("255.255.255.255"), 1234),
                                          Create<Packet> (123)), 123);
  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_receivedPacket->GetSize (), 123);
  NS_TEST_ASSERT_EQUAL (m_receivedPacket2->GetSize (), 123);

  m_receivedPacket->RemoveAllTags ();
  m_receivedPacket2->RemoveAllTags ();

  Simulator::Destroy ();

  return result;
}


static UdpSocketTest gUdpSocketTest;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */
