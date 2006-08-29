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
#include "llc-snap-encapsulation.h"
#include "chunk-llc-snap.h"
#include "packet.h"
#include "chunk.h"
#include "mac-network-interface.h"
#include <cassert>

namespace ns3 {

uint32_t
LlcSnapEncapsulation::get_overhead (void) const
{
	ChunkLlcSnap llc;
	return llc.get_size ();
}
void 
LlcSnapEncapsulation::set_ipv4_callback (RxCallback callback)
{
	m_ipv4_callback = callback;
}
void 
LlcSnapEncapsulation::set_arp_callback (RxCallback callback)
{
	m_arp_callback = callback;
}
void 
LlcSnapEncapsulation::set_mac_interface (MacNetworkInterface *interface)
{
	m_interface = interface;
}
void 
LlcSnapEncapsulation::send_ipv4 (Packet packet, MacAddress to)
{
	ChunkLlcSnap llc;
	llc.set_type (ChunkLlcSnap::TYPE_IPV4);
	packet.add (&llc);
	m_interface->send (packet, to);
}
void 
LlcSnapEncapsulation::send_arp (Packet packet, MacAddress to)
{
	ChunkLlcSnap llc;
	llc.set_type (ChunkLlcSnap::TYPE_ARP);
	packet.add (&llc);
	m_interface->send (packet, to);
}

void
LlcSnapEncapsulation::receive (Packet packet, MacNetworkInterface *interface)
{
	assert (interface == m_interface);
	ChunkLlcSnap llc;
	packet.peek (&llc);
	packet.remove (&llc);
	switch (llc.get_type ()) {
	case ChunkLlcSnap::TYPE_IPV4:
		m_ipv4_callback (packet);
		break;
	case ChunkLlcSnap::TYPE_ARP:
		m_arp_callback (packet);
		break;
	default:
		assert (false);
		//NOT REACHED
		break;
	}
}

}; // namespace ns3
