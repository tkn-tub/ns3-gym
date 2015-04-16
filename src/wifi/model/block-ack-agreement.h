/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
  /**
   * Constructor for BlockAckAgreement with given peer and TID.
   *
   * \param peer
   * \param tid
   */
  BlockAckAgreement (Mac48Address peer, uint8_t tid);
  ~BlockAckAgreement ();
  /**
   * Set buffer size.
   *
   * \param bufferSize
   */
  void SetBufferSize (uint16_t bufferSize);
  /**
   * Set timeout.
   *
   * \param timeout
   */
  void SetTimeout (uint16_t timeout);
  /**
   * Set starting sequence number.
   *
   * \param seq the starting sequence number
   */
  void SetStartingSequence (uint16_t seq);
  /**
   * Set Block ACK policy to immediate ACK.
   */
  void SetImmediateBlockAck (void);
  /**
   * Set Block ACK policy to delayed ACK.
   */
  void SetDelayedBlockAck (void);
  /**
   * Enable or disable A-MSDU support.
   *
   * \param supported enable or disable A-MSDU support
   */
  void SetAmsduSupport (bool supported);
  /**
   * Set ending sequence number.
   *
   * \param seq the ending sequence number
   */
  void SetWinEnd (uint16_t seq);
  /**
   * Return the Traffic ID (TID).
   *
   * \return TID
   */
  uint8_t GetTid (void) const;
  /**
   * Return the peer address.
   *
   * \return Mac48Address
   */
  Mac48Address GetPeer (void) const;
  /**
   * Return the buffer size.
   *
   * \return buffer size
   */
  uint16_t GetBufferSize (void) const;
  /**
   * Return the timeout.
   *
   * \return timeout
   */
  uint16_t GetTimeout (void) const;
  /**
   * Return the starting squence number.
   *
   * \return starting sequence number
   */
  uint16_t GetStartingSequence (void) const;
  /**
   * Return the starting squence control
   *
   * \return starting sequence control
   */
  uint16_t GetStartingSequenceControl (void) const;
  /**
   * Return the ending sequence number
   *
   * \return ending sequence number
   */
  uint16_t GetWinEnd (void) const;
  /**
   * Check whether the current ACK policy is immediate block ACK.
   *
   * \return true if the current ACK policy is immediate block ACK,
   *         false otherwise
   */
  bool IsImmediateBlockAck (void) const;
  /**
   * Check whether A-MSDU is supported
   *
   * \return true if A-MSDU is supported,
   *         false otherwise
   */
  bool IsAmsduSupported (void) const;
  /**
   * Enable or disable HT support.
   *
   * \param htSupported enable or disable HT support
   */
  void SetHtSupported (bool htSupported);
  /**
   * Check whether HT is supported
   *
   * \return true if HT is supported,
   *         false otherwise
   */
  bool IsHtSupported (void) const;

protected:
  Mac48Address m_peer;       //!< Peer address
  uint8_t m_amsduSupported;  //!< Flag whether MSDU aggregation is supported
  uint8_t m_blockAckPolicy;  //!< Type of block ack: immediate or delayed
  uint8_t m_tid;             //!< Traffic ID
  uint16_t m_bufferSize;     //!< Buffer size
  uint16_t m_timeout;        //!< Timeout
  uint16_t m_startingSeq;    //!< Starting squence control
  uint16_t m_winEnd;         //!< Ending sequence number
  uint8_t m_htSupported;     //!< Flag whether HT is supported
  EventId m_inactivityEvent; //!<
};

} // namespace ns3

#endif /* BLOCK_ACK_AGREEMENT_H */
