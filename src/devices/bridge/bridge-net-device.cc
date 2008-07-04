/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Gustavo Carneiro  <gjc@inescporto.pt>
 */
#include "bridge-net-device.h"
#include "ns3/node.h"
#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("BridgeNetDevice");

namespace ns3 {


TypeId
BridgeNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BridgeNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<BridgeNetDevice> ()
    ;
  return tid;
}


BridgeNetDevice::BridgeNetDevice ()
  : m_node (0),
    m_mtu (0xffff),
    m_name (""),
    m_ifIndex (0)
{}

void
BridgeNetDevice::PromiscReceive (Ptr<NetDevice> incomingPort, Ptr<Packet> packet, uint16_t protocol,
                                 Address const &src, Address const &dst, bool forMe)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_LOGIC ("UID is " << packet->GetUid ());

  Mac48Address src48 = Mac48Address::ConvertFrom (src);
  Mac48Address dst48 = Mac48Address::ConvertFrom (dst);

//
// We never forward up packets that we sent.  Real devices don't do this since
// their receivers are disabled during send, so we don't.  Drop the packet 
// silently (no tracing) since it would really never get here in a real device.
//
  if (src48 == m_address)
    {
      NS_LOG_LOGIC ("Ignoring packet sourced by this device");
      return;
    }

//
// An IP host group address is mapped to an Ethernet multicast address
// by placing the low-order 23-bits of the IP address into the low-order
// 23 bits of the Ethernet multicast address 01-00-5E-00-00-00 (hex).
//
// We are going to receive all packets destined to any multicast address,
// which means clearing the low-order 23 bits the header destination 
//
  Mac48Address mcDest;
  uint8_t      mcBuf[6];

  dst48.CopyTo (mcBuf);
  mcBuf[3] &= 0x80;
  mcBuf[4] = 0;
  mcBuf[5] = 0;
  mcDest.CopyFrom (mcBuf);

  Mac48Address multicast = Mac48Address::ConvertFrom (GetMulticast ());
  Mac48Address broadcast = Mac48Address::ConvertFrom (GetBroadcast ());

  // decide whether this node should receive the packet for itself
  
  NS_LOG_DEBUG ("incomingPort: "  << incomingPort->GetName ()
                << "; my address: " << m_address
                << "; broadcast: " << broadcast
                << "; dst48: " << dst48);

  if ((dst48 == broadcast) ||
      (mcDest == multicast) ||
      (dst48 == m_address))
    {
      m_rxCallback (this, packet, protocol, src);
      // m_rxTrace (originalPacket);
    }

  // decide whether the packet should be forwarded
  if ((dst48 == broadcast) ||
      (mcDest == multicast) ||
      (dst48 != m_address))
    {
      LearningBridgeForward (incomingPort, packet, protocol, src48, dst48);
    }
}

void
BridgeNetDevice::LearningBridgeForward (Ptr<NetDevice> incomingPort, Ptr<Packet> packet,
                                        uint16_t protocol, Mac48Address src, Mac48Address dst)
{
  NS_LOG_DEBUG ("LearningBridgeForward (incomingPort=" << incomingPort->GetName ()
                << ", packet=" << packet << ", protocol="<<protocol
                << ", src=" << src << ", dst=" << dst << ")");
  // TODO: add the "learning" part

  for (std::vector< Ptr<NetDevice> >::iterator iter = m_ports.begin ();
         iter != m_ports.end (); iter++)
    {
      Ptr<NetDevice> port = *iter;
      if (port != incomingPort)
        {
          // TODO: port->SendFrom (packet, protocol, dst, src);
          port->Send (packet->Copy (), dst, protocol);
        }
    }
}


void 
BridgeNetDevice::AddBridgePort (Ptr<NetDevice> bridgePort)
{
  if (m_address == Mac48Address ())
    {
      m_address = Mac48Address::ConvertFrom (bridgePort->GetAddress ());
    }

  m_node->RegisterPromiscuousProtocolHandler (MakeCallback (&BridgeNetDevice::PromiscReceive, this),
                                              0, bridgePort);
  m_ports.push_back (bridgePort);
}

void 
BridgeNetDevice::SetName(const std::string name)
{
  m_name = name;
}

std::string 
BridgeNetDevice::GetName(void) const
{
  return m_name;
}

