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
#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include "buffer.h"
#include "header.h"
#include "trailer.h"
#include "tags.h"
#include "packet-metadata.h"
#include "tag.h"
#include "ns3/callback.h"
#include "ns3/assert.h"

namespace ns3 {

class PacketPrinter;

/**
 * \brief network packets
 *
 * Each network packet contains a byte buffer, a list of tags, and
 * metadata.
 *
 * - The byte buffer stores the serialized content of the headers and trailers 
 * added to a packet. The serialized representation of these headers is expected
 * to match that of real network packets bit for bit (although nothing
 * forces you to do this) which means that the content of a packet buffer
 * is expected to be that of a real packet.
 *
 * - The list of tags stores an arbitrarily large set of arbitrary 
 * user-provided data structures in the packet: only one instance of
 * each type of data structure is allowed in a list of tags. 
 * These tags typically contain per-packet cross-layer information or 
 * flow identifiers. Each tag stored in the tag list can be at most
 * 16 bytes big. Trying to attach bigger data structures will trigger
 * crashes at runtime.
 *
 * - The metadata describes the type of the headers and trailers which
 * were serialized in the byte buffer. The maintenance of metadata is
 * optional and disabled by default. To enable it, you must call
 * Packet::EnableMetadata and this will allow you to get non-empty
 * output from Packet::Print and Packet::Print.
 *
 * Implementing a new type of Header or Trailer for a new protocol is 
 * pretty easy and is a matter of creating a subclass of the ns3::Header 
 * or of the ns3::Trailer base class, and implementing the 5 pure virtual 
 * methods defined in either of the two base classes. Users _must_
 * also make sure that they class defines a public default constructor and
 * a public method named GetUid, as documented in the ns3::Header and ns::Trailer
 * API documentations.
 *
 * Implementing a new type of Tag requires roughly the same amount of
 * work: users must implement a total of 6 methods which are described in
 * \ref tags
 *
 * The performance aspects of the Packet API are discussed in 
 * \ref packetperf
 */
class Packet {
public:
  /**
   * Create an empty packet with a new uid (as returned
   * by getUid).
   */
  Packet ();
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
  Packet CreateFragment (uint32_t start, uint32_t length) const;
  /**
   * \returns the size in bytes of the packet (including the zero-filled
   *          initial payload)
   */
  uint32_t GetSize (void) const;
  /**
   * Add header to this packet. This method invokes the
   * ns3::Header::GetSerializedSize and ns3::Header::SerializeTo 
   * methods to reserve space in the buffer and request the 
   * header to serialize itself in the packet buffer.
   *
   * \param header a reference to the header to add to this packet.
   */
  template <typename T>
  void AddHeader (T const &header);
  /**
   * Deserialize and remove the header from the internal buffer.
   * This method invokes ns3::Header::DeserializeFrom.
   *
   * \param header a reference to the header to remove from the internal buffer.
   * \returns the number of bytes removed from the packet.
   */
  template <typename T>
  uint32_t RemoveHeader (T &header);
  /**
   * Add trailer to this packet. This method invokes the
   * ns3::Trailer::GetSerializedSize and ns3::Trailer::serializeTo 
   * methods to reserve space in the buffer and request the trailer 
   * to serialize itself in the packet buffer.
   *
   * \param trailer a reference to the trailer to add to this packet.
   */
  template <typename T>
  void AddTrailer (T const &trailer);
  /**
   * Remove a deserialized trailer from the internal buffer.
   * This method invokes the ns3::Trailer::DeserializeFrom method.
   *
   * \param trailer a reference to the trailer to remove from the internal buffer.
   * \returns the number of bytes removed from the end of the packet.
   */
  template <typename T>
  uint32_t RemoveTrailer (T &trailer);
  /**
   * Attach a tag to this packet. The tag is fully copied
   * in a packet-specific internal buffer. This operation 
   * is expected to be really fast.
   *
   * \param tag a pointer to the tag to attach to this packet.
   */
  template <typename T>
  void AddTag (T const &tag);
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
  bool RemoveTag (T &tag);
  /**
   * Copy a tag stored internally to the input tag. If no instance
   * of this tag is present internally, the input tag is not modified.
   *
   * \param tag a pointer to the tag to read from this packet
   * \returns true if an instance of this tag type is stored
   *          in this packet, false otherwise.
   */
  template <typename T>
  bool PeekTag (T &tag) const;
  /**
   * Remove all the tags stored in this packet. This operation is
   * much much faster than invoking removeTag n times.
   */
  void RemoveAllTags (void);
  /**
   * Concatenate the input packet at the end of the current
   * packet. This does not alter the uid of either packet.
   *
   * \param packet packet to concatenate
   */
  void AddAtEnd (Packet packet);
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
   *
   * \returns a pointer to the internal buffer of the packet.
   */
  uint8_t const *PeekData (void) const;

