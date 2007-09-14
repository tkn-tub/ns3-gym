/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#include "ns3/log.h"
#include "ipv4-address.h"
#include "ns3/assert.h"

NS_LOG_COMPONENT_DEFINE("Ipv4Address");

namespace ns3 {

#define ASCII_DOT (0x2e)
#define ASCII_ZERO (0x30)

static uint32_t 
AsciiToIpv4Host (char const *address)
{
  uint32_t host = 0;
  while (true) {
    uint8_t byte = 0;
    while (*address != ASCII_DOT &&
           *address != 0) {
      byte *= 10;
      byte += *address - ASCII_ZERO;
      address++;
    }
    host <<= 8;
    host |= byte;
    if (*address == 0) {
      break;
    }
    address++;
  }
  return host;
}

}//namespace ns3

namespace ns3 {

Ipv4Mask::Ipv4Mask ()
  : m_mask (0x66666666)
{}

Ipv4Mask::Ipv4Mask (uint32_t mask)
  : m_mask (mask)
{}
Ipv4Mask::Ipv4Mask (char const *mask)
{
  m_mask = AsciiToIpv4Host (mask);
}

bool 
Ipv4Mask::IsEqual (Ipv4Mask other) const
{
  if (other.m_mask == m_mask) {
    return true;
  } else {
    return false;
  }
}


bool 
Ipv4Mask::IsMatch (Ipv4Address a, Ipv4Address b) const
{
  if ((a.GetHostOrder () & m_mask) == (b.GetHostOrder () & m_mask)) {
    return true;
  } else {
    return false;
  }
}

uint32_t 
Ipv4Mask::GetHostOrder (void) const
{
  return m_mask;
}
void 
Ipv4Mask::SetHostOrder (uint32_t value)
{
  m_mask = value;
}
uint32_t 
Ipv4Mask::GetInverse (void) const
{
  return ~m_mask;
}

void 
Ipv4Mask::Print (std::ostream &os) const
{
  os << ((m_mask >> 24) & 0xff) << "."
     << ((m_mask >> 16) & 0xff) << "."
     << ((m_mask >> 8) & 0xff) << "."
     << ((m_mask >> 0) & 0xff);
}


Ipv4Mask
Ipv4Mask::GetLoopback (void)
{
  static Ipv4Mask loopback = Ipv4Mask ("255.0.0.0");
  return loopback;
}
Ipv4Mask
Ipv4Mask::GetZero (void)
{
  static Ipv4Mask zero = Ipv4Mask ("0.0.0.0");
  return zero;
}

Ipv4Address::Ipv4Address ()
  : m_address (0x66666666)
{}
Ipv4Address::Ipv4Address (uint32_t address)
{
  m_address = address;
}
Ipv4Address::Ipv4Address (char const *address)
{
  m_address = AsciiToIpv4Host (address);
}

void
Ipv4Address::Set (uint32_t address)
{
  m_address = address;
}
void
Ipv4Address::Set (char const *address)
{
  m_address = AsciiToIpv4Host (address);
}

bool 
Ipv4Address::IsEqual (Ipv4Address other) const
{
  if (other.m_address == m_address) {
    return true;
  } else {
    return false;
  }
}

Ipv4Address
Ipv4Address::CombineMask (Ipv4Mask const &mask) const
{
  return Ipv4Address (GetHostOrder () & mask.GetHostOrder ());
}

Ipv4Address 
Ipv4Address::GetSubnetDirectedBroadcast (Ipv4Mask const &mask) const
{
  return Ipv4Address (GetHostOrder () | mask.GetInverse ());
}

bool
Ipv4Address::IsSubnetDirectedBroadcast (Ipv4Mask const &mask) const
{
  return ( (GetHostOrder () | mask.GetInverse ()) == GetHostOrder () );
}

bool
Ipv4Address::IsBroadcast (void) const
{
  return (m_address == 0xffffffffU);
}

bool 
Ipv4Address::IsMulticast (void) const
{
//
// Multicast addresses are defined as ranging from 224.0.0.0 through 
// 239.255.255.255 (which is E0000000 through EFFFFFFF in hex).
//
  return (m_address >= 0xe0000000 && m_address <= 0xefffffff);
}

uint32_t
Ipv4Address::GetHostOrder (void) const
{
  return m_address;
}
void 
Ipv4Address::SetHostOrder (uint32_t ip)
{
  m_address = ip;
}
void
Ipv4Address::Serialize (uint8_t buf[4]) const
{
  buf[0] = (m_address >> 24) & 0xff;
  buf[1] = (m_address >> 16) & 0xff;
  buf[2] = (m_address >> 8) & 0xff;
  buf[3] = (m_address >> 0) & 0xff;
}
Ipv4Address 
Ipv4Address::Deserialize (const uint8_t buf[4])
{
  Ipv4Address ipv4;
  ipv4.m_address = 0;
  ipv4.m_address |= buf[0];
  ipv4.m_address <<= 8;
  ipv4.m_address |= buf[1];
  ipv4.m_address <<= 8;
  ipv4.m_address |= buf[2];
  ipv4.m_address <<= 8;
  ipv4.m_address |= buf[3];
  return ipv4;
}

void 
Ipv4Address::Print (std::ostream &os) const
{
  os << ((m_address >> 24) & 0xff) << "."
     << ((m_address >> 16) & 0xff) << "."
     << ((m_address >> 8) & 0xff) << "."
     << ((m_address >> 0) & 0xff);
}

bool 
Ipv4Address::IsMatchingType (const Address &address)
{
  return address.CheckCompatible (GetType (), 4);
}
Ipv4Address::operator Address ()
{
  return ConvertTo ();
}

Address 
Ipv4Address::ConvertTo (void) const
{
  uint8_t buf[4];
  Serialize (buf);
  return Address (GetType (), buf, 4);
}

Ipv4Address
Ipv4Address::ConvertFrom (const Address &address)
{
  NS_ASSERT (address.CheckCompatible (GetType (), 4));
  uint8_t buf[4];
  address.CopyTo (buf);
  return Deserialize (buf);
}

uint8_t 
Ipv4Address::GetType (void)
{
  static uint8_t type = Address::Register ();
  return type;
}

Ipv4Address 
Ipv4Address::GetZero (void)
{
  static Ipv4Address zero ("0.0.0.0");
  return zero;
}
Ipv4Address 
Ipv4Address::GetAny (void)
{
  static Ipv4Address any ("0.0.0.0");
  return any;
}
Ipv4Address 
Ipv4Address::GetBroadcast (void)
{
  static Ipv4Address broadcast ("255.255.255.255");
  return broadcast;
}
Ipv4Address 
Ipv4Address::GetLoopback (void)
{
  Ipv4Address loopback ("127.0.0.1");
  return loopback;
}

bool operator == (Ipv4Address const &a, Ipv4Address const &b)
{
  return a.IsEqual (b);
}
bool operator != (Ipv4Address const &a, Ipv4Address const &b)
{
  return !a.IsEqual (b);
}
bool operator < (Ipv4Address const &addrA, Ipv4Address const &addrB)
{
  return (addrA.GetHostOrder () < addrB.GetHostOrder ());
}

size_t Ipv4AddressHash::operator()(Ipv4Address const &x) const 
{ 
  return x.GetHostOrder ();
}

std::ostream& operator<< (std::ostream& os, Ipv4Address const& address)
{
  address.Print (os);
  return os;
}
std::ostream& operator<< (std::ostream& os, Ipv4Mask const& mask)
{
  mask.Print (os);
  return os;
}
bool operator == (Ipv4Mask const &a, Ipv4Mask const &b)
{
  return a.IsEqual (b);
}
bool operator != (Ipv4Mask const &a, Ipv4Mask const &b)
{
  return !a.IsEqual (b);
}


}; // namespace ns3
