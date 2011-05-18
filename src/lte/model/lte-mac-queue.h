/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#ifndef LTE_MAC_QUEUE_H
#define LTE_MAC_QUEUE_H

#include <queue>
#include <stdint.h>
#include "ns3/packet.h"
#include "ns3/object.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"

namespace ns3 {

/**
 * \ingroup lte
 *
 * The MAC queue for each bearer
 */
class LteMacQueue : public Object
{
public:
  static TypeId GetTypeId (void);
  LteMacQueue (void);

  /**
   * Create the Mac queue defining its maximum size
   * \param maxSize the maximum size of the queue
   */
  LteMacQueue (uint32_t maxSize);
  ~LteMacQueue (void);

  /**
   * \brief set the maximum queue size
   * \param maxSize the max queue size
   */
  void SetMaxSize (uint32_t maxSize);
  /**
   * \return the maximum queue size
   */
  uint32_t GetMaxSize (void) const;


  /**
   * \brief Enqueue a packet
   * \param packet the packet to enqueue
   */
  bool Enqueue (Ptr<Packet> packet);

  /**
   * \brief Dequeue  a packet from the queue
   * \return the first packet in the queue
   */
  Ptr<Packet> Dequeue (void);

  /**
   * \brief Dequeue a fragment of size availableByte from the queue
   * \param availableByte the size of the fragment
   * \return the first packet in the queue if its size is lower than availableByte, the first availableByte of the
   * first packet otherwise
   */
  Ptr<Packet> Dequeue (uint32_t availableByte);

  /**
   * \brief Same as Dequeue but does not pop from queue
   */
  Ptr<Packet> Peek (void) const;

  /**
   * \brief Check if there are packets into the queue
   */
  bool IsEmpty (void) const;


  /**
   * \brief Get the size of the queue
   * \return the size of the queue
   */
  uint32_t GetSize (void) const;


  /**
   * \brief Get the number of bytes into the queue
   * \return the number of bytes into the queue
   */
  uint32_t GetNBytes (void) const;
  /**
   * \brief Get the length of the queue with MAC + RLC + CRC overhead
   * \return the length of the queue with MAC + RLC + CRC overhead
   */
  uint32_t GetQueueLengthWithMACOverhead (void);




private:
  struct QueueElement
  {
    QueueElement (void);
    QueueElement (Ptr<Packet> packet,
                  Time timeStamp);
    uint32_t GetSize (void) const;
    Ptr<Packet> m_packet;
    Time m_timeStamp;
  };


  /**
   * \brief Get the first element og the queue
   * \return the first element og the queue
   */
  LteMacQueue::QueueElement Front () const;

  /**
   * \brief erase the first element og the queue
   */
  void Pop (void);

  typedef std::deque<QueueElement> PacketQueue;
  PacketQueue m_queue;

  uint32_t m_maxSize;
  uint32_t m_bytes;
  uint32_t m_nrDataPackets;

  TracedCallback<Ptr<const Packet> > m_traceEnqueue;
  TracedCallback<Ptr<const Packet> > m_traceDequeue;
  TracedCallback<Ptr<const Packet> > m_traceDrop;

public:
  /**
   * \brief Get the packet queue
   * \return the apcket queue
   */
  const LteMacQueue::PacketQueue & GetPacketQueue (void) const;
};

} // namespace ns3

#endif /* LTE_MAC_QUEUE_H */
