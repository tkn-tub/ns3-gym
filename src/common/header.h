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
 * \relates Header
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
 * from a Packet instance must derive from this abstract base class
 * and implement the private pure virtual methods listed below:
 *   - ns3::Header::SerializeTo
 *   - ns3::Header::DeserializeFrom
 *   - ns3::Header::GetSerializedSize
 *   - ns3::Header::PrintTo
 *   - ns3::Header::DoGetName
 *
 * Each header must also make sure that:
 *   - it defines a public default constructor
 *   - it defines a public static method named GetUid which returns a 32 bit integer
 *
 * The latter should look like the following:
 * \code
 * // in the header,
 * class MyHeader : public Header
 * {
 * public:
 *   static uint32_t GetUid (void);
 * };
 *
 * // in the source file:
 * NS_HEADER_ENSURE_REGISTERED (MyHeader);
 *
 * uint32_t MyHeader::GetUid (void)
 * {
 *   static uint32_t uid = Header::Register<MyHeader> ("MyHeader.unique.prefix");
 *   return uid;
 * }
 * \endcode
 *
 * Sample code which shows how to create a new Header, and how to use it, 
 * is shown in the sample file samples/main-packet-header.cc
 */
class Header {
public:
protected:
  template <typename T>
  static uint32_t AllocateUid (std::string uuid);
#if 0
private:
  /**
   * \returns a user-readable name to identify this type of header.
   *
   * The string returned is expected to be a single word with 
   * all capital letters
   */
  virtual std::string DoGetName (void) const = 0;
  /**
   * \param os the std output stream in which this 
   *       protocol header must print itself.
   *
   * Although the header is free to format its output as it
   * wishes, it is recommended to follow a few rules to integrate
   * with the packet pretty printer:
   *   - start with flags, small field values located between a
   *     pair of parens. Values should be separated by whitespace.
   *   - follow the parens with the important fields, separated by
   *     whitespace.
   * i.e.:
   * (field1 val1 field2 val2 field3 val3) field4 val4 field5 val5
   */
  virtual void PrintTo (std::ostream &os) const = 0;

  /**
   * \returns the size of the serialized Header.
   *
   * This method is used by Packet::AddHeader to reserve
   * enough room in the packet byte buffer prior to calling
   * Header::Serialize.
   */
  virtual uint32_t GetSerializedSize (void) const = 0;

  /**
   * \param start the buffer iterator in which the protocol header
   *    must serialize itself. This iterator identifies 
   *    the start of the buffer.
   */
  virtual void SerializeTo (Buffer::Iterator start) const = 0;
  /**
   * \param start the buffer iterator from which the protocol header must
   *    deserialize itself. This iterator identifies 
   *    the start of the buffer.
   * \returns the number of bytes read from the buffer
   *
   * The value returned is used to trim the packet byte buffer of the 
   * corresponding amount when this method is invoked from 
   * Packet::RemoveHeader
   */
  virtual uint32_t DeserializeFrom (Buffer::Iterator start) = 0;
#endif
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
