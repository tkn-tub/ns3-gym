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
#include "lte-enb-net-device.h"
#include "lte-ue-net-device.h"
#include "lte-ue-mac.h"
#include "lte-ue-rrc.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4.h"
#include "lte-amc.h"
#include <ns3/lte-ue-phy.h>
#include <ns3/ipv4-l3-protocol.h>
#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE ("LteUeNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED ( LteUeNetDevice);

uint64_t LteUeNetDevice::m_imsiCounter = 0;


TypeId LteUeNetDevice::GetTypeId (void)
{
  static TypeId
    tid =
    TypeId ("ns3::LteUeNetDevice")
    .SetParent<LteNetDevice> ()
    .AddAttribute ("LteUeRrc",
                   "The RRC associated to this UeNetDevice",
                   PointerValue (),
                   MakePointerAccessor (&LteUeNetDevice::m_rrc),
                   MakePointerChecker <LteUeRrc> ())
    .AddAttribute ("LteUeMac",
                   "The MAC associated to this UeNetDevice",
                   PointerValue (),
                   MakePointerAccessor (&LteUeNetDevice::m_mac),
                   MakePointerChecker <LteUeMac> ())
    .AddAttribute ("LteUePhy",
                   "The PHY associated to this UeNetDevice",
                   PointerValue (),
                   MakePointerAccessor (&LteUeNetDevice::m_phy),
                   MakePointerChecker <LteUePhy> ())
/*    .AddAttribute ("Imsi",
                   "International Mobile Subscriber Identity assigned to this UE",
                   TypeId::ATTR_GET,
                   UintegerValue (0), // not used because the attribute is read-only
                   MakeUintegerAccessor (&LteUeNetDevice::m_imsi),
                   MakeUintegerChecker<uint64_t> ())*/
  ;

  return tid;
}


LteUeNetDevice::LteUeNetDevice (void)
{
  NS_LOG_FUNCTION (this);
  NS_FATAL_ERROR ("This constructor should not be called");
}


LteUeNetDevice::LteUeNetDevice (Ptr<Node> node, Ptr<LteUePhy> phy, Ptr<LteUeMac> mac, Ptr<LteUeRrc> rrc)
{
  NS_LOG_FUNCTION (this);
  m_phy = phy;
  m_mac = mac;
  m_rrc = rrc;
  SetNode (node);
  m_imsi = ++m_imsiCounter;
}

LteUeNetDevice::~LteUeNetDevice (void)
{
  NS_LOG_FUNCTION (this);
}

void
LteUeNetDevice::DoDispose (void)
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
LteUeNetDevice::UpdateConfig (void)
{
  NS_LOG_FUNCTION (this);
}



Ptr<LteUeMac>
LteUeNetDevice::GetMac (void)
{
  NS_LOG_FUNCTION (this);
  return m_mac;
}


Ptr<LteUeRrc>
LteUeNetDevice::GetRrc (void)
{
  NS_LOG_FUNCTION (this);
  return m_rrc;
}


Ptr<LteUePhy>
LteUeNetDevice::GetPhy (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phy;
}

void
LteUeNetDevice::SetTargetEnb (Ptr<LteEnbNetDevice> enb)
{
  NS_LOG_FUNCTION (this << enb);
  m_targetEnb = enb;

  // should go through RRC and then through PHY SAP
  m_phy->DoSetCellId (enb->GetCellId ());
}


Ptr<LteEnbNetDevice>
LteUeNetDevice::GetTargetEnb (void)
{
  NS_LOG_FUNCTION (this);
  return m_targetEnb;
}

uint64_t
LteUeNetDevice::GetImsi ()
{
  NS_LOG_FUNCTION (this);
  return m_imsi;
}


void 
LteUeNetDevice::DoStart (void)
{
  NS_LOG_FUNCTION (this);
  UpdateConfig ();
  m_phy->Start ();
  m_mac->Start ();
  m_rrc->Start ();
}

bool
LteUeNetDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << dest << protocolNumber);
  NS_ASSERT_MSG (protocolNumber == Ipv4L3Protocol::PROT_NUMBER, "unsupported protocol " << protocolNumber << ", only IPv4 is supported");
  
  return m_rrc->Send (packet);
}


} // namespace ns3
