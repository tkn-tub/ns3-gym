/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Universita' degli Studi di Napoli Federico II
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
 */

#ifndef IPV4_QUEUE_DISC_ITEM_H
#define IPV4_QUEUE_DISC_ITEM_H

#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/net-device.h"
#include "ns3/traced-value.h"
#include "ns3/queue-disc.h"
#include "ipv4-header.h"

namespace ns3 {

/**
 * \ingroup ipv4
 * \ingroup traffic-control
 *
 * Ipv4QueueDiscItem is a subclass of QueueDiscItem which stores IPv4 packets.
 * Header and payload are kept separate to allow the queue disc to manipulate
 * the header, which is added to the packet when the packet is dequeued.
 */
class Ipv4QueueDiscItem : public QueueDiscItem {
public:
  /**
   * \brief Create an IPv4 queue disc item containing an IPv4 packet.
   * \param p the packet included in the created item.
   * \param addr the destination MAC address
   * \param protocol the protocol number
   * \param header the IPv4 header
   */
  Ipv4QueueDiscItem (Ptr<Packet> p, const Address & addr, uint16_t protocol, const Ipv4Header & header);

  virtual ~Ipv4QueueDiscItem ();

  /**
   * \return the correct packet size (header plus payload).
   */
  virtual uint32_t GetPacketSize (void) const;

  /**
   * \return the header stored in this item..
   */
  const Ipv4Header & GetHeader (void) const;

  /**
   * \brief Add the header to the packet
   */
  virtual void AddHeader (void);

  /**
   * \brief Print the item contents.
   * \param os output stream in which the data should be printed.
   */
  virtual void Print (std::ostream &os) const;

private:
  /**
   * \brief Default constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  Ipv4QueueDiscItem ();
  /**
   * \brief Copy constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  Ipv4QueueDiscItem (const Ipv4QueueDiscItem &);
  /**
   * \brief Assignment operator
   *
   * Defined and unimplemented to avoid misuse
   * \returns
   */
  Ipv4QueueDiscItem &operator = (const Ipv4QueueDiscItem &);

  Ipv4Header m_header;  //!< The IPv4 header.
  bool m_headerAdded;   //!< True if the header has already been added to the packet.
};

} // namespace ns3

#endif /* IPV4_QUEUE_DISC_ITEM_H */
