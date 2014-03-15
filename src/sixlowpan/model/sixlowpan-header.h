/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Universita' di Firenze, Italy
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
 * Author: Tommaso Pecorella <tommaso.pecorella@unifi.it>
 *         Michele Muccio <michelemuccio@virgilio.it>
 */

#ifndef SIXLOWPANHEADER_H_
#define SIXLOWPANHEADER_H_

#include "ns3/header.h"
#include "ns3/ipv6-address.h"
#include "ns3/packet.h"
#include "ns3/ipv6-header.h"

namespace ns3 {

/**
* \ingroup sixlowpan
* \brief   Dispatch header helper. This class only purpose is to interpret
* the Dispatch header into its correct type.
*
* The dispatch type is defined by a zero bit as the first bit and a one
*  bit as the second bit.
  \verbatim
                        1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |0 1| Dispatch  |  type-specific header
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  \endverbatim
*/
class SixLowPanDispatch
{
public:
  /**
   *  \brief Dispatch values, as defined in RFC4944 and RFC6282
  \verbatim
     Pattern    Header Type
   +------------+------------------------------------------------+
   | 00  xxxxxx | NALP        - Not a LoWPAN frame               |
   | 01  000000 | ESC         - Additional Dispatch byte follows |
   | 01  000001 | IPv6        - Uncompressed IPv6 Addresses      |
   | 01  000010 | LOWPAN_HC1  - LOWPAN_HC1 compressed IPv6       |
   | 01  000011 | reserved    - Reserved for future use          |
   |   ...      | reserved    - Reserved for future use          |
   | 01  001111 | reserved    - Reserved for future use          |
   | 01  010000 | LOWPAN_BC0  - LOWPAN_BC0 broadcast             |
   | 01  010001 | reserved    - Reserved for future use          |
   |   ...      | reserved    - Reserved for future use          |
   | 01  1xxxxx | LOWPAN_IPHC - LOWPAN_IPHC compressed IPv6      |
   | 10  xxxxxx | MESH        - Mesh Header                      |
   | 11  000xxx | FRAG1       - Fragmentation Header (first)     |
   | 11  001000 | reserved    - Reserved for future use          |
   |   ...      | reserved    - Reserved for future use          |
   | 11  011111 | reserved    - Reserved for future use          |
   | 11  100xxx | FRAGN       - Fragmentation Header (subsequent)|
   | 11  101000 | reserved    - Reserved for future use          |
   |   ...      | reserved    - Reserved for future use          |
   | 11  111111 | reserved    - Reserved for future use          |
   +------------+------------------------------------------------+
   \endverbatim
   */
  enum Dispatch_e
  {
    LOWPAN_NALP = 0x0,
    LOWPAN_NALP_N = 0x3F,
    LOWPAN_IPv6 = 0x41,
    LOWPAN_HC1 = 0x42,
    LOWPAN_BC0 = 0x50,
    LOWPAN_IPHC = 0x60,
    LOWPAN_IPHC_N = 0x7F,
    LOWPAN_MESH = 0x80,
    LOWPAN_MESH_N = 0xBF,
    LOWPAN_FRAG1 = 0xC0,
    LOWPAN_FRAG1_N = 0xC7,
    LOWPAN_FRAGN = 0xE0,
    LOWPAN_FRAGN_N = 0xE7,
    LOWPAN_UNSUPPORTED = 0xFF
  };

  /**
   *  \brief Dispatch values for Next Header compression.
   *
   *  The dispatch values reflect the dispatch use, since
   *  some dispatch bits carry actual header compression bits.
   */
  enum NhcDispatch_e
  {
    LOWPAN_NHC = 0xE0,
    LOWPAN_NHC_N = 0xEF,
    LOWPAN_UDPNHC = 0xF0,
    LOWPAN_UDPNHC_N = 0xF7,
    LOWPAN_NHCUNSUPPORTED = 0xFF
  };

  SixLowPanDispatch (void);

