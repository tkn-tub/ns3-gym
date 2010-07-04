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
#ifndef QOS_UTILS_H
#define QOS_UTILS_H

#include "ns3/uinteger.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"

namespace ns3 {

/* This enumeration defines the Access Categories as an enumeration
 * with values corresponding to the AC index (ACI) values specified in
 * Table 7-36 of IEEE 802.11-2007.
 */
enum AcIndex {
  /** Best Effort */
  AC_BE = 0,
  /** Background */
  AC_BK = 1,
  /** Video */
  AC_VI = 2,
  /** Voice */
  AC_VO = 3,

  /** Total number of ACs */
  AC_BE_NQOS = 4,
  AC_UNDEF
};

/* Maps TID (Traffic ID) to Access classes.
 * For more details see table 9-1 of IEEE802.11 standard.
 */
AcIndex QosUtilsMapTidToAc (uint8_t tid);

/* If a qos tag is attached to the packet, returns a value < 8.
   A value >= 8 is returned otherwise.
 */
uint8_t QosUtilsGetTidForPacket (Ptr<const Packet> packet);

/*
 * Next function is useful to correctly sort buffered packets under block ack.
 * When an BAR is received from originator station, completed "old"
 * (see section 9.10.3 in IEEE802.11e) packets must be forwarded up before "new" packets.
 */
uint32_t QosUtilsMapSeqControlToUniqueInteger (uint16_t seqControl, uint16_t endSequence);

} //namespace ns3

#endif /* QOS_UTILS_H */
