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

#include "chunk.h"

/**
 * \relates Trailer
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
#define NS_TRAILER_ENSURE_REGISTERED(x)         \
namespace {                                     \
static class thisisaveryverylongclassname       \
{                                               \
public:                                         \
  thisisaveryverylongclassname ()               \
  { uint32_t uid; uid = x::GetUid ();}          \
} g_thisisanotherveryveryverylongname;          \
}

namespace ns3 {

/**
 * \brief Protocol trailer serialization and deserialization.
 *
 * Every Protocol trailer which needs to be inserted or removed
 * from a Packet instance must derive from this abstract base class
 * and implement the private pure virtual methods listed below:
 *   - ns3::Trailer::SerializeTo
 *   - ns3::Trailer::DeserializeFrom
 *   - ns3::Trailer::GetSerializedSize
 *   - ns3::Trailer::PrintTo
 *   - ns3::Trailer::DoGetName
 *
 * Each trailer must also make sure that:
 *   - it defines a public default constructor
 *   - it defines a public static method named GetUid which returns a string.
 *
 * The latter should look like the following to ensure that
 * every trailer returns a unique string.
 * \code
 * class MyTrailer : public Header
 * {
 * public:
 *   static const char *GetUid (void);
 * };
 *
 * const char *MyTrailer::GetUid (void)
 * {
 *   return "MyTrailer.unique.prefix";
 * }
 * \endcode
 *
 * Note that the SerializeTo and DeserializeFrom methods behave
 * in a way which might seem surprising to users: the input iterator
 * really points to the end of the buffer to which and from which
 * the user is expected to write and read respectively. This means that
 * if the trailer has a fixed size and if the user wishes to read or
 * write that trailer from front to back, the user must rewind the 
 * iterator by hand to go to the start of the trailer. Typical code
 * looks like this:
 * \code
 * void CrcTrailer::SerializeTo (Buffer::Iterator end)
 * {
 *   end.Prev (4);
 *   end.WriteHtonU32 (m_crc);
 * }
 * \endcode
 *
 * Some users would have expected that the iterator would be rewinded 
 * to the "start" of the trailer before calling SerializeTo and DeserializeFrom.
 * However, this behavior was not implemented because it cannot be made to
 * work reliably for trailers which have a variable size. i.e., if the trailer 
 * contains options, the code which calls DeserializeFrom cannot rewind
 * to the start of the trailer because it does not know the real size of the 
 * trailer. Hence, to make this legitimate use-case work (variable-sized 
 * trailers), the input iterator to DeserializeFrom and SerializeTo points
 * to the end of the trailer, and not its start.
 */
class Trailer : public Chunk {
public:
  virtual ~Trailer ();
protected:
  template <typename T>
  static uint32_t Register (std::string uidString);
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
   *       protocol trailer must print itself.
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
   * \returns the size of the serialized Trailer.
   *
   * This method is used by Packet::AddTrailer to reserve
   * enough room in the packet byte buffer prior to calling
   * Trailer::Serialize.
   */
  virtual uint32_t GetSerializedSize (void) const = 0;

  /**
   * \param end the buffer iterator in which the protocol trailer
   *    must serialize itself. This iterator identifies 
   *    the end of the buffer.
   *
   * This iterator must be typically moved with the Buffer::Iterator::Prev
   * method before writing any byte in the buffer.
   */
  virtual void SerializeTo (Buffer::Iterator end) const = 0;
  /**
   * \param end the buffer iterator from which the protocol trailer must
   *    deserialize itself. This iterator identifies 
   *    the end of the buffer.
   * \returns the number of bytes read from the buffer
   *
   * This iterator must be typically moved with the Buffer::Iterator::Prev
   * method before reading any byte in the buffer. The value returned
   * is used to trim the packet byte buffer of the corresponding
   * amount when this method is invoked from Packet::RemoveTrailer
   */
  virtual uint32_t DeserializeFrom (Buffer::Iterator end) = 0;
};

} // namespace ns3

namespace ns3 {

template <typename T>
uint32_t 
Trailer::Register (std::string uidString)
{
  return ChunkRegistry::RegisterTrailer<T> (uidString);
}


} // namespace ns3

#endif /* TRAILER_H */
