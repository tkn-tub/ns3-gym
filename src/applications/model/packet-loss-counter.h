/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008,2009 INRIA, UDCAST
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
 * Author: Amine Ismail <amine.ismail@sophia.inria.fr>
 *                      <amine.ismail@udcast.com>
 *
 */

#ifndef PACKET_LOSS_COUNTER_H
#define PACKET_LOSS_COUNTER_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/address.h"

namespace ns3 {

class Socket;
class Packet;

class PacketLossCounter
{
public:
  PacketLossCounter (uint8_t bitmapSize);
  ~PacketLossCounter ();
  void NotifyReceived (uint32_t seq);
  uint32_t GetLost (void) const;
  uint16_t GetBitMapSize (void) const;
  void SetBitMapSize (uint16_t size);
private:
  bool GetBit (uint32_t seqNum);
  void SetBit (uint32_t seqNum, bool val);

  uint32_t m_lost;
  uint16_t m_bitMapSize;
  uint32_t m_lastMaxSeqNum;
  uint8_t * m_receiveBitMap;
};
}

#endif /* PACKET_LOSS_COUNTER_H */
