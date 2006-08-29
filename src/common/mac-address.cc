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

#include "mac-address.h"
#include "utils.h"

namespace ns3 {

MacAddress::MacAddress ()
{
	m_address[0] = 0;
	m_address[1] = 0;
	m_address[2] = 0;
	m_address[3] = 0;
	m_address[4] = 0;
	m_address[5] = 0;
}

MacAddress::MacAddress (uint8_t const address[6])
{
	m_address[0] = address[0];
	m_address[1] = address[1];
	m_address[2] = address[2];
	m_address[3] = address[3];
	m_address[4] = address[4];
	m_address[5] = address[5];
}
MacAddress::MacAddress (char const *address)
{
	ascii_to_mac_network (address, m_address);
}
MacAddress::MacAddress (uint32_t ip_multicast_address)
{
	m_address[0] = 1 | ((ip_multicast_address & 0x7f) << 1);
	m_address[1] = (ip_multicast_address >> 7) & 0xff;
	m_address[2] = (ip_multicast_address >> 16) & 0xff;
	m_address[3] = 0x5e;
	m_address[4] = 0;
	m_address[5] = 0;
}
MacAddress::~MacAddress ()
{}

bool 
MacAddress::is_equal (MacAddress other) const
{
	if (m_address[0] == other.m_address[0] &&
	    m_address[1] == other.m_address[1] &&
	    m_address[2] == other.m_address[2] &&
	    m_address[3] == other.m_address[3] &&
	    m_address[4] == other.m_address[4] &&
	    m_address[5] == other.m_address[5]) {
		return true;
	} else {
		return false;
	}
}
bool 
MacAddress::is_broadcast (void) const
{
	if (m_address[0] == 0xff &&
	    m_address[1] == 0xff &&
	    m_address[2] == 0xff &&
	    m_address[3] == 0xff &&
	    m_address[4] == 0xff &&
	    m_address[5] == 0xff) {
		return true;
	} else {
		return false;
	}
}
bool 
MacAddress::is_multicast (void) const
{
	if (m_address[0] & 0x1) {
		return true;
	} else {
		return false;
	}
}
bool 
MacAddress::is_multicast_equal (MacAddress other) const
{
	if (get_multicast_part () == other.get_multicast_part ()) {
		return true;
	} else {
		return false;
	}
}

uint32_t
MacAddress::get_multicast_part (void) const
{
	uint32_t part = 0;
	part |= m_address[0] >> 1;
	part |= m_address[1] << 7;
	part |= (m_address[1] << 15);
	return part;
}


void
MacAddress::peek (uint8_t ad[6]) const
{
	memcpy (ad, m_address, 6);
}
void
MacAddress::set (uint8_t const ad[6])
{
	memcpy (m_address, ad, 6);
}

void
MacAddress::print (std::ostream *os) const
{
	os->setf (std::ios::hex, std::ios::basefield);
	*os << (uint32_t)m_address[0] << ":"
	    << (uint32_t)m_address[1] << ":"
	    << (uint32_t)m_address[2] << ":"
	    << (uint32_t)m_address[3] << ":"
	    << (uint32_t)m_address[4] << ":"
	    << (uint32_t)m_address[5];
	os->setf (std::ios::dec, std::ios::basefield);
}




MacAddress
MacAddress::get_broadcast (void)
{
	static MacAddress broadcast = MacAddress ("ff:ff:ff:ff:ff:ff");
	return broadcast;
}

bool operator == (MacAddress const&a, MacAddress const&b)
{
	return a.is_equal (b);
}

bool operator != (MacAddress const&a, MacAddress const&b)
{
	return !a.is_equal (b);
}

bool operator < (MacAddress const&a, MacAddress const&b)
{
	uint8_t a_p[6];
	uint8_t b_p[6];
	a.peek (a_p);
	b.peek (b_p);
	for (uint8_t i = 0; i < 6; i++) {
		if (a_p[i] < b_p[i]) {
			return true;
		} else if (a_p[i] > b_p[i]) {
			return false;
		}
	}
	return false;
}

std::ostream& operator<< (std::ostream& os, MacAddress const& address)
{
	address.print (&os);
	return os;
}


}; // namespace ns3
