/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#include "ipv4-end-point.h"
#include "ns3/packet.h"

namespace ns3 {

Ipv4EndPoint::Ipv4EndPoint (Ipv4Address address, uint16_t port, Ipv4Address localInterface)
  : m_localAddr (address), 
    m_localPort (port),
    m_peerAddr (Ipv4Address::GetAny ()),
    m_peerPort (0),
    m_localInterface (localInterface)
{}
Ipv4EndPoint::~Ipv4EndPoint ()
{
  if (!m_destroyCallback.IsNull ())
    {
      m_destroyCallback ();
    }
}

Ipv4Address 
Ipv4EndPoint::GetLocalAddress (void)
{
  return m_localAddr;
}

void 
Ipv4EndPoint::SetLocalAddress (Ipv4Address address)
{
  m_localAddr = address;
}

Ipv4Address Ipv4EndPoint::GetLocalInterface (void) const
{
  return m_localInterface;
}

void
Ipv4EndPoint::SetLocalInterface (Ipv4Address localInterface)
{
  m_localInterface = localInterface;
}

uint16_t 
Ipv4EndPoint::GetLocalPort (void)
{
  return m_localPort;
}
Ipv4Address 
Ipv4EndPoint::GetPeerAddress (void)
{
  return m_peerAddr;
}
uint16_t 
Ipv4EndPoint::GetPeerPort (void)
{
  return m_peerPort;
}
void 
Ipv4EndPoint::SetPeer (Ipv4Address address, uint16_t port)
{
  m_peerAddr = address;
  m_peerPort = port;
}

void 
Ipv4EndPoint::SetRxCallback (Callback<void,Ptr<Packet>, Ipv4Address, uint16_t> callback)
{
  m_rxCallback = callback;
}

void 
Ipv4EndPoint::SetDestroyCallback (Callback<void> callback)
{
  m_destroyCallback = callback;
}

void 
Ipv4EndPoint::ForwardUp (Ptr<Packet> p, Ipv4Address saddr, uint16_t sport)
{
  if (!m_rxCallback.IsNull ())
  {
    m_rxCallback (p, saddr, sport);
  }
}



}; // namespace ns3
