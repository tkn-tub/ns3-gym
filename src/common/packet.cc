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
#include "packet-printer.h"
#include "ns3/assert.h"

namespace ns3 {

uint32_t Packet::m_globalUid = 0;

Packet::Packet ()
  : m_buffer (),
    m_metadata (m_globalUid, 0)
{
  m_globalUid++;
}

Packet::Packet (uint32_t size)
  : m_buffer (size),
    m_metadata (m_globalUid, size)
{
  m_globalUid++;
}
Packet::Packet (uint8_t const*buffer, uint32_t size)
  : m_buffer (),
    m_metadata (m_globalUid, size)
{
  m_globalUid++;
  m_buffer.AddAtStart (size);
  Buffer::Iterator i = m_buffer.Begin ();
  i.Write (buffer, size);
}

Packet::Packet (Buffer buffer, Tags tags, PacketMetadata metadata)
  : m_buffer (buffer),
    m_tags (tags),
    m_metadata (metadata)
{}

Packet 
Packet::CreateFragment (uint32_t start, uint32_t length) const
{
  Buffer buffer = m_buffer.CreateFragment (start, length);
  NS_ASSERT (m_buffer.GetSize () >= start + length);
  uint32_t end = m_buffer.GetSize () - (start + length);
  PacketMetadata metadata = m_metadata.CreateFragment (start, end);
  return Packet (buffer, m_tags, metadata);
}

uint32_t 
Packet::GetSize (void) const
{
  return m_buffer.GetSize ();
}

void 
Packet::AddAtEnd (Packet packet)
{
  packet.m_buffer.TransformIntoRealBuffer ();
  m_buffer.TransformIntoRealBuffer ();

  Buffer src = packet.m_buffer;
  m_buffer.AddAtEnd (src.GetSize ());
  Buffer::Iterator destStart = m_buffer.End ();
  destStart.Prev (src.GetSize ());
  destStart.Write (src.Begin (), src.End ());
  /**
   * XXX: we might need to merge the tag list of the
   * other packet into the current packet.
   */
  m_metadata.AddAtEnd (packet.m_metadata);
}
void
Packet::AddPaddingAtEnd (uint32_t size)
{
  m_buffer.AddAtEnd (size);
  m_metadata.AddPaddingAtEnd (size);
}
void 
Packet::RemoveAtEnd (uint32_t size)
{
  m_buffer.RemoveAtEnd (size);
  m_metadata.RemoveAtEnd (size);
}
void 
Packet::RemoveAtStart (uint32_t size)
{
  m_buffer.RemoveAtStart (size);
  m_metadata.RemoveAtStart (size);
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
  return m_metadata.GetUid ();
}

void 
Packet::Print (std::ostream &os) const
{
  m_metadata.PrintDefault (os, m_buffer);
}

void 
Packet::Print (std::ostream &os, const PacketPrinter &printer) const
{
  m_metadata.Print (os, m_buffer, printer);
}

void
Packet::EnableMetadata (void)
{
  PacketMetadata::Enable ();
}

}; // namespace ns3



#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include <string>

namespace ns3 {

class PacketTest: public Test {
public:
  virtual bool RunTests (void);
  PacketTest ();
};


PacketTest::PacketTest ()
  : Test ("Packet") {}


bool
PacketTest::RunTests (void)
{
  bool ok = true;

  Packet pkt1 (reinterpret_cast<const uint8_t*> ("hello"), 5);
  Packet pkt2 (reinterpret_cast<const uint8_t*> (" world"), 6);
  Packet packet;
  packet.AddAtEnd (pkt1);
  packet.AddAtEnd (pkt2);
  
  if (packet.GetSize () != 11)
    {
      Failure () << "expected size 11, got " << packet.GetSize () << std::endl;
      ok = false;
    }

  std::string msg = std::string (reinterpret_cast<const char *>(packet.PeekData ()),
                                 packet.GetSize ());
  if (msg != "hello world")
    {
      Failure () << "expected size 'hello world', got " << msg << std::endl;
      ok = false;
    }

  return ok;
}


static PacketTest gPacketTest;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */
