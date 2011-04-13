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

#include "ue-record.h"

NS_LOG_COMPONENT_DEFINE ("UeRecord");

namespace ns3 {

UeRecord::UeRecord (void)
  :  m_ue (0),
    m_enb (0)
{
  NS_LOG_FUNCTION (this);
}

UeRecord::UeRecord (Ptr<NetDevice> ue, Ptr<NetDevice> enb)
  :  m_ue (ue),
    m_enb (enb)
{
  NS_LOG_FUNCTION (this << ue << enb);
}

UeRecord::~UeRecord (void)
{
  NS_LOG_FUNCTION (this);
  m_ue = 0;
  m_enb = 0;
}

void
UeRecord::SetUe (Ptr<NetDevice> ue)
{
  NS_LOG_FUNCTION (this << ue);
  m_ue = ue;
}


Ptr<NetDevice>
UeRecord::GetUe (void)
{
  NS_LOG_FUNCTION (this);
  return m_ue;
}


void
UeRecord::SetEnb (Ptr<NetDevice> enb)
{
  NS_LOG_FUNCTION (this << enb);
  m_enb = enb;
}


Ptr<NetDevice>
UeRecord::GetEnb (void)
{
  NS_LOG_FUNCTION (this);
  return m_enb;
}



void
UeRecord::SetCqiFeedbacks (UeRecord::CqiFeedbacks cqiFeedbacks)
{
  NS_LOG_FUNCTION (this);
  // XXX: copy all value of the list!
  m_cqiFeedbacks = cqiFeedbacks;
}


UeRecord::CqiFeedbacks
UeRecord::GetCqiFeedbacks (void)
{
  NS_LOG_FUNCTION (this);
  return m_cqiFeedbacks;
}

}
; // namespace ns3
