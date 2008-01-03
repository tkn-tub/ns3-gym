/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

#include <iostream>
#include "ns3/assert.h"
#include "ns3/object.h"
#include "ns3/log.h"
#include "ns3/trace-resolver.h"
#include "ns3/packet.h"
#include "channel.h"
#include "net-device.h"
#include "node.h"

NS_LOG_COMPONENT_DEFINE ("NetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (NetDevice);

InterfaceId NetDevice::iid (void)
{
  static InterfaceId iid = InterfaceId ("NetDevice")
    .SetParent<Object> ();
  return iid;
}

NetDevice::NetDevice(Ptr<Node> node, const Address& addr) : 
  m_node (node), 
  m_name(""), 
  m_ifIndex (0), 
  m_address (addr),
  m_mtu (0xffff), 
  m_isUp (false), 
  m_isBroadcast (false), 
  m_isMulticast (false), 
  m_isPointToPoint (false)
{
  NS_LOG_FUNCTION;
  m_node->AddDevice (this);
}

NetDevice::~NetDevice ()
{
  NS_LOG_FUNCTION;
}

Address 
NetDevice::GetAddress (void) const
{
  NS_LOG_FUNCTION;
  return m_address;
}

bool
NetDevice::SetMtu (const uint16_t mtu) 
{
  NS_LOG_FUNCTION;
  m_mtu = mtu;
  return true;
}

uint16_t 
NetDevice::GetMtu (void) const
{
  NS_LOG_FUNCTION;
  return m_mtu;
}

void
NetDevice::SetName(const std::string name) 
{ 
  NS_LOG_FUNCTION;
  m_name = name; 
}

std::string 
NetDevice::GetName(void) const 
{ 
  NS_LOG_FUNCTION;
  return m_name; 
}

void
NetDevice::SetIfIndex(uint32_t index) 
{ 
  NS_LOG_FUNCTION;
  m_ifIndex = index; 
}

uint32_t
NetDevice::GetIfIndex(void) const 
{ 
  NS_LOG_FUNCTION;
  return m_ifIndex; 
}

bool 
NetDevice::IsLinkUp (void) const
{
  NS_LOG_FUNCTION;
  return m_isUp;
}

void 
NetDevice::SetLinkChangeCallback (Callback<void> callback)
{
  NS_LOG_FUNCTION;
  m_linkChangeCallback = callback;
}

bool
NetDevice::IsBroadcast (void) const
{
  NS_LOG_FUNCTION;
  return m_isBroadcast;
}

Address const &
NetDevice::GetBroadcast (void) const
{
  NS_LOG_FUNCTION;
  NS_ASSERT (m_isBroadcast);
  return m_broadcast;
}

void
NetDevice::EnableBroadcast (Address broadcast)
{
  NS_LOG_FUNCTION;
  m_isBroadcast = true;
  m_broadcast = broadcast;
}

void
NetDevice::DisableBroadcast (void)
{
  NS_LOG_FUNCTION;
  m_isBroadcast = false;
}

bool
NetDevice::IsMulticast (void) const
{
  NS_LOG_FUNCTION;
  return m_isMulticast;
}

Address 
NetDevice::GetMulticast (void) const
{
  NS_LOG_FUNCTION;
  NS_ASSERT_MSG (m_isMulticast, "NetDevice::GetMulticast (): "
    "Invalid operation when not IsMulticast ()");
  return m_multicast;
}

Address
NetDevice::MakeMulticastAddress(Ipv4Address multicastGroup) const
{
  NS_LOG_FUNCTION;
  NS_ASSERT_MSG (m_isMulticast, "NetDevice::GetMulticast (): "
    "Invalid operation when not IsMulticast ()");
  return m_multicast;
}

void
NetDevice::EnableMulticast (Address multicast)
{
  NS_LOG_FUNCTION;
  m_isMulticast = true;
  m_multicast = multicast;
}

void
NetDevice::DisableMulticast (void)
{
  NS_LOG_FUNCTION;
  m_isMulticast = false;
}

bool
NetDevice::IsPointToPoint (void) const
{
  NS_LOG_FUNCTION;
  return m_isPointToPoint;
}

void
NetDevice::EnablePointToPoint (void)
{
  NS_LOG_FUNCTION;
  m_isPointToPoint = true;
}

void
NetDevice::DisablePointToPoint (void)
{
  NS_LOG_FUNCTION;
  m_isPointToPoint = false;
}

// Receive packet from above
bool 
NetDevice::Send(Ptr<Packet> p, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION;
  if (m_isUp)
    {
      return SendTo(p, dest, protocolNumber);
    }
  else
    {
      return false;
    }
}

Ptr<Channel>
NetDevice::GetChannel (void) const
{
  NS_LOG_FUNCTION;
  return DoGetChannel ();
}

// Receive packets from below
bool
NetDevice::ForwardUp(Ptr<Packet> p, uint16_t param, const Address &from)
{
  NS_LOG_FUNCTION;
  bool retval = false;

  NS_LOG_LOGIC ("UID is " << p->GetUid() << " device is: " << GetName());
  
  if (!m_receiveCallback.IsNull ())
    {
      retval = m_receiveCallback (this, p, param, from);
    } 
  else 
    {
      NS_LOG_WARN ("NetDevice::Receive call back is NULL");
    }

    return retval;
}

void 
NetDevice::NotifyLinkUp (void)
{
  NS_LOG_FUNCTION;
  m_isUp = true;
  if (!m_linkChangeCallback.IsNull ())
    {
      m_linkChangeCallback ();
    }
}

void 
NetDevice::NotifyLinkDown (void)
{
  NS_LOG_FUNCTION;
  m_isUp = false;
  if (!m_linkChangeCallback.IsNull ())
    {
      m_linkChangeCallback ();
    }
}

Ptr<Node>
NetDevice::GetNode (void) const
{
  NS_LOG_FUNCTION;
  return m_node;
}

bool
NetDevice::NeedsArp (void) const
{
  NS_LOG_FUNCTION;
  return DoNeedsArp ();
}

void 
NetDevice::SetReceiveCallback (ReceiveCallback cb)
{
  NS_LOG_FUNCTION;
  m_receiveCallback = cb;
}

void
NetDevice::DoDispose()
{
  NS_LOG_FUNCTION;
  m_node = 0;
}

}; // namespace ns3
