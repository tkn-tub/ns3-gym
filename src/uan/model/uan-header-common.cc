/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

#include "uan-header-common.h"
#include "uan-address.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (UanHeaderCommon);

UanHeaderCommon::UanHeaderCommon ()
{
}

UanHeaderCommon::UanHeaderCommon (const UanAddress src, const UanAddress dest, uint8_t type)
  : Header (),
    m_dest (dest),
    m_src (src),
    m_type (type)
{

}

TypeId
UanHeaderCommon::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UanHeaderCommon")
    .SetParent<Header> ()
    .AddConstructor<UanHeaderCommon> ()
  ;
  return tid;
}

TypeId
UanHeaderCommon::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
UanHeaderCommon::~UanHeaderCommon ()
{
}


void
UanHeaderCommon::SetDest (UanAddress dest)
{
  m_dest = dest;
}
void
UanHeaderCommon::SetSrc (UanAddress src)
{
  m_src = src;
}

void
UanHeaderCommon::SetType (uint8_t type)
{
  m_type = type;
}

UanAddress
UanHeaderCommon::GetDest (void) const
{
  return m_dest;
}
UanAddress
UanHeaderCommon::GetSrc (void) const
{
  return m_src;
}
uint8_t
UanHeaderCommon::GetType (void) const
{
  return m_type;
}

// Inherrited methods

uint32_t
UanHeaderCommon::GetSerializedSize (void) const
{
  return 1 + 1 + 1;
}

void
UanHeaderCommon::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (m_src.GetAsInt ());
  start.WriteU8 (m_dest.GetAsInt ());
  start.WriteU8 (m_type);
}

uint32_t
UanHeaderCommon::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator rbuf = start;

  m_src = UanAddress (rbuf.ReadU8 ());
  m_dest = UanAddress (rbuf.ReadU8 ());
  m_type = rbuf.ReadU8 ();

  return rbuf.GetDistanceFrom (start);
}

void
UanHeaderCommon::Print (std::ostream &os) const
{
  os << "UAN src=" << m_src << " dest=" << m_dest << " type=" << (uint32_t) m_type;
}




} // namespace ns3
