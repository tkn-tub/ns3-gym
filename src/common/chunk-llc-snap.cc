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

#include "chunk-llc-snap.h"
#include <cassert>

#define noTRACE_CHUNK_LLC_SNAP 1

#ifdef TRACE_CHUNK_LLC_SNAP
#include <iostream>
#include "simulator.h"
# define TRACE(x) \
std::cout << "CHUNK LLCSNAP TRACE " << Simulator::now_s () << " " << x << std::endl;
#else /* TRACE_CHUNK_LLC_SNAP */
# define TRACE(format,...)
#endif /* TRACE_CHUNK_LLC_SNAP */


namespace yans {

ChunkLlcSnap::ChunkLlcSnap ()
{}

ChunkLlcSnap::~ChunkLlcSnap ()
{}
void 
ChunkLlcSnap::set_type (enum Type type)
{
	m_ether_type = type;
}
enum ChunkLlcSnap::Type
ChunkLlcSnap::get_type (void)
{
	return (enum ChunkLlcSnap::Type) m_ether_type;
}

uint32_t 
ChunkLlcSnap::get_size (void) const
{
	return 1 + 1 + 1 + 3 + 2;
}
void 
ChunkLlcSnap::print (std::ostream *os) const
{
	*os << "(mac)"
	    << " EtherType: ";
	os->setf (std::ios::hex, std::ios::basefield);
	*os << m_ether_type;
	os->setf (std::ios::dec, std::ios::basefield);
}

void 
ChunkLlcSnap::add_to (Buffer *buffer) const
{
	buffer->add_at_start (get_size ());
	Buffer::Iterator i = buffer->begin ();
	uint8_t buf[] = {0xaa, 0xaa, 0x03, 0, 0, 0};
	i.write (buf, 6);
	i.write_hton_u16 (m_ether_type);
}
void 
ChunkLlcSnap::peek_from (Buffer const *buffer)
{
	Buffer::Iterator i = buffer->begin ();
	i.next (5+1);
	m_ether_type = i.read_ntoh_u16 ();
}
void 
ChunkLlcSnap::remove_from (Buffer *buffer)
{
	buffer->remove_at_start (get_size ());
}



}; // namespace yans
