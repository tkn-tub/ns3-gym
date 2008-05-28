/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <vector>

#define BUFFER_HEURISTICS 1
#define BUFFER_USE_INLINE 1


#ifdef BUFFER_USE_INLINE
#define BUFFER_INLINE inline
#else
#define BUFFER_INLINE
#endif

namespace ns3 {

/**
 * \brief automatically resized byte buffer
 *
 * This represents a buffer of bytes. Its size is
 * automatically adjusted to hold any data prepended
 * or appended by the user. Its implementation is optimized
 * to ensure that the number of buffer resizes is minimized,
 * by creating new Buffers of the maximum size ever used.
 * The correct maximum size is learned at runtime during use by 
 * recording the maximum size of each packet.
 *
 * \internal
 * The implementation of the Buffer class uses a COW (Copy On Write)
 * technique to ensure that the underlying data buffer which holds
 * the data bytes is shared among a lot of Buffer instances despite
 * data being added or removed from them.
 *
 * When multiple Buffer instances hold a reference to the same 
 * underlying BufferData object, they must be able to detect when
 * the operation they want to perform should trigger a copy of the
 * BufferData. If the BufferData::m_count field is one, it means that
 * there exist only one instance of Buffer which references the 
 * BufferData instance so, it is safe to modify it. It is also
 * safe to modify the content of a BufferData if someone has not yet
 * made incompatible changes to the BufferData and we detect that
 * case when the m_ownerId field is equal to the m_ownerId of the
 * BufferData itself. Whenever a change potentially incompatible
 * with other users is made to a BufferData object, we update the
 * m_ownerId field of the BufferData to ensure that the other users
 * will detect out modification.
 * In every other case, the BufferData must be copied before
 * being modified.
 *
 * To understand the way the Buffer::Add and Buffer::Remove methods
 * work, you first need to understand the "virtual offsets" used to
 * keep track of the content of buffers. Each Buffer instance
 * contains real data bytes in its BufferData instance but it also
 * contains "virtual zero data" which typically is used to represent
 * application-level payload. No memory is allocated to store the
 * zero bytes of application-level payload unless the user fragments
 * a Buffer: this application-level payload is kept track of with
 * a pair of integers which describe where in the buffer content
 * the "virtual zero area" starts and ends.
 *
 * ***: unused bytes
 * xxx: bytes "added" at the front of the zero area
 * ...: bytes "added" at the back of the zero area
 * 000: virtual zero bytes
 *
 * Real byte buffer:      |********xxxxxxxxxxxx.........*****|
 *                        |--------^ m_start
 *                        |-------------------^ m_zeroAreaStart
 *                        |-----------------------------^ m_end - (m_zeroAreaEnd - m_zeroAreaStart)
 * virtual byte buffer:           |xxxxxxxxxxxx0000000000000.........|
 *                        |--------^ m_start
 *                        |--------------------^ m_zeroAreaStart
 *                        |---------------------------------^ m_zeroAreaEnd
 *                        |------------------------------------------^ m_end
 *
 * A simple state invariant is that m_start <= m_zeroStart <= m_zeroEnd <= m_end
 */
class Buffer 
{
public:
  /**
   * \brief iterator in a Buffer instance
   */
  class Iterator 
  {
  public:
      Iterator ();
      /**
       * go forward by one byte
       */
      void Next (void);
      /**
       * go backward by one byte
       */
      void Prev (void);
      /**
       * \param delta number of bytes to go forward
       */
      void Next (uint32_t delta);
      /**
       * \param delta number of bytes to go backward
       */
      void Prev (uint32_t delta);
      /**
       * \param o the second iterator
       * \return number of bytes included between the two iterators
       *
       * This method works only if the two iterators point
       * to the same underlying buffer. Debug builds ensure
       * this with an assert.
       */
      uint32_t GetDistanceFrom (Iterator const &o) const;
      