  /**
   * \brief Get the Dispatch type.
   * \param dispatch the dispatch value
   * \return the Dispatch type
   */
  static Dispatch_e GetDispatchType (uint8_t dispatch);

  /**
   * \brief Get the NhcDispatch type.
   * \param dispatch the dispatch value
   * \return the NhcDispatch type
   */
  static NhcDispatch_e GetNhcDispatchType (uint8_t dispatch);

};

/**
 * \ingroup sixlowpan
 * \brief 6LoWPAN HC1 header - see RFC 4944
 */
class SixLowPanHc1 : public Header
{
public:
  /**
   * \brief Kind of address compression.
   *
   * The address compression is handled in 4 bits and might mean:
   * PI: Prefix inline, PC: Prefix Compressed,
   * II: Interface Identifier, Inline, IC: Interface Identifier Compressed
   */
  enum LowPanHc1Addr_e
  {
    HC1_PIII = 0x00,
    HC1_PIIC = 0x01,
    HC1_PCII = 0x02,
    HC1_PCIC = 0x03
  };

  /**
   * \brief Next header information.
   *
   * The Next header compression is handled in 4 bits and might mean:
   * NC: Not Compressed, UDP, ICMP or TCP.
   */
  enum LowPanHc1NextHeader_e
  {
    HC1_NC = 0x00,
    HC1_UDP = 0x01,
    HC1_ICMP = 0x02,
    HC1_TCP = 0x03
  };

  SixLowPanHc1 (void);

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Return the instance type identifier.
   * \return instance type ID
   */

  virtual TypeId GetInstanceTypeId (void) const;

  virtual void Print (std::ostream& os) const;

  /**
   * \brief Get the serialized size of the packet.
   * \return size
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serialize the packet.
   * \param start Buffer iterator
   */
  virtual void Serialize (Buffer::Iterator start) const;

  /**
   * \brief Deserialize the packet.
   * \param start Buffer iterator
   * \return size of the packet
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * \brief Set the "Hop limit" field (TTL).
   * \param limit the hop limit value
   */
  void SetHopLimit (uint8_t limit);

  /**
   * \brief Get the "Hop limit" field (TTL).
   * \return the hop limit value
   */
  uint8_t GetHopLimit (void) const;

  /**
   * \brief Get Destination Compression type
   * \returns the kind of address compression
   */
  LowPanHc1Addr_e GetDstCompression () const;

  /**
   * \brief Get the destination interface
   * \returns the destination interface
   */
  const uint8_t* GetDstInterface () const;

  /**
   * \brief Get the destination prefix
   * \returns the destination prefix
   */
  const uint8_t* GetDstPrefix () const;

  /**
   * \brief Get the Flow Label value
   * \returns the Flow Label
   */
  uint32_t GetFlowLabel () const;

  /**
   * \brief Get the Next Header value
   * \returns the Next Header value
   */
  uint8_t GetNextHeader () const;

  /**
   * \brief Get Source Compression type
   * \returns the kind of address compression
   */
  LowPanHc1Addr_e GetSrcCompression () const;

  /**
   * \brief Get the source interface
   * \returns the source interface
   */
  const uint8_t* GetSrcInterface () const;

  /**
   * \brief Get the source prefix
   * \returns the source prefix
   */
  const uint8_t* GetSrcPrefix () const;

  /**
   * \brief Get the Traffic Class value
   * \returns the Traffic Class value
   */
  uint8_t GetTrafficClass () const;

  /**
   * \brief Check if the Traffic Class and Flow Labels are compressed
   * \returns true if TC and FL are compressed
   */
  bool IsTcflCompression () const;

  /**
   * \brief Check if there is a HC2 compressed header
   * \returns true if next header is HC2 copressed
   */
  bool IsHc2HeaderPresent () const;

  /**
   * \brief Set Destination Compression type
   * \param dstCompression the kind of address compression
   */
  void SetDstCompression (LowPanHc1Addr_e dstCompression);

  /**
   * \brief Set the destination interface
   * \param dstInterface the destination interface
   */
  void SetDstInterface (const uint8_t* dstInterface);

