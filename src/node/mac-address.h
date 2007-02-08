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

/*
 * This implementation borrowed from yans
 */

#ifndef MAC_ADDRESS_H
#define MAC_ADDRESS_H

#include <stdint.h>
#include <ostream>

namespace ns3 {

const int MAX_ADDR_LEN = 32;

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
  MacAddress (void);
  /* 
   * low byte should be first.
	 */
  MacAddress (uint8_t const *address, uint8_t len);
  /* The string should look like this:
   * hh:xx:xx:xx:xx:ll
   * where hh is the high byte and ll is
   * the low byte.
   */
  MacAddress (char const *address);
  ~MacAddress ();
  
  bool IsEqual (MacAddress other) const;
  void Print (std::ostream &os) const;
  
  uint8_t GetLength() const;  
  void Peek (uint8_t ad[MAX_ADDR_LEN]) const;
  void Set (uint8_t const ad[MAX_ADDR_LEN]);
  
private:
  uint8_t m_address[MAX_ADDR_LEN];
  uint8_t m_len;
};

bool operator == (MacAddress const&a, MacAddress const&b);
bool operator != (MacAddress const&a, MacAddress const&b);
bool operator < (MacAddress const&a, MacAddress const&b);

std::ostream& operator<< (std::ostream& os, MacAddress const& address);

}; // namespace ns3

#endif /* MAC_ADDRESS_H */
