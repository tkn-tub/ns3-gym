/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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

#include "ns3/debug.h"
#include "ns3/node.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-route.h"
#include "ns3/ipv4.h"
#include "udp-socket.h"
#include "udp-l4-protocol.h"
#include "ipv4-end-point.h"
#include "ipv4-l4-demux.h"
#include "ns3/ipv4.h"

NS_DEBUG_COMPONENT_DEFINE ("UdpSocket");

namespace ns3 {

UdpSocket::UdpSocket (Ptr<Node> node, Ptr<UdpL4Protocol> udp)
  : m_endPoint (0),
    m_node (node),
    m_udp (udp),
    m_errno (ERROR_NOTERROR),
    m_shutdownSend (false),
    m_shutdownRecv (false),
    m_connected (false)
{
  NS_DEBUG("UdpSocket::UdpSocket ()");
}

UdpSocket::~UdpSocket ()
{
  NS_DEBUG("UdpSocket::~UdpSocket ()");

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

enum Socket::SocketErrno
UdpSocket::GetErrno (void) const
{
  NS_DEBUG("UdpSocket::GetErrno ()");

  return m_errno;
}

Ptr<Node>
UdpSocket::GetNode (void) const
{
  NS_DEBUG("UdpSocket::GetNode ()");
  return m_node;
}

void 
UdpSocket::Destroy (void)
{
  NS_DEBUG("UdpSocket::Destroy ()");
  m_node = 0;
  m_endPoint = 0;
  m_udp = 0;
}

int
UdpSocket::FinishBind (void)
{
  NS_DEBUG("UdpSocket::FinishBind ()");

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
  NS_DEBUG("UdpSocket::Bind ()");

  m_endPoint = m_udp->Allocate ();
  return FinishBind ();
}

int 
UdpSocket::Bind (const Address &address)
{
  NS_DEBUG("UdpSocket::Bind (" << address << ")");

  if (!InetSocketAddress::IsMatchingType (address))
    {
      NS_DEBUG("UdpSocket::Bind (): Not IsMatchingType");
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
  NS_DEBUG("UdpSocket::ShutDownSend ()");
  m_shutdownSend = true;
  return 0;
}

int 
UdpSocket::ShutdownRecv (void)
{
  NS_DEBUG("UdpSocket::ShutDownRecv ()");
  m_shutdownRecv = false;
  return 0;
}

int
UdpSocket::Close(void)
{
  NS_DEBUG("UdpSocket::Close ()");
  NotifyCloseCompleted ();
  return 0;
}

int
UdpSocket::Connect(const Address & address)
{
  NS_DEBUG ("UdpSocket::Connect (" << address << ")");
  Ipv4Route routeToDest;
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  m_defaultAddress = transport.GetIpv4 ();
  m_defaultPort = transport.GetPort ();
  NotifyConnectionSucceeded ();
  m_connected = true;

  NS_DEBUG ("UdpSocket::Connect (): Updating local address");

  uint32_t localIfIndex;

  Ptr<Ipv4> ipv4 = m_node->QueryInterface<Ipv4> (Ipv4::iid);
  
  if (ipv4->GetIfIndexForDestination (m_defaultAddress, localIfIndex))
    {
      m_endPoint->SetLocalAddress (ipv4->GetAddress(localIfIndex));
    }

  NS_DEBUG ("UdpSocket::Connect (): Local address is " << 
    m_endPoint->GetLocalAddress());

  return 0;
}

int 
UdpSocket::Send (const Packet &p)
{
  NS_DEBUG("UdpSocket::Send (" << &p << ")");

  if (!m_connected)
    {
      m_errno = ERROR_NOTCONN;
      return -1;
    }
  return DoSend (p);
}

int 
UdpSocket::DoSend (const Packet &p)
{
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
UdpSocket::DoSendTo (const Packet &p, const Address &address)
{
  NS_DEBUG("UdpSocket::DoSendTo (" << &p << ", " << address << ")");

  if (!m_connected)
    {
      NS_DEBUG("UdpSocket::DoSendTo (): Not connected");
      InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
      Ipv4Address ipv4 = transport.GetIpv4 ();
      uint16_t port = transport.GetPort ();
      return DoSendTo (p, ipv4, port);
    }
  else
    {
      // connected UDP socket must use default addresses
      NS_DEBUG("UdpSocket::DoSendTo (): Connected");
      return DoSendTo (p, m_defaultAddress, m_defaultPort);
    }
}

int
UdpSocket::DoSendTo (const Packet &p, Ipv4Address dest, uint16_t port)
{
  NS_DEBUG("UdpSocket::DoSendTo (" << &p << ", " << dest << ", " <<
    port << ")");

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
  Ptr<Ipv4> ipv4 = m_node->QueryInterface<Ipv4> (Ipv4::iid);

  //
  // If dest is sent to the limited broadcast address (all ones),
  // convert it to send a copy of the packet out of every interface
  //
  if (dest.IsBroadcast ())
    {
      NS_DEBUG("UdpSocket::DoSendTo (): Limited broadcast");
      for (uint32_t i = 0; i < ipv4->GetNInterfaces (); i++ )
        {
          Ipv4Address addri = ipv4->GetAddress (i);
          Ipv4Mask maski = ipv4->GetNetworkMask (i);
          m_udp->Send (p, addri, addri.GetSubnetDirectedBroadcast (maski),
                       m_endPoint->GetLocalPort (), port);
          NotifyDataSent (p.GetSize ());
        }
    }
  else if (ipv4->GetIfIndexForDestination(dest, localIfIndex))
    {
      NS_DEBUG("UdpSocket::DoSendTo (): Route exists");
      m_udp->Send (p, ipv4->GetAddress (localIfIndex), dest,
		   m_endPoint->GetLocalPort (), port);
      NotifyDataSent (p.GetSize ());
      return 0;
    }
  else
   {
      NS_DEBUG("UdpSocket::DoSendTo (): ERROR_NOROUTETOHOST");
      m_errno = ERROR_NOROUTETOHOST;
      return -1;
   }

  return 0;
}

int 
UdpSocket::SendTo(const Address &address, const Packet &p)
{
  NS_DEBUG("UdpSocket::SendTo (" << address << ", " << &p << ")");
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  Ipv4Address ipv4 = transport.GetIpv4 ();
  uint16_t port = transport.GetPort ();
  return DoSendTo (p, ipv4, port);
}

void 
UdpSocket::ForwardUp (const Packet &packet, Ipv4Address ipv4, uint16_t port)
{
  NS_DEBUG("UdpSocket::ForwardUp (" << &packet << ", " << ipv4 << ", " <<
    port << ")");

  if (m_shutdownRecv)
    {
      return;
    }
  
  Address address = InetSocketAddress (ipv4, port);
  Packet p = packet;
  NotifyDataReceived (p, address);
}

} //namespace ns3
