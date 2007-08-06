/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
 * All rights reserved.
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
  static uint32_t GetUid (void);
  /**
   * \brief Construct a null IPv4 header
   */
  Ipv4Header ();
  virtual ~Ipv4Header ();
  /**
   * \brief Enable checksum calculation for IP (XXX currently has no effect)
   */
  static void EnableChecksums (void);
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
   * \param offset the ipv4 fragment offset
   */
  void SetFragmentOffset (uint16_t offset);
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
   * \returns true if this is the last fragment of a packet, false otherwise.
   */
  bool IsLastFragment (void) const;
  /**
   * \returns true if this is this packet can be fragmented.
   */  
  bool IsDontFragment (void) const;
  /**
   * \returns the offset of this fragment.
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
   * \returns true if the upv4 checksum is correct, false otherwise.
   *
   * If Ipv4Header::EnableChecksums has not been called prior to
   * creating this packet, this method will always return true.
   */
  bool IsChecksumOk (void) const;

private:
  virtual std::string DoGetName (void) const;
  virtual void PrintTo (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void SerializeTo (Buffer::Iterator start) const;
  virtual uint32_t DeserializeFrom (Buffer::Iterator start);

  enum FlagsE {
    DONT_FRAGMENT = (1<<0),
    MORE_FRAGMENTS = (1<<1)
  };

  static bool m_calcChecksum;

  uint16_t m_payloadSize;
  uint16_t m_identification;
  uint32_t m_tos : 8;
  uint32_t m_ttl : 8;
  uint32_t m_protocol : 8;
  uint32_t m_flags : 3;
  uint16_t m_fragmentOffset : 13;
  Ipv4Address m_source;
  Ipv4Address m_destination;
  bool m_goodChecksum;
};

}; // namespace ns3


#endif /* IPV4_HEADER_H */
