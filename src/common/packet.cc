/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/test.h"
#include <string>
#include <stdarg.h>

NS_LOG_COMPONENT_DEFINE ("Packet");

namespace ns3 {

uint32_t Packet::m_globalUid = 0;

TypeId 
ByteTagIterator::Item::GetTypeId (void) const
{
  return m_tid;
}
uint32_t 
ByteTagIterator::Item::GetStart (void) const
{
  return m_start;
}
uint32_t 
ByteTagIterator::Item::GetEnd (void) const
{
  return m_end;
}
void 
ByteTagIterator::Item::GetTag (Tag &tag) const
{
  if (tag.GetInstanceTypeId () != GetTypeId ())
    {
      NS_FATAL_ERROR ("The tag you provided is not of the right type.");
    }
  tag.Deserialize (m_buffer);
}
ByteTagIterator::Item::Item (TypeId tid, uint32_t start, uint32_t end, TagBuffer buffer)
  : m_tid (tid),
    m_start (start),
    m_end (end),
    m_buffer (buffer)
{
}
bool
ByteTagIterator::HasNext (void) const
{
  return m_current.HasNext ();
}
ByteTagIterator::Item
ByteTagIterator::Next (void)
{
  ByteTagList::Iterator::Item i = m_current.Next ();
  return ByteTagIterator::Item (i.tid,
                                i.start-m_current.GetOffsetStart (),
                                i.end-m_current.GetOffsetStart (),
                                i.buf);
}
ByteTagIterator::ByteTagIterator (ByteTagList::Iterator i)
  : m_current (i)
{
}


PacketTagIterator::PacketTagIterator (const struct PacketTagList::TagData *head)
  : m_current (head)
{
}
bool
PacketTagIterator::HasNext (void) const
{
  return m_current != 0;
}
PacketTagIterator::Item
PacketTagIterator::Next (void)
{
  NS_ASSERT (HasNext ());
  const struct PacketTagList::TagData *prev = m_current;
  m_current = m_current->next;
  return PacketTagIterator::Item (prev);
}

PacketTagIterator::Item::Item (const struct PacketTagList::TagData *data)
  : m_data (data)
{
}
TypeId
PacketTagIterator::Item::GetTypeId (void) const
{
  return m_data->tid;
}
void
PacketTagIterator::Item::GetTag (Tag &tag) const
{
  NS_ASSERT (tag.GetInstanceTypeId () == m_data->tid);
  tag.Deserialize (TagBuffer ((uint8_t*)m_data->data, (uint8_t*)m_data->data+PACKET_TAG_MAX_SIZE));
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
    m_byteTagList (),
    m_packetTagList (),
    /* The upper 32 bits of the packet id in 
     * metadata is for the system id. For non-
     * distributed simulations, this is simply 
     * zero.  The lower 32 bits are for the 
     * global UID
     */
    m_metadata (static_cast<uint64_t> (Simulator::GetSystemId ()) << 32 | m_globalUid, 0),
    m_nixVector (0)
{
  m_globalUid++;
}

Packet::Packet (const Packet &o)
  : m_buffer (o.m_buffer),
    m_byteTagList (o.m_byteTagList),
    m_packetTagList (o.m_packetTagList),
    m_metadata (o.m_metadata)
{
  o.m_nixVector ? m_nixVector = o.m_nixVector->Copy ()
    : m_nixVector = 0;
}

Packet &
Packet::operator = (const Packet &o)
{
  if (this == &o)
    {
      return *this;
    }
  m_buffer = o.m_buffer;
  m_byteTagList = o.m_byteTagList;
  m_packetTagList = o.m_packetTagList;
  m_metadata = o.m_metadata;
  o.m_nixVector ? m_nixVector = o.m_nixVector->Copy () 
    : m_nixVector = 0;
  return *this;
}

Packet::Packet (uint32_t size)
  : m_buffer (size),
    m_byteTagList (),
    m_packetTagList (),
    /* The upper 32 bits of the packet id in 
     * metadata is for the system id. For non-
     * distributed simulations, this is simply 
     * zero.  The lower 32 bits are for the 
     * global UID
     */
    m_metadata (static_cast<uint64_t> (Simulator::GetSystemId ()) << 32 | m_globalUid, size),
    m_nixVector (0)
{
  m_globalUid++;
}
Packet::Packet (uint8_t const *buffer, uint32_t size, bool magic)
  : m_buffer (0, false),
    m_byteTagList (),
    m_packetTagList (),
    m_metadata (0,0),
    m_nixVector (0)
{
  NS_ASSERT (magic);
  Deserialize (buffer, size);
}

Packet::Packet (uint8_t const*buffer, uint32_t size)
  : m_buffer (),
    m_byteTagList (),
    m_packetTagList (),
    /* The upper 32 bits of the packet id in 
     * metadata is for the system id. For non-
     * distributed simulations, this is simply 
     * zero.  The lower 32 bits are for the 
     * global UID
     */
    m_metadata (static_cast<uint64_t> (Simulator::GetSystemId ()) << 32 | m_globalUid, size),
    m_nixVector (0)
{
  m_globalUid++;
  m_buffer.AddAtStart (size);
  Buffer::Iterator i = m_buffer.Begin ();
  i.Write (buffer, size);
}

Packet::Packet (const Buffer &buffer,  const ByteTagList &byteTagList, 
                const PacketTagList &packetTagList, const PacketMetadata &metadata)
  : m_buffer (buffer),
    m_byteTagList (byteTagList),
    m_packetTagList (packetTagList),
    m_metadata (metadata),
    m_nixVector (0)
{
}

Ptr<Packet>
Packet::CreateFragment (uint32_t start, uint32_t length) const
{
  NS_LOG_FUNCTION (this << start << length);
  Buffer buffer = m_buffer.CreateFragment (start, length);
  NS_ASSERT (m_buffer.GetSize () >= start + length);
  uint32_t end = m_buffer.GetSize () - (start + length);
  PacketMetadata metadata = m_metadata.CreateFragment (start, end);
  // again, call the constructor directly rather than
  // through Create because it is private.
  return Ptr<Packet> (new Packet (buffer, m_byteTagList, m_packetTagList, metadata), false);
}

void
Packet::SetNixVector (Ptr<NixVector> nixVector)
{
  m_nixVector = nixVector;
}

Ptr<NixVector>
Packet::GetNixVector (void) const
{
  return m_nixVector;
} 

void
Packet::AddHeader (const Header &header)
{
  uint32_t size = header.GetSerializedSize ();
  NS_LOG_FUNCTION (this << header.GetInstanceTypeId ().GetName () << size);
  uint32_t orgStart = m_buffer.GetCurrentStartOffset ();
  bool resized = m_buffer.AddAtStart (size);
  if (resized)
    {
      m_byteTagList.AddAtStart (m_buffer.GetCurrentStartOffset () + size - orgStart,
                                m_buffer.GetCurrentStartOffset () + size);
    }
  header.Serialize (m_buffer.Begin ());
  m_metadata.AddHeader (header, size);
}
uint32_t
Packet::RemoveHeader (Header &header)
{
  uint32_t deserialized = header.Deserialize (m_buffer.Begin ());
  NS_LOG_FUNCTION (this << header.GetInstanceTypeId ().GetName () << deserialized);
  m_buffer.RemoveAtStart (deserialized);
  m_metadata.RemoveHeader (header, deserialized);
  return deserialized;
}
uint32_t
Packet::PeekHeader (Header &header) const
{
  uint32_t deserialized = header.Deserialize (m_buffer.Begin ());
  NS_LOG_FUNCTION (this << header.GetInstanceTypeId ().GetName () << deserialized);
  return deserialized;
}
void
Packet::AddTrailer (const Trailer &trailer)
{
  uint32_t size = trailer.GetSerializedSize ();
  NS_LOG_FUNCTION (this << trailer.GetInstanceTypeId ().GetName () << size);
  uint32_t orgStart = m_buffer.GetCurrentStartOffset ();
  bool resized = m_buffer.AddAtEnd (size);
  if (resized)
    {
      m_byteTagList.AddAtEnd (m_buffer.GetCurrentStartOffset () - orgStart,
                              m_buffer.GetCurrentEndOffset () - size);
    }
  Buffer::Iterator end = m_buffer.End ();
  trailer.Serialize (end);
  m_metadata.AddTrailer (trailer, size);
}
uint32_t
Packet::RemoveTrailer (Trailer &trailer)
{
  uint32_t deserialized = trailer.Deserialize (m_buffer.End ());
  NS_LOG_FUNCTION (this << trailer.GetInstanceTypeId ().GetName () << deserialized);
  m_buffer.RemoveAtEnd (deserialized);
  m_metadata.RemoveTrailer (trailer, deserialized);
  return deserialized;
}
uint32_t
Packet::PeekTrailer (Trailer &trailer)
{
  uint32_t deserialized = trailer.Deserialize (m_buffer.End ());
  NS_LOG_FUNCTION (this << trailer.GetInstanceTypeId ().GetName () << deserialized);
  return deserialized;
}

void 
Packet::AddAtEnd (Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (this << packet << packet->GetSize ());
  uint32_t aStart = m_buffer.GetCurrentStartOffset ();
  uint32_t bEnd = packet->m_buffer.GetCurrentEndOffset ();
  m_buffer.AddAtEnd (packet->m_buffer);
  uint32_t appendPrependOffset = m_buffer.GetCurrentEndOffset () - packet->m_buffer.GetSize ();
  m_byteTagList.AddAtEnd (m_buffer.GetCurrentStartOffset () - aStart, 
                          appendPrependOffset);
  ByteTagList copy = packet->m_byteTagList;
  copy.AddAtStart (m_buffer.GetCurrentEndOffset () - bEnd,
                   appendPrependOffset);
  m_byteTagList.Add (copy);
  m_metadata.AddAtEnd (packet->m_metadata);
}
void
Packet::AddPaddingAtEnd (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  uint32_t orgEnd = m_buffer.GetCurrentEndOffset ();
  bool resized = m_buffer.AddAtEnd (size);
  if (resized)
    {
      m_byteTagList.AddAtEnd (m_buffer.GetCurrentEndOffset () - orgEnd,
                              m_buffer.GetCurrentEndOffset () - size);
    }
  m_metadata.AddPaddingAtEnd (size);
}
void 
Packet::RemoveAtEnd (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_buffer.RemoveAtEnd (size);
  m_metadata.RemoveAtEnd (size);
}
void 
Packet::RemoveAtStart (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  m_buffer.RemoveAtStart (size);
  m_metadata.RemoveAtStart (size);
}

void 
Packet::RemoveAllByteTags (void)
{
  NS_LOG_FUNCTION (this);
  m_byteTagList.RemoveAll ();
}

uint8_t const *
Packet::PeekData (void) const
{
  NS_LOG_FUNCTION (this);
  uint32_t oldStart = m_buffer.GetCurrentStartOffset ();
  uint8_t const * data = m_buffer.PeekData ();
  uint32_t newStart = m_buffer.GetCurrentStartOffset ();
 
  // Update tag offsets if buffer offsets were changed
  const_cast<ByteTagList &>(m_byteTagList).AddAtStart (newStart - oldStart, newStart);
  return data;
}

uint32_t 
Packet::CopyData (uint8_t *buffer, uint32_t size) const
{
  return m_buffer.CopyData (buffer, size);
}

void
Packet::CopyData(std::ostream *os, uint32_t size) const
{
  return m_buffer.CopyData (os, size);
}

uint64_t 
Packet::GetUid (void) const
{
  return m_metadata.GetUid ();
}

void 
Packet::PrintByteTags (std::ostream &os) const
{
  ByteTagIterator i = GetByteTagIterator ();
  while (i.HasNext ())
    {
      ByteTagIterator::Item item = i.Next ();
      os << item.GetTypeId ().GetName () << " [" << item.GetStart () << "-" << item.GetEnd () << "]";
      Callback<ObjectBase *> constructor = item.GetTypeId ().GetConstructor ();
      if (constructor.IsNull ())
        {
          if (i.HasNext ())
            {
              os << " ";
            }
          continue;
        }
      Tag *tag = dynamic_cast<Tag *> (constructor ());
      NS_ASSERT (tag != 0);
      os << " ";
      item.GetTag (*tag);
      tag->Print (os);
      if (i.HasNext ())
        {
          os << " ";
        }
      delete tag;
    }
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
                delete chunk;
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
                for (uint32_t j = 0; j < item.tid.GetAttributeN (); j++)
                  {
                    std::string attrName = item.tid.GetAttributeName (j);
                    std::string value;
                    bool ok = chunk->GetAttribute (attrName, value);
                    NS_ASSERT (ok);
                    os << attrName << "=" << value;
                    if ((j + 1) < item.tid.GetAttributeN ())
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
Packet::EnablePrinting (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  PacketMetadata::Enable ();
}

void
Packet::EnableChecking (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  PacketMetadata::EnableChecking ();
}

uint32_t Packet::GetSerializedSize (void) const
{
  uint32_t size = 0;

  if (m_nixVector)
    {
      // increment total size by the size of the nix-vector
      // ensuring 4-byte boundary
      size += ((m_nixVector->GetSerializedSize () + 3) & (~3));

      // add 4-bytes for entry of total length of nix-vector
      size += 4;
    }
  else
    {
      // if no nix-vector, still have to add 4-bytes
      // to account for the entry of total size for 
      // nix-vector in the buffer
      size += 4;
    }

  //Tag size
  //XXX
  //size += m_tags.GetSerializedSize ();

  // increment total size by size of meta-data 
  // ensuring 4-byte boundary
  size += ((m_metadata.GetSerializedSize () + 3) & (~3));

  // add 4-bytes for entry of total length of meta-data
  size += 4;

  // increment total size by size of buffer 
  // ensuring 4-byte boundary
  size += ((m_buffer.GetSerializedSize () + 3) & (~3));

  // add 4-bytes for entry of total length of buffer 
  size += 4;

  return size;
}

uint32_t 
Packet::Serialize (uint8_t* buffer, uint32_t maxSize) const
{
  uint32_t* p = reinterpret_cast<uint32_t *> (buffer);
  uint32_t size = 0;

  // if nix-vector exists, serialize it
  if (m_nixVector)
    {
      uint32_t nixSize = m_nixVector->GetSerializedSize ();
      if (size + nixSize <= maxSize)
        {
          // put the total length of nix-vector in the
          // buffer. this includes 4-bytes for total 
          // length itself
          *p++ = nixSize + 4;
          size += nixSize;

          // serialize the nix-vector
          uint32_t serialized = 
            m_nixVector->Serialize (p, nixSize);
          if (serialized)
            {
              // increment p by nixSize bytes
              // ensuring 4-byte boundary
              p += ((nixSize+3) & (~3)) / 4;
            }
          else
            {
              return 0;
            }
        }
      else 
        {
          return 0;
        }
    }
  else
    { 
      // no nix vector, set zero length, 
      // ie 4-bytes, since it must include 
      // length for itself
      if (size + 4 <= maxSize)
        {
          size += 4;
          *p++ = 4;
        }
      else
        {
          return 0;
        }
    }

  // Serialize Tags
  // XXX

  // Serialize Metadata
  uint32_t metaSize = m_metadata.GetSerializedSize ();
  if (size + metaSize <= maxSize)
    {
      // put the total length of metadata in the
      // buffer. this includes 4-bytes for total 
      // length itself
      *p++ = metaSize + 4;
      size += metaSize;

      // serialize the metadata
      uint32_t serialized = 
        m_metadata.Serialize (reinterpret_cast<uint8_t *> (p), metaSize); 
      if (serialized)
        {
          // increment p by metaSize bytes
          // ensuring 4-byte boundary
          p += ((metaSize+3) & (~3)) / 4;
        }
      else
        {
          return 0;
        }
    }
  else
    {
      return 0;
    }

  // Serialize the packet contents
  uint32_t bufSize = m_buffer.GetSerializedSize ();
  if (size + bufSize <= maxSize)
    {
      // put the total length of the buffer in the
      // buffer. this includes 4-bytes for total 
      // length itself
      *p++ = bufSize + 4;
      size += bufSize;

      // serialize the buffer
      uint32_t serialized = 
        m_buffer.Serialize (reinterpret_cast<uint8_t *> (p), bufSize);
      if (serialized)
        {
          // increment p by bufSize bytes
          // ensuring 4-byte boundary
          p += ((bufSize+3) & (~3)) / 4;
        }
      else 
        {
          return 0;
        }
    }
  else
    {
      return 0;
    }

  // Serialized successfully
  return 1;
}

uint32_t 
Packet::Deserialize (const uint8_t* buffer, uint32_t size)
{
  NS_LOG_FUNCTION (this);

  const uint32_t* p = reinterpret_cast<const uint32_t *> (buffer);

  // read nix-vector
  NS_ASSERT (!m_nixVector);
  uint32_t nixSize = *p++;
  size -= nixSize;

  // if size less than zero, the buffer 
  // will be overrun, assert
  NS_ASSERT (size >= 0);

  if (nixSize > 4)
    {
      Ptr<NixVector> nix = Create<NixVector> ();
      uint32_t nixDeserialized = nix->Deserialize (p, nixSize);
      if (!nixDeserialized)
        {
          // nix-vector not deserialized
          // completely
          return 0;
        }
      m_nixVector = nix;
      // increment p by nixSize ensuring
      // 4-byte boundary
      p += ((((nixSize - 4) + 3) & (~3)) / 4);
    }

  // read tags
  //XXX
  //uint32_t tagsDeserialized = m_tags.Deserialize (buffer.Begin ());
  //buffer.RemoveAtStart (tagsDeserialized);

  // read metadata
  uint32_t metaSize = *p++;
  size -= metaSize;

  // if size less than zero, the buffer 
  // will be overrun, assert
  NS_ASSERT (size >= 0);

  uint32_t metadataDeserialized = 
    m_metadata.Deserialize (reinterpret_cast<const uint8_t *> (p), metaSize);
  if (!metadataDeserialized)
    {
      // meta-data not deserialized 
      // completely
      return 0;
    }
  // increment p by metaSize ensuring 
  // 4-byte boundary
  p += ((((metaSize - 4) + 3) & (~3)) / 4);

  // read buffer contents
  uint32_t bufSize = *p++;
  size -= bufSize;

  // if size less than zero, the buffer 
  // will be overrun, assert
  NS_ASSERT (size >= 0);

  uint32_t bufferDeserialized =
    m_buffer.Deserialize (reinterpret_cast<const uint8_t *> (p), bufSize);
  if (!bufferDeserialized)
    {
      // buffer not deserialized 
      // completely
      return 0;
    }

  // return zero if did not deserialize the 
  // number of expected bytes
  return (size == 0);
}

void 
Packet::AddByteTag (const Tag &tag) const
{
  NS_LOG_FUNCTION (this << tag.GetInstanceTypeId ().GetName () << tag.GetSerializedSize ());
  ByteTagList *list = const_cast<ByteTagList *> (&m_byteTagList);
  TagBuffer buffer = list->Add (tag.GetInstanceTypeId (), tag.GetSerializedSize (), 
                                m_buffer.GetCurrentStartOffset (),
                                m_buffer.GetCurrentEndOffset ());
  tag.Serialize (buffer);
}
ByteTagIterator 
Packet::GetByteTagIterator (void) const
{
  return ByteTagIterator (m_byteTagList.Begin (m_buffer.GetCurrentStartOffset (), m_buffer.GetCurrentEndOffset ()));
}

bool 
Packet::FindFirstMatchingByteTag (Tag &tag) const
{
  TypeId tid = tag.GetInstanceTypeId ();
  ByteTagIterator i = GetByteTagIterator ();
  while (i.HasNext ())
    {
      ByteTagIterator::Item item = i.Next ();
      if (tid == item.GetTypeId ())
        {
          item.GetTag (tag);
          return true;
        }
    }
  return false;
}

void 
Packet::AddPacketTag (const Tag &tag) const
{
  NS_LOG_FUNCTION (this << tag.GetInstanceTypeId ().GetName () << tag.GetSerializedSize ());
  m_packetTagList.Add (tag);
}
bool 
Packet::RemovePacketTag (Tag &tag)
{
  NS_LOG_FUNCTION (this << tag.GetInstanceTypeId ().GetName () << tag.GetSerializedSize ());
  bool found = m_packetTagList.Remove (tag);
  return found;
}
bool 
Packet::PeekPacketTag (Tag &tag) const
{
  bool found = m_packetTagList.Peek (tag);
  return found;
}
void 
Packet::RemoveAllPacketTags (void)
{
  NS_LOG_FUNCTION (this);
  m_packetTagList.RemoveAll ();
}

void 
Packet::PrintPacketTags (std::ostream &os) const
{
  PacketTagIterator i = GetPacketTagIterator ();
  while (i.HasNext ())
    {
      PacketTagIterator::Item item = i.Next ();
      NS_ASSERT (item.GetTypeId ().HasConstructor ());
      Callback<ObjectBase *> constructor = item.GetTypeId ().GetConstructor ();
      NS_ASSERT (!constructor.IsNull ());
      ObjectBase *instance = constructor ();
      Tag *tag = dynamic_cast<Tag *> (instance);
      NS_ASSERT (tag != 0);
      item.GetTag (*tag);
      tag->Print (os);
      delete tag;
      if (i.HasNext ())
        {
          os << " ";
        }
    }
}

PacketTagIterator 
Packet::GetPacketTagIterator (void) const
{
  return PacketTagIterator (m_packetTagList.Head ());
}

std::ostream& operator<< (std::ostream& os, const Packet &packet)
{
  packet.Print (os);
  return os;
}

//-----------------------------------------------------------------------------
// Unit tests
//-----------------------------------------------------------------------------
namespace {

class ATestTagBase : public Tag
{
public:
  ATestTagBase () : m_error (false) {}
  bool m_error;
};

template <int N>
class ATestTag : public ATestTagBase
{
public:
  static TypeId GetTypeId (void) {
    std::ostringstream oss;
    oss << "anon::ATestTag<" << N << ">";
    static TypeId tid = TypeId (oss.str ().c_str ())
      .SetParent<Tag> ()
      .AddConstructor<ATestTag<N> > ()
      .HideFromDocumentation ()
    ;
    return tid;
  }
  virtual TypeId GetInstanceTypeId (void) const {
    return GetTypeId ();
  }
  virtual uint32_t GetSerializedSize (void) const {
    return N;
  }
  virtual void Serialize (TagBuffer buf) const {
    for (uint32_t i = 0; i < N; ++i)
      {
        buf.WriteU8 (N);
      }
  }
  virtual void Deserialize (TagBuffer buf) {
    for (uint32_t i = 0; i < N; ++i)
      {
        uint8_t v = buf.ReadU8 ();
        if (v != N)
          {
            m_error = true;
          }
      }
  }
  virtual void Print (std::ostream &os) const {
    os << N;
  }
  ATestTag ()
    : ATestTagBase () {}
};

class ATestHeaderBase : public Header
{
public:
  ATestHeaderBase () : Header (), m_error (false) {}
  bool m_error;
};

template <int N>
class ATestHeader : public ATestHeaderBase
{
public:
  static TypeId GetTypeId (void) {
    std::ostringstream oss;
    oss << "anon::ATestHeader<" << N << ">";
    static TypeId tid = TypeId (oss.str ().c_str ())
      .SetParent<Header> ()
      .AddConstructor<ATestHeader<N> > ()
      .HideFromDocumentation ()
    ;
    return tid;
  }
  virtual TypeId GetInstanceTypeId (void) const {
    return GetTypeId ();
  }
  virtual uint32_t GetSerializedSize (void) const {
    return N;
  }
  virtual void Serialize (Buffer::Iterator iter) const {
    for (uint32_t i = 0; i < N; ++i)
      {
        iter.WriteU8 (N);
      }
  }
  virtual uint32_t Deserialize (Buffer::Iterator iter) {
    for (uint32_t i = 0; i < N; ++i)
      {
        uint8_t v = iter.ReadU8 ();
        if (v != N)
          {
            m_error = true;
          }
      }
    return N;
  }
  virtual void Print (std::ostream &os) const {
  }
  ATestHeader ()
    : ATestHeaderBase () {}

};

class ATestTrailerBase : public Trailer
{
public:
  ATestTrailerBase () : Trailer (), m_error (false) {}
  bool m_error;
};

template <int N>
class ATestTrailer : public ATestTrailerBase
{
public:
  static TypeId GetTypeId (void) {
    std::ostringstream oss;
    oss << "anon::ATestTrailer<" << N << ">";
    static TypeId tid = TypeId (oss.str ().c_str ())
      .SetParent<Header> ()
      .AddConstructor<ATestTrailer<N> > ()
      .HideFromDocumentation ()
    ;
    return tid;
  }
  virtual TypeId GetInstanceTypeId (void) const {
    return GetTypeId ();
  }
  virtual uint32_t GetSerializedSize (void) const {
    return N;
  }
  virtual void Serialize (Buffer::Iterator iter) const {
    iter.Prev (N);
    for (uint32_t i = 0; i < N; ++i)
      {
        iter.WriteU8 (N);
      }
  }
  virtual uint32_t Deserialize (Buffer::Iterator iter) {
    iter.Prev (N);
    for (uint32_t i = 0; i < N; ++i)
      {
        uint8_t v = iter.ReadU8 ();
        if (v != N)
          {
            m_error = true;
          }
      }
    return N;
  }
  virtual void Print (std::ostream &os) const {
  }
  ATestTrailer ()
    : ATestTrailerBase () {}

};


struct Expected
{
  Expected (uint32_t n_, uint32_t start_, uint32_t end_)
    : n (n_), start (start_), end (end_) {}

  uint32_t n;
  uint32_t start;
  uint32_t end;
};

}

// tag name, start, end
#define E(a,b,c) a,b,c

#define CHECK(p, n, ...)                                \
  DoCheck (p, __FILE__, __LINE__, n, __VA_ARGS__)

class PacketTest : public TestCase
{
public:
  PacketTest ();
  virtual bool DoRun (void);
private:
  void DoCheck (Ptr<const Packet> p, const char *file, int line, uint32_t n, ...);
};


PacketTest::PacketTest ()
  : TestCase ("Packet") {
}

void
PacketTest::DoCheck (Ptr<const Packet> p, const char *file, int line, uint32_t n, ...)
{
  std::vector<struct Expected> expected;
  va_list ap;
  va_start (ap, n);
  for (uint32_t k = 0; k < n; ++k)
    {
      uint32_t N = va_arg (ap, uint32_t);
      uint32_t start = va_arg (ap, uint32_t);
      uint32_t end = va_arg (ap, uint32_t);
      expected.push_back (Expected (N, start, end));
    }
  va_end (ap);

  ByteTagIterator i = p->GetByteTagIterator ();
  uint32_t j = 0;
  while (i.HasNext () && j < expected.size ())
    {
      ByteTagIterator::Item item = i.Next ();
      struct Expected e = expected[j];
      std::ostringstream oss;
      oss << "anon::ATestTag<" << e.n << ">";
      NS_TEST_EXPECT_MSG_EQ_INTERNAL (item.GetTypeId ().GetName (), oss.str (), "trivial", file, line);
      NS_TEST_EXPECT_MSG_EQ_INTERNAL (item.GetStart (), e.start, "trivial", file, line);
      NS_TEST_EXPECT_MSG_EQ_INTERNAL (item.GetEnd (), e.end, "trivial", file, line);
      ATestTagBase *tag = dynamic_cast<ATestTagBase *> (item.GetTypeId ().GetConstructor () ());
      NS_TEST_EXPECT_MSG_NE (tag, 0, "trivial");
      item.GetTag (*tag);
      NS_TEST_EXPECT_MSG_EQ (tag->m_error, false, "trivial");
      delete tag;
      j++;
    }
  NS_TEST_EXPECT_MSG_EQ (i.HasNext (), false, "Nothing left");
  NS_TEST_EXPECT_MSG_EQ (j, expected.size (), "Size match");
}

bool
PacketTest::DoRun (void)
{
  Ptr<Packet> pkt1 = Create<Packet> (reinterpret_cast<const uint8_t*> ("hello"), 5);
  Ptr<Packet> pkt2 = Create<Packet> (reinterpret_cast<const uint8_t*> (" world"), 6);
  Ptr<Packet> packet = Create<Packet> ();
  packet->AddAtEnd (pkt1);
  packet->AddAtEnd (pkt2);

  NS_TEST_EXPECT_MSG_EQ (packet->GetSize (), 11, "trivial");

  uint8_t *buf = new uint8_t[packet->GetSize ()];
  packet->CopyData (buf, packet->GetSize ());

  std::string msg = std::string (reinterpret_cast<const char *>(buf),
                                 packet->GetSize ());
  delete [] buf;

  NS_TEST_EXPECT_MSG_EQ (msg, "hello world", "trivial");


  Ptr<const Packet> p = Create<Packet> (1000);

  p->AddByteTag (ATestTag<1> ());
  CHECK (p, 1, E (1, 0, 1000));
  Ptr<const Packet> copy = p->Copy ();
  CHECK (copy, 1, E (1, 0, 1000));

  p->AddByteTag (ATestTag<2> ());
  CHECK (p, 2, E (1, 0, 1000), E(2, 0, 1000));
  CHECK (copy, 1, E (1, 0, 1000));

  {
    Packet c0 = *copy;
    Packet c1 = *copy;
    c0 = c1;
    CHECK (&c0, 1, E (1, 0, 1000));
    CHECK (&c1, 1, E (1, 0, 1000));
    CHECK (copy, 1, E (1, 0, 1000));
    c0.AddByteTag (ATestTag<10> ());
    CHECK (&c0, 2, E (1, 0, 1000), E (10, 0, 1000));
    CHECK (&c1, 1, E (1, 0, 1000));
    CHECK (copy, 1, E (1, 0, 1000));
  }

  Ptr<Packet> frag0 = p->CreateFragment (0, 10);
  Ptr<Packet> frag1 = p->CreateFragment (10, 90);
  Ptr<const Packet> frag2 = p->CreateFragment (100, 900);
  frag0->AddByteTag (ATestTag<3> ());
  CHECK (frag0, 3, E (1, 0, 10), E(2, 0, 10), E (3, 0, 10));
  frag1->AddByteTag (ATestTag<4> ());
  CHECK (frag1, 3, E (1, 0, 90), E(2, 0, 90), E (4, 0, 90));
  frag2->AddByteTag (ATestTag<5> ());
  CHECK (frag2, 3, E (1, 0, 900), E(2, 0, 900), E (5, 0, 900));

  frag1->AddAtEnd (frag2);
  CHECK (frag1, 6, E (1, 0, 90), E(2, 0, 90), E (4, 0, 90), E (1, 90, 990), E(2, 90, 990), E (5, 90, 990));

  CHECK (frag0, 3, E (1, 0, 10), E(2, 0, 10), E (3, 0, 10));
  frag0->AddAtEnd (frag1);
  CHECK (frag0, 9, 
         E (1, 0, 10), E(2, 0, 10), E (3, 0, 10),
         E (1, 10, 100), E(2, 10, 100), E (4, 10, 100), 
         E (1, 100, 1000), E(2, 100, 1000), E (5, 100, 1000));


  // force caching a buffer of the right size.
  frag0 = Create<Packet> (1000);
  frag0->AddHeader (ATestHeader<10> ());
  frag0 = 0;

  p = Create<Packet> (1000);
  p->AddByteTag (ATestTag<20> ());
  CHECK (p, 1, E (20, 0, 1000));
  frag0 = p->CreateFragment (10, 90);
  CHECK (p, 1, E (20, 0, 1000));
  CHECK (frag0, 1, E (20, 0, 90));
  p = 0;
  frag0->AddHeader (ATestHeader<10> ());
  CHECK (frag0, 1, E (20, 10, 100));

  {
    Ptr<Packet> tmp = Create<Packet> (100);
    tmp->AddByteTag (ATestTag<20> ());
    CHECK (tmp, 1, E (20, 0, 100));
    tmp->AddHeader (ATestHeader<10> ());
    CHECK (tmp, 1, E (20, 10, 110));
    ATestHeader<10> h;
    tmp->RemoveHeader (h);
    CHECK (tmp, 1, E (20, 0, 100));
    tmp->AddHeader (ATestHeader<10> ());
    CHECK (tmp, 1, E (20, 10, 110));

    tmp = Create<Packet> (100);
    tmp->AddByteTag (ATestTag<20> ());
    CHECK (tmp, 1, E (20, 0, 100));
    tmp->AddTrailer (ATestTrailer<10> ());
    CHECK (tmp, 1, E (20, 0, 100));
    ATestTrailer<10> t;
    tmp->RemoveTrailer (t);
    CHECK (tmp, 1, E (20, 0, 100));
    tmp->AddTrailer (ATestTrailer<10> ());
    CHECK (tmp, 1, E (20, 0, 100));

  }

  {
    Ptr<Packet> tmp = Create<Packet> (0);
    tmp->AddHeader (ATestHeader<156> ());
    tmp->AddByteTag (ATestTag<20> ());
    CHECK (tmp, 1, E (20, 0, 156));
    tmp->RemoveAtStart (120);
    CHECK (tmp, 1, E (20, 0, 36));
    Ptr<Packet> a = Create<Packet> (0);
    a->AddAtEnd (tmp);
    CHECK (a, 1, E (20, 0, 36));
  }

  {
    Ptr<Packet> tmp = Create<Packet> (0);
    tmp->AddByteTag (ATestTag<20> ());
    CHECK (tmp, 0, E (20, 0, 0));
  }
  {
    Ptr<Packet> tmp = Create<Packet> (1000);
    tmp->AddByteTag (ATestTag<20> ());
    CHECK (tmp, 1, E (20, 0, 1000));
    tmp->RemoveAtStart (1000);
    CHECK (tmp, 0,  E (0,0,0));
    Ptr<Packet> a = Create<Packet> (10);
    a->AddByteTag (ATestTag<10> ());
    CHECK (a, 1, E (10, 0, 10));
    tmp->AddAtEnd (a);
    CHECK (tmp, 1, E (10, 0, 10));
  }

  {
    Packet p;
    ATestTag<10> a;
    p.AddPacketTag (a);
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (a), true, "trivial");
    ATestTag<11> b;
    p.AddPacketTag (b);
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (b), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (a), true, "trivial");
    Packet copy = p;
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (b), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (a), true, "trivial");
    ATestTag<12> c;
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (c), false, "trivial");
    copy.AddPacketTag (c);
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (c), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (b), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (a), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (c), false, "trivial");
    copy.RemovePacketTag (b);
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (b), false, "trivial");
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (b), true, "trivial");
    p.RemovePacketTag (a);
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (a), false, "trivial");
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (a), true, "trivial");
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (c), false, "trivial");
    NS_TEST_EXPECT_MSG_EQ (copy.PeekPacketTag (c), true, "trivial");
    p.RemoveAllPacketTags ();
    NS_TEST_EXPECT_MSG_EQ (p.PeekPacketTag (b), false, "trivial");
  }

  {
    // bug 572
    Ptr<Packet> tmp = Create<Packet> (1000);
    tmp->AddByteTag (ATestTag<20> ());
    CHECK (tmp, 1, E (20, 0, 1000));
    tmp->AddHeader (ATestHeader<2> ());
    CHECK (tmp, 1, E (20, 2, 1002));
    tmp->RemoveAtStart (1);
    CHECK (tmp, 1, E (20, 1, 1001));
#if 0
    tmp->PeekData ();
    CHECK (tmp, 1, E (20, 1, 1001));
#endif
  }

  return GetErrorStatus ();
}
//-----------------------------------------------------------------------------
class PacketTestSuite : public TestSuite
{
public:
  PacketTestSuite ();
};

PacketTestSuite::PacketTestSuite ()
  : TestSuite ("packet", UNIT)
{
  AddTestCase (new PacketTest);
}

PacketTestSuite g_packetTestSuite;


} // namespace ns3
