/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Mirko Banchi <mk.banchi@gmail.com>
 */

#include "mac-low-transmission-parameters.h"

namespace ns3 {

MacLowTransmissionParameters::MacLowTransmissionParameters ()
  : m_nextSize (0),
    m_waitAck (ACK_NONE),
    m_sendRts (false)
{
}

void
MacLowTransmissionParameters::EnableNextData (uint32_t size)
{
  m_nextSize = size;
}

void
MacLowTransmissionParameters::DisableNextData (void)
{
  m_nextSize = 0;
}

void
MacLowTransmissionParameters::EnableBasicBlockAck (void)
{
  m_waitAck = BLOCK_ACK_BASIC;
}

void
MacLowTransmissionParameters::EnableCompressedBlockAck (void)
{
  m_waitAck = BLOCK_ACK_COMPRESSED;
}

void
MacLowTransmissionParameters::EnableMultiTidBlockAck (void)
{
  m_waitAck = BLOCK_ACK_MULTI_TID;
}

void
MacLowTransmissionParameters::EnableAck (void)
{
  m_waitAck = ACK_NORMAL;
}

void
MacLowTransmissionParameters::DisableAck (void)
{
  m_waitAck = ACK_NONE;
}

void
MacLowTransmissionParameters::EnableRts (void)
{
  m_sendRts = true;
}

void
MacLowTransmissionParameters::DisableRts (void)
{
  m_sendRts = false;
}

bool
MacLowTransmissionParameters::MustWaitNormalAck (void) const
{
  return (m_waitAck == ACK_NORMAL);
}

bool
MacLowTransmissionParameters::MustWaitBasicBlockAck (void) const
{
  return (m_waitAck == BLOCK_ACK_BASIC) ? true : false;
}

bool
MacLowTransmissionParameters::MustWaitCompressedBlockAck (void) const
{
  return (m_waitAck == BLOCK_ACK_COMPRESSED) ? true : false;
}

bool
MacLowTransmissionParameters::MustWaitMultiTidBlockAck (void) const
{
  return (m_waitAck == BLOCK_ACK_MULTI_TID) ? true : false;
}

bool
MacLowTransmissionParameters::MustSendRts (void) const
{
  return m_sendRts;
}

bool
MacLowTransmissionParameters::HasNextPacket (void) const
{
  return (m_nextSize != 0);
}

uint32_t
MacLowTransmissionParameters::GetNextPacketSize (void) const
{
  NS_ASSERT (HasNextPacket ());
  return m_nextSize;
}

std::ostream &operator << (std::ostream &os, const MacLowTransmissionParameters &params)
{
  os << "["
     << "send rts=" << params.m_sendRts << ", "
     << "next size=" << params.m_nextSize << ", "
     << "ack=";
  switch (params.m_waitAck)
    {
    case MacLowTransmissionParameters::ACK_NONE:
      os << "none";
      break;
    case MacLowTransmissionParameters::ACK_NORMAL:
      os << "normal";
      break;
    case MacLowTransmissionParameters::BLOCK_ACK_BASIC:
      os << "basic-block-ack";
      break;
    case MacLowTransmissionParameters::BLOCK_ACK_COMPRESSED:
      os << "compressed-block-ack";
      break;
    case MacLowTransmissionParameters::BLOCK_ACK_MULTI_TID:
      os << "multi-tid-block-ack";
      break;
    }
  os << "]";
  return os;
}

} //namespace ns3
