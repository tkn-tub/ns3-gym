/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "udp-impl.h"
#include "udp-l4-protocol.h"
#include "ns3/socket.h"
#include "ns3/assert.h"

namespace ns3 {

UdpImpl::UdpImpl ()
  : m_udp (0)
{}
UdpImpl::~UdpImpl ()
{
  NS_ASSERT (m_udp == 0);
}

void 
UdpImpl::SetUdp (Ptr<UdpL4Protocol> udp)
{
  m_udp = udp;
}

Ptr<Socket>
UdpImpl::CreateSocket (void)
{
  return m_udp->CreateSocket ();
}

void 
UdpImpl::DoDispose (void)
{
  m_udp = 0;
  Udp::DoDispose ();
}

} // namespace ns3
