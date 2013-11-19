/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Universita' di Firenze, Italy
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
 * Author: Tommaso Pecorella <tommaso.pecorella@unifi.it>
 */
#include "error-net-device.h"
#include "error-channel.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/error-model.h"
#include "ns3/trace-source-accessor.h"

NS_LOG_COMPONENT_DEFINE ("ErrorNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ErrorNetDevice)
  ;

TypeId 
ErrorNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ErrorNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<ErrorNetDevice> ()
    .AddAttribute ("ReceiveErrorModel",
                   "The receiver error model used to simulate packet loss",
                   PointerValue (),
                   MakePointerAccessor (&ErrorNetDevice::m_receiveErrorModel),
                   MakePointerChecker<ErrorModel> ())
    .AddTraceSource ("PhyRxDrop",
                     "Trace source indicating a packet has been dropped by the device during reception",
                     MakeTraceSourceAccessor (&ErrorNetDevice::m_phyRxDropTrace))
    ;
  return tid;
}

ErrorNetDevice::ErrorNetDevice ()
  : m_channel (0),
    m_node (0),
    m_mtu (0xffff),
    m_ifIndex (0)
{}

void 
ErrorNetDevice::Receive (Ptr<Packet> packet, uint16_t protocol,
			  Mac48Address to, Mac48Address from)
{
  NS_LOG_FUNCTION (packet << protocol << to << from << *packet);
  NetDevice::PacketType packetType;

  if (m_receiveErrorModel && m_receiveErrorModel->IsCorrupt (packet) )
    {
      m_phyRxDropTrace (packet);
      return;
    }

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
ErrorNetDevice::SetChannel (Ptr<ErrorChannel> channel)
{
  m_channel = channel;
  m_channel->Add (this);
}

void
ErrorNetDevice::SetReceiveErrorModel (Ptr<ErrorModel> em)
{
  m_receiveErrorModel = em;
}

void 
ErrorNetDevice::SetIfIndex(const uint32_t index)
{
  m_ifIndex = index;
}
uint32_t 
ErrorNetDevice::GetIfIndex(void) const
{
  return m_ifIndex;
}
Ptr<Channel> 
ErrorNetDevice::GetChannel (void) const
{
  return m_channel;
}
void
ErrorNetDevice::SetAddress (Address address)
{
  m_address = Mac48Address::ConvertFrom(address);
}
Address 
ErrorNetDevice::GetAddress (void) const
{
  //
  // Implicit conversion from Mac48Address to Address
  //
  return m_address;
}
bool 
ErrorNetDevice::SetMtu (const uint16_t mtu)
{
  m_mtu = mtu;
  return true;
}
uint16_t 
ErrorNetDevice::GetMtu (void) const
{
  return m_mtu;
}
bool 
ErrorNetDevice::IsLinkUp (void) const
{
  return true;
}
void 
ErrorNetDevice::AddLinkChangeCallback (Callback<void> callback)
{}
bool 
ErrorNetDevice::IsBroadcast (void) const
{
  return true;
}
Address
ErrorNetDevice::GetBroadcast (void) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}
bool 
ErrorNetDevice::IsMulticast (void) const
{
  return false;
}
Address 
ErrorNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  return Mac48Address::GetMulticast (multicastGroup);
}

Address ErrorNetDevice::GetMulticast (Ipv6Address addr) const
{
  return Mac48Address::GetMulticast (addr);
}

bool 
ErrorNetDevice::IsPointToPoint (void) const
{
  return false;
}

bool 
ErrorNetDevice::IsBridge (void) const
{
  return false;
}

bool 
ErrorNetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << dest << protocolNumber << *packet);
  Mac48Address to = Mac48Address::ConvertFrom (dest);
  m_channel->Send (packet, protocolNumber, to, m_address, this);
  return true;
}
bool 
ErrorNetDevice::SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << source << dest << protocolNumber << *packet);
  Mac48Address to = Mac48Address::ConvertFrom (dest);
  Mac48Address from = Mac48Address::ConvertFrom (source);
  m_channel->Send (packet, protocolNumber, to, from, this);
  return true;
}

Ptr<Node> 
ErrorNetDevice::GetNode (void) const
{
  return m_node;
}
void 
ErrorNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}
bool 
ErrorNetDevice::NeedsArp (void) const
{
  return false;
}
void 
ErrorNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}

void
ErrorNetDevice::DoDispose (void)
{
  m_channel = 0;
  m_node = 0;
  m_receiveErrorModel = 0;
  NetDevice::DoDispose ();
}


void
ErrorNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  m_promiscCallback = cb;
}

bool
ErrorNetDevice::SupportsSendFrom (void) const
{
  return true;
}

} // namespace ns3
