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

#ifndef IPV4_ADDRESS_H
#define IPV4_ADDRESS_H

#include <stdint.h>
#include <ostream>

namespace yans {

/* Ipv4 addresses are stored in host order in
 * this class.
 */
class Ipv4Address {
public:
	Ipv4Address ();
	/* input address is in host order. */
	Ipv4Address (uint32_t address);
	/* input address is in format:
	 * hhh.xxx.xxx.lll
	 * where h is the high byte and l the
	 * low byte
	 */
	Ipv4Address (char const *address);

	bool is_equal (Ipv4Address other) const;

	/* Using this method is frowned upon.
	 * Please, do _not_ use this method.
	 * It is there only for chunk-ipv4.
	 */
	uint32_t get_host_order (void) const;
	void set_host_order (uint32_t ip);
	void serialize (uint8_t buf[4]) const;

	void print (std::ostream *os) const;

	bool is_broadcast (void);
	bool is_multicast (void);

	static Ipv4Address get_zero (void);
	static Ipv4Address get_any (void);
	static Ipv4Address get_broadcast (void);
	static Ipv4Address get_loopback (void);
private:
	uint32_t m_address;
};


class Ipv4Mask {
public:
	Ipv4Mask ();
	Ipv4Mask (uint32_t mask);
	Ipv4Mask (char const *mask);

	bool is_match (Ipv4Address a, Ipv4Address b) const;

	bool is_equal (Ipv4Mask other) const;


	/* Using this method is frowned upon.
	 * Please, do _not_ use this method.
	 */
	uint32_t get_host_order (void) const;
	void set_host_order (uint32_t value);

	void print (std::ostream *os) const;

	static Ipv4Mask get_loopback (void);
	static Ipv4Mask get_zero (void);
private:
	uint32_t m_mask;
};

std::ostream& operator<< (std::ostream& os, Ipv4Address const& address);
std::ostream& operator<< (std::ostream& os, Ipv4Mask const& mask);

bool operator == (Ipv4Address const &a, Ipv4Address const &b);
bool operator != (Ipv4Address const &a, Ipv4Address const &b);
class Ipv4AddressHash : public std::unary_function<Ipv4Address, size_t> {
public:
	size_t operator()(Ipv4Address const &x) const;
};
bool operator != (Ipv4Address const &a, Ipv4Address const &b);

}; // namespace yans

#endif /* IPV4_ADDRESS_H */
