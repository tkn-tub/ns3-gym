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
 * Author: Jaume Nin <jnin@cttc.cat>
 */


#ifndef EPS_GTPU_V1_H
#define EPS_GTPU_V1_H

#include <ns3/ptr.h>
#include <ns3/header.h>
#include <ns3/ipv4-header.h>

namespace ns3 {

class Packet;


/**
 * \ingroup lte
 *
 * Implementation of the GTPv1-U Release 10 as per 3Gpp TS 29.281 document
 */
class GtpuHeader : public Header
{
public:
  static TypeId GetTypeId (void);
  GtpuHeader ();
  virtual ~GtpuHeader ();
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

  bool GetExtensionHeaderFlag () const;
  uint16_t GetLength () const;
  uint8_t GetMessageType () const;
  uint8_t GetNPduNumber () const;
  bool GetNPduNumberFlag () const;
  uint8_t GetNextExtensionType () const;
  bool GetProtocolType () const;
  uint16_t GetSequenceNumber () const;
  bool GetSequenceNumberFlag () const;
  uint32_t GetTeid () const;
  uint8_t GetVersion () const;
  void SetExtensionHeaderFlag (bool m_extensionHeaderFlag);
  void SetLength (uint16_t m_length);
  void SetMessageType (uint8_t m_messageType);
  void SetNPduNumber (uint8_t m_nPduNumber);
  void SetNPduNumberFlag (bool m_nPduNumberFlag);
  void SetNextExtensionType (uint8_t m_nextExtensionType);
  void SetProtocolType (bool m_protocolType);
  void SetSequenceNumber (uint16_t m_sequenceNumber);
  void SetSequenceNumberFlag (bool m_sequenceNumberFlag);
  void SetTeid (uint32_t m_teid);
  void SetVersion (uint8_t m_version);

  bool operator == (const GtpuHeader& b) const;


private:
  /**
   * This field is used to determine the version of the GTPU-U protocol. The version number shall be set to 1.
   */
  uint8_t m_version;   // really a 3 uint3_t

  /**
   * This bit is used as a protocol discriminator between GTP (when PT is '1') and GTP' (when
   * PT is '0').
   */
  bool m_protocolType;

  /**
   * This flag indicates the presence of a meaningful value of the Next Extension Header
   * field. When it is set to '0', the Next Extension Header field either is not present
   * or, if present, shall not be interpreted. When it is set to '1', the Next Extension
   *  Header field is present, and shall be interpreted
   */
  bool m_extensionHeaderFlag;

  /**
   * This flag indicates the presence of a meaningful value of the Sequence Number field.
   * When it is set to '0', the Sequence Number field either is not present or, if present,
   * shall not be interpreted. When it is set to '1', the Sequence Number field is present,
   * and shall be interpreted
   */
  bool m_sequenceNumberFlag;
  /**
   * This flag indicates the presence of a meaningful value of the N-PDU Number field.
   * When it is set to '0', the N-PDU Number field either is not present, or, if present,
   * shall not be interpreted. When it is set to '1', the N-PDU Number field is present,
   * and shall be interpreted
   */
  bool m_nPduNumberFlag;
  /**
   *  This field indicates the type of GTP-U message
   */
  uint8_t m_messageType;
  /**
   *  This field indicates the length in octets of the payload, i.e. the rest of the packet
   *  following the mandatory part of the GTP header (that is the first 8 octets).
   *  The Sequence Number, the N-PDU Number or any Extension headers shall be considered
   *  to be part of the payload, i.e. included in the length count
   */
  uint16_t m_length;

  /**
   * This field unambiguously identifies a tunnel endpoint in the receiving GTP-U
   * protocol entity. The receiving end side of a GTP tunnel locally assigns the
   * TEID value the transmitting side has to use.
   */
  uint32_t m_teid;
  /**
   * If Sequence Number field is used for G-PDUs (T-PDUs+headers), an increasing
   * sequence number for T-PDUs is transmitted via GTP-U tunnels, when transmission
   * order must be preserved
   */
  uint16_t m_sequenceNumber;
  /**
   * This field is used at the Inter SGSN Routeing Area Update procedure and some
   * inter-system handover procedures (e.g. between 2G and 3G radio access networks).
   * This field is used to co-ordinate the data transmission for acknowledged mode
   * of communication between the MS and the SGSN
   */
  uint8_t m_nPduNumber;
  /**
   * This field defines the type of Extension Header that follows this field in
   * the GTP-PDU
   */
  uint8_t m_nextExtensionType;

};

} // namespace ns3

#endif /* EPS_GTPU_V1_H */
