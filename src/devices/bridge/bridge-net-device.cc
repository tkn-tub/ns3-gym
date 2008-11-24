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

NS_OBJECT_ENSURE_REGISTERED (BridgeNetDevice);


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
{
  m_channel = CreateObject<BridgeChannel> ();
}

void
BridgeNetDevice::ReceiveFromDevice (Ptr<NetDevice> incomingPort, Ptr<const Packet> packet, uint16_t protocol,
                                    Address const &src, Address const &dst, PacketType packetType)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_DEBUG ("UID is " << packet->GetUid ());

  Mac48Address src48 = Mac48Address::ConvertFrom (src);
  Mac48Address dst48 = Mac48Address::ConvertFrom (dst);

  if (!m_promiscRxCallback.IsNull ())
    {
      m_promiscRxCallback (this, packet, protocol, src, dst, packetType);
    }

  switch (packetType)
    {
    case PACKET_HOST:
      if (dst48 == m_address)
        {
          m_rxCallback (this, packet, protocol, src);
        }
      break;

    case PACKET_BROADCAST:
    case PACKET_MULTICAST:
      m_rxCallback (this, packet, protocol, src);
      ForwardBroadcast (incomingPort, packet, protocol, src48, dst48);
      break;

    case PACKET_OTHERHOST:
      ForwardUnicast (incomingPort, packet, protocol, src48, dst48);
      break;
    }
}

void
BridgeNetDevice::ForwardUnicast (Ptr<NetDevice> incomingPort, Ptr<const Packet> packet,
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
BridgeNetDevice::ForwardBroadcast (Ptr<NetDevice> incomingPort, Ptr<const Packet> packet,
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
      Time now = Simulator::Now ();
      std::map<Mac48Address, LearnedState>::iterator iter =
        m_learnState.find (source);
      if (iter != m_learnState.end ())
        {
          LearnedState &state = iter->second;
          if (state.expirationTime > now)
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
  if (!Mac48Address::IsMatchingType (bridgePort->GetAddress ()))
    {
      NS_FATAL_ERROR ("Device does not support eui 48 addresses: cannot be added to bridge.");
    }
  if (!bridgePort->SupportsSendFrom ())
    {
      NS_FATAL_ERROR ("Device does not support SendFrom: cannot be added to bridge.");
    }
  if (m_address == Mac48Address ())
    {
      m_address = Mac48Address::ConvertFrom (bridgePort->GetAddress ());
    }

  NS_LOG_DEBUG ("RegisterProtocolHandler for " << bridgePort->GetName ());
  m_node->RegisterProtocolHandler (MakeCallback (&BridgeNetDevice::ReceiveFromDevice, this),
                                   0, bridgePort, true);
  m_ports.push_back (bridgePort);
  m_channel->AddChannel (bridgePort->GetChannel ());
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
  return m_channel;
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
BridgeNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
 NS_LOG_FUNCTION (this << multicastGroup);
 Mac48Address multicast = Mac48Address::GetMulticast (multicastGroup);
 return multicast;
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
BridgeNetDevice::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
{
  m_promiscRxCallback = cb;
}

bool
BridgeNetDevice::SupportsSendFrom () const
{
  return true;
}


void
BridgeNetDevice::DoDispose (void)
{
  m_node = 0;
  NetDevice::DoDispose ();
}

Address BridgeNetDevice::GetMulticast (Ipv6Address addr) const
{
  NS_LOG_FUNCTION (this << addr);
  return Mac48Address::GetMulticast (addr);
}

} // namespace ns3
