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
 * Author: Giuseppe Piro <g.piro@poliba.it>
 */


#include "enb-mac-entity.h"
#include <ns3/log.h>
#include <ns3/pointer.h>
#include <ns3/packet.h>
#include "packet-scheduler.h"
#include "simple-packet-scheduler.h"
#include "amc-module.h"
#include "ideal-control-messages.h"
#include "enb-net-device.h"
#include "ue-net-device.h"
#include "ue-record.h"
#include "ue-manager.h"

NS_LOG_COMPONENT_DEFINE ("EnbMacEntity");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (EnbMacEntity);

TypeId EnbMacEntity::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EnbMacEntity")
    .SetParent<MacEntity> ();
  return tid;
}


EnbMacEntity::EnbMacEntity ()
{
  SetAmcModule (CreateObject<AmcModule> ());
  m_downlinkScheduler = CreateObject<SimplePacketScheduler> ();
  m_downlinkScheduler->SetMacEntity (this);
  m_uplinkScheduler = 0;
}


EnbMacEntity::~EnbMacEntity ()
{
}

void
EnbMacEntity::DoDispose ()
{
  m_downlinkScheduler = 0;
  m_uplinkScheduler = 0;
  MacEntity::DoDispose ();
}


void
EnbMacEntity::SetUplinkPacketScheduler (Ptr<PacketScheduler> s)
{
  NS_LOG_FUNCTION (this << s);
  m_uplinkScheduler = s;
}


void
EnbMacEntity::SetDownlinkPacketScheduler (Ptr<PacketScheduler> s)
{
  NS_LOG_FUNCTION (this << s);
  m_downlinkScheduler = s;
}


Ptr<PacketScheduler>
EnbMacEntity::GetUplinkPacketScheduler (void)
{
  NS_LOG_FUNCTION (this);
  return m_uplinkScheduler;
}


Ptr<PacketScheduler>
EnbMacEntity::GetDownlinkPacketScheduler (void)
{
  NS_LOG_FUNCTION (this);
  return m_downlinkScheduler;
}


void
EnbMacEntity::ReceiveCqiIdealControlMessage  (Ptr<CqiIdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);
  NS_LOG_FUNCTION (this << msg->GetSourceDevice () << msg->GetDestinationDevice ());

  CqiIdealControlMessage::CqiFeedbacks *cqi = msg->GetMessage ();

  Ptr<UeNetDevice> ue = msg->GetSourceDevice ()->GetObject<UeNetDevice> ();
  Ptr<UeRecord> record = GetDevice ()->GetObject<EnbNetDevice> ()->GetUeManager ()->GetUeRecord (ue);
  UeRecord::CqiFeedbacks cqiFeedbacks;

  // STORE RECEIVED CQI FEEDBACKS INTO A PROPER UeRecord
  for (CqiIdealControlMessage::CqiFeedbacks::iterator it = cqi->begin (); it != cqi->end (); it++)
    {
      NS_LOG_FUNCTION (this << "cqi " << (*it).m_idSubChannel << (*it).m_cqi);
      UeRecord::CqiFeedback cqi;
      cqi.m_subChannelId = (*it).m_idSubChannel;
      cqi.m_cqi = static_cast<int> ((*it).m_cqi);
      cqiFeedbacks.push_back (cqi);
    }

  record->SetCqiFeedbacks (cqiFeedbacks);

}


void
EnbMacEntity::SendPdcchMapIdealControlMessage  (Ptr<PdcchMapIdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);

  std::vector< Ptr<UeRecord> >* ues = GetDevice ()->GetObject<EnbNetDevice> ()->GetUeManager ()->GetUeRecords ();
  std::vector< Ptr<UeRecord> >::iterator it;
  Ptr<UeNetDevice> ue;

  for (it = ues->begin (); it != ues->end (); it++)
    {
      ue = (*it)->GetUe ()->GetObject<UeNetDevice> ();

      ue->GetPhy ()->ReceiveIdealControlMessage (msg);
    }

}


} // namespacens3