  /**
   * \brief Set the destination prefix
   * \param dstPrefix the destination prefix
   */
  void SetDstPrefix (const uint8_t* dstPrefix);

  /**
   * \brief Set the Flow Label value
   * \param flowLabel the Flow Label
   */
  void SetFlowLabel (uint32_t flowLabel);

  /**
   * \brief Set the Next Header value
   * \param nextHeader the Next Header value
   */
  void SetNextHeader (uint8_t nextHeader);

  /**
   * \brief Set Source Compression type
   * \param srcCompression the kind of address compression
   */
  void SetSrcCompression (LowPanHc1Addr_e srcCompression);

  /**
   * \brief Set the source interface
   * \param srcInterface the source interface
   */
  void SetSrcInterface (const uint8_t* srcInterface);

  /**
   * \brief Set the source prefix
   * \param srcPrefix the source prefix
   */
  void SetSrcPrefix (const uint8_t* srcPrefix);

  /**
   * \brief Set the Traffic Class and Flow Labels as compressed
   * \param tcflCompression true if TC and FL are compressed
   */
  void SetTcflCompression (bool tcflCompression);

  /**
   * \brief Set the next header a HC2 compressed header
   * \param hc2HeaderPresent true if next header is HC2 compressed
   */
  void SetHc2HeaderPresent (bool hc2HeaderPresent);

  /**
   * \brief Set the Traffic Class value
   * \param trafficClass the Traffic Class value
   */
  void SetTrafficClass (uint8_t trafficClass);

private:
  uint8_t m_hopLimit;           //!< Hop Limit
  uint8_t m_srcPrefix[8];       //!< Source prefix
  uint8_t m_srcInterface[8];    //!< Source interface
  uint8_t m_dstPrefix[8];       //!< Destinaiton prefix
  uint8_t m_dstInterface[8];    //!< Destination interface
  uint8_t m_trafficClass;       //!< Traffic Class
  uint32_t m_flowLabel;         //!< Flow Label
  uint8_t m_nextHeader;         //!< Next header
  LowPanHc1Addr_e m_srcCompression; //!< Source compresison type
  LowPanHc1Addr_e m_dstCompression; //!< Destination compresison type
  bool m_tcflCompression;       //!< is TC and FL compressed
  LowPanHc1NextHeader_e m_nextHeaderCompression; //!< next header compression
  bool m_hc2HeaderPresent;      //!< is next header HC2 compressed
};

/**
 * \brief Stream insertion operator.
 *
 * \param os the reference to the output stream
 * \param header the HC1 Header
 * \returns the reference to the output stream
 */
std::ostream & operator<< (std::ostream & os, SixLowPanHc1 const &header);

/**
 * \ingroup sixlowpan
 * \brief 6LoWPAN FRAG1 header - see RFC 4944
 */
class SixLowPanFrag1 : public Header
{
public:
  SixLowPanFrag1 (void);

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Return the instance type identifier.
   * \return instance type ID
   */
  virtual TypeId GetInstanceTypeId (void) const;

  virtual void Print (std::ostream& os) const;

  /**
   * \brief Get the serialized size of the packet.
   * \return size
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serialize the packet.
   * \param start Buffer iterator
   */
  virtual void Serialize (Buffer::Iterator start) const;

  /**
   * \brief Deserialize the packet.
   * \param start Buffer iterator
   * \return size of the packet
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * \brief Set the datagram size
   * \param datagramSize the datagram size
   */
  void SetDatagramSize (uint16_t datagramSize);

  /**
   * \brief Get the datagram size
   * \returns the datagram size
   */
  uint16_t GetDatagramSize (void) const;

  /**
   * \brief Set the datagram tag
   * \param datagramTag the datagram tag
   */
  void SetDatagramTag (uint16_t datagramTag);

