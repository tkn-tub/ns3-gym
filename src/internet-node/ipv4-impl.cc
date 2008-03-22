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

Ipv4Impl::Ipv4Impl ()
  : m_ipv4 (0)
{}
Ipv4Impl::~Ipv4Impl ()
{
  NS_ASSERT (m_ipv4 == 0);
}
void 
Ipv4Impl::SetIpv4 (Ptr<Ipv4L3Protocol> ipv4)
{
  m_ipv4 = ipv4;
}
void 
Ipv4Impl::DoDispose (void)
{
  m_ipv4 = 0;
}

void
Ipv4Impl::AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol,
                              int16_t priority)
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

void
Ipv4Impl::AddMulticastRoute (Ipv4Address origin,
                             Ipv4Address group,
                             uint32_t inputInterface,
                             std::vector<uint32_t> outputInterfaces)
{
  m_ipv4->AddMulticastRoute (origin, group, inputInterface, outputInterfaces);
}

void
Ipv4Impl::SetDefaultMulticastRoute (uint32_t outputInterface)
{
  m_ipv4->SetDefaultMulticastRoute (outputInterface);
}

uint32_t 
Ipv4Impl::GetNMulticastRoutes (void) const
{
  return m_ipv4->GetNMulticastRoutes ();
}

Ipv4MulticastRoute 
Ipv4Impl::GetMulticastRoute (uint32_t i) const
{
  return *m_ipv4->GetMulticastRoute (i);
}

void
Ipv4Impl::RemoveMulticastRoute (Ipv4Address origin,
                                Ipv4Address group,
                                uint32_t inputInterface)
{
  m_ipv4->RemoveMulticastRoute (origin, group, inputInterface);
}

void 
Ipv4Impl::RemoveMulticastRoute (uint32_t i)
{
  return m_ipv4->RemoveMulticastRoute (i);
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

uint32_t 
Ipv4Impl::FindInterfaceForAddr (Ipv4Address addr) const
{
  return m_ipv4->FindInterfaceForAddr (addr);
}

uint32_t 
Ipv4Impl::FindInterfaceForAddr (Ipv4Address addr, Ipv4Mask mask) const
{
  return m_ipv4->FindInterfaceForAddr (addr, mask);
}

int32_t 
Ipv4Impl::FindInterfaceForDevice (Ptr<NetDevice> device) const
{
  return m_ipv4->FindInterfaceIndexForDevice (device);
}

Ptr<NetDevice>
Ipv4Impl::GetNetDevice (uint32_t i)
{
  return m_ipv4->GetInterface (i)-> GetDevice ();
}

void 
Ipv4Impl::JoinMulticastGroup (Ipv4Address origin, Ipv4Address group)
{
  m_ipv4->JoinMulticastGroup(origin, group);
}

void
Ipv4Impl::LeaveMulticastGroup (Ipv4Address origin, Ipv4Address group)
{
  m_ipv4->LeaveMulticastGroup(origin, group);
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

void
Ipv4Impl::SetMetric (uint32_t i, uint16_t metric) 
{
  m_ipv4->SetMetric (i, metric);
}

uint16_t
Ipv4Impl::GetMetric (uint32_t i) const
{
  return m_ipv4->GetMetric (i);
}

bool
Ipv4Impl::GetIfIndexForDestination (Ipv4Address dest, uint32_t &ifIndex) const
{
  return m_ipv4->GetIfIndexForDestination (dest, ifIndex);
}

Ipv4Address 
Ipv4Impl::GetSourceAddress (Ipv4Address destination) const
{
  uint32_t ifIndex = 0xffffffff;

  bool result = m_ipv4->GetIfIndexForDestination (destination, ifIndex);

  if (result)
    {
      return m_ipv4->GetAddress (ifIndex);
    }
  else
    {
//
// If we can't find any address, just leave it 0.0.0.0
//
      return Ipv4Address::GetAny ();
    }
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
