/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#include "wifi-net-device.h"
#include "wifi-mac.h"
#include "wifi-phy.h"
#include "wifi-remote-station-manager.h"
#include "wifi-channel.h"
#include "ns3/llc-snap-header.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/node.h"
#include "ns3/trace-source-accessor.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WifiNetDevice);

TypeId 
WifiNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiNetDevice")
    .SetParent<NetDevice> ()
    .AddAttribute ("Channel", "The channel attached to this device",
                   PointerValue (),
                   MakePointerAccessor (&WifiNetDevice::DoGetChannel,
                                        &WifiNetDevice::SetChannel),
                   MakePointerChecker<WifiChannel> ())
    .AddAttribute ("Phy", "The PHY layer attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&WifiNetDevice::GetPhy,
                                        &WifiNetDevice::SetPhy),
                   MakePointerChecker<WifiPhy> ())
    .AddAttribute ("Mac", "The MAC layer attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&WifiNetDevice::GetMac,
                                        &WifiNetDevice::SetMac),
                   MakePointerChecker<WifiMac> ())
    .AddAttribute ("RemoteStationManager", "The station manager attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&WifiNetDevice::SetRemoteStationManager,
                                        &WifiNetDevice::GetRemoteStationManager),
                   MakePointerChecker<WifiRemoteStationManager> ())
    .AddTraceSource ("Rx", "Received payload from the MAC layer.",
                     MakeTraceSourceAccessor (&WifiNetDevice::m_rxLogger))
    .AddTraceSource ("Tx", "Send payload to the MAC layer.",
                     MakeTraceSourceAccessor (&WifiNetDevice::m_txLogger))
    ;
  return tid;
}

WifiNetDevice::WifiNetDevice ()
  : m_mtu (0)
{}
WifiNetDevice::~WifiNetDevice ()
{}

void
WifiNetDevice::DoDispose (void)
{
  m_node = 0;
  m_mac->Dispose ();
  m_phy->Dispose ();
  m_stationManager->Dispose ();
  m_mac = 0;
  m_phy = 0;
  m_channel = 0;
  m_stationManager = 0;
  // chain up.
  NetDevice::DoDispose ();
}
  
void 
WifiNetDevice::SetMac (Ptr<WifiMac> mac)
{
  m_mac = mac;
  if (m_mac != 0)
    {
      if (m_stationManager != 0)
        {
          m_mac->SetWifiRemoteStationManager (m_stationManager);
        }
      if (m_phy != 0)
        {
          m_mac->SetWifiPhy (m_phy);
        }
      m_mac->SetForwardUpCallback (MakeCallback (&WifiNetDevice::ForwardUp, this));
      m_mac->SetLinkUpCallback (MakeCallback (&WifiNetDevice::LinkUp, this));
      m_mac->SetLinkDownCallback (MakeCallback (&WifiNetDevice::LinkDown, this));
    }
}
void 
WifiNetDevice::SetPhy (Ptr<WifiPhy> phy)
{
  m_phy = phy;
  if (m_phy != 0)
    {
      if (m_channel != 0)
        {
          m_channel->Add (this, m_phy);
          m_phy->SetChannel (m_channel);
        }
      if (m_stationManager != 0)
        {
          m_stationManager->SetupPhy (m_phy);
        }
      if (m_mac != 0)
        {
          m_mac->SetWifiPhy (m_phy);
        }
    }
}
void 
WifiNetDevice::SetRemoteStationManager (Ptr<WifiRemoteStationManager> manager)
{
  m_stationManager = manager;
  if (m_stationManager != 0)
    {
      if (m_phy != 0)
        {
          m_stationManager->SetupPhy (m_phy);
        }
      if (m_mac != 0)
        {
          m_mac->SetWifiRemoteStationManager (m_stationManager);
        }
    }
}
void 
WifiNetDevice::SetChannel (Ptr<WifiChannel> channel)
{
  m_channel = channel;
  if (m_channel != 0 && m_phy != 0)
    {
      m_channel->Add (this, m_phy);
      m_phy->SetChannel (m_channel);
    }
}
Ptr<WifiMac> 
WifiNetDevice::GetMac (void) const
{
  return m_mac;
}
Ptr<WifiPhy> 
WifiNetDevice::GetPhy (void) const
{
  return m_phy;
}
Ptr<WifiRemoteStationManager> 
WifiNetDevice::GetRemoteStationManager (void) const
{
  return m_stationManager;
}

