/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * Copyright (c) 2011 The Boeing Company
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
 */

#include "mac16-address.h"
#include "ns3/address.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include <iomanip>
#include <iostream>
#include <cstring>

NS_LOG_COMPONENT_DEFINE ("Mac16Address");

namespace ns3 {

ATTRIBUTE_HELPER_CPP (Mac16Address);

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


Mac16Address::Mac16Address ()
{
  NS_LOG_FUNCTION (this);
  memset (m_address, 0, 2);
}

Mac16Address::Mac16Address (const char *str)
{
  NS_LOG_FUNCTION (this << str);
  int i = 0;
  while (*str != 0 && i < 2)
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
  NS_ASSERT (i == 2);
}

void
Mac16Address::CopyFrom (const uint8_t buffer[2])
{
  NS_LOG_FUNCTION (this << &buffer);
  memcpy (m_address, buffer, 2);
}
void
Mac16Address::CopyTo (uint8_t buffer[2]) const
{
  NS_LOG_FUNCTION (this << &buffer);
  memcpy (buffer, m_address, 2);
}

bool
Mac16Address::IsMatchingType (const Address &address)
{
  NS_LOG_FUNCTION (&address);
  return address.CheckCompatible (GetType (), 2);
}

Mac16Address::operator Address () const
{
  return ConvertTo ();
}

Mac16Address
Mac16Address::ConvertFrom (const Address &address)
{
  NS_LOG_FUNCTION (address);
  NS_ASSERT (address.CheckCompatible (GetType (), 2));
  Mac16Address retval;
  address.CopyTo (retval.m_address);
  return retval;
}
Address
Mac16Address::ConvertTo (void) const
{
  NS_LOG_FUNCTION (this);
  return Address (GetType (), m_address, 2);
}

Mac16Address
Mac16Address::Allocate (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  static uint64_t id = 0;
  id++;
  Mac16Address address;
  address.m_address[0] = (id >> 8) & 0xff;
  address.m_address[1] = (id >> 0) & 0xff;
  return address;
}

uint8_t
Mac16Address::GetType (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  static uint8_t type = Address::Register ();
  return type;
}

std::ostream & operator<< (std::ostream& os, const Mac16Address & address)
{
  uint8_t ad[2];
  address.CopyTo (ad);

  os.setf (std::ios::hex, std::ios::basefield);
  os.fill ('0');
  for (uint8_t i = 0; i < 1; i++)
    {
      os << std::setw (2) << (uint32_t)ad[i] << ":";
    }
  // Final byte not suffixed by ":"
  os << std::setw (2) << (uint32_t)ad[1];
  os.setf (std::ios::dec, std::ios::basefield);
  os.fill (' ');
  return os;
}

static uint8_t
AsInt (std::string v)
{
  NS_LOG_FUNCTION (v);
  std::istringstream iss;
  iss.str (v);
  uint32_t retval;
  iss >> std::hex >> retval >> std::dec;
  return retval;
}

std::istream& operator>> (std::istream& is, Mac16Address & address)
{
  std::string v;
  is >> v;

  std::string::size_type col = 0;
  for (uint8_t i = 0; i < 2; ++i)
    {
      std::string tmp;
      std::string::size_type next;
      next = v.find (":", col);
      if (next == std::string::npos)
        {
          tmp = v.substr (col, v.size ()-col);
          address.m_address[i] = AsInt (tmp);
          break;
        }
      else
        {
          tmp = v.substr (col, next-col);
          address.m_address[i] = AsInt (tmp);
          col = next + 1;
        }
    }
  return is;
}

}
