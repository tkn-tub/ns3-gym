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
#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include "buffer.h"
#include "header.h"
#include "trailer.h"
#include "packet-metadata.h"
#include "tag.h"
#include "byte-tag-list.h"
#include "packet-tag-list.h"
#include "nix-vector.h"
#include "ns3/callback.h"
#include "ns3/assert.h"
#include "ns3/ptr.h"
#include "ns3/deprecated.h"

namespace ns3 {

/**
 * \ingroup network
 * \defgroup packet Packet
 */

/**
 * \ingroup packet
 * \brief Iterator over the set of tags in a packet
 *
 * This is a java-style iterator.
 */
class ByteTagIterator
{
public:
  /**
   * Identifies a tag and a set of bytes within a packet
   * to which the tag applies.
   */
  class Item
  {
public:
    /**
     * \returns the ns3::TypeId associated to this tag.
     */
    TypeId GetTypeId (void) const;
    /**
     * \returns the index of the first byte tagged by this tag.
     *
     * The index is an offset from the start of the packet.
     */
    uint32_t GetStart (void) const;
    /**
     * \returns the index of the last byte tagged by this tag.
     *
     * The index is an offset from the start of the packet.
     */
    uint32_t GetEnd (void) const;
    /**
     * \param tag the user tag to which the data should be copied.
     *
     * Read the requested tag and store it in the user-provided
     * tag instance. This method will crash if the type of the
     * tag provided by the user does not match the type of
     * the underlying tag.
     */
    void GetTag (Tag &tag) const;
private:
    friend class ByteTagIterator;
    Item (TypeId tid, uint32_t start, uint32_t end, TagBuffer buffer);
    TypeId m_tid;
    uint32_t m_start;
    uint32_t m_end;
    TagBuffer m_buffer;
  };
  /**
   * \returns true if calling Next is safe, false otherwise.
   */
  bool HasNext (void) const;
  /**
   * \returns the next item found and prepare for the next one.
   */
  Item Next (void);
private:
  friend class Packet;
  ByteTagIterator (ByteTagList::Iterator i);
  ByteTagList::Iterator m_current;
};

/**
 * \ingroup packet
 * \brief Iterator over the set of 'packet' tags in a packet
 *
 * This is a java-style iterator.
 */
class PacketTagIterator
{
public:
  /**
   * Identifies a tag within a packet.
   */
  class Item 
  {
public:
    /**
     * \returns the ns3::TypeId associated to this tag.
     */
    TypeId GetTypeId (void) const;
    /**
     * \param tag the user tag to which the data should be copied.
     *
     * Read the requested tag and store it in the user-provided
     * tag instance. This method will crash if the type of the
     * tag provided by the user does not match the type of
     * the underlying tag.
     */
    void GetTag (Tag &tag) const;
private:
    friend class PacketTagIterator;
    Item (const struct PacketTagList::TagData *data);
    const struct PacketTagList::TagData *m_data;
  };
  /**
   * \returns true if calling Next is safe, false otherwise.
   */
  bool HasNext (void) const;
  /**
   * \returns the next item found and prepare for the next one.
   */
  Item Next (void);
private:
  friend class Packet;
  PacketTagIterator (const struct PacketTagList::TagData *head);
  const struct PacketTagList::TagData *m_current;
};

/**
 * \ingroup packet
 * \brief network packets
 *
 * Each network packet contains a byte buffer, a set of byte tags, a set of
 * packet tags, and metadata.
 *
 * - The byte buffer stores the serialized content of the headers and trailers 
 * added to a packet. The serialized representation of these headers is expected
 * to match that of real network packets bit for bit (although nothing
 * forces you to do this) which means that the content of a packet buffer
 * is expected to be that of a real packet.
 *
 * - The metadata describes the type of the headers and trailers which
 * were serialized in the byte buffer. The maintenance of metadata is
 * optional and disabled by default. To enable it, you must call
 * Packet::EnablePrinting and this will allow you to get non-empty
 * output from Packet::Print. If you wish to only enable
 * checking of metadata, and do not need any printing capability, you can
 * call Packet::EnableChecking: its runtime cost is lower than
 * Packet::EnablePrinting.
 *
 * - The set of tags contain simulation-specific information which cannot
 * be stored in the packet byte buffer because the protocol headers or trailers
 * have no standard-conformant field for this information. So-called
 * 'byte' tags are used to tag a subset of the bytes in the packet byte buffer
 * while 'packet' tags are used to tag the packet itself. The main difference
 * between these two kinds of tags is what happens when packets are copied,
 * fragmented, and reassembled: 'byte' tags follow bytes while 'packet' tags
 * follow packets. Another important difference between these two kinds of tags
 * is that byte tags cannot be removed and are expected to be written once,
 * and read many times, while packet tags are expected to be written once,
 * read many times, and removed exactly once. An example of a 'byte' 
 * tag is a FlowIdTag which contains a flow id and is set by the application
 * generating traffic. An example of a 'packet' tag is a cross-layer 
 * qos class id set by an application and processed by a lower-level MAC 
 * layer.
 *
 * Implementing a new type of Header or Trailer for a new protocol is 
 * pretty easy and is a matter of creating a subclass of the ns3::Header 
 * or of the ns3::Trailer base class, and implementing the methods
 * described in their respective API documentation.
 *
 * Implementing a new type of Tag requires roughly the same amount of
 * work and this work is described in the ns3::Tag API documentation.
 *
 * The performance aspects of the Packet API are discussed in 
 * \ref packetperf
 */
class Packet : public SimpleRefCount<Packet>
{
public:

