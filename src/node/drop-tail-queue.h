/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
 * All rights reserved.
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
 */

#ifndef DROPTAIL_H
#define DROPTAIL_H

#include <queue>
#include "ns3/packet.h"
#include "ns3/queue.h"

namespace ns3 {

class TraceContainer;

const int DTQ_NPACKETS_MAX_DEFAULT = 100;

/**
 * \brief A FIFO packet queue that drops tail-end packets on overflow
 */
class DropTailQueue : public Queue {
public:
  static TypeId GetTypeId (void);
  /**
   * \brief DropTailQueue Constructor
   *
   * Creates a droptail queue with a maximum size of 100 packets by default
   */
  DropTailQueue ();

  virtual ~DropTailQueue();
  /**
   * \param npackets The maximum number of packets this queue will hold before
   * dropping packets.
   */
  void SetMaxPackets (uint32_t npackets);
  /**
   * \return The maximum number of packets this queue will hold before dropping
   * packets.
   */
  uint32_t GetMaxPackets (void);

private:
  virtual bool DoEnqueue (Ptr<Packet> p);
  virtual Ptr<Packet> DoDequeue (void);
  virtual Ptr<Packet> DoPeek (void) const;

private:
  std::queue<Ptr<Packet> > m_packets;
  uint32_t m_maxPackets;
};

}; // namespace ns3

#endif /* DROPTAIL_H */