      /**
       * \return true if this iterator points to the end of the byte array.
       *     false otherwise.
       */
      bool IsEnd (void) const;
      /**
       * \return true if this iterator points to the start of the byte array.
       *     false otherwise.
       */
      bool IsStart (void) const;

      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by one byte.
       */
      BUFFER_INLINE void WriteU8 (uint8_t  data);
      /**
       * \param data data to write in buffer
       * \param len number of times data must be written in buffer
       *
       * Write the data in buffer len times and avance the iterator position
       * by len byte.
       */
      BUFFER_INLINE void WriteU8 (uint8_t data, uint32_t len);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by two bytes. The format of the data written in the byte
       * buffer is non-portable. We only ensure that readU16 will
       * return exactly what we wrote with writeU16 if the program
       * is run on the same machine.
       */
      void WriteU16 (uint16_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by four bytes. The format of the data written in the byte
       * buffer is non-portable. We only ensure that readU32 will
       * return exactly what we wrote with writeU32 if the program
       * is run on the same machine.
       */
      void WriteU32 (uint32_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by eight bytes. The format of the data written in the byte
       * buffer is non-portable. We only ensure that readU64 will
       * return exactly what we wrote with writeU64 if the program
       * is run on the same machine.
       */
      void WriteU64 (uint64_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by two bytes. The data is written in network order and the
       * input data is expected to be in host order.
       */
      void WriteHtolsbU16 (uint16_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by four bytes. The data is written in least significant byte order and the
       * input data is expected to be in host order.
       */
      void WriteHtolsbU32 (uint32_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by eight bytes. The data is written in least significant byte order and the
       * input data is expected to be in host order.
       */
      void WriteHtolsbU64 (uint64_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by two bytes. The data is written in least significant byte order and the
       * input data is expected to be in host order.
       */
      void WriteHtonU16 (uint16_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by four bytes. The data is written in network order and the
       * input data is expected to be in host order.
       */
      void WriteHtonU32 (uint32_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by eight bytes. The data is written in network order and the
       * input data is expected to be in host order.
       */
      void WriteHtonU64 (uint64_t data);
      /**
       * \param buffer a byte buffer to copy in the internal buffer.
       * \param size number of bytes to copy.
       *
       * Write the data in buffer and avance the iterator position
       * by size bytes.
       */
      void Write (uint8_t const*buffer, uint32_t size);
      /**
       * \param start the start of the data to copy
       * \param end the end of the data to copy
       *
       * Write the data delimited by start and end in internal buffer 
       * and avance the iterator position by the number of bytes
       * copied.
       * The input interators _must_ not point to the same Buffer as
       * we do to avoid overlapping copies. This is enforced 
       * in debug builds by asserts.
       */
      void Write (Iterator start, Iterator end);

      /**
       * \return the byte read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       */
      BUFFER_INLINE uint8_t  ReadU8 (void);
      /**
       * \return the two bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in the format written by writeU16.
       */
      uint16_t ReadU16 (void);
      /**
       * \return the four bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in the format written by writeU32.
       */
      uint32_t ReadU32 (void);
      /**
       * \return the eight bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in the format written by writeU64.
       */
      uint64_t ReadU64 (void);
      /**
       * \return the two bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in network format and return in host format.
       */
      uint16_t ReadNtohU16 (void);
      /**
       * \return the four bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in network format and return in host format.
       */
      uint32_t ReadNtohU32 (void);
      /**
       * \return the eight bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in network format and return in host format.
       */
      uint64_t ReadNtohU64 (void);
      /**
       * \return the two bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in network format and return in host format.
       */
      uint16_t ReadLsbtohU16 (void);
      /**
       * \return the four bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in network format and return in host format.
       */
      uint32_t ReadLsbtohU32 (void);
      /**
       * \return the eight bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in network format and return in host format.
       */
      uint64_t ReadLsbtohU64 (void);
      /**
       * \param buffer buffer to copy data into
       * \param size number of bytes to copy
       *
       * Copy size bytes of data from the internal buffer to the
       * input buffer and avance the Iterator by the number of
       * bytes read.
       */
      void Read (uint8_t *buffer, uint32_t size);
  private:
      friend class Buffer;
      Iterator (Buffer const*buffer);
      Iterator (Buffer const*buffer, bool);
      void Construct (const Buffer *buffer);
      bool CheckNoZero (uint32_t start, uint32_t end) const;
      bool Check (uint32_t i) const;

    /* offset in virtual bytes from the start of the data buffer to the
     * start of the "virtual zero area".
     */
      uint32_t m_zeroStart;
    /* offset in virtual bytes from the start of the data buffer to the
     * end of the "virtual zero area".
     */
      uint32_t m_zeroEnd;
    /* offset in virtual bytes from the start of the data buffer to the
     * start of the data which can be read by this iterator
     */
      uint32_t m_dataStart;
    /* offset in virtual bytes from the start of the data buffer to the
     * end of the data which can be read by this iterator
     */
      uint32_t m_dataEnd;
    /* offset in virtual bytes from the start of the data buffer to the
     * current position represented by this iterator.
     */
      uint32_t m_current;
    /* a pointer to the underlying byte buffer. All offsets are relative
     * to this pointer.
     */
      uint8_t *m_data;
  };

  /**
   * \return the number of bytes stored in this buffer.
   */
  uint32_t GetSize (void) const;

  /**
   * \return a pointer to the start of the internal 
   * byte buffer.
   *
   * The returned pointer points to an area of
   * memory which is ns3::Buffer::GetSize () bytes big.
   * Please, try to never ever use this method. It is really
   * evil and is present only for a few specific uses.
   */
  uint8_t const*PeekData (void) const;

  /**
   * \param start size to reserve
   * \returns true if the buffer needed resizing, false otherwise.
   *
   * Add bytes at the start of the Buffer. The
   * content of these bytes is undefined but debugging
   * builds initialize them to 0x33.
   * Any call to this method invalidates any Iterator
   * pointing to this Buffer.
   */
  bool AddAtStart (uint32_t start);
  /**
   * \param end size to reserve
   * \returns true if the buffer needed resizing, false otherwise.
   *
   * Add bytes at the end of the Buffer. The
   * content of these bytes is undefined but debugging
   * builds initialize them to 0x33.
   * Any call to this method invalidates any Iterator
   * pointing to this Buffer.
   */
  bool AddAtEnd (uint32_t end);

  /**
   * \param o the buffer to append to the end of this buffer.
   *
   * Add bytes at the end of the Buffer.
   * Any call to this method invalidates any Iterator
   * pointing to this Buffer.
   */
  void AddAtEnd (const Buffer &o);
  /**
   * \param start size to remove
   *
   * Remove bytes at the start of the Buffer.
   * Any call to this method invalidates any Iterator
   * pointing to this Buffer.
   */
  void RemoveAtStart (uint32_t start);
  /**
   * \param end size to remove
   *
   * Remove bytes at the end of the Buffer.
   * Any call to this method invalidates any Iterator
   * pointing to this Buffer.
   */
  void RemoveAtEnd (uint32_t end);

  /**
   * \param start offset from start of packet
   * \param length
   *
   * \return a fragment of size length starting at offset
   * start.
   */
  Buffer CreateFragment (uint32_t start, uint32_t length) const;

  /**
   * \return an Iterator which points to the
   * start of this Buffer.
   */
  Buffer::Iterator Begin (void) const;
  /**
   * \return an Iterator which points to the
   * end of this Buffer.
   */
  Buffer::Iterator End (void) const;

  Buffer CreateFullCopy (void) const;

  int32_t GetCurrentStartOffset (void) const;
  int32_t GetCurrentEndOffset (void) const;

  Buffer (Buffer const &o);
  Buffer &operator = (Buffer const &o);
  Buffer ();
  Buffer (uint32_t dataSize);
  ~Buffer ();
private:

  void TransformIntoRealBuffer (void) const;
  bool CheckInternalState (void) const;
  void Initialize (uint32_t zeroSize);
  uint32_t GetInternalSize (void) const;
  uint32_t GetInternalEnd (void) const;
  static void Recycle (struct BufferData *data);
  static struct BufferData *Create (uint32_t size);

  /* This structure is described in the buffer.cc file.
   */
  struct BufferData *m_data;
  /* id of the owner of the BufferData object.
   * If this id matches the id stored in the BufferData object,
   * we are the current owner of the data. If we are not the owner,
   * and we need to acquire ownership before making modifications
   * by doing a copy.
   */
  uint32_t m_ownerId;
#ifdef BUFFER_HEURISTICS
  /* keep track of the maximum value of m_zeroAreaStart across
   * the lifetime of a Buffer instance. This variable is used
   * purely as a source of information for the heuristics which
   * decide on the position of the zero area in new buffers.
   * It is read from the Buffer destructor to update the global
   * heuristic data and these global heuristic data are used from
   * the Buffer constructor to choose an initial value for 
   * m_zeroAreaStart.
   * It is possible to disable all these heuristics by undefining the
   * BUFFER_HEURISTICS macro at the top of buffer.h
   */
  uint32_t m_maxZeroAreaStart;
#endif /* BUFFER_HEURISTICS */
  /* offset to the start of the virtual zero area from the start 
   * of m_data->m_data
   */
  uint32_t m_zeroAreaStart;
  /* offset to the end of the virtual zero area from the start 
   * of m_data->m_data
   */
  uint32_t m_zeroAreaEnd;
  /* offset to the start of the data referenced by this Buffer
   * instance from the start of m_data->m_data
   */
  uint32_t m_start;
  /* offset to the end of the data referenced by this Buffer
   * instance from the start of m_data->m_data
   */
  uint32_t m_end;
};

} // namespace ns3

#ifdef BUFFER_USE_INLINE

#include "ns3/assert.h"

namespace ns3 {

void
Buffer::Iterator::WriteU8 (uint8_t data)
{
  NS_ASSERT (Check (m_current));

  if (m_current < m_zeroStart)
    {
      m_data[m_current] = data;
      m_current++;
    }
  else
    {
      m_data[m_current - (m_zeroEnd-m_zeroStart)] = data;
      m_current++;      
    }
}

void 
Buffer::Iterator::WriteU8 (uint8_t  data, uint32_t len)
{
  NS_ASSERT (CheckNoZero (m_current, m_current + len));
  if (m_current <= m_zeroStart)
    {
      memset (&(m_data[m_current]), data, len);
      m_current += len;
    }
  else
    {
      uint8_t *buffer = &m_data[m_current - (m_zeroEnd-m_zeroStart)];
      memset (buffer, data, len);
      m_current += len;
    }
}

uint8_t  
Buffer::Iterator::ReadU8 (void)
{
  NS_ASSERT (m_current >= m_dataStart &&
             m_current <= m_dataEnd);

  if (m_current < m_zeroStart)
    {
      uint8_t data = m_data[m_current];
      m_current++;
      return data;
    }
  else if (m_current < m_zeroEnd)
    {
      m_current++;
      return 0;
    }
  else
    {
      uint8_t data = m_data[m_current - (m_zeroEnd-m_zeroStart)];
      m_current++;
      return data;
    }
}


} // namespace ns3

#endif /* BUFFER_USE_INLINE */

#endif /* BUFFER_H */