  /**
   * \brief Get the datagram tag
   * \returns the datagram tag
   */
  uint16_t GetDatagramTag (void) const;

private:
  uint16_t m_datagramSize; //!< datagram size
  uint16_t m_datagramTag;  //!< datagram tag

};

/**
 * \brief Stream insertion operator.
 *
 * \param os the reference to the output stream
 * \param header the Frag1 Header
 * \returns the reference to the output stream
 */
std::ostream & operator<< (std::ostream & os, SixLowPanFrag1 const & header);

/**
 * \ingroup sixlowpan
 * \brief 6LoWPAN FRAGN header - see RFC 4944
 */
class SixLowPanFragN : public Header
{
public:
  SixLowPanFragN (void);

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Return the instance type identifier.
   * \return instance type ID
   */
  virtual TypeId GetInstanceTypeId (void) const;

  virtual void Print (std::ostream& os) const;

  /**
   * \brief Get the serialized size of the packet.
   * \return size
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serialize the packet.
   * \param start Buffer iterator
   */
  virtual void Serialize (Buffer::Iterator start) const;

  /**
   * \brief Deserialize the packet.
   * \param start Buffer iterator
   * \return size of the packet
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * \brief Set the datagram size
   * \param datagramSize the datagram size
   */
  void SetDatagramSize (uint16_t datagramSize);

  /**
   * \brief Get the datagram size
   * \returns the datagram size
   */
  uint16_t GetDatagramSize (void) const;

  /**
   * \brief Set the datagram tag
   * \param datagramTag the datagram tag
   */
  void SetDatagramTag (uint16_t datagramTag);

  /**
   * \brief Get the datagram tag
   * \returns the datagram tag
   */
  uint16_t GetDatagramTag (void) const;

  /**
   * \brief Set the datagram offset
   * \param datagramOffset the datagram offset
   */
  void SetDatagramOffset (uint8_t datagramOffset);

  /**
   * \brief Get the datagram offset
   * \returns the datagram offset
   */
  uint8_t GetDatagramOffset (void) const;

private:
  uint16_t m_datagramSize;  //!< datagram size
  uint16_t m_datagramTag;   //!< datagram tag
  uint8_t m_datagramOffset; //!< datagram offset

};

/**
 * \brief Stream insertion operator.
 *
 * \param os the reference to the output stream
 * \param header the FragN Header
 * \returns the reference to the output stream
 */
std::ostream & operator<< (std::ostream & os, SixLowPanFragN const &header);

/**
 * \ingroup sixlowpan
 * \brief 6LoWPAN IPv6 uncomprssed header - see RFC 4944
 */
class SixLowPanIpv6 : public Header
{
public:
  SixLowPanIpv6 (void);

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Return the instance type identifier.
   * \return instance type ID
   */
  virtual TypeId GetInstanceTypeId (void) const;

  virtual void Print (std::ostream& os) const;

  /**
   * \brief Get the serialized size of the packet.
   * \return size
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serialize the packet.
   * \param start Buffer iterator
   */
  virtual void Serialize (Buffer::Iterator start) const;

  /**
   * \brief Deserialize the packet.
   * \param start Buffer iterator
   * \return size of the packet
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);

};

/**
 * \brief Stream insertion operator.
 *
 * \param os the reference to the output stream
 * \param header the Frag1 Header
 * \returns the reference to the output stream
 */
std::ostream & operator<< (std::ostream & os, SixLowPanIpv6 const & header);

/**
* \ingroup sixlowpan
* \brief   LOWPAN_IPHC base Encoding - see RFC 6282
  \verbatim
       0                                       1
       0   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5
     +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     | 0 | 1 | 1 |  TF   |NH | HLIM  |CID|SAC|  SAM  | M |DAC|  DAM  |
     +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
  \endverbatim
*/
class SixLowPanIphc : public Header
{
public:
  /**
   *  \brief TF: Traffic Class, Flow Label
   *
   *  00:  ECN + DSCP + 4-bit Pad + Flow Label (4 bytes)
   *  01:  ECN + 2-bit Pad + Flow Label (3 bytes), DSCP is elided.
   *  10:  ECN + DSCP (1 byte), Flow Label is elided.
   *  11:  Traffic Class and Flow Label are elided.
   *
   */
  enum TrafficClassFlowLabel_e
  {
    TF_FULL = 0,
    TF_DSCP_ELIDED,
    TF_FL_ELIDED,
    TF_ELIDED
  };

