/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:f -*- */
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

/**
 * \brief network packets
 *
 * Each network packet contains a byte buffer and a list of tags.
 * The byte buffer stores the serialized content of the chunks added 
 * to a packet. The serialized representation of these chunks is expected
 * to match that of real network packets bit for bit (although nothing
 * forces you to do this) which means that the content of a packet buffer
 * is expected to be that of a real packet.
 */
class Packet {
public:
    typedef Callback<void,uint8_t *,uint32_t> PacketReadWriteCallback;
	/**
	 * Create an empty packet.
	 */
    Packet ();
	/**
	 * Create a packet with a zero-filled payload.
	 * The memory necessary for the payload is not allocated:
	 * it will be allocated at any later point if you attempt
	 * to fragment this packet or to access the zero-filled
	 * bytes.
	 * 
	 * \param size the size of the zero-filled payload
	 */
    Packet (uint32_t size);
	/**
	 * Create a new packet which contains a fragment of the original
	 * packet.
	 *
	 * \param start offset from start of packet to start of fragment to create
	 * \param length length of fragment to create
	 * \returns a fragment of the original packet
	 */
    Packet createFragment (uint32_t start, uint32_t length) const;
	/**
	 * \returns the size in bytes of the packet (including the zero-filled
	 *          initial payload)
	 */
    uint32_t getSize (void) const;
	/**
	 * Add chunk to this packet. This method invokes the
	 * ns3::Chunk::addTo method to request the chunk to serialize
	 * itself in the packet buffer.
	 *
	 * \param chunk a pointer to the chunk to add to this packet.
	 */
    void add (Chunk *chunk);
	/**
	 * Deserialize chunk from this packet. This method invokes the
	 * ns3::Chunk::peekFrom method to request the chunk to deserialize
	 * itself from the packet buffer. This method does not remove
	 * the chunk from the buffer.
	 *
	 * \param chunk a pointer to the chunk to deserialize from the buffer
	 */
    void peek (Chunk *chunk) const;
	/**
	 * Remove a deserialized chunk from the internal buffer.
	 * This method invokes ns3::Chunk::removeFrom to complete
	 * the work initiated by Packet::peek and ns3::Chunk::peekFrom.
	 *
	 * \param chunk a pointer to the chunk to remove from the internal buffer.
	 */
    void remove (Chunk *chunk);
	/**
	 * Attach a tag to this packet. The tag is fully copied
	 * in a packet-specific internal buffer. This operation 
	 * is expected to be really fast.
	 *
	 * \param tag a pointer to the tag to attach to this packet.
	 */
    template <typename T>
    void addTag (T const *tag);
	/**
	 * Remove a tag from this packet. The data stored internally
	 * for this tag is copied in the input tag if an instance
	 * of this tag type is present in the internal buffer. If this
	 * tag type is not present, the input tag is not modified. 
	 *
	 * This operation can be potentially slow and might trigger
	 * unexpectedly large memory allocations. It is thus
	 * usually a better idea to create a copy of this packet,
	 * and invoke removeAllTags on the copy to remove all 
	 * tags rather than remove the tags one by one from a packet.
	 *
	 * \param tag a pointer to the tag to remove from this packet
	 * \returns true if an instance of this tag type is stored
	 *          in this packet, false otherwise.
	 */
    template <typename T>
    bool removeTag (T *tag);
	/**
	 * Copy a tag stored internally to the input tag. If no instance
	 * of this tag is present internally, the input tag is not modified.
	 *
	 * \param tag a pointer to the tag to read from this packet
	 * \returns true if an instance of this tag type is stored
	 *          in this packet, false otherwise.
	 */
    template <typename T>
    bool peekTag (T *tag) const;
	/**
	 * Remove all the tags stored in this packet. This operation is
	 * much much faster than invoking removeTag n times.
	 */
    void removeAllTags (void);
    void write (PacketReadWriteCallback callback) const;
	/**
	 * Concatenate the input packet at the end of the current
	 * packet.
	 *
	 * \param packet packet to concatenate
	 */
    void addAtEnd (Packet packet);
	/**
	 * Concatenate the fragment of the input packet identified
	 * by the offset and size parameters at the end of the current
	 * packet.
	 *
	 * \param packet to concatenate
	 * \param offset offset of fragment to copy from the start of the input packet
	 * \param size size of fragment of input packet to copy.
	 */
    void addAtEnd (Packet packet, uint32_t offset, uint32_t size);
	/** 
	 * Remove size bytes from the end of the current packet
	 * It is safe to remove more bytes that what is present in
	 * the packet.
	 *
	 * \param size number of bytes from remove
	 */
    void removeAtEnd (uint32_t size);
	/** 
	 * Remove size bytes from the start of the current packet.
	 * It is safe to remove more bytes that what is present in
	 * the packet.
	 *
	 * \param size number of bytes from remove
	 */
    void removeAtStart (uint32_t size);

private:
    Packet (Buffer buffer, Tags tags);
    Buffer m_buffer;
    Tags m_tags;
};

}; // namespace ns3


/**************************************************
    Start of implementation of templates defined
    above
 *************************************************/

namespace ns3 {

template <typename T>
void Packet::addTag (T const*tag)
{
    m_tags.add (tag);
}
template <typename T>
bool Packet::removeTag (T *tag)
{
    return m_tags.remove (tag);
}
template <typename T>
bool Packet::peekTag (T *tag) const
{
    return m_tags.peek (tag);
}
}; // namespace ns3

#endif /* PACKET_H */