void 
BridgeNetDevice::SetIfIndex(const uint32_t index)
{
  m_ifIndex = index;
}

uint32_t 
BridgeNetDevice::GetIfIndex(void) const
{
  return m_ifIndex;
}

Ptr<Channel> 
BridgeNetDevice::GetChannel (void) const
{
  return 0;
}

Address 
BridgeNetDevice::GetAddress (void) const
{
  return m_address;
}

bool 
BridgeNetDevice::SetMtu (const uint16_t mtu)
{
  m_mtu = mtu;
  return true;
}

uint16_t 
BridgeNetDevice::GetMtu (void) const
{
  return m_mtu;
}


bool 
BridgeNetDevice::IsLinkUp (void) const
{
  return true;
}


void 
BridgeNetDevice::SetLinkChangeCallback (Callback<void> callback)
{}


bool 
BridgeNetDevice::IsBroadcast (void) const
{
  return true;
}


Address
BridgeNetDevice::GetBroadcast (void) const
{
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

bool
BridgeNetDevice::IsMulticast (void) const
{
  return true;
}


Address
BridgeNetDevice::GetMulticast (void) const
{
  return Mac48Address ("01:00:5e:00:00:00");
}


Address
BridgeNetDevice::MakeMulticastAddress (Ipv4Address multicastGroup) const
{
 NS_LOG_FUNCTION (this << multicastGroup);
//
// First, get the generic multicast address.
//
  Address hardwareDestination = GetMulticast ();

  NS_LOG_LOGIC ("Device multicast address: " << hardwareDestination);
//
// It's our address, and we know we're playing with an EUI-48 address here
// primarily since we know that by construction, but also since the parameter
// is an Ipv4Address.
//
  Mac48Address etherAddr = Mac48Address::ConvertFrom (hardwareDestination);
//
// We now have the multicast address in an abstract 48-bit container.  We 
// need to pull it out so we can play with it.  When we're done, we have the 
// high order bits in etherBuffer[0], etc.
//
  uint8_t etherBuffer[6];
  etherAddr.CopyTo (etherBuffer);
//
// Now we need to pull the raw bits out of the Ipv4 destination address.
//
  uint8_t ipBuffer[4];
  multicastGroup.Serialize (ipBuffer);
//
// RFC 1112 says that an Ipv4 host group address is mapped to an EUI-48
// multicast address by placing the low-order 23-bits of the IP address into 
// the low-order 23 bits of the Ethernet multicast address 
// 01-00-5E-00-00-00 (hex). 
//
  etherBuffer[3] |= ipBuffer[1] & 0x7f;
  etherBuffer[4] = ipBuffer[2];
  etherBuffer[5] = ipBuffer[3];
//
// Now, etherBuffer has the desired ethernet multicast address.  We have to
// suck these bits back into the Mac48Address,
//
  etherAddr.CopyFrom (etherBuffer);
//
// Implicit conversion (operator Address ()) is defined for Mac48Address, so
// use it by just returning the EUI-48 address which is automagically converted
// to an Address.
//
  NS_LOG_LOGIC ("multicast address is " << etherAddr);

  return etherAddr;
}


bool 
BridgeNetDevice::IsPointToPoint (void) const
{
  return false;
}


bool 
BridgeNetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  Mac48Address to = Mac48Address::ConvertFrom (dest);

  for (std::vector< Ptr<NetDevice> >::iterator iter = m_ports.begin ();
         iter != m_ports.end (); iter++)
    {
      Ptr<NetDevice> port = *iter;
      // TODO: port->SendFrom (packet, protocolNumber, to, m_address);
      port->Send (packet->Copy (), to, protocolNumber);
    }

  return true;
}


Ptr<Node> 
BridgeNetDevice::GetNode (void) const
{
  return m_node;
}


void 
BridgeNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}


bool 
BridgeNetDevice::NeedsArp (void) const
{
  return true;
}


void 
BridgeNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_rxCallback = cb;
}


void 
BridgeNetDevice::SetPromiscuousReceiveCallback (NetDevice::PromiscuousReceiveCallback cb)
{
  m_promiscRxCallback = cb;
}


void
BridgeNetDevice::DoDispose (void)
{
  m_node = 0;
  NetDevice::DoDispose ();
}



} // namespace ns3
