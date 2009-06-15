/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#include "ie-dot11s-id.h"
#include "ns3/assert.h"

namespace ns3 {
namespace dot11s {
IeMeshId::IeMeshId ()
{
  for (uint8_t i = 0; i < 32; i++) 
    {
      m_meshId[i] = 0;
    }
}
IeMeshId::IeMeshId (std::string s)
{
  NS_ASSERT (s.size () < 32);
  const char *meshid = s.c_str ();
  uint8_t len = 0;
  while (*meshid != 0 && len < 32) 
    {
      m_meshId[len] = *meshid;
      meshid++;
      len++;
    }
  NS_ASSERT (len <= 32);
  while (len < 33) 
    {
      m_meshId[len] = 0;
      len++;
    }
}
bool 
IeMeshId::IsEqual (IeMeshId const &o) const
{
  uint8_t i = 0;
  while (i < 32 && 
         m_meshId[i] == o.m_meshId[i] &&
         m_meshId[i] != 0) 
    {
      i++;
    }
  if (m_meshId[i] != o.m_meshId[i]) 
    {
      return false;
    }
  return true;
}
bool 
IeMeshId::IsBroadcast (void) const
{
  if (m_meshId[0] == 0) 
    {
      return true;
    }
  return false;
}
char *
IeMeshId::PeekString (void) const
{
  return (char *)m_meshId;
}
uint8_t 
IeMeshId::GetInformationSize (void) const
{
  uint8_t size = 0;
  while (m_meshId[size] != 0 && size < 32)
  {
    size++;
  }
  NS_ASSERT (size <= 32);
  return size;
}
void
IeMeshId::SerializeInformation (Buffer::Iterator i) const
{
  uint8_t size = 0;
  while (m_meshId[size] != 0 && size < 32)
  {
    i.WriteU8 (m_meshId [size]);
    size ++;
  }
}
uint8_t
IeMeshId::DeserializeInformation (Buffer::Iterator start, uint8_t length)
{
  Buffer::Iterator i = start;
  NS_ASSERT (length <= 32);
  i.Read (m_meshId, length);
  m_meshId[length] = 0;
  return i.GetDistanceFrom(start);
}
void 
IeMeshId::PrintInformation (std::ostream& os) const
{
  os << "meshId =  " << PeekString (); 
}
bool operator== (const IeMeshId & a, const IeMeshId & b)
{
  bool result (true);
  uint8_t size = 0;

  while(size < 32)
  {
    result = result && (a.m_meshId[size] == b.m_meshId[size]);
    if(a.m_meshId[size] == 0)
      return result;
    size ++;
  }
  return result;
};

std::ostream &
operator << (std::ostream &os, const IeMeshId &meshId)
{
  os << meshId.PeekString ();
  return os;
}
#ifdef RUN_SELF_TESTS

/// Built-in self test for IeMeshId
struct IeMeshIdBist : public IeTest 
{
  IeMeshIdBist () : IeTest ("Mesh/802.11s/IE/MESH_ID") {}
  virtual bool RunTests(); 
};

/// Test instance
static IeMeshIdBist g_IeMeshIdBist;

bool IeMeshIdBist::RunTests ()
{
  bool result(true);
  
  // create test information element
  IeMeshId a("qwerty");
  result = result && TestRoundtripSerialization (a);
  return result;
}
#endif // RUN_SELF_TESTS
} //namespace dot11s
} // namespace ns3
