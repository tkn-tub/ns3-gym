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
#include "buffer.h"
#include "ns3/assert.h"
#include "ns3/debug.h"

#include <iostream>

NS_DEBUG_COMPONENT_DEFINE ("Buffer");

namespace ns3 {

Buffer::BufferDataList  Buffer::m_freeList;
uint32_t Buffer::m_maxTotalAddStart = 0;
uint32_t Buffer::m_maxTotalAddEnd = 0;

Buffer::BufferDataList::~BufferDataList ()
{
  for (BufferDataList::iterator i = begin ();
       i != end (); i++)
    {
      Buffer::Deallocate (*i);
    }
}

struct Buffer::BufferData *
Buffer::Allocate (uint32_t reqSize, uint32_t reqStart)
{
  if (reqSize == 0) 
    {
      reqSize = 1;
    }
  NS_ASSERT (reqSize >= 1);
  uint32_t size = reqSize - 1 + sizeof (struct Buffer::BufferData);
  uint8_t *b = new uint8_t [size];
  struct BufferData *data = reinterpret_cast<struct Buffer::BufferData*>(b);
  data->m_size = reqSize;
  data->m_initialStart = reqStart;
  data->m_dirtyStart = reqStart;
  data->m_dirtySize = 0;
  data->m_count = 1;
  return data;
}

void
Buffer::Deallocate (struct Buffer::BufferData *data)
{
  uint8_t *buf = reinterpret_cast<uint8_t *> (data);
  delete [] buf;
}
#ifdef USE_FREE_LIST
void
Buffer::Recycle (struct Buffer::BufferData *data)
{
  NS_ASSERT (data->m_count == 0);
  /* get rid of it if it is too small for later reuse. */
  if (data->m_size < (Buffer::m_maxTotalAddStart + Buffer::m_maxTotalAddEnd)) 
    {
      Buffer::Deallocate (data);
      return; 
    }
  /* feed into free list */
  if (Buffer::m_freeList.size () > 1000) 
    {
      Buffer::Deallocate (data);
    } 
  else 
    {
      Buffer::m_freeList.push_back (data);
    }
}

Buffer::BufferData *
Buffer::Create (void)
{
  /* try to find a buffer correctly sized. */
  while (!Buffer::m_freeList.empty ()) 
    {
      struct Buffer::BufferData *data = Buffer::m_freeList.back ();
      Buffer::m_freeList.pop_back ();
      if (data->m_size >= (m_maxTotalAddStart + m_maxTotalAddEnd)) 
        {
          data->m_initialStart = m_maxTotalAddStart;
          data->m_dirtyStart = m_maxTotalAddStart;
          data->m_dirtySize = 0;
          data->m_count = 1;
          return data;
        }
      Buffer::Deallocate (data);
    }
  struct Buffer::BufferData *data = Buffer::Allocate (m_maxTotalAddStart+m_maxTotalAddEnd,
                              m_maxTotalAddStart);
  NS_ASSERT (data->m_count == 1);
  return data;
}
#else
void
Buffer::Recycle (struct Buffer::BufferData *data)
{
  Buffer::Deallocate (data);
}

Buffer::BufferData *
Buffer::Create (void)
{
  return Buffer::Allocate (m_maxTotalAddStart+m_maxTotalAddEnd,
                           m_maxTotalAddStart);
}
#endif

}; // namespace ns3


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
  return Buffer::Iterator (this);
}
Buffer::Iterator 
Buffer::End (void) const
{
  return Buffer::Iterator (this, false);
}

Buffer::Iterator::Iterator ()
  : m_zeroStart (0),
    m_zeroEnd (0),
    m_dataEnd (0),
    m_current (0),
    m_data (0)
{}
Buffer::Iterator::Iterator (Buffer const*buffer)
{
  Construct (buffer);
  m_current = m_dataStart;
}
Buffer::Iterator::Iterator (Buffer const*buffer, bool dummy)
{
  Construct (buffer);
  m_current = m_dataEnd;
}

