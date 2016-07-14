/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Universita' degli Studi di Napoli Federico II
 *               2016 University of Washington
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
 * Authors:  Stefano Avallone <stavallo@unina.it>
 *           Tom Henderson <tomhend@u.washington.edu>
 */

#ifndef IPV4_PACKET_FILTER_H
#define IPV4_PACKET_FILTER_H

#include "ns3/object.h"
#include "ns3/packet-filter.h"

namespace ns3 {

/**
 * \ingroup ipv4
 * \ingoup traffic-control
 *
 * Ipv4PacketFilter is the abstract base class for filters defined for IPv4 packets.
 */
class Ipv4PacketFilter: public PacketFilter {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  Ipv4PacketFilter ();
  virtual ~Ipv4PacketFilter ();

private:
  virtual bool CheckProtocol (Ptr<QueueDiscItem> item) const;
  virtual int32_t DoClassify (Ptr<QueueDiscItem> item) const = 0;
};


/**
 * \ingroup ipv4
 * \ingroup traffic-control
 *
 * PfifoFastIpv4PacketFilter is the filter to be added to the PfifoFast
 * queue disc to simulate the behavior of the pfifo_fast Linux queue disc.
 * 
 * Two modes of operation are provided. In PF_MODE_TOS mode, packets are
 * classified based on the TOS byte (originally defined by RFC 1349:
 * http://www.ietf.org/rfc/rfc1349.txt)
 *
 *               0     1     2     3     4     5     6     7
 *           +-----+-----+-----+-----+-----+-----+-----+-----+
 *           |   PRECEDENCE    |          TOS          | MBZ |
 *           +-----+-----+-----+-----+-----+-----+-----+-----+
 *
 * where MBZ stands for 'must be zero'.
 *
 * In the eight-bit legacy TOS byte, there were five lower bits for TOS
 * and three upper bits for Precedence.  Bit 7 was never used.  Bits 6-7
 * are now repurposed for ECN.  The below TOS values correspond to
 * bits 3-7 in the TOS byte (i.e. including MBZ), omitting the precedence
 * bits 0-2.
 *
 * TOS  | Bits | Means                   | Linux Priority | Band
 * -----|------|-------------------------|----------------|-----
 * 0x0  | 0    |  Normal Service         | 0 Best Effort  |  1
 * 0x2  | 1    |  Minimize Monetary Cost | 1 Filler       |  2
 * 0x4  | 2    |  Maximize Reliability   | 0 Best Effort  |  1
 * 0x6  | 3    |  mmc+mr                 | 0 Best Effort  |  1
 * 0x8  | 4    |  Maximize Throughput    | 2 Bulk         |  2
 * 0xa  | 5    |  mmc+mt                 | 2 Bulk         |  2
 * 0xc  | 6    |  mr+mt                  | 2 Bulk         |  2
 * 0xe  | 7    |  mmc+mr+mt              | 2 Bulk         |  2
 * 0x10 | 8    |  Minimize Delay         | 6 Interactive  |  0
 * 0x12 | 9    |  mmc+md                 | 6 Interactive  |  0
 * 0x14 | 10   |  mr+md                  | 6 Interactive  |  0
 * 0x16 | 11   |  mmc+mr+md              | 6 Interactive  |  0
 * 0x18 | 12   |  mt+md                  | 4 Int. Bulk    |  1
 * 0x1a | 13   |  mmc+mt+md              | 4 Int. Bulk    |  1
 * 0x1c | 14   |  mr+mt+md               | 4 Int. Bulk    |  1
 * 0x1e | 15   |  mmc+mr+mt+md           | 4 Int. Bulk    |  1
 *
 * In PF_MODE_TOS, the above values are used to map packets into bands, and
 * IP precedence bits are disregarded.
 *
 * In PF_MODE_DSCP (the default), the following mappings are used.
 *
 * For DSCP, the following values are recommended for Linux in a patch
 * to the netdev mailing list from Jesper Dangaard Brouer <brouer@redhat.com>
 * on 15 Sept 2014.  CS* values I made up myself.
 *
 * DSCP | Hex  | Means                      | Linux Priority | Band
 * -----|------|----------------------------|----------------|-----
 * EF   | 0x2E | TC_PRIO_INTERACTIVE_BULK=4 | 4 Int. Bulk    |  1
 * AF11 | 0x0A | TC_PRIO_BULK=2             | 2 Bulk         |  2
 * AF21 | 0x12 | TC_PRIO_BULK=2             | 2 Bulk         |  2
 * AF31 | 0x1A | TC_PRIO_BULK=2             | 2 Bulk         |  2
 * AF41 | 0x22 | TC_PRIO_BULK=2             | 2 Bulk         |  2
 * AF12 | 0x0C | TC_PRIO_INTERACTIVE=6      | 6 Interactive  |  0
 * AF22 | 0x14 | TC_PRIO_INTERACTIVE=6      | 6 Interactive  |  0
 * AF32 | 0x1C | TC_PRIO_INTERACTIVE=6      | 6 Interactive  |  0
 * AF42 | 0x34 | TC_PRIO_INTERACTIVE=6      | 6 Interactive  |  0
 * AF13 | 0x0E | TC_PRIO_INTERACTIVE_BULK=4 | 4 Int. Bulk    |  1
 * AF23 | 0x16 | TC_PRIO_INTERACTIVE_BULK=4 | 4 Int. Bulk    |  1
 * AF33 | 0x1E | TC_PRIO_INTERACTIVE_BULK=4 | 4 Int. Bulk    |  1
 * AF43 | 0x26 | TC_PRIO_INTERACTIVE_BULK=4 | 4 Int. Bulk    |  1
 * CS0  | 0x00 | TC_PRIO_BESTEFFORT         | 0 Best Effort  |  1
 * CS1  | 0x20 | TC_PRIO_FILLER             | 1 Filler       |  2
 * CS2  | 0x40 | TC_PRIO_BULK               | 2 Bulk         |  1
 * CS3  | 0x60 | TC_PRIO_INTERACTIVE_BULK   | 4 Int. Bulk    |  1
 * CS4  | 0x80 | TC_PRIO_INTERACTIVE        | 6 Interactive  |  0
 * CS5  | 0xA0 | TC_PRIO_INTERACTIVE        | 6 Interactive  |  0
 * CS6  | 0xC0 | TC_PRIO_INTERACTIVE        | 6 Interactive  |  0
 * CS7  | 0xE0 | TC_PRIO_CONTROL            | 8 Control      |  0
 *
 */
class PfifoFastIpv4PacketFilter: public Ipv4PacketFilter {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  PfifoFastIpv4PacketFilter ();
  virtual ~PfifoFastIpv4PacketFilter ();

  /**
   * \brief Enumeration of modes of Ipv4 header traffic class semantics
   */
  enum Ipv4TrafficClassMode
  {
    PF_MODE_TOS,       //!< use legacy TOS semantics to interpret TOS byte
    PF_MODE_DSCP,      //!< use DSCP semantics to interpret TOS byte
  };

private:
  virtual int32_t DoClassify (Ptr<QueueDiscItem> item) const;

  /**
   * \brief Converts a TOS field value into a priority band.
   * \param tos The tos to convert.
   * \returns The converted band value.
   */
  uint32_t TosToBand (uint8_t tos) const;

  /**
   * \brief Converts a DSCP field value into a priority band.
   * \param dscpType The DSCP to convert.
   * \returns The converted band value.
   */
  uint32_t DscpToBand (Ipv4Header::DscpType dscpType) const;

  Ipv4TrafficClassMode m_trafficClassMode; //!< traffic class mode
};

} // namespace ns3

#endif /* IPV4_PACKET_FILTER */
