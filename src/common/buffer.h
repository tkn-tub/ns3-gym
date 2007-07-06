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
      inline Iterator ();
      /**
       * go forward by one byte
       */
      inline void Next (void);
      /**
       * go backward by one byte
       */
      inline void Prev (void);
      /**
       * \param delta number of bytes to go forward
       */
      inline void Next (uint32_t delta);
      /**
       * \param delta number of bytes to go backward
       */
      inline void Prev (uint32_t delta);
      /**
       * \param o the second iterator
       * \return number of bytes included between the two iterators
       *
       * This method works only if the two iterators point
       * to the same underlying buffer. Debug builds ensure
       * this with an assert.
       */
      inline int32_t GetDistanceFrom (Iterator const &o) const;
      
      /**
       * \return true if this iterator points to the end of the byte array.
       *     false otherwise.
       */
      inline bool IsEnd (void) const;
      /**
       * \return true if this iterator points to the start of the byte array.
       *     false otherwise.
       */
      inline bool IsStart (void) const;

      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by one byte.
       */
      inline void WriteU8 (uint8_t  data);
      /**
       * \param data data to write in buffer
       * \param len number of times data must be written in buffer
       *
       * Write the data in buffer len times and avance the iterator position
       * by len byte.
       */
      inline void WriteU8 (uint8_t data, uint32_t len);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by two bytes. The format of the data written in the byte
       * buffer is non-portable. We only ensure that readU16 will
       * return exactly what we wrote with writeU16 if the program
       * is run on the same machine.
       */
      inline void WriteU16 (uint16_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by four bytes. The format of the data written in the byte
       * buffer is non-portable. We only ensure that readU32 will
       * return exactly what we wrote with writeU32 if the program
       * is run on the same machine.
       */
      inline void WriteU32 (uint32_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by eight bytes. The format of the data written in the byte
       * buffer is non-portable. We only ensure that readU64 will
       * return exactly what we wrote with writeU64 if the program
       * is run on the same machine.
       */
      inline void WriteU64 (uint64_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by two bytes. The data is written in network order and the
       * input data is expected to be in host order.
       */
      inline void WriteHtonU16 (uint16_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by four bytes. The data is written in network order and the
       * input data is expected to be in host order.
       */
      inline void WriteHtonU32 (uint32_t data);
      /**
       * \param data data to write in buffer
       *
       * Write the data in buffer and avance the iterator position
       * by eight bytes. The data is written in network order and the
       * input data is expected to be in host order.
       */
      inline void WriteHtonU64 (uint64_t data);
      /**
       * \param buffer a byte buffer to copy in the internal buffer.
       * \param size number of bytes to copy.
       *
       * Write the data in buffer and avance the iterator position
       * by size bytes.
       */
      inline void Write (uint8_t const*buffer, uint16_t size);
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
      inline void Write (Iterator start, Iterator end);

      /**
       * \return the byte read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       */
      inline uint8_t  ReadU8 (void);
      /**
       * \return the two bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in the format written by writeU16.
       */
      inline uint16_t ReadU16 (void);
      /**
       * \return the four bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in the format written by writeU32.
       */
      inline uint32_t ReadU32 (void);
      /**
       * \return the eight bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in the format written by writeU64.
       */
      inline uint64_t ReadU64 (void);
      /**
       * \return the two bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in network format and return in host format.
       */
      inline uint16_t ReadNtohU16 (void);
      /**
       * \return the four bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in network format and return in host format.
       */
      inline uint32_t ReadNtohU32 (void);
      /**
       * \return the eight bytes read in the buffer.
       *
       * Read data and advance the Iterator by the number of bytes
       * read.
       * The data is read in network format and return in host format.
       */
      inline uint64_t ReadNtohU64 (void);
      /**
       * \param buffer buffer to copy data into
       * \param size number of bytes to copy
       *
       * Copy size bytes of data from the internal buffer to the
       * input buffer and avance the Iterator by the number of
       * bytes read.
       */
      inline void Read (uint8_t *buffer, uint16_t size);
  private:
      friend class Buffer;
      inline Iterator (Buffer const*buffer, uint32_t m_current);
      inline uint32_t GetIndex (uint32_t n);
      uint32_t m_zeroStart;
      uint32_t m_zeroEnd;
      uint32_t m_dataEnd;
      uint32_t m_current;
      uint8_t *m_data;
  };

  /**
   * \return the number of bytes stored in this buffer.
   */
  inline uint32_t GetSize (void) const;

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
  inline Buffer::Iterator Begin (void) const;
  /**
   * \return an Iterator which points to the
   * end of this Buffer.
   */
  inline Buffer::Iterator End (void) const;

  void TransformIntoRealBuffer (void) const;

  inline Buffer (Buffer const &o);
  inline Buffer &operator = (Buffer const &o);
  inline Buffer ();
  inline Buffer (uint32_t dataSize);
  inline ~Buffer ();
private:
  struct BufferData {
      uint32_t m_count;
      uint32_t m_size;
      uint32_t m_initialStart;
      uint32_t m_dirtyStart;
      uint32_t m_dirtySize;
      uint8_t m_data[1];
  };
  typedef std::vector<struct Buffer::BufferData*> BufferDataList;

  inline uint8_t *GetStart (void) const;
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

}; // namespace ns3


/**************************************************
   Start of implementation of methods which 
   need to be inline for performance reasons.
 *************************************************/

#include "ns3/assert.h"

namespace ns3 {

Buffer::Buffer ()
  : m_data (Buffer::Create ()),
    m_zeroAreaSize (0),
    m_start (m_maxTotalAddStart),
    m_size (0)
{
  if (m_start > m_data->m_size) 
    {
      m_start = 0;
    }
  NS_ASSERT (m_start <= m_data->m_size);
}

Buffer::Buffer (uint32_t dataSize)
  : m_data (Buffer::Create ()),
    m_zeroAreaSize (dataSize),
    m_start (m_maxTotalAddStart),
    m_size (0)
{
  if (m_start > m_data->m_size) 
    {
      m_start = 0;
    }
  NS_ASSERT (m_start <= m_data->m_size);
}


Buffer::Buffer (Buffer const&o)
  : m_data (o.m_data),
    m_zeroAreaSize (o.m_zeroAreaSize),
    m_start (o.m_start),
    m_size (o.m_size)
{
  m_data->m_count++;
  NS_ASSERT (m_start <= m_data->m_size);
}

Buffer &
Buffer::operator = (Buffer const&o)
{
  if (m_data != o.m_data) 
    {
      // not assignment to self.
      m_data->m_count--;
      if (m_data->m_count == 0) 
        {
          Recycle (m_data);
        }
      m_data = o.m_data;
      m_data->m_count++;
    }
  m_zeroAreaSize = o.m_zeroAreaSize;
  m_start = o.m_start;
  m_size = o.m_size;
  NS_ASSERT (m_start <= m_data->m_size);
  return *this;
}

Buffer::~Buffer ()
{
  m_data->m_count--;
  if (m_data->m_count == 0) 
    {
      Recycle (m_data);
    }
}


uint8_t *
Buffer::GetStart (void) const
{
  return m_data->m_data + m_start;
}

uint32_t 
Buffer::GetSize (void) const
{
  return m_size + m_zeroAreaSize;
}

Buffer::Iterator 
Buffer::Begin (void) const
{
  return Buffer::Iterator (this, 0);
}
Buffer::Iterator 
Buffer::End (void) const
{
  return Buffer::Iterator (this, GetSize ());
}


Buffer::Iterator::Iterator ()
  : m_zeroStart (0),
    m_zeroEnd (0),
    m_dataEnd (0),
    m_current (0),
    m_data (0)
{}
Buffer::Iterator::Iterator (Buffer const*buffer, uint32_t current)
  : m_zeroStart (buffer->m_data->m_initialStart-buffer->m_start),
    m_zeroEnd (m_zeroStart+buffer->m_zeroAreaSize),
    m_dataEnd (buffer->GetSize ()),
    m_current (current),
    m_data (buffer->m_data->m_data+buffer->m_start)
{}

void 
Buffer::Iterator::Next (void)
{
  NS_ASSERT (m_current + 1 <= m_dataEnd);
  m_current++;
}
void 
Buffer::Iterator::Prev (void)
{
  NS_ASSERT (m_current >= 1);
  m_current--;
}
void 
Buffer::Iterator::Next (uint32_t delta)
{
  NS_ASSERT (m_current + delta <= m_dataEnd);
  m_current += delta;
}
void 
Buffer::Iterator::Prev (uint32_t delta)
{
  NS_ASSERT (m_current >= delta);
  m_current -= delta;
}
int32_t
Buffer::Iterator::GetDistanceFrom (Iterator const &o) const
{
  NS_ASSERT (m_data == o.m_data);
  int32_t start = m_current;
  int32_t end = o.m_current;
  return end - start;
}

bool 
Buffer::Iterator::IsEnd (void) const
{
  return m_current == m_dataEnd;
}
bool 
Buffer::Iterator::IsStart (void) const
{
  return m_current == 0;
}

uint32_t
Buffer::Iterator::GetIndex (uint32_t n)
{
  NS_ASSERT ( 
      (m_current + n <= m_dataEnd) &&
      ((m_current + n <= m_zeroStart) ||
       (m_current >= m_zeroEnd) ||
       m_zeroStart == m_zeroEnd)
      );
  uint32_t index;
  if (m_current < m_zeroStart) 
    {
      index = m_current;
    } 
  else 
    {
      index = m_current - (m_zeroEnd-m_zeroStart);
    }
  return index;
}


void 
Buffer::Iterator::Write (Iterator start, Iterator end)
{
  NS_ASSERT (start.m_data == end.m_data);
  NS_ASSERT (start.m_current <= end.m_current);
  NS_ASSERT (start.m_zeroStart == end.m_zeroStart);
  NS_ASSERT (start.m_zeroEnd == end.m_zeroEnd);
  NS_ASSERT (m_data != start.m_data);
  uint32_t size = end.m_current - start.m_current;
  uint8_t *src = start.m_data + start.GetIndex (size);
  uint8_t *dest = m_data + GetIndex (size);
  memcpy (dest, src, size);
  m_current += size;
}

void 
Buffer::Iterator::WriteU8 (uint8_t  data, uint32_t len)
{
  uint8_t *current = m_data + GetIndex (len);
  memset (current, data, len);
  m_current += len;
}
void 
Buffer::Iterator::WriteU8  (uint8_t  data)
{
  m_data[GetIndex (1)] = data;
  m_current++;
}
void 
Buffer::Iterator::WriteU16 (uint16_t data)
{
  uint16_t *buffer = (uint16_t *)(m_data + GetIndex (2));
  *buffer = data;
  m_current += 2;
}
void 
Buffer::Iterator::WriteU32 (uint32_t data)
{
  uint32_t *buffer = (uint32_t *)(m_data + GetIndex (4));
  *buffer = data;
  m_current += 4;
}
void 
Buffer::Iterator::WriteU64 (uint64_t data)
{
  uint64_t *buffer = (uint64_t *)(m_data + GetIndex (8));
  *buffer = data;
  m_current += 8;
}
void 
Buffer::Iterator::WriteHtonU16 (uint16_t data)
{
  uint8_t *current = m_data + GetIndex (2);
  *(current+0) = (data >> 8) & 0xff;
  *(current+1) = (data >> 0) & 0xff;
  m_current += 2;
}
void 
Buffer::Iterator::WriteHtonU32 (uint32_t data)
{
  uint8_t *current = m_data + GetIndex (4);
  *(current+0) = (data >> 24) & 0xff;
  *(current+1) = (data >> 16) & 0xff;
  *(current+2) = (data >> 8) & 0xff;
  *(current+3) = (data >> 0) & 0xff;
  m_current += 4;
}
void 
Buffer::Iterator::WriteHtonU64 (uint64_t data)
{
  uint8_t *current = m_data + GetIndex (8);
  *(current+0) = (data >> 56) & 0xff;
  *(current+1) = (data >> 48) & 0xff;
  *(current+2) = (data >> 40) & 0xff;
  *(current+3) = (data >> 32) & 0xff;
  *(current+4) = (data >> 24) & 0xff;
  *(current+5) = (data >> 16) & 0xff;
  *(current+6) = (data >> 8) & 0xff;
  *(current+7) = (data >> 0) & 0xff;
  m_current += 8;
}
void 
Buffer::Iterator::Write (uint8_t const*buffer, uint16_t size)
{
  uint8_t *current = m_data + GetIndex (size);
  memcpy (current, buffer, size);
  m_current += size;
}

uint8_t  
Buffer::Iterator::ReadU8 (void)
{
  uint8_t data = m_data[GetIndex(1)];
  m_current++;
  return data;
}
uint16_t 
Buffer::Iterator::ReadU16 (void)
{
  uint16_t *buffer = reinterpret_cast<uint16_t *>(m_data + GetIndex (2));
  m_current += 2;
  return *buffer;
}
uint32_t 
Buffer::Iterator::ReadU32 (void)
{
  uint32_t *buffer = reinterpret_cast<uint32_t *>(m_data + GetIndex (4));
  m_current += 4;
  return *buffer;
}
uint64_t 
Buffer::Iterator::ReadU64 (void)
{
  uint64_t *buffer = reinterpret_cast<uint64_t *>(m_data + GetIndex (8));
  m_current += 8;
  return *buffer;
}
uint16_t 
Buffer::Iterator::ReadNtohU16 (void)
{
  uint8_t *current = m_data + GetIndex (2);
  uint16_t retval = 0;
  retval |= static_cast<uint16_t> (current[0]) << 8;
  retval |= static_cast<uint16_t> (current[1]) << 0;
  m_current += 2;
  return retval;
}
uint32_t 
Buffer::Iterator::ReadNtohU32 (void)
{
  uint8_t *current = m_data + GetIndex (4);
  uint32_t retval = 0;
  retval |= static_cast<uint32_t> (current[0]) << 24;
  retval |= static_cast<uint32_t> (current[1]) << 16;
  retval |= static_cast<uint32_t> (current[2]) << 8;
  retval |= static_cast<uint32_t> (current[3]) << 0;
  m_current += 4;
  return retval;
}
uint64_t 
Buffer::Iterator::ReadNtohU64 (void)
{
  uint8_t *current = m_data + GetIndex (8);
  uint64_t retval = 0;
  retval |= static_cast<uint64_t> (current[0]) << 56;
  retval |= static_cast<uint64_t> (current[1]) << 48;
  retval |= static_cast<uint64_t> (current[2]) << 40;
  retval |= static_cast<uint64_t> (current[3]) << 32;
  retval |= static_cast<uint64_t> (current[4]) << 24;
  retval |= static_cast<uint64_t> (current[5]) << 16;
  retval |= static_cast<uint64_t> (current[6]) << 8;
  retval |= static_cast<uint64_t> (current[7]) << 0;
  m_current += 8;
  return retval;
}
void 
Buffer::Iterator::Read (uint8_t *buffer, uint16_t size)
{
  uint8_t *current = m_data + GetIndex (size);
  memcpy (buffer, current, size);
  m_current += size;
}

}; // namespace ns3


#endif /* BUFFER_H */
