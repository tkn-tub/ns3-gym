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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#ifndef WIFI_MAC_QUEUE_H
#define WIFI_MAC_QUEUE_H

#include <list>
#include <utility>
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "wifi-mac-header.h"

namespace ns3 {
class QosBlockedDestinations;

/**
 * \ingroup wifi
 *
 * This queue implements the timeout procedure described in
 * (Section 9.19.2.6 "Retransmit procedures" paragraph 6; IEEE 802.11-2012).
 *
 * When a packet is received by the MAC, to be sent to the PHY,
 * it is queued in the internal queue after being tagged by the
 * current time.
 *
 * When a packet is dequeued, the queue checks its timestamp
 * to verify whether or not it should be dropped. If
 * dot11EDCATableMSDULifetime has elapsed, it is dropped.
 * Otherwise, it is returned to the caller.
 */
class WifiMacQueue : public Object
{
public:
  static TypeId GetTypeId (void);
  WifiMacQueue ();
  ~WifiMacQueue ();

  /**
   * Set the maximum queue size.
   *
   * \param maxSize the maximum queue size
   */
  void SetMaxSize (uint32_t maxSize);
  /**
   * Set the maximum delay before the packet is discarded.
   *
   * \param delay the maximum delay
   */
  void SetMaxDelay (Time delay);
  /**
   * Return the maximum queue size.
   *
   * \return the maximum queue size
   */
  uint32_t GetMaxSize (void) const;
  /**
   * Return the maximum delay before the packet is discarded.
   *
   * \return the maximum delay
   */
  Time GetMaxDelay (void) const;

  /**
   * Enqueue the given packet and its corresponding WifiMacHeader at the <i>end</i> of the queue.
   *
   * \param packet the packet to be euqueued at the end
   * \param hdr the header of the given packet
   */
  void Enqueue (Ptr<const Packet> packet, const WifiMacHeader &hdr);
  /**
   * Enqueue the given packet and its corresponding WifiMacHeader at the <i>front</i> of the queue.
   *
   * \param packet the packet to be euqueued at the end
   * \param hdr the header of the given packet
   */
  void PushFront (Ptr<const Packet> packet, const WifiMacHeader &hdr);
  /**
   * Dequeue the packet in the front of the queue.
   *
   * \param hdr the WifiMacHeader of the packet
   * \return the packet
   */
  Ptr<const Packet> Dequeue (WifiMacHeader *hdr);
  /**
   * Peek the packet in the front of the queue. The packet is not removed.
   *
   * \param hdr the WifiMacHeader of the packet
   * \return the packet
   */
  Ptr<const Packet> Peek (WifiMacHeader *hdr);
  /**
   * Searchs and returns, if is present in this queue, first packet having
   * address indicated by <i>type</i> equals to <i>addr</i>, and tid
   * equals to <i>tid</i>. This method removes the packet from this queue.
   * Is typically used by ns3::EdcaTxopN in order to perform correct MSDU
   * aggregation (A-MSDU).
   *
   * \param hdr the header of the dequeued packet
   * \param tid the given TID
   * \param type the given address type
   * \param addr the given destination
   * \return packet
   */
  Ptr<const Packet> DequeueByTidAndAddress (WifiMacHeader *hdr,
                                            uint8_t tid,
                                            WifiMacHeader::AddressType type,
                                            Mac48Address addr);
  /**
   * Searchs and returns, if is present in this queue, first packet having
   * address indicated by <i>type</i> equals to <i>addr</i>, and tid
   * equals to <i>tid</i>. This method doesn't remove the packet from this queue.
   * Is typically used by ns3::EdcaTxopN in order to perform correct MSDU
   * aggregation (A-MSDU).
   *
   * \param hdr the header of the dequeued packet
   * \param tid the given TID
   * \param type the given address type
   * \param addr the given destination
   * \param timestamp
   * \return packet
   */
  Ptr<const Packet> PeekByTidAndAddress (WifiMacHeader *hdr,
                                         uint8_t tid,
                                         WifiMacHeader::AddressType type,
                                         Mac48Address addr,
                                         Time *timestamp);
  /**
   * If exists, removes <i>packet</i> from queue and returns true. Otherwise it
   * takes no effects and return false. Deletion of the packet is
   * performed in linear time (O(n)).
   *
   * \param packet the packet to be removed
   * \return true if the packet was removed, false otherwise
   */
  bool Remove (Ptr<const Packet> packet);
  /**
   * Returns number of QoS packets having tid equals to <i>tid</i> and address
   * specified by <i>type</i> equals to <i>addr</i>.
   *
   * \param tid the given TID
   * \param type the given address type
   * \param addr the given destination
   * \return the number of QoS packets
   */
  uint32_t GetNPacketsByTidAndAddress (uint8_t tid,
                                       WifiMacHeader::AddressType type,
                                       Mac48Address addr);
  /**
   * Returns first available packet for transmission. A packet could be no available
   * if it's a QoS packet with a tid and an address1 fields equal to <i>tid</i> and <i>addr</i>
   * respectively that index a pending agreement in the BlockAckManager object.
   * So that packet must not be transmitted until reception of an ADDBA response frame from station
   * addressed by <i>addr</i>. This method removes the packet from queue.
   *
   * \param hdr the header of the dequeued packet
   * \param tStamp
   * \param blockedPackets
   * \return packet
   */
  Ptr<const Packet> DequeueFirstAvailable (WifiMacHeader *hdr,
                                           Time &tStamp,
                                           const QosBlockedDestinations *blockedPackets);
  /**
   * Returns first available packet for transmission. The packet isn't removed from queue.  
   *
   * \param hdr the header of the dequeued packet
   * \param tStamp
   * \param blockedPackets
   * \return packet
   */
  Ptr<const Packet> PeekFirstAvailable (WifiMacHeader *hdr,
                                        Time &tStamp,
                                        const QosBlockedDestinations *blockedPackets);
  /**
   * Flush the queue.
   */
  void Flush (void);

