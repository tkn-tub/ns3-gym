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
#include "block-ack-cache.h"
#include "ctrl-headers.h"
#include "wifi-mac-header.h"
#include "qos-utils.h"

#define WINSIZE_ASSERT NS_ASSERT ((m_winEnd - m_winStart + 4096) % 4096 == m_winSize - 1)

namespace ns3 {

void
BlockAckCache::Init (uint16_t winStart, uint16_t winSize)
{
  m_winStart = winStart;
  m_winSize = winSize <= 64 ? winSize : 64;
  m_winEnd = (m_winStart + m_winSize - 1) % 4096;
  memset (m_bitmap, 0, sizeof (m_bitmap));
}

void
BlockAckCache::UpdateWithMpdu (const WifiMacHeader *hdr)
{
  uint16_t seqNumber = hdr->GetSequenceNumber ();
  if (!QosUtilsIsOldPacket (m_winStart, seqNumber))
    {
      if (!IsInWindow (seqNumber))
        {
          uint16_t delta = (seqNumber - m_winEnd + 4096) % 4096;
          if (delta > 1)
            {
              ResetPortionOfBitmap ((m_winEnd + 1) % 4096, ((seqNumber - 1) + 4096) % 4096);
            }
          m_winStart = (m_winStart + delta) % 4096;
          m_winEnd = seqNumber;

          WINSIZE_ASSERT;
        }
      m_bitmap[seqNumber] |= (0x0001 << hdr->GetFragmentNumber ());
    }
}

void
BlockAckCache::UpdateWithBlockAckReq (uint16_t startingSeq)
{
  if (!QosUtilsIsOldPacket (m_winStart, startingSeq))
    {
      if (IsInWindow (startingSeq))
        {
          if (startingSeq != m_winStart)
            {
              m_winStart = startingSeq;
              uint16_t newWinEnd = (m_winStart + m_winSize - 1) % 4096;
              ResetPortionOfBitmap ((m_winEnd + 1) % 4096, newWinEnd);
              m_winEnd = newWinEnd;

              WINSIZE_ASSERT;
            }
        }
      else
        {
          m_winStart = startingSeq;
          m_winEnd = (m_winStart + m_winSize - 1) % 4096;
          ResetPortionOfBitmap (m_winStart, m_winEnd);

          WINSIZE_ASSERT;
        }
    }
}

void
BlockAckCache::ResetPortionOfBitmap (uint16_t start, uint16_t end)
{
  uint32_t i = start;
  for (; i != end; i = (i + 1) % 4096)
    {
      m_bitmap[i] = 0;
    }
  m_bitmap[i] = 0;
}

bool
BlockAckCache::IsInWindow (uint16_t seq)
{
  return ((seq - m_winStart + 4096) % 4096) < m_winSize;
}

void
BlockAckCache::FillBlockAckBitmap (CtrlBAckResponseHeader *blockAckHeader)
{
  if (blockAckHeader->IsBasic ())
    {
      NS_FATAL_ERROR ("Basic block ack is only partially implemented.");
    }
  else if (blockAckHeader->IsCompressed ())
    {
      uint32_t i = blockAckHeader->GetStartingSequence ();
      uint32_t end = (i + m_winSize - 1) % 4096;
      for (; i != end; i = (i + 1) % 4096)
        {
          if (m_bitmap[i] == 1)
            {
              blockAckHeader->SetReceivedPacket (i);
            }
        }
      if (m_bitmap[i] == 1)
        {
          blockAckHeader->SetReceivedPacket (i);
        }
    }
  else if (blockAckHeader->IsMultiTid ())
    {
      NS_FATAL_ERROR ("Multi-tid block ack is not supported.");
    }
}

} // namespace ns3
