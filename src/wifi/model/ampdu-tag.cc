/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013
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
 * Author: Ghada Badawy <gbadawy@gmail.com>
 */
#include "ampdu-tag.h"
#include "ns3/tag.h"
#include "ns3/uinteger.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (AmpduTag);

TypeId
AmpduTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AmpduTag")
    .SetParent<Tag> ()
    .AddConstructor<AmpduTag> ()
    .AddAttribute ("Ampdu Exists", "The value that indicates that the packet contains an AMPDU",
                   UintegerValue (false),
                   MakeUintegerAccessor (&AmpduTag::GetAmpdu),
                   MakeUintegerChecker<uint8_t> ())
  ;
  return tid;
}

TypeId
AmpduTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

AmpduTag::AmpduTag ()
  : m_ampdu (0)
{
}

void
AmpduTag::SetAmpdu (bool supported)
{
  m_ampdu = supported;
}

void
AmpduTag::SetNoOfMpdus (uint8_t noofmpdus)
{
  NS_ASSERT (noofmpdus <= 64);
  m_noOfMpdus = noofmpdus;
}

uint32_t
AmpduTag::GetSerializedSize (void) const
{
  return 2;
}

void
AmpduTag::Serialize (TagBuffer i) const
{
  i.WriteU8 (m_ampdu);
  i.WriteU8 (m_noOfMpdus);
}

void
AmpduTag::Deserialize (TagBuffer i)
{
  m_ampdu = i.ReadU8 ();
  m_noOfMpdus = i.ReadU8 ();
}

bool
AmpduTag::GetAmpdu () const
{
  return (m_ampdu == 1) ? true : false;
}

uint8_t
AmpduTag::GetNoOfMpdus () const
{
  return m_noOfMpdus;
}

void
AmpduTag::Print (std::ostream &os) const
{
  os << "A-MPDU exists=" << m_ampdu;
}

} // namespace ns3
