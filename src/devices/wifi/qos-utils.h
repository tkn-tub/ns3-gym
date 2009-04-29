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

enum AccessClass {
  AC_VO = 0,
  AC_VI = 1,
  AC_BE = 2,
  AC_BK = 3,
  AC_UNDEF
};

/* Maps TID (Traffic ID) to Access classes.
 * For more details see table 9-1 of IEEE802.11 standard.
 */
AccessClass QosUtilsMapTidToAc (uint8_t tid);

/* If a qos tag is attached to the packet, returns a value < 8.
   A value >= 8 is returned otherwise.
 */
uint8_t QosUtilsGetTidForPacket (Ptr<const Packet> packet);

} //namespace ns3

#endif /* QOS_UTILS_H */
