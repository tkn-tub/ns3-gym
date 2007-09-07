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
 */
class Buffer {
public:
  /**
   * \brief iterator in a Buffer instance
   */
  class Iterator {
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
      void WriteU8 (uint8_t  data);
      /**
       * \param data data to write in buffer
       * \param len number of times data must be written in buffer
       *
       * Write the data in buffer len times and avance the iterator position
       * by len byte.
       */
      void WriteU8 (uint8_t data, uint32_t len);
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
      uint8_t  ReadU8 (void);
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
      uint32_t m_zeroStart;
      uint32_t m_zeroEnd;
      uint32_t m_dataStart;
      uint32_t m_dataEnd;
      uint32_t m_current;
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
   *
   * Add bytes at the start of the Buffer. The
   * content of these bytes is undefined but debugging
   * builds initialize them to 0x33.
   * Any call to this method invalidates any Iterator
   * pointing to this Buffer.
   */
  void AddAtStart (uint32_t start);
  /**
   * \param end size to reserve
   *
   * Add bytes at the end of the Buffer. The
   * content of these bytes is undefined but debugging
   * builds initialize them to 0x33.
   * Any call to this method invalidates any Iterator
   * pointing to this Buffer.
   */
  void AddAtEnd (uint32_t end);
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

  Buffer (Buffer const &o);
  Buffer &operator = (Buffer const &o);
  Buffer ();
  Buffer (uint32_t dataSize);
  ~Buffer ();
private:
  struct BufferData {
      uint32_t m_count;
      uint32_t m_size;
      uint32_t m_initialStart;
      uint32_t m_dirtyStart;
      uint32_t m_dirtySize;
      uint8_t m_data[1];
  };
  class BufferDataList : public std::vector<struct Buffer::BufferData*>
  {
  public:
    ~BufferDataList ();
  };

  uint8_t *GetStart (void) const;
  void TransformIntoRealBuffer (void) const;
  static void Recycle (struct Buffer::BufferData *data);
  static struct Buffer::BufferData *Create (void);
  static struct Buffer::BufferData *Allocate (uint32_t size, uint32_t start);
  static void Deallocate (struct Buffer::BufferData *data);

  static BufferDataList m_freeList;
  static uint32_t m_maxTotalAddStart;
  static uint32_t m_maxTotalAddEnd;

  struct BufferData *m_data;
  uint32_t m_zeroAreaSize;
  uint32_t m_start;
  uint32_t m_size;
};

} // namespace ns3

#endif /* BUFFER_H */
