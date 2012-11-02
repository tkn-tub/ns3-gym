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
#include "ns3/epc-x2-header.h"


NS_LOG_COMPONENT_DEFINE ("EpcX2Header");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (EpcX2Header);

EpcX2Header::EpcX2Header ()
  : m_messageType (0xfa),
    m_procedureCode (0xfa),
    m_lengthOfIes (0xfa),
    m_numberOfIes (0xfa)
{
}

EpcX2Header::~EpcX2Header ()
{
  m_messageType = 0xfb;
  m_procedureCode = 0xfb;
  m_lengthOfIes = 0xfb;
  m_numberOfIes = 0xfb;
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
  return 7;
}

void
EpcX2Header::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (m_messageType);
  i.WriteU8 (m_procedureCode);

  i.WriteU8 (0x00); // criticality = REJECT
  i.WriteU8 (m_lengthOfIes + 3);
  i.WriteHtonU16 (0);
  i.WriteU8 (m_numberOfIes);
}

uint32_t
EpcX2Header::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_messageType = i.ReadU8 ();
  m_procedureCode = i.ReadU8 ();

  i.ReadU8 ();
  m_lengthOfIes = i.ReadU8 () - 3;
  i.ReadNtohU16 ();
  m_numberOfIes = i.ReadU8 ();
  
  return GetSerializedSize ();
}

void
EpcX2Header::Print (std::ostream &os) const
{
  os << "MessageType=" << (uint32_t) m_messageType;
  os << " ProcedureCode=" << (uint32_t) m_procedureCode;
  os << " LengthOfIEs=" << (uint32_t) m_lengthOfIes;
  os << " NumberOfIEs=" << (uint32_t) m_numberOfIes;
}

uint8_t
EpcX2Header::GetMessageType () const
{
  return m_messageType;
}

void
EpcX2Header::SetMessageType (uint8_t messageType)
{
  m_messageType = messageType;
}

uint8_t
EpcX2Header::GetProcedureCode () const
{
  return m_procedureCode;
}

void
EpcX2Header::SetProcedureCode (uint8_t procedureCode)
{
  m_procedureCode = procedureCode;
}


void
EpcX2Header::SetLengthOfIes (uint32_t lengthOfIes)
{
  m_lengthOfIes = lengthOfIes;
}

void
EpcX2Header::SetNumberOfIes (uint32_t numberOfIes)
{
  m_numberOfIes = numberOfIes;
}

/////////////////////////////////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (EpcX2HandoverRequestHeader);

EpcX2HandoverRequestHeader::EpcX2HandoverRequestHeader ()
  : m_numberOfIes (0),
    m_headerLength (0),
    m_oldEnbUeX2apId (0xfffa),
    m_cause (0xfffa),
    m_targetCellId (0xfffa)
{
}

EpcX2HandoverRequestHeader::~EpcX2HandoverRequestHeader ()
{
  m_numberOfIes = 0;
  m_headerLength = 0;
  m_oldEnbUeX2apId = 0xfffb;
  m_cause = 0xfffb;
  m_targetCellId = 0xfffb;
  m_erabsToBeSetupList.clear ();
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
  return m_headerLength;
}