  /**
   * Create an empty packet with a new uid (as returned
   * by getUid).
   */
  Packet ();
  Packet (const Packet &o);
  Packet &operator = (const Packet &o);
  /**
   * Create a packet with a zero-filled payload.
   * The memory necessary for the payload is not allocated:
   * it will be allocated at any later point if you attempt
   * to fragment this packet or to access the zero-filled
   * bytes. The packet is allocated with a new uid (as 
   * returned by getUid).
   * 
   * \param size the size of the zero-filled payload
   */
  Packet (uint32_t size);
  /**
   * Create a new packet from the serialized buffer. This new packet 
   * is identical to the serialized packet contained in the buffer 
   * and is magically deserialized for you
   * 
   * \param buffer the serialized packet to be created
   * \param size the size of the packet for deserialization
   * \param magic allows packet deserialization; 
   *        asserts when set to false
   */
  Packet (uint8_t const*buffer, uint32_t size, bool magic);
  /**
   * Create a packet with payload filled with the content
   * of this buffer. The input data is copied: the input
   * buffer is untouched.
   *
   * \param buffer the data to store in the packet.
   * \param size the size of the input buffer.
   */
  Packet (uint8_t const*buffer, uint32_t size);
  /**
   * Create a new packet which contains a fragment of the original
   * packet. The returned packet shares the same uid as this packet.
   *
   * \param start offset from start of packet to start of fragment to create
   * \param length length of fragment to create
   * \returns a fragment of the original packet
   */
  Ptr<Packet> CreateFragment (uint32_t start, uint32_t length) const;
  /**
   * \returns the size in bytes of the packet (including the zero-filled
   *          initial payload)
   */
  inline uint32_t GetSize (void) const;
  /**
   * Add header to this packet. This method invokes the
   * Header::GetSerializedSize and Header::Serialize
   * methods to reserve space in the buffer and request the 
   * header to serialize itself in the packet buffer.
   *
   * \param header a reference to the header to add to this packet.
   */
  void AddHeader (const Header & header);
  /**
   * Deserialize and remove the header from the internal buffer.
   * This method invokes Header::Deserialize.
   *
   * \param header a reference to the header to remove from the internal buffer.
   * \returns the number of bytes removed from the packet.
   */
  uint32_t RemoveHeader (Header &header);
  /**
   * Deserialize but does _not_ remove the header from the internal buffer.
   * This method invokes Header::Deserialize.
   *
   * \param header a reference to the header to read from the internal buffer.
   * \returns the number of bytes read from the packet.
   */
  uint32_t PeekHeader (Header &header) const;
  /**
   * Add trailer to this packet. This method invokes the
   * Trailer::GetSerializedSize and Trailer::Serialize
   * methods to reserve space in the buffer and request the trailer 
   * to serialize itself in the packet buffer.
   *
   * \param trailer a reference to the trailer to add to this packet.
   */
  void AddTrailer (const Trailer &trailer);
  /**
   * Remove a deserialized trailer from the internal buffer.
   * This method invokes the Deserialize method.
   *
   * \param trailer a reference to the trailer to remove from the internal buffer.
   * \returns the number of bytes removed from the end of the packet.
   */
  uint32_t RemoveTrailer (Trailer &trailer);
  /**
   * Deserialize but does _not_ remove a trailer from the internal buffer.
   * This method invokes the Trailer::Deserialize method.
   *
   * \param trailer a reference to the trailer to read from the internal buffer.
   * \returns the number of bytes read from the end of the packet.
   */
  uint32_t PeekTrailer (Trailer &trailer);

