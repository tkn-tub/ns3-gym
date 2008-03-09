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
#ifndef MAC48_ADDRESS_H
#define MAC48_ADDRESS_H

#include <stdint.h>
#include <ostream>
#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"

namespace ns3 {

class Address;

/**
 * \brief an EUI-48 address
 *
 * This class can contain 48 bit IEEE addresses.
 */
class Mac48Address
{
public:
  Mac48Address ();
  /**
   * \param str a string representing the new Mac48Address
   *
   * The format of the string is "xx:xx:xx:xx:xx:xx"
   */
  Mac48Address (const char *str);

  /**
   * \param buffer address in network order
   *
   * Copy the input address to our internal buffer.
   */
  void CopyFrom (const uint8_t buffer[6]);
  /**
   * \param buffer address in network order
   *
   * Copy the internal address to the input buffer.
   */
  void CopyTo (uint8_t buffer[6]) const;

  /**
   * \returns a new Address instance
   *
   * Convert an instance of this class to a polymorphic Address instance.
   */
  operator Address () const;
  /**
   * \param address a polymorphic address
   * \returns a new Mac48Address from the polymorphic address
   * 
   * This function performs a type check and asserts if the
   * type of the input address is not compatible with an
   * Mac48Address.
   */
  static Mac48Address ConvertFrom (const Address &address);
  /**
   * \returns true if the address matches, false otherwise.
   */
  static bool IsMatchingType (const Address &address);
  /**
   * Allocate a new Mac48Address.
   */
  static Mac48Address Allocate (void);

  /**
   * \returns true if this is a broadcast address, false otherwise.
   */
  bool IsBroadcast (void) const;
  /**
   * \returns true if this is a multicast address, false otherwise.
   */
  bool IsMulticast (void) const;

  /**
   * \returns the broadcast address
   */
  static Mac48Address GetBroadcast (void);

  ATTRIBUTE_HELPER_HEADER_1 (Mac48Address);
private:
  /**
   * \returns a new Address instance
   *
   * Convert an instance of this class to a polymorphic Address instance.
   */
  Address ConvertTo (void) const;
  static uint8_t GetType (void);
  friend bool operator < (const Mac48Address &a, const Mac48Address &b);
  friend bool operator == (const Mac48Address &a, const Mac48Address &b);

  uint8_t m_address[6];
};

ATTRIBUTE_HELPER_HEADER_2 (Mac48Address);

bool operator == (const Mac48Address &a, const Mac48Address &b);
bool operator != (const Mac48Address &a, const Mac48Address &b);
bool operator < (const Mac48Address &a, const Mac48Address &b);
std::ostream& operator<< (std::ostream& os, const Mac48Address & address);
std::istream& operator>> (std::istream& is, const Mac48Address & address);

} // namespace ns3

#endif /* MAC48_ADDRESS_H */
