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

#ifndef EPC_X2_HEADER_H
#define EPC_X2_HEADER_H

#include "ns3/header.h"

namespace ns3 {

class EpcX2Header : public Header
{
public:
  EpcX2Header ();
  virtual ~EpcX2Header ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;


  uint8_t GetMessageType () const;
  void SetMessageType (uint8_t messageType);

  uint8_t GetProcedureCode () const;
  void SetProcedureCode (uint8_t procedureCode);


  enum ProcedureCode_t {
    HANDOVER_PREPARATION_TYPE   = 0
  };

  enum TypeOfMessage_t {
    INITIATING_MESSAGE      = 0,
    SUCCESSFUL_OUTCOME      = 1,
    UNSUCCESSFUL_OUTCOME    = 2
  };

private:
  uint8_t m_messageType;
  uint8_t m_procedureCode;
};


class EpcX2HandoverRequestHeader : public Header
{
public:
  EpcX2HandoverRequestHeader ();
  virtual ~EpcX2HandoverRequestHeader ();
  
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
  
  
  uint16_t GetCause () const;
  void SetCause (uint16_t cause);

  uint16_t GetTargetCellId () const;
  void SetTargetCellId (uint16_t targetCellId);

private:
  uint16_t          m_oldEnbUeX2apId; // TODO MRE When and why this is used? 
  uint16_t          m_cause;
  uint16_t          m_targetCellId;
  std::list<uint16_t>   m_erabsList;
};


class EpcX2HandoverRequestAckHeader : public Header
{
public:
  EpcX2HandoverRequestAckHeader ();
  virtual ~EpcX2HandoverRequestAckHeader ();
  
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
  
  
  uint16_t GetCause () const;
  void SetCause (uint16_t cause);

  uint16_t GetTargetCellId () const;
  void SetTargetCellId (uint16_t targetCellId);

private:
  uint16_t          m_oldEnbUeX2apId; // TODO MRE When and why this is used? 
  uint16_t          m_cause;
  uint16_t          m_targetCellId;
  std::list<uint16_t>   m_erabsList;
};


} // namespace ns3

#endif // EPC_X2_HEADER_H