void
EpcX2HandoverRequestHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteHtonU16 (10);              // id = OLD_ENB_UE_X2AP_ID
  i.WriteU8 (0);                    // criticality = REJECT
  i.WriteU8 (2);                    // length of OLD_ENB_UE_X2AP_ID
  i.WriteHtonU16 (m_oldEnbUeX2apId);

  i.WriteHtonU16 (5);               // id = CAUSE
  i.WriteU8 (1 << 6);               // criticality = IGNORE
  i.WriteU8 (1);                    // length of CAUSE
  i.WriteU8 (m_cause);

  i.WriteHtonU16 (11);              // id = TARGET_CELLID
  i.WriteU8 (0);                    // criticality = REJECT
  i.WriteU8 (8);                    // length of TARGET_CELLID
  i.WriteHtonU32 (0x123456);        // fake PLMN
  i.WriteHtonU32 (m_targetCellId << 4);

  i.WriteHtonU16 (14);              // id = UE_CONTEXT_INFORMATION
  i.WriteU8 (0);                    // criticality = REJECT

  i.WriteHtonU64 (m_ueAggregateMaxBitRateDownlink);
  i.WriteHtonU64 (m_ueAggregateMaxBitRateUplink);

  std::vector <EpcX2Sap::ErabToBeSetupItem>::size_type sz = m_erabsToBeSetupList.size (); 
  i.WriteHtonU32 (sz);              // number of bearers
  for (int j = 0; j < (int) sz; j++)
    {
      i.WriteHtonU16 (m_erabsToBeSetupList [j].erabId);
      i.WriteHtonU16 (m_erabsToBeSetupList [j].erabLevelQosParameters.qci);
      i.WriteHtonU64 (m_erabsToBeSetupList [j].erabLevelQosParameters.gbrQosInfo.gbrDl);
      i.WriteHtonU64 (m_erabsToBeSetupList [j].erabLevelQosParameters.gbrQosInfo.gbrUl);
      i.WriteHtonU64 (m_erabsToBeSetupList [j].erabLevelQosParameters.gbrQosInfo.mbrDl);
      i.WriteHtonU64 (m_erabsToBeSetupList [j].erabLevelQosParameters.gbrQosInfo.mbrUl);
      i.WriteU8 (m_erabsToBeSetupList [j].erabLevelQosParameters.arp.priorityLevel);
      i.WriteU8 (m_erabsToBeSetupList [j].erabLevelQosParameters.arp.preemptionCapability);
      i.WriteU8 (m_erabsToBeSetupList [j].erabLevelQosParameters.arp.preemptionVulnerability);
      i.WriteU8 (m_erabsToBeSetupList [j].dlForwarding);
      i.WriteHtonU32 (m_erabsToBeSetupList [j].transportLayerAddress.Get ());
      i.WriteHtonU32 (m_erabsToBeSetupList [j].gtpTeid);
    }

}

uint32_t
EpcX2HandoverRequestHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_headerLength = 0;

  i.ReadNtohU16 ();
  i.ReadU8 ();
  i.ReadU8 ();
  m_oldEnbUeX2apId = i.ReadNtohU16 ();
  m_headerLength += 6;

  i.ReadNtohU16 ();
  i.ReadU8 ();
  i.ReadU8 ();
  m_cause = i.ReadU8 ();
  m_headerLength += 5;
  
  i.ReadNtohU16 ();
  i.ReadU8 ();
  i.ReadU8 ();
  i.ReadNtohU32 ();
  m_targetCellId = i.ReadNtohU32 () >> 4;
  m_headerLength += 12;

  i.ReadNtohU16 ();
  i.ReadU8 ();
  m_ueAggregateMaxBitRateDownlink = i.ReadNtohU64 ();
  m_ueAggregateMaxBitRateUplink   = i.ReadNtohU64 ();

  int sz = i.ReadNtohU32 ();
  for (int j = 0; j < sz; j++)
    {
      EpcX2Sap::ErabToBeSetupItem erabItem;

      erabItem.erabId = i.ReadNtohU16 ();
      erabItem.erabLevelQosParameters = EpsBearer ((EpsBearer::Qci) i.ReadNtohU16 ());

#if 0 // TODO missing some parameters in EpsBearer
      erabItem.erabLevelQosParameters.gbrQosInfo.gbrDl = i.ReadNtohU64 ();
      erabItem.erabLevelQosParameters.gbrQosInfo.gbrUl = i.ReadNtohU64 ();
      erabItem.erabLevelQosParameters.gbrQosInfo.mbrDl = i.ReadNtohU64 ();
      erabItem.erabLevelQosParameters.gbrQosInfo.mbrUl = i.ReadNtohU64 ();
      erabItem.erabLevelQosParameters.arp.priorityLevel = i.ReadU8 ();
      erabItem.erabLevelQosParameters.arp.preemptionCapability = i.ReadU8 ();
      erabItem.erabLevelQosParameters.arp.preemptionVulnerability = i.ReadU8 ();
#else
      i.ReadNtohU64 ();
      i.ReadNtohU64 ();
      i.ReadNtohU64 ();
      i.ReadNtohU64 ();
      i.ReadU8 ();
      i.ReadU8 ();
      i.ReadU8 ();
#endif

      erabItem.dlForwarding = i.ReadU8 ();
      erabItem.transportLayerAddress = Ipv4Address (i.ReadNtohU32 ());
      erabItem.gtpTeid = i.ReadNtohU32 ();

      m_erabsToBeSetupList[j] = erabItem;
    }

  return GetSerializedSize ();
}

