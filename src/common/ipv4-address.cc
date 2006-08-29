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

#include "utils.h"
#include "ipv4-address.h"

namespace yans {

Ipv4Mask::Ipv4Mask ()
	: m_mask (0x66666666)
{}

Ipv4Mask::Ipv4Mask (uint32_t mask)
	: m_mask (mask)
{}
Ipv4Mask::Ipv4Mask (char const *mask)
{
	m_mask = ascii_to_ipv4_host (mask);
}

bool 
Ipv4Mask::is_equal (Ipv4Mask other) const
{
	if (other.m_mask == m_mask) {
		return true;
	} else {
		return false;
	}
}


bool 
Ipv4Mask::is_match (Ipv4Address a, Ipv4Address b) const
{
	if ((a.get_host_order () & m_mask) == (b.get_host_order () & m_mask)) {
		return true;
	} else {
		return false;
	}
}

uint32_t 
Ipv4Mask::get_host_order (void) const
{
	return m_mask;
}
void 
Ipv4Mask::set_host_order (uint32_t value)
{
	m_mask = value;
}

void 
Ipv4Mask::print (std::ostream *os) const
{
	*os << ((m_mask >> 24) & 0xff) << "."
	    << ((m_mask >> 16) & 0xff) << "."
	    << ((m_mask >> 8) & 0xff) << "."
	    << ((m_mask >> 0) & 0xff);
}


Ipv4Mask
Ipv4Mask::get_loopback (void)
{
	static Ipv4Mask loopback = Ipv4Mask ("255.0.0.0");
	return loopback;
}
Ipv4Mask
Ipv4Mask::get_zero (void)
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
	m_address = ascii_to_ipv4_host (address);
}

bool 
Ipv4Address::is_equal (Ipv4Address other) const
{
	if (other.m_address == m_address) {
		return true;
	} else {
		return false;
	}
}

bool 
Ipv4Address::is_multicast (void)
{
	// XXX
	return false;
}

uint32_t
Ipv4Address::get_host_order (void) const
{
	return m_address;
}
void 
Ipv4Address::set_host_order (uint32_t ip)
{
	m_address = ip;
}
void
Ipv4Address::serialize (uint8_t buf[4]) const
{
	buf[0] = (m_address >> 24) & 0xff;
	buf[1] = (m_address >> 16) & 0xff;
	buf[2] = (m_address >> 8) & 0xff;
	buf[3] = (m_address >> 0) & 0xff;
}

void 
Ipv4Address::print (std::ostream *os) const
{
	*os << ((m_address >> 24) & 0xff) << "."
	    << ((m_address >> 16) & 0xff) << "."
	    << ((m_address >> 8) & 0xff) << "."
	    << ((m_address >> 0) & 0xff);
}



Ipv4Address 
Ipv4Address::get_zero (void)
{
	static Ipv4Address zero ("0.0.0.0");
	return zero;
}
Ipv4Address 
Ipv4Address::get_any (void)
{
	static Ipv4Address any ("0.0.0.0");
	return any;
}
Ipv4Address 
Ipv4Address::get_broadcast (void)
{
	static Ipv4Address broadcast ("255.255.255.255");
	return broadcast;
}
Ipv4Address 
Ipv4Address::get_loopback (void)
{
	Ipv4Address loopback ("127.0.0.1");
	return loopback;
}

bool operator == (Ipv4Address const &a, Ipv4Address const &b)
{
	return a.is_equal (b);
}
bool operator != (Ipv4Address const &a, Ipv4Address const &b)
{
	return !a.is_equal (b);
}
size_t Ipv4AddressHash::operator()(Ipv4Address const &x) const 
{ 
	return x.get_host_order ();
}

std::ostream& operator<< (std::ostream& os, Ipv4Address const& address)
{
	address.print (&os);
	return os;
}
std::ostream& operator<< (std::ostream& os, Ipv4Mask const& mask)
{
	mask.print (&os);
	return os;
}


}; // namespace yans
