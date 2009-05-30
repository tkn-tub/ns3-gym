/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

class WifiMacParameters;

/**
 * \brief a 802.11e-specific queue.
 *
 * This queue implements what is needed for the 802.11e standard
 * Specifically, it refers to 802.11e/D9, section 9.9.1.6, paragraph 6.
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

  void SetMaxSize (uint32_t maxSize);
  void SetMaxDelay (Time delay);
  uint32_t GetMaxSize (void) const;
  Time GetMaxDelay (void) const;

  void Enqueue (Ptr<const Packet> packet, const WifiMacHeader &hdr);
  Ptr<const Packet> Dequeue (WifiMacHeader *hdr);
  Ptr<const Packet> Peek (WifiMacHeader *hdr);
  /**
   * Searchs and returns, if is present in this queue, first packet having 
   * address indicated by <i>type</i> equals to <i>addr</i>, and tid 
   * equals to <i>tid</i>. This method removes the packet from this queue. 
   * Is typically used by ns3::EdcaTxopN in order to perform correct MSDU 
   * aggregation (A-MSDU).
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
   */
  Ptr<const Packet> PeekByTidAndAddress (WifiMacHeader *hdr,
                                         uint8_t tid,
                                         WifiMacHeader::AddressType type,
                                         Mac48Address addr);
  /**
   * If exists, removes <i>packet</i> from queue and returns true. Otherwise it
   * takes no effects and return false. Deletion of the packet is
   * performed in linear time (O(n)).  
   */
  bool Remove (Ptr<const Packet> packet);
  
  void Flush (void);

  bool IsEmpty (void);
  uint32_t GetSize (void);

private:
  struct Item;
  
  typedef std::list<struct Item> PacketQueue;
  typedef std::list<struct Item>::reverse_iterator PacketQueueRI;
  typedef std::list<struct Item>::iterator PacketQueueI;
  
  void Cleanup (void);
  Mac48Address GetAddressForPacket (enum WifiMacHeader::AddressType type, PacketQueueI);
  
  struct Item {
    Item (Ptr<const Packet> packet, 
          const WifiMacHeader &hdr, 
          Time tstamp);
    Ptr<const Packet> packet;
    WifiMacHeader hdr;
    Time tstamp;
  };

  PacketQueue m_queue;
  WifiMacParameters *m_parameters;
  uint32_t m_size;
  uint32_t m_maxSize;
  Time m_maxDelay;
};

} // namespace ns3

#endif /* WIFI_MAC_QUEUE_H */
