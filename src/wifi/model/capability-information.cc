/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#include "capability-information.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CapabilityInformation");

CapabilityInformation::CapabilityInformation ()
  : m_capability (0)
{
  NS_LOG_FUNCTION (this);
}

void
CapabilityInformation::SetEss (void)
{
  NS_LOG_FUNCTION (this);
  Set (0);
  Clear (1);
}

void
CapabilityInformation::SetIbss (void)
{
  NS_LOG_FUNCTION (this);
  Clear (0);
  Set (1);
}

void
CapabilityInformation::SetShortPreamble (bool shortPreamble)
{
  NS_LOG_FUNCTION (this);
  if (shortPreamble)
    {
      Set (5);
    }
}

void
CapabilityInformation::SetShortSlotTime (bool shortSlotTime)
{
  NS_LOG_FUNCTION (this);
  if (shortSlotTime)
    {
      Set (10);
    }
}

bool
CapabilityInformation::IsEss (void) const
{
  NS_LOG_FUNCTION (this);
  return Is (0);
}

bool
CapabilityInformation::IsIbss (void) const
{
  NS_LOG_FUNCTION (this);
  return Is (1);
}

bool
CapabilityInformation::IsShortPreamble (void) const
{
  NS_LOG_FUNCTION (this);
  return Is (5);
}

bool
CapabilityInformation::IsShortSlotTime (void) const
{
  NS_LOG_FUNCTION (this);
  return Is (10);
}

void
CapabilityInformation::Set (uint8_t n)
{
  NS_LOG_FUNCTION (this << static_cast<uint32_t> (n));
  uint32_t mask = 1 << n;
  m_capability |= mask;
}

void
CapabilityInformation::Clear (uint8_t n)
{
  NS_LOG_FUNCTION (this << static_cast<uint32_t> (n));
  uint32_t mask = 1 << n;
  m_capability &= ~mask;
}

bool
CapabilityInformation::Is (uint8_t n) const
{
  NS_LOG_FUNCTION (this << static_cast<uint32_t> (n));
  uint32_t mask = 1 << n;
  return (m_capability & mask) == mask;
}

uint32_t
CapabilityInformation::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return 2;
}

Buffer::Iterator
CapabilityInformation::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION (this << &start);
  start.WriteHtolsbU16 (m_capability);
  return start;
}

Buffer::Iterator
CapabilityInformation::Deserialize (Buffer::Iterator start)
{
  NS_LOG_FUNCTION (this << &start);
  m_capability = start.ReadLsbtohU16 ();
  return start;
}

} //namespace ns3
