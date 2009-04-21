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
#include "ns3/udp-socket-factory.h"
#include "ns3/trace-source-accessor.h"
#include "udp-socket-impl.h"
#include "udp-l4-protocol.h"
#include "ipv4-end-point.h"
#include <limits>

NS_LOG_COMPONENT_DEFINE ("UdpSocketImpl");

namespace ns3 {

static const uint32_t MAX_IPV4_UDP_DATAGRAM_SIZE = 65507;

// Add attributes generic to all UdpSockets to base class UdpSocket
TypeId
UdpSocketImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpSocketImpl")
    .SetParent<UdpSocket> ()
    .AddConstructor<UdpSocketImpl> ()
    .AddTraceSource ("Drop", "Drop UDP packet due to receive buffer overflow",
                     MakeTraceSourceAccessor (&UdpSocketImpl::m_dropTrace))
    .AddAttribute ("IcmpCallback", "Callback invoked whenever an icmp error is received on this socket.",
                   CallbackValue (),
                   MakeCallbackAccessor (&UdpSocketImpl::m_icmpCallback),
                   MakeCallbackChecker ())
    ;
  return tid;
}

UdpSocketImpl::UdpSocketImpl ()
  : m_endPoint (0),
    m_node (0),
    m_udp (0),
    m_errno (ERROR_NOTERROR),
    m_shutdownSend (false),
    m_shutdownRecv (false),
    m_connected (false),
    m_rxAvailable (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

UdpSocketImpl::~UdpSocketImpl ()
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
UdpSocketImpl::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = node;

}
void 
UdpSocketImpl::SetUdp (Ptr<UdpL4Protocol> udp)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_udp = udp;
}


enum Socket::SocketErrno
UdpSocketImpl::GetErrno (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_errno;
}

Ptr<Node>
UdpSocketImpl::GetNode (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_node;
}

void 
UdpSocketImpl::Destroy (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_endPoint = 0;
  m_udp = 0;
}

int
UdpSocketImpl::FinishBind (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_endPoint == 0)
    {
      return -1;
    }
  m_endPoint->SetRxCallback (MakeCallback (&UdpSocketImpl::ForwardUp, Ptr<UdpSocketImpl> (this)));
  m_endPoint->SetIcmpCallback (MakeCallback (&UdpSocketImpl::ForwardIcmp, Ptr<UdpSocketImpl> (this)));
  m_endPoint->SetDestroyCallback (MakeCallback (&UdpSocketImpl::Destroy, Ptr<UdpSocketImpl> (this)));
  return 0;
}

int
UdpSocketImpl::Bind (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_endPoint = m_udp->Allocate ();
  return FinishBind ();
}

