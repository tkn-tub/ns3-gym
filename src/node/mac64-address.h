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
#ifndef MAC64_ADDRESS_H
#define MAC64_ADDRESS_H

#include <stdint.h>
#include <ostream>

namespace ns3 {

class Address;

/**
 * \brief an EUI-64 address
 *
 * This class can contain 64 bit IEEE addresses.
 */
class Mac64Address
{
public:
  Mac64Address ();
  /**
   * \param str a string representing the new Mac64Address
   *
   * The format of the string is "xx:xx:xx:xx:xx:xx"
   */
  Mac64Address (const char *str);

  /**
   * \param buffer address in network order
   *
   * Copy the input address to our internal buffer.
   */
  void CopyFrom (const uint8_t buffer[8]);
  /**
   * \param buffer address in network order
   *
   * Copy the internal address to the input buffer.
   */
  void CopyTo (uint8_t buffer[8]) const;
  /**
   * \returns a new Address instance
   *
   * Convert an instance of this class to a polymorphic Address instance.
   */
  operator Address () const;
  /**
   * \param address a polymorphic address
   * \returns a new Mac64Address from the polymorphic address
   * 
   * This function performs a type check and asserts if the
   * type of the input address is not compatible with an
   * Mac64Address.
   */
  static Mac64Address ConvertFrom (const Address &address);
  /**
   * \returns true if the address matches, false otherwise.
   */
  static bool IsMatchingType (const Address &address);
  /**
   * Allocate a new Mac64Address.
   */
  static Mac64Address Allocate (void);
private:
  /**
   * \returns a new Address instance
   *
   * Convert an instance of this class to a polymorphic Address instance.
   */
  Address ConvertTo (void) const;
  static uint8_t GetType (void);
  uint8_t m_address[8];
};

bool operator == (const Mac64Address &a, const Mac64Address &b);
bool operator != (const Mac64Address &a, const Mac64Address &b);
std::ostream& operator<< (std::ostream& os, const Mac64Address & address);

} // namespace ns3

#endif /* MAC64_ADDRESS_H */
