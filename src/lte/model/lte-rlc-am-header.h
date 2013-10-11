/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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

#ifndef LTE_RLC_AM_HEADER_H
#define LTE_RLC_AM_HEADER_H

#include "ns3/header.h"
#include "ns3/lte-rlc-sequence-number.h"

#include <list>

namespace ns3 {

/**
 * \ingroup lte
 * \brief The packet header for the AM Radio Link Control (RLC) protocol packets
 *
 * This class has fields corresponding to those in an RLC header as well as
 * methods for serialization to and deserialization from a byte buffer.
 * It follows 3GPP TS 36.322 Radio Link Control (RLC) protocol specification.
 */
class LteRlcAmHeader : public Header
{
public:

  /**
   * \brief Constructor
   *
   * Creates a null header
   */
  LteRlcAmHeader ();
  ~LteRlcAmHeader ();

  void SetDataPdu (void);
  void SetControlPdu (uint8_t controlPduType);
  bool IsDataPdu (void) const;
  bool IsControlPdu (void) const;

  typedef enum {
    CONTROL_PDU = 0,
    DATA_PDU    = 1
  } DataControlPdu_t;

  typedef enum {
    STATUS_PDU = 000,
  } ControPduType_t;

  //
  // DATA PDU
  //

  void SetSequenceNumber (SequenceNumber10 sequenceNumber);
  SequenceNumber10 GetSequenceNumber () const;

  void SetFramingInfo (uint8_t framingInfo);
  uint8_t GetFramingInfo () const;

  typedef enum {
    FIRST_BYTE    = 0x00,
    NO_FIRST_BYTE = 0x02
  } FramingInfoFirstByte_t;

  typedef enum {
    LAST_BYTE    = 0x00,
    NO_LAST_BYTE = 0x01
  } FramingInfoLastByte_t;

  void PushExtensionBit (uint8_t extensionBit);
  void PushLengthIndicator (uint16_t lengthIndicator);

  uint8_t PopExtensionBit (void);
  uint16_t PopLengthIndicator (void);

  typedef enum {
    DATA_FIELD_FOLLOWS  = 0,
    E_LI_FIELDS_FOLLOWS = 1
  } ExtensionBit_t;

  void SetResegmentationFlag (uint8_t resegFlag);
  uint8_t GetResegmentationFlag () const;

  typedef enum {
    PDU = 0,
    SEGMENT = 1
  } ResegmentationFlag_t;

  void SetPollingBit (uint8_t pollingBit);
  uint8_t GetPollingBit () const;

  typedef enum {
    STATUS_REPORT_NOT_REQUESTED = 0,
    STATUS_REPORT_IS_REQUESTED  = 1
  } PollingBit_t;

  void SetLastSegmentFlag (uint8_t lsf);
  uint8_t GetLastSegmentFlag () const;

  typedef enum {
    NO_LAST_PDU_SEGMENT = 0,
    LAST_PDU_SEGMENT    = 1
  } LastSegmentFlag_t;

  void SetSegmentOffset (uint16_t segmentOffset);
  uint16_t GetSegmentOffset () const;
  uint16_t GetLastOffset () const;

  //
  // CONTROL PDU
  //

  void SetAckSn (SequenceNumber10 ackSn);
  SequenceNumber10 GetAckSn () const;


  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  // Brett - Methods to handle NACKs
  void PushNack (int nack);
  int PopNack (void);

private:
  uint16_t m_headerLength;
  uint8_t  m_dataControlBit;

  // Data PDU fields
  uint8_t  m_resegmentationFlag;
  uint8_t  m_pollingBit;
  uint8_t  m_framingInfo;      //  2 bits
  SequenceNumber10 m_sequenceNumber;
  uint8_t  m_lastSegmentFlag;
  uint16_t m_segmentOffset;
  uint16_t m_lastOffset;

  std::list <uint8_t> m_extensionBits; // Includes extensionBit of the fixed part
  std::list <uint16_t> m_lengthIndicators;

  // Control PDU fields
  uint8_t  m_controlPduType;

  // Status PDU fields
  SequenceNumber10 m_ackSn;
  std::list <int> m_nackSnList;

  std::list <uint8_t> m_extensionBits1; // Includes E1 after ACK_SN
  std::list <uint8_t> m_extensionBits2;

};

}; // namespace ns3

#endif // LTE_RLC_AM_HEADER_H
