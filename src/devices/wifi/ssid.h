/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#ifndef SSID_H
#define SSID_H

#include <stdint.h>
#include "ns3/buffer.h"

namespace ns3 {

class Ssid 
{
public:
  // broadcast ssid
  Ssid ();
  /* 0-terminated string */
  Ssid (char const *ssid);
  Ssid (char const ssid[32], uint8_t length);

  bool IsEqual (Ssid const &o) const;
  bool IsBroadcast (void) const;

  uint32_t GetLength (void) const;

  uint32_t GetSerializedSize (void) const;
  Buffer::Iterator Serialize (Buffer::Iterator i) const;
  Buffer::Iterator Deserialize (Buffer::Iterator i);
private:
  uint8_t m_ssid[33];
  uint8_t m_length;
};

std::ostream &operator << (std::ostream &os, const Ssid &ssid);

} // namespace ns3

#endif /* SSID_H */
