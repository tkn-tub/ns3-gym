/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#include "ns3/llc-snap-header.h"
#include "ns3/simulator.h"
#include "ns3/callback.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "lte-net-device.h"
#include "ns3/packet-burst.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/pointer.h"
#include "ns3/enum.h"
#include "radio-bearer-instance.h"
#include "amc-module.h"
#include "rrc-entity.h"
#include "rlc-entity.h"
#include "lte-mac-header.h"
#include "ns3/ipv4-header.h"

NS_LOG_COMPONENT_DEFINE ("LteNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED ( LteNetDevice);


TypeId LteNetDevice::GetTypeId (void)
{
  static TypeId
    tid =
    TypeId ("ns3::LteNetDevice")

    .SetParent<NetDevice> ()

    .AddAttribute ("Mtu", "The MAC-level Maximum Transmission Unit",
                   UintegerValue (1500),
                   MakeUintegerAccessor (&LteNetDevice::SetMtu,
                                         &LteNetDevice::GetMtu),
                   MakeUintegerChecker<uint16_t> (0,MAX_MSDU_SIZE))

    .AddAttribute ("Phy",
                   "The PHY layer attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&LteNetDevice::GetPhy, &LteNetDevice::SetPhy),
                   MakePointerChecker<LtePhy> ())
  ;
  return tid;
}

LteNetDevice::LteNetDevice (void)
{
  NS_LOG_FUNCTION (this);
}


LteNetDevice::~LteNetDevice (void)
{
  NS_LOG_FUNCTION (this);
}


void
LteNetDevice::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  m_phy->Dispose ();
  m_phy = 0;
  m_node = 0;
  m_rrcEntity->Dispose ();
  m_rrcEntity = 0;
  m_phyMacTxStartCallback = MakeNullCallback< bool, Ptr<Packet> > ();
  NetDevice::DoDispose ();
}


Ptr<Channel>
LteNetDevice::GetChannel (void) const
{
  NS_LOG_FUNCTION (this);
  return GetPhy ()->GetDownlinkSpectrumPhy ()->GetChannel ();
}


void
LteNetDevice::SetPhy (Ptr<LtePhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  m_phy = phy;
}


Ptr<LtePhy>
LteNetDevice::GetPhy (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy;
}


void
LteNetDevice::SetRrcEntity (Ptr<RrcEntity> rrc)
{
  NS_LOG_FUNCTION (this << rrc);
  m_rrcEntity = rrc;
}


Ptr<RrcEntity>
LteNetDevice::GetRrcEntity (void)
{
  NS_LOG_FUNCTION (this);
  return m_rrcEntity;
}


void
LteNetDevice::SetAddress (Address address)
{
  NS_LOG_FUNCTION (this << address);
  m_address = Mac48Address::ConvertFrom (address);
}


Address
LteNetDevice::GetAddress (void) const
{
  NS_LOG_FUNCTION (this);
  return m_address;
}


void
LteNetDevice::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);
  m_node = node;
}


Ptr<Node>
LteNetDevice::GetNode (void) const
{
  NS_LOG_FUNCTION (this);
  return m_node;
}


void
LteNetDevice::SetReceiveCallback (ReceiveCallback cb)
{
  NS_LOG_FUNCTION (this);
  m_rxCallback = cb;
}


void
LteNetDevice::ForwardUp (Ptr<Packet> packet, const Mac48Address &source, const Mac48Address &dest)
{

}


void
LteNetDevice::ForwardUp (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);

  m_macRxTrace (packet);

  LteMacHeader header;
  packet->RemoveHeader (header);
  NS_LOG_LOGIC ("packet " << header.GetSource () << " --> " << header.GetDestination () << " (here: " << m_address << ")");

  LlcSnapHeader llc;
  packet->RemoveHeader (llc);

  m_rxCallback (this, packet, llc.GetType (), header.GetSource ());
}



bool
LteNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << dest << protocolNumber);
  return SendFrom (packet, m_address, dest, protocolNumber);
}


bool
LteNetDevice::SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << source << dest << protocolNumber);

  LlcSnapHeader llcHdr;
  llcHdr.SetType (protocolNumber);
  packet->AddHeader (llcHdr);

  Mac48Address from = Mac48Address::ConvertFrom (source);
  Mac48Address to = Mac48Address::ConvertFrom (dest);

  LteMacHeader header;
  header.SetSource (from);
  header.SetDestination (to);
  packet->AddHeader (header);

  // m_macTxTrace (packet);

  return DoSend (packet, from, to, protocolNumber);
}


bool
LteNetDevice::SupportsSendFrom (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}


void
LteNetDevice::SetGenericPhyTxStartCallback (GenericPhyTxStartCallback c)
{
  NS_LOG_FUNCTION (this);
  m_phyMacTxStartCallback = c;
}


void
LteNetDevice::Receive (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
  Ptr<Packet> packet = p->Copy ();
  DoReceive (packet);
}



bool
LteNetDevice::SetMtu (const uint16_t mtu)
{
  NS_LOG_FUNCTION (this << mtu);
  if (mtu > MAX_MSDU_SIZE)
    {
      return false;
    }
  m_mtu = mtu;
  return true;
}

uint16_t
LteNetDevice::GetMtu (void) const
{
  NS_LOG_FUNCTION (this);
  return m_mtu;
}


void
LteNetDevice::SetIfIndex (const uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
  m_ifIndex = index;
}

uint32_t
LteNetDevice::GetIfIndex (void) const
{
  NS_LOG_FUNCTION (this);
  return m_ifIndex;
}


bool
LteNetDevice::IsLinkUp (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy != 0 && m_linkUp;
}


bool
LteNetDevice::IsBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

Address
LteNetDevice::GetBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  return Mac48Address::GetBroadcast ();
}

bool
LteNetDevice::IsMulticast (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}


bool
LteNetDevice::IsPointToPoint (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}


bool
LteNetDevice::NeedsArp (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}


bool
LteNetDevice::IsBridge (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

Address
LteNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  NS_LOG_FUNCTION (this);
  NS_LOG_FUNCTION (multicastGroup);

  Mac48Address ad = Mac48Address::GetMulticast (multicastGroup);

  //
  // Implicit conversion (operator Address ()) is defined for Mac48Address, so
  // use it by just returning the EUI-48 address which is automagically converted
  // to an Address.
  //
  NS_LOG_LOGIC ("multicast address is " << ad);

  return ad;
}

Address
LteNetDevice::GetMulticast (Ipv6Address addr) const
{
  NS_LOG_FUNCTION (this << addr);
  Mac48Address ad = Mac48Address::GetMulticast (addr);

  NS_LOG_LOGIC ("MAC IPv6 multicast address is " << ad);
  return ad;
}

void
LteNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
  NS_LOG_FUNCTION (this);
  m_linkChangeCallbacks.ConnectWithoutContext (callback);
}


void
LteNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  NS_LOG_FUNCTION (this);
  m_promiscRxCallback = cb;
}


void
LteNetDevice::SetPacketToSend (Ptr<PacketBurst> p)
{
  m_packetToSend = p;
}


Ptr<PacketBurst>
LteNetDevice::GetPacketToSend (void)
{
  return m_packetToSend;
}



}
