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
#include "tap-net-device.h"
#include "tap-manager-client.h"
#include "ns3/node.h"
#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/system-thread.h"
#include "ns3/realtime-simulator-impl.h"
#include "ns3/make-event.h"
#include "ns3/simulator.h"
#include "ns3/ethernet-header.h"
#include "ns3/trace-source-accessor.h"
#include "host-tap-net-device.h"
#include "tap-channel.h"
#include <errno.h>
#include <stdlib.h>

NS_LOG_COMPONENT_DEFINE ("TapNetDevice");

namespace ns3 {

TypeId 
TapNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TapNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<TapNetDevice> ()
    .AddTraceSource ("Rx", "A packet has been received",
                     MakeTraceSourceAccessor (&TapNetDevice::m_rxTrace))
    .AddTraceSource ("Tx", "A packet has been sent",
                     MakeTraceSourceAccessor (&TapNetDevice::m_txTrace))
    .AddTraceSource ("Drop", "A packet has been dropped",
                     MakeTraceSourceAccessor (&TapNetDevice::m_dropTrace))
    ;
  return tid;
}

TapNetDevice::TapNetDevice ()
  : m_node (0),
    m_mtu (0xffff),
    m_name (""),
    m_ifIndex (0),
    m_tap (-1)
{
  NS_LOG_FUNCTION (this);
}

void 
TapNetDevice::SetChannel (Ptr<TapChannel> channel)
{
  m_channel = channel;
  m_channel->SetDevice (this);
}

void 
TapNetDevice::SetupHost (Ipv4Address ad, Ipv4Mask mask, Ipv4Address gateway)
{
  NS_LOG_FUNCTION (this << ad << mask << gateway);
  NS_ASSERT (m_tap == -1);

  Mac48Address hostMacAddress = m_channel->GetHostDevice ()->GetMacAddress ();

  TapManagerClient manager;
  m_tap = manager.AllocateTap (hostMacAddress, ad, mask, gateway);

  m_thread = Create<SystemThread> (MakeCallback (&TapNetDevice::ReadThread, this));
  m_thread->Start ();
}


void
TapNetDevice::ReadThread (void)
{
  NS_LOG_FUNCTION (this);

  while (1)
    {
      uint8_t *buffer = (uint8_t *)malloc (0xffff);
      ssize_t bytesRead = read (m_tap, buffer, 0xffff);
      if (bytesRead == -1)
        {
          if (errno == EBADF || errno == EINTR)
            {
              // the device was closed from under us by ::DoDispose
              return;
            }
          NS_FATAL_ERROR ("Error reading from tap device: errno=" << strerror (errno));
        }
      // Note: we purposedly don't use a smart pointer to manage this packet
      // because the want to hand over ownership of this packet to the ForwardUp
      // method.
      DynamicCast<RealtimeSimulatorImpl> (Simulator::GetImplementation ())->
        ScheduleRealtimeNow (MakeEvent (&TapNetDevice::ForwardUp, this, buffer, (uint32_t)bytesRead));
    }
}

void
TapNetDevice::ForwardUp (uint8_t *buffer, uint32_t size)
{
  NS_LOG_FUNCTION (this << buffer << size);

  // swallow packet reference in smart pointer.
  Ptr<Packet> packet = Create<Packet> (buffer, size);
  free (buffer);
  Ptr<Packet> copy = packet->Copy ();

  EthernetHeader header = EthernetHeader (false);
  packet->RemoveHeader (header);

  uint16_t protocol = header.GetLengthType ();
  Mac48Address to = header.GetDestination ();
  Mac48Address from = header.GetSource ();
  
  NetDevice::PacketType packetType;
  if (to == m_address)
    {
      packetType = NetDevice::PACKET_HOST;
    }
  else if (to.IsBroadcast ())
    {
      packetType = NetDevice::PACKET_HOST;
    }
  else if (to.IsMulticast ())
    {
      packetType = NetDevice::PACKET_MULTICAST;
    }
  else 
    {
      packetType = NetDevice::PACKET_OTHERHOST;
    }
  m_rxTrace (copy, from, to);
  if (packetType != NetDevice::PACKET_OTHERHOST)
    {
      m_rxCallback (this, packet, protocol, from);
    }
  if (!m_promiscCallback.IsNull ())
    {
      m_promiscCallback (this, packet, protocol, from, to, packetType);
    }
}

void 
TapNetDevice::SetAddress (Mac48Address address)
{
  m_address = address;
}

void 
TapNetDevice::SetName(const std::string name)
{
  m_name = name;
}
std::string 
TapNetDevice::GetName(void) const
{
  return m_name;
}
void 
TapNetDevice::SetIfIndex(const uint32_t index)
{
  m_ifIndex = index;
}
uint32_t 
TapNetDevice::GetIfIndex(void) const
{
  return m_ifIndex;
}
Ptr<Channel> 
TapNetDevice::GetChannel (void) const
{
  return m_channel;
}
Address 
TapNetDevice::GetAddress (void) const
{
  return m_address;
}
bool 
TapNetDevice::SetMtu (const uint16_t mtu)
{
  m_mtu = mtu;
  return true;
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
{}
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
  return true;
}
Address 
TapNetDevice::GetMulticast (void) const
{
  return Mac48Address::GetMulticastPrefix ();
}
Address 
TapNetDevice::MakeMulticastAddress (Ipv4Address multicastGroup) const
{
  return Mac48Address::GetMulticast (multicastGroup);
}
bool 
TapNetDevice::IsPointToPoint (void) const
{
  return false;
}
bool 
TapNetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << dest << protocolNumber);
  return SendFrom (packet, m_address, dest, protocolNumber);
}
bool 
TapNetDevice::SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << packet << source << dest << protocolNumber);
  Mac48Address to = Mac48Address::ConvertFrom (dest);
  Mac48Address from = Mac48Address::ConvertFrom (source);

  EthernetHeader header = EthernetHeader (false);
  header.SetSource (from);
  header.SetDestination (to);
  header.SetLengthType (protocolNumber);
  packet->AddHeader (header);

  ssize_t written = write (m_tap, packet->PeekData (), packet->GetSize ());
  if (written == -1 || written != (ssize_t)packet->GetSize ())
    {
      m_dropTrace (packet, from, to);
      return false;
    }

  m_txTrace (packet, from, to);
  
  return true;
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
  return true;
}
void 
TapNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}

void
TapNetDevice::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  close (m_tap);
  m_thread->Join ();
  m_thread = 0;
  m_node = 0;
  m_channel = 0;
  NetDevice::DoDispose ();
}


void
TapNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  m_promiscCallback = cb;
}

bool
TapNetDevice::SupportsSendFrom (void) const
{
  return true;
}

} // namespace ns3