void
Buffer::Iterator::Construct (const Buffer *buffer)
{
  m_zeroStart = buffer->m_data->m_initialStart-buffer->m_start;
  m_zeroEnd = m_zeroStart+buffer->m_zeroAreaSize;
  m_dataStart = 0;
  m_dataEnd = buffer->GetSize ();
  m_data = buffer->m_data->m_data+buffer->m_start;
}

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
uint32_t
Buffer::Iterator::GetDistanceFrom (Iterator const &o) const
{
  NS_ASSERT (m_data == o.m_data);
  int32_t start = m_current;
  int32_t end = o.m_current;
  int32_t diff = end - start;
  if (diff < 0)
    {
      return -diff;
    }
  else
    {
      return diff;
    }
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
Buffer::Iterator::Write (uint8_t const*buffer, uint32_t size)
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

void 
Buffer::AddAtStart (uint32_t start)
{
  NS_ASSERT (m_start <= m_data->m_initialStart);
  bool isDirty = m_data->m_count > 1 && m_start > m_data->m_dirtyStart;
  if (m_start >= start && !isDirty) 
    {
      /* enough space in the buffer and not dirty. */
      m_start -= start;
      m_size += start;
    } 
  else if (m_size + start <= m_data->m_size && !isDirty) 
    {
      /* enough space but need to move data around to fit new data */
      memmove (m_data->m_data + start, GetStart (), m_size);
      NS_ASSERT (start > m_start);
      m_data->m_initialStart += start - m_start;
      m_start = 0;
      m_size += start;
    } 
  else if (m_start < start) 
    {
      /* not enough space in buffer */
      uint32_t newSize = m_size + start;
      struct Buffer::BufferData *newData = Buffer::Allocate (newSize, 0);
      memcpy (newData->m_data + start, GetStart (), m_size);
      newData->m_initialStart = m_data->m_initialStart + start;
      m_data->m_count--;
      if (m_data->m_count == 0) 
        {
          Buffer::Deallocate (m_data);
        }
      m_data = newData;
      m_start = 0;
      m_size = newSize;
    } 
  else 
    {
      /* enough space in the buffer but it is dirty ! */
      NS_ASSERT (isDirty);
      struct Buffer::BufferData *newData = Buffer::Create ();
      memcpy (newData->m_data + m_start, GetStart (), m_size);
      newData->m_initialStart = m_data->m_initialStart;
      m_data->m_count--;
      if (m_data->m_count == 0) 
        {
          Recycle (m_data);
        }
      m_data = newData;
      m_start -= start;
      m_size += start;
    } 
  // update dirty area
  m_data->m_dirtyStart = m_start;
  m_data->m_dirtySize = m_size;
  // update m_maxTotalAddStart
  uint32_t addedAtStart;
  if (m_data->m_initialStart > m_start) 
    {
      addedAtStart = m_data->m_initialStart - m_start;
    } 
  else 
    {
      addedAtStart = 0;
    }
  if (addedAtStart > m_maxTotalAddStart) 
    {
      m_maxTotalAddStart = addedAtStart;
    }
  NS_DEBUG ("start add="<<start<<", start="<<m_start<<", size="<<m_size<<", zero="<<m_zeroAreaSize<<
            ", real size="<<m_data->m_size<<", ini start="<<m_data->m_initialStart<<
            ", dirty start="<<m_data->m_dirtyStart<<", dirty size="<<m_data->m_dirtySize); 
}
void 
Buffer::AddAtEnd (uint32_t end)
{
  NS_ASSERT (m_start <= m_data->m_initialStart);
  bool isDirty = m_data->m_count > 1 &&
      m_start + m_size < m_data->m_dirtyStart + m_data->m_dirtySize;
  if (m_start + m_size + end <= m_data->m_size && !isDirty) 
    {
      /* enough space in buffer and not dirty */
      m_size += end;
    } 
  else if (m_size + end <= m_data->m_size && !isDirty) 
    {
      /* enough space but need to move data around to fit the extra data */
      uint32_t newStart = m_data->m_size - (m_size + end);
      memmove (m_data->m_data + newStart, GetStart (), m_size);
      NS_ASSERT (newStart < m_start);
      m_data->m_initialStart -= m_start - newStart;
      m_start = newStart;
      m_size += end;
    } 
  else if (m_start + m_size + end > m_data->m_size) 
    {
      /* not enough space in buffer */
      uint32_t newSize = m_size + end;
      struct Buffer::BufferData *newData = Buffer::Allocate (newSize, 0);
      memcpy (newData->m_data, GetStart (), m_size);
      newData->m_initialStart = m_data->m_initialStart - m_start;
      m_data->m_count--;
      if (m_data->m_count == 0) 
        {
          Buffer::Deallocate (m_data);
        }
      m_data = newData;
      m_size = newSize;
      m_start = 0;
    } 
  else 
    {
      /* enough space in the buffer but it is dirty ! */
      NS_ASSERT (isDirty);
      struct Buffer::BufferData *newData = Buffer::Create ();
      memcpy (newData->m_data + m_start, GetStart (), m_size);
      newData->m_initialStart = m_data->m_initialStart;
      m_data->m_count--;
      if (m_data->m_count == 0) 
        {
          Recycle (m_data);
        }
      m_data = newData;
      m_size += end;
    } 
  // update dirty area
  m_data->m_dirtyStart = m_start;
  m_data->m_dirtySize = m_size;
  // update m_maxTotalAddEnd
  uint32_t endLoc = m_start + m_size;
  uint32_t addedAtEnd;
  if (m_data->m_initialStart < endLoc) 
    {
      addedAtEnd = endLoc - m_data->m_initialStart;
    } 
  else 
    {
      addedAtEnd = 0;
    }
  if (addedAtEnd > m_maxTotalAddEnd) 
    {
      m_maxTotalAddEnd = addedAtEnd;
    }
  NS_DEBUG ("end add="<<end<<", start="<<m_start<<", size="<<m_size<<", zero="<<m_zeroAreaSize<<
            ", real size="<<m_data->m_size<<", ini start="<<m_data->m_initialStart<<
            ", dirty start="<<m_data->m_dirtyStart<<", dirty size="<<m_data->m_dirtySize); 
}

void 
Buffer::RemoveAtStart (uint32_t start)
{
  if (m_zeroAreaSize == 0) 
    {
      if (m_size <= start) 
        {
          m_start += m_size;
          m_size = 0;
        } 
      else 
        {
          m_start += start;
          m_size -= start;
        }
    } 
  else 
    {
      NS_ASSERT (m_data->m_initialStart >= m_start);
      uint32_t zeroStart = m_data->m_initialStart - m_start;
      uint32_t zeroEnd = zeroStart + m_zeroAreaSize;
      uint32_t dataEnd = m_size + m_zeroAreaSize;
      if (start <= zeroStart) 
        {
          /* only remove start of buffer */
          m_start += start;
          m_size -= start;
        } 
      else if (start <= zeroEnd) 
        {
          /* remove start of buffer _and_ start of zero area */
          m_start += zeroStart;
          uint32_t zeroDelta = start - zeroStart;
          m_zeroAreaSize -= zeroDelta;
          NS_ASSERT (zeroDelta <= start);
          m_size -= zeroStart;
        } 
      else if (start <= dataEnd) 
        {
          /* remove start of buffer, complete zero area, and part
           * of end of buffer */
          m_start += start - m_zeroAreaSize;
          m_size -= start - m_zeroAreaSize;
          m_zeroAreaSize = 0;
        } 
      else 
        {
          /* remove all buffer */
          m_start += m_size;
          m_size = 0;
          m_zeroAreaSize = 0;
        }
    }
  NS_DEBUG ("start remove="<<start<<", start="<<m_start<<", size="<<m_size<<
            ", zero="<<m_zeroAreaSize<<
            ", real size="<<m_data->m_size<<", ini start="<<m_data->m_initialStart<<
            ", dirty start="<<m_data->m_dirtyStart<<", dirty size="<<m_data->m_dirtySize); 
}
void 
Buffer::RemoveAtEnd (uint32_t end)
{
  if (m_zeroAreaSize == 0) 
    {
      if (m_size <= end) 
        {
          m_size = 0;
        } 
      else 
        {
          m_size -= end;
        } 
    } 
  else 
    {
      NS_ASSERT (m_data->m_initialStart >= m_start);
      uint32_t zeroStart = m_data->m_initialStart - m_start;
      uint32_t zeroEnd = zeroStart + m_zeroAreaSize;
      uint32_t dataEnd = m_size + m_zeroAreaSize;
      NS_ASSERT (zeroStart <= m_size);
      NS_ASSERT (zeroEnd <= m_size + m_zeroAreaSize);
      if (dataEnd <= end) 
        {
          /* remove all buffer */
          m_zeroAreaSize = 0;
          m_start += m_size;
          m_size = 0;
        } 
      else if (dataEnd - zeroStart <= end) 
        {
          /* remove end of buffer, zero area, part of start of buffer */
          NS_ASSERT (end >= m_zeroAreaSize);
          m_size -= end - m_zeroAreaSize;
          m_zeroAreaSize = 0;
        } 
      else if (dataEnd - zeroEnd <= end) 
        {
          /* remove end of buffer, part of zero area */
          uint32_t zeroDelta = end - (dataEnd - zeroEnd);
          m_zeroAreaSize -= zeroDelta;
          m_size -= end - zeroDelta;
        } 
      else 
        {
          /* remove part of end of buffer */
          m_size -= end;
        }
    }
  NS_DEBUG ("end remove="<<end<<", start="<<m_start<<", size="<<m_size<<", zero="<<m_zeroAreaSize<<
            ", real size="<<m_data->m_size<<", ini start="<<m_data->m_initialStart<<
            ", dirty start="<<m_data->m_dirtyStart<<", dirty size="<<m_data->m_dirtySize); 
}

Buffer 
Buffer::CreateFragment (uint32_t start, uint32_t length) const
{
  uint32_t zeroStart = m_data->m_initialStart - m_start;
  uint32_t zeroEnd = zeroStart + m_zeroAreaSize;
  if (m_zeroAreaSize != 0 &&
      start + length > zeroStart &&
      start <= zeroEnd) 
    {
      TransformIntoRealBuffer ();
    }
  Buffer tmp = *this;
  tmp.RemoveAtStart (start);
  tmp.RemoveAtEnd (GetSize () - (start + length));
  return tmp;
}

Buffer 
Buffer::CreateFullCopy (void) const
{
  if (m_zeroAreaSize != 0) 
    {
      NS_ASSERT (m_data->m_initialStart >= m_start);
      NS_ASSERT (m_size >= (m_data->m_initialStart - m_start));
      Buffer tmp;
      tmp.AddAtStart (m_zeroAreaSize);
      tmp.Begin ().WriteU8 (0, m_zeroAreaSize);
      uint32_t dataStart = m_data->m_initialStart - m_start;
      tmp.AddAtStart (dataStart);
      tmp.Begin ().Write (m_data->m_data+m_start, dataStart);
      uint32_t dataEnd = m_size - (m_data->m_initialStart - m_start);
      tmp.AddAtEnd (dataEnd);
      Buffer::Iterator i = tmp.End ();
      i.Prev (dataEnd);
      i.Write (m_data->m_data+m_data->m_initialStart,dataEnd);
      return tmp;
    }
  return *this;
}

void
Buffer::TransformIntoRealBuffer (void) const
{
  Buffer tmp = CreateFullCopy ();
  *const_cast<Buffer *> (this) = tmp;
}


uint8_t const*
Buffer::PeekData (void) const
{
  TransformIntoRealBuffer ();
  return m_data->m_data + m_start;
}


} // namespace ns3


