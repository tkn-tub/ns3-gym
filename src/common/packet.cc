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

void 
Packet::Ref (void) const
{
  m_refCount++;
}
void 
Packet::Unref (void) const
{
  m_refCount--;
  if (m_refCount == 0)
    {
      delete this;
    }
}

Ptr<Packet> 
Packet::Copy (void) const
{
  // we need to invoke the copy constructor directly
  // rather than calling Create because the copy constructor
  // is private.
  return Ptr<Packet> (new Packet (*this), false);
}

Packet::Packet ()
  : m_buffer (),
    m_tags (),
    m_metadata (m_globalUid, 0),
    m_refCount (1)
{
  m_globalUid++;
}

Packet::Packet (const Packet &o)
  : m_buffer (o.m_buffer),
    m_tags (o.m_tags),
    m_metadata (o.m_metadata),
    m_refCount (1)
{}

Packet &
Packet::operator = (const Packet &o)
{
  if (this == &o)
    {
      return *this;
    }
  m_buffer = o.m_buffer;
  m_tags = o.m_tags;
  m_metadata = o.m_metadata;
  return *this;
}

Packet::Packet (uint32_t size)
  : m_buffer (size),
    m_tags (),
    m_metadata (m_globalUid, size),
    m_refCount (1)
{
  m_globalUid++;
}
Packet::Packet (uint8_t const*buffer, uint32_t size)
  : m_buffer (),
    m_tags (),
    m_metadata (m_globalUid, size),
    m_refCount (1)
{
  m_globalUid++;
  m_buffer.AddAtStart (size);
  Buffer::Iterator i = m_buffer.Begin ();
  i.Write (buffer, size);
}

Packet::Packet (Buffer buffer, Tags tags, PacketMetadata metadata)
  : m_buffer (buffer),
    m_tags (tags),
    m_metadata (metadata),
    m_refCount (1)
{}

Ptr<Packet>
Packet::CreateFragment (uint32_t start, uint32_t length) const
{
  Buffer buffer = m_buffer.CreateFragment (start, length);
  NS_ASSERT (m_buffer.GetSize () >= start + length);
  uint32_t end = m_buffer.GetSize () - (start + length);
  PacketMetadata metadata = m_metadata.CreateFragment (start, end);
  // again, call the constructor directly rather than
  // through Create because it is private.
  return Ptr<Packet> (new Packet (buffer, m_tags, metadata), false);
}

uint32_t 
Packet::GetSize (void) const
{
  return m_buffer.GetSize ();
}

void
Packet::AddHeader (const Header &header)
{
  uint32_t size = header.GetSerializedSize ();
  m_buffer.AddAtStart (size);
  header.Serialize (m_buffer.Begin ());
  m_metadata.AddHeader (header, size);
}
uint32_t
Packet::RemoveHeader (Header &header)
{
  uint32_t deserialized = header.Deserialize (m_buffer.Begin ());
  m_buffer.RemoveAtStart (deserialized);
  m_metadata.RemoveHeader (header, deserialized);
  return deserialized;
}
void
Packet::AddTrailer (const Trailer &trailer)
{
  uint32_t size = trailer.GetSerializedSize ();
  m_buffer.AddAtEnd (size);
  Buffer::Iterator end = m_buffer.End ();
  trailer.Serialize (end);
  m_metadata.AddTrailer (trailer, size);
}
uint32_t
Packet::RemoveTrailer (Trailer &trailer)
{
  uint32_t deserialized = trailer.Deserialize (m_buffer.End ());
  m_buffer.RemoveAtEnd (deserialized);
  m_metadata.RemoveTrailer (trailer, deserialized);
  return deserialized;
}

