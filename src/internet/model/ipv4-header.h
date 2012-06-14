/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef IPV4_HEADER_H
#define IPV4_HEADER_H

#include "ns3/header.h"
#include "ns3/ipv4-address.h"

namespace ns3 {
/**
 * \brief Packet header for IPv4
 */
class Ipv4Header : public Header 
{
public:
  /**
   * \brief Construct a null IPv4 header
   */
  Ipv4Header ();
  /**
   * \brief Enable checksum calculation for this header.
   */
  void EnableChecksum (void);
  /**
   * \param size the size of the payload in bytes
   */
  void SetPayloadSize (uint16_t size);
  /**
   * \param identification the Identification field of IPv4 packets.
   *
   * By default, set to zero.
   */
  void SetIdentification (uint16_t identification);
  /**
   * \param tos the 8 bits of Ipv4 TOS.
   */
  void SetTos (uint8_t tos);
  /**
   * \enum DscpType
   * \brief DiffServ Code Points 
   * Code Points defined in
   * Assured Forwarding (AF) RFC 2597
   * Expedited Forwarding (EF) RFC 2598
   * Default and Class Selector (CS) RFC 2474
   */
  enum DscpType
    {
      DscpDefault = 0x00,

      CS1 = 0x20,
      AF11 = 0x28,
      AF12 = 0x30,
      AF13 = 0x38,

      CS2 = 0x40,
      AF21 = 0x48,
      AF22 = 0x50,
      AF23 = 0x58,

      CS3 = 0x60,
      AF31 = 0x68,
      AF32 = 0x70,
      AF33 = 0x78,

      CS4 = 0x80,
      AF41 = 0x88,
      AF42 = 0x90,
      AF43 = 0x98,

      CS5 = 0xA0,
      EF = 0xB8,
      
      CS6 = 0xC0,
      CS7 = 0xE0
      
    };
  /**
   * \brief Set DSCP Field
   * \param dscp DSCP value
   */
  void SetDscp (DscpType dscp);

  /**
   * \enum EcnType
   * \brief ECN Type defined in RFC 3168
   */
  enum EcnType
    {
      NotECT = 0x00,
      ECT1 = 0x01,
      ECT0 = 0x02,
      CE = 0x03
    }; 
  /**
   * \brief Set ECN Field
   * \param ecn ECN Type
   */
  void SetEcn (EcnType ecn);
  /**
   * This packet is not the last packet of a fragmented ipv4 packet.
   */
  void SetMoreFragments (void);
  /**
   * This packet is the last packet of a fragmented ipv4 packet.
   */
  void SetLastFragment (void);
  /**
   * Don't fragment this packet: if you need to anyway, drop it.
   */
  void SetDontFragment (void);
  /**
   * If you need to fragment this packet, you can do it.
   */
  void SetMayFragment (void);
  /**
   * The offset is measured in bytes for the packet start.
   * Mind that IPv4 "fragment offset" field is 13 bits long and is measured in 8-bytes words.
   * Hence, the function does enforce that the offset is a multiple of 8.
   * \param offsetBytes the ipv4 fragment offset measured in bytes from the start.
   */
  void SetFragmentOffset (uint16_t offsetBytes);
  /**
   * \param ttl the ipv4 TTL
   */
  void SetTtl (uint8_t ttl);
  /**
   * \param num the ipv4 protocol field
   */
  void SetProtocol (uint8_t num);
  /**
   * \param source the source of this packet
   */
  void SetSource (Ipv4Address source);
  /**
   * \param destination the destination of this packet.
   */
  void SetDestination (Ipv4Address destination);
  /**
   * \returns the size of the payload in bytes
   */
  uint16_t GetPayloadSize (void) const;
  /**
   * \returns the identification field of this packet.
   */
  uint16_t GetIdentification (void) const;
  /**
   * \returns the TOS field of this packet.
   */
  uint8_t GetTos (void) const;
  /**
   * \returns the DSCP field of this packet.
   */
  DscpType GetDscp (void) const;
  /**
   * \returns std::string of DSCPType
   */
  std::string DscpTypeToString (DscpType dscp) const;
  /**
   * \returns the ECN field of this packet.
   */
  EcnType GetEcn (void) const;
  /**
   * \returns std::string of ECNType
   */
  std::string EcnTypeToString (EcnType ecn) const;
  /**
   * \returns true if this is the last fragment of a packet, false otherwise.
   */
  bool IsLastFragment (void) const;
  /**
   * \returns true if this is this packet can be fragmented.
   */
  bool IsDontFragment (void) const;
  /**
   * \returns the offset of this fragment measured in bytes from the start.
   */
  uint16_t GetFragmentOffset (void) const;
  /**
   * \returns the TTL field of this packet
   */
  uint8_t GetTtl (void) const;
  /**
   * \returns the protocol field of this packet
   */
  uint8_t GetProtocol (void) const;
  /**
   * \returns the source address of this packet
   */
  Ipv4Address GetSource (void) const;
  /**
   * \returns the destination address of this packet
   */
  Ipv4Address GetDestination (void) const;

  /**
   * \returns true if the ipv4 checksum is correct, false otherwise.
   *
   * If Ipv4Header::EnableChecksums has not been called prior to
   * deserializing this header, this method will always return true.
   */
  bool IsChecksumOk (void) const;

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
private:

  enum FlagsE {
    DONT_FRAGMENT = (1<<0),
    MORE_FRAGMENTS = (1<<1)
  };

  bool m_calcChecksum;

  uint16_t m_payloadSize;
  uint16_t m_identification;
  uint32_t m_tos : 8; //Also used as DSCP + ECN value
  uint32_t m_ttl : 8;
  uint32_t m_protocol : 8;
  uint32_t m_flags : 3;
  uint16_t m_fragmentOffset;
  Ipv4Address m_source;
  Ipv4Address m_destination;
  uint16_t m_checksum;
  bool m_goodChecksum;
  uint16_t m_headerSize;
};

} // namespace ns3


#endif /* IPV4_HEADER_H */
