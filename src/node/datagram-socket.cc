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
#include "datagram-socket.h"
#include "udp.h"
#include "udp-end-point.h"
#include "node.h"
#include "ipv4-l4-demux.h"

namespace ns3 {

DatagramSocket::DatagramSocket (Node *node)
  : m_endPoint (0),
    m_node (node)
{
  NS_ASSERT (GetUdp () != 0);
}
DatagramSocket::~DatagramSocket ()
{
  delete m_endPoint;
}

int
DatagramSocket::Bind (void)
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
DatagramSocket::Bind (Ipv4Address address)
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
DatagramSocket::Bind (uint16_t port)
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
DatagramSocket::Bind (Ipv4Address address, uint16_t port)
{
  m_endPoint = GetUdp ()->Allocate (address, port);
  if (m_endPoint == 0)
    {
      return -1;
    }
  m_endPoint->SetSocket (this);
  return 0;
}

void 
DatagramSocket::SetDefaultDestination (Ipv4Address daddr, uint16_t dport)
{
  m_defaultAddress = daddr;
  m_defaultPort = dport;
}
void 
DatagramSocket::SendDummy (uint32_t size)
{
  SendDummyTo (size, m_defaultAddress, m_defaultPort);
}
void 
DatagramSocket::SendDummyTo (uint32_t size, Ipv4Address daddr, uint16_t dport)
{
  if (m_endPoint == 0)
    {
      Bind ();
      if (m_endPoint == 0)
	{
	  return;
	}
    }
  Packet p = Packet (size);
  GetUdp ()->Send (p, m_endPoint->GetLocalAddress (), daddr,
		   m_endPoint->GetLocalPort (), dport);
}

void 
DatagramSocket::Send (uint8_t const*buffer, uint32_t size)
{
  SendTo (buffer, size, m_defaultAddress, m_defaultPort);
}
void 
DatagramSocket::SendTo (uint8_t const*buffer, uint32_t size,
		   Ipv4Address daddr, uint16_t dport)
{
  if (m_endPoint == 0)
    {
      Bind ();
      if (m_endPoint == 0)
	{
	  return;
	}
    }
  Packet p = Packet (buffer, size);
  GetUdp ()->Send (p, m_endPoint->GetLocalAddress (), daddr,
		   m_endPoint->GetLocalPort (), dport);
}
void 
DatagramSocket::SetDummyRxCallback (Callback<void,DatagramSocket*,
			       uint32_t,
			       Ipv4Address,uint16_t> cb)
{
  m_dummyRxCallback = cb;
}
void 
DatagramSocket::SetRxCallback (Callback<void,DatagramSocket*,
			  uint8_t const*,uint32_t,
			  Ipv4Address,uint16_t> cb)
{
  m_rxCallback = cb;
}

void 
DatagramSocket::ForwardUp (Packet &p, Ipv4Address saddr, uint16_t sport)
{
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
DatagramSocket::GetUdp (void) const
{
  return m_node->GetUdp ();
}

Node *
DatagramSocket::GetNode (void) const
{
  return m_node;
}

}//namespace ns3
