/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005, 2009 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Mirko Banchi <mk.banchi@gmail.com>
 *          Stefano Avallone <stavallo@unina.it>
 */

#ifndef WIFI_MAC_QUEUE_ITEM_H
#define WIFI_MAC_QUEUE_ITEM_H

#include "ns3/nstime.h"
#include "ns3/queue.h"
#include "wifi-mac-header.h"

namespace ns3 {

class QosBlockedDestinations;

/**
 * \ingroup wifi
 *
 * WifiMacQueueItem stores (const) packets along with their Wifi MAC headers
 * and the time when they were enqueued.
 */
class WifiMacQueueItem : public SimpleRefCount<WifiMacQueueItem>
{
public:
  /**
   * \brief Create a Wifi MAC queue item containing a packet and a Wifi MAC header.
   * \param p the const packet included in the created item.
   * \param header the Wifi Mac header included in the created item.
   */
  WifiMacQueueItem (Ptr<const Packet> p, const WifiMacHeader & header);

  virtual ~WifiMacQueueItem ();

  /**
   * \brief Get the packet stored in this item
   * \return the packet stored in this item.
   */
  Ptr<const Packet> GetPacket (void) const;

  /**
   * \brief Get the header stored in this item
   * \return the header stored in this item.
   */
  const WifiMacHeader & GetHeader (void) const;

  /**
   * \brief Return the destination address present in the header
   * \return the destination address
   */
  Mac48Address GetDestinationAddress (void) const;

  /**
   * \brief Get the timestamp included in this item
   * \return the timestamp included in this item.
   */
  Time GetTimeStamp (void) const;

  /**
   * \brief Return the size of the packet included in this item
   *
   * \return the size of the packet included in this item.
   */
  uint32_t GetSize (void) const;

private:
  /**
   * \brief Default constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  WifiMacQueueItem ();
  /**
   * \brief Copy constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  WifiMacQueueItem (const WifiMacQueueItem &);
  /**
   * \brief Assignment operator
   *
   * Defined and unimplemented to avoid misuse
   * \returns
   */
  WifiMacQueueItem &operator = (const WifiMacQueueItem &);

  Ptr<const Packet> m_packet;  //!< The packet contained in this queue item
  WifiMacHeader m_header;      //!< Wifi MAC header associated with the packet
  Time m_tstamp;               //!< timestamp when the packet arrived at the queue
};


// The following explicit template instantiation declaration prevents modules
// including this header file from implicitly instantiating Queue<WifiMacQueueItem>.
// This would cause python examples using wifi to crash at runtime with the
// following error message: "Trying to allocate twice the same uid:
// ns3::Queue<WifiMacQueueItem>"
extern template class Queue<WifiMacQueueItem>;


} //namespace ns3

#endif /* WIFI_MAC_QUEUE_ITEM_H */
