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
#include "ipv4-private.h"
#include "ipv4-l3-protocol.h"
#include "ns3/assert.h"
#include "ns3/net-device.h"

namespace ns3 {

const InterfaceId Ipv4Private::iid = MakeInterfaceId ("Ipv4Private", Object::iid);

Ipv4Private::Ipv4Private (Ptr<Ipv4L3Protocol> ipv4)
  : m_ipv4 (ipv4)
{
  SetInterfaceId (Ipv4Private::iid);
}
Ipv4Private::~Ipv4Private ()
{
  NS_ASSERT (m_ipv4 == 0);
}
TraceResolver *
Ipv4Private::CreateTraceResolver (TraceContext const &context)
{
  return m_ipv4->CreateTraceResolver (context);
}
void 
Ipv4Private::Send (Packet const &packet, Ipv4Address source, 
		    Ipv4Address destination, uint8_t protocol)
{
  m_ipv4->Send (packet, source, destination, protocol);
}
Ipv4Interface *
Ipv4Private::FindInterfaceForDevice (Ptr<const NetDevice>device)
{
  return m_ipv4->FindInterfaceForDevice (device);
}
void 
Ipv4Private::Receive(Packet& p, Ptr<NetDevice> device)
{
  m_ipv4->Receive (p, device);
}
void 
Ipv4Private::DoDispose (void)
{
  m_ipv4 = 0;
  Object::DoDispose ();
}

} // namespace ns3
