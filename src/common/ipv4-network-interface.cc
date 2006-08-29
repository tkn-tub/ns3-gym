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
#include "ipv4-network-interface.h"

namespace ns3 {

Ipv4NetworkInterface::~Ipv4NetworkInterface ()
{}

void 
Ipv4NetworkInterface::set_address (Ipv4Address ad)
{
	m_address = ad;
}
void 
Ipv4NetworkInterface::set_mask (Ipv4Mask mask)
{
	m_mask = mask;
}
uint16_t
Ipv4NetworkInterface::get_mtu (void) const
{
	return real_get_mtu ();
}	
Ipv4Mask 
Ipv4NetworkInterface::get_mask (void) const
{
	return m_mask;
}
Ipv4Address 
Ipv4NetworkInterface::get_address (void) const
{
	return m_address;
}
Ipv4Address 
Ipv4NetworkInterface::get_broadcast (void) const
{
	uint32_t mask = m_mask.get_host_order ();
	uint32_t address = m_address.get_host_order ();
	Ipv4Address broadcast = Ipv4Address (address | (~mask));
	return broadcast;
}

void 
Ipv4NetworkInterface::send (Packet packet, Ipv4Address to)
{
	real_send (packet, to);
}

void 
Ipv4NetworkInterface::set_rx_callback (RxCallback callback)
{
	m_rx_callback = callback;
}

void 
Ipv4NetworkInterface::forward_up (Packet packet)
{
	m_rx_callback (packet, this);
}



}; // namespace ns3