  /**
   *  \brief HLIM: Hop Limit
   *
   *  00:  The Hop Limit field is carried in-line.
   *  01:  The Hop Limit field is compressed and the hop limit is 1.
   *  10:  The Hop Limit field is compressed and the hop limit is 64.
   *  11:  The Hop Limit field is compressed and the hop limit is 255.
   */
  enum Hlim_e
  {
    HLIM_INLINE = 0,
    HLIM_COMPR_1,
    HLIM_COMPR_64,
    HLIM_COMPR_255
  };

  /**
   *  \brief Source or Destination Address Mode
   *
   *  00:  128 bits.
   *  01:  64 bits (or 48 bits if multicast).
   *  10:  16 bits (or 32 bits if multicast).
   *  11:  Fully elided (or 8 bits if multicast).
   */
  enum HeaderCompression_e
  {
    HC_INLINE = 0,
    HC_COMPR_64,
    HC_COMPR_16,
    HC_COMPR_0
  };

  SixLowPanIphc (void);
  /**
   * \brief Constructor
   * \param dispatch dispatch value
   */
  SixLowPanIphc (uint8_t dispatch);

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Return the instance type identifier.
   * \return instance type ID
   */
  virtual TypeId GetInstanceTypeId (void) const;

  virtual void Print (std::ostream& os) const;

  /**
   * \brief Get the serialized size of the packet.
   * \return size
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serialize the packet.
   * \param start Buffer iterator
   */
  virtual void Serialize (Buffer::Iterator start) const;

  /**
   * \brief Deserialize the packet.
   * \param start Buffer iterator
   * \return size of the packet
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * \brief Set the TF (Traffic Class, Flow Label) compression.
   * \param tfField ECN, DSCP, Flow Label compression type
   */
  void SetTf (TrafficClassFlowLabel_e tfField);

  /**
   * \brief Get the TF (Traffic Class, Flow Label) compression.
   * \return the ECN, DSCP, Flow Label compression type
   */
  TrafficClassFlowLabel_e GetTf (void) const;

  /**
   * \brief Set the NH (Next Header) compression.
   * \param nhField false (Next Header carried in-line), true (compressed NH)
   */
  void SetNh (bool nhField);

  /**
   * \brief Get the NH (Next Header) compression.
   * \return false (Next Header carried in-line), true (compressed NH)
   */
  bool GetNh (void) const;

  /**
   * \brief Set the HLIM (Hop Limit) compression.
   * \param hlimField Hop Limit compression type
   */
  void SetHlim (Hlim_e hlimField);

  /**
   * \brief Get the HLIM (Hop Limit) compression.
   * \return Hop Limit compression type
   */
  Hlim_e GetHlim (void) const;

  /**
   * \brief Set the CID (Context Identifier Extension) compression.
   * \param cidField false (no CID present), true (CID follows)
   */
  void SetCid (bool cidField);

  /**
   * \brief Get the CID (Context Identifier Extension) compression.
   * \return false (no CID present), true (CID follows)
   */
  bool GetCid (void) const;

  /**
   * \brief Set the SAC (Source Address Compression) compression.
   * \param sacField false (stateless), true (stateful)
   */
  void SetSac (bool sacField);

  /**
   * \brief Get the SAC (Source Address Compression) compression.
   * \return false (stateless), true (stateful)
   */
  bool GetSac (void) const;

  /**
   * \brief Set the SAM (Source Address Mode) compression.
   * \param samField - depends on the SAC
   */
  void SetSam (HeaderCompression_e samField);

  /**
   * \brief Get the SAM (Source Address Mode) compression.
   * \return depends on the SAC field
   */
  HeaderCompression_e GetSam (void) const;

  /**
   * \brief Set the M (Multicast) compression.
   * \param mField true if destination is multicast
   */
  void SetM (bool mField);

  /**
   * \brief Get the M (Multicast) compression.
   * \return true if destination is multicast
   */
  bool GetM (void) const;