  /**
   * A packet is allocated a new uid when it is created
   * empty or with zero-filled payload.
   *
   * \returns an integer identifier which uniquely
   *          identifies this packet.
   */
  uint32_t GetUid (void) const;

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
   * \param os output stream in which the data should be printed.
   * \param printer the output formatter to use to print
   *        the content of this packet.
   *
   * Iterate over the headers and trailers present in this packet,
   * either in the "forward" (first header to last trailer) or in
   * the "backward" (last trailer to first header) direction, as
   * specified by the PacketPrinter::PrintForward or the
   * PacketPrinter::PrintBackward methods. For each header, trailer,
   * or fragment of a header or a trailer, invoke the user-specified
   * print callback stored in the specified PacketPrinter.
   */
  void Print (std::ostream &os, const PacketPrinter &printer) const;

  /**
   * By default, packets do not keep around enough metadata to
   * perform the operations requested by the Print methods. If you
   * want to be able to invoke any of the two ::Print methods, 
   * you need to invoke this method at least once during the 
   * simulation setup and before any packet is created.
   *
   * The packet metadata is also used to perform extensive
   * sanity checks at runtime when performing operations on a 
   * Packet. For example, this metadata is used to verify that
   * when you remove a header from a packet, this same header
   * was actually present at the front of the packet. These
   * errors will be detected and will abort the program.
   */
  static void EnableMetadata (void);

  /**
   * \returns a byte buffer
   *
   * This method creates a serialized representation of a Packet object
   * ready to be transmitted over a network to another system. This
   * serialized representation contains a copy of the packet byte buffer,
   * the tag list, and the packet metadata (if there is one).
   *
   * This method will typically be used by parallel simulations where
   * the simulated system is partitioned and each partition runs on
   * a different CPU.
   */
  Buffer Serialize (void) const;
  /**
   * \param a byte buffer
   *
   * This method reads a byte buffer as created by Packet::Serialize
   * and restores the state of the Packet to what it was prio to
   * calling Serialize.
   *
   * This method will typically be used by parallel simulations where
   * the simulated system is partitioned and each partition runs on
   * a different CPU.
   */
  void Deserialize (Buffer buffer);
private:
  Packet (Buffer buffer, Tags tags, PacketMetadata metadata);
  Buffer m_buffer;
  Tags m_tags;
  PacketMetadata m_metadata;
  static uint32_t m_globalUid;
};

/**
 * \defgroup tags Packet Tags
 *
 * A tag is a class which must define:
 *  - a public default constructor
 *  - a public static method named GetUid
 *  - a public method named Print
 *  - a public method named GetSerializedSize
 *  - a public method named Serialize
 *  - a public method named Deserialize
 *
 * So, a tag class should look like this:
 * \code
 * // in header file
 * // note how a tag class does not derive from any other class.
 * class MyTag 
 * {
 * public:
 *   // we need a public default constructor
 *   MyTag ();
 *   // we need a public static GetUid
 *   // GetUid must return a 32 bit integer which uniquely
 *   // identifies this tag type
 *   static uint32_t GetUid (void);
 *   // Print should record in the output stream
 *   // the content of the tag instance.
 *   void Print (std::ostream &os) const;
 *   // GetSerializedSize should return the number of bytes needed
 *   // to store the state of a tag instance
 *   uint32_t GetSerializedSize (void) const;
 *   // Serialize should store its state in the input
 *   // buffer with the help of the iterator. It should
 *   // write exactly size bytes.
 *   void Serialize (Buffer::Iterator i, uint32_t size) const;
 *   // Deserialize should restore the state of a Tag instance
 *   // from a byte buffer with the help of the iterator
 *   uint32_t Deserialize (Buffer::Iterator i);
 * };
 *
 * // in source file
 * 
 * NS_TAG_ENSURE_REGISTERED (MyTag);
 *
 * std::string MyTag::GetUid (void)
 * {
 *   // we really want to make sure that this
 *   // string is unique in the universe.
 *   static uint32_t uid = TagRegistry::Register<MyTag> ("MyTag.unique.prefix");
 *   return uid;
 * }
 * \endcode
 */

/**
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
 *   - ns3::Packet::RemoveTag
 *   - ns3::Packet::AddHeader
 *   - ns3::Packet::AddTrailer
 *   - both versions of ns3::Packet::AddAtEnd
 *
 * Non-dirty operations:
 *   - ns3::Packet::AddTag
 *   - ns3::Packet::RemoveAllTags
 *   - ns3::Packet::PeekTag
 *   - ns3::Packet::RemoveHeader
 *   - ns3::Packet::RemoveTrailer
 *   - ns3::Packet::CreateFragment
 *   - ns3::Packet::RemoveAtStart
 *   - ns3::Packet::RemoveAtEnd
 *
 * Dirty operations will always be slower than non-dirty operations,
 * sometimes by several orders of magnitude. However, even the
 * dirty operations have been optimized for common use-cases which
 * means that most of the time, these operations will not trigger
 * data copies and will thus be still very fast.
 */

} // namespace ns3


