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
#include "ue-record.h"
#include "ue-manager.h"
#include "enb-mac-entity.h"
#include "enb-net-device.h"
#include "packet-scheduler.h"
#include "rlc-entity.h"
#include "rrc-entity.h"
#include "lte-mac-header.h"
#include "ue-net-device.h"
#include "enb-phy.h"

NS_LOG_COMPONENT_DEFINE ("EnbNetDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED ( EnbNetDevice);

TypeId EnbNetDevice::GetTypeId (void)
{
  static TypeId
    tid =
    TypeId ("ns3::EnbNetDevice")
    .SetParent<LteNetDevice> ();
  return tid;
}

EnbNetDevice::EnbNetDevice (void)
{
  NS_LOG_FUNCTION (this);
  InitEnbNetDevice ();
}

EnbNetDevice::EnbNetDevice (Ptr<Node> node, Ptr<LtePhy> phy)
{
  NS_LOG_FUNCTION (this);
  InitEnbNetDevice ();
  SetNode (node);
  SetPhy (phy);
}

EnbNetDevice::~EnbNetDevice (void)
{
  NS_LOG_FUNCTION (this);
}

void
EnbNetDevice::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_ueManager->Dispose ();
  m_ueManager = 0;

  m_macEntity->Dispose ();
  m_macEntity = 0;

  LteNetDevice::DoDispose ();
}


void
EnbNetDevice::InitEnbNetDevice (void)
{
  NS_LOG_FUNCTION (this);
  SetRrcEntity (CreateObject<RrcEntity> ());
  m_ueManager = CreateObject<UeManager> ();
  m_macEntity = CreateObject<EnbMacEntity> ();
  m_macEntity->SetDevice (this->GetObject<LteNetDevice> ());
  SetNode (0);
  SetPhy (0);
}

void
EnbNetDevice::Start (void)
{
  NS_LOG_FUNCTION (this);
  Simulator::ScheduleNow (&EnbLtePhy::StartFrame, GetPhy ()->GetObject<EnbLtePhy> ());
}


void
EnbNetDevice::Stop (void)
{
  NS_LOG_FUNCTION (this);
}


void
EnbNetDevice::SetUeManager (Ptr<UeManager> m)
{
  NS_LOG_FUNCTION (this);
  m_ueManager = m;
}


Ptr<UeManager>
EnbNetDevice::GetUeManager (void)
{
  NS_LOG_FUNCTION (this);
  return m_ueManager;
}


void
EnbNetDevice::SetMacEntity (Ptr<EnbMacEntity> m)
{
  NS_LOG_FUNCTION (this);
  m_macEntity = m;
}


Ptr<EnbMacEntity>
EnbNetDevice::GetMacEntity (void)
{
  NS_LOG_FUNCTION (this);
  return m_macEntity;
}

bool
EnbNetDevice::DoSend (Ptr<Packet> packet, const Mac48Address& source,
                      const Mac48Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << source << dest << protocolNumber);

  /*
   * The classification of traffic in DL is done by the PGW (not
   * by the eNB).
   * Hovever, the core network is not implemented yet.
   * For now the classification is managed by the eNB.
   */
  //Ptr<RadioBearerInstance> bearer = GetRrcEntity ()->GetDefaultBearer (); 

  Ptr<RadioBearerInstance> bearer;

  if (protocolNumber == 2048)
    {
      // it is an IP packet
      bearer = GetRrcEntity ()->Classify (packet);
    }

  if (protocolNumber != 2048 || bearer == 0)
    {
      bearer = GetRrcEntity ()->GetDefaultBearer ();
    }

  return bearer->Enqueue (packet);
} 


void
EnbNetDevice::DoReceive (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
  ForwardUp (p->Copy ());
}


void
EnbNetDevice::StartTransmission (void)
{
  NS_LOG_FUNCTION (this);
  GetPhy ()->SendPacket (GetPacketToSend ());
}


bool
EnbNetDevice::SendPacket (Ptr<PacketBurst> p)
{
  NS_LOG_FUNCTION (this);
  return GetPhy ()->GetDownlinkSpectrumPhy ()->StartTx (p);
}


void
EnbNetDevice::SendIdealPdcchMessage (void)
{
  NS_LOG_FUNCTION (this);
  /*
   * Get both PDCCH ideal message for UL and DL and
   * set assigned resources to UEs using
   * SendAssignedDLResources and SendAssignedULResources
   */
}


} // namespace ns3
