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
#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include "buffer.h"
#include "chunk.h"
#include "tags.h"
#include "ns3/callback.h"

namespace ns3 {

class Packet {
public:
	typedef Callback<void,uint8_t *,uint32_t> PacketReadWriteCallback;
	Packet ();
	Packet (uint32_t size);
	Packet create_fragment (uint32_t start, uint32_t length) const;
	uint32_t get_size (void) const;
	void add (Chunk *chunk);
	void peek (Chunk *chunk) const;
	void remove (Chunk *chunk);
	template <typename T>
	void add_tag (T const *tag);
	template <typename T>
	bool remove_tag (T *tag);
	template <typename T>
	bool peek_tag (T *tag) const;
	template <typename T>
	bool update_tag (T const*tag);
	void remove_all_tags (void);
	void write (PacketReadWriteCallback callback) const;
	void add_at_end (Packet packet);
	void add_at_end (Packet packet, uint32_t offset, uint32_t size);
	void remove_at_end (uint32_t size);
	void remove_at_start (uint32_t size);

private:
	Packet (Buffer buffer, Tags tags);
	Buffer m_buffer;
	Tags m_tags;
};

}; // namespace ns3

namespace ns3 {

template <typename T>
void Packet::add_tag (T const*tag)
{
	m_tags.add (tag);
}
template <typename T>
bool Packet::remove_tag (T *tag)
{
	return m_tags.remove (tag);
}
template <typename T>
bool Packet::peek_tag (T *tag) const
{
	return m_tags.peek (tag);
}
template <typename T>
bool Packet::update_tag (T const*tag)
{
	return m_tags.update (tag);
}

}; // namespace ns3

#endif /* PACKET_H */
