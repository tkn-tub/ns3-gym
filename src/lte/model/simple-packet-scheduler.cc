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


#include <ns3/log.h>
#include <ns3/pointer.h>
#include "ue-net-device.h"
#include "enb-net-device.h"
#include "simple-packet-scheduler.h"
#include "enb-net-device.h"
#include "rlc-entity.h"
#include "rrc-entity.h"
#include "lte-mac-header.h"
#include "ns3/mac48-address.h"
#include "enb-mac-entity.h"
#include "ue-manager.h"
#include "ue-record.h"
#include "amc-module.h"

NS_LOG_COMPONENT_DEFINE ("SimplePacketScheduler");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (SimplePacketScheduler);

SimplePacketScheduler::SimplePacketScheduler ()
{
  NS_LOG_FUNCTION (this);
}


SimplePacketScheduler::SimplePacketScheduler (Ptr<EnbNetDevice> enb)
  : PacketScheduler (enb)
{
  NS_LOG_FUNCTION (this << enb);
}


TypeId
SimplePacketScheduler::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SimplePacketScheduler")
    .SetParent<PacketScheduler> ()
    .AddConstructor<SimplePacketScheduler> ()
  ;
  return tid;
}


SimplePacketScheduler::~SimplePacketScheduler ()
{
  NS_LOG_FUNCTION (this);
}


void
SimplePacketScheduler::DoRunPacketScheduler (void)
{
  NS_LOG_FUNCTION (this);
  Ptr<EnbNetDevice> enb = GetMacEntity ()->GetDevice ()->GetObject<EnbNetDevice> ();


  Ptr<PacketBurst> pb = CreateObject<PacketBurst> ();
  Ptr<PdcchMapIdealControlMessage> msg = Create<PdcchMapIdealControlMessage> ();


  //This scheduler takes one packet from each bearer

  std::vector< Ptr<RadioBearerInstance> >::iterator it;

  //default bearer
  if (!enb->GetRrcEntity ()->GetDefaultBearer ()->GetQueue ()->IsEmpty ())
    {
      Ptr<Packet> p = enb->GetRrcEntity ()->GetDefaultBearer ()->GetRlcEntity ()->Dequeue ();
      pb->AddPacket (p);
    }

  // GBR bearers
  for (it = enb->GetRrcEntity ()->GetDownlinkGbrBearers ()->begin (); 
       it != enb->GetRrcEntity ()->GetDownlinkGbrBearers ()->end (); it++)
    {
      if (!(*it)->GetQueue ()->IsEmpty ())
        {
          // Get the first packet from the queue.
          Ptr<Packet> p = (*it)->GetRlcEntity ()->Dequeue ();
          pb->AddPacket (p);
        }
    }

  // NGBR bearers
  for (it = enb->GetRrcEntity ()->GetDownlinkNgbrBearers ()->begin (); 
       it != enb->GetRrcEntity ()->GetDownlinkNgbrBearers ()->end (); it++)
    {
      if (!(*it)->GetQueue ()->IsEmpty ())
        {
          // Get the first packet form the queue.
          Ptr<Packet> p = (*it)->GetRlcEntity ()->Dequeue ();
          pb->AddPacket (p);
        }
    }


  if (pb->GetNPackets () == 0)
    {
      // The eNB Sends a virtual packet in order to allow UEs to compute SINR
      Ptr<PacketBurst> pb = CreateObject<PacketBurst> ();
      Ptr<Packet> p = Create<Packet> (1);

      Mac48Address from = Mac48Address::ConvertFrom (enb->GetAddress ());
      Mac48Address to = Mac48Address::ConvertFrom (enb->GetAddress ());

      LteMacHeader header;
      header.SetSource (from);
      header.SetDestination (to);
      p->AddHeader (header);

      pb->AddPacket (p);
    }


  enb->SetPacketToSend (pb);

  GetMacEntity ()->GetObject<EnbMacEntity> ()->SendPdcchMapIdealControlMessage (msg);

  enb->StartTransmission ();
}


} // namespace ns3
