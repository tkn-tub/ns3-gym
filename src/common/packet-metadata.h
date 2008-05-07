/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
#ifndef PACKET_METADATA_H
#define PACKET_METADATA_H

#include <stdint.h>
#include <vector>
#include "ns3/callback.h"
#include "ns3/assert.h"
#include "ns3/type-id.h"
#include "buffer.h"

namespace ns3 {

class Chunk;
class Buffer;
class Header;
class Trailer;

/**
 * \internal
 * \brief handle packet metadata about packet headers and trailers
 *
 * This class is used by the Packet class to record every operation
 * performed on the packet's buffer. This class also provides
 * an implementation of the Packet::Print methods which uses
 * the metadata to analyse the content of the packet's buffer.
 *
 * To achieve this, this class maintains a linked list of so-called
 * "items", each of which represents a header or a trailer, or 
 * payload, or a fragment of any of these. Each item contains a "next"
 * and a "prev" field which point to the next and previous entries
 * in the linked list. The PacketMetadata class maintains a pair
 * of pointers to the head and the tail of the linked list.
 *
 * Each entry in the list also maintains:
 *   - its native size (the size it had when it was first added
 *     to the packet)
 *   - its type: identifies what kind of header, what kind of trailer,
 *     if it is payload or not
 *   - the uid of the packet to which it was first added
 *   - the start and end of the area represented by a fragment
 *     if it is one.
 *
 * This linked list is flattened in a byte buffer stored in
 * struct PacketMetadata::Data. Each entry of the linked list is
 * identified by an offset which identifies the first byte of the
 * entry from the start of the data buffer. The size of this data
 * buffer is 2^16-1 bytes maximum which somewhat limits the number
 * of entries which can be stored in this linked list but it is
 * quite unlikely to hit this limit in practice.
 *
 * Each item of the linked list is a variable-sized byte buffer
 * made of a number of fields. Some of these fields are stored
 * as fixed-size 32 bit integers, others as fixed-size 16 bit 
 * integers, and some others as variable-size 32-bit integers.
 * The variable-size 32 bit integers are stored using the uleb128
 * encoding.
 */
class PacketMetadata 
{
public:
  struct Item 
  {
    enum {
      PAYLOAD,
      HEADER,
      TRAILER
    } type;
    /* true: this is a fragmented header, trailer, or, payload.
     * false: this is a whole header, trailer, or, payload.
     */
    bool isFragment;
    /* TypeId of Header or Trailer. Valid only if type is 
     * header or trailer.
     */
    TypeId tid;
    /* size of item. If fragment, size of fragment. Otherwise, 
     * size of original item. 
     */
    uint32_t currentSize;
    /* how many bytes were trimed from the start of a fragment.
     * if isFragment is true, this field is zero.
     */
    uint32_t currentTrimedFromStart;
    /* how many bytes were trimed from the end of a fragment.
     * if isFragment is true, this field is zero.
     */
    uint32_t currentTrimedFromEnd;
    /* an iterator which can be fed to Deserialize. Valid only
     * if isFragment and isPayload are false.
     */
    Buffer::Iterator current;
  };
  class ItemIterator 
  {
  public:
    ItemIterator (const PacketMetadata *metadata, Buffer buffer);
    bool HasNext (void) const;
    Item Next (void);
  private:
    const PacketMetadata *m_metadata;
    Buffer m_buffer;
    uint16_t m_current;
    uint32_t m_offset;
    bool m_hasReadTail;
  };

  static void Enable (void);

  inline PacketMetadata (uint32_t uid, uint32_t size);
  inline PacketMetadata (PacketMetadata const &o);
  inline PacketMetadata &operator = (PacketMetadata const& o);
  inline ~PacketMetadata ();

  void AddHeader (Header const &header, uint32_t size);
  void RemoveHeader (Header const &header, uint32_t size);

  void AddTrailer (Trailer const &trailer, uint32_t size);
  void RemoveTrailer (Trailer const &trailer, uint32_t size);

  PacketMetadata CreateFragment (uint32_t start, uint32_t end) const;
  void AddAtEnd (PacketMetadata const&o);
  void AddPaddingAtEnd (uint32_t end);
  void RemoveAtStart (uint32_t start);
  void RemoveAtEnd (uint32_t end);

  uint32_t GetUid (void) const;

  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator i, uint32_t size) const;
  uint32_t Deserialize (Buffer::Iterator i);

  static void PrintStats (void);

