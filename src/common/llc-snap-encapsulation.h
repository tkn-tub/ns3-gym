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
#ifndef LLC_SNAP_ENCAPSULATION
#define LLC_SNAP_ENCAPSULATION

#include <stdint.h>
#include "ns3/callback.h"
#include "mac-address.h"
#include "packet.h"

namespace ns3 {

class MacNetworkInterface;

class LlcSnapEncapsulation {
public:	
	typedef Callback<void, Packet > RxCallback;

	uint32_t get_overhead (void) const;
	void set_ipv4_callback (RxCallback callback);
	void set_arp_callback (RxCallback callback);
	void set_mac_interface (MacNetworkInterface *interface);
	void send_ipv4 (Packet packet, MacAddress to);
	void send_arp (Packet packet, MacAddress to);
	void receive (Packet packet, MacNetworkInterface *interface);
private:
	RxCallback m_ipv4_callback;
	RxCallback m_arp_callback;
	MacNetworkInterface *m_interface;
};

}; // namespace ns3

#endif /* LLC_SNAP_ENCAPSULATION */
