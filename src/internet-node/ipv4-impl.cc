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
#include "ipv4-impl.h"
#include "ipv4-l3-protocol.h"
#include "ipv4-interface.h"
#include "ns3/assert.h"
#include "ns3/net-device.h"

namespace ns3 {

Ipv4Impl::Ipv4Impl (Ptr<Ipv4L3Protocol> ipv4)
  : m_ipv4 (ipv4)
{}
Ipv4Impl::~Ipv4Impl ()
{
  NS_ASSERT (m_ipv4 == 0);
}
void 
Ipv4Impl::DoDispose (void)
{
  m_ipv4 = 0;
}

void
Ipv4Impl::AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol,
                              int priority)
{
  m_ipv4->AddRoutingProtocol (routingProtocol, priority);
}

void 
Ipv4Impl::AddHostRouteTo (Ipv4Address dest, 
			   Ipv4Address nextHop, 
			   uint32_t interface)
{
  m_ipv4->AddHostRouteTo (dest, nextHop, interface);
}
void 
Ipv4Impl::AddHostRouteTo (Ipv4Address dest, 
			   uint32_t interface)
{
  m_ipv4->AddHostRouteTo (dest, interface);
}
void 
Ipv4Impl::AddNetworkRouteTo (Ipv4Address network, 
			      Ipv4Mask networkMask, 
			      Ipv4Address nextHop, 
			      uint32_t interface)
{
  m_ipv4->AddNetworkRouteTo (network, networkMask, nextHop, interface);
}
void 
Ipv4Impl::AddNetworkRouteTo (Ipv4Address network, 
			      Ipv4Mask networkMask, 
			      uint32_t interface)
{
  m_ipv4->AddNetworkRouteTo (network, networkMask, interface);
}
void 
Ipv4Impl::SetDefaultRoute (Ipv4Address nextHop, 
			    uint32_t interface)
{
  m_ipv4->SetDefaultRoute (nextHop, interface);
}
uint32_t 
Ipv4Impl::GetNRoutes (void)
{
  return m_ipv4->GetNRoutes ();
}
Ipv4Route 
Ipv4Impl::GetRoute (uint32_t i)
{
  return *m_ipv4->GetRoute (i);
}
void 
Ipv4Impl::RemoveRoute (uint32_t i)
{
  return m_ipv4->RemoveRoute (i);
}
uint32_t 
Ipv4Impl::AddInterface (Ptr<NetDevice> device)
{
  return m_ipv4->AddInterface (device);
}
uint32_t 
Ipv4Impl::GetNInterfaces (void)
{
  return m_ipv4->GetNInterfaces ();
}
Ptr<NetDevice>
Ipv4Impl::GetNetDevice (uint32_t i)
{
  return m_ipv4->GetInterface (i)-> GetDevice ();
}

void 
Ipv4Impl::SetAddress (uint32_t i, Ipv4Address address)
{
  m_ipv4->SetAddress (i, address);
}
void 
Ipv4Impl::SetNetworkMask (uint32_t i, Ipv4Mask mask)
{
  m_ipv4->SetNetworkMask (i, mask);
}
Ipv4Mask 
Ipv4Impl::GetNetworkMask (uint32_t i) const
{
  return m_ipv4->GetNetworkMask (i);
}
Ipv4Address 
Ipv4Impl::GetAddress (uint32_t i) const
{
  return m_ipv4->GetAddress (i);
}
uint16_t 
Ipv4Impl::GetMtu (uint32_t i) const
{
  return m_ipv4->GetMtu (i);
}
bool 
Ipv4Impl::IsUp (uint32_t i) const
{
  return m_ipv4->IsUp (i);
}
void 
Ipv4Impl::SetUp (uint32_t i)
{
  m_ipv4->SetUp (i);
}
void 
Ipv4Impl::SetDown (uint32_t i)
{
  m_ipv4->SetDown (i);
}

}//namespace ns3
