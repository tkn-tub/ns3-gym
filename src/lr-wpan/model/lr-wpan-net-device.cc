/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 * Author:  Tom Henderson <thomas.r.henderson@boeing.com>
 */

#include "ns3/abort.h"
#include "ns3/node.h"
#include "lr-wpan-net-device.h"
#include "ns3/log.h"
#include "ns3/lr-wpan-mac.h"
#include "ns3/lr-wpan-phy.h"
#include "ns3/lr-wpan-csmaca.h"
#include "ns3/spectrum-channel.h"
#include "ns3/pointer.h"

NS_LOG_COMPONENT_DEFINE ("LrWpanNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LrWpanNetDevice);

TypeId
LrWpanNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LrWpanNetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<LrWpanNetDevice> ()
    .AddAttribute ("Channel", "The channel attached to this device",
                   PointerValue (),
                   MakePointerAccessor (&LrWpanNetDevice::DoGetChannel),
                   MakePointerChecker<SpectrumChannel> ())
    .AddAttribute ("Phy", "The PHY layer attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&LrWpanNetDevice::GetPhy,
                                        &LrWpanNetDevice::SetPhy),
                   MakePointerChecker<LrWpanPhy> ())
    .AddAttribute ("Mac", "The MAC layer attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&LrWpanNetDevice::GetMac,
                                        &LrWpanNetDevice::SetMac),
                   MakePointerChecker<LrWpanMac> ())
  ;
  return tid;
}

LrWpanNetDevice::LrWpanNetDevice ()
  : m_configComplete (false)
{
  NS_LOG_FUNCTION (this);
  m_mac = CreateObject<LrWpanMac> ();
  m_phy = CreateObject<LrWpanPhy> ();
  m_csmaca = CreateObject<LrWpanCsmaCa> ();
  CompleteConfig ();
}

LrWpanNetDevice::~LrWpanNetDevice ()
{
  NS_LOG_FUNCTION (this);
}


void
LrWpanNetDevice::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_mac->Dispose ();
  m_phy->Dispose ();
  m_csmaca->Dispose ();
  m_phy = 0;
  m_mac = 0;
  m_csmaca = 0;
  m_node = 0;
  // chain up.
  NetDevice::DoDispose ();

}

void
LrWpanNetDevice::DoStart (void)
{
  NS_LOG_FUNCTION (this);
  m_phy->Start ();
  m_mac->Start ();
  NetDevice::DoStart ();
}


void
LrWpanNetDevice::CompleteConfig (void)
{
  NS_LOG_FUNCTION (this);
  if (m_mac == 0
      || m_phy == 0
      || m_csmaca == 0
      || m_node == 0
      || m_configComplete)
    {
      return;
    }
  m_mac->SetPhy (m_phy);
  m_mac->SetCsmaCa (m_csmaca);
  m_csmaca->SetMac (m_mac);

  m_phy->SetMobility (m_node->GetObject<MobilityModel> ());
  Ptr<LrWpanErrorModel> model = CreateObject<LrWpanErrorModel> ();
  m_phy->SetErrorModel (model);

  m_phy->SetPdDataIndicationCallback (MakeCallback (&LrWpanMac::PdDataIndication, m_mac));
  m_phy->SetPdDataConfirmCallback (MakeCallback (&LrWpanMac::PdDataConfirm, m_mac));
  m_phy->SetPlmeEdConfirmCallback (MakeCallback (&LrWpanMac::PlmeEdConfirm, m_mac));
  m_phy->SetPlmeGetAttributeConfirmCallback (MakeCallback (&LrWpanMac::PlmeGetAttributeConfirm, m_mac));
  m_phy->SetPlmeSetTRXStateConfirmCallback (MakeCallback (&LrWpanMac::PlmeSetTRXStateConfirm, m_mac));
  m_phy->SetPlmeSetAttributeConfirmCallback (MakeCallback (&LrWpanMac::PlmeSetAttributeConfirm, m_mac));

  m_csmaca->SetLrWpanMacStateCallback (MakeCallback (&LrWpanMac::SetLrWpanMacState, m_mac));
  m_phy->SetPlmeCcaConfirmCallback (MakeCallback (&LrWpanCsmaCa::PlmeCcaConfirm, m_csmaca));
  m_configComplete = true;
}

void
LrWpanNetDevice::SetMac (Ptr<LrWpanMac> mac)
{
  NS_LOG_FUNCTION (this);
  m_mac = mac;
  CompleteConfig ();
}

void
LrWpanNetDevice::SetPhy (Ptr<LrWpanPhy> phy)
{
  NS_LOG_FUNCTION (this);
  m_phy = phy;
  CompleteConfig ();
}

void
LrWpanNetDevice::SetCsmaCa (Ptr<LrWpanCsmaCa> csmaca)
{
  NS_LOG_FUNCTION (this);
  m_csmaca = csmaca;
  CompleteConfig ();
}

