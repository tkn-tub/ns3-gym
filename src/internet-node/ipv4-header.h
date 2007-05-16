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
class Ipv4Header : public Header {
public:
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
   * \param size
   */
  void SetPayloadSize (uint16_t size);
  /**
   * \param identification
   */
  void SetIdentification (uint16_t identification);
  /**
   * \param tos
   */
  void SetTos (uint8_t tos);
  /**
   *
   */
  void SetMoreFragments (void);
  /**
   *
   */
  void SetLastFragment (void);
  /**
   *
   */
  void SetDontFragment (void);
  /**
   *
   */
  void SetMayFragment (void);
  /**
   * \param offset
   */
  void SetFragmentOffset (uint16_t offset);
  /**
   * \param ttl
   */
  void SetTtl (uint8_t ttl);
  /**
   * \param num
   */
  void SetProtocol (uint8_t num);
  /**
   * \param source
   */
  void SetSource (Ipv4Address source);
  /**
   * \param destination
   */
  void SetDestination (Ipv4Address destination);
  /**
   * \param 
   */


  uint16_t GetPayloadSize (void) const;
  uint16_t GetIdentification (void) const;
  uint8_t GetTos (void) const;
  bool IsLastFragment (void) const;
  bool IsDontFragment (void) const;
  uint16_t GetFragmentOffset (void) const;
  uint8_t GetTtl (void) const;
  uint8_t GetProtocol (void) const;
  Ipv4Address GetSource (void) const;
  Ipv4Address GetDestination (void) const;
  
  bool IsChecksumOk (void) const;

private:
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
