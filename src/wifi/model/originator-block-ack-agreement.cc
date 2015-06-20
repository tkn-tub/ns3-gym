/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009, 2010 MIRKO BANCHI
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
 * Authors: Mirko Banchi <mk.banchi@gmail.com>
 *          Tommaso Pecorella <tommaso.pecorella@unifi.it>
 */

#include "originator-block-ack-agreement.h"

namespace ns3 {

OriginatorBlockAckAgreement::OriginatorBlockAckAgreement ()
  : BlockAckAgreement (),
    m_state (PENDING),
    m_sentMpdus (0),
    m_needBlockAckReq (false)
{
}

OriginatorBlockAckAgreement::OriginatorBlockAckAgreement (Mac48Address recipient, uint8_t tid)
  : BlockAckAgreement (recipient, tid),
    m_state (PENDING),
    m_sentMpdus (0),
    m_needBlockAckReq (false)
{
}

OriginatorBlockAckAgreement::~OriginatorBlockAckAgreement ()
{
}

void
OriginatorBlockAckAgreement::SetState (enum State state)
{
  m_state = state;
  if (state == INACTIVE)
    {
      m_needBlockAckReq = false;
      m_sentMpdus = 0;
    }
}

bool
OriginatorBlockAckAgreement::IsPending (void) const
{
  return (m_state == PENDING) ? true : false;
}

bool
OriginatorBlockAckAgreement::IsEstablished (void) const
{
  return (m_state == ESTABLISHED) ? true : false;
}

bool
OriginatorBlockAckAgreement::IsInactive (void) const
{
  return (m_state == INACTIVE) ? true : false;
}

bool
OriginatorBlockAckAgreement::IsUnsuccessful (void) const
{
  return (m_state == UNSUCCESSFUL) ? true : false;
}

void
OriginatorBlockAckAgreement::NotifyMpduTransmission (uint16_t nextSeqNumber)
{
  NS_ASSERT (m_sentMpdus < m_bufferSize);
  m_sentMpdus++;
  uint16_t delta = (nextSeqNumber - m_startingSeq + 4096) % 4096;
  uint16_t min = m_bufferSize < 64 ? m_bufferSize : 64;
  if (delta >= min || m_sentMpdus == m_bufferSize)
    {
      m_needBlockAckReq = true;
    }
}

bool
OriginatorBlockAckAgreement::IsBlockAckRequestNeeded (void) const
{
  return m_needBlockAckReq;
}

void
OriginatorBlockAckAgreement::CompleteExchange (void)
{
  m_needBlockAckReq = false;
  m_sentMpdus = 0;
}

} //namespace ns3