  /**
   * \brief Set the DAC (Destination Address Compression) compression.
   * \param dacField false (stateless), true (stateful)
   */
  void SetDac (bool dacField);

  /**
   * \brief Get the DAC (Destination Address Compression) compression.
   * \return false (stateless), true (stateful)
   */
  bool GetDac (void) const;

  /**
   * \brief Set the DAM (Destination Address Mode) compression.
   * \param damField - depends on the DAC and M fields
   */
  void SetDam (HeaderCompression_e damField);

  /**
   * \brief Get the DAM (Destination Address Mode) compression.
   * \return depends on the DAC and M fields
   */
  HeaderCompression_e GetDam (void) const;

  /**
   * \brief Set the SrcContextId.
   * \param srcContextId - valid values are [0:15]
   */
  void SetSrcContextId (uint8_t srcContextId);

  /**
   * \brief Get the SrcContextId.
   * \return the SrcContextId
   */
  uint8_t GetSrcContextId (void) const;

  /**
   * \brief Set the DstContextId.
   * \param dstContextId - valid values are [0:15]
   */
  void SetDstContextId (uint8_t dstContextId);

  /**
   * \brief Get the DstContextId.
   * \return the DstContextId
   */
  uint8_t GetDstContextId (void) const;

  /**
   * \brief Set the ECN (2bits).
   * \param ecn - valid values are [0:3]
   */
  void SetEcn (uint8_t ecn);

  /**
   * \brief Get the ECN.
   * \return the ECN
   */
  uint8_t GetEcn (void) const;

  /**
   * \brief Set the DSCP (6bits).
   * \param dscp - valid values are [0:63]
   */
  void SetDscp (uint8_t dscp);

  /**
   * \brief Get the DSCP.
   * \return the DSCP
   */
  uint8_t GetDscp (void) const;

  /**
   * \brief Set the Flow Label (20bits).
   * \param flowLabel - valid values are 20 bits long.
   */
  void SetFlowLabel (uint32_t flowLabel);

  /**
   * \brief Get the Flow Label.
   * \return the Flow Label
   */
  uint32_t GetFlowLabel (void) const;

  /**
   * \brief Set the Next Header field.
   * \param nextHeader Next Header field.
   */
  void SetNextHeader (uint8_t nextHeader);

  /**
   * \brief Get the Next Header field.
   * \return the Next Header field.
   */
  uint8_t GetNextHeader (void) const;

  /**
   * \brief Set the Hop Limit field.
   * \param hopLimit Hop Limit field.
   */
  void SetHopLimit (uint8_t hopLimit);

  /**
   * \brief Get the Hop Limit field.
   * \return the Hop Limit field.
   */
  uint8_t GetHopLimit (void) const;

  /**
   * \brief Set the Source Address.
   * \param srcAddress the Source Address.
   */
  void SetSrcAddress (Ipv6Address srcAddress);

  /**
   * \brief Get the Source Address.
   * \return the Source Address.
   */
  Ipv6Address GetSrcAddress () const;

  /**
   * \brief Set the Destination Address.
   * \param dstAddress the Destination Address.
   */
  void SetDstAddress (Ipv6Address dstAddress);

  /**
   * \brief Get the Destination Address.
   * \return the Destination Address.
   */
  Ipv6Address GetDstAddress () const;

private:
  uint16_t m_baseFormat;      //!< Dispatch + encoding fields
  uint8_t m_srcdstContextId;  //!< Src and Dst Context ID
  uint8_t m_ecn : 2;          //!< ECN bits
  uint8_t m_dscp : 6;         //!< DSCP bits
  uint32_t m_flowLabel : 20;  //!< Flow Label bits
  uint8_t m_nextHeader;       //!< Next header
  uint8_t m_hopLimit;         //!< Hop Limit
  Ipv6Address m_srcAddress;   //!< Src address
  Ipv6Address m_dstAddress;   //!< Dst address

