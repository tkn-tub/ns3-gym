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


#include "radio-bearer-instance.h"
#include "bearer-qos-parameters.h"
#include "ns3/ipcs-classifier-record.h"
#include "rlc-entity.h"
#include <ns3/log.h>
#include <ns3/pointer.h>

NS_LOG_COMPONENT_DEFINE ("RadioBearerInstance");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (RadioBearerInstance);

TypeId RadioBearerInstance::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RadioBearerInstance")
    .SetParent<Object> ()
    .AddAttribute ("TxQueue",
                   "Transmit queue",
                   PointerValue (),
                   MakePointerAccessor (&RadioBearerInstance::GetQueue),
                   MakePointerChecker<LteMacQueue> ());
  return tid;
}


RadioBearerInstance::RadioBearerInstance ()
  : m_queue (CreateObject<LteMacQueue> (1024)),
    m_qosParameters (0),
    m_rlcEntity (CreateObject<RlcEntity> ()),
    m_ipcsClassifierRecord (0)
{
  NS_LOG_FUNCTION (this);
  m_rlcEntity->SetRadioBearer (this);
}


RadioBearerInstance::~RadioBearerInstance ()
{
  NS_LOG_FUNCTION (this);
  m_rlcEntity = 0;
  m_queue = 0;
  m_ipcsClassifierRecord = 0;
}


void
RadioBearerInstance::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_queue = 0;
  m_qosParameters = 0;
  m_rlcEntity->Dispose ();
  m_rlcEntity = 0;
}


void
RadioBearerInstance::SetBearerDirection (BearerDirection direction)
{
  m_direction = direction;
}


RadioBearerInstance::BearerDirection
RadioBearerInstance::GetBearerDirection (void) const
{
  return m_direction;
}


void
RadioBearerInstance::SetBearerType (BearerType type)
{
  m_bearerType = type;
}


RadioBearerInstance::BearerType
RadioBearerInstance::GetBearerType (void) const
{
  return m_bearerType;
}


void
RadioBearerInstance::SetQosParameters (Ptr<BearerQosParameters> qosParameters)
{
  m_qosParameters = qosParameters;
}


Ptr<BearerQosParameters>
RadioBearerInstance::GetQosParameters (void)
{
  return m_qosParameters;
}



Ptr<LteMacQueue>
RadioBearerInstance::GetQueue (void) const
{
  NS_LOG_FUNCTION (this);
  return m_queue;
}


bool
RadioBearerInstance::Enqueue (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this);
  return m_queue->Enqueue (packet);
}


Ptr<Packet>
RadioBearerInstance::Dequeue (void)
{
  NS_LOG_FUNCTION (this);
  return m_queue->Dequeue ();
}


Ptr<Packet>
RadioBearerInstance::Dequeue (uint32_t availableByte)
{
  NS_LOG_FUNCTION (this);
  /*
   This function can be caled when the UM or AM RLC mode is abilited.
   The bearer gives packets to rlc-entiry until all available byte will be used.
   */
  // return m_queue->Dequeue (availableByte);
  return 0;
}


bool
RadioBearerInstance::HasPackets (void) const
{
  NS_LOG_FUNCTION (this);
  return !m_queue->IsEmpty ();
}


void
RadioBearerInstance::SetRlcEntity (Ptr<RlcEntity> rlc)
{
  NS_LOG_FUNCTION (this);
  m_rlcEntity = rlc;
}


Ptr<RlcEntity>
RadioBearerInstance::GetRlcEntity (void)
{
  NS_LOG_FUNCTION (this);
  return m_rlcEntity;
}


void 
RadioBearerInstance::SetIpcsClassifierRecord (IpcsClassifierRecord* c)
{
  NS_LOG_FUNCTION (this << c);
  NS_ASSERT (m_ipcsClassifierRecord == 0);
  m_ipcsClassifierRecord = c;
}

IpcsClassifierRecord* 
RadioBearerInstance::GetIpcsClassifierRecord (void)
{
  NS_LOG_FUNCTION (this);
  return m_ipcsClassifierRecord;
}

} // namespace ns3
