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
#include "i-ipv4-private.h"
#include "ipv4.h"
#include "ns3/assert.h"
#include "ns3/iid-manager.h"

namespace ns3 {

const uint32_t IIpv4Private::iid = IidManager::Allocate ("IIpv4Private");

IIpv4Private::IIpv4Private (Ipv4 *ipv4)
  : NsUnknown (IIpv4Private::iid),
    m_ipv4 (ipv4)
{
  m_ipv4->Ref ();
}
IIpv4Private::~IIpv4Private ()
{
  NS_ASSERT (m_ipv4 == 0);
}
TraceResolver *
IIpv4Private::CreateTraceResolver (TraceContext const &context)
{
  return m_ipv4->CreateTraceResolver (context);
}
void 
IIpv4Private::Send (Packet const &packet, Ipv4Address source, 
		    Ipv4Address destination, uint8_t protocol)
{
  m_ipv4->Send (packet, source, destination, protocol);
}
Ipv4Interface *
IIpv4Private::FindInterfaceForDevice (NetDevice const*device)
{
  return m_ipv4->FindInterfaceForDevice (device);
}
void 
IIpv4Private::Receive(Packet& p, NetDevice *device)
{
  m_ipv4->Receive (p, device);
}
void 
IIpv4Private::DoDispose (void)
{
  m_ipv4->Unref ();
  m_ipv4 = 0;
  NsUnknown::DoDispose ();
}

} // namespace ns3
