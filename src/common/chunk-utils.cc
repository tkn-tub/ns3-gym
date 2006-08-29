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
#include "chunk-utils.h"

namespace ns3 {

void write_to (Buffer::Iterator &i, Ipv4Address ad)
{
	i.write_hton_u32 (ad.get_host_order ());
}
void write_to (Buffer::Iterator &i, MacAddress ad)
{
	uint8_t mac[6];
	ad.peek (mac);
	i.write (mac, 6);
}

void read_from (Buffer::Iterator &i, Ipv4Address &ad)
{
	ad.set_host_order (i.read_ntoh_u32 ());
}
void read_from (Buffer::Iterator &i, MacAddress &ad)
{
	uint8_t mac[6];
	i.read (mac, 6);
	ad.set (mac);
}



}; // namespace ns3
