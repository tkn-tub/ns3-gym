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

class Ipv4Header : public Header {
public:
  Ipv4Header ();
  virtual ~Ipv4Header ();

  static void EnableChecksums (void);

  void SetPayloadSize (uint16_t size);
  void SetIdentification (uint16_t identification);
  void SetTos (uint8_t);
  void SetMoreFragments (void);
  void SetLastFragment (void);
  void SetDontFragment (void);
  void SetMayFragment (void);
  void SetFragmentOffset (uint16_t offset);
  void SetTtl (uint8_t);
  void SetProtocol (uint8_t);
  void SetSource (Ipv4Address source);
  void SetDestination (Ipv4Address destination);


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
