/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
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

class MacAddress {
public:
	MacAddress (void);
	/* low byte should be first.
	 */
	MacAddress (uint8_t const address[6]);
	/* The string should look like this:
	 * hh-xx-xx-xx-xx-ll
	 * where hh is the high byte and ll is
	 * the low byte.
	 */
	MacAddress (char const *address);
	/* create the mac address associated to 
	 * this multicast ip address. 
	 */
	MacAddress (uint32_t multicast_ip_address);
	~MacAddress ();

	bool is_equal (MacAddress other) const;
	bool is_broadcast (void) const;
	bool is_multicast (void) const;
	bool is_multicast_equal (MacAddress other) const;

	void print (std::ostream *os) const;

	void peek (uint8_t ad[6]) const;
	void set (uint8_t const ad[6]);

	static MacAddress get_broadcast (void);
private:
	uint32_t get_multicast_part (void) const;
	uint8_t m_address[6];
};

bool operator == (MacAddress const&a, MacAddress const&b);
bool operator != (MacAddress const&a, MacAddress const&b);
bool operator < (MacAddress const&a, MacAddress const&b);

std::ostream& operator<< (std::ostream& os, MacAddress const& address);

}; // namespace ns3

#endif /* MAC_ADDRESS_H */
