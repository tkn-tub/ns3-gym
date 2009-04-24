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
#include "qos-utils.h"
#include "qos-tag.h"

namespace ns3 {

AccessClass
QosUtilsMapTidToAc (uint8_t tid)
{
  switch (tid) {
  case 0 :
    return AC_BE;
    break;
  case 1 :
    return AC_BK;
    break;
  case 2 :
    return AC_BK;
    break;
  case 3 :
    return AC_BE;
    break;
  case 4 :
    return AC_VI;
    break;
  case 5 :
    return AC_VI;
    break;
  case 6 :
    return AC_VO;
    break;
  case 7 : 
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
  if (packet->FindFirstMatchingTag (qos))
    {
      if (qos.Get () < 8)
        {
          tid = qos.Get ();
        }
    }
  return tid;
}

} //namespace ns3
