/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#include "qos-tag.h"
#include "ns3/tag.h"
#include "ns3/uinteger.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (QosTag);

TypeId
QosTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::QosTag")
    .SetParent<Tag> ()
    .SetGroupName ("Wifi")
    .AddConstructor<QosTag> ()
    .AddAttribute ("tid", "The tid that indicates AC which packet belongs",
                   UintegerValue (0),
                   MakeUintegerAccessor (&QosTag::GetTid),
                   MakeUintegerChecker<uint8_t> ())
  ;
  return tid;
}

TypeId
QosTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

QosTag::QosTag ()
  : m_tid (0)
{
}
QosTag::QosTag (uint8_t tid)
  : m_tid (tid)
{
}

void
QosTag::SetTid (uint8_t tid)
{
  m_tid = tid;
}

void
QosTag::SetUserPriority (UserPriority up)
{
  m_tid = up;
}

uint32_t
QosTag::GetSerializedSize (void) const
{
  return 1;
}

void
QosTag::Serialize (TagBuffer i) const
{
  i.WriteU8 (m_tid);
}

void
QosTag::Deserialize (TagBuffer i)
{
  m_tid = (UserPriority) i.ReadU8 ();
}

uint8_t
QosTag::GetTid () const
{
  return m_tid;
}

void
QosTag::Print (std::ostream &os) const
{
  os << "Tid=" << static_cast<int> (m_tid);
}

} // namespace ns3