  /**
   * Concatenate the input packet at the end of the current
   * packet. This does not alter the uid of either packet.
   *
   * \param packet packet to concatenate
   */
  void AddAtEnd (Ptr<const Packet> packet);
  /**
   * \param size number of padding bytes to add.
   */
  void AddPaddingAtEnd (uint32_t size);
  /** 
   * Remove size bytes from the end of the current packet
   * It is safe to remove more bytes that what is present in
   * the packet.
   *
   * \param size number of bytes from remove
   */
  void RemoveAtEnd (uint32_t size);
  /** 
   * Remove size bytes from the start of the current packet.
   * It is safe to remove more bytes that what is present in
   * the packet.
   *
   * \param size number of bytes from remove
   */
  void RemoveAtStart (uint32_t size);

  /**
   * If you try to change the content of the buffer
   * returned by this method, you will die.
   * Note that this method is now deprecated and will be removed in
   * the next version of ns-3. If you need to get access to the content
   * of the byte buffer of a packet, you need to call
   * ns3::Packet::CopyData to perform an explicit copy.
   *
   * \returns a pointer to the internal buffer of the packet.
   */
  uint8_t const *PeekData (void) const NS_DEPRECATED;

  /**
   * \param buffer a pointer to a byte buffer where the packet data 
   *        should be copied.
   * \param size the size of the byte buffer. 
   * \returns the number of bytes read from the packet
   *
   * No more than \b size bytes will be copied by this function.
   */
  uint32_t CopyData (uint8_t *buffer, uint32_t size) const;

  /**
   * \param os pointer to output stream in which we want
   *        to write the packet data.
   * \param size the maximum number of bytes we want to write
   *        in the output stream.
   */
  void CopyData (std::ostream *os, uint32_t size) const;

  /**
   * \returns a COW copy of the packet.
   *
   * The returns packet will behave like an independent copy of
   * the original packet, even though they both share the
   * same datasets internally.
   */
  Ptr<Packet> Copy (void) const;

  /**
   * A packet is allocated a new uid when it is created
   * empty or with zero-filled payload.
   *
   * Note: This uid is an internal uid and cannot be counted on to
   * provide an accurate counter of how many "simulated packets" of a
   * particular protocol are in the system. It is not trivial to make
   * this uid into such a counter, because of questions such as what
   * should the uid be when the packet is sent over broadcast media, or
   * when fragmentation occurs. If a user wants to trace actual packet
   * counts, he or she should look at e.g. the IP ID field or transport
   * sequence numbers, or other packet or frame counters at other
   * protocol layers.
   *
   * \returns an integer identifier which uniquely
   *          identifies this packet.
   */
  uint64_t GetUid (void) const;

