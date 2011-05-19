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
 * Author: Cecchi Niccolò <insa@igeek.it>
 */
#include "qos-utils.h"
#include "qos-tag.h"

namespace ns3 {

AcIndex
QosUtilsMapTidToAc (uint8_t tid)
{
  switch (tid)
    {
    case 0:
      return AC_BE;
      break;
    case 1:
      return AC_BK;
      break;
    case 2:
      return AC_BK;
      break;
    case 3:
      return AC_BE;
      break;
    case 4:
      return AC_VI;
      break;
    case 5:
      return AC_VI;
      break;
    case 6:
      return AC_VO;
      break;
    case 7:
      return AC_VO;
      break;
    }
  return AC_UNDEF;
}

uint8_t
QosUtilsGetTidForPacket (Ptr<const Packet> packet)
{
  QosTag qos;
  uint8_t tid = 8;
  if (packet->PeekPacketTag (qos))
    {
      if (qos.GetTid () < 8)
        {
          tid = qos.GetTid ();
        }
    }
  return tid;
}

uint32_t
QosUtilsMapSeqControlToUniqueInteger (uint16_t seqControl, uint16_t endSequence)
{
  uint32_t integer = 0;
  uint16_t numberSeq = (seqControl >> 4) & 0x0fff;
  integer = (4096 - (endSequence + 1) + numberSeq) % 4096;
  integer *= 16;
  integer += (seqControl & 0x000f);
  return integer;
}

bool
QosUtilsIsOldPacket (uint16_t startingSeq, uint16_t seqNumber)
{
  NS_ASSERT (startingSeq < 4096);
  NS_ASSERT (seqNumber < 4096);
  uint16_t distance = ((seqNumber - startingSeq) + 4096) % 4096;
  return (distance >= 2048);
}

} //namespace ns3
