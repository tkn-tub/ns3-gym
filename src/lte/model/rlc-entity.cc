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


#include "rlc-entity.h"
#include <ns3/log.h>
#include <ns3/pointer.h>
#include <ns3/packet.h>
#include "ue-net-device.h"
#include "enb-net-device.h"
#include "rrc-entity.h"
#include "radio-bearer-instance.h"

NS_LOG_COMPONENT_DEFINE ("RlcEntity");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (RlcEntity);

TypeId RlcEntity::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RlcEntity")
    .SetParent<Object> ();
  return tid;
}



RlcEntity::RlcEntity ()
  : m_device (0)
{
  NS_LOG_FUNCTION (this);
}


RlcEntity::RlcEntity (Ptr<LteNetDevice> d)
  : m_device (d)
{
  NS_LOG_FUNCTION (this << d);
}


RlcEntity::~RlcEntity ()
{
  NS_LOG_FUNCTION (this);
}

void
RlcEntity::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_device = 0;
  m_bearer = 0;
}

void
RlcEntity::SetDevice (Ptr<LteNetDevice> d)
{
  NS_LOG_FUNCTION (this << d);
  m_device = d;
}


Ptr<LteNetDevice>
RlcEntity::GetDevice (void)
{
  NS_LOG_FUNCTION (this);
  return m_device;
}


Ptr<Packet>
RlcEntity::Dequeue ()
{
  NS_LOG_FUNCTION (this);

  Ptr<Packet> p = GetRadioBearer ()->Dequeue ();
  return p;
}


void 
RlcEntity::SetRadioBearer (Ptr<RadioBearerInstance> b)
{
  NS_LOG_FUNCTION (this << b);
  m_bearer = b;
}

Ptr<RadioBearerInstance> 
RlcEntity::GetRadioBearer (void)
{
  NS_LOG_FUNCTION (this);
  return m_bearer;
}



} // namespace ns3