  /**
   * \param os output stream in which the data should be printed.
   *
   * Iterate over the headers and trailers present in this packet, 
   * from the first header to the last trailer and invoke, for
   * each of them, the user-provided method Header::DoPrint or 
   * Trailer::DoPrint methods.
   */
  void Print (std::ostream &os) const;

  /**
   * \returns an iterator which points to the first 'item'
   * stored in this buffer. Note that this iterator will point
   * to an empty array of items if you don't call EnablePrinting
   * or EnableChecking before.
   *
   * \sa EnablePrinting EnableChecking
   */
  PacketMetadata::ItemIterator BeginItem (void) const;

  /**
   * By default, packets do not keep around enough metadata to
   * perform the operations requested by the Print methods. If you
   * want to be able the Packet::Print method, 
   * you need to invoke this method at least once during the 
   * simulation setup and before any packet is created.
   */
  static void EnablePrinting (void);
  /**
   * The packet metadata is also used to perform extensive
   * sanity checks at runtime when performing operations on a 
   * Packet. For example, this metadata is used to verify that
   * when you remove a header from a packet, this same header
   * was actually present at the front of the packet. These
   * errors will be detected and will abort the program.
   */
  static void EnableChecking (void);

  /**
   * For packet serializtion, the total size is checked 
   * in order to determine the size of the buffer 
   * required for serialization
   *
   * \returns number of bytes required for packet 
   * serialization
   */
  uint32_t GetSerializedSize (void) const;

  /*
   * \param buffer a raw byte buffer to which the packet will be serialized
   * \param maxSize the max size of the buffer for bounds checking
   *
   * A packet is completely serialized and placed into the raw byte buffer
   *
   * \returns zero if buffer size was too small
   */
  uint32_t Serialize (uint8_t* buffer, uint32_t maxSize) const;

  /**
   * \param tag the new tag to add to this packet
   *
   * Tag each byte included in this packet with the
   * new tag.
   *
   * Note that adding a tag is a const operation which is pretty 
   * un-intuitive. The rationale is that the content and behavior of
   * a packet is _not_ changed when a tag is added to a packet: any
   * code which was not aware of the new tag is going to work just
   * the same if the new tag is added. The real reason why adding a
   * tag was made a const operation is to allow a trace sink which gets
   * a packet to tag the packet, even if the packet is const (and most
   * trace sources should use const packets because it would be
   * totally evil to allow a trace sink to modify the content of a
   * packet).
   */
  void AddByteTag (const Tag &tag) const;
  /**
   * \returns an iterator over the set of byte tags included in this packet.
   */
  ByteTagIterator GetByteTagIterator (void) const;
  /**
   * \param tag the tag to search in this packet
   * \returns true if the requested tag type was found, false otherwise.
   *
   * If the requested tag type is found, it is copied in the user's 
   * provided tag instance.
   */
  bool FindFirstMatchingByteTag (Tag &tag) const;

  /**
   * Remove all the tags stored in this packet.
   */
  void RemoveAllByteTags (void);

  /**
   * \param os output stream in which the data should be printed.
   *
   * Iterate over the tags present in this packet, and
   * invoke the Print method of each tag stored in the packet.
   */
  void PrintByteTags (std::ostream &os) const;

  /**
   * \param tag the tag to store in this packet
   *
   * Add a tag to this packet. This method calls the
   * Tag::GetSerializedSize and, then, Tag::Serialize.
   *
   * Note that this method is const, that is, it does not
   * modify the state of this packet, which is fairly
   * un-intuitive.
   */
  void AddPacketTag (const Tag &tag) const;
  /**
   * \param tag the tag to remove from this packet
   * \returns true if the requested tag is found, false
   *          otherwise.
   *
   * Remove a tag from this packet. This method calls
   * Tag::Deserialize if the tag is found.
   */
  bool RemovePacketTag (Tag &tag);
  /**
   * \param tag the tag to search in this packet
   * \returns true if the requested tag is found, false
   *          otherwise.
   *
   * Search a matching tag and call Tag::Deserialize if it is found.
   */
  bool PeekPacketTag (Tag &tag) const;
  /**
   * Remove all packet tags.
   */
  void RemoveAllPacketTags (void);

