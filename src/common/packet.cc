/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "ns3/assert.h"

namespace ns3 {

uint32_t Packet::m_globalUid = 0;

Packet::Packet ()
  : m_buffer (),
    m_history (m_globalUid, 0),
    m_uid (m_globalUid)
{
  m_globalUid++;
}

Packet::Packet (uint32_t size)
  : m_buffer (size),
    m_history (m_globalUid, size),
    m_uid (m_globalUid)
{
  m_globalUid++;
}
Packet::Packet (uint8_t const*buffer, uint32_t size)
  : m_buffer (),
    m_history (m_globalUid, size),
    m_uid (m_globalUid)
{
  m_globalUid++;
  m_buffer.AddAtStart (size);
  Buffer::Iterator i = m_buffer.Begin ();
  i.Write (buffer, size);
}

Packet::Packet (Buffer buffer, Tags tags, PacketHistory history, uint32_t uid)
  : m_buffer (buffer),
    m_tags (tags),
    m_history (history),
    m_uid (uid)
{}

Packet 
Packet::CreateFragment (uint32_t start, uint32_t length) const
{
  Buffer buffer = m_buffer.CreateFragment (start, length);
  PacketHistory history = m_history.CreateFragment (start, length);
  return Packet (buffer, m_tags, history, m_uid);
}

uint32_t 
Packet::GetSize (void) const
{
  return m_buffer.GetSize ();
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
  m_history.AddAtEnd (packet.m_history);
}
void
Packet::AddPaddingAtEnd (uint32_t size)
{
  m_buffer.AddAtEnd (size);
  m_history.AddPaddingAtEnd (size);
}
void 
Packet::RemoveAtEnd (uint32_t size)
{
  m_buffer.RemoveAtEnd (size);
  m_history.RemoveAtEnd (size);
}
void 
Packet::RemoveAtStart (uint32_t size)
{
  m_buffer.RemoveAtStart (size);
  m_history.RemoveAtStart (size);
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

void 
Packet::Print (std::ostream &os) const
{
  m_history.PrintDefault (os, m_buffer);
}

}; // namespace ns3
