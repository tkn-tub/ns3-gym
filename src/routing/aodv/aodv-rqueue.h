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
#include "ns3/ipv4-routing-protocol.h"


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
  typedef Callback<void, Ptr<Ipv4Route>, Ptr<const Packet>, const Ipv4Header &> UnicastForwardCallback;
  typedef Callback<void, Ptr<const Packet>, const Ipv4Header &, Socket::SocketErrno > ErrorCallback;

  Ptr<const Packet> m_packet;
  Ipv4Header m_header;
  UnicastForwardCallback m_ucb;
  ErrorCallback m_ecb;
  /// Expire time for queue entry
  Time m_expire;
  /// c-tor
  QueueEntry(Ptr<const Packet> pa = 0, Ipv4Header const & h = Ipv4Header(), UnicastForwardCallback ucb = 0, ErrorCallback ecb = 0, Time exp = Seconds(0)) : m_packet(pa),
                        m_header(h), m_ucb(ucb), m_ecb(ecb), m_expire(exp) {}
  /**
   * Compare queue entries
   * \return true if equal
   */
  bool operator==(QueueEntry const & o) const
  {
    return ((m_packet == o.m_packet)/*&& header == o.header*/ && (m_expire == o.m_expire));
  }

  UnicastForwardCallback GetUnicastForwardCallback() const { return m_ucb; }
  void SetUnicastForwardCallback(UnicastForwardCallback ucb) { m_ucb = ucb; }
  Ptr<const Packet> GetPacket() const { return m_packet; }
  void SetPacket(Ptr<const Packet> p) { m_packet = p; }
  Ipv4Header GetIpv4Header() const { return m_header; }
  void SetIpv4Header(Ipv4Header h) { m_header = h; }
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
  /// Remove all packets with destination IP address dst
  void DropPacketWithDst (Ipv4Address dst);
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
  static bool IsEqual(QueueEntry  en, const Ipv4Address dst) { return (en.m_header.GetDestination() == dst);}
};


}}

#endif /* __aodv_rqueue_h__ */
