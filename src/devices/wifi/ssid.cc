/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#include "ssid.h"
#include <cassert>

namespace ns3 {

Ssid::Ssid ()
{
  m_length = 0;
  for (uint8_t i = 0; i < 33; i++) 
    {
      m_ssid[i] = 0;
    }
}
Ssid::Ssid (char const *ssid)
{
  uint8_t len = 0;
  while (*ssid != 0 && len < 32) 
    {
      m_ssid[len] = *ssid;
      ssid++;
      len++;
    }
  while (len < 33) 
    {
      m_ssid[len] = 0;
      len++;
    }
}
Ssid::Ssid (char const ssid[32], uint8_t length)
{
  assert (length <= 32);
  uint8_t len = 0;
  while (len < length) 
    {
      m_ssid[len] = ssid[len];
      len++;
    }
  while (len < 33) 
    {
      m_ssid[len] = 0;
      len++;
    }
}
bool 
Ssid::IsEqual (Ssid const &o) const
{
  uint8_t i = 0;
  while (i < 32 && 
         m_ssid[i] == o.m_ssid[i] &&
         m_ssid[i] != 0) 
    {
      i++;
    }
  if (m_ssid[i] != o.m_ssid[i]) 
    {
      return false;
    }
  return true;
}
bool 
Ssid::IsBroadcast (void) const
{
  if (m_ssid[0] == 0) 
    {
      return true;
    }
  return false;
}
uint32_t 
Ssid::GetLength (void) const
{
  uint8_t size = 0;
  while (m_ssid[size] != 0 && size < 32) 
    {
      size++;
    }
  assert (size <= 32);
  return size;
}

} // namespace ns3
