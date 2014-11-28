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
 * Authors: Elena Buchatskaia <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */
#ifndef AODV_RQUEUE_H
#define AODV_RQUEUE_H

#include <vector>
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/simulator.h"


namespace ns3 {
namespace aodv {

/**
 * \ingroup aodv
 * \brief AODV Queue Entry
 */
class QueueEntry
{
public:
  typedef Ipv4RoutingProtocol::UnicastForwardCallback UnicastForwardCallback;
  typedef Ipv4RoutingProtocol::ErrorCallback ErrorCallback;
  /// c-tor
  QueueEntry (Ptr<const Packet> pa = 0, Ipv4Header const & h = Ipv4Header (),
              UnicastForwardCallback ucb = UnicastForwardCallback (),
              ErrorCallback ecb = ErrorCallback (), Time exp = Simulator::Now ()) :
    m_packet (pa), m_header (h), m_ucb (ucb), m_ecb (ecb),
    m_expire (exp + Simulator::Now ())
  {}

  /**
   * Compare queue entries
   * \return true if equal
   */
  bool operator== (QueueEntry const & o) const
  {
    return ((m_packet == o.m_packet) && (m_header.GetDestination () == o.m_header.GetDestination ()) && (m_expire == o.m_expire));
  }

  // Fields
  UnicastForwardCallback GetUnicastForwardCallback () const { return m_ucb; }
  void SetUnicastForwardCallback (UnicastForwardCallback ucb) { m_ucb = ucb; }
  ErrorCallback GetErrorCallback () const { return m_ecb; }
  void SetErrorCallback (ErrorCallback ecb) { m_ecb = ecb; }
  Ptr<const Packet> GetPacket () const { return m_packet; }
  void SetPacket (Ptr<const Packet> p) { m_packet = p; }
  Ipv4Header GetIpv4Header () const { return m_header; }
  void SetIpv4Header (Ipv4Header h) { m_header = h; }
  void SetExpireTime (Time exp) { m_expire = exp + Simulator::Now (); }
  Time GetExpireTime () const { return m_expire - Simulator::Now (); }

private:
  
  /// Data packet
  Ptr<const Packet> m_packet;
  /// IP header
  Ipv4Header m_header;
  /// Unicast forward callback
  UnicastForwardCallback m_ucb;
  /// Error callback
  ErrorCallback m_ecb;
  /// Expire time for queue entry
  Time m_expire;
};
/**
 * \ingroup aodv
 * \brief AODV route request queue
 * 
 * Since AODV is an on demand routing we queue requests while looking for route.
 */
class RequestQueue
{
public:
  /// Default c-tor
  RequestQueue (uint32_t maxLen, Time routeToQueueTimeout) :
    m_maxLen (maxLen), m_queueTimeout (routeToQueueTimeout)
  {
  }
  /// Push entry in queue, if there is no entry with the same packet and destination address in queue.
  bool Enqueue (QueueEntry & entry);
  /// Return first found (the earliest) entry for given destination
  bool Dequeue (Ipv4Address dst, QueueEntry & entry);
  /// Remove all packets with destination IP address dst
  void DropPacketWithDst (Ipv4Address dst);
  /// Finds whether a packet with destination dst exists in the queue
  bool Find (Ipv4Address dst);
  /// Number of entries
  uint32_t GetSize ();
  
  // Fields
  uint32_t GetMaxQueueLen () const { return m_maxLen; }
  void SetMaxQueueLen (uint32_t len) { m_maxLen = len; }
  Time GetQueueTimeout () const { return m_queueTimeout; }
  void SetQueueTimeout (Time t) { m_queueTimeout = t; }

private:
  
  std::vector<QueueEntry> m_queue;
  /// Remove all expired entries
  void Purge ();
  /// Notify that packet is dropped from queue by timeout
  void Drop (QueueEntry en, std::string reason);
  /// The maximum number of packets that we allow a routing protocol to buffer.
  uint32_t m_maxLen;
  /// The maximum period of time that a routing protocol is allowed to buffer a packet for, seconds.
  Time m_queueTimeout;
  static bool IsEqual (QueueEntry en, const Ipv4Address dst) { return (en.GetIpv4Header ().GetDestination () == dst); }
};


}
}

#endif /* AODV_RQUEUE_H */
