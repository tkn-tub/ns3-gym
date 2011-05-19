/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Hajime Tazaki
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
 * Authors: Hajime Tazaki <tazaki@sfc.wide.ad.jp>
 */

#ifndef __IPV4_PACKET_INFO_TAG_H__
#define __IPV4_PACKET_INFO_TAG_H__

#include "ns3/tag.h"
#include "ns3/ipv4-address.h"

namespace ns3 {


class Node;
class Packet;

/**
 * \brief This class implements Linux struct pktinfo 
 * in order to deliver ancillary information to the socket interface.
 * This is used with socket option such as IP_PKTINFO, IP_RECVTTL, 
 * IP_RECVTOS. See linux manpage ip(7).
 *
 * This tag in the send direction is presently not enabled but we
 * would accept a patch along those lines in the future.
 */
class Ipv4PacketInfoTag : public Tag
{
public:
  Ipv4PacketInfoTag ();
  // Implemented, but not used in the stack yet
  void SetAddress (Ipv4Address addr);
  // Implemented, but not used in the stack yet
  Ipv4Address GetAddress (void) const;
  // This corresponds to "ipi_spec_dst" in struct in_pktinfo.
  // Implemented, but not used in the stack yet
  void SetLocalAddress (Ipv4Address addr);
  // This corresponds to "ipi_spec_dst" in struct in_pktinfo.
  // Implemented, but not used in the stack yet
  Ipv4Address GetLocalAddress (void) const;
  void SetRecvIf (uint32_t ifindex);
  uint32_t GetRecvIf (void) const;
  // Implemented, but not used in the stack yet
  void SetTtl (uint8_t ttl);
  // Implemented, but not used in the stack yet
  uint8_t GetTtl (void) const;

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

private:
  // Linux IP_PKTINFO ip(7) implementation
  // 
  // struct in_pktinfo {
  //   unsigned int   ipi_ifindex;  /* Interface index */
  //   struct in_addr ipi_spec_dst; /* Local address */
  //   struct in_addr ipi_addr;     /* Header Destination
  //                                   address */
  // };

  Ipv4Address m_addr;
  Ipv4Address m_spec_dst;
  uint32_t m_ifindex;

  // Uset for IP_RECVTTL, though not implemented yet.
  uint8_t m_ttl;
};
} //namespace ns3

#endif /* __IPV4_PACKET_INFO_TAG_H__ */