  ItemIterator BeginItem (Buffer buffer) const;

private:
  struct Data {
    /* number of references to this struct Data instance. */
    uint16_t m_count;
    /* size (in bytes) of m_data buffer below */
    uint16_t m_size;
    /* max of the m_used field over all objects which 
     * reference this struct Data instance */
    uint16_t m_dirtyEnd;
    /* variable-sized buffer of bytes */
    uint8_t m_data[10];
  };
  /* Note that since the next and prev fields are 16 bit integers
     and since the value 0xffff is reserved to identify the 
     fact that the end or the start of the list is reached,
     only a limited number of elements can be stored in 
     a m_data byte buffer.
   */
  struct SmallItem {
    /* offset (in bytes) from start of m_data buffer 
       to next element in linked list. value is 0xffff 
       if next element does not exist.
       stored as a fixed-size 16 bit integer.
    */
    uint16_t next;
    /* offset (in bytes) from start of m_data buffer 
       to previous element in linked list. value is 0xffff 
       if previous element does not exist.
       stored as a fixed-size 16 bit integer.
     */
    uint16_t prev;
    /* the high 31 bits of this field identify the 
       type of the header or trailer represented by 
       this item: the value zero represents payload.
       If the low bit of this uid is one, an ExtraItem
       structure follows this SmallItem structure.
       stored as a variable-size 32 bit integer.
     */
    uint32_t typeUid;
    /* the size (in bytes) of the header or trailer represented
       by this element.
       stored as a variable-size 32 bit integer.
     */
    uint32_t size;
    /* this field tries to uniquely identify each header or 
       trailer _instance_ while the typeUid field uniquely
       identifies each header or trailer _type_. This field
       is used to test whether two items are equal in the sense 
       that they represent the same header or trailer instance.
       That equality test is based on the typeUid and chunkUid
       fields so, the likelyhood that two header instances 
       share the same chunkUid _and_ typeUid is very small 
       unless they are really representations of the same header
       instance.
       stored as a fixed-size 16 bit integer.
     */
    uint16_t chunkUid;
  };
  struct ExtraItem {
    /* offset (in bytes) from start of original header to 
       the start of the fragment still present.
       stored as a variable-size 32 bit integer.
     */
    uint32_t fragmentStart;
    /* offset (in bytes) from start of original header to 
       the end of the fragment still present.
       stored as a variable-size 32 bit integer.
     */
    uint32_t fragmentEnd;
    /* the packetUid of the packet in which this header or trailer
       was first added. It could be different from the m_packetUid
       field if the user has aggregated multiple packets into one.
       stored as a fixed-size 32 bit integer.
     */
    uint32_t packetUid;
  };

  class DataFreeList : public std::vector<struct Data *>
  {
  public:
    ~DataFreeList ();
  };

  friend DataFreeList::~DataFreeList ();
  friend class ItemIterator;

  PacketMetadata ();

  inline uint16_t AddSmall (const PacketMetadata::SmallItem *item);
  uint16_t AddBig (uint32_t head, uint32_t tail,
                   const PacketMetadata::SmallItem *item, 
                   const PacketMetadata::ExtraItem *extraItem);
  void ReplaceTail (PacketMetadata::SmallItem *item, 
                    PacketMetadata::ExtraItem *extraItem,
                    uint32_t available);
  inline void UpdateHead (uint16_t written);
  inline void UpdateTail (uint16_t written);
  inline uint32_t GetUleb128Size (uint32_t value) const;
  uint32_t ReadUleb128 (const uint8_t **pBuffer) const;
  inline void Append16 (uint16_t value, uint8_t *buffer);
  inline bool TryToAppend (uint32_t value, uint8_t **pBuffer, uint8_t *end);
  inline bool TryToAppend32 (uint32_t value, uint8_t **pBuffer, uint8_t *end);
  inline bool TryToAppend16 (uint16_t value, uint8_t **pBuffer, uint8_t *end);
  inline void AppendValue (uint32_t value, uint8_t *buffer);
  void AppendValueExtra (uint32_t value, uint8_t *buffer);
  inline void Reserve (uint32_t n);
  void ReserveCopy (uint32_t n);
  uint32_t GetTotalSize (void) const;
  uint32_t ReadItems (uint16_t current, 
                      struct PacketMetadata::SmallItem *item,
                      struct PacketMetadata::ExtraItem *extraItem) const;
  void DoAddHeader (uint32_t uid, uint32_t size);


  static struct PacketMetadata::Data *Create (uint32_t size);
  static void Recycle (struct PacketMetadata::Data *data);
  static struct PacketMetadata::Data *Allocate (uint32_t n);
  static void Deallocate (struct PacketMetadata::Data *data);
  
  static DataFreeList m_freeList;
  static bool m_enable;

  // set to true when adding metadata to a packet is skipped because
  // m_enable is false; used to detect enabling of metadata in the
  // middle of a simulation, which isn't allowed.
  static bool m_metadataSkipped;

  static uint32_t m_maxSize;
  static uint16_t m_chunkUid;
  
  struct Data *m_data;
  /**
     head -(next)-> tail
       ^             |
        \---(prev)---|
   */
  uint16_t m_head;
  uint16_t m_tail;
  uint16_t m_used;
  uint32_t m_packetUid;
};

}; // namespace ns3

namespace ns3 {

PacketMetadata::PacketMetadata (uint32_t uid, uint32_t size)
  : m_data (m_data = PacketMetadata::Create (10)),
    m_head (0xffff),
    m_tail (0xffff),
    m_used (0),
    m_packetUid (uid)
{
  memset (m_data->m_data, 0xff, 4);
  if (size > 0)
    {
      DoAddHeader (0, size);
    }
}
PacketMetadata::PacketMetadata (PacketMetadata const &o)
  : m_data (o.m_data),
    m_head (o.m_head),
    m_tail (o.m_tail),
    m_used (o.m_used),
    m_packetUid (o.m_packetUid)
{
  NS_ASSERT (m_data != 0);
  m_data->m_count++;
}
PacketMetadata &
PacketMetadata::operator = (PacketMetadata const& o)
{
  if (m_data != o.m_data) 
    {
      // not self assignment
      NS_ASSERT (m_data != 0);
      m_data->m_count--;
      if (m_data->m_count == 0) 
        {
          PacketMetadata::Recycle (m_data);
        }
      m_data = o.m_data;
      NS_ASSERT (m_data != 0);
      m_data->m_count++;
    }
  m_head = o.m_head;
  m_tail = o.m_tail;
  m_used = o.m_used;
  m_packetUid = o.m_packetUid;
  return *this;
}
PacketMetadata::~PacketMetadata ()
{
  NS_ASSERT (m_data != 0);
  m_data->m_count--;
  if (m_data->m_count == 0) 
    {
      PacketMetadata::Recycle (m_data);
    }
}

} // namespace ns3


#endif /* PACKET_METADATA_H */
