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
#include "tap-net-device.h"
#include "ns3/channel.h"
#include "ns3/trace-source-accessor.h"


NS_LOG_COMPONENT_DEFINE ("TapNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TapNetDevice);

TypeId
TapNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TapNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<TapNetDevice> ()
    .AddTraceSource ("Rx", "Received payload from the MAC layer.",
                     MakeTraceSourceAccessor (&TapNetDevice::m_rxTrace))
    .AddTraceSource ("Tx", "Send payload to the MAC layer.",
                     MakeTraceSourceAccessor (&TapNetDevice::m_txTrace))
    ;
  return tid;
}

TapNetDevice::TapNetDevice ()
{
  m_needsArp = false;
  m_supportsSendFrom = true;
  m_mtu = 65535;
}


void
TapNetDevice::SetSendFromCallback (SendFromCallback sendCb)
{
  m_sendCb = sendCb;
}

void
TapNetDevice::SetNeedsArp (bool needsArp)
{
  m_needsArp = needsArp;
}

void
TapNetDevice::SetSupportsSendFrom (bool supportsSendFrom)
{
  m_supportsSendFrom = supportsSendFrom;
}

bool
TapNetDevice::SetMtu (const uint16_t mtu)
{
  m_mtu = mtu;
  return true;
}


TapNetDevice::~TapNetDevice()
{
  NS_LOG_FUNCTION_NOARGS ();
}


void TapNetDevice::DoDispose()
{
  NS_LOG_FUNCTION_NOARGS ();
  NetDevice::DoDispose ();
}

bool
TapNetDevice::Receive (Ptr<Packet> packet, uint16_t protocol, const Address &address)
{
  if (m_rxCallback (this, packet, protocol, address))
    {
      m_rxTrace (packet);
      return true;
    }
  return false;
}

bool
TapNetDevice::PromiscReceive (Ptr<Packet> packet, uint16_t protocol,
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
TapNetDevice::SetIfIndex (const uint32_t index)
{
  m_index = index;
}

uint32_t
TapNetDevice::GetIfIndex (void) const
{
  return m_index;
}

Ptr<Channel>
TapNetDevice::GetChannel (void) const
{
  return Ptr<Channel> ();
}

Address
TapNetDevice::GetAddress (void) const
{
  return Mac48Address ();
}

uint16_t
TapNetDevice::GetMtu (void) const
{
  return m_mtu;
}

bool
TapNetDevice::IsLinkUp (void) const
{
  return true;
}

void
TapNetDevice::SetLinkChangeCallback (Callback<void> callback)
{
}

bool
TapNetDevice::IsBroadcast (void) const
{
  return true;
}

Address
TapNetDevice::GetBroadcast (void) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

bool
TapNetDevice::IsMulticast (void) const
{
  return false;
}

Address TapNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

Address TapNetDevice::GetMulticast (Ipv6Address addr) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");  
}


bool
TapNetDevice::IsPointToPoint (void) const
{
  return true;
}

bool
TapNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  return SendFrom (packet, GetAddress (), dest, protocolNumber);
}

bool
TapNetDevice::SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  if (m_sendCb (packet, source, dest, protocolNumber))
    {
      m_txTrace (packet);
      return true;
    }
  return false;
}

Ptr<Node>
TapNetDevice::GetNode (void) const
{
  return m_node;
}

void
TapNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}

bool
TapNetDevice::NeedsArp (void) const
{
  return m_needsArp;
}

void
TapNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}

void
TapNetDevice::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
{
  m_promiscRxCallback = cb;
}

bool
TapNetDevice::SupportsSendFrom () const
{
  return m_supportsSendFrom;
}

bool TapNetDevice::IsBridge (void) const
{
  return false;
}


} // namespace ns3
