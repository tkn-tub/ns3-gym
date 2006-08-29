/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2006 INRIA
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
#ifndef IPV4_NETWORK_INTERFACE
#define IPV4_NETWORK_INTERFACE

#include "ipv4-address.h"
#include "yans/callback.h"
#include "packet.h"

namespace yans {

class Packet;

class Ipv4NetworkInterface {
public:
	typedef Callback<void, Packet , Ipv4NetworkInterface *>  RxCallback;
	virtual ~Ipv4NetworkInterface () = 0;

	void set_address (Ipv4Address ad);
	void set_mask (Ipv4Mask mask);
	
	uint16_t get_mtu (void) const;
	Ipv4Mask get_mask (void) const;
	Ipv4Address get_address (void) const;
	Ipv4Address get_broadcast (void) const;

	void send (Packet packet, Ipv4Address to);
	void set_rx_callback (RxCallback callback);
protected:
	void forward_up (Packet packet);
private:
	virtual uint16_t real_get_mtu (void) const = 0;
	virtual void real_send (Packet packet, Ipv4Address to) = 0;
	RxCallback m_rx_callback;
	Ipv4Address m_address;
	Ipv4Mask m_mask;
};

}; // namespace yans

#endif /* IPV4_NETWORK_INTERFACE */
