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

class Packet {
public:
    typedef Callback<void,uint8_t *,uint32_t> PacketReadWriteCallback;
    Packet ();
    Packet (uint32_t size);
    Packet createFragment (uint32_t start, uint32_t length) const;
    uint32_t getSize (void) const;
    void add (Chunk *chunk);
    void peek (Chunk *chunk) const;
    void remove (Chunk *chunk);
    template <typename T>
    void addTag (T const *tag);
    template <typename T>
    bool removeTag (T *tag);
    template <typename T>
    bool peekTag (T *tag) const;
    template <typename T>
    bool updateTag (T const*tag);
    void removeAllTags (void);
    void write (PacketReadWriteCallback callback) const;
    void addAtEnd (Packet packet);
    void addAtEnd (Packet packet, uint32_t offset, uint32_t size);
    void removeAtEnd (uint32_t size);
    void removeAtStart (uint32_t size);

private:
    Packet (Buffer buffer, Tags tags);
    Buffer m_buffer;
    Tags m_tags;
};

}; // namespace ns3

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
template <typename T>
bool Packet::updateTag (T const*tag)
{
    return m_tags.update (tag);
}

}; // namespace ns3

#endif /* PACKET_H */
