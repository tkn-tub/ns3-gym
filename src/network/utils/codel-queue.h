/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Andrew McGregor
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
 * Codel, the COntrolled DELay Queueing discipline
 * Based on ns2 simulation code presented by Kathie Nichols
 *
 * This port based on linux kernel code by
 * Authors:	Dave Täht <d@taht.net>
 *		Eric Dumazet <edumazet@google.com>
 *
 * Ported to ns-3 by: Andrew McGregor <andrewmcgr@gmail.com>
 */

#ifndef CODEL_H
#define CODEL_H

#include <queue>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"

namespace ns3 {

typedef uint32_t codel_time_t;
typedef uint16_t rec_inv_sqrt_t;

#define CODEL_SHIFT 10
#define REC_INV_SQRT_BITS (8*sizeof(rec_inv_sqrt_t))
#define REC_INV_SQRT_SHIFT (32 - REC_INV_SQRT_BITS)

class TraceContainer;

/**
 * \ingroup queue
 *
 * \brief A FIFO packet queue that drops tail-end packets on overflow
 */
class CoDelQueue : public Queue {
public:
  friend class Fq_CoDelQueue;

  static TypeId GetTypeId (void);
  /**
   * \brief CoDelQueue Constructor
   *
   * Creates a codel queue with a maximum size of 100 packets by default
   */
  CoDelQueue ();

  virtual ~CoDelQueue();

  /**
   * Enumeration of the modes supported in the class.
   *
   */
  enum Mode {
    ILLEGAL,     /**< Mode not set */
    PACKETS,     /**< Use number of packets for maximum queue size */
    BYTES,       /**< Use number of bytes for maximum queue size */
  };

  /**
   * Set the operating mode of this device.
   *
   * \param mode The operating mode of this device.
   *
   */
  void SetMode (CoDelQueue::Mode mode);

  /**
   * Get the encapsulation mode of this device.
   *
   * \returns The encapsulation mode of this device.
   */
  CoDelQueue::Mode  GetMode (void);

  uint32_t GetQueueSize (void);

private:
  virtual bool DoEnqueue (Ptr<Packet> p);
  virtual Ptr<Packet> DoDequeue (void);
  virtual Ptr<const Packet> DoPeek (void) const;
  void NewtonStep(void);
  codel_time_t ControlLaw(codel_time_t t);
  bool ShouldDrop(Ptr<Packet> p, codel_time_t now);

  std::queue<Ptr<Packet> > m_packets;
  uint32_t m_maxPackets;
  uint32_t m_maxBytes;
  uint32_t m_bytesInQueue;
  uint32_t *backlog;
  uint32_t m_minbytes;
  Time m_Interval;
  Time m_Target;
  TracedValue<uint32_t> m_count;
  TracedValue<uint32_t> m_drop_count;
  TracedValue<uint32_t> m_lastcount;
  bool m_dropping;
  uint16_t m_rec_inv_sqrt;
  codel_time_t m_first_above_time;
  codel_time_t m_drop_next;
  uint32_t m_state1;
  uint32_t m_state2;
  uint32_t m_state3;
  uint32_t m_states;
  uint32_t m_drop_overlimit;
  Mode     m_mode;
};

} // namespace ns3

#endif /* CODEL_H */