void
EpcX2HandoverRequestHeader::Print (std::ostream &os) const
{
  os << "OldEnbUeX2apId=" << m_oldEnbUeX2apId;
  os << " Cause=" << m_cause;
  os << " TargetCellId=" << m_targetCellId;
  os << " UeAggrMaxBitRateDownlink= " << m_ueAggregateMaxBitRateDownlink;
  os << " UeAggrMaxBitRateUplink= " << m_ueAggregateMaxBitRateUplink;
  os << " NumOfBearers=" << m_erabsToBeSetupList.size ();

  std::vector <EpcX2Sap::ErabToBeSetupItem>::size_type sz = m_erabsToBeSetupList.size ();
  os << " [";
  for (int j = 0; j < (int) sz; j++)
    {
      os << m_erabsToBeSetupList[j].erabId << " ";
    }
  os << "]";
}

uint16_t
EpcX2HandoverRequestHeader::GetOldEnbUeX2apId () const
{
  return m_oldEnbUeX2apId;
}

void
EpcX2HandoverRequestHeader::SetOldEnbUeX2apId (uint16_t x2apId)
{
  m_oldEnbUeX2apId = x2apId;

  m_headerLength += 6;
  m_numberOfIes++;
}

uint16_t
EpcX2HandoverRequestHeader::GetCause () const
{
  return m_cause;
}

void
EpcX2HandoverRequestHeader::SetCause (uint16_t cause)
{
  m_cause = cause;

  m_headerLength += 5;
  m_numberOfIes++;
}

uint16_t
EpcX2HandoverRequestHeader::GetTargetCellId () const
{
  return m_targetCellId;
}

void
EpcX2HandoverRequestHeader::SetTargetCellId (uint16_t targetCellId)
{
  m_targetCellId = targetCellId;

  m_headerLength += 12;
  m_numberOfIes++;
}

std::vector <EpcX2Sap::ErabToBeSetupItem>
EpcX2HandoverRequestHeader::GetBearers () const
{
  return m_erabsToBeSetupList;
}

void
EpcX2HandoverRequestHeader::SetBearers (std::vector <EpcX2Sap::ErabToBeSetupItem> bearers)
{
  m_erabsToBeSetupList = bearers;
}

uint64_t
EpcX2HandoverRequestHeader::GetUeAggregateMaxBitRateDownlink () const
{
  return m_ueAggregateMaxBitRateDownlink;
}

void
EpcX2HandoverRequestHeader::SetUeAggregateMaxBitRateDownlink (uint64_t bitRate)
{
  m_ueAggregateMaxBitRateDownlink = bitRate;
  
  m_headerLength += 8;
}

uint64_t
EpcX2HandoverRequestHeader::GetUeAggregateMaxBitRateUplink () const
{
  return m_ueAggregateMaxBitRateUplink;
}

void
EpcX2HandoverRequestHeader::SetUeAggregateMaxBitRateUplink (uint64_t bitRate)
{
  m_ueAggregateMaxBitRateUplink = bitRate;

  m_headerLength += 8;
}

uint32_t
EpcX2HandoverRequestHeader::GetLengthOfIes () const
{
  return m_headerLength;
}

uint32_t
EpcX2HandoverRequestHeader::GetNumberOfIes () const
{
  return m_numberOfIes + 1;
}

/////////////////////////////////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (EpcX2HandoverRequestAckHeader);

EpcX2HandoverRequestAckHeader::EpcX2HandoverRequestAckHeader ()
  : m_oldEnbUeX2apId (0xfffa),
    m_newEnbUeX2apId (0xfffa)
{
}

EpcX2HandoverRequestAckHeader::~EpcX2HandoverRequestAckHeader ()
{
  m_oldEnbUeX2apId = 0xfffb;
  m_newEnbUeX2apId = 0xfffb;
  m_erabsAdmittedList.clear ();
  m_erabsNotAdmittedList.clear ();
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
  return 4;
}

void
EpcX2HandoverRequestAckHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteHtonU16 (m_oldEnbUeX2apId);
  i.WriteHtonU16 (m_newEnbUeX2apId);
}

