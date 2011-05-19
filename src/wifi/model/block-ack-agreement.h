/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#ifndef BLOCK_ACK_AGREEMENT_H
#define BLOCK_ACK_AGREEMENT_H

#include "ns3/mac48-address.h"
#include "ns3/event-id.h"

namespace ns3 {
/**
 * \brief Maintains information for a block ack agreement.
 * \ingroup wifi
 */
class BlockAckAgreement
{
  friend class MacLow;
public:
  BlockAckAgreement ();
  BlockAckAgreement (Mac48Address peer, uint8_t tid);
  ~BlockAckAgreement ();
  void SetBufferSize (uint16_t bufferSize);
  void SetTimeout (uint16_t timeout);
  void SetStartingSequence (uint16_t seq);
  void SetImmediateBlockAck (void);
  void SetDelayedBlockAck (void);
  void SetAmsduSupport (bool supported);

  uint8_t GetTid (void) const;
  Mac48Address GetPeer (void) const;
  uint16_t GetBufferSize (void) const;
  uint16_t GetTimeout (void) const;
  uint16_t GetStartingSequence (void) const;
  uint16_t GetStartingSequenceControl (void) const;
  bool IsImmediateBlockAck (void) const;
  bool IsAmsduSupported (void) const;

protected:
  Mac48Address m_peer;
  uint8_t m_amsduSupported;
  uint8_t m_blockAckPolicy; /* represents type of block ack: immediate or delayed */
  uint8_t m_tid;
  uint16_t m_bufferSize;
  uint16_t m_timeout;
  uint16_t m_startingSeq;

  EventId m_inactivityEvent;
};

} //namespace ns3

#endif /* BLOCK_ACK_AGREEMENT_H */
