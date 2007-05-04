/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "ipv4-checksum.h"

namespace ns3 {

uint16_t 
Ipv4ChecksumCalculate (uint16_t checksum, uint8_t *buffer, uint16_t size)
{
  /* see RFC 1071 to understand this code. */
  uint32_t sum = checksum;
  uint16_t *data = (uint16_t *) buffer;
  for (uint16_t i = 0; i < (size/2); i++) {
    sum += data[i];
  }
  if ((size % 2) != 0) {
    uint8_t tmpBuf[2];
    tmpBuf[0] = buffer[size-1];
    tmpBuf[1] = 0;
    data = (uint16_t *)tmpBuf;
    sum += *data;
  }
  while (sum >> 16) {
    sum = (sum & 0xffff) + (sum >> 16);
  }
  return sum;
}

uint16_t 
Ipv4ChecksumComplete (uint16_t checksum)
{
  return ~checksum;
}

}; //namespace ns3
