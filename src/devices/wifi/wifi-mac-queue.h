/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
#ifndef WIFI_MAC_QUEUE_H
#define WIFI_MAC_QUEUE_H

/* This queue implements what is needed for the 802.11e standard
 * Specifically, it refers to 802.11e/D9, section 9.9.1.6, paragraph 6.
 *
 * When a packet is received by the MAC, to be sent to the PHY, 
 * it is queued in the internal queue after being tagged by the 
 * current time. If the queue is non-Empty (quite likely), we
 * notify m_packetNotify. This information is forwarded to 
 * The associated MacLow80211 which might try to dequeue packets
 * from this queue if it is not doing anything else more important.
 *
 * If it is doing something too important to handle new packets,
 * the MacLow80211 is supposed to try to dequeue packets the next 
 * time it can.
 *
 * When a packet is dequeued, the queue checks its timestamp 
 * to verify whether or not it should be dropped. If 
 * dot11EDCATableMSDULifetime has elapsed, it is dropped.
 * Otherwise, it is returned to the caller.
 */

#include <deque>
#include <utility>
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "wifi-mac-header.h"

namespace ns3 {

class MacParameters;

class WifiMacQueue {
public:
  WifiMacQueue ();
  ~WifiMacQueue ();

  void SetMaxSize (uint32_t maxSize);
  void SetMaxDelay (Time delay);

  void Enqueue (Packet packet, WifiMacHeader const &hdr);
  Packet Dequeue (WifiMacHeader *hdr, bool *found);

  void Flush (void);

  bool IsEmpty (void);
  uint32_t GetSize (void);

private:
  void Cleanup (void);
  struct Item {
    Item (Packet packet, 
          WifiMacHeader const&hdr, 
          Time tstamp);
    Packet packet;
    WifiMacHeader hdr;
    Time tstamp;
  };
  typedef std::deque<struct Item> PacketQueue;
  typedef std::deque<struct Item>::reverse_iterator PacketQueueRI;
  typedef std::deque<struct Item>::iterator PacketQueueI;
  PacketQueue m_queue;
  MacParameters *m_parameters;
  uint32_t m_size;
  uint32_t m_maxSize;
  Time m_maxDelay;
};

} // namespace ns3


#endif /* WIFI_MAC_QUEUE_H */
