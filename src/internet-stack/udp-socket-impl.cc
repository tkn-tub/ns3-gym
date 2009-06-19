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
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-routing-protocol.h"
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

  // XXX todo:  leave any multicast groups that have been joined
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
  m_shutdownRecv = true;
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

  Ptr<Ipv4> ipv4 = m_node->GetObject<Ipv4> ();

  // Locally override the IP TTL for this socket
  // We cannot directly modify the TTL at this stage, so we set a Packet tag
  // The destination can be either multicast, unicast/anycast, or
  // either all-hosts broadcast or limited (subnet-directed) broadcast.
  // For the latter two broadcast types, the TTL will later be set to one
  // irrespective of what is set in these socket options.  So, this tagging  
  // may end up setting the TTL of a limited broadcast packet to be
  // the same as a unicast, but it will be fixed further down the stack
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
  else if (ipv4->GetRoutingProtocol () != 0)
    {
      Ipv4Header header;
      header.SetDestination (dest);
      Socket::SocketErrno errno_;
      Ptr<Ipv4Route> route;
      uint32_t oif = 0; //specify non-zero if bound to a source address
      // TBD-- we could cache the route and just check its validity
      route = ipv4->GetRoutingProtocol ()->RouteOutput (header, oif, errno_); 
      if (route != 0)
        {
          NS_LOG_LOGIC ("Route exists");
          header.SetSource (route->GetSource ());
          m_udp->Send (p->Copy (), header.GetSource (), header.GetDestination (),
                       m_endPoint->GetLocalPort (), port, route);
          NotifyDataSent (p->GetSize ());
          return p->GetSize();
        }
      else 
        {
          NS_LOG_LOGIC ("No route to destination");
          NS_LOG_ERROR (errno_);
          m_errno = errno_;
          return -1;
        }
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

int 
UdpSocketImpl::MulticastJoinGroup (uint32_t interface, const Address &groupAddress)
{
  NS_LOG_FUNCTION (interface << groupAddress);
  /*
   1) sanity check interface
   2) sanity check that it has not been called yet on this interface/group
   3) determine address family of groupAddress
   4) locally store a list of (interface, groupAddress)
   5) call ipv4->MulticastJoinGroup () or Ipv6->MulticastJoinGroup ()
  */
  return 0;
} 

int 
UdpSocketImpl::MulticastLeaveGroup (uint32_t interface, const Address &groupAddress) 
{
  NS_LOG_FUNCTION (interface << groupAddress);
  /*
   1) sanity check interface
   2) determine address family of groupAddress
   3) delete from local list of (interface, groupAddress); raise a LOG_WARN
      if not already present (but return 0) 
   5) call ipv4->MulticastLeaveGroup () or Ipv6->MulticastLeaveGroup ()
  */
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
UdpSocketImpl::SetIpTtl (uint8_t ipTtl)
{
  m_ipTtl = ipTtl;
}

uint8_t 
UdpSocketImpl::GetIpTtl (void) const
{
  return m_ipTtl;
}

void 
UdpSocketImpl::SetIpMulticastTtl (uint8_t ipTtl)
{
  m_ipMulticastTtl = ipTtl;
}

uint8_t 
UdpSocketImpl::GetIpMulticastTtl (void) const
{
  return m_ipMulticastTtl;
}

void 
UdpSocketImpl::SetIpMulticastIf (int32_t ipIf)
{
  m_ipMulticastIf = ipIf;
}

int32_t 
UdpSocketImpl::GetIpMulticastIf (void) const
{
  return m_ipMulticastIf;
}

void 
UdpSocketImpl::SetIpMulticastLoop (bool loop)
{
  m_ipMulticastLoop = loop;
}

bool 
UdpSocketImpl::GetIpMulticastLoop (void) const
{
  return m_ipMulticastLoop;
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
