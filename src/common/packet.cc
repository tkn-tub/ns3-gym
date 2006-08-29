/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#include "packet.h"
#include <cassert>

namespace ns3 {

Packet::Packet ()
	: m_buffer () {}

Packet::Packet (uint32_t size)
	: m_buffer (size)
{}
Packet::Packet (Buffer buffer, Tags tags)
	: m_buffer (buffer),
	  m_tags (tags)
{}


Packet 
Packet::create_fragment (uint32_t start, uint32_t length) const
{
	Buffer tmp = m_buffer.create_fragment (start, length);
	return Packet (tmp, m_tags);
}

uint32_t 
Packet::get_size (void) const
{
	return m_buffer.get_size ();
}

void 
Packet::add (Chunk *chunk)
{
	chunk->add (&m_buffer);
}

void 
Packet::peek (Chunk *chunk) const
{
	chunk->peek (&m_buffer);
}

void 
Packet::remove (Chunk *chunk)
{
	chunk->remove (&m_buffer);
}


void 
Packet::write (PacketReadWriteCallback callback) const
{
	uint8_t *data = m_buffer.peek_data ();
	uint32_t to_write = get_size ();
	callback (data, to_write);
}


void 
Packet::add_at_end (Packet packet)
{
	Buffer src = packet.m_buffer;
	m_buffer.add_at_end (src.get_size ());
	Buffer::Iterator dest_start = m_buffer.end ();
	dest_start.prev (src.get_size ());
	dest_start.write (src.begin (), src.end ());
	/**
	 * XXX: we might need to merge the tag list of the
	 * other packet into the current packet.
	 */
}
void 
Packet::add_at_end (Packet packet, uint32_t start, uint32_t size)
{
	assert (packet.get_size () <= start + size);
	Buffer src = packet.m_buffer;
	m_buffer.add_at_end (src.get_size ());
	Buffer::Iterator dest_start = m_buffer.end ();
	dest_start.prev (size);
	Buffer::Iterator src_start = src.begin ();
	src_start.next (start);
	Buffer::Iterator src_end = src_start;
	src_end.next (size);
	dest_start.write (src_start, src_end);
	/**
	 * XXX: we might need to merge the tag list of the
	 * other packet into the current packet.
	 */
}
void 
Packet::remove_at_end (uint32_t size)
{
	m_buffer.remove_at_end (size);
}
void 
Packet::remove_at_start (uint32_t size)
{
	m_buffer.remove_at_start (size);
}

void 
Packet::remove_all_tags (void)
{
	m_tags.remove_all ();
}

}; // namespace ns3
