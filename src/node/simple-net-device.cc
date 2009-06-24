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
#include "simple-net-device.h"
#include "simple-channel.h"
#include "node.h"
#include "ns3/packet.h"

namespace ns3 {

TypeId 
SimpleNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SimpleNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<SimpleNetDevice> ()
    ;
  return tid;
}

SimpleNetDevice::SimpleNetDevice ()
  : m_channel (0),
    m_node (0),
    m_mtu (0xffff),
    m_ifIndex (0)
{}

void 
SimpleNetDevice::Receive (Ptr<Packet> packet, uint16_t protocol, 
			  Mac48Address to, Mac48Address from)
{
  NetDevice::PacketType packetType;
  if (to == m_address)
    {
      packetType = NetDevice::PACKET_HOST;
    }
  else if (to.IsBroadcast ())
    {
      packetType = NetDevice::PACKET_HOST;
    }
  else if (to.IsGroup ())
    {
      packetType = NetDevice::PACKET_MULTICAST;
    }
  else 
    {
      packetType = NetDevice::PACKET_OTHERHOST;
    }
  m_rxCallback (this, packet, protocol, from);
  if (!m_promiscCallback.IsNull ())
    {
      m_promiscCallback (this, packet, protocol, from, to, packetType);
    }
}

void 
SimpleNetDevice::SetChannel (Ptr<SimpleChannel> channel)
{
  m_channel = channel;
  m_channel->Add (this);
}

void 
SimpleNetDevice::SetIfIndex(const uint32_t index)
{
  m_ifIndex = index;
}
uint32_t 
SimpleNetDevice::GetIfIndex(void) const
{
  return m_ifIndex;
}
Ptr<Channel> 
SimpleNetDevice::GetChannel (void) const
{
  return m_channel;
}
void
SimpleNetDevice::SetAddress (Address address)
{
  m_address = Mac48Address::ConvertFrom(address);
}
Address 
SimpleNetDevice::GetAddress (void) const
{
  //
  // Implicit conversion from Mac48Address to Address
  //
  return m_address;
}
bool 
SimpleNetDevice::SetMtu (const uint16_t mtu)
{
  m_mtu = mtu;
  return true;
}
uint16_t 
SimpleNetDevice::GetMtu (void) const
{
  return m_mtu;
}
bool 
SimpleNetDevice::IsLinkUp (void) const
{
  return true;
}
void 
SimpleNetDevice::SetLinkChangeCallback (Callback<void> callback)
{}
bool 
SimpleNetDevice::IsBroadcast (void) const
{
  return true;
}
Address
SimpleNetDevice::GetBroadcast (void) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}
bool 
SimpleNetDevice::IsMulticast (void) const
{
  return false;
}
Address 
SimpleNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  return Mac48Address::GetMulticast (multicastGroup);
}

Address SimpleNetDevice::GetMulticast (Ipv6Address addr) const
{
	return Mac48Address::GetMulticast (addr);
}

bool 
SimpleNetDevice::IsPointToPoint (void) const
{
  return false;
}

bool 
SimpleNetDevice::IsBridge (void) const
{
  return false;
}

bool 
SimpleNetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  Mac48Address to = Mac48Address::ConvertFrom (dest);
  m_channel->Send (packet, protocolNumber, to, m_address, this);
  return true;
}
bool 
SimpleNetDevice::SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  Mac48Address to = Mac48Address::ConvertFrom (dest);
  Mac48Address from = Mac48Address::ConvertFrom (source);
  m_channel->Send (packet, protocolNumber, to, from, this);
  return true;
}

Ptr<Node> 
SimpleNetDevice::GetNode (void) const
{
  return m_node;
}
void 
SimpleNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}
bool 
SimpleNetDevice::NeedsArp (void) const
{
  return false;
}
void 
SimpleNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}

void
SimpleNetDevice::DoDispose (void)
{
  m_channel = 0;
  m_node = 0;
  NetDevice::DoDispose ();
}


void
SimpleNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  m_promiscCallback = cb;
}

bool
SimpleNetDevice::SupportsSendFrom (void) const
{
  return true;
}

} // namespace ns3