uint32_t
EpcX2HandoverRequestAckHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_oldEnbUeX2apId = i.ReadNtohU16 ();
  m_newEnbUeX2apId = i.ReadNtohU16 ();

  return GetSerializedSize ();
}

void
EpcX2HandoverRequestAckHeader::Print (std::ostream &os) const
{
  os << "OldEnbUeX2apId=" << m_oldEnbUeX2apId;
  os << " NewEnbUeX2apId=" << m_newEnbUeX2apId;
}

uint16_t
EpcX2HandoverRequestAckHeader::GetOldEnbUeX2apId () const
{
  return m_oldEnbUeX2apId;
}

void
EpcX2HandoverRequestAckHeader::SetOldEnbUeX2apId (uint16_t x2apId)
{
  m_oldEnbUeX2apId = x2apId;
}

uint16_t
EpcX2HandoverRequestAckHeader::GetNewEnbUeX2apId () const
{
  return m_newEnbUeX2apId;
}

void
EpcX2HandoverRequestAckHeader::SetNewEnbUeX2apId (uint16_t x2apId)
{
  m_newEnbUeX2apId = x2apId;
}

std::vector <EpcX2Sap::ErabAdmittedItem> 
EpcX2HandoverRequestAckHeader::GetAdmittedBearers () const
{
  return m_erabsAdmittedList;
}

void
EpcX2HandoverRequestAckHeader::SetAdmittedBearers (std::vector <EpcX2Sap::ErabAdmittedItem> bearers)
{
  m_erabsAdmittedList = bearers;
}

std::vector <EpcX2Sap::ErabNotAdmittedItem>
EpcX2HandoverRequestAckHeader::GetNotAdmittedBearers () const
{
  return m_erabsNotAdmittedList;
}

void
EpcX2HandoverRequestAckHeader::SetNotAdmittedBearers (std::vector <EpcX2Sap::ErabNotAdmittedItem> bearers)
{
  m_erabsNotAdmittedList = bearers;
}

/////////////////////////////////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (EpcX2UeContextReleaseHeader);

EpcX2UeContextReleaseHeader::EpcX2UeContextReleaseHeader ()
  : m_oldEnbUeX2apId (0xfffa),
    m_newEnbUeX2apId (0xfffa)
{
}

EpcX2UeContextReleaseHeader::~EpcX2UeContextReleaseHeader ()
{
  m_oldEnbUeX2apId = 0xfffb;
  m_newEnbUeX2apId = 0xfffb;
}

TypeId
EpcX2UeContextReleaseHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcX2UeContextReleaseHeader")
    .SetParent<Header> ()
    .AddConstructor<EpcX2UeContextReleaseHeader> ()
  ;
  return tid;
}

TypeId
EpcX2UeContextReleaseHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
EpcX2UeContextReleaseHeader::GetSerializedSize (void) const
{
  return 4;
}

void
EpcX2UeContextReleaseHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteHtonU16 (m_oldEnbUeX2apId);
  i.WriteHtonU16 (m_newEnbUeX2apId);
}

uint32_t
EpcX2UeContextReleaseHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_oldEnbUeX2apId = i.ReadNtohU16 ();
  m_newEnbUeX2apId = i.ReadNtohU16 ();

  return GetSerializedSize ();
}

void
EpcX2UeContextReleaseHeader::Print (std::ostream &os) const
{
  os << "OldEnbUeX2apId=" << m_oldEnbUeX2apId;
  os << " NewEnbUeX2apId=" << m_newEnbUeX2apId;
}

uint16_t
EpcX2UeContextReleaseHeader::GetOldEnbUeX2apId () const
{
  return m_oldEnbUeX2apId;
}

void
EpcX2UeContextReleaseHeader::SetOldEnbUeX2apId (uint16_t x2apId)
{
  m_oldEnbUeX2apId = x2apId;
}

uint16_t
EpcX2UeContextReleaseHeader::GetNewEnbUeX2apId () const
{
  return m_newEnbUeX2apId;
}

void
EpcX2UeContextReleaseHeader::SetNewEnbUeX2apId (uint16_t x2apId)
{
  m_newEnbUeX2apId = x2apId;
}


} // namespace ns3
