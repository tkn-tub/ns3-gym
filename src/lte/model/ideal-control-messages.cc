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

#include "ideal-control-messages.h"
#include "ns3/address-utils.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"
#include "lte-net-device.h"
#include "ue-net-device.h"

NS_LOG_COMPONENT_DEFINE ("IdealControlMessage");


namespace ns3 {

IdealControlMessage::IdealControlMessage (void)
  : m_source (0),
    m_destination (0)
{
}


IdealControlMessage::~IdealControlMessage (void)
{
  m_source = 0;
  m_destination = 0;
}


void
IdealControlMessage::SetSourceDevice (Ptr<LteNetDevice> src)
{
  m_source = src;
}


void
IdealControlMessage::SetDestinationDevice (Ptr<LteNetDevice> dst)
{
  m_destination = dst;
}


Ptr<LteNetDevice>
IdealControlMessage::GetSourceDevice (void)
{
  return m_source;
}


Ptr<LteNetDevice>
IdealControlMessage::GetDestinationDevice (void)
{
  return m_destination;
}


void
IdealControlMessage::SetMessageType (IdealControlMessage::MessageType type)
{
  m_type = type;
}


IdealControlMessage::MessageType
IdealControlMessage::GetMessageType (void)
{
  return m_type;
}



// ----------------------------------------------------------------------------------------------------------

PdcchMapIdealControlMessage::PdcchMapIdealControlMessage (void)
{
  m_idealPdcchMessage =  new IdealPdcchMessage ();
  SetMessageType (IdealControlMessage::ALLOCATION_MAP);
}


PdcchMapIdealControlMessage::~PdcchMapIdealControlMessage (void)
{
  delete m_idealPdcchMessage;
}

void
PdcchMapIdealControlMessage::AddNewRecord (Direction direction,
                                           int subChannel, Ptr<LteNetDevice> ue, double mcs)
{

}


PdcchMapIdealControlMessage::IdealPdcchMessage*
PdcchMapIdealControlMessage::GetMessage (void)
{
  return m_idealPdcchMessage;
}



// ----------------------------------------------------------------------------------------------------------


CqiIdealControlMessage::CqiIdealControlMessage (void)
{
  m_cqiFeedbacks = new CqiFeedbacks ();
  SetMessageType (IdealControlMessage::CQI_FEEDBACKS);
}


CqiIdealControlMessage::~CqiIdealControlMessage (void)
{
  delete m_cqiFeedbacks;
}

void
CqiIdealControlMessage::AddNewRecord (int subChannel, double cqi)
{
  CqiFeedback c;
  c.m_idSubChannel = subChannel;
  c.m_cqi = cqi;

  m_cqiFeedbacks->push_back (c);

}


CqiIdealControlMessage::CqiFeedbacks*
CqiIdealControlMessage::GetMessage (void)
{
  return m_cqiFeedbacks;
}


} // namespace ns3