int 
UdpSocketImpl::Bind (const Address &address)
{
  NS_LOG_FUNCTION (this << address);

  if (!InetSocketAddress::IsMatchingType (address))
    {
      NS_LOG_ERROR ("Not IsMatchingType");
      m_errno = ERROR_INVAL;
      return -1;
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
UdpSocketImpl::ShutdownSend (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_shutdownSend = true;
  return 0;
}

int 
UdpSocketImpl::ShutdownRecv (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_shutdownRecv = false;
  return 0;
}

int
UdpSocketImpl::Close(void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return 0;
}

int
UdpSocketImpl::Connect(const Address & address)
{
  NS_LOG_FUNCTION (this << address);
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  m_defaultAddress = transport.GetIpv4 ();
  m_defaultPort = transport.GetPort ();
  NotifyConnectionSucceeded ();
  m_connected = true;

  return 0;
}

int 
UdpSocketImpl::Listen (void)
{
  m_errno = Socket::ERROR_OPNOTSUPP;
  return -1;
}

int 
UdpSocketImpl::Send (Ptr<Packet> p, uint32_t flags)
{
  NS_LOG_FUNCTION (this << p << flags);

  if (!m_connected)
    {
      m_errno = ERROR_NOTCONN;
      return -1;
    }
  return DoSend (p);
}

int 
UdpSocketImpl::DoSend (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
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
UdpSocketImpl::DoSendTo (Ptr<Packet> p, const Address &address)
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
UdpSocketImpl::DoSendTo (Ptr<Packet> p, Ipv4Address dest, uint16_t port)
{
  NS_LOG_FUNCTION (this << p << dest << port);

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

  if (p->GetSize () > GetTxAvailable () )
    {
      m_errno = ERROR_MSGSIZE;
      return -1;
    }

  uint32_t localInterface;
  Ptr<Ipv4> ipv4 = m_node->GetObject<Ipv4> ();

  // Locally override the IP TTL for this socket
  // We cannot directly modify the TTL at this stage, so we set a Packet tag
  // The destination can be either multicast, unicast/anycast, or
  // either all-hosts broadcast or limited (subnet-directed) broadcast.
  // For the latter two broadcast types, the TTL will later be set to one
  // irrespective of what is set in these socket options.  So, this tagging  
  // may end up setting the TTL of a limited broadcast packet to be
  // the same as a unicast, but it will be fixed further down the stack
  //NS_LOG_UNCOND ("IPttl: " << m_ipTtl);
  if (m_ipMulticastTtl != 0 && dest.IsMulticast ())
    {
      SocketIpTtlTag tag;
      tag.SetTtl (m_ipMulticastTtl);
      p->AddPacketTag (tag);
    }
  else if (m_ipTtl != 0 && !dest.IsMulticast () && !dest.IsBroadcast ())
    {
      SocketIpTtlTag tag;
      tag.SetTtl (m_ipTtl);
      p->AddPacketTag (tag);
    }
  {
    SocketSetDontFragmentTag tag;
    bool found = p->RemovePacketTag (tag);
    if (!found)
      {
        if (m_mtuDiscover)
          {
            tag.Enable ();
          }
        else
          {
            tag.Disable ();
          }
        p->AddPacketTag (tag);
      }
  }
  //
  // If dest is set to the limited broadcast address (all ones),
  // convert it to send a copy of the packet out of every 
  // interface as a subnet-directed broadcast.
  // Exception:  if the interface has a /32 address, there is no
  // valid subnet-directed broadcast, so send it as limited broadcast
  // Note also that some systems will only send limited broadcast packets
  // out of the "default" interface; here we send it out all interfaces
  //
  if (dest.IsBroadcast ())
    {
      NS_LOG_LOGIC ("Limited broadcast start.");
      for (uint32_t i = 0; i < ipv4->GetNInterfaces (); i++ )
        {
          // Get the primary address
          Ipv4InterfaceAddress iaddr = ipv4->GetAddress (i, 0);
          Ipv4Address addri = iaddr.GetLocal ();
          Ipv4Mask maski = iaddr.GetMask ();
          if (maski == Ipv4Mask::GetOnes ())
            {
              // if the network mask is 255.255.255.255, do not convert dest
              NS_LOG_LOGIC ("Sending one copy from " << addri << " to " << dest
                            << " (mask is " << maski << ")");
              m_udp->Send (p->Copy (), addri, dest,
                           m_endPoint->GetLocalPort (), port);
              NotifyDataSent (p->GetSize ());
              NotifySend (GetTxAvailable ());
            }
          else
            {
              // Convert to subnet-directed broadcast
              Ipv4Address bcast = addri.GetSubnetDirectedBroadcast (maski);
              NS_LOG_LOGIC ("Sending one copy from " << addri << " to " << bcast
                            << " (mask is " << maski << ")");
              m_udp->Send (p->Copy (), addri, bcast,
                           m_endPoint->GetLocalPort (), port);
              NotifyDataSent (p->GetSize ());
              NotifySend (GetTxAvailable ());
            }
        }
      NS_LOG_LOGIC ("Limited broadcast end.");
      return p->GetSize();
    }
  else if (ipv4->GetInterfaceForDestination(dest, localInterface))
    {
      NS_LOG_LOGIC ("Route exists");
      m_udp->Send (p->Copy (), ipv4->GetSourceAddress (dest), dest,
		   m_endPoint->GetLocalPort (), port);
      NotifyDataSent (p->GetSize ());
      NotifySend (GetTxAvailable ());
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

// XXX maximum message size for UDP broadcast is limited by MTU
// size of underlying link; we are not checking that now.
uint32_t
UdpSocketImpl::GetTxAvailable (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  // No finite send buffer is modelled, but we must respect
  // the maximum size of an IP datagram (65535 bytes - headers).
  return MAX_IPV4_UDP_DATAGRAM_SIZE;
}

int 
UdpSocketImpl::SendTo (Ptr<Packet> p, uint32_t flags, const Address &address)
{
  NS_LOG_FUNCTION (this << p << flags << address);
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  Ipv4Address ipv4 = transport.GetIpv4 ();
  uint16_t port = transport.GetPort ();
  return DoSendTo (p, ipv4, port);
}

uint32_t
UdpSocketImpl::GetRxAvailable (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  // We separately maintain this state to avoid walking the queue 
  // every time this might be called
  return m_rxAvailable;
}

Ptr<Packet>
UdpSocketImpl::Recv (uint32_t maxSize, uint32_t flags)
{
  NS_LOG_FUNCTION (this << maxSize << flags);
  if (m_deliveryQueue.empty() )
    {
      m_errno = ERROR_AGAIN;
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

Ptr<Packet>
UdpSocketImpl::RecvFrom (uint32_t maxSize, uint32_t flags, 
                         Address &fromAddress)
{
  NS_LOG_FUNCTION (this << maxSize << flags);
  Ptr<Packet> packet = Recv (maxSize, flags);
  if (packet != 0)
    {
      SocketAddressTag tag;
      bool found;
      found = packet->PeekPacketTag (tag);
      NS_ASSERT (found);
      fromAddress = tag.GetAddress ();
    }
  return packet;
}

int
UdpSocketImpl::GetSockName (Address &address) const
{
  NS_LOG_FUNCTION_NOARGS ();
  if (m_endPoint != 0)
    {
      address = InetSocketAddress (m_endPoint->GetLocalAddress (), m_endPoint->GetLocalPort());
    }
  else
    {
      address = InetSocketAddress(Ipv4Address::GetZero(), 0);
    }
  return 0;
}

void 
UdpSocketImpl::ForwardUp (Ptr<Packet> packet, Ipv4Address ipv4, uint16_t port)
{
  NS_LOG_FUNCTION (this << packet << ipv4 << port);

  if (m_shutdownRecv)
    {
      return;
    }
  if ((m_rxAvailable + packet->GetSize ()) <= m_rcvBufSize)
    {
      Address address = InetSocketAddress (ipv4, port);
      SocketAddressTag tag;
      tag.SetAddress (address);
      packet->AddPacketTag (tag);
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
UdpSocketImpl::ForwardIcmp (Ipv4Address icmpSource, uint8_t icmpTtl, 
                            uint8_t icmpType, uint8_t icmpCode,
                            uint32_t icmpInfo)
{
  NS_LOG_FUNCTION (this << icmpSource << (uint32_t)icmpTtl << (uint32_t)icmpType <<
                   (uint32_t)icmpCode << icmpInfo);
  if (!m_icmpCallback.IsNull ())
    {
      m_icmpCallback (icmpSource, icmpTtl, icmpType, icmpCode, icmpInfo);
    }
}


void 
UdpSocketImpl::SetRcvBufSize (uint32_t size)
{
  m_rcvBufSize = size;
}

uint32_t 
UdpSocketImpl::GetRcvBufSize (void) const
{
  return m_rcvBufSize;
}

void 
UdpSocketImpl::SetIpTtl (uint32_t ipTtl)
{
  m_ipTtl = ipTtl;
}

uint32_t 
UdpSocketImpl::GetIpTtl (void) const
{
  return m_ipTtl;
}

void 
UdpSocketImpl::SetIpMulticastTtl (uint32_t ipTtl)
{
  m_ipMulticastTtl = ipTtl;
}

uint32_t 
UdpSocketImpl::GetIpMulticastTtl (void) const
{
  return m_ipMulticastTtl;
}

void 
UdpSocketImpl::SetMtuDiscover (bool discover)
{
  m_mtuDiscover = discover;
}
bool 
UdpSocketImpl::GetMtuDiscover (void) const
{
  return m_mtuDiscover;
}


} //namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include "ns3/socket-factory.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/simulator.h"
#include "ns3/simple-channel.h"
#include "ns3/simple-net-device.h"
#include "ns3/drop-tail-queue.h"
#include "internet-stack.h"
#include <string>

namespace ns3 {

class UdpSocketImplTest: public Test
{
  Ptr<Packet> m_receivedPacket;
  Ptr<Packet> m_receivedPacket2;

public:
  virtual bool RunTests (void);
  UdpSocketImplTest ();

  void ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from);
  void ReceivePacket2 (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from);
  void ReceivePkt (Ptr<Socket> socket);
  void ReceivePkt2 (Ptr<Socket> socket);
};


UdpSocketImplTest::UdpSocketImplTest ()
  : Test ("UdpSocketImpl") 
{
}

void UdpSocketImplTest::ReceivePacket (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from)
{
  m_receivedPacket = packet;
}

void UdpSocketImplTest::ReceivePacket2 (Ptr<Socket> socket, Ptr<Packet> packet, const Address &from)
{
  m_receivedPacket2 = packet;
}

void UdpSocketImplTest::ReceivePkt (Ptr<Socket> socket)
{
  uint32_t availableData;
  availableData = socket->GetRxAvailable ();
  m_receivedPacket = socket->Recv (std::numeric_limits<uint32_t>::max(), 0);
  NS_ASSERT (availableData == m_receivedPacket->GetSize ());
}

void UdpSocketImplTest::ReceivePkt2 (Ptr<Socket> socket)
{
  uint32_t availableData;
  availableData = socket->GetRxAvailable ();
  m_receivedPacket2 = socket->Recv (std::numeric_limits<uint32_t>::max(), 0);
  NS_ASSERT (availableData == m_receivedPacket2->GetSize ());
}

bool
UdpSocketImplTest::RunTests (void)
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
    Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.0.1"), Ipv4Mask (0xffff0000U));
    ipv4->AddAddress (netdev_idx, ipv4Addr);
    ipv4->SetUp (netdev_idx);
  }

  { // second interface
    rxDev2 = CreateObject<SimpleNetDevice> ();
    rxDev2->SetAddress (Mac48Address::Allocate ());
    rxNode->AddDevice (rxDev2);
    Ptr<Ipv4> ipv4 = rxNode->GetObject<Ipv4> ();
    uint32_t netdev_idx = ipv4->AddInterface (rxDev2);
    Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.1.1"), Ipv4Mask (0xffff0000U));
    ipv4->AddAddress (netdev_idx, ipv4Addr);
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
    Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.0.2"), Ipv4Mask (0xffff0000U));
    ipv4->AddAddress (netdev_idx, ipv4Addr);
    ipv4->SetUp (netdev_idx);
  }
  Ptr<SimpleNetDevice> txDev2;
  {
    txDev2 = CreateObject<SimpleNetDevice> ();
    txDev2->SetAddress (Mac48Address::Allocate ());
    txNode->AddDevice (txDev2);
    Ptr<Ipv4> ipv4 = txNode->GetObject<Ipv4> ();
    uint32_t netdev_idx = ipv4->AddInterface (txDev2);
    Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (Ipv4Address ("10.0.1.2"), Ipv4Mask (0xffff0000U));
    ipv4->AddAddress (netdev_idx, ipv4Addr);
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
  Ptr<SocketFactory> rxSocketFactory = rxNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> rxSocket = rxSocketFactory->CreateSocket ();
  NS_TEST_ASSERT_EQUAL (rxSocket->Bind (InetSocketAddress (Ipv4Address ("10.0.0.1"), 1234)), 0);
  rxSocket->SetRecvCallback (MakeCallback (&UdpSocketImplTest::ReceivePkt, this));

  Ptr<Socket> rxSocket2 = rxSocketFactory->CreateSocket ();
  rxSocket2->SetRecvCallback (MakeCallback (&UdpSocketImplTest::ReceivePkt2, this));
  NS_TEST_ASSERT_EQUAL (rxSocket2->Bind (InetSocketAddress (Ipv4Address ("10.0.1.1"), 1234)), 0);

  Ptr<SocketFactory> txSocketFactory = txNode->GetObject<UdpSocketFactory> ();
  Ptr<Socket> txSocket = txSocketFactory->CreateSocket ();

  // ------ Now the tests ------------

  // Unicast test
  m_receivedPacket = Create<Packet> ();
  m_receivedPacket2 = Create<Packet> ();
  NS_TEST_ASSERT_EQUAL (txSocket->SendTo ( Create<Packet> (123), 0, 
    InetSocketAddress (Ipv4Address("10.0.0.1"), 1234)), 123);
  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_receivedPacket->GetSize (), 123);
  NS_TEST_ASSERT_EQUAL (m_receivedPacket2->GetSize (), 0); // second interface should receive it

  m_receivedPacket->RemoveAllPacketTags ();
  m_receivedPacket2->RemoveAllPacketTags ();

  // Simple broadcast test

  m_receivedPacket = Create<Packet> ();
  m_receivedPacket2 = Create<Packet> ();
  NS_TEST_ASSERT_EQUAL (txSocket->SendTo ( Create<Packet> (123), 0, 
    InetSocketAddress (Ipv4Address("255.255.255.255"), 1234)), 123);
  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_receivedPacket->GetSize (), 123);
  // second socket should not receive it (it is bound specifically to the second interface's address
  NS_TEST_ASSERT_EQUAL (m_receivedPacket2->GetSize (), 0);

  m_receivedPacket->RemoveAllPacketTags ();
  m_receivedPacket2->RemoveAllPacketTags ();

  // Broadcast test with multiple receiving sockets

  // When receiving broadcast packets, all sockets sockets bound to
  // the address/port should receive a copy of the same packet -- if
  // the socket address matches.
  rxSocket2->Dispose ();
  rxSocket2 = rxSocketFactory->CreateSocket ();
  rxSocket2->SetRecvCallback (MakeCallback (&UdpSocketImplTest::ReceivePkt2, this));
  NS_TEST_ASSERT_EQUAL (rxSocket2->Bind (InetSocketAddress (Ipv4Address ("0.0.0.0"), 1234)), 0);

  m_receivedPacket = Create<Packet> ();
  m_receivedPacket2 = Create<Packet> ();
  NS_TEST_ASSERT_EQUAL (txSocket->SendTo (Create<Packet> (123), 0,
InetSocketAddress (Ipv4Address("255.255.255.255"), 1234)), 123);
  Simulator::Run ();
  NS_TEST_ASSERT_EQUAL (m_receivedPacket->GetSize (), 123);
  NS_TEST_ASSERT_EQUAL (m_receivedPacket2->GetSize (), 123);

  m_receivedPacket->RemoveAllPacketTags ();
  m_receivedPacket2->RemoveAllPacketTags ();

  Simulator::Destroy ();

  return result;
}

static UdpSocketImplTest gUdpSocketImplTest;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */
