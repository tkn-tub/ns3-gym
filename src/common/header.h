/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifndef HEADER_H
#define HEADER_H

#include "chunk-registry.h"

/**
 * \relates ns3::Header
 * \brief this macro should be instantiated exactly once for each
 *        new type of Header
 *
 * This macro will ensure that your new Header type is registered
 * within the packet header registry. In most cases, this macro
 * is not really needed but, for safety, please, use it all the
 * time.
 *
 * Note: This macro is _absolutely_ needed if you try to run a
 * distributed simulation.
 */
#define NS_HEADER_ENSURE_REGISTERED(x)         \
static class thisisaveryverylongclassname ##x  \
{                                              \
 public:                                       \
  thisisaveryverylongclassname ##x ()          \
    { uint32_t uid; uid = x::GetUid ();}       \
} g_thisisanotherveryveryverylongname ## x;


namespace ns3 {

/**
 * \brief Protocol header serialization and deserialization.
 *
 * Every Protocol header which needs to be inserted or removed
 * from a Packet instance must derive from this base class and
 * implement the following public methods:
 *   - a default constructor: is used by the internal implementation
 *     if the Packet class.
 *   - a static method named GetUid: is used to uniquely identify
 *     the type of each header. This method shall return a unique
 *     integer allocated with Header::AllocateUid.
 *   - a method named Print: is used by Packet::Print to print the 
 *     content of a header as ascii data to a c++ output stream.
 *     Although the header is free to format its output as it
 *     wishes, it is recommended to follow a few rules to integrate
 *     with the packet pretty printer: start with flags, small field 
 *     values located between a pair of parens. Values should be separated 
 *     by whitespace. Follow the parens with the important fields, 
 *     separated by whitespace.
 *     i.e.: (field1 val1 field2 val2 field3 val3) field4 val4 field5 val5
 *   - a method named GetName: is used by Packet::Print to print
 *     header fragments. This method should return a user-readable
 *     single word as all capitalized letters.
 *
 * Sample code which shows how to create a new type of Header, and how to use it, 
 * is shown in the sample file samples/main-packet-header.cc
 */
class Header
{
public:
  virtual ~Header ();
  /**
   * \returns the expected size of the header.
   *
   * This method is used by Packet::AddHeader
   * to store a header into the byte buffer of a packet. This method
   * should return the number of bytes which are needed to store
   * the full header data by Serialize.
   */
  virtual uint32_t GetSerializedSize (void) const = 0;
  /**
   * \param start an iterator which points to where the header should
   *        be written.
   *
   * This method is used by Packet::AddHeader to
   * store a header into the byte buffer of a packet.
   * The data written
   * is expected to match bit-for-bit the representation of this
   * header in a real network.
   */
  virtual void Serialize (Buffer::Iterator start) const = 0;
  /**
   * \param start an iterator which points to where the header should
   *        written.
   * \returns the number of bytes read.
   *
   * This method is used by Packet::RemoveHeader to
   * re-create a header from the byte buffer of a packet. 
   * The data read is expected to
   * match bit-for-bit the representation of this header in real
   * networks.
   */
  virtual uint32_t Deserialize (Buffer::Iterator start) = 0;
protected:
  template <typename T>
  static uint32_t AllocateUid (std::string uuid);
};

} // namespace ns3

namespace ns3 {

template <typename T>
uint32_t 
Header::AllocateUid (std::string uuid)
{
  return ChunkRegistry::RegisterHeader<T> (uuid);
}

} // namespace ns3

#endif /* HEADER_H */
