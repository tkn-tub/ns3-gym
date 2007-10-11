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
#include "ns3/packet.h"
#include "ns3/llc-snap-header.h"
#include "ns3/node.h"

#include "wifi-net-device.h"
#include "wifi-phy.h"
#include "wifi-channel.h"
#include "mac-stations.h"
#include "mac-low.h"
#include "mac-parameters.h"
#include "mac-rx-middle.h"
#include "mac-tx-middle.h"
#include "mac-high-adhoc.h"
#include "mac-high-nqsta.h"
#include "mac-high-nqap.h"
#include "dca-txop.h"

namespace ns3 {

WifiNetDevice::WifiNetDevice (Ptr<Node> node)
  : NetDevice (node, Mac48Address::Allocate ())
{
  SetMtu (2300);
  EnableBroadcast (Mac48Address ("ff:ff:ff:ff:ff:ff"));
}

WifiNetDevice::~WifiNetDevice ()
{
  delete m_phy;
  delete m_stations;
  delete m_low;
  delete m_parameters;
  delete m_txMiddle;
  delete m_rxMiddle;
}

void 
WifiNetDevice::ConnectTo (Ptr<WifiChannel> channel)
{
  m_phy->SetChannel (channel);
  NotifyConnected ();
}
bool
WifiNetDevice::SendTo (const Packet &packet, const Address &to, uint16_t protocolNumber)
{
  NS_ASSERT (Mac48Address::IsMatchingType (to));

  Mac48Address realTo = Mac48Address::ConvertFrom (to);

  Packet p = packet;

  LlcSnapHeader llc;
  llc.SetType (protocolNumber);
  p.AddHeader (llc);

  m_txLogger (p, realTo);

  return DoSendTo (p, realTo);
}
void 
WifiNetDevice::DoForwardUp (Packet packet, const Mac48Address &from)
{
  m_rxLogger (packet, from);

  LlcSnapHeader llc;
  packet.RemoveHeader (llc);
  NetDevice::ForwardUp (packet, llc.GetType (), from);
}
Mac48Address 
WifiNetDevice::GetSelfAddress (void) const
{
  NS_ASSERT (Mac48Address::IsMatchingType (GetAddress ()));
  Mac48Address self = Mac48Address::ConvertFrom (GetAddress ());
  return self;
}

/*****************************************************
 *            Adhoc code
 *****************************************************/

AdhocWifiNetDevice::AdhocWifiNetDevice (Ptr<Node> node)
  : WifiNetDevice (node)
{}
AdhocWifiNetDevice::~AdhocWifiNetDevice ()
{
  delete m_dca;
  delete m_high;
}
Mac48Address 
AdhocWifiNetDevice::GetBssid (void) const
{
  return m_high->GetBssid ();
}
Ssid 
AdhocWifiNetDevice::GetSsid (void) const
{
  return m_ssid;
}
void 
AdhocWifiNetDevice::SetSsid (Ssid ssid)
{
  // XXX restart adhoc network join.
  m_ssid = ssid;
}
bool
AdhocWifiNetDevice::DoSendTo (const Packet &packet, Mac48Address const &to)
{
  m_high->Enqueue (packet, to);
  return true;
}
void
AdhocWifiNetDevice::NotifyConnected (void)
{
  NotifyLinkUp ();
}

/*****************************************************
 *            STA code
 *****************************************************/

NqstaWifiNetDevice::NqstaWifiNetDevice (Ptr<Node> node)
  : WifiNetDevice (node)
{}
NqstaWifiNetDevice::~NqstaWifiNetDevice ()
{
  delete m_dca;
  delete m_high;
}
Mac48Address 
NqstaWifiNetDevice::GetBssid (void) const
{
  return m_high->GetBssid ();
}
Ssid 
NqstaWifiNetDevice::GetSsid (void) const
{
  return m_ssid;
}
void 
NqstaWifiNetDevice::StartActiveAssociation (Ssid ssid)
{
  m_ssid = ssid;
  m_high->StartActiveAssociation ();
}
bool
NqstaWifiNetDevice::DoSendTo (const Packet &packet, Mac48Address const &to)
{
  m_high->Queue (packet, to);
  return true;
}
void
NqstaWifiNetDevice::NotifyConnected (void)
{
  // do nothing because link status is kept track of in
  // ::Associated and ::Disassociated
}
void 
NqstaWifiNetDevice::Associated (void)
{
  NetDevice::NotifyLinkUp ();
}

void 
NqstaWifiNetDevice::DisAssociated (void)
{
  NetDevice::NotifyLinkDown ();
}


/*****************************************************
 *            AP code
 *****************************************************/


NqapWifiNetDevice::NqapWifiNetDevice (Ptr<Node> node)
  : WifiNetDevice (node)
{}
NqapWifiNetDevice::~NqapWifiNetDevice ()
{
  delete m_dca;
  delete m_high;
}
Mac48Address 
NqapWifiNetDevice::GetBssid (void) const
{
  return GetSelfAddress ();
}
Ssid 
NqapWifiNetDevice::GetSsid (void) const
{
  return m_ssid;
}
void 
NqapWifiNetDevice::SetSsid (Ssid ssid)
{
  m_ssid = ssid;
}
bool
NqapWifiNetDevice::DoSendTo (const Packet &packet, Mac48Address const & to)
{
  m_high->Queue (packet, to);
  return true;
}
void
NqapWifiNetDevice::NotifyConnected (void)
{
  NotifyLinkUp ();
}


} // namespace ns3