  /**
   * \brief Post-process the Source address stateful compression
   * \note currently unsupported
   */
  void PostProcessSac ();
  /**
   * \brief Post-process the Destination address stateful compression
   * \note currently unsupported
   */
  void PostProcessDac ();

};

/**
 * \brief Stream insertion operator.
 *
 * \param os the reference to the output stream
 * \param header the IPHC Header
 * \returns the reference to the output stream
 */
std::ostream & operator<< (std::ostream & os, SixLowPanIphc const &header);

/**
* \ingroup sixlowpan
* \brief   LOWPAN_NHC Extension Header Encoding - see RFC 6282
  \verbatim
     0   1   2   3   4   5   6   7
   +---+---+---+---+---+---+---+---+
   | 1 | 1 | 1 | 0 |    EID    |NH |
   +---+---+---+---+---+---+---+---+
  \endverbatim
*/
class SixLowPanNhcExtension : public Header
{
public:
  /**
   *  \brief EID: IPv6 Extension Header ID
   *
   *   EID: IPv6 Extension Header ID:
   *      0: IPv6 Hop-by-Hop Options Header [RFC2460]
   *      1: IPv6 Routing Header [RFC2460]
   *      2: IPv6 Fragment Header [RFC2460]
   *      3: IPv6 Destination Options Header [RFC2460]
   *      4: IPv6 Mobility Header [RFC6275]
   *      5: Reserved
   *      6: Reserved
   *      7: IPv6 Header
   */
  enum Eid_e
  {
    EID_HOPBYHOP_OPTIONS_H = 0,
    EID_ROUTING_H,
    EID_FRAGMENTATION_H,
    EID_DESTINATION_OPTIONS_H,
    EID_MOBILITY_H,
    EID_IPv6_H = 7
  };

  SixLowPanNhcExtension (void);

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Return the instance type identifier.
   * \return instance type ID
   */
  virtual TypeId GetInstanceTypeId (void) const;

  virtual void Print (std::ostream& os) const;

  /**
   * \brief Get the serialized size of the packet.
   * \return size
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serialize the packet.
   * \param start Buffer iterator
   */
  virtual void Serialize (Buffer::Iterator start) const;

  /**
   * \brief Deserialize the packet.
   * \param start Buffer iterator
   * \return size of the packet
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * \brief Get the NhcDispatch type.
   * \return the NhcDispatch type
   */
  virtual SixLowPanDispatch::NhcDispatch_e GetNhcDispatchType (void) const;

  /**
   * \brief Set the Extension Header Type.
   * \param extensionHeaderType the Extension Header Type
   */
  void SetEid (Eid_e extensionHeaderType);

  /**
   * \brief Get the Extension Header Type.
   * \return the Extension Header Type
   */
  Eid_e GetEid (void) const;

  /**
   * \brief Set the Next Header field values.
   * \param nextHeader the Next Header field value
   */
  void SetNextHeader (uint8_t nextHeader);

  /**
   * \brief Get the Next Header field value.
   * \return the Next Header field value
   */
  uint8_t GetNextHeader (void) const;

  /**
   * \brief Set the NH field values.
   * \param nhField the NH field value
   */
  void SetNh (bool nhField);

  /**
   * \brief Get the Next Header field value.
   * \return the NH field value
   */
  bool GetNh (void) const;

  /**
   * \brief Set the option header data blob.
   * \param blob a buffer holding the blob data
   * \param size the data blob size
   */
  void SetBlob (const uint8_t* blob, uint32_t size);

