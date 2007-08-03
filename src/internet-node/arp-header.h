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
#include "ns3/mac-address.h"
#include "ns3/ipv4-address.h"

namespace ns3 {
/**
 * \brief The packet header for an ARP packet
 */
class ArpHeader : public Header 
{
public:
  static const char *GetUid (void);

  virtual ~ArpHeader ();

  void SetRequest (MacAddress sourceHardwareAddress,
                   Ipv4Address sourceProtocolAddress,
                   MacAddress destinationHardwareAddress,
                   Ipv4Address destinationProtocolAddress);
  void SetReply (MacAddress sourceHardwareAddress,
                 Ipv4Address sourceProtocolAddress,
                 MacAddress destinationHardwareAddress,
                 Ipv4Address destinationProtocolAddress);
  bool IsRequest (void) const;
  bool IsReply (void) const;
  MacAddress GetSourceHardwareAddress (void);
  MacAddress GetDestinationHardwareAddress (void);
  Ipv4Address GetSourceIpv4Address (void);
  Ipv4Address GetDestinationIpv4Address (void);

private:
  virtual std::string DoGetName (void) const;
  /**
   * \param os
   */
  virtual void PrintTo (std::ostream &os) const;
  /**
   * \return
   */
  virtual uint32_t GetSerializedSize (void) const;
  /**
   * \param start
   */
  virtual void SerializeTo (Buffer::Iterator start) const;
  /**
   * \param start
   * \return
   */
  virtual uint32_t DeserializeFrom (Buffer::Iterator start);

  enum ArpType_e {
    ARP_TYPE_REQUEST = 1,
    ARP_TYPE_REPLY   = 2
  };
  uint16_t m_type;
  MacAddress m_macSource;
  MacAddress m_macDest;
  Ipv4Address m_ipv4Source;
  Ipv4Address m_ipv4Dest;
};

}; // namespace ns3

#endif /* ARP_HEADER_H */
