/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
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

uint32_t Packet::m_global_uid = 0;

Packet::Packet ()
    : m_buffer (),
      m_uid (m_global_uid)
{
    m_global_uid++;
}

Packet::Packet (uint32_t size)
    : m_buffer (size),
      m_uid (m_global_uid)
{
    m_global_uid++;
}
Packet::Packet (Buffer buffer, Tags tags, uint32_t uid)
    : m_buffer (buffer),
      m_tags (tags),
      m_uid (uid)
{}

Packet 
Packet::CreateFragment (uint32_t start, uint32_t length) const
{
    Buffer tmp = m_buffer.CreateFragment (start, length);
    return Packet (tmp, m_tags, m_uid);
}

uint32_t 
Packet::GetSize (void) const
{
    return m_buffer.GetSize ();
}

void 
Packet::Add (Header const &header)
{
    m_buffer.AddAtStart (header.GetSize ());
    header.Serialize (m_buffer.Begin ());
}
void 
Packet::Peek (Header &header)
{
    header.Deserialize (m_buffer.Begin ());
}
void 
Packet::Remove (Header const &header)
{
    assert (header.IsDeserialized ());
    m_buffer.RemoveAtStart (header.GetSize ());
}
void 
Packet::Add (Trailer const &trailer)
{
    m_buffer.AddAtEnd (trailer.GetSize ());
    Buffer::Iterator start = m_buffer.End ();
    start.Prev (trailer.GetSize ());
    trailer.Serialize (start);
}
void 
Packet::Peek (Trailer &trailer)
{
    Buffer::Iterator start = m_buffer.End ();
    start.Prev (trailer.GetSize ());
    trailer.Deserialize (start);
}
void 
Packet::Remove (Trailer const &trailer)
{
    assert (trailer.IsDeserialized ());
    m_buffer.RemoveAtEnd (trailer.GetSize ());
}


void 
Packet::AddAtEnd (Packet packet)
{
    Buffer src = packet.m_buffer;
    m_buffer.AddAtEnd (src.GetSize ());
    Buffer::Iterator destStart = m_buffer.End ();
    destStart.Prev (src.GetSize ());
    destStart.Write (src.Begin (), src.End ());
    /**
     * XXX: we might need to merge the tag list of the
     * other packet into the current packet.
     */
}
void 
Packet::AddAtEnd (Packet packet, uint32_t start, uint32_t size)
{
    assert (packet.GetSize () <= start + size);
    Buffer src = packet.m_buffer;
    m_buffer.AddAtEnd (src.GetSize ());
    Buffer::Iterator destStart = m_buffer.End ();
    destStart.Prev (size);
    Buffer::Iterator srcStart = src.Begin ();
    srcStart.Next (start);
    Buffer::Iterator srcEnd = srcStart;
    srcEnd.Next (size);
    destStart.Write (srcStart, srcEnd);
    /**
     * XXX: we might need to merge the tag list of the
     * other packet into the current packet.
     */
}
void 
Packet::RemoveAtEnd (uint32_t size)
{
    m_buffer.RemoveAtEnd (size);
}
void 
Packet::RemoveAtStart (uint32_t size)
{
    m_buffer.RemoveAtStart (size);
}

void 
Packet::RemoveAllTags (void)
{
    m_tags.RemoveAll ();
}

uint8_t const *
Packet::PeekData (void) const
{
    return m_buffer.PeekData ();
}

uint32_t 
Packet::GetUid (void) const
{
    return m_uid;
}

}; // namespace ns3
