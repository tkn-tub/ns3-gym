/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 1997, 1998 Carnegie Mellon University.
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
 * Authors: The AODV code developed by the CMU/MONARCH group was optimized and
 * tuned by Samir Das and Mahesh Marina, University of Cincinnati.
 * The work was partially done in Sun Microsystems.
 *
 * Ported to ns-3 by Elena Borovkova <borovkovaes@iitp.ru>
 */
#ifndef __aodv_rqueue_h__
#define __aodv_rqueue_h_

#include "ns3/ipv4-header.h"
#include "ns3/nstime.h"
#include "ns3/packet.h"
#include <vector>

namespace ns3 {
namespace aodv {

/// The maximum number of packets that we allow a routing protocol to buffer.
#define AODV_RTQ_MAX_LEN   64
/// The maximum period of time that a routing protocol is allowed to buffer a packet for, seconds.
#define AODV_RTQ_TIMEOUT   30

struct QueueEntry
{
  Ptr<Packet> p;
  Ipv4Header header;
  Time enExpire;
};

class aodv_rqueue 
{
public:
  /// default c-tor
  aodv_rqueue();
  /// Push element in queue.
  void enque(QueueEntry entry);
  /// Returns a element from the head of the queue.
  QueueEntry deque();
  /// Return first found (the earliest) entry for given destination 
  bool deque(Ipv4Address dst, QueueEntry & entry);
  /// Finds whether a packet with destination dst exists in the queue
  bool find(Ipv4Address dst);

private:
  std::vector<QueueEntry> queue;
  /// Remove and return first entry from queue
  QueueEntry remove_head();
  void purge();
  /// Find packet with destination address dst, return true on success
  bool findPacketWithDst(Ipv4Address dst, QueueEntry & entry);
  /// Notify that packet is dropped from queue by timeout
  void drop (QueueEntry e);
  
  uint32_t limit_;
  Time timeout_;
};
}}

#endif /* __aodv_rqueue_h__ */
