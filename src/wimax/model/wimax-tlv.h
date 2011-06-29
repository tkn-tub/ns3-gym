/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2009 INRIA, UDcast
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
 *         Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *
 */

#include "ns3/ipv4-address.h"
#include <stdlib.h>
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/uinteger.h"
#include "ns3/header.h"
#include <vector>

#ifndef WIMAXTLV_H_
#define WIMAXTLV_H_
#define WIMAX_TLV_EXTENDED_LENGTH_MASK 0x80



namespace ns3 {

/**
 * \ingroup wimax
 * \brief The value field of a tlv can take different values (uint8_t, uint16, vector...). This class is a virtual interface
 * that all the types of tlv values should derive
 */
class TlvValue
{
public:
  virtual ~TlvValue ()
  {
  }
  virtual uint32_t GetSerializedSize (void) const = 0;
  virtual void Serialize (Buffer::Iterator start) const = 0;
  virtual uint32_t Deserialize (Buffer::Iterator start, uint64_t valueLen ) = 0;
  virtual TlvValue * Copy (void) const = 0;
private:
};


// =============================================================================
/**
 * \ingroup wimax
 * \brief This class implements the Type-Len-Value structure channel encodings as described by "IEEE Standard for
 * Local and metropolitan area networks Part 16: Air Interface for Fixed Broadband Wireless Access Systems"
 * 11. TLV encodings, page 645
 *
 */
class Tlv : public Header
{
public:
  enum CommonTypes
  {
    HMAC_TUPLE = 149,
    MAC_VERSION_ENCODING = 148,
    CURRENT_TRANSMIT_POWER = 147,
    DOWNLINK_SERVICE_FLOW = 146,
    UPLINK_SERVICE_FLOW = 145,
    VENDOR_ID_EMCODING = 144,
    VENDOR_SPECIFIC_INFORMATION = 143
  };
  Tlv (uint8_t type, uint64_t length, const TlvValue & value);
  Tlv (void);
  ~Tlv (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  uint8_t GetSizeOfLen (void) const;
  uint8_t GetType (void) const;
  uint64_t GetLength (void) const;
  TlvValue* PeekValue (void);
  Tlv * Copy (void) const;
  TlvValue * CopyValue (void) const;
  Tlv &operator = (Tlv const& o);
  Tlv (const Tlv & tlv);

private:
  uint8_t m_type;
  uint64_t m_length;
  TlvValue * m_value;
};

// ==============================================================================
/**
 * \ingroup wimax
 */
class U8TlvValue : public TlvValue
{
public:
  U8TlvValue (uint8_t value);
  U8TlvValue ();
  ~U8TlvValue (void);
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start,uint64_t valueLen);
  uint32_t Deserialize (Buffer::Iterator start);
  uint8_t GetValue (void) const;
  U8TlvValue * Copy (void) const;
private:
  uint8_t  m_value;
};

// ==============================================================================
/**
 * \ingroup wimax
 */
class U16TlvValue : public TlvValue
{
public:
  U16TlvValue (uint16_t value);
  U16TlvValue ();
  ~U16TlvValue (void);
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start,uint64_t valueLen);
  uint32_t Deserialize (Buffer::Iterator start);
  uint16_t GetValue (void) const;
  virtual U16TlvValue * Copy (void) const;
private:
  uint16_t  m_value;
};

// ==============================================================================
/**
 * \ingroup wimax
 */
class U32TlvValue : public TlvValue
{
public:
  U32TlvValue (uint32_t value);
  U32TlvValue ();
  ~U32TlvValue (void);

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start, uint64_t valueLen);
  uint32_t Deserialize (Buffer::Iterator start);
  uint32_t GetValue (void) const;
  virtual U32TlvValue * Copy (void) const;
private:
  uint32_t  m_value;
};

// ==============================================================================

/**
 * \ingroup wimax
 * \brief this class is used to implement a vector of values in one tlv value field
 */
class VectorTlvValue : public TlvValue
{
public:
  typedef std::vector<Tlv*>::const_iterator Iterator;
  VectorTlvValue (void);
  ~VectorTlvValue (void);
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start, uint64_t valueLength) = 0;
  Iterator Begin () const;
  Iterator End () const;
  void Add (const Tlv & val);
  virtual VectorTlvValue * Copy (void) const = 0;
private:
  std::vector<Tlv*>  * m_tlvList;
};

// ==============================================================================
/**
 * \ingroup wimax
 */
