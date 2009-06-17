/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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

#include "flame-protocol.h"
namespace ns3 {
namespace flame {
//-----------------------------------------------------------------------------
// FlameTag
//-----------------------------------------------------------------------------
NS_OBJECT_ENSURE_REGISTERED (FlameTag);

TypeId
FlameTag::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::flame::FlameTag")
     .SetParent<Tag> ()
     .AddConstructor<FlameTag> ();
   return tid;
}

TypeId
FlameTag::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
FlameTag::GetSerializedSize () const
{
  return (sizeof (uint16_t) + sizeof (uint8_t) + 6);
}

void
FlameTag::Serialize (TagBuffer i) const
{
  i.WriteU8 (seqno);
  i.WriteU16 (seqno);
  uint8_t buf[6];
  address.CopyTo (buf);
  for (int j = 0; j < 6; j ++)
    i.WriteU8 (buf[j]);
}

void
FlameTag::Deserialize (TagBuffer i)
{
  seqno = i.ReadU8 ();
  seqno = i.ReadU16 ();
  uint8_t buf[6];
  for (int j = 0; j < 6; j ++)
    buf[j] = i.ReadU8 ();
  address.CopyFrom (buf);
}

void
FlameTag::Print (std::ostream &os) const
{
  os << "TTL = " << seqno << ", seqno = " << seqno << "address = " << address;
}

} //namespace flame
} //namespace ns3
