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
#include "ns3/boolean.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("BridgeNetDevice");

namespace ns3 {


TypeId
BridgeNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BridgeNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<BridgeNetDevice> ()
    .AddAttribute ("EnableLearning",
                   "Enable the learning mode of the Learning Bridge",
                   BooleanValue (true),
                   MakeBooleanAccessor (&BridgeNetDevice::m_enableLearning),
                   MakeBooleanChecker ())
    .AddAttribute ("ExpirationTime",
                   "Time it takes for learned MAC state entry to expire.",
                   TimeValue (Seconds (30)),
                   MakeTimeAccessor (&BridgeNetDevice::m_expirationTime),
                   MakeTimeChecker ())
    ;
  return tid;
}


BridgeNetDevice::BridgeNetDevice ()
  : m_node (0),
    m_name (""),
    m_ifIndex (0),
    m_mtu (0xffff)
{}

void
BridgeNetDevice::ReceiveFromDevice (Ptr<NetDevice> incomingPort, Ptr<Packet> packet, uint16_t protocol,
                                    Address const &src, Address const &dst, PacketType packetType)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_DEBUG ("UID is " << packet->GetUid ());

  Mac48Address src48 = Mac48Address::ConvertFrom (src);
  Mac48Address dst48 = Mac48Address::ConvertFrom (dst);


  switch (packetType)
    {
    case PACKET_HOST:
      if (dst48 == m_address)
        {
          m_rxCallback (this, packet, protocol, src, dst, packetType);
        }
      break;

    case PACKET_BROADCAST:
    case PACKET_MULTICAST:
      m_rxCallback (this, packet, protocol, src, dst, packetType);
      ForwardBroadcast (incomingPort, packet, protocol, src48, dst48);
      break;

    case PACKET_OTHERHOST:
      ForwardUnicast (incomingPort, packet, protocol, src48, dst48);
      break;
    }
}

void
BridgeNetDevice::ForwardUnicast (Ptr<NetDevice> incomingPort, Ptr<Packet> packet,
                                 uint16_t protocol, Mac48Address src, Mac48Address dst)
{
  NS_LOG_DEBUG ("LearningBridgeForward (incomingPort=" << incomingPort->GetName ()
                << ", packet=" << packet << ", protocol="<<protocol
                << ", src=" << src << ", dst=" << dst << ")");

  Learn (src, incomingPort);
  Ptr<NetDevice> outPort = GetLearnedState (dst);
  if (outPort != NULL && outPort != incomingPort)
    {
      NS_LOG_LOGIC ("Learning bridge state says to use port `" << outPort->GetName () << "'");
      outPort->SendFrom (packet->Copy (), src, dst, protocol);
    }
  else
    {
      NS_LOG_LOGIC ("No learned state: send through all ports");
      for (std::vector< Ptr<NetDevice> >::iterator iter = m_ports.begin ();
           iter != m_ports.end (); iter++)
        {
          Ptr<NetDevice> port = *iter;
          if (port != incomingPort)
            {
              NS_LOG_LOGIC ("LearningBridgeForward (" << src << " => " << dst << "): " << incomingPort->GetName ()
                            << " --> " << port->GetName ()
                            << " (UID " << packet->GetUid () << ").");
              port->SendFrom (packet->Copy (), src, dst, protocol);
            }
        }
    }
}

void
BridgeNetDevice::ForwardBroadcast (Ptr<NetDevice> incomingPort, Ptr<Packet> packet,
                                        uint16_t protocol, Mac48Address src, Mac48Address dst)
{
  NS_LOG_DEBUG ("LearningBridgeForward (incomingPort=" << incomingPort->GetName ()
                << ", packet=" << packet << ", protocol="<<protocol
                << ", src=" << src << ", dst=" << dst << ")");
  Learn (src, incomingPort);

  for (std::vector< Ptr<NetDevice> >::iterator iter = m_ports.begin ();
         iter != m_ports.end (); iter++)
    {
      Ptr<NetDevice> port = *iter;
      if (port != incomingPort)
        {
          NS_LOG_LOGIC ("LearningBridgeForward (" << src << " => " << dst << "): " << incomingPort->GetName ()
                        << " --> " << port->GetName ()
                        << " (UID " << packet->GetUid () << ").");
          port->SendFrom (packet->Copy (), src, dst, protocol);
        }
    }
}

void BridgeNetDevice::Learn (Mac48Address source, Ptr<NetDevice> port)
{
  if (m_enableLearning)
    {
      LearnedState &state = m_learnState[source];
      state.associatedPort = port;
      state.expirationTime = Simulator::Now () + m_expirationTime;
    }
}

Ptr<NetDevice> BridgeNetDevice::GetLearnedState (Mac48Address source)
{
  if (m_enableLearning)
    {
      std::map<Mac48Address, LearnedState>::iterator iter =
        m_learnState.find (source);
      if (iter != m_learnState.end ())
        {
          LearnedState &state = iter->second;
          if (state.expirationTime > Simulator::Now ())
            {
              return state.associatedPort;
            }
          else
            {
              m_learnState.erase (iter);
            }
        }
    }
  return NULL;
}

void 
BridgeNetDevice::AddBridgePort (Ptr<NetDevice> bridgePort)
{
  NS_ASSERT (bridgePort != this);
  if (m_address == Mac48Address ())
    {
      m_address = Mac48Address::ConvertFrom (bridgePort->GetAddress ());
    }

  m_node->RegisterProtocolHandler (MakeCallback (&BridgeNetDevice::ReceiveFromDevice, this),
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
BridgeNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  for (std::vector< Ptr<NetDevice> >::iterator iter = m_ports.begin ();
         iter != m_ports.end (); iter++)
    {
      Ptr<NetDevice> port = *iter;
      port->SendFrom (packet, m_address, dest, protocolNumber);
    }

  return true;
}

bool 
BridgeNetDevice::SendFrom (Ptr<Packet> packet, const Address& src, const Address& dest, uint16_t protocolNumber)
{
  for (std::vector< Ptr<NetDevice> >::iterator iter = m_ports.begin ();
         iter != m_ports.end (); iter++)
    {
      Ptr<NetDevice> port = *iter;
      port->SendFrom (packet, src, dest, protocolNumber);
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
BridgeNetDevice::DoDispose (void)
{
  m_node = 0;
  NetDevice::DoDispose ();
}



} // namespace ns3
