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
  Buffer src = packet.m_buffer.CreateFullCopy ();
  Buffer dst = m_buffer.CreateFullCopy ();

  dst.AddAtEnd (src.GetSize ());
  Buffer::Iterator destStart = dst.End ();
  destStart.Prev (src.GetSize ());
  destStart.Write (src.Begin (), src.End ());
  m_buffer = dst;
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
  m_metadata.Print (os, m_buffer, PacketPrinter ());
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

Buffer 
Packet::Serialize (void) const
{
  Buffer buffer;
  uint32_t reserve;

  // write metadata
  reserve = m_metadata.GetSerializedSize ();
  buffer.AddAtStart (reserve);
  m_metadata.Serialize (buffer.Begin (), reserve);

  // write tags
  reserve = m_tags.GetSerializedSize ();
  buffer.AddAtStart (reserve);
  m_tags.Serialize (buffer.Begin (), reserve);
  
  // aggregate byte buffer, metadata, and tags
  Buffer tmp = m_buffer.CreateFullCopy ();
  buffer.AddAtStart (tmp.GetSize ());
  buffer.Begin ().Write (tmp.Begin (), tmp.End ());
  
  // write byte buffer size.
  buffer.AddAtStart (4);
  buffer.Begin ().WriteU32 (m_buffer.GetSize ());

  return buffer;
}
void 
Packet::Deserialize (Buffer buffer)
{
  Buffer buf = buffer;
  // read size
  uint32_t packetSize = buf.Begin ().ReadU32 ();
  buf.RemoveAtStart (4);

  // read buffer.
  buf.RemoveAtEnd (buf.GetSize () - packetSize);
  m_buffer = buf;

  // read tags
  buffer.RemoveAtStart (4 + packetSize);
  uint32_t tagsDeserialized = m_tags.Deserialize (buffer.Begin ());
  buffer.RemoveAtStart (tagsDeserialized);

  // read metadata
  uint32_t metadataDeserialized = 
    m_metadata.Deserialize (buffer.Begin ());
  buffer.RemoveAtStart (metadataDeserialized);
}


} // namespace ns3



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
