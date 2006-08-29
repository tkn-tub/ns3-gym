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

#include "chunk-constant-data.h"

namespace ns3 {

ChunkConstantData::ChunkConstantData (uint32_t len, uint8_t data)
	: m_len (len), m_data (data)
{}

ChunkConstantData::~ChunkConstantData ()
{}


void 
ChunkConstantData::print (std::ostream *os) const
{
	*os << "(constant data)"
	    << " len=" << m_len
	    << ", data=" << m_data;
}

void 
ChunkConstantData::add_to (Buffer *buffer) const
{
	buffer->add_at_start (m_len);
#ifndef NDEBUG
	buffer->begin ().write_u8 (m_data, m_len);
#endif
}
void 
ChunkConstantData::peek_from (Buffer const *buffer)
{
	m_len = buffer->get_size ();
	m_data = buffer->begin ().read_u8 ();
}
void 
ChunkConstantData::remove_from (Buffer *buffer)
{
	buffer->remove_at_start (m_len);
}


}; // namespace ns3
