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
#include "ssid.h"
#include "ns3/assert.h"

#define ELEMENT_ID (0)

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
  NS_ASSERT (len <= 32);
  m_length = len;
  while (len < 33) 
    {
      m_ssid[len] = 0;
      len++;
    }
}
Ssid::Ssid (char const ssid[32], uint8_t length)
{
  NS_ASSERT (length <= 32);
  uint8_t len = 0;
  while (len < length) 
    {
      m_ssid[len] = ssid[len];
      len++;
    }
  m_length = length;
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
  NS_ASSERT (size <= 32);
  return size;
}

char *
Ssid::PeekString (void) const
{
  // it is safe to return a pointer to the buffer because it is
  // guaranteed to be zero-terminated.
  return (char *)m_ssid;
}

uint32_t 
Ssid::GetSerializedSize (void) const
{
  return 1 + 1 + m_length;
}
Buffer::Iterator 
Ssid::Serialize (Buffer::Iterator i) const
{
  NS_ASSERT (m_length <= 32);
  i.WriteU8 (ELEMENT_ID);
  i.WriteU8 (m_length);
  i.Write (m_ssid, m_length);
  return i;
}
Buffer::Iterator 
Ssid::Deserialize (Buffer::Iterator i)
{
  uint8_t elementId;
  elementId = i.ReadU8 ();
  NS_ASSERT (elementId == ELEMENT_ID);
  m_length = i.ReadU8 ();
  NS_ASSERT (m_length <= 32);
  i.Read (m_ssid, m_length);
  return i;
}

ATTRIBUTE_HELPER_CPP (Ssid);

std::ostream &
operator << (std::ostream &os, const Ssid &ssid)
{
  os << ssid.PeekString ();
  return os;
}

std::istream &operator >> (std::istream &is, Ssid &ssid)
{
  std::string str;
  is >> str;
  ssid = Ssid (str.c_str ());
  return is;
}


} // namespace ns3