/**************************************************
  Start of implementation of templates defined
  above
 *************************************************/

namespace ns3 {

template <typename T>
void
Packet::AddHeader (T const &header)
{
  const Header *testHeader;
  // if the following assignment fails, it is because the 
  // input to this function is not a subclass of the Header class
  testHeader = &header;
  uint32_t size = header.GetSerializedSize ();
  m_buffer.AddAtStart (size);
  header.Serialize (m_buffer.Begin ());
  m_metadata.AddHeader (header, size);
}
template <typename T>
uint32_t
Packet::RemoveHeader (T &header)
{
  Header *testHeader;
  // if the following assignment fails, it is because the 
  // input to this function is not a subclass of the Header class
  testHeader = &header;
  uint32_t deserialized = header.Deserialize (m_buffer.Begin ());
  m_buffer.RemoveAtStart (deserialized);
  m_metadata.RemoveHeader (header, deserialized);
  return deserialized;
}
template <typename T>
void
Packet::AddTrailer (T const &trailer)
{
  const Trailer *testTrailer;
  // if the following assignment fails, it is because the 
  // input to this function is not a subclass of the Trailer class
  testTrailer = &trailer;
  uint32_t size = trailer.GetSerializedSize ();
  m_buffer.AddAtEnd (size);
  Buffer::Iterator end = m_buffer.End ();
  trailer.Serialize (end);
  m_metadata.AddTrailer (trailer, size);
}
template <typename T>
uint32_t
Packet::RemoveTrailer (T &trailer)
{
  Trailer *testTrailer;
  // if the following assignment fails, it is because the 
  // input to this function is not a subclass of the Trailer class
  testTrailer = &trailer;
  uint32_t deserialized = trailer.Deserialize (m_buffer.End ());
  m_buffer.RemoveAtEnd (deserialized);
  m_metadata.RemoveTrailer (trailer, deserialized);
  return deserialized;
}


template <typename T>
void Packet::AddTag (T const& tag)
{
  const Tag *parent;
  // if the following assignment fails, it is because the
  // input to this function is not a subclass of the Tag class.
  parent = &tag;
  m_tags.Add (tag);
}
template <typename T>
bool Packet::RemoveTag (T & tag)
{
  Tag *parent;
  // if the following assignment fails, it is because the
  // input to this function is not a subclass of the Tag class.
  parent = &tag;
  return m_tags.Remove (tag);
}
template <typename T>
bool Packet::PeekTag (T & tag) const
{
  Tag *parent;
  // if the following assignment fails, it is because the
  // input to this function is not a subclass of the Tag class.
  parent = &tag;
  return m_tags.Peek (tag);
}

} // namespace ns3

#endif /* PACKET_H */