class SfVectorTlvValue : public VectorTlvValue
{

public:
  enum Type
  {
    SFID = 1,
    CID = 2,
    Service_Class_Name = 3,
    reserved1 = 4,
    QoS_Parameter_Set_Type = 5,
    Traffic_Priority = 6,
    Maximum_Sustained_Traffic_Rate = 7,
    Maximum_Traffic_Burst = 8,
    Minimum_Reserved_Traffic_Rate = 9,
    Minimum_Tolerable_Traffic_Rate = 10,
    Service_Flow_Scheduling_Type = 11,
    Request_Transmission_Policy = 12,
    Tolerated_Jitter = 13,
    Maximum_Latency = 14,
    Fixed_length_versus_Variable_length_SDU_Indicator = 15,
    SDU_Size = 16,
    Target_SAID = 17,
    ARQ_Enable = 18,
    ARQ_WINDOW_SIZE = 19,
    ARQ_RETRY_TIMEOUT_Transmitter_Delay = 20,
    ARQ_RETRY_TIMEOUT_Receiver_Delay = 21,
    ARQ_BLOCK_LIFETIME = 22,
    ARQ_SYNC_LOSS = 23,
    ARQ_DELIVER_IN_ORDER = 24,
    ARQ_PURGE_TIMEOUT = 25,
    ARQ_BLOCK_SIZE = 26,
    reserved2 = 27,
    CS_Specification = 28,
    IPV4_CS_Parameters = 100
  };
  SfVectorTlvValue ();
  virtual uint32_t Deserialize (Buffer::Iterator start, uint64_t valueLength);
  virtual SfVectorTlvValue * Copy (void) const;

};
// ==============================================================================

/**
 * \ingroup wimax
 * \brief this class implements the convergence sub-layer descriptor as a tlv vector
 */
class CsParamVectorTlvValue : public VectorTlvValue
{
public:
  enum Type
  {
    Classifier_DSC_Action = 1,
    Packet_Classification_Rule = 3,
  };
  CsParamVectorTlvValue ();
  virtual uint32_t Deserialize (Buffer::Iterator start, uint64_t valueLength);
  virtual CsParamVectorTlvValue * Copy (void) const;
private:
};

// ==============================================================================

/**
 * \ingroup wimax
 * \brief this class implements the classifier descriptor as a tlv vector
 */
class ClassificationRuleVectorTlvValue : public VectorTlvValue
{
public:
  enum ClassificationRuleTlvType
  {
    Priority = 1,
    ToS = 2,
    Protocol = 3,
    IP_src = 4,
    IP_dst = 5,
    Port_src = 6,
    Port_dst = 7,
    Index = 14,
  };
  ClassificationRuleVectorTlvValue ();
  virtual uint32_t Deserialize (Buffer::Iterator start, uint64_t valueLength);
  virtual ClassificationRuleVectorTlvValue * Copy (void) const;
private:
};

// ==============================================================================
/**
 * \ingroup wimax
 */
class TosTlvValue : public TlvValue
{
public:
  TosTlvValue ();
  TosTlvValue (uint8_t, uint8_t, uint8_t);
  ~TosTlvValue ();
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start, uint64_t valueLength);
  uint8_t GetLow (void) const;
  uint8_t GetHigh (void) const;
  uint8_t GetMask (void) const;
  virtual TosTlvValue * Copy () const;
private:
  uint8_t m_low;
  uint8_t m_high;
  uint8_t m_mask;
};

// ==============================================================================
/**
 * \ingroup wimax
 */
class PortRangeTlvValue : public TlvValue
{
public:
  struct PortRange
  {
    uint16_t PortLow;
    uint16_t PortHigh;
  };
  typedef std::vector<struct PortRange>::const_iterator Iterator;
  PortRangeTlvValue ();
  ~PortRangeTlvValue ();
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start, uint64_t valueLength);
  void Add (uint16_t portLow, uint16_t portHigh);
  Iterator Begin () const;
  Iterator End () const;
  virtual PortRangeTlvValue * Copy (void) const;
private:
  std::vector<struct PortRange> * m_portRange;
};

// ==============================================================================
/**
 * \ingroup wimax
 */
class ProtocolTlvValue : public TlvValue
{
public:
  ProtocolTlvValue ();
  ~ProtocolTlvValue ();
  typedef std::vector<uint8_t>::const_iterator Iterator;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start, uint64_t valueLength);
  void Add (uint8_t protiocol);
  Iterator Begin () const;
  Iterator End () const;
  virtual ProtocolTlvValue * Copy (void) const;
private:
  std::vector<uint8_t> * m_protocol;
};

// ==============================================================================

/**
 * \ingroup wimax
 */
class Ipv4AddressTlvValue : public TlvValue
{
public:
  struct ipv4Addr
  {
    Ipv4Address Address;
    Ipv4Mask Mask;
  };
  typedef std::vector<struct ipv4Addr>::const_iterator Iterator;
  Ipv4AddressTlvValue ();
  ~Ipv4AddressTlvValue ();
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start, uint64_t valueLength);
  void Add (Ipv4Address address, Ipv4Mask Mask);
  Iterator Begin () const;
  Iterator End () const;
  virtual Ipv4AddressTlvValue * Copy () const;
private:
  std::vector<struct ipv4Addr> * m_ipv4Addr;
};

}

#endif /* TLV_H_ */
