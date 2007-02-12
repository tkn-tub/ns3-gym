/* -*-	Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#include <iostream>
#include <iomanip>
#include <cassert>
#include "mac-address.h"

#define ASCII_a (0x41)
#define ASCII_z (0x5a)
#define ASCII_A (0x61)
#define ASCII_Z (0x7a)
#define ASCII_COLON (0x3a)
#define ASCII_ZERO (0x30)

namespace ns3 {

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


MacAddress::MacAddress () : m_len(0)
{
  for (int i=0; i < MacAddress::MAX_LEN; i++) 
    {
      m_address[i] = 0;
    }
}

MacAddress::MacAddress (uint8_t const *address, uint8_t len)
{
  assert(len <= MacAddress::MAX_LEN);
  for (int i=0; i < len; i++) 
    {
      m_address[i] = address[i];
    }
  for (int i=len; i < MacAddress::MAX_LEN; i++) 
    {
      m_address[i] = 0;
    } 
  m_len = len;
}

MacAddress::MacAddress (char const *str)
{       
  int i = 0;
  while (*str != 0 && i < MacAddress::MAX_LEN) {
    uint8_t byte = 0;
    while (*str != ASCII_COLON && *str != 0) {
      byte <<= 4;
      char low = AsciiToLowCase (*str);
      if (low >= ASCII_a) {
        byte |= low - ASCII_a + 10;
      } else {
        byte |= low - ASCII_ZERO;
      }
      str++;
    }
    m_address[i] = byte;
    i++;
    if (*str == 0) {
      break;
    }
    str++;
  }
  m_len = i;
}

MacAddress::~MacAddress ()
{}

bool 
MacAddress::IsEqual (MacAddress other) const
{
    if (memcmp(other.m_address, m_address, m_len)) 
      {
        return false;
      } 
    else 
      {
        return true;
      } 
}

void
MacAddress::Print (std::ostream &os) const
{
    int i;
    if (m_len == 0) 
      {
        os << "NULL-ADDRESS";
        return;
      }
    os.setf (std::ios::hex, std::ios::basefield);
    std::cout.fill('0');
    for (i=0; i< (m_len-1); i++) 
      {
	os << std::setw(2) << (uint32_t)m_address[i] << ":";
      }
    // Final byte not suffixed by ":"
    os << std::setw(2) << (uint32_t)m_address[i];
    os.setf (std::ios::dec, std::ios::basefield);
    std::cout.fill(' ');
}

uint8_t
MacAddress::GetLength () const
{
    return m_len;
}

void
MacAddress::Peek (uint8_t ad[MacAddress::MAX_LEN]) const
{
	memcpy (ad, m_address, MacAddress::MAX_LEN);
}
void
MacAddress::Set (uint8_t const ad[MacAddress::MAX_LEN])
{
	memcpy (m_address, ad, MacAddress::MAX_LEN);
}

bool operator == (MacAddress const&a, MacAddress const&b)
{
	return a.IsEqual (b);
}

bool operator != (MacAddress const&a, MacAddress const&b)
{
	return !a.IsEqual (b);
}

bool operator < (MacAddress const&a, MacAddress const&b)
{
        uint8_t a_p[MacAddress::MAX_LEN];
        uint8_t b_p[MacAddress::MAX_LEN];
        a.Peek (a_p);
        b.Peek (b_p);
        assert(a.GetLength() == b.GetLength());
        for (uint8_t i = 0; i < a.GetLength(); i++) 
          {
            if (a_p[i] < b_p[i]) 
              {
                return true;
              } 
            else if (a_p[i] > b_p[i]) 
              {
                return false;
              }
          }
        return false;
}

std::ostream& operator<< (std::ostream& os, MacAddress const& address)
{
	address.Print (os);
	return os;
}


}; // namespace ns3