  /**
   * \param os the stream in which we want to print data.
   *
   * Print the list of 'packet' tags.
   *
   * \sa Packet::AddPacketTag, Packet::RemovePacketTag, Packet::PeekPacketTag,
   *  Packet::RemoveAllPacketTags
   */
  void PrintPacketTags (std::ostream &os) const;

  /**
   * \returns an object which can be used to iterate over the list of
   *  packet tags.
   */
  PacketTagIterator GetPacketTagIterator (void) const;

  /* Note: These functions support a temporary solution 
   * to a specific problem in this generic class, i.e. 
   * how to associate something specific like nix-vector 
   * with a packet.  This design methodology 
   * should _not_ be followed, and is only here as an 
   * impetus to fix this general issue. */
  void SetNixVector (Ptr<NixVector>);
  Ptr<NixVector> GetNixVector (void) const; 

private:
  Packet (const Buffer &buffer, const ByteTagList &byteTagList, 
          const PacketTagList &packetTagList, const PacketMetadata &metadata);

  uint32_t Deserialize (uint8_t const*buffer, uint32_t size);

  Buffer m_buffer;
  ByteTagList m_byteTagList;
  PacketTagList m_packetTagList;
  PacketMetadata m_metadata;

  /* Please see comments above about nix-vector */
  Ptr<NixVector> m_nixVector;

  static uint32_t m_globalUid;
};

std::ostream& operator<< (std::ostream& os, const Packet &packet);

/**
 * \ingroup network
 * \defgroup packetperf Packet Performance
 * The current implementation of the byte buffers and tag list is based
 * on COW (Copy On Write. An introduction to COW can be found in Scott 
 * Meyer's "More Effective C++", items 17 and 29). What this means is that
 * copying packets without modifying them is very cheap (in terms of cpu
 * and memory usage) and modifying them can be also very cheap. What is 
 * key for proper COW implementations is being
 * able to detect when a given modification of the state of a packet triggers
 * a full copy of the data prior to the modification: COW systems need
 * to detect when an operation is "dirty".
 *
 * Dirty operations:
 *   - ns3::Packet::AddHeader
 *   - ns3::Packet::AddTrailer
 *   - both versions of ns3::Packet::AddAtEnd
 *   - ns3::Packet::RemovePacketTag
 *
 * Non-dirty operations:
 *   - ns3::Packet::AddPacketTag
 *   - ns3::Packet::PeekPacketTag
 *   - ns3::Packet::RemoveAllPacketTags
 *   - ns3::Packet::AddByteTag
 *   - ns3::Packet::FindFirstMatchingByteTag
 *   - ns3::Packet::RemoveAllByteTags
 *   - ns3::Packet::RemoveHeader
 *   - ns3::Packet::RemoveTrailer
 *   - ns3::Packet::CreateFragment
 *   - ns3::Packet::RemoveAtStart
 *   - ns3::Packet::RemoveAtEnd
 *   - ns3::Packet::CopyData
 *
 * Dirty operations will always be slower than non-dirty operations,
 * sometimes by several orders of magnitude. However, even the
 * dirty operations have been optimized for common use-cases which
 * means that most of the time, these operations will not trigger
 * data copies and will thus be still very fast.
 */

} // namespace ns3

namespace ns3 {

uint32_t 
Packet::GetSize (void) const
{
  return m_buffer.GetSize ();
}

} // namespace ns3

#endif /* PACKET_H */
