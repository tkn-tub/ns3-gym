/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Based on 
 *      NS-2 AODV model developed by the CMU/MONARCH group and optimized and
 *      tuned by Samir Das and Mahesh Marina, University of Cincinnati;
 * 
 *      AODV-UU implementation by Erik Nordström of Uppsala University
 *      http://core.it.uu.se/core/index.php/AODV-UU
 *
 * Authors: Elena Borovkova <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */
#ifndef __aodv_rqueue_h__
#define __aodv_rqueue_h__

#include "ns3/ipv4-header.h"
#include "ns3/nstime.h"
#include "ns3/packet.h"
#include <vector>

namespace ns3 {
namespace aodv {

/// The maximum number of packets that we allow a routing protocol to buffer.
#define AODV_RTQ_MAX_LEN 64
/// The maximum period of time that a routing protocol is allowed to buffer a packet for, seconds.
#define AODV_RTQ_TIMEOUT 30
/**
 * \ingroup aodv
 * \brief AODV Queue Entry
 */
struct QueueEntry
{
  Ptr<Packet> m_packet;
  Ipv4Header m_header;
  /// Expire time for queue entry
  Time m_expire;
  /// c-tor
  QueueEntry(Ptr<Packet> pa, Ipv4Header const & h, Time exp = Seconds(0)) : m_packet(pa), m_header(h), m_expire(exp) {}
  /**
   * Compare queue entries
   * \return true if equal
   */
  bool operator==(QueueEntry const & o) const
  {
    return ((m_packet == o.m_packet)/*&& header == o.header*/ && (m_expire == o.m_expire));
  }
};
/**
 * \ingroup aodv
 * \brief AODV Queue
 */
class AodvQueue
{
public:
  /// Default c-tor
  AodvQueue ();
  /// Push entry in queue.
  void Enqueue (QueueEntry & entry);
  /// Returns a entry from the head of the queue.
  QueueEntry Dequeue ();
  /// Return first found (the earliest) entry for given destination
  bool Dequeue (Ipv4Address dst, QueueEntry & entry);
  /// Finds whether a packet with destination dst exists in the queue
  bool Find (Ipv4Address dst);
  /// Number of entries
  uint32_t GetSize ();

private:
  std::vector<QueueEntry> m_queue;
  /// Remove and return first entry from queue
  QueueEntry RemoveHead();
  /// Remove all expired entries
  void Purge();
  /// Notify that packet is dropped from queue by timeout
  void Drop (QueueEntry e);
  /// Maximum number of entries in queue
  uint32_t m_maxSize;
  /// Life time of queue entry in queue
  Time m_timeout;
};
}}

#endif /* __aodv_rqueue_h__ */
