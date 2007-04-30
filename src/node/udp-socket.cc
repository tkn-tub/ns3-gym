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
#include "udp-socket.h"
#include "udp.h"
#include "udp-end-point.h"
#include "node.h"
#include "ipv4-l4-demux.h"

namespace ns3 {

UdpSocket::UdpSocket (Node *node)
  : m_endPoint (0),
    m_node (node),
    m_errno (ENOTERROR),
    m_shutdownSend (false),
    m_shutdownRecv (false),
    m_connected (false)
{
  NS_ASSERT (GetUdp () != 0);
}
UdpSocket::~UdpSocket ()
{
  delete m_endPoint;
}

Node *
UdpSocket::GetNode (void) const
{
  return m_node;
}

int
UdpSocket::Bind (void)
{
  m_endPoint = GetUdp ()->Allocate ();
  if (m_endPoint == 0)
    {
      return -1;
    }
  m_endPoint->SetSocket (this);
  return 0;
}
int 
UdpSocket::Bind (Ipv4Address address)
{
  m_endPoint = GetUdp ()->Allocate (address);
  if (m_endPoint == 0)
    {
      return -1;
    }
  m_endPoint->SetSocket (this);
  return 0;
}
int 
UdpSocket::Bind (uint16_t port)
{
  m_endPoint = GetUdp ()->Allocate (port);
  if (m_endPoint == 0)
    {
      return -1;
    }
  m_endPoint->SetSocket (this);
  return 0;
}
int 
UdpSocket::Bind (Ipv4Address address, uint16_t port)
{
  m_endPoint = GetUdp ()->Allocate (address, port);
  if (m_endPoint == 0)
    {
      return -1;
    }
  m_endPoint->SetSocket (this);
  return 0;
}

enum Socket::SocketErrno
UdpSocket::GetErrno (void) const
{
  return m_errno;
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

void 
UdpSocket::DoClose(ns3::Callback<void, Socket*> closeCompleted)
{
  // XXX: we should set the close state and check it in all API methods.
  if (!closeCompleted.IsNull ())
    {
      closeCompleted (this);
    }
}
void 
UdpSocket::DoConnect(const Ipv4Address & address,
                     uint16_t portNumber,
                     ns3::Callback<void, Socket*> connectionSucceeded,
                     ns3::Callback<void, Socket*> connectionFailed,
                     ns3::Callback<void, Socket*> halfClose)
{
  m_defaultAddress = address;
  m_defaultPort = portNumber;
  if (!connectionSucceeded.IsNull ())
    {
      connectionSucceeded (this);
    }
  m_connected = true;
}
int
UdpSocket::DoAccept(ns3::Callback<bool, Socket*, const Ipv4Address&, uint16_t> connectionRequest,
                    ns3::Callback<void, Socket*, const Ipv4Address&, uint16_t> newConnectionCreated,
                    ns3::Callback<void, Socket*> closeRequested)
{
  // calling accept on a udp socket is a programming error.
  m_errno = EOPNOTSUPP;
  return -1;
}
int 
UdpSocket::DoSend (const uint8_t* buffer,
                   uint32_t size,
                   ns3::Callback<void, Socket*, uint32_t> dataSent)
{
  if (!m_connected)
    {
      m_errno = ENOTCONN;
      return -1;
    }
  Packet p;
  if (buffer == 0)
    {
      p = Packet (size);
    }
  else
    {
      p = Packet (buffer, size);
    }
  return DoSendPacketTo (p, m_defaultAddress, m_defaultPort, dataSent);
}
int
UdpSocket::DoSendPacketTo (const Packet &p, Ipv4Address daddr, uint16_t dport,
                           ns3::Callback<void, Socket*, uint32_t> dataSent)
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
      m_errno = ESHUTDOWN;
      return -1;
    }
  GetUdp ()->Send (p, m_endPoint->GetLocalAddress (), daddr,
		   m_endPoint->GetLocalPort (), dport);
  if (!dataSent.IsNull ())
    {
      dataSent (this, p.GetSize ());
    }
  return 0;
}
int 
UdpSocket::DoSendTo(const Ipv4Address &address,
                    uint16_t port,
                    const uint8_t *buffer,
                    uint32_t size,
                    ns3::Callback<void, Socket*, uint32_t> dataSent)
{
  if (m_connected)
    {
      m_errno = EISCONN;
      return -1;
    }
  Packet p;
  if (buffer == 0)
    {
      p = Packet (size);
    }
  else
    {
      p = Packet (buffer, size);
    }
  return DoSendPacketTo (p, address, port, dataSent);
}
void 
UdpSocket::DoRecv(ns3::Callback<void, Socket*, const uint8_t*, uint32_t,const Ipv4Address&, uint16_t> callback)
{
  m_rxCallback = callback;
}
void 
UdpSocket::DoRecvDummy(ns3::Callback<void, Socket*, uint32_t,const Ipv4Address&, uint16_t> callback)
{
  m_dummyRxCallback = callback;
}

void 
UdpSocket::ForwardUp (Packet &p, Ipv4Address saddr, uint16_t sport)
{
  if (m_shutdownRecv)
    {
      return;
    }
  if (!m_dummyRxCallback.IsNull ())
    {
      m_dummyRxCallback (this, p.GetSize (), saddr, sport);
    }
  if (!m_rxCallback.IsNull ())
    {
      m_rxCallback (this, p.PeekData (), p.GetSize (), saddr, sport);
    }
}

Udp *
UdpSocket::GetUdp (void) const
{
  return m_node->GetUdp ();
}


}//namespace ns3