void 
WifiNetDevice::SetName(const std::string name)
{
  m_name = name;
}
std::string 
WifiNetDevice::GetName(void) const
{
  return m_name;
}
void 
WifiNetDevice::SetIfIndex(const uint32_t index)
{
  m_ifIndex = index;
}
uint32_t 
WifiNetDevice::GetIfIndex(void) const
{
  return m_ifIndex;
}
Ptr<Channel> 
WifiNetDevice::GetChannel (void) const
{
  return m_channel;
}
Ptr<WifiChannel> 
WifiNetDevice::DoGetChannel (void) const
{
  return m_channel;
}
Address 
WifiNetDevice::GetAddress (void) const
{
  return m_mac->GetAddress ();
}
bool 
WifiNetDevice::SetMtu (const uint16_t mtu)
{
  UintegerValue maxMsduSize;
  m_mac->GetAttribute ("MaxMsduSize", maxMsduSize);
  if (mtu > maxMsduSize.Get () || mtu == 0)
    {
      return false;
    }
  m_mtu = mtu;
  return true;
}
uint16_t 
WifiNetDevice::GetMtu (void) const
{
  if (m_mtu == 0)
    {
      UintegerValue maxMsduSize;
      m_mac->GetAttribute ("MaxMsduSize", maxMsduSize);
      m_mtu = maxMsduSize.Get ();
    }
  return m_mtu;
}
bool 
WifiNetDevice::IsLinkUp (void) const
{
  return m_phy != 0 && m_linkUp;
}
void 
WifiNetDevice::SetLinkChangeCallback (Callback<void> callback)
{
  m_linkChange = callback;
}
bool 
WifiNetDevice::IsBroadcast (void) const
{
  return true;
}
Address 
WifiNetDevice::GetBroadcast (void) const
{
  return Mac48Address::GetBroadcast ();
}
bool 
WifiNetDevice::IsMulticast (void) const
{
  return false;
}
Address 
WifiNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  return Mac48Address::GetMulticast (multicastGroup);
}
Address WifiNetDevice::GetMulticast (Ipv6Address addr) const
{
  return Mac48Address::GetMulticast (addr);
}
bool 
WifiNetDevice::IsPointToPoint (void) const
{
  return false;
}
bool 
WifiNetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  NS_ASSERT (Mac48Address::IsMatchingType (dest));

  Mac48Address realTo = Mac48Address::ConvertFrom (dest);

  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  packet->AddHeader (llc);

  m_txLogger (packet, realTo);

  m_mac->Enqueue (packet, realTo);
  return true;
}
Ptr<Node> 
WifiNetDevice::GetNode (void) const
{
  return m_node;
}
void 
WifiNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}
bool 
WifiNetDevice::NeedsArp (void) const
{
  return true;
}
void 
WifiNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_forwardUp = cb;
}

void
WifiNetDevice::ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to)
{
  m_rxLogger (packet, from);
  LlcSnapHeader llc;
  packet->RemoveHeader (llc);
  enum NetDevice::PacketType type;
  if (to.IsBroadcast ())
    {
      type = NetDevice::PACKET_BROADCAST;
    }
  else if (to.IsMulticast ())
    {
      type = NetDevice::PACKET_MULTICAST;
    }
  else if (to == m_mac->GetAddress ())
    {
      type = NetDevice::PACKET_HOST;
    }
  else
    {
      type = NetDevice::PACKET_OTHERHOST;
    }
  if (type != NetDevice::PACKET_OTHERHOST)
    {
      m_forwardUp (this, packet, llc.GetType (), from);
    }
  if (!m_promiscRx.IsNull ())
    {
      m_promiscRx (this, packet, llc.GetType (), from, to, type);
    }
}

void
WifiNetDevice::LinkUp (void)
{
  m_linkUp = true;
  if (!m_linkChange.IsNull ())
    {
      m_linkChange ();
    }
}
void
WifiNetDevice::LinkDown (void)
{
  m_linkUp = false;
  if (!m_linkChange.IsNull ())
    {
      m_linkChange ();
    }
}

bool
WifiNetDevice::SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_ASSERT (Mac48Address::IsMatchingType (dest));
  NS_ASSERT (Mac48Address::IsMatchingType (source));

  Mac48Address realTo = Mac48Address::ConvertFrom (dest);
  Mac48Address realFrom = Mac48Address::ConvertFrom (source);

  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  packet->AddHeader (llc);

  m_txLogger (packet, realTo);

  m_mac->Enqueue (packet, realTo, realFrom);

  return true;
}

void
WifiNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  m_promiscRx = cb;
}

bool
WifiNetDevice::SupportsSendFrom (void) const
{
  return m_mac->SupportsSendFrom ();
}

} // namespace ns3

