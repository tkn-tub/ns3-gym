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
#include "ue-record.h"
#include "ue-manager.h"
#include "enb-net-device.h"
#include "ue-net-device.h"
#include "ue-mac-entity.h"
#include "rlc-entity.h"
#include "rrc-entity.h"
#include "lte-mac-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4.h"
#include "amc-module.h"
// #include "ideal-control-messages.h"

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
  InitUeNetDevice ();
}


UeNetDevice::UeNetDevice (Ptr<Node> node, Ptr<LtePhy> phy)
{
  NS_LOG_FUNCTION (this);
  InitUeNetDevice ();
  SetNode (node);
  SetPhy (phy);
}


UeNetDevice::UeNetDevice (Ptr<Node> node, Ptr<LtePhy> phy, Ptr<EnbNetDevice> targetEnb)
{
  NS_LOG_FUNCTION (this << node << phy << targetEnb);
  InitUeNetDevice ();
  SetNode (node);
  SetPhy (phy);
  m_targetEnb = targetEnb;
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
  m_macEntity->Dispose ();
  m_macEntity = 0;
  LteNetDevice::DoDispose ();
}

void
UeNetDevice::InitUeNetDevice (void)
{
  NS_LOG_FUNCTION (this);
  m_targetEnb = 0;
  SetNode (0);
  SetPhy (0);
  m_macEntity = CreateObject<UeMacEntity> ();
  m_macEntity->SetDevice (this);
  SetRrcEntity (CreateObject<RrcEntity> ());
}


void
UeNetDevice::SetMacEntity (Ptr<UeMacEntity> m)
{
  NS_LOG_FUNCTION (this);
  m_macEntity = m;
}


Ptr<UeMacEntity>
UeNetDevice::GetMacEntity (void)
{
  NS_LOG_FUNCTION (this);
  return m_macEntity;
}

void
UeNetDevice::Start (void)
{
  NS_LOG_FUNCTION (this);
}


void
UeNetDevice::Stop (void)
{
  NS_LOG_FUNCTION (this);
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
  // Ptr<RadioBearerInstance> bearer = GetIpClassifier ()->Classify (packet);
  // return Enqueue (packet, bearer);

  /*
   * XXX: the uplink is not implemented yet.
   * For now the UE send the packet as soon as
   * it arrives from the upper layer
   *
   * For any question, please contact me at g.piro@poliba.it
   */

  Ptr<PacketBurst> pb = CreateObject<PacketBurst> ();
  pb->AddPacket (packet);
  return SendPacket (pb);
}


void
UeNetDevice::DoReceive (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);

  Ptr<Packet> packet = p->Copy ();

  LteMacHeader header;
  packet->RemoveHeader (header);
  NS_LOG_LOGIC ("packet " << header.GetSource () << " --> " << header.GetDestination () <<
                " (here: " << Mac48Address::ConvertFrom (GetAddress ()) << ")");

  if (header.GetDestination () == GetAddress () || header.GetDestination () == GetBroadcast ())
    {
      LlcSnapHeader llcHdr;
      packet->RemoveHeader (llcHdr);
      NS_LOG_FUNCTION (this << llcHdr);

      ForwardUp (p->Copy ());
    }
  else
    {
      // not for me
    }

}


void
UeNetDevice::StartTransmission (void)
{
  NS_LOG_FUNCTION (this);
  GetPhy ()->SendPacket (GetPacketToSend ());
}


bool
UeNetDevice::SendPacket (Ptr<PacketBurst> p)
{
  return GetPhy ()->GetUplinkSpectrumPhy ()->StartTx (p);
}
} // namespace ns3
