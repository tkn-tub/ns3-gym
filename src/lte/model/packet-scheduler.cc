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
#include "packet-scheduler.h"
#include "enb-mac-entity.h"

NS_LOG_COMPONENT_DEFINE ("PacketScheduler");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (PacketScheduler);

PacketScheduler::PacketScheduler ()
  : m_enb (0),
    m_macEntity (0)
{
  NS_LOG_FUNCTION (this);
}


PacketScheduler::PacketScheduler (Ptr<EnbNetDevice> enb)
  : m_enb (enb),
    m_macEntity (0)
{
  NS_LOG_FUNCTION (this);
}



TypeId
PacketScheduler::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PacketScheduler")
    .SetParent<Object> ()
  ;
  return tid;
}


PacketScheduler::~PacketScheduler ()
{
  NS_LOG_FUNCTION (this);
}

void
PacketScheduler::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_enb = 0;
  m_macEntity = 0;
  Object::DoDispose ();
}

void
PacketScheduler::SetDevice (Ptr<EnbNetDevice> enb)
{
  NS_LOG_FUNCTION (this << enb);
  m_enb = enb;
}


Ptr<EnbNetDevice>
PacketScheduler::GetDevice ()
{
  NS_LOG_FUNCTION (this);
  return m_enb;
}



void
PacketScheduler::SetMacEntity (Ptr<MacEntity> mac)
{
  NS_LOG_FUNCTION (this);
  m_macEntity = mac;
}


Ptr<MacEntity>
PacketScheduler::GetMacEntity (void)
{
  NS_LOG_FUNCTION (this);
  return m_macEntity;
}


void
PacketScheduler::RunPacketScheduler (void)
{
  NS_LOG_FUNCTION (this);
  DoRunPacketScheduler ();
}


} // namespace ns3
