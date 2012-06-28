/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#include "ns3/log.h"
// #include "ns3/packet.h"
#include "ns3/epc-x2-header.h"

NS_LOG_COMPONENT_DEFINE ("EpcX2Header");

namespace ns3 {
  
NS_OBJECT_ENSURE_REGISTERED (EpcX2Header);

EpcX2Header::EpcX2Header ()
  : m_messageType (0xfa),
    m_procedureCode (0xfa)
{
}

EpcX2Header::~EpcX2Header ()
{
  m_messageType = 0xfb;
  m_procedureCode = 0xfb;
}

TypeId
EpcX2Header::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcX2Header")
    .SetParent<Header> ()
    .AddConstructor<EpcX2Header> ()
  ;
  return tid;
}

TypeId
EpcX2Header::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
EpcX2Header::GetSerializedSize (void) const
{
  return 2;
}

void
EpcX2Header::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (m_messageType);
  i.WriteU8 (m_procedureCode);
}

uint32_t
EpcX2Header::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_messageType = i.ReadU8 ();
  m_procedureCode = i.ReadU8 ();

  return GetSerializedSize ();
}

void
EpcX2Header::Print (std::ostream &os) const
{
  os << "MessageType=" << (uint32_t) m_messageType;
  os << " ProcedureCode=" << (uint32_t) m_procedureCode;
}

uint8_t
EpcX2Header::GetMessageType () const
{
  return m_messageType;
}

void
EpcX2Header::SetMessageType (uint8_t messageType)
{
  this->m_messageType = messageType;
}

uint8_t
EpcX2Header::GetProcedureCode () const
{
  return m_procedureCode;
}

void
EpcX2Header::SetProcedureCode (uint8_t procedureCode)
{
  this->m_procedureCode = procedureCode;
}

/////////////////////////////////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (EpcX2HandoverRequestHeader);

EpcX2HandoverRequestHeader::EpcX2HandoverRequestHeader ()
  : m_oldEnbUeX2apId (0xfffa),
    m_cause (0xfffa),
    m_targetCellId (0xfffa)
{
}

EpcX2HandoverRequestHeader::~EpcX2HandoverRequestHeader ()
{
  m_oldEnbUeX2apId = 0xfffb;
  m_cause = 0xfffb;
  m_targetCellId = 0xfffb;
  m_erabsList.clear (); // TODO Clearing of a list
}

TypeId
EpcX2HandoverRequestHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcX2HandoverRequestHeader")
    .SetParent<Header> ()
    .AddConstructor<EpcX2HandoverRequestHeader> ()
  ;
  return tid;
}

TypeId
EpcX2HandoverRequestHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
EpcX2HandoverRequestHeader::GetSerializedSize (void) const
{
  return 6;
}

void
EpcX2HandoverRequestHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteHtonU16 (m_oldEnbUeX2apId);
  i.WriteHtonU16 (m_cause);
  i.WriteHtonU16 (m_targetCellId);
}

uint32_t
EpcX2HandoverRequestHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_oldEnbUeX2apId = i.ReadNtohU16 ();
  m_cause = i.ReadNtohU16 ();
  m_targetCellId = i.ReadNtohU16 ();

  return GetSerializedSize ();
}

void
EpcX2HandoverRequestHeader::Print (std::ostream &os) const
{
  os << "Cause=" << m_cause;
  os << " TargetCellId=" << m_targetCellId;
}

uint16_t
EpcX2HandoverRequestHeader::GetCause () const
{
  return m_cause;
}

void
EpcX2HandoverRequestHeader::SetCause (uint16_t cause)
{
  this->m_cause = cause;
}

uint16_t
EpcX2HandoverRequestHeader::GetTargetCellId () const
{
  return m_targetCellId;
}

void
EpcX2HandoverRequestHeader::SetTargetCellId (uint16_t targetCellId)
{
  this->m_targetCellId = targetCellId;
}

/////////////////////////////////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (EpcX2HandoverRequestAckHeader);

EpcX2HandoverRequestAckHeader::EpcX2HandoverRequestAckHeader ()
  : m_oldEnbUeX2apId (0xfffa),
    m_cause (0xfffa),
    m_targetCellId (0xfffa)
{
}

EpcX2HandoverRequestAckHeader::~EpcX2HandoverRequestAckHeader ()
{
  m_oldEnbUeX2apId = 0xfffb;
  m_cause = 0xfffb;
  m_targetCellId = 0xfffb;
  m_erabsList.clear (); // TODO Clearing of a list
}

TypeId
EpcX2HandoverRequestAckHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcX2HandoverRequestAckHeader")
    .SetParent<Header> ()
    .AddConstructor<EpcX2HandoverRequestAckHeader> ()
  ;
  return tid;
}

TypeId
EpcX2HandoverRequestAckHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
EpcX2HandoverRequestAckHeader::GetSerializedSize (void) const
{
  return 6;
}

void
EpcX2HandoverRequestAckHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteHtonU16 (m_oldEnbUeX2apId);
  i.WriteHtonU16 (m_cause);
  i.WriteHtonU16 (m_targetCellId);
}

uint32_t
EpcX2HandoverRequestAckHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_oldEnbUeX2apId = i.ReadNtohU16 ();
  m_cause = i.ReadNtohU16 ();
  m_targetCellId = i.ReadNtohU16 ();

  return GetSerializedSize ();
}

void
EpcX2HandoverRequestAckHeader::Print (std::ostream &os) const
{
  os << "Cause=" << m_cause;
  os << " TargetCellId=" << m_targetCellId;
}

uint16_t
EpcX2HandoverRequestAckHeader::GetCause () const
{
  return m_cause;
}

void
EpcX2HandoverRequestAckHeader::SetCause (uint16_t cause)
{
  this->m_cause = cause;
}

uint16_t
EpcX2HandoverRequestAckHeader::GetTargetCellId () const
{
  return m_targetCellId;
}

void
EpcX2HandoverRequestAckHeader::SetTargetCellId (uint16_t targetCellId)
{
  this->m_targetCellId = targetCellId;
}


} // namespace ns3
