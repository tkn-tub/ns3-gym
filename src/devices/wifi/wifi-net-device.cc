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
#include "ns3/node.h"
#include "ns3/trace-source-accessor.h"

namespace ns3 {

TypeId 
WifiNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("WifiNetDevice")
    .SetParent<NetDevice> ()
    .AddAttribute ("Channel", "XXX",
                   Ptr<Channel> (0),
                   MakePtrAccessor (&WifiNetDevice::GetChannel),
                   MakePtrChecker<Channel> ())
    .AddAttribute ("Phy", "XXX",
                   Ptr<WifiPhy> (0),
                   MakePtrAccessor (&WifiNetDevice::m_phy),
                   MakePtrChecker<WifiPhy> ())
    .AddAttribute ("Mac", "XXX",
                   Ptr<WifiMac> (0),
                   MakePtrAccessor (&WifiNetDevice::m_mac),
                   MakePtrChecker<WifiMac> ())
    .AddAttribute ("RemoteStationManager", "XXX",
                   Ptr<WifiRemoteStationManager> (0),
                   MakePtrAccessor (&WifiNetDevice::m_stationManager),
                   MakePtrChecker<WifiRemoteStationManager> ())
    .AddTraceSource ("Rx", "XXX",
                     MakeTraceSourceAccessor (&WifiNetDevice::m_rxLogger))
    .AddTraceSource ("Tx", "XXX",
                     MakeTraceSourceAccessor (&WifiNetDevice::m_txLogger))
    ;
  return tid;
}

WifiNetDevice::WifiNetDevice ()
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
  m_stationManager = 0;
  // chain up.
  NetDevice::DoDispose ();
}
  
void 
WifiNetDevice::Setup (Ptr<Node> node, Ptr<WifiMac> mac, Ptr<WifiPhy> phy,
                      Ptr<WifiRemoteStationManager> manager,
                      Ptr<WifiChannel> channel)
{
  m_node = node;
  m_mac = mac;
  m_phy = phy;
  m_stationManager = manager;

  m_stationManager->SetupPhy (m_phy);

  m_mac->SetWifiRemoteStationManager (m_stationManager);
  m_mac->SetWifiPhy (m_phy);
  m_mac->SetForwardUpCallback (MakeCallback (&WifiNetDevice::ForwardUp, this));
  m_mac->SetLinkUpCallback (MakeCallback (&WifiNetDevice::LinkUp, this));
  m_mac->SetLinkDownCallback (MakeCallback (&WifiNetDevice::LinkDown, this));
  channel->Add (this, m_phy);

  m_phy->SetChannel (channel);
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
  return m_phy->GetChannel ();
}
Address 
WifiNetDevice::GetAddress (void) const
{
  return m_mac->GetAddress ();
}
bool 
WifiNetDevice::SetMtu (const uint16_t mtu)
{
  Uinteger maxMsduSize = m_mac->GetAttribute ("MaxMsduSize");
  if (mtu > maxMsduSize && mtu > 0)
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
      Uinteger maxMsduSize = m_mac->GetAttribute ("MaxMsduSize");
      m_mtu = maxMsduSize;
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
WifiNetDevice::GetMulticast (void) const
{
  return Mac48Address ("01:00:5e:00:00:00");
}
Address 
WifiNetDevice::MakeMulticastAddress (Ipv4Address multicastGroup) const
{
  return GetMulticast ();
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
WifiNetDevice::ForwardUp (Ptr<Packet> packet, const Mac48Address &from)
{
  LlcSnapHeader llc;
  packet->RemoveHeader (llc);
  m_forwardUp (this, packet, llc.GetType (), from);
  m_rxLogger (packet, from);
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

} // namespace ns3

