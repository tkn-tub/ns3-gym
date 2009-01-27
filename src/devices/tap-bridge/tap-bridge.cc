/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#include "tap-bridge.h"
#include "ns3/node.h"
#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/boolean.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("TapBridge");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TapBridge);

TypeId
TapBridge::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TapBridge")
    .SetParent<NetDevice> ()
    .AddConstructor<TapBridge> ()
    ;
  return tid;
}


TapBridge::TapBridge ()
  : m_node (0),
    m_ifIndex (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

TapBridge::~TapBridge()
{
  NS_LOG_FUNCTION_NOARGS ();
}

  void 
TapBridge::DoDispose ()
{
  NS_LOG_FUNCTION_NOARGS ();
  NetDevice::DoDispose ();
}

void 
TapBridge::SetBridgedDevice (Ptr<NetDevice> bridgedDevice)
{
  NS_LOG_FUNCTION_NOARGS ();

  NS_ASSERT_MSG (m_node != 0, "TapBridge::SetBridgedDevice:  Bridge not installed in a node");
  NS_ASSERT_MSG (bridgedDevice != this, "TapBridge::SetBridgedDevice:  Cannot bridge to self");
  NS_ASSERT_MSG (m_bridgedDevice == 0, "TapBridge::SetBridgedDevice:  Already bridged");
  
  if (!Mac48Address::IsMatchingType (bridgedDevice->GetAddress ()))
    {
      NS_FATAL_ERROR ("TapBridge::SetBridgedDevice: Device does not support eui 48 addresses: cannot be added to bridge.");
    }
  
  if (!bridgedDevice->SupportsSendFrom ())
    {
      NS_FATAL_ERROR ("TapBridge::SetBridgedDevice: Device does not support SendFrom: cannot be added to bridge.");
    }

  //
  // Tell the bridged device to forward its received packets here.
  //
  m_node->RegisterProtocolHandler (MakeCallback (&TapBridge::ReceiveFromBridgedDevice, this), 0, bridgedDevice, true);
  m_bridgedDevice = bridgedDevice;
}

void
TapBridge::ReceiveFromBridgedDevice (
  Ptr<NetDevice> device, 
  Ptr<const Packet> packet, 
  uint16_t protocol,
  Address const &src, 
  Address const &dst, 
  PacketType packetType)
{
  NS_LOG_FUNCTION (device << packet << protocol << src << dst << packetType);
  NS_ASSERT_MSG (device == m_bridgedDevice, "TapBridge::SetBridgedDevice:  Received packet from unexpected device");
  
  NS_LOG_DEBUG ("Packet UID is " << packet->GetUid ());

  Mac48Address src48 = Mac48Address::ConvertFrom (src);
  Mac48Address dst48 = Mac48Address::ConvertFrom (dst);

  //
  // We have received a packet from the ns-3 net device that has been associated
  // with this bridge.  We want to take these bits and send them off to the 
  // Tap device on the Linux host.
  //
  NS_LOG_LOGIC ("TapBridge::ReceiveFromDevice: Not implemented");
}

void 
TapBridge::SetName(const std::string name)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_name = name;
}

std::string 
TapBridge::GetName(void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_name;
}

void 
TapBridge::SetIfIndex(const uint32_t index)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_ifIndex = index;
}

uint32_t 
TapBridge::GetIfIndex(void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_ifIndex;
}

Ptr<Channel> 
TapBridge::GetChannel (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return 0;
}

Address 
TapBridge::GetAddress (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_address;
}

bool 
TapBridge::SetMtu (const uint16_t mtu)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_mtu = mtu;
  return true;
}

uint16_t 
TapBridge::GetMtu (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_mtu;
}


bool 
TapBridge::IsLinkUp (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

void 
TapBridge::SetLinkChangeCallback (Callback<void> callback)
{
  NS_LOG_FUNCTION_NOARGS ();
}

bool 
TapBridge::IsBroadcast (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

Address
TapBridge::GetBroadcast (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

bool
TapBridge::IsMulticast (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

Address
TapBridge::GetMulticast (Ipv4Address multicastGroup) const
{
 NS_LOG_FUNCTION (this << multicastGroup);
 Mac48Address multicast = Mac48Address::GetMulticast (multicastGroup);
 return multicast;
}

bool 
TapBridge::IsPointToPoint (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return false;
}

bool 
TapBridge::IsBridge (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

bool 
TapBridge::Send (Ptr<Packet> packet, const Address& dst, uint16_t protocol)
{
  NS_LOG_FUNCTION (packet << dst << protocol);
  NS_FATAL_ERROR ("TapBridge::Send: You may not call Send on a TapBridge directly");
  return false;
}

bool 
TapBridge::SendFrom (Ptr<Packet> packet, const Address& src, const Address& dst, uint16_t protocol)
{
  NS_LOG_FUNCTION (packet << src << dst << protocol);
  NS_FATAL_ERROR ("TapBridge::Send: You may not call SendFrom on a TapBridge directly");
  return false;
}

Ptr<Node> 
TapBridge::GetNode (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_node;
}

void 
TapBridge::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = node;
}

bool 
TapBridge::NeedsArp (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

void 
TapBridge::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_rxCallback = cb;
}

void 
TapBridge::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_promiscRxCallback = cb;
}

bool
TapBridge::SupportsSendFrom () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

Address TapBridge::GetMulticast (Ipv6Address addr) const
{
  NS_LOG_FUNCTION (this << addr);
  return Mac48Address::GetMulticast (addr);
}

} // namespace ns3