#ifdef RUN_SELF_TESTS

#include "ns3/test.h"
#include "ns3/random-variable.h"
#include <iomanip>

namespace ns3 {

class BufferTest: public Test {
private:
  bool EnsureWrittenBytes (Buffer b, uint32_t n, uint8_t array[]);
public:
  virtual bool RunTests (void);
  BufferTest ();
};


BufferTest::BufferTest ()
  : Test ("Buffer") {}

bool
BufferTest::EnsureWrittenBytes (Buffer b, uint32_t n, uint8_t array[])
{
  bool success = true;
  uint8_t *expected = array;
  uint8_t const*got;
  got = b.PeekData ();
  for (uint32_t j = 0; j < n; j++) 
    {
      if (got[j] != expected[j]) 
        {
          success = false;
        }
    }
  if (!success) 
    {
      Failure () << "Buffer -- ";
      Failure () << "expected: n=";
      Failure () << n << ", ";
      Failure ().setf (std::ios::hex, std::ios::basefield);
      for (uint32_t j = 0; j < n; j++) 
        {
          Failure () << (uint16_t)expected[j] << " ";
        }
      Failure ().setf (std::ios::dec, std::ios::basefield);
      Failure () << "got: ";
      Failure ().setf (std::ios::hex, std::ios::basefield);
      for (uint32_t j = 0; j < n; j++) 
        {
          Failure () << (uint16_t)got[j] << " ";
        }
      Failure () << std::endl;
    }
  return success;
}

/* Note: works only when variadic macros are
 * available which is the case for gcc.
 * XXX
 */
#define ENSURE_WRITTEN_BYTES(buffer, n, ...)     \
  {                                              \
  uint8_t bytes[] = {__VA_ARGS__};             \
  if (!EnsureWrittenBytes (buffer, n , bytes)) \
    {                                          \
      result = false;                          \
    }                                          \
  }

bool
BufferTest::RunTests (void)
{
  bool result = true;
  Buffer buffer;
  Buffer::Iterator i;
  buffer.AddAtStart (6);
  i = buffer.Begin ();
  i.WriteU8 (0x66);
  ENSURE_WRITTEN_BYTES (buffer, 1, 0x66);
  i = buffer.Begin ();
  i.WriteU8 (0x67);
  ENSURE_WRITTEN_BYTES (buffer, 1, 0x67);
  i.WriteHtonU16 (0x6568);
  i = buffer.Begin ();
  ENSURE_WRITTEN_BYTES (buffer, 3, 0x67, 0x65, 0x68);
  i.WriteHtonU16 (0x6369);
  ENSURE_WRITTEN_BYTES (buffer, 3, 0x63, 0x69, 0x68);
  i.WriteHtonU32 (0xdeadbeaf);
  ENSURE_WRITTEN_BYTES (buffer, 6, 0x63, 0x69, 0xde, 0xad, 0xbe, 0xaf);
  buffer.AddAtStart (2);
  i = buffer.Begin ();
  i.WriteU16 (0);
  ENSURE_WRITTEN_BYTES (buffer, 8, 0, 0, 0x63, 0x69, 0xde, 0xad, 0xbe, 0xaf);
  buffer.AddAtEnd (2);
  i = buffer.Begin ();
  i.Next (8);
  i.WriteU16 (0);
  ENSURE_WRITTEN_BYTES (buffer, 10, 0, 0, 0x63, 0x69, 0xde, 0xad, 0xbe, 0xaf, 0, 0);
  buffer.RemoveAtStart (3);
  i = buffer.Begin ();
  ENSURE_WRITTEN_BYTES (buffer, 7, 0x69, 0xde, 0xad, 0xbe, 0xaf, 0, 0);
  buffer.RemoveAtEnd (4);
  i = buffer.Begin ();
  ENSURE_WRITTEN_BYTES (buffer, 3, 0x69, 0xde, 0xad);
  buffer.AddAtStart (1);
  i = buffer.Begin ();
  i.WriteU8 (0xff);
  ENSURE_WRITTEN_BYTES (buffer, 4, 0xff, 0x69, 0xde, 0xad);
  buffer.AddAtEnd (1);
  i = buffer.Begin ();
  i.Next (4);
  i.WriteU8 (0xff);
  i.Prev (2);
  uint16_t saved = i.ReadU16 ();
  i.Prev (2);
  i.WriteHtonU16 (0xff00);
  i.Prev (2);
  if (i.ReadNtohU16 () != 0xff00) 
    {
      result = false;
    }
  i.Prev (2);
  i.WriteU16 (saved);
  ENSURE_WRITTEN_BYTES (buffer, 5, 0xff, 0x69, 0xde, 0xad, 0xff);
  Buffer o = buffer;
  ENSURE_WRITTEN_BYTES (o, 5, 0xff, 0x69, 0xde, 0xad, 0xff);
  o.AddAtStart (1);
  i = o.Begin ();
  i.WriteU8 (0xfe);
  ENSURE_WRITTEN_BYTES (o, 6, 0xfe, 0xff, 0x69, 0xde, 0xad, 0xff);
  buffer.AddAtStart (2);
  i = buffer.Begin ();
  i.WriteU8 (0xfd);
  i.WriteU8 (0xfd);
  ENSURE_WRITTEN_BYTES (o, 6, 0xfe, 0xff, 0x69, 0xde, 0xad, 0xff);
  ENSURE_WRITTEN_BYTES (buffer, 7, 0xfd, 0xfd, 0xff, 0x69, 0xde, 0xad, 0xff);

  // test self-assignment
  {
      Buffer a = o;
      a = a;
  }

  // test Remove start.
  buffer = Buffer (5);
  ENSURE_WRITTEN_BYTES (buffer, 5, 0, 0, 0, 0, 0);
  buffer.RemoveAtStart (1);
  ENSURE_WRITTEN_BYTES (buffer, 4, 0, 0, 0, 0);
  buffer.AddAtStart (1);
  buffer.Begin ().WriteU8 (0xff);
  ENSURE_WRITTEN_BYTES (buffer, 5, 0xff, 0, 0, 0, 0);
  buffer.RemoveAtStart(3);
  ENSURE_WRITTEN_BYTES (buffer, 2, 0, 0);
  buffer.AddAtStart (4);
  buffer.Begin ().WriteHtonU32 (0xdeadbeaf);
  ENSURE_WRITTEN_BYTES (buffer, 6,  0xde, 0xad, 0xbe, 0xaf, 0, 0);
  buffer.RemoveAtStart (2);
  ENSURE_WRITTEN_BYTES (buffer, 4,  0xbe, 0xaf, 0, 0);
  buffer.AddAtEnd (4);
  i = buffer.Begin ();
  i.Next (4);
  i.WriteHtonU32 (0xdeadbeaf);
  ENSURE_WRITTEN_BYTES (buffer, 8,  0xbe, 0xaf, 0, 0, 0xde, 0xad, 0xbe, 0xaf);
  buffer.RemoveAtStart (5);
  ENSURE_WRITTEN_BYTES (buffer, 3,  0xad, 0xbe, 0xaf);
  // test Remove end
  buffer = Buffer (5);
  ENSURE_WRITTEN_BYTES (buffer, 5, 0, 0, 0, 0, 0);
  buffer.RemoveAtEnd (1);
  ENSURE_WRITTEN_BYTES (buffer, 4, 0, 0, 0, 0);
  buffer.AddAtEnd (2);
  i = buffer.Begin ();
  i.Next (4);
  i.WriteU8 (0xab);
  i.WriteU8 (0xac);
  ENSURE_WRITTEN_BYTES (buffer, 6, 0, 0, 0, 0, 0xab, 0xac);
  buffer.RemoveAtEnd (1);
  ENSURE_WRITTEN_BYTES (buffer, 5, 0, 0, 0, 0, 0xab);
  buffer.RemoveAtEnd (3);
  ENSURE_WRITTEN_BYTES (buffer, 2, 0, 0);
  buffer.AddAtEnd (6);
  i = buffer.Begin ();
  i.Next (2);
  i.WriteU8 (0xac);
  i.WriteU8 (0xad);
  i.WriteU8 (0xae);
  i.WriteU8 (0xaf);
  i.WriteU8 (0xba);
  i.WriteU8 (0xbb);
  ENSURE_WRITTEN_BYTES (buffer, 8, 0, 0, 0xac, 0xad, 0xae, 0xaf, 0xba, 0xbb);
  buffer.AddAtStart (3);
  i = buffer.Begin ();
  i.WriteU8 (0x30);
  i.WriteU8 (0x31);
  i.WriteU8 (0x32);
  ENSURE_WRITTEN_BYTES (buffer, 11, 0x30, 0x31, 0x32, 0, 0, 0xac, 0xad, 0xae, 0xaf, 0xba, 0xbb);
  buffer.RemoveAtEnd (9);
  ENSURE_WRITTEN_BYTES (buffer, 2, 0x30, 0x31);
  buffer = Buffer (3);
  buffer.AddAtEnd (2);
  i = buffer.Begin ();
  i.Next (3);
  i.WriteHtonU16 (0xabcd);
  buffer.AddAtStart (1);
  buffer.Begin ().WriteU8 (0x21);
  ENSURE_WRITTEN_BYTES (buffer, 6, 0x21, 0, 0, 0, 0xab, 0xcd);
  buffer.RemoveAtEnd (8);
  if (buffer.GetSize () != 0) 
    {
      result = false;
    }

  buffer = Buffer (6);
  buffer.AddAtStart (9);
  buffer.AddAtEnd (3);
  i = buffer.End ();
  i.Prev (1);
  i.WriteU8 (1, 1);

  buffer = Buffer (6);
  buffer.AddAtStart (3);
  buffer.RemoveAtEnd (8);
  buffer.AddAtEnd (4);
  i = buffer.End ();
  i.Prev (4);
  i.WriteU8 (1, 4);

  buffer = Buffer (1);
  buffer.AddAtEnd (100);
  i = buffer.End ();
  i.Prev (100);
  i.WriteU8 (1, 100);

  // Bug #54
  {
    const uint32_t actualSize = 72602;
    const uint32_t chunkSize = 67624;
    UniformVariable bytesRng (0, 256);

    Buffer inputBuffer;
    Buffer outputBuffer;
    
    inputBuffer.AddAtEnd (actualSize);
    {
      Buffer::Iterator iter = inputBuffer.Begin ();
      for (uint32_t i = 0; i < actualSize; i++)
        iter.WriteU8 (static_cast<uint8_t> (bytesRng.GetValue ()));
    }

    outputBuffer.AddAtEnd (chunkSize);
    Buffer::Iterator iter = outputBuffer.End ();
    iter.Prev (chunkSize);
    iter.Write (inputBuffer.PeekData (), chunkSize);

    NS_TEST_ASSERT (memcmp (inputBuffer.PeekData (), outputBuffer.PeekData (), chunkSize) == 0);
  }

  return result;
}



static BufferTest gBufferTest;

} // namespace ns3

#endif /* RUN_SELF_TESTS */


