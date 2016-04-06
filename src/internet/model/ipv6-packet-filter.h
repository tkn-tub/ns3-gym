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

#ifndef IPV6_PACKET_FILTER_H
#define IPV6_PACKET_FILTER_H

#include "ns3/object.h"
#include "ns3/packet-filter.h"

namespace ns3 {

/**
 * \ingroup internet
 *
 * Ipv6PacketFilter is the abstract base class for filters defined for IPv6 packets.
 */
class Ipv6PacketFilter: public PacketFilter {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  Ipv6PacketFilter ();
  virtual ~Ipv6PacketFilter ();

private:
  virtual bool CheckProtocol (Ptr<QueueDiscItem> item) const;
  virtual int32_t DoClassify (Ptr<QueueDiscItem> item) const = 0;
};


/**
 * \ingroup internet
 *
 * PfifoFastIpv6PacketFilter is the filter to be added to the PfifoFast
 * queue disc to simulate the behavior of the pfifo_fast Linux queue disc.
 * 
 * Packets are classified based on the DSCP. The following values are
 * recommended for Linux in a patch to the netdev mailing list from
 * Jesper Dangaard Brouer <brouer@redhat.com> on 15 Sept 2014.
 * CS* values I made up myself.
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
class PfifoFastIpv6PacketFilter: public Ipv6PacketFilter {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  PfifoFastIpv6PacketFilter ();
  virtual ~PfifoFastIpv6PacketFilter ();

private:
  virtual int32_t DoClassify (Ptr<QueueDiscItem> item) const;

  /**
   * \brief Converts a DSCP field value into a priority band.
   * \param dscpType The DSCP to convert.
   * \returns The converted band value.
   */
  uint32_t DscpToBand (Ipv6Header::DscpType dscpType) const;
};

} // namespace ns3

#endif /* IPV6_PACKET_FILTER */