  /**
   * \brief Get the option header data blob.
   * \param blob a buffer to copy the blob data into
   * \param size the size of the buffer
   * \return the length of the copied data
   */
  uint32_t CopyBlob (uint8_t* blob, uint32_t size) const;

private:
  uint8_t m_nhcExtensionHeader; //!< NHC extension header type
  uint8_t m_nhcNextHeader;      //!< Next header
  uint8_t m_nhcBlobLength;      //!< Length of the NHC compressed header
  uint8_t m_nhcBlob[256];       //!< NHC compressed header
};

/**
 * \brief Stream insertion operator.
 *
 * \param os the reference to the output stream
 * \param header the NHC Extension Header
 * \returns the reference to the output stream
 */
std::ostream & operator<< (std::ostream & os, SixLowPanNhcExtension const &header);


/**
* \ingroup sixlowpan
* \brief   UDP LOWPAN_NHC Extension Header Encoding - see RFC 6282
  \verbatim
     0   1   2   3   4   5   6   7
   +---+---+---+---+---+---+---+---+
   | 1 | 1 | 1 | 1 | 0 | C |   P   |
   +---+---+---+---+---+---+---+---+
  \endverbatim
*/
class SixLowPanUdpNhcExtension : public Header
{
public:
  /**
   *  \brief Ports:
   *
   *  00: 16 bits for both Source Port and Destination Port
   *  01: 16 bits for Source Port. Last 8 bits for Destination Port
   *  10: Last 8 bits for Source Port. All 16 bits for Destination Port
   *  11: Last 4 bits of both Source Port and Destination Port
   */
  enum Ports_e
  {
    PORTS_INLINE = 0,
    PORTS_ALL_SRC_LAST_DST,
    PORTS_LAST_SRC_ALL_DST,
    PORTS_LAST_SRC_LAST_DST
  };

  SixLowPanUdpNhcExtension (void);

  /**
   * \brief Return the instance type identifier.
   * \return instance type ID
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Return the instance type identifier.
   * \return instance type ID
   */
  virtual TypeId GetInstanceTypeId (void) const;

  virtual void Print (std::ostream& os) const;

  /**
   * \brief Get the serialized size of the packet.
   * \return size
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serialize the packet.
   * \param start Buffer iterator
   */
  virtual void Serialize (Buffer::Iterator start) const;

  /**
   * \brief Deserialize the packet.
   * \param start Buffer iterator
   * \return size of the packet
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * \brief Get the NhcDispatch type.
   * \return the NhcDispatch type
   */
  virtual SixLowPanDispatch::NhcDispatch_e GetNhcDispatchType (void) const;

  /**
   * \brief Set the compressed Src and Dst Ports.
   * \param port Src and Dst the Ports
   */
  void SetPorts (Ports_e port);

  /**
   * \brief Get the compressed Src and Dst Ports.
   * \return the Src and Dst Ports
   */
  Ports_e GetPorts (void) const;

  /**
   * \brief Set the Destination Port.
   * \param port the Destination Port.
   */
  void SetSrcPort (uint16_t port);

  /**
   * \brief Get the Destination Port.
   * \return the Destination Port.
   */
  uint16_t GetSrcPort () const;

  /**
   * \brief Set the Destination Port.
   * \param port the Destination Port.
   */
  void SetDstPort (uint16_t port);

  /**
   * \brief Get the Destination Port.
   * \return the Destination Port.
   */
  uint16_t GetDstPort () const;

  /**
   * \brief Set the C (Checksum).
   * \param cField false (All checksum carried in-line), true (Checksum elided)
   */
  void SetC (bool cField);

  /**
   * \brief Get the C (Checksum).
   * \return false (All checksum carried in-line), true (Checksum elided)
   */
  bool GetC (void) const;

  /**
   * \brief Set the Checksum field values.
   * \param checksum the Checksum field value
   */
  void SetChecksum (uint16_t checksum);

  /**
   * \brief Get the Checksum field value.
   * \return the Checksum field value
   */
  uint16_t GetChecksum (void) const;

private:
  uint8_t m_baseFormat; //!< Dispatch + encoding fields
  uint16_t m_checksum;  //!< Checksum
  uint16_t m_srcPort;   //!< Source port
  uint16_t m_dstPort;   //!< Destination port
};

/**
 * \brief Stream insertion operator.
 *
 * \param os the reference to the output stream
 * \param header the UDP NHC Extension Header
 * \returns the reference to the output stream
 */
std::ostream & operator<< (std::ostream & os, SixLowPanUdpNhcExtension const &header);

}

#endif /* SIXLOWPANHEADER_H_ */