void 
Packet::AddAtEnd (Ptr<const Packet> packet)
{
  Buffer src = packet->m_buffer.CreateFullCopy ();
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
  m_metadata.AddAtEnd (packet->m_metadata);
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
Packet::PrintTags (std::ostream &os) const
{
  m_tags.Print (os, " ");
}

void 
Packet::Print (std::ostream &os) const
{
  PacketMetadata::ItemIterator i = m_metadata.BeginItem (m_buffer);
  while (i.HasNext ())
    {
      PacketMetadata::Item item = i.Next ();
      if (item.isFragment)
        {
          switch (item.type) {
          case PacketMetadata::Item::PAYLOAD:
            os << "Payload";
            break;
          case PacketMetadata::Item::HEADER:
          case PacketMetadata::Item::TRAILER:
            os << item.tid.GetName ();
            break;
          }
          os << " Fragment [" << item.currentTrimedFromStart<<":"
             << (item.currentTrimedFromStart + item.currentSize) << "]";
        }
      else
        {
          switch (item.type) {
          case PacketMetadata::Item::PAYLOAD:
            os << "Payload (size=" << item.currentSize << ")";
            break;
          case PacketMetadata::Item::HEADER:
          case PacketMetadata::Item::TRAILER:
            os << item.tid.GetName () << " (";
            {
              NS_ASSERT (item.tid.HasConstructor ());
              Callback<ObjectBase *> constructor = item.tid.GetConstructor ();
              NS_ASSERT (!constructor.IsNull ());
              ObjectBase *instance = constructor ();
              NS_ASSERT (instance != 0);
              Chunk *chunk = dynamic_cast<Chunk *> (instance);
              NS_ASSERT (chunk != 0);
              chunk->Deserialize (item.current);
              chunk->Print (os);
            }
            os << ")";
            break;
          }          
        }
      if (i.HasNext ())
        {
          os << " ";
        }
    }
#if 0
  // The code below will work only if headers and trailers
  // define the right attributes which is not the case for
  // now. So, as a temporary measure, we use the 
  // headers' and trailers' Print method as shown above.
  PacketMetadata::ItemIterator i = m_metadata.BeginItem (m_buffer);
  while (i.HasNext ())
    {
      PacketMetadata::Item item = i.Next ();
      if (item.isFragment)
        {
          switch (item.type) {
          case PacketMetadata::Item::PAYLOAD:
            os << "Payload";
            break;
          case PacketMetadata::Item::HEADER:
          case PacketMetadata::Item::TRAILER:
            os << item.tid.GetName ();
            break;
          }
          os << " Fragment [" << item.currentTrimedFromStart<<":"
             << (item.currentTrimedFromStart + item.currentSize) << "]";
        }
      else
        {
          switch (item.type) {
          case PacketMetadata::Item::PAYLOAD:
            os << "Payload (size=" << item.currentSize << ")";
            break;
          case PacketMetadata::Item::HEADER:
          case PacketMetadata::Item::TRAILER:
            os << item.tid.GetName () << "(";
            {
              NS_ASSERT (item.tid.HasConstructor ());
              Callback<ObjectBase *> constructor = item.tid.GetConstructor ();
              NS_ASSERT (constructor.IsNull ());
              ObjectBase *instance = constructor ();
              NS_ASSERT (instance != 0);
              Chunk *chunk = dynamic_cast<Chunk *> (instance);
              NS_ASSERT (chunk != 0);
              chunk->Deserialize (item.current);
              for (uint32_t j = 0; j < item.tid.GetAttributeListN (); j++)
                {
                  std::string attrName = item.tid.GetAttributeName (j);
                  std::string value;
                  bool ok = chunk->GetAttribute (attrName, value);
                  NS_ASSERT (ok);
                  os << attrName << "=" << value;
                  if ((j + 1) < item.tid.GetAttributeListN ())
                    {
                      os << ",";
                    }
                }
            }
            os << ")";
            break;
          }          
        }
      if (i.HasNext ())
        {
          os << " ";
        }
    }
#endif   
}

PacketMetadata::ItemIterator 
Packet::BeginItem (void) const
{
  return m_metadata.BeginItem (m_buffer);
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

std::ostream& operator<< (std::ostream& os, const Packet &packet)
{
  packet.Print (os);
  return os;
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

  Ptr<Packet> pkt1 = Create<Packet> (reinterpret_cast<const uint8_t*> ("hello"), 5);
  Ptr<Packet> pkt2 = Create<Packet> (reinterpret_cast<const uint8_t*> (" world"), 6);
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddAtEnd (pkt1);
  packet->AddAtEnd (pkt2);
  
  if (packet->GetSize () != 11)
    {
      Failure () << "expected size 11, got " << packet->GetSize () << std::endl;
      ok = false;
    }

  std::string msg = std::string (reinterpret_cast<const char *>(packet->PeekData ()),
                                 packet->GetSize ());
  if (msg != "hello world")
    {
      Failure () << "expected 'hello world', got '" << msg << "'" << std::endl;
      ok = false;
    }

  return ok;
}


static PacketTest gPacketTest;

}; // namespace ns3

#endif /* RUN_SELF_TESTS */
