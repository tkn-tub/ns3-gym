/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
#include "host-tap-net-device.h"
#include "ns3/node.h"
#include "ns3/tap-channel.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("HostTapNetDevice");

namespace ns3 {

TypeId 
HostTapNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HostTapNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<HostTapNetDevice> ()
    ;
  return tid;
}

HostTapNetDevice::HostTapNetDevice ()
  : m_node (0),
    m_mtu (0xffff),
    m_name (""),
    m_ifIndex (0)
{
  NS_LOG_FUNCTION (this);
}

void 
HostTapNetDevice::SetChannel (Ptr<TapChannel> channel)
{
  m_channel = channel;
  m_channel->SetHostDevice (this);
}

void 
HostTapNetDevice::SetAddress (Mac48Address address)
{
  m_address = address;
}

Mac48Address 
HostTapNetDevice::GetMacAddress (void) const
{
  return m_address;
}

void 
HostTapNetDevice::SetName(const std::string name)
{
  m_name = name;
}
std::string 
HostTapNetDevice::GetName(void) const
{
  return m_name;
}
void 
HostTapNetDevice::SetIfIndex(const uint32_t index)
{
  m_ifIndex = index;
}
uint32_t 
HostTapNetDevice::GetIfIndex(void) const
{
  return m_ifIndex;
}
Ptr<Channel> 
HostTapNetDevice::GetChannel (void) const
{
  return m_channel;
}
Address 
HostTapNetDevice::GetAddress (void) const
{
  return m_address;
}
bool 
HostTapNetDevice::SetMtu (const uint16_t mtu)
{
  m_mtu = mtu;
  return true;
}
uint16_t 
HostTapNetDevice::GetMtu (void) const
{
  return m_mtu;
}
bool 
HostTapNetDevice::IsLinkUp (void) const
{
  return true;
}
void 
HostTapNetDevice::SetLinkChangeCallback (Callback<void> callback)
{}
bool 
HostTapNetDevice::IsBroadcast (void) const
{
  return true;
}
Address
HostTapNetDevice::GetBroadcast (void) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}
bool 
HostTapNetDevice::IsMulticast (void) const
{
  return true;
}
Address 
HostTapNetDevice::GetMulticast (void) const
{
  return Mac48Address::GetMulticastPrefix ();
}
Address 
HostTapNetDevice::MakeMulticastAddress (Ipv4Address multicastGroup) const
{
  return Mac48Address::GetMulticast (multicastGroup);
}
bool 
HostTapNetDevice::IsPointToPoint (void) const
{
  return false;
}
bool 
HostTapNetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  return false;
}
bool 
HostTapNetDevice::SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  return false;
}
Ptr<Node> 
HostTapNetDevice::GetNode (void) const
{
  return m_node;
}
void 
HostTapNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}
bool 
HostTapNetDevice::NeedsArp (void) const
{
  return false;
}
void 
HostTapNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{}

void
HostTapNetDevice::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_node = 0;
  m_channel = 0;
  NetDevice::DoDispose ();
}

void
HostTapNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{}

bool
HostTapNetDevice::SupportsSendFrom (void) const
{
  return false;
}

} // namespace ns3
