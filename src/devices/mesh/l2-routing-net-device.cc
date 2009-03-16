/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#include "ns3/node.h"
#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/boolean.h"
#include "ns3/simulator.h"
#include "ns3/l2-routing-net-device.h"

NS_LOG_COMPONENT_DEFINE ("L2RoutingNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (L2RoutingNetDevice);


TypeId
L2RoutingNetDevice::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::L2RoutingNetDevice")
                      .SetParent<NetDevice> ()
                      .AddConstructor<L2RoutingNetDevice> ()
                      ;
  return tid;
}


L2RoutingNetDevice::L2RoutingNetDevice ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_channel = CreateObject<BridgeChannel> ();
}

L2RoutingNetDevice::~L2RoutingNetDevice()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
L2RoutingNetDevice::DoDispose ()
{
  NS_LOG_FUNCTION_NOARGS ();
  for (std::vector< Ptr<NetDevice> >::iterator iter = m_ports.begin (); iter != m_ports.end (); iter++)
    *iter = 0;
  m_ports.clear ();
  m_node = 0;
  NetDevice::DoDispose ();

}

void
L2RoutingNetDevice::ReceiveFromDevice (Ptr<NetDevice> incomingPort, Ptr<const Packet> packet, uint16_t protocol,
                                       Address const &src, Address const &dst, PacketType packetType)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_DEBUG ("UID is " << packet->GetUid ());
  const Mac48Address src48 = Mac48Address::ConvertFrom (src);
  const Mac48Address dst48 = Mac48Address::ConvertFrom (dst);
  if (!m_promiscRxCallback.IsNull ())
    m_promiscRxCallback (this, packet, protocol, src, dst, packetType);
  switch (packetType)
    {
    case PACKET_HOST:
      if (dst48 == m_address)
        m_rxCallback (this, packet, protocol, src);
      break;
    case PACKET_BROADCAST:
    case PACKET_MULTICAST:
      m_rxCallback (this, packet, protocol, src);
    case PACKET_OTHERHOST:
      Forward (incomingPort, packet->Copy(), protocol, src48, dst48);
      break;
    }

}

void
L2RoutingNetDevice::Forward (Ptr<NetDevice> inport, Ptr<Packet> packet,
                             uint16_t protocol, const Mac48Address src, const Mac48Address dst)
{
  //pass through routing protocol
  m_requestRoute(inport->GetIfIndex(), src, dst, packet, protocol, m_myResponse);
}

uint32_t
L2RoutingNetDevice::GetNPorts () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return m_ports.size ();
  }

Ptr<NetDevice>
L2RoutingNetDevice::GetPort (uint32_t n) const
  {
    NS_ASSERT(m_ports.size () > n);
    return m_ports[n];
  }

void
L2RoutingNetDevice::AddPort (Ptr<NetDevice> routingPort)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (routingPort != this);
  if (!Mac48Address::IsMatchingType (routingPort->GetAddress ()))
    NS_FATAL_ERROR ("Device does not support eui 48 addresses: cannot be added to bridge.");
  if (!routingPort->SupportsSendFrom ())
    NS_FATAL_ERROR ("Device does not support SendFrom: cannot be added to bridge.");
  m_address = Mac48Address::ConvertFrom (routingPort->GetAddress ());
  NS_LOG_DEBUG ("RegisterProtocolHandler for " << routingPort->GetName ());
  m_node->RegisterProtocolHandler (MakeCallback (&L2RoutingNetDevice::ReceiveFromDevice, this),
                                   0, routingPort, true);
  m_ports.push_back (routingPort);
  m_channel->AddChannel (routingPort->GetChannel ());
}

void
L2RoutingNetDevice::SetName(const std::string name)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_name = name;
}

std::string
L2RoutingNetDevice::GetName() const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return m_name;
  }

void
L2RoutingNetDevice::SetIfIndex(const uint32_t index)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_ifIndex = index;
}

