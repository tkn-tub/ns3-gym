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

#ifndef CHUNK_LLC_SNAP_H
#define CHUNK_LLC_SNAP_H

#include "chunk.h"
#include <stdint.h>

namespace yans {

class ChunkLlcSnap : public Chunk {
 public:
	ChunkLlcSnap ();
	virtual ~ChunkLlcSnap ();

	enum Type {
		TYPE_IPV4 = 0x0800,
		TYPE_ARP  = 0x0806
	};

	void set_type (enum Type type);
	enum Type get_type (void);

	uint32_t get_size (void) const;

private:
	virtual void print (std::ostream *os) const;
	virtual void add_to (Buffer *buffer) const;
	virtual void peek_from (Buffer const *buffer);
	virtual void remove_from (Buffer *buffer);
	uint16_t m_ether_type;
};

}; // namespace yans

#endif /* CHUNK_LLC_SNAP_H */
