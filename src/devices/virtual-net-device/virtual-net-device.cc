/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 INESC Porto
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
 * Author:  Gustavo J. A. M. Carneiro  <gjc@inescporto.pt>
 */

#include "ns3/log.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/mac48-address.h"
#include "ns3/llc-snap-header.h"
#include "ns3/error-model.h"
#include "virtual-net-device.h"
#include "ns3/channel.h"
#include "ns3/trace-source-accessor.h"


NS_LOG_COMPONENT_DEFINE ("VirtualNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (VirtualNetDevice);

TypeId
VirtualNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::VirtualNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<VirtualNetDevice> ()
    .AddTraceSource ("Rx", "Received payload from the MAC layer.",
                     MakeTraceSourceAccessor (&VirtualNetDevice::m_rxTrace))
    .AddTraceSource ("Tx", "Send payload to the MAC layer.",
                     MakeTraceSourceAccessor (&VirtualNetDevice::m_txTrace))
    ;
  return tid;
}

VirtualNetDevice::VirtualNetDevice ()
{
  m_needsArp = false;
  m_supportsSendFrom = true;
  m_mtu = 65535;
  m_isPointToPoint = true;
}


void
VirtualNetDevice::SetSendFromCallback (SendFromCallback sendCb)
{
  m_sendCb = sendCb;
}

void
VirtualNetDevice::SetNeedsArp (bool needsArp)
{
  m_needsArp = needsArp;
}

void
VirtualNetDevice::SetSupportsSendFrom (bool supportsSendFrom)
{
  m_supportsSendFrom = supportsSendFrom;
}

void
VirtualNetDevice::SetIsPointToPoint (bool isPointToPoint)
{
  m_isPointToPoint = isPointToPoint;
}

bool
VirtualNetDevice::SetMtu (const uint16_t mtu)
{
  m_mtu = mtu;
  return true;
}


VirtualNetDevice::~VirtualNetDevice()
{
  NS_LOG_FUNCTION_NOARGS ();
}


void VirtualNetDevice::DoDispose()
{
  NS_LOG_FUNCTION_NOARGS ();
  NetDevice::DoDispose ();
}

bool
VirtualNetDevice::Receive (Ptr<Packet> packet, uint16_t protocol, const Address &address)
{
  if (m_rxCallback (this, packet, protocol, address))
    {
      m_rxTrace (packet);
      return true;
    }
  return false;
}

bool
VirtualNetDevice::PromiscReceive (Ptr<Packet> packet, uint16_t protocol,
                              const Address &source, const Address &destination,
                              PacketType packetType)
{
  if (m_promiscRxCallback (this, packet, protocol, source, destination, packetType))
    {
      m_rxTrace (packet);
      return true;
    }
  return false;
}


void
VirtualNetDevice::SetIfIndex (const uint32_t index)
{
  m_index = index;
}

uint32_t
VirtualNetDevice::GetIfIndex (void) const
{
  return m_index;
}

Ptr<Channel>
VirtualNetDevice::GetChannel (void) const
{
  return Ptr<Channel> ();
}

Address
VirtualNetDevice::GetAddress (void) const
{
  return Mac48Address ();
}

uint16_t
VirtualNetDevice::GetMtu (void) const
{
  return m_mtu;
}

bool
VirtualNetDevice::IsLinkUp (void) const
{
  return true;
}

void
VirtualNetDevice::SetLinkChangeCallback (Callback<void> callback)
{
}

bool
VirtualNetDevice::IsBroadcast (void) const
{
  return true;
}

Address
VirtualNetDevice::GetBroadcast (void) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

bool
VirtualNetDevice::IsMulticast (void) const
{
  return false;
}

Address VirtualNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

Address VirtualNetDevice::GetMulticast (Ipv6Address addr) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");  
}


bool
VirtualNetDevice::IsPointToPoint (void) const
{
  return m_isPointToPoint;
}

bool
VirtualNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  return SendFrom (packet, GetAddress (), dest, protocolNumber);
}

bool
VirtualNetDevice::SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  if (m_sendCb (packet, source, dest, protocolNumber))
    {
      m_txTrace (packet);
      return true;
    }
  return false;
}

Ptr<Node>
VirtualNetDevice::GetNode (void) const
{
  return m_node;
}

void
VirtualNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}

bool
VirtualNetDevice::NeedsArp (void) const
{
  return m_needsArp;
}

void
VirtualNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}

void
VirtualNetDevice::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
{
  m_promiscRxCallback = cb;
}

bool
VirtualNetDevice::SupportsSendFrom () const
{
  return m_supportsSendFrom;
}

bool VirtualNetDevice::IsBridge (void) const
{
  return false;
}


} // namespace ns3