void
LrWpanNetDevice::SetChannel (Ptr<SpectrumChannel> channel)
{
  NS_LOG_FUNCTION (this << channel);
  m_phy->SetChannel (channel);
  channel->AddRx (m_phy);
  CompleteConfig ();
}

Ptr<LrWpanMac>
LrWpanNetDevice::GetMac (void) const
{
  NS_LOG_FUNCTION (this);
  return m_mac;
}

Ptr<LrWpanPhy>
LrWpanNetDevice::GetPhy (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy;
}

Ptr<LrWpanCsmaCa>
LrWpanNetDevice::GetCsmaCa (void) const
{
  NS_LOG_FUNCTION (this);
  return m_csmaca;
}
void
LrWpanNetDevice::SetIfIndex (const uint32_t index)
{
  NS_LOG_FUNCTION (this << index);
  m_ifIndex = index;
}

uint32_t
LrWpanNetDevice::GetIfIndex (void) const
{
  NS_LOG_FUNCTION (this);
  return m_ifIndex;
}

Ptr<Channel>
LrWpanNetDevice::GetChannel (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy->GetChannel ();
}

void
LrWpanNetDevice::LinkUp (void)
{
  NS_LOG_FUNCTION (this);
  m_linkUp = true;
  m_linkChanges ();
}
void
LrWpanNetDevice::LinkDown (void)
{
  NS_LOG_FUNCTION (this);
  m_linkUp = false;
  m_linkChanges ();
}


Ptr<SpectrumChannel>
LrWpanNetDevice::DoGetChannel (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy->GetChannel ();
}
void
LrWpanNetDevice::SetAddress (Address address)
{
  NS_LOG_FUNCTION (this);
  m_mac->SetShortAddress (Mac16Address::ConvertFrom (address));
}

Address
LrWpanNetDevice::GetAddress (void) const
{
  NS_LOG_FUNCTION (this);
  return m_mac->GetShortAddress ();
}

bool
LrWpanNetDevice::SetMtu (const uint16_t mtu)
{
  NS_ABORT_MSG ("Unsupported");
  return false;
}

uint16_t
LrWpanNetDevice::GetMtu (void) const
{
  NS_ABORT_MSG ("Unsupported");
  return 0;
}

bool
LrWpanNetDevice::IsLinkUp (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy != 0 && m_linkUp;
}

void
LrWpanNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
  NS_LOG_FUNCTION (this);
  m_linkChanges.ConnectWithoutContext (callback);
}

bool
LrWpanNetDevice::IsBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  return true;
}

Address
LrWpanNetDevice::GetBroadcast (void) const
{
  NS_ABORT_MSG ("Unsupported; add me");
  return Address ();
}

bool
LrWpanNetDevice::IsMulticast (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

Address
LrWpanNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  NS_ABORT_MSG ("Unsupported");
  return Address ();
}

Address
LrWpanNetDevice::GetMulticast (Ipv6Address addr) const
{
  NS_ABORT_MSG ("Unsupported");
  return Address ();
}

bool
LrWpanNetDevice::IsBridge (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

bool
LrWpanNetDevice::IsPointToPoint (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

bool
LrWpanNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  // This method basically assumes an 802.3-compliant device, but a raw
  // 802.15.4 device does not have an ethertype, and requires specific
  // McpsDataRequest parameters.
  // For further study:  how to support these methods somehow, such as
  // inventing a fake ethertype and packet tag for McpsDataRequest
  NS_ABORT_MSG ("Unsupported; use McpsDataRequest instead");
  return false;
}

bool
LrWpanNetDevice::SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_ABORT_MSG ("Unsupported; use McpsDataRequest instead");
  return false;
}

Ptr<Node>
LrWpanNetDevice::GetNode (void) const
{
  NS_LOG_FUNCTION (this);
  return m_node;
}

void
LrWpanNetDevice::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this);
  m_node = node;
  CompleteConfig ();
}

bool
LrWpanNetDevice::NeedsArp (void) const
{
  NS_ABORT_MSG ("Unsupported");
  return false;
}

void
LrWpanNetDevice::SetReceiveCallback (ReceiveCallback cb)
{
  // This method basically assumes an 802.3-compliant device, but a raw
  // 802.15.4 device does not have an ethertype, and requires specific
  // McpsDataIndication parameters.
  // For further study:  how to support these methods somehow, such as
  // inventing a fake ethertype and packet tag for McpsDataRequest
  NS_LOG_WARN ("Unsupported; use LrWpan MAC APIs instead");
}

void
LrWpanNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  // This method basically assumes an 802.3-compliant device, but a raw
  // 802.15.4 device does not have an ethertype, and requires specific
  // McpsDataIndication parameters.
  // For further study:  how to support these methods somehow, such as
  // inventing a fake ethertype and packet tag for McpsDataRequest
  NS_LOG_WARN ("Unsupported; use LrWpan MAC APIs instead");
}

bool
LrWpanNetDevice::SupportsSendFrom (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return false;
}

} // namespace ns3
