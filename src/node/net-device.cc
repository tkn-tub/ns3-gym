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
#include "ns3/debug.h"


#include "channel.h"
#include "net-device.h"
#include "node.h"

NS_DEBUG_COMPONENT_DEFINE ("NetDevice");

namespace ns3 {

const InterfaceId NetDevice::iid = MakeInterfaceId ("NetDevice", Object::iid);

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
  SetInterfaceId (NetDevice::iid);
  m_node->AddDevice (this);
}

NetDevice::~NetDevice ()
{}

Address 
NetDevice::GetAddress (void) const
{
  return m_address;
}

bool
NetDevice::SetMtu (const uint16_t mtu) 
{
  m_mtu = mtu;
  return true;
}

uint16_t 
NetDevice::GetMtu (void) const
{
  return m_mtu;
}

void
NetDevice::SetName(const std::string name) 
{ 
  m_name = name; 
}

std::string 
NetDevice::GetName(void) const 
{ 
  return m_name; 
}

void
NetDevice::SetIfIndex(uint32_t index) 
{ 
  m_ifIndex = index; 
}

uint32_t
NetDevice::GetIfIndex(void) const 
{ 
  return m_ifIndex; 
}

bool 
NetDevice::IsLinkUp (void) const
{
  return m_isUp;
}

void 
NetDevice::SetLinkChangeCallback (Callback<void> callback)
{
  m_linkChangeCallback = callback;
}

bool
NetDevice::IsBroadcast (void) const
{
  return m_isBroadcast;
}

Address const &
NetDevice::GetBroadcast (void) const
{
  NS_ASSERT (m_isBroadcast);
  return m_broadcast;
}

void
NetDevice::EnableBroadcast (Address broadcast)
{
  m_isBroadcast = true;
  m_broadcast = broadcast;
}

void
NetDevice::DisableBroadcast (void)
{
  m_isBroadcast = false;
}

bool
NetDevice::IsMulticast (void) const
{
  return m_isMulticast;
}

Address 
NetDevice::GetMulticast (void) const
{
  NS_ASSERT_MSG (m_isMulticast, "NetDevice::GetMulticast (): "
    "Invalid operation when not IsMulticast ()");
  return m_multicast;
}

Address
NetDevice::MakeMulticastAddress(Ipv4Address multicastGroup) const
{
  NS_ASSERT_MSG (m_isMulticast, "NetDevice::GetMulticast (): "
    "Invalid operation when not IsMulticast ()");
  return m_multicast;
}

void
NetDevice::EnableMulticast (Address multicast)
{
  m_isMulticast = true;
  m_multicast = multicast;
}

void
NetDevice::DisableMulticast (void)
{
  m_isMulticast = false;
}

bool
NetDevice::IsPointToPoint (void) const
{
  return m_isPointToPoint;
}

void
NetDevice::EnablePointToPoint (void)
{
  m_isPointToPoint = true;
}

void
NetDevice::DisablePointToPoint (void)
{
  m_isPointToPoint = false;
}

// Receive packet from above
bool 
NetDevice::Send(const Packet& p, const Address& dest, uint16_t protocolNumber)
{
  if (m_isUp)
    {
      return SendTo(p, dest, protocolNumber);
    }
  else
    {
      return false;
    }
}

TraceResolver *
NetDevice::CreateTraceResolver (TraceContext const &context)
{
  return DoCreateTraceResolver (context);
}

Ptr<Channel>
NetDevice::GetChannel (void) const
{
  return DoGetChannel ();
}

// Receive packets from below
bool
NetDevice::ForwardUp(const Packet& p, uint16_t param, const Address &from)
{
  bool retval = false;

  NS_DEBUG ("NetDevice::ForwardUp: UID is " << p.GetUid()
            << " device is: " << GetName());
  
  if (!m_receiveCallback.IsNull ())
    {
      retval = m_receiveCallback (this, p, param, from);
    } 
  else 
    {
      NS_DEBUG ("NetDevice::Receive call back is NULL");
    }

    return retval;
}

void 
NetDevice::NotifyLinkUp (void)
{
  m_isUp = true;
  if (!m_linkChangeCallback.IsNull ())
    {
      m_linkChangeCallback ();
    }
}

void 
NetDevice::NotifyLinkDown (void)
{
  m_isUp = false;
  if (!m_linkChangeCallback.IsNull ())
    {
      m_linkChangeCallback ();
    }
}

Ptr<Node>
NetDevice::GetNode (void) const
{
  return m_node;
}

bool
NetDevice::NeedsArp (void) const
{
  return DoNeedsArp ();
}

void 
NetDevice::SetReceiveCallback (ReceiveCallback cb)
{
  m_receiveCallback = cb;
}

void
NetDevice::DoDispose()
{
  m_node = 0;
}

}; // namespace ns3
