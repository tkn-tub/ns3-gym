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

#ifndef MAC_ADDRESS_H
#define MAC_ADDRESS_H

#include <stdint.h>
#include <ostream>

namespace ns3 {

/**
 * \brief base class for Network Device addresses
 *
 * This class is a base class for different types of Network
 * Device addresses.  It generically stores an address of 
 * MAX_ADDR_LEN bytes, and provides methods to compare, print, and set
 * the address.
 */
class MacAddress {
public:
  enum {
    MAX_LEN = 32
  };
  /**
   * \brief Construct a null MacAddress
   *
   * This MacAddress has length of zero, and is internally all zeros
   */
  MacAddress (void);
  /**
   * \brief Construct a MacAddress from a byte-array
   *
   * low byte should be first.
   * \param address a byte array indicating the address
   * \param len length, in bytes, of the address points to
   */
  MacAddress (uint8_t const *address, uint8_t len);
  /**
   * \brief Construct a MacAddress from a C-string
   *
   * The string should look like this:
   * hh:xx:xx:xx:xx:ll
   * where hh is the high byte and ll is
   * the low byte.
   * \param address the C-string representation of the address
   */
  MacAddress (char const *address);
  ~MacAddress ();
  
  /**
   * \brief Comparison operation between MacAddresses
   * \param other The address against which to compare this one
   * \return True if they are equal, false otherwise.
   */   
  bool IsEqual (MacAddress other) const;
  /**
   * \brief Print this MacAddress to a stream
   *
   * The format is colon seperated groups of two hexadecimal digits
   * \param os The output stream desired
   */ 
  void Print (std::ostream &os) const;
  
  /**
   * \return The length in bytes of this MacAddress
   */
  uint8_t GetLength() const;
  /**
   * \brief Copy routine to peek the contents of the MacAddress
   *
   * \param ad Output parameter which holds a copy of this MacAddress
   */
  void Peek (uint8_t ad[MAX_LEN]) const;
  /**
   * \brief Sets this MacAddress to a specific value
   * \param ad byte buffer to set the MacAddress to
   * \param len the length of the buffer
   */
  void Set (uint8_t const ad[MAX_LEN], uint8_t len);

private:
  uint8_t m_address[MAX_LEN];
  uint8_t m_len;
};

bool operator == (MacAddress const&a, MacAddress const&b);
bool operator != (MacAddress const&a, MacAddress const&b);
bool operator < (MacAddress const&a, MacAddress const&b);

std::ostream& operator<< (std::ostream& os, MacAddress const& address);

}; // namespace ns3

#endif /* MAC_ADDRESS_H */
