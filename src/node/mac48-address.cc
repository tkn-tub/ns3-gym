/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "mac48-address.h"
#include "address.h"
#include "ns3/assert.h"
#include <iomanip>
#include <iostream>

namespace ns3 {

#define ASCII_a (0x41)
#define ASCII_z (0x5a)
#define ASCII_A (0x61)
#define ASCII_Z (0x7a)
#define ASCII_COLON (0x3a)
#define ASCII_ZERO (0x30)

static char
AsciiToLowCase (char c)
{
  if (c >= ASCII_a && c <= ASCII_z) {
    return c;
  } else if (c >= ASCII_A && c <= ASCII_Z) {
    return c + (ASCII_a - ASCII_A);
  } else {
    return c;
  }
}


Mac48Address::Mac48Address ()
{
  memset (m_address, 0, 6);
}
Mac48Address::Mac48Address (const char *str)
{
  int i = 0;
  while (*str != 0 && i < 6) 
    {
      uint8_t byte = 0;
      while (*str != ASCII_COLON && *str != 0) 
	{
	  byte <<= 4;
	  char low = AsciiToLowCase (*str);
	  if (low >= ASCII_a) 
	    {
	      byte |= low - ASCII_a + 10;
	    } 
	  else 
	    {
	      byte |= low - ASCII_ZERO;
	    }
	  str++;
	}
      m_address[i] = byte;
      i++;
      if (*str == 0) 
	{
	  break;
	}
      str++;
    }
  NS_ASSERT (i == 6);
}
void 
Mac48Address::CopyFrom (const uint8_t buffer[6])
{
  memcpy (m_address, buffer, 6);
}
void 
Mac48Address::CopyTo (uint8_t buffer[6]) const
{
  memcpy (buffer, m_address, 6);
}

bool 
Mac48Address::IsMatchingType (const Address &address)
{
  return address.CheckCompatible (GetType (), 6);
}
Mac48Address::operator Address () const
{
  return ConvertTo ();
}
Address 
Mac48Address::ConvertTo (void) const
{
  return Address (GetType (), m_address, 6);
}
Mac48Address 
Mac48Address::ConvertFrom (const Address &address)
{
  NS_ASSERT (address.CheckCompatible (GetType (), 6));
  Mac48Address retval;
  address.CopyTo (retval.m_address);
  return retval;
}
Mac48Address 
Mac48Address::Allocate (void)
{
  static uint64_t id = 0;
  id++;
  Mac48Address address;
  address.m_address[0] = (id >> 40) & 0xff;
  address.m_address[1] = (id >> 32) & 0xff;
  address.m_address[2] = (id >> 24) & 0xff;
  address.m_address[3] = (id >> 16) & 0xff;
  address.m_address[4] = (id >> 8) & 0xff;
  address.m_address[5] = (id >> 0) & 0xff;
  return address;
}
uint8_t 
Mac48Address::GetType (void)
{
  static uint8_t type = Address::Register ();
  return type;
}

bool operator == (const Mac48Address &a, const Mac48Address &b)
{
  uint8_t ada[6];
  uint8_t adb[6];
  a.CopyTo (ada);
  b.CopyTo (adb);
  return memcmp (ada, adb, 6) == 0;
}
bool operator != (const Mac48Address &a, const Mac48Address &b)
{
  return ! (a == b);
}

std::ostream& operator<< (std::ostream& os, const Mac48Address & address)
{
  uint8_t ad[6];
  address.CopyTo (ad);

  os.setf (std::ios::hex, std::ios::basefield);
  os.fill('0');
  for (uint8_t i=0; i < 5; i++) 
    {
      os << std::setw(2) << (uint32_t)ad[i] << ":";
    }
  // Final byte not suffixed by ":"
  os << std::setw(2) << (uint32_t)ad[5];
  os.setf (std::ios::dec, std::ios::basefield);
  os.fill(' ');
  return os;
}


} // namespace ns3
