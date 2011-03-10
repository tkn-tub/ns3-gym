/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 *         Nicola Baldo <nbaldo@cttc.es>
 *         Marco Miozzo <mmiozzo@cttc.es>
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
#include "enb-net-device.h"
#include "ue-net-device.h"
#include "lte-ue-mac.h"
#include "lte-ue-rrc.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4.h"
#include "amc-module.h"
// #include "ideal-control-messages.h"
#include <ns3/ue-phy.h>

NS_LOG_COMPONENT_DEFINE ("UeNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED ( UeNetDevice);


TypeId UeNetDevice::GetTypeId (void)
{
  static TypeId
  tid =
    TypeId ("ns3::UeNetDevice")
    .SetParent<LteNetDevice> ();
  return tid;
}


UeNetDevice::UeNetDevice (void)
{
  NS_LOG_FUNCTION (this);
  NS_FATAL_ERROR ("This constructor should not be called");
  InitUeNetDevice ();
}


UeNetDevice::UeNetDevice (Ptr<Node> node, Ptr<UeLtePhy> phy)
  : m_phy (phy)
{
  NS_LOG_FUNCTION (this);
  InitUeNetDevice ();
  SetNode (node);

  /**
  * WILD HACK
  * to be translated to PHY-SAP primitive
  */
  phy->DoSetBandwidth (25,25);
}

UeNetDevice::~UeNetDevice (void)
{
  NS_LOG_FUNCTION (this);
}

void
UeNetDevice::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_targetEnb = 0;
  m_mac->Dispose ();
  m_mac = 0;
  m_rrc->Dispose ();
  m_rrc = 0;
  m_phy->Dispose ();
  m_phy = 0;
  LteNetDevice::DoDispose ();
}

void
UeNetDevice::InitUeNetDevice (void)
{
  NS_LOG_FUNCTION (this);
  m_targetEnb = 0;
  SetNode (0);
  m_mac = CreateObject<LteUeMac> ();
  m_rrc = Create<LteUeRrc> ();
  m_rrc->SetLteUeCmacSapProvider (m_mac->GetLteUeCmacSapProvider ());
  m_mac->SetLteUeCmacSapUser (m_rrc->GetLteUeCmacSapUser ());
  m_rrc->SetLteMacSapProvider (m_mac->GetLteMacSapProvider ());
  m_phy->SetLteUePhySapUser (m_mac->GetLteUePhySapUser ());
  m_mac->SetLteUePhySapProvider (m_phy->GetLteUePhySapProvider ());
}



Ptr<LteUeMac>
UeNetDevice::GetMac (void)
{
  NS_LOG_FUNCTION (this);
  return m_mac;
}


Ptr<LteUeRrc>
UeNetDevice::GetRrc (void)
{
  NS_LOG_FUNCTION (this);
  return m_rrc;
}


Ptr<UeLtePhy>
UeNetDevice::GetPhy (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy;
}

void
UeNetDevice::SetTargetEnb (Ptr<EnbNetDevice> enb)
{
  NS_LOG_FUNCTION (this << enb);
  m_targetEnb = enb;
}


Ptr<EnbNetDevice>
UeNetDevice::GetTargetEnb (void)
{
  NS_LOG_FUNCTION (this);
  return m_targetEnb;
}


bool
UeNetDevice::DoSend (Ptr<Packet> packet, const Mac48Address& source,
                     const Mac48Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this);

  NS_FATAL_ERROR ("IP connectivity not implemented yet");

  return (true);
}


void
UeNetDevice::DoReceive (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);

  NS_FATAL_ERROR ("IP connectivity not implemented yet");

  Ptr<Packet> packet = p->Copy ();

  LlcSnapHeader llcHdr;
  packet->RemoveHeader (llcHdr);
  NS_LOG_FUNCTION (this << llcHdr);

  ForwardUp (packet);
}


} // namespace ns3
