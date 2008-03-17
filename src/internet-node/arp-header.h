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

#ifndef ARP_HEADER_H
#define ARP_HEADER_H

#include "ns3/header.h"
#include "ns3/address.h"
#include "ns3/ipv4-address.h"
#include <string>

namespace ns3 {
/**
 * \brief The packet header for an ARP packet
 */
class ArpHeader : public Header 
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  static uint32_t GetUid (void);

  void SetRequest (Address sourceHardwareAddress,
                   Ipv4Address sourceProtocolAddress,
                   Address destinationHardwareAddress,
                   Ipv4Address destinationProtocolAddress);
  void SetReply (Address sourceHardwareAddress,
                 Ipv4Address sourceProtocolAddress,
                 Address destinationHardwareAddress,
                 Ipv4Address destinationProtocolAddress);
  bool IsRequest (void) const;
  bool IsReply (void) const;
  Address GetSourceHardwareAddress (void);
  Address GetDestinationHardwareAddress (void);
  Ipv4Address GetSourceIpv4Address (void);
  Ipv4Address GetDestinationIpv4Address (void);

  std::string GetName (void) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);

  enum ArpType_e {
    ARP_TYPE_REQUEST = 1,
    ARP_TYPE_REPLY   = 2
  };
  uint16_t m_type;
  Address m_macSource;
  Address m_macDest;
  Ipv4Address m_ipv4Source;
  Ipv4Address m_ipv4Dest;
};

}; // namespace ns3

#endif /* ARP_HEADER_H */
