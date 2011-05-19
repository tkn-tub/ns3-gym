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

#ifndef __IPV6_PACKET_INFO_TAG_H__
#define __IPV6_PACKET_INFO_TAG_H__

#include "ns3/tag.h"
#include "ns3/ipv6-address.h"

namespace ns3 {


class Node;
class Packet;

/**
 * \brief This class implements a tag that carries socket ancillary 
 * data to the socket interface. This is used like 
 * socket option of IP_PKTINFO/IPV6_PKTINFO in RFC 3542
 * See: http://tools.ietf.org/html/rfc3542
 *
 * This tag in the send direction is presently not enabled but we
 * would accept a patch along those lines in the future. To include
 * the nexthop in the send direction would increase the size of the
 * tag beyond 20 bytes, so in that case, we recommend that an
 * additional tag be used to carry the IPv6 next hop address.
 */
class Ipv6PacketInfoTag : public Tag
{
public:
  Ipv6PacketInfoTag ();
  // Implemented, but not used in the stack yet
  void SetAddress (Ipv6Address addr);
  // Implemented, but not used in the stack yet
  Ipv6Address GetAddress (void) const;
  void SetRecvIf (uint32_t ifindex);
  uint32_t GetRecvIf (void) const;
  // Implemented, but not used in the stack yet
  void SetHoplimit (uint8_t ttl);
  // Implemented, but not used in the stack yet
  uint8_t GetHoplimit (void) const;
  // Implemented, but not used in the stack yet
  void SetTrafficClass (uint8_t tclass);
  // Implemented, but not used in the stack yet
  uint8_t GetTrafficClass (void) const;

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

private:
  /* 
   * RFC 3542 includes
   * for outgoing packet,
   *  1.  the source IPv6 address,
   *  2.  the outgoing interface index,
   *  3.  the outgoing hop limit,
   *  4.  the next hop address, and
   *  5.  the outgoing traffic class value.
   *
   * for incoming packet,
   *  1.  the destination IPv6 address,
   *  2.  the arriving interface index,
   *  3.  the arriving hop limit, and
   *  4.  the arriving traffic class value.
  */
  Ipv6Address m_addr;
  uint8_t m_ifindex;
  uint8_t m_hoplimit;
  uint8_t m_tclass;
};
} //namespace ns3

#endif /* __IPV6_PACKET_INFO_TAG_H__ */

