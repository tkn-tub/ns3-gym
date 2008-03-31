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
    m_name (""),
    m_ifIndex (0)
{}

void 
SimpleNetDevice::Receive (Ptr<Packet> packet, uint16_t protocol, 
			  Mac48Address to, Mac48Address from)
{
  if (to == m_address || to == Mac48Address::GetBroadcast ())
    {
      m_rxCallback (this, packet, protocol, from);
    }
}

void 
SimpleNetDevice::SetChannel (Ptr<SimpleChannel> channel)
{
  m_channel = channel;
  m_channel->Add (this);
}

void 
SimpleNetDevice::SetAddress (Mac48Address address)
{
  m_address = address;
}

void 
SimpleNetDevice::SetName(const std::string name)
{
  m_name = name;
}
std::string 
SimpleNetDevice::GetName(void) const
{
  return m_name;
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
Address 
SimpleNetDevice::GetAddress (void) const
{
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
SimpleNetDevice::GetMulticast (void) const
{
  return Mac48Address ("01:00:5e:00:00:00");
}
Address 
SimpleNetDevice::MakeMulticastAddress (Ipv4Address multicastGroup) const
{
  return Mac48Address ("01:00:5e:00:00:00");
}
bool 
SimpleNetDevice::IsPointToPoint (void) const
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



} // namespace ns3
