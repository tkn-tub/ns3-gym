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
#include "block-ack-agreement.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("BlockAckAgreement");

BlockAckAgreement::BlockAckAgreement ()
  : m_amsduSupported (0),
    m_blockAckPolicy (1),
    m_htSupported (0),
    m_inactivityEvent ()
{
  NS_LOG_FUNCTION (this);
}

BlockAckAgreement::BlockAckAgreement (Mac48Address peer, uint8_t tid)
  : m_amsduSupported (0),
    m_blockAckPolicy (1),
    m_htSupported (0),
    m_inactivityEvent ()
{
  NS_LOG_FUNCTION (this << peer << static_cast<uint32_t> (tid));
  m_tid = tid;
  m_peer = peer;
}

BlockAckAgreement::~BlockAckAgreement ()
{
  NS_LOG_FUNCTION (this);
  m_inactivityEvent.Cancel ();
}

void
BlockAckAgreement::SetBufferSize (uint16_t bufferSize)
{
  NS_LOG_FUNCTION (this << bufferSize);
  NS_ASSERT (bufferSize <= 1024);
  NS_ASSERT (bufferSize % 16 == 0);
  m_bufferSize = bufferSize;
}
void
BlockAckAgreement::SetTimeout (uint16_t timeout)
{
  NS_LOG_FUNCTION (this << timeout);
  m_timeout = timeout;
}
void
BlockAckAgreement::SetStartingSequence (uint16_t seq)
{
  NS_LOG_FUNCTION (this << seq);
  NS_ASSERT (seq < 4096);
  m_startingSeq = seq;
}
void
BlockAckAgreement::SetImmediateBlockAck (void)
{
  NS_LOG_FUNCTION (this);
  m_blockAckPolicy = 1;
}
void
BlockAckAgreement::SetDelayedBlockAck (void)
{
  NS_LOG_FUNCTION (this);
  m_blockAckPolicy = 0;
}
void
BlockAckAgreement::SetAmsduSupport (bool supported)
{
  NS_LOG_FUNCTION (this << supported);
  m_amsduSupported = supported;
}

uint8_t
BlockAckAgreement::GetTid (void) const
{
  NS_LOG_FUNCTION (this);
  return m_tid;
}
Mac48Address
BlockAckAgreement::GetPeer (void) const
{
  NS_LOG_FUNCTION (this);
  return m_peer;
}
uint16_t
BlockAckAgreement::GetBufferSize (void) const
{
  NS_LOG_FUNCTION (this);
  return m_bufferSize;
}
uint16_t
BlockAckAgreement::GetTimeout (void) const
{
  NS_LOG_FUNCTION (this);
  return m_timeout;
}
uint16_t
BlockAckAgreement::GetStartingSequence (void) const
{
  NS_LOG_FUNCTION (this);
  return m_startingSeq;
}
uint16_t
BlockAckAgreement::GetStartingSequenceControl (void) const
{
  NS_LOG_FUNCTION (this);
  uint16_t seqControl = (m_startingSeq << 4) | 0xfff0;
  return seqControl;
}
bool
BlockAckAgreement::IsImmediateBlockAck (void) const
{
  NS_LOG_FUNCTION (this);
  return (m_blockAckPolicy == 1);
}
bool
BlockAckAgreement::IsAmsduSupported (void) const
{
  NS_LOG_FUNCTION (this);
  return (m_amsduSupported == 1) ? true : false;
}
uint16_t
BlockAckAgreement::GetWinEnd (void) const
{
  return m_winEnd;
}
void
BlockAckAgreement::SetWinEnd (uint16_t seq)
{
  m_winEnd = seq;
}
void
BlockAckAgreement::SetHtSupported (bool htSupported)
{
  NS_LOG_FUNCTION (this << htSupported);
  m_htSupported = htSupported;
}
bool
BlockAckAgreement::IsHtSupported (void) const
{
    NS_LOG_FUNCTION (this);
    return (m_htSupported == 1) ? true : false;
}

} // namespace ns3
