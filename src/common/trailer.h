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

#ifndef TRAILER_H
#define TRAILER_H

#include "chunk-registry.h"

/**
 * \relates ns3::Trailer
 * \brief this macro should be instantiated exactly once for each
 *        new type of Trailer
 *
 * This macro will ensure that your new Trailer type is registered
 * within the packet trailer registry. In most cases, this macro
 * is not really needed but, for safety, please, use it all the
 * time.
 *
 * Note: This macro is _absolutely_ needed if you try to run a
 * distributed simulation.
 */
#define NS_TRAILER_ENSURE_REGISTERED(x)          \
static class thisisaveryverylongclassname ##x    \
{                                                \
 public:                                         \
  thisisaveryverylongclassname ##x ()            \
    { uint32_t uid; uid = x::GetUid ();}         \
} g_thisisanotherveryveryverylongname ##x;

namespace ns3 {

/**
 * \brief Protocol trailer serialization and deserialization.
 *
 * Every Protocol trailer which needs to be inserted or removed
 * from a Packet instance must derive from this base class and
 * implement the following public methods:
 *   - a default constructor: is used by the internal implementation
 *     if the Packet class.
 *   - a static method named GetUid: is used to uniquely identify
 *     the type of each trailer. This method shall return a unique
 *     integer allocated with Trailer::AllocateUid.
 *   - a method named Serialize: is used by Packet::AddTrailer to
 *     store a trailer into the byte buffer of a packet.
 *     The input iterator points to the end of the byte buffer in
 *     which the trailer should write its data: the user is thus
 *     required to call Buffer::Iterator::Prev prior to writing
 *     any data in the buffer. The data written is expected to 
 *     match bit-for-bit the representation of this trailer in a 
 *     real network.
 *   - a method named GetSerializedSize: is used by Packet::AddTrailer
 *     to store a trailer into the byte buffer of a packet. This method
 *     should return the number of bytes which are needed to store
 *     the full trailer data by Serialize.
 *   - a method named Deserialize: is used by Packet::RemoveTrailer to
 *     re-create a trailer from the byte buffer of a packet. The input
 *     iterator points to the end of the byte buffer from which
 *     the trailer should read its data: the user is thus required to
 *     call Buffer::Iterator::Prev prior to reading any data from the
 *     buffer. The data read is expected to match bit-for-bit the 
 *     representation of this trailer in real networks. This method 
 *     shall return an integer which identifies the number of bytes read.
 *   - a method named Print: is used by Packet::Print to print the 
 *     content of a trailer as ascii data to a c++ output stream.
 *     Although the trailer is free to format its output as it
 *     wishes, it is recommended to follow a few rules to integrate
 *     with the packet pretty printer: start with flags, small field 
 *     values located between a pair of parens. Values should be separated 
 *     by whitespace. Follow the parens with the important fields, 
 *     separated by whitespace.
 *     i.e.: (field1 val1 field2 val2 field3 val3) field4 val4 field5 val5
 *   - a method named GetName: is used by Packet::Print to print
 *     trailer fragments. This method should return a user-readable
 *     single word as all capitalized letters.
 *
 */
class Trailer 
{
protected:
  template <typename T>
  static uint32_t AllocateUid (std::string uidString);
};

} // namespace ns3

namespace ns3 {

template <typename T>
uint32_t 
Trailer::AllocateUid (std::string uidString)
{
  return ChunkRegistry::RegisterTrailer<T> (uidString);
}


} // namespace ns3

#endif /* TRAILER_H */
