/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#include "uan-address.h"
#include "ns3/address.h"

namespace ns3 {

UanAddress::UanAddress ()
{
  m_address = 255;
}

UanAddress::UanAddress (uint8_t addr)
  : m_address (addr)
{
}

UanAddress::~UanAddress ()
{
}

uint8_t
UanAddress::GetType (void)
{
  static uint8_t type = Address::Register ();
  return type;
}

Address
UanAddress::ConvertTo (void) const
{
  return Address (GetType (), &m_address, 1);
}

UanAddress
UanAddress::ConvertFrom (const Address &address)
{
  NS_ASSERT (IsMatchingType (address));
  UanAddress uAddr;
  address.CopyTo (&uAddr.m_address);
  return uAddr;
}

uint8_t
UanAddress::GetAsInt (void) const
{
  return m_address;
}
bool
UanAddress::IsMatchingType (const Address &address)
{
  return address.CheckCompatible (GetType (), 1);
}

UanAddress::operator Address () const
{
  return ConvertTo ();
}

void
UanAddress::CopyFrom (const uint8_t *pBuffer)
{
  m_address = *pBuffer;
}

void
UanAddress::CopyTo (uint8_t *pBuffer)
{
  *pBuffer = m_address;

}

UanAddress
UanAddress::GetBroadcast ()
{
  return UanAddress (255);
}
UanAddress
UanAddress::Allocate ()
{
  static uint8_t nextAllocated = 0;

  uint32_t address = nextAllocated++;
  if (nextAllocated == 255)
    {
      nextAllocated = 0;
    }

  return UanAddress (address);
}

bool
operator < (const UanAddress &a, const UanAddress &b)
{
  return a.m_address < b.m_address;
}

bool
operator == (const UanAddress &a, const UanAddress &b)
{
  return a.m_address == b.m_address;
}

bool
operator != (const UanAddress &a, const UanAddress &b)
{
  return !(a == b);
}

std::ostream&
operator<< (std::ostream& os, const UanAddress & address)
{
  os << (int) address.m_address;
  return os;
}
std::istream&
operator>> (std::istream& is, UanAddress & address)
{
  int x;
  is >> x;
  NS_ASSERT (0 <= x);
  NS_ASSERT (x <= 255);
  address.m_address = x;
  return is;
}

} // namespace ns3