uint32_t
L2RoutingNetDevice::GetIfIndex() const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return m_ifIndex;
  }

Ptr<Channel>
L2RoutingNetDevice::GetChannel () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return m_channel;
  }

Address
L2RoutingNetDevice::GetAddress () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return m_address;
  }

bool
L2RoutingNetDevice::SetMtu (const uint16_t mtu)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_mtu = mtu;
  return true;
}

uint16_t
L2RoutingNetDevice::GetMtu () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return 1500;
  }


bool
L2RoutingNetDevice::IsLinkUp () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return true;
  }


void
L2RoutingNetDevice::SetLinkChangeCallback (Callback<void> callback)
{}

bool
L2RoutingNetDevice::IsBroadcast () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return true;
  }


Address
L2RoutingNetDevice::GetBroadcast () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return Mac48Address ("ff:ff:ff:ff:ff:ff");
  }

bool
L2RoutingNetDevice::IsMulticast () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return true;
  }

Address
L2RoutingNetDevice::GetMulticast (Ipv4Address multicastGroup) const
  {
    NS_LOG_FUNCTION (this << multicastGroup);
    Mac48Address multicast = Mac48Address::GetMulticast (multicastGroup);
    return multicast;
  }


bool
L2RoutingNetDevice::IsPointToPoint () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return false;
  }

bool
L2RoutingNetDevice::IsBridge () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return false;
  }


bool
L2RoutingNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{

  const Mac48Address dst48 = Mac48Address::ConvertFrom (dest);
  return m_requestRoute(m_ifIndex, m_address, dst48, packet, protocolNumber, m_myResponse);
}

bool
L2RoutingNetDevice::SendFrom (Ptr<Packet> packet, const Address& src, const Address& dest, uint16_t protocolNumber)
{
  const Mac48Address src48 = Mac48Address::ConvertFrom (src);
  const Mac48Address dst48 = Mac48Address::ConvertFrom (dest);
  return m_requestRoute(m_ifIndex, src48, dst48, packet, protocolNumber, m_myResponse);
}


Ptr<Node>
L2RoutingNetDevice::GetNode () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return m_node;
  }


void
L2RoutingNetDevice::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = node;
}


bool
L2RoutingNetDevice::NeedsArp () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return true;
  }


void
L2RoutingNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_rxCallback = cb;
}

void
L2RoutingNetDevice::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_promiscRxCallback = cb;
}

bool
L2RoutingNetDevice::SupportsSendFrom () const
  {
    NS_LOG_FUNCTION_NOARGS ();
    return true;
  }

Address
L2RoutingNetDevice::GetMulticast (Ipv6Address addr) const
  {
    NS_LOG_FUNCTION (this << addr);
    return Mac48Address::GetMulticast (addr);
  }
//L2RouitingSpecific methods:

bool
L2RoutingNetDevice::AttachProtocol(Ptr<L2RoutingProtocol> protocol)
{
  m_requestRoute = MakeCallback(&L2RoutingProtocol::RequestRoute, protocol);
  m_myResponse = MakeCallback(&L2RoutingNetDevice::ProtocolResponse, this);
  protocol->SetIfIndex(m_ifIndex);
  return protocol->AttachPorts(m_ports);
}

void
L2RoutingNetDevice::ProtocolResponse(
  bool success,
  Ptr<Packet> packet,
  Mac48Address src,
  Mac48Address dst,
  uint16_t protocol,
  uint32_t outPort
)
{
  if (!success)
    {
      NS_LOG_UNCOND("Resolve failed");
      //TODO: SendError callback
      return;
    }
  //just do sendFrom!
  if (outPort!=0xffffffff)
    GetPort(outPort)->SendFrom(packet, src, dst, protocol);
  else
    for (std::vector<Ptr<NetDevice> >::iterator i = m_ports.begin(); i != m_ports.end(); i++)
      (*i) -> SendFrom(packet, src, dst, protocol);
}

} // namespace ns3
