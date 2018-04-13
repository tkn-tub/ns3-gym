/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 MIRKO BANCHI
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

#ifndef BLOCK_ACK_CACHE_H
#define BLOCK_ACK_CACHE_H

//#include <stdint.h>

namespace ns3 {

class WifiMacHeader;
class CtrlBAckResponseHeader;

/**
 * \ingroup wifi
 * \brief BlockAckCache cache
 *
 */
class BlockAckCache
{
public:
  /**
   * Init function
   * \param winStart the window start
   * \param winSize the window size
   */
  void Init (uint16_t winStart, uint16_t winSize);

  /**
   * Update with MPDU function
   * \param hdr the wifi MAC header
   */
  void UpdateWithMpdu (const WifiMacHeader *hdr);
  /**
   * Update with block ack request function
   * \param startingSeq the starting sequence
   */
  void UpdateWithBlockAckReq (uint16_t startingSeq);
  /**
   * When an A-MPDU is received, the window start may change to a new value
   * depending on the sequence number of the received MPDU (standard11n page 134).
   * This function is used to retrieve this value in order to add it to the BlockAck.
   * \returns window start
   */
  uint16_t GetWinStart (void) const;

  /**
   * Fill block ack bitmap function
   * \param blockAckHeader the block ack bitmap
   */
  void FillBlockAckBitmap (CtrlBAckResponseHeader *blockAckHeader);


private:
  /**
   * Reset portion of bitmap functiion
   * \param start the starting position
   * \param end the ending position
   */
  void ResetPortionOfBitmap (uint16_t start, uint16_t end);

  uint16_t m_winStart; ///< window start
  uint16_t m_winSize; ///< window size
  uint16_t m_winEnd; ///< window end

  uint16_t m_bitmap[4096]; ///< bitmap
};

} //namespace ns3

#endif /* BLOCK_ACK_CACHE_H */
