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
private:
  /**
   * \param os the std output stream in which this 
   *       protocol trailer must print itself.
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

}; // namespace ns3

#endif /* TRAILER_H */
