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
 *         Marco Miozzo <marco.miozzo@cttc.es>
 */

#include "lte-control-messages.h"
#include "ns3/address-utils.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"
#include "lte-net-device.h"
#include "lte-ue-net-device.h"

NS_LOG_COMPONENT_DEFINE ("LteControlMessage");


namespace ns3 {

LteControlMessage::LteControlMessage (void)
  : m_source (0),
    m_destination (0)
{
}


LteControlMessage::~LteControlMessage (void)
{
  m_source = 0;
  m_destination = 0;
}


void
LteControlMessage::SetMessageType (LteControlMessage::MessageType type)
{
  m_type = type;
}


LteControlMessage::MessageType
LteControlMessage::GetMessageType (void)
{
  return m_type;
}


// ----------------------------------------------------------------------------------------------------------


DlDciLteControlMessage::DlDciLteControlMessage (void)
{
  SetMessageType (LteControlMessage::DL_DCI);
}


DlDciLteControlMessage::~DlDciLteControlMessage (void)
{

}

void
DlDciLteControlMessage::SetDci (DlDciListElement_s dci)
{
  m_dci = dci;

}


DlDciListElement_s
DlDciLteControlMessage::GetDci (void)
{
  return m_dci;
}


// ----------------------------------------------------------------------------------------------------------


UlDciLteControlMessage::UlDciLteControlMessage (void)
{
  SetMessageType (LteControlMessage::UL_DCI);
}


UlDciLteControlMessage::~UlDciLteControlMessage (void)
{

}

void
UlDciLteControlMessage::SetDci (UlDciListElement_s dci)
{
  m_dci = dci;

}


UlDciListElement_s
UlDciLteControlMessage::GetDci (void)
{
  return m_dci;
}


// ----------------------------------------------------------------------------------------------------------


DlCqiLteControlMessage::DlCqiLteControlMessage (void)
{
  SetMessageType (LteControlMessage::DL_CQI);
}


DlCqiLteControlMessage::~DlCqiLteControlMessage (void)
{

}

void
DlCqiLteControlMessage::SetDlCqi (CqiListElement_s dlcqi)
{
  m_dlCqi = dlcqi;

}


CqiListElement_s
DlCqiLteControlMessage::GetDlCqi (void)
{
  return m_dlCqi;
}



// ----------------------------------------------------------------------------------------------------------


BsrLteControlMessage::BsrLteControlMessage (void)
{
  SetMessageType (LteControlMessage::BSR);
}


BsrLteControlMessage::~BsrLteControlMessage (void)
{

}

void
BsrLteControlMessage::SetBsr (MacCeListElement_s bsr)
{
  m_bsr = bsr;

}


MacCeListElement_s
BsrLteControlMessage::GetBsr (void)
{
  return m_bsr;
}


} // namespace ns3

