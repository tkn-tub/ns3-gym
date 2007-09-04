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
#include "ns3/node.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-route.h"
#include "ns3/ipv4.h"
#include "udp-socket.h"
#include "udp-l4-protocol.h"
#include "ipv4-end-point.h"
#include "ipv4-l4-demux.h"

namespace ns3 {

UdpSocket::UdpSocket (Ptr<Node> node, Ptr<UdpL4Protocol> udp)
  : m_endPoint (0),
    m_node (node),
    m_udp (udp),
    m_errno (ERROR_NOTERROR),
    m_shutdownSend (false),
    m_shutdownRecv (false),
    m_connected (false)
{}
UdpSocket::~UdpSocket ()
{
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
  return m_errno;
}

Ptr<Node>
UdpSocket::GetNode (void) const
{
  return m_node;
}

void 
UdpSocket::Destroy (void)
{
  m_node = 0;
  m_endPoint = 0;
  m_udp = 0;
}
int
UdpSocket::FinishBind (void)
{
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
  m_endPoint = m_udp->Allocate ();
  return FinishBind ();
}
int 
UdpSocket::Bind (const Address &address)
{
  if (!InetSocketAddress::IsMatchingType (address))
    {
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
  m_shutdownSend = true;
  return 0;
}
int 
UdpSocket::ShutdownRecv (void)
{
  m_shutdownRecv = false;
  return 0;
}

int
UdpSocket::Close(void)
{
  NotifyCloseCompleted ();
  return 0;
}

int
UdpSocket::Connect(const Address & address)
{
  Ipv4Route routeToDest;
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  m_defaultAddress = transport.GetIpv4 ();
  m_defaultPort = transport.GetPort ();
  if (m_defaultAddress.IsBroadcast () )
    {
      NS_ASSERT_MSG(false, "UdpSocket::Connect, can't connect to broadcast");
    }
  NotifyConnectionSucceeded ();
  m_connected = true;
  if (GetIpv4RouteToDestination (m_node, routeToDest, m_defaultAddress) )
    {
      uint32_t localIfIndex = routeToDest.GetInterface ();
      Ptr<Ipv4> ipv4 = m_node->QueryInterface<Ipv4> (Ipv4::iid);
      m_endPoint->SetLocalAddress (ipv4->GetAddress(localIfIndex) );
    }
  return 0;
}

int 
UdpSocket::Send (const Packet &p)
{
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
  m_udp->Send (p, m_endPoint->GetLocalAddress (), m_defaultAddress,
                  m_endPoint->GetLocalPort (), m_defaultPort);
  NotifyDataSent (p.GetSize ());
  return 0;
}


int
UdpSocket::DoSendTo (const Packet &p, const Address &address)
{
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  Ipv4Address ipv4 = transport.GetIpv4 ();
  uint16_t port = transport.GetPort ();
  return DoSendTo (p, ipv4, port);
}
int
UdpSocket::DoSendTo (const Packet &p, Ipv4Address dest, uint16_t port)
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
  Ipv4Route routeToDest;
  if (GetIpv4RouteToDestination (m_node, routeToDest, dest) )
    {
      uint32_t localIfIndex = routeToDest.GetInterface ();
      Ptr<Ipv4> ipv4 = m_node->QueryInterface<Ipv4> (Ipv4::iid);
      m_udp->Send (p, ipv4->GetAddress (localIfIndex), dest,
		   m_endPoint->GetLocalPort (), port);
      NotifyDataSent (p.GetSize ());
      return 0;
    }
  else
   {
      m_errno = ERROR_NOROUTETOHOST;
      return -1;
   }
  return 0;
}

int 
UdpSocket::SendTo(const Address &address, const Packet &p)
{
  InetSocketAddress transport = InetSocketAddress::ConvertFrom (address);
  Ipv4Address ipv4 = transport.GetIpv4 ();
  uint16_t port = transport.GetPort ();
  return DoSendTo (p, ipv4, port);
}

void 
UdpSocket::ForwardUp (const Packet &packet, Ipv4Address ipv4, uint16_t port)
{
  if (m_shutdownRecv)
    {
      return;
    }
  
  Address address = InetSocketAddress (ipv4, port);
  Packet p = packet;
  NotifyDataReceived (p, address);
}

}//namespace ns3
