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

#ifndef UDP_HEADER_H
#define UDP_HEADER_H

#include <stdint.h>
#include "ns3/header.h"
#include "ipv4-address.h"

namespace ns3 {

class UdpHeader : public Header {
public:
  UdpHeader ();
  virtual ~UdpHeader ();

  static void EnableChecksums (void);

  void SetDestination (uint16_t port);
  void SetSource (uint16_t port);
  uint16_t GetSource (void) const;
  uint16_t GetDestination (void) const;

  void SetPayloadSize (uint16_t size);

  void InitializeChecksum (Ipv4Address source, 
                           Ipv4Address destination,
                           uint8_t protocol);

private:
  virtual void PrintTo (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void SerializeTo (Buffer::Iterator start) const;
  virtual void DeserializeFrom (Buffer::Iterator start);

  uint16_t m_sourcePort;
  uint16_t m_destinationPort;
  uint16_t m_payloadSize;
  uint16_t m_initialChecksum;

  static bool m_calcChecksum;
};

}; // namespace ns3

#endif /* UDP_HEADER */