  /**
   * Return if the queue is empty.
   *
   * \return true if the queue is empty, false otherwise
   */
  bool IsEmpty (void);
  /**
   * Return the current queue size.
   *
   * \return the current queue size
   */
  uint32_t GetSize (void);

protected:
  /**
   * Clean up the queue by removing packets that exceeded the maximum delay.
   */
  virtual void Cleanup (void);

  /**
   * A struct that holds information about a packet for putting
   * in a packet queue.
   */
  struct Item
  {
    /**
     * Create a struct with the given parameters.
     *
     * \param packet
     * \param hdr
     * \param tstamp
     */
    Item (Ptr<const Packet> packet,
          const WifiMacHeader &hdr,
          Time tstamp);
    Ptr<const Packet> packet; //!< Actual packet
    WifiMacHeader hdr; //!< Wifi MAC header associated with the packet
    Time tstamp; //!< timestamp when the packet arrived at the queue
  };

  /**
   * typedef for packet (struct Item) queue.
   */
  typedef std::list<struct Item> PacketQueue;
  /**
   * typedef for packet (struct Item) queue reverse iterator.
   */
  typedef std::list<struct Item>::reverse_iterator PacketQueueRI;
  /**
   * typedef for packet (struct Item) queue iterator.
   */
  typedef std::list<struct Item>::iterator PacketQueueI;
  /**
   * Return the appropriate address for the given packet (given by PacketQueue iterator).
   *
   * \param type
   * \param it
   * \return the address
   */
  Mac48Address GetAddressForPacket (enum WifiMacHeader::AddressType type, PacketQueueI it);

  PacketQueue m_queue; //!< Packet (struct Item) queue
  uint32_t m_size; //!< Current queue size
  uint32_t m_maxSize; //!< Queue capacity
  Time m_maxDelay; //!< Time to live for packets in the queue
};

} // namespace ns3

#endif /* WIFI_MAC_QUEUE_H */
