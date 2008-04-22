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
#include "ns3/log.h"
#include <iostream>

NS_LOG_COMPONENT_DEFINE ("Buffer");

#define LOG_INTERNAL_STATE(y)                                                                    \
NS_LOG_LOGIC (y << "start="<<m_start<<", end="<<m_end<<", zero start="<<m_zeroAreaStart<<              \
          ", zero end="<<m_zeroAreaEnd<<", count="<<m_data->m_count<<", size="<<m_data->m_size<<   \
          ", dirty start="<<m_data->m_dirtyStart<<", dirty end="<<m_data->m_dirtyEnd)

#ifdef BUFFER_HEURISTICS
#define HEURISTICS(x) x
#else
#define HEURISTICS(x)
#endif

//#define PRINT_STATS 1

namespace ns3 {

/**
 * This data structure is variable-sized through its last member whose size
 * is determined at allocation time and stored in the m_size field.
 *
 * The so-called "dirty area" describes the area in the buffer which
 * has been reserved and used by a user. Multiple Buffer instances
 * may reference the same BufferData object instance and may
 * reference different parts of the underlying byte buffer. The
 * "dirty area" is union of all the areas referenced by the Buffer
 * instances which reference the same BufferData instance.
 * New user data can be safely written only outside of the "dirty
 * area" if the reference count is higher than 1 (that is, if
 * more than one Buffer instance references the same BufferData).
 */
struct BufferData {
  /* The reference count of an instance of this data structure.
   * Each buffer which references an instance holds a count.
   */
  uint32_t m_count;
  /* the size of the m_data field below.
   */
  uint32_t m_size;
  /* offset from the start of the m_data field below to the
   * start of the area in which user bytes were written.
   */
  uint32_t m_dirtyStart;
  /* offset from the start of the m_data field below to the
   * end of the area in which user bytes were written.
   */
  uint32_t m_dirtyEnd;
  /* The real data buffer holds _at least_ one byte.
   * Its real size is stored in the m_size field.
   */
  uint8_t m_data[1];
};
class BufferDataList : public std::vector<struct BufferData*>
{
public:
  ~BufferDataList ();
};

static struct BufferData *BufferAllocate (uint32_t reqSize);

static void BufferDeallocate (struct BufferData *data);


} // namespace ns3

namespace ns3 {

#ifdef BUFFER_HEURISTICS
static uint32_t g_recommendedStart = 0;
static uint64_t g_nAddNoRealloc = 0;
static uint64_t g_nAddRealloc = 0;
static BufferDataList  g_freeList;
static uint32_t g_maxSize = 0;
static uint64_t g_nAllocs = 0;
static uint64_t g_nCreates = 0;
#endif /* BUFFER_HEURISTICS */

BufferDataList::~BufferDataList ()
{
#ifdef PRINT_STATS
#ifdef BUFFER_HEURISTICS
  double efficiency;
  efficiency = g_nAllocs;
  efficiency /= g_nCreates;
  std::cout <<"buffer free list efficiency="<<efficiency<<" (lower is better)" << std::endl;
  std::cout <<"buffer free list max size="<<g_maxSize<<std::endl;
  std::cout <<"buffer free list recommended start="<<g_recommendedStart<<std::endl;
  double addEfficiency;
  addEfficiency = g_nAddRealloc;
  addEfficiency /= g_nAddNoRealloc;
  std::cout <<"buffer add efficiency=" << addEfficiency << " (lower is better)"<<std::endl;
  //std::cout <<"n add reallocs="<< g_nAddRealloc << std::endl;
  //std::cout <<"n add no reallocs="<< g_nAddNoRealloc << std::endl;
#endif /* BUFFER_HEURISTICS */
#endif /* PRINT_STATS */
  for (BufferDataList::iterator i = begin ();
       i != end (); i++)
    {
      BufferDeallocate (*i);
    }
}

struct BufferData *
BufferAllocate (uint32_t reqSize)
{
  if (reqSize == 0) 
    {
      reqSize = 1;
    }
  NS_ASSERT (reqSize >= 1);
  uint32_t size = reqSize - 1 + sizeof (struct BufferData);
  uint8_t *b = new uint8_t [size];
  struct BufferData *data = reinterpret_cast<struct BufferData*>(b);
  data->m_size = reqSize;
  data->m_count = 1;
  return data;
}

void
BufferDeallocate (struct BufferData *data)
{
  NS_ASSERT (data->m_count == 0);
  uint8_t *buf = reinterpret_cast<uint8_t *> (data);
  delete [] buf;
}
#ifdef BUFFER_HEURISTICS
void
Buffer::Recycle (struct BufferData *data)
{
  NS_ASSERT (data->m_count == 0);
  g_maxSize = std::max (g_maxSize, data->m_size);
  /* feed into free list */
  if (data->m_size < g_maxSize ||
      g_freeList.size () > 1000)
    {
      BufferDeallocate (data);
    }
  else
    {
      g_freeList.push_back (data);
    }
}

BufferData *
Buffer::Create (uint32_t dataSize)
{
  /* try to find a buffer correctly sized. */
  g_nCreates++;
  while (!g_freeList.empty ()) 
    {
      struct BufferData *data = g_freeList.back ();
      g_freeList.pop_back ();
      if (data->m_size >= dataSize) 
        {
          data->m_count = 1;
          return data;
        }
      BufferDeallocate (data);
    }
  g_nAllocs++;
  struct BufferData *data = BufferAllocate (dataSize);
  NS_ASSERT (data->m_count == 1);
  return data;
}
#else
void
Buffer::Recycle (struct BufferData *data)
{
  NS_ASSERT (data->m_count == 0);
  BufferDeallocate (data);
}

BufferData *
Buffer::Create (uint32_t size)
{
  return BufferAllocate (size);
}
#endif

Buffer::Buffer ()
{
  Initialize (0);
}

Buffer::Buffer (uint32_t dataSize)
{
  Initialize (dataSize);
}

bool
Buffer::CheckInternalState (void) const
{
  bool offsetsOk = 
    m_start <= m_zeroAreaStart &&
    m_zeroAreaStart <= m_zeroAreaEnd &&
    m_zeroAreaEnd <= m_end;
  bool dirtyOk =
    m_start >= m_data->m_dirtyStart &&
    m_end <= m_data->m_dirtyEnd;
  bool internalSizeOk = m_end - (m_zeroAreaEnd - m_zeroAreaStart) <= m_data->m_size &&
    m_start <= m_data->m_size &&
    m_zeroAreaStart <= m_data->m_size;

  NS_ASSERT (offsetsOk);
  NS_ASSERT (dirtyOk);
  NS_ASSERT (internalSizeOk);

  return m_data->m_count > 0 && offsetsOk && dirtyOk && 
    internalSizeOk;
}

void
Buffer::Initialize (uint32_t zeroSize)
{
  m_data = Buffer::Create (0);
#ifdef BUFFER_HEURISTICS
  m_start = std::min (m_data->m_size, g_recommendedStart);
  m_maxZeroAreaStart = m_start;
#else
  m_start = 0;
#endif /* BUFFER_HEURISTICS */
  m_zeroAreaStart = m_start;
  m_zeroAreaEnd = m_zeroAreaStart + zeroSize;
  m_end = m_zeroAreaEnd;
  m_data->m_dirtyStart = m_start;
  m_data->m_dirtyEnd = m_end;
  NS_ASSERT (CheckInternalState ());
}

Buffer::Buffer (Buffer const&o)
  : m_data (o.m_data),
#ifdef BUFFER_HEURISTICS
    m_maxZeroAreaStart (o.m_zeroAreaStart),
#endif
    m_zeroAreaStart (o.m_zeroAreaStart),
    m_zeroAreaEnd (o.m_zeroAreaEnd),
    m_start (o.m_start),
    m_end (o.m_end)
{
  m_data->m_count++;
  NS_ASSERT (CheckInternalState ());
}

Buffer &
Buffer::operator = (Buffer const&o)
{
  NS_ASSERT (CheckInternalState ());
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
  HEURISTICS (
  g_recommendedStart = std::max (g_recommendedStart, m_maxZeroAreaStart);
  m_maxZeroAreaStart = o.m_maxZeroAreaStart;
  );
  m_zeroAreaStart = o.m_zeroAreaStart;
  m_zeroAreaEnd = o.m_zeroAreaEnd;
  m_start = o.m_start;
  m_end = o.m_end;
  NS_ASSERT (CheckInternalState ());
  return *this;
}

Buffer::~Buffer ()
{
  HEURISTICS (g_recommendedStart = std::max (g_recommendedStart, m_maxZeroAreaStart));
  m_data->m_count--;
  if (m_data->m_count == 0) 
    {
      Recycle (m_data);
    }
}

uint32_t 
Buffer::GetSize (void) const
{
  NS_ASSERT (CheckInternalState ());
  return m_end - m_start;
}

Buffer::Iterator 
Buffer::Begin (void) const
{
  NS_ASSERT (CheckInternalState ());
  return Buffer::Iterator (this);
}
Buffer::Iterator 
Buffer::End (void) const
{
  NS_ASSERT (CheckInternalState ());
  return Buffer::Iterator (this, false);
}

uint32_t
Buffer::GetInternalSize (void) const
{
  return m_zeroAreaStart - m_start + m_end - m_zeroAreaEnd;
}
uint32_t
Buffer::GetInternalEnd (void) const
{
  return m_end - (m_zeroAreaEnd - m_zeroAreaStart);
}

void 
Buffer::AddAtStart (uint32_t start)
{
  NS_ASSERT (CheckInternalState ());
  bool isDirty = m_data->m_count > 1 && m_start > m_data->m_dirtyStart;
  if (m_start >= start && !isDirty)
    {
      /* enough space in the buffer and not dirty. 
       * To add: |..|
       * Before: |*****---------***|
       * After:  |***..---------***|
       */
      NS_ASSERT (m_data->m_count == 1 || m_start == m_data->m_dirtyStart);
      m_start -= start;
      HEURISTICS (g_nAddNoRealloc++);
    } 
#if 0
  // the following is an optimization
  else if (m_start >= start)
    {
      struct BufferData *newData = Buffer::Create (m_data->m_size);
      memcpy (newData->m_data + m_start, m_data->m_data + m_start, GetInternalSize ());
      m_data->m_count--;
      if (m_data->m_count == 0)
        {
          Buffer::Recycle (m_data);
        }
      m_data = newData;

      m_start -= start;
      HEURISTICS (g_nAddRealloc++);
    }
  else
    {
      NS_ASSERT (m_start < start);
#else
  else
    {
#endif
      uint32_t newSize = GetInternalSize () + start;
      struct BufferData *newData = Buffer::Create (newSize);
      memcpy (newData->m_data + start, m_data->m_data + m_start, GetInternalSize ());
      m_data->m_count--;
      if (m_data->m_count == 0)
        {
          Buffer::Recycle (m_data);
        }
      m_data = newData;

      int32_t delta = start - m_start;
      m_start = 0;
      m_zeroAreaStart += delta;
      m_zeroAreaEnd += delta;
      m_end += delta;

      HEURISTICS (g_nAddRealloc++);
    }
  HEURISTICS (m_maxZeroAreaStart = std::max (m_maxZeroAreaStart, m_zeroAreaStart));
  // update dirty area
  m_data->m_dirtyStart = m_start;
  m_data->m_dirtyEnd = m_end;
  LOG_INTERNAL_STATE ("add start=" << start << ", ");
  NS_ASSERT (CheckInternalState ());
}
void 
Buffer::AddAtEnd (uint32_t end)
{
  NS_ASSERT (CheckInternalState ());
  bool isDirty = m_data->m_count > 1 && m_end < m_data->m_dirtyEnd;
  if (GetInternalEnd () + end <= m_data->m_size && !isDirty)
    {
      /* enough space in buffer and not dirty
       * Add:    |...|
       * Before: |**----*****|
       * After:  |**----...**|
       */
      NS_ASSERT (m_data->m_count == 1 || m_end == m_data->m_dirtyEnd);
      m_end += end;

      HEURISTICS (g_nAddNoRealloc++);
    } 
#if 0
  // this is an optimization
  else if (GetInternalEnd () + end > m_data->m_size)
    {
      struct BufferData *newData = Buffer::Create (newSize);
      memcpy (newData->m_data + m_start, m_data->m_data + m_start, GetInternalSize ());
      m_data->m_count--;
      if (m_data->m_count == 0) 
        {
          Buffer::Recycle (m_data);
        }
      m_data = newData;

      m_end += end;
      HEURISTICS (g_nAddRealloc++);
    }
#endif
  else
    {
      uint32_t newSize = GetInternalSize () + end;
      struct BufferData *newData = Buffer::Create (newSize);
      memcpy (newData->m_data, m_data->m_data + m_start, GetInternalSize ());
      m_data->m_count--;
      if (m_data->m_count == 0) 
        {
          Buffer::Recycle (m_data);
        }
      m_data = newData;


      m_zeroAreaStart -= m_start;
      m_zeroAreaEnd -= m_start;
      m_end -= m_start;
      m_start = 0;

      m_end += end;

      HEURISTICS (g_nAddRealloc++);
    } 
  HEURISTICS (m_maxZeroAreaStart = std::max (m_maxZeroAreaStart, m_zeroAreaStart));
  // update dirty area
  m_data->m_dirtyStart = m_start;
  m_data->m_dirtyEnd = m_end;
  LOG_INTERNAL_STATE ("add end=" << end << ", ");
  NS_ASSERT (CheckInternalState ());
}

void 
Buffer::AddAtEnd (const Buffer &o)
{
  if (m_end == m_zeroAreaEnd &&
      o.m_start == o.m_zeroAreaStart &&
      o.m_zeroAreaEnd - o.m_zeroAreaStart > 0)
    {
      /**
       * This is an optimization which kicks in when
       * we attempt to aggregate two buffers which contain
       * adjacent zero areas.
       */
      uint32_t zeroSize = o.m_zeroAreaEnd - o.m_zeroAreaStart;
      m_zeroAreaEnd += zeroSize;
      m_end = m_zeroAreaEnd;
      uint32_t endData = o.m_end - o.m_zeroAreaEnd;
      AddAtEnd (endData);
      Buffer::Iterator dst = End ();
      dst.Prev (endData);
      Buffer::Iterator src = o.End ();
      src.Prev (endData);
      dst.Write (src, o.End ());
      return;
    }
  Buffer dst = CreateFullCopy ();
  Buffer src = o.CreateFullCopy ();

  dst.AddAtEnd (src.GetSize ());
  Buffer::Iterator destStart = dst.End ();
  destStart.Prev (src.GetSize ());
  destStart.Write (src.Begin (), src.End ());
  *this = dst;
}

void 
Buffer::RemoveAtStart (uint32_t start)
{
  NS_ASSERT (CheckInternalState ());
  uint32_t newStart = m_start + start;
  if (newStart <= m_zeroAreaStart)
    {
      /* only remove start of buffer 
       */
      m_start = newStart;
    }
  else if (newStart <= m_zeroAreaEnd)
    {
      /* remove start of buffer _and_ start of zero area
       */
      uint32_t delta = newStart - m_zeroAreaStart;
      m_start = m_zeroAreaStart;
      m_zeroAreaEnd -= delta;
      m_end -= delta;
    } 
  else if (newStart <= m_end)
    {
      /* remove start of buffer, complete zero area, and part
       * of end of buffer 
       */
      NS_ASSERT (m_end >= start);
      uint32_t zeroSize = m_zeroAreaEnd - m_zeroAreaStart;
      m_start = newStart - zeroSize;
      m_end -= zeroSize;
      m_zeroAreaStart = m_start;
      m_zeroAreaEnd = m_start;
    }
  else 
    {
      /* remove all buffer */
      m_end -= m_zeroAreaEnd - m_zeroAreaStart;
      m_start = m_end;
      m_zeroAreaEnd = m_end;
      m_zeroAreaStart = m_end;
    }
  HEURISTICS (m_maxZeroAreaStart = std::max (m_maxZeroAreaStart, m_zeroAreaStart));
  LOG_INTERNAL_STATE ("rem start=" << start << ", ");
  NS_ASSERT (CheckInternalState ());
}
void 
Buffer::RemoveAtEnd (uint32_t end)
{
  NS_ASSERT (CheckInternalState ());
  uint32_t newEnd = m_end - std::min (end, m_end - m_start);
  if (newEnd > m_zeroAreaEnd)
    {
      /* remove part of end of buffer */
      m_end = newEnd;
    }
  else if (newEnd > m_zeroAreaStart)
    {
      /* remove end of buffer, part of zero area */
      m_end = newEnd;
      m_zeroAreaEnd = newEnd;
    }
  else if (newEnd > m_start)
    {
      /* remove end of buffer, zero area, part of start of buffer */
      m_end = newEnd;
      m_zeroAreaEnd = newEnd;
      m_zeroAreaStart = newEnd;
    }
  else
    {
      /* remove all buffer */
      m_end = m_start;
      m_zeroAreaEnd = m_start;
      m_zeroAreaStart = m_start;
    }
  HEURISTICS (m_maxZeroAreaStart = std::max (m_maxZeroAreaStart, m_zeroAreaStart));
  LOG_INTERNAL_STATE ("rem end=" << end << ", ");
  NS_ASSERT (CheckInternalState ());
}

Buffer 
Buffer::CreateFragment (uint32_t start, uint32_t length) const
{
  NS_ASSERT (CheckInternalState ());
  Buffer tmp = *this;
  tmp.RemoveAtStart (start);
  tmp.RemoveAtEnd (GetSize () - (start + length));
  NS_ASSERT (CheckInternalState ());
  return tmp;
}

Buffer 
Buffer::CreateFullCopy (void) const
{
  NS_ASSERT (CheckInternalState ());
  if (m_zeroAreaEnd - m_zeroAreaStart != 0) 
    {
      Buffer tmp;
      tmp.AddAtStart (m_zeroAreaEnd - m_zeroAreaStart);
      tmp.Begin ().WriteU8 (0, m_zeroAreaEnd - m_zeroAreaStart);
      uint32_t dataStart = m_zeroAreaStart - m_start;
      tmp.AddAtStart (dataStart);
      tmp.Begin ().Write (m_data->m_data+m_start, dataStart);
      uint32_t dataEnd = m_end - m_zeroAreaEnd;
      tmp.AddAtEnd (dataEnd);
      Buffer::Iterator i = tmp.End ();
      i.Prev (dataEnd);
      i.Write (m_data->m_data+m_zeroAreaStart,dataEnd);
      return tmp;
    }
  NS_ASSERT (CheckInternalState ());
  return *this;
}

void
Buffer::TransformIntoRealBuffer (void) const
{
  NS_ASSERT (CheckInternalState ());
  Buffer tmp = CreateFullCopy ();
  *const_cast<Buffer *> (this) = tmp;
  NS_ASSERT (CheckInternalState ());
}


uint8_t const*
Buffer::PeekData (void) const
{
  NS_ASSERT (CheckInternalState ());
  TransformIntoRealBuffer ();
  NS_ASSERT (CheckInternalState ());
  return m_data->m_data + m_start;
}

/******************************************************
 *            The buffer iterator below.
 ******************************************************/


Buffer::Iterator::Iterator ()
  : m_zeroStart (0),
    m_zeroEnd (0),
    m_dataStart (0),
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
  m_zeroStart = buffer->m_zeroAreaStart;
  m_zeroEnd = buffer->m_zeroAreaEnd;
  m_dataStart = buffer->m_start;
  m_dataEnd = buffer->m_end;
  m_data = buffer->m_data->m_data;
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
  int32_t diff = m_current - o.m_current;
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
  return m_current == m_dataStart;
}

bool 
Buffer::Iterator::CheckNoZero (uint32_t start, uint32_t end) const
{
  bool ok = true;
  for (uint32_t i = start; i < end; i++)
    {
      if (!Check (i))
        {
          ok = false;
        }
    }
  return ok;
}
bool 
Buffer::Iterator::Check (uint32_t i) const
{
  return i >= m_dataStart && 
    !(i >= m_zeroStart && i < m_zeroEnd) &&
    i <= m_dataEnd;
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
  Iterator cur = start;
  for (uint32_t i = 0; i < size; i++)
    {
      uint8_t data = cur.ReadU8 ();
      WriteU8 (data);
    }
}

void 
Buffer::Iterator::WriteU16 (uint16_t data)
{
  WriteU8 (data & 0xff);
  data >>= 8;
  WriteU8 (data & 0xff);
}
void 
Buffer::Iterator::WriteU32 (uint32_t data)
{
  WriteU8 (data & 0xff);
  data >>= 8;
  WriteU8 (data & 0xff);
  data >>= 8;
  WriteU8 (data & 0xff);
  data >>= 8;
  WriteU8 (data & 0xff);
}
void 
Buffer::Iterator::WriteU64 (uint64_t data)
{
  WriteU8 (data & 0xff);
  data >>= 8;
  WriteU8 (data & 0xff);
  data >>= 8;
  WriteU8 (data & 0xff);
  data >>= 8;
  WriteU8 (data & 0xff);
  data >>= 8;
  WriteU8 (data & 0xff);
  data >>= 8;
  WriteU8 (data & 0xff);
  data >>= 8;
  WriteU8 (data & 0xff);
  data >>= 8;
  WriteU8 (data & 0xff);
}
void 
Buffer::Iterator::WriteHtolsbU16 (uint16_t data)
{
  WriteU8 ((data >> 0) & 0xff);
  WriteU8 ((data >> 8) & 0xff);
}
void 
Buffer::Iterator::WriteHtolsbU32 (uint32_t data)
{
  WriteU8 ((data >> 0) & 0xff);
  WriteU8 ((data >> 8) & 0xff);
  WriteU8 ((data >> 16) & 0xff);
  WriteU8 ((data >> 24) & 0xff);
}
void 
Buffer::Iterator::WriteHtolsbU64 (uint64_t data)
{
  WriteU8 ((data >> 0) & 0xff);
  WriteU8 ((data >> 8) & 0xff);
  WriteU8 ((data >> 16) & 0xff);
  WriteU8 ((data >> 24) & 0xff);
  WriteU8 ((data >> 32) & 0xff);
  WriteU8 ((data >> 40) & 0xff);
  WriteU8 ((data >> 48) & 0xff);
  WriteU8 ((data >> 54) & 0xff);
}

void 
Buffer::Iterator::WriteHtonU16 (uint16_t data)
{
  WriteU8 ((data >> 8) & 0xff);
  WriteU8 ((data >> 0) & 0xff);
}
void 
Buffer::Iterator::WriteHtonU32 (uint32_t data)
{
  WriteU8 ((data >> 24) & 0xff);
  WriteU8 ((data >> 16) & 0xff);
  WriteU8 ((data >> 8) & 0xff);
  WriteU8 ((data >> 0) & 0xff);
}
void 
Buffer::Iterator::WriteHtonU64 (uint64_t data)
{
  WriteU8 ((data >> 56) & 0xff);
  WriteU8 ((data >> 48) & 0xff);
  WriteU8 ((data >> 40) & 0xff);
  WriteU8 ((data >> 32) & 0xff);
  WriteU8 ((data >> 24) & 0xff);
  WriteU8 ((data >> 16) & 0xff);
  WriteU8 ((data >> 8) & 0xff);
  WriteU8 ((data >> 0) & 0xff);
}
void 
Buffer::Iterator::Write (uint8_t const*buffer, uint32_t size)
{
  for (uint32_t i = 0; i < size; i++)
    {
      WriteU8 (buffer[i]);
    }
}

uint16_t 
Buffer::Iterator::ReadU16 (void)
{
  uint8_t byte0 = ReadU8 ();
  uint8_t byte1 = ReadU8 ();
  uint16_t data = byte1;
  data <<= 8;
  data |= byte0;

  return data;
}
uint32_t 
Buffer::Iterator::ReadU32 (void)
{
  uint8_t byte0 = ReadU8 ();
  uint8_t byte1 = ReadU8 ();
  uint8_t byte2 = ReadU8 ();
  uint8_t byte3 = ReadU8 ();
  uint32_t data = byte3;
  data <<= 8;
  data |= byte2;
  data <<= 8;
  data |= byte1;
  data <<= 8;
  data |= byte0;
  return data;
}
uint64_t 
Buffer::Iterator::ReadU64 (void)
{
  uint8_t byte0 = ReadU8 ();
  uint8_t byte1 = ReadU8 ();
  uint8_t byte2 = ReadU8 ();
  uint8_t byte3 = ReadU8 ();
  uint8_t byte4 = ReadU8 ();
  uint8_t byte5 = ReadU8 ();
  uint8_t byte6 = ReadU8 ();
  uint8_t byte7 = ReadU8 ();
  uint32_t data = byte7;
  data <<= 8;
  data |= byte6;
  data <<= 8;
  data |= byte5;
  data <<= 8;
  data |= byte4;
  data <<= 8;
  data |= byte3;
  data <<= 8;
  data |= byte2;
  data <<= 8;
  data |= byte1;
  data <<= 8;
  data |= byte0;

  return data;
}
uint16_t 
Buffer::Iterator::ReadNtohU16 (void)
{
  uint16_t retval = 0;
  retval |= ReadU8 ();
  retval <<= 8;
  retval |= ReadU8 ();
  return retval;
}
uint32_t 
Buffer::Iterator::ReadNtohU32 (void)
{
  uint32_t retval = 0;
  retval |= ReadU8 ();
  retval <<= 8;
  retval |= ReadU8 ();
  retval <<= 8;
  retval |= ReadU8 ();
  retval <<= 8;
  retval |= ReadU8 ();
  return retval;
}
uint64_t 
Buffer::Iterator::ReadNtohU64 (void)
{
  uint64_t retval = 0;
  retval |= ReadU8 ();
  retval <<= 8;
  retval |= ReadU8 ();
  retval <<= 8;
  retval |= ReadU8 ();
  retval <<= 8;
  retval |= ReadU8 ();
  retval <<= 8;
  retval |= ReadU8 ();
  retval <<= 8;
  retval |= ReadU8 ();
  retval <<= 8;
  retval |= ReadU8 ();
  retval <<= 8;
  retval |= ReadU8 ();
  return retval;
}
uint16_t 
Buffer::Iterator::ReadLsbtohU16 (void)
{
  uint8_t byte0 = ReadU8 ();
  uint8_t byte1 = ReadU8 ();
  uint16_t data = byte1;
  data <<= 8;
  data |= byte0;
  return data;
}
uint32_t 
Buffer::Iterator::ReadLsbtohU32 (void)
{
  uint8_t byte0 = ReadU8 ();
  uint8_t byte1 = ReadU8 ();
  uint8_t byte2 = ReadU8 ();
  uint8_t byte3 = ReadU8 ();
  uint32_t data = byte3;
  data <<= 8;
  data |= byte2;
  data <<= 8;
  data |= byte1;
  data <<= 8;
  data |= byte0;
  return data;
}
uint64_t 
Buffer::Iterator::ReadLsbtohU64 (void)
{
  uint8_t byte0 = ReadU8 ();
  uint8_t byte1 = ReadU8 ();
  uint8_t byte2 = ReadU8 ();
  uint8_t byte3 = ReadU8 ();
  uint8_t byte4 = ReadU8 ();
  uint8_t byte5 = ReadU8 ();
  uint8_t byte6 = ReadU8 ();
  uint8_t byte7 = ReadU8 ();
  uint32_t data = byte7;
  data <<= 8;
  data |= byte6;
  data <<= 8;
  data |= byte5;
  data <<= 8;
  data |= byte4;
  data <<= 8;
  data |= byte3;
  data <<= 8;
  data |= byte2;
  data <<= 8;
  data |= byte1;
  data <<= 8;
  data |= byte0;

  return data;
}
void 
Buffer::Iterator::Read (uint8_t *buffer, uint32_t size)
{
  for (uint32_t i = 0; i < size; i++)
    {
      buffer[i] = ReadU8 ();
    }
}

#ifndef BUFFER_USE_INLINE

void 
Buffer::Iterator::WriteU8  (uint8_t  data)
{
  if (m_current < m_dataStart)
    {
      // XXX trying to write outside of data area
      NS_ASSERT (false);
    }
  else if (m_current < m_zeroStart)
    {
      m_data[m_current] = data;
      m_current++;
    }
  else if (m_current < m_zeroEnd)
    {
      // XXX trying to write in zero area
      NS_ASSERT (false);
    }
  else if (m_current < m_dataEnd)
    {
      m_data[m_current - (m_zeroEnd-m_zeroStart)] = data;
      m_current++;      
    }
  else 
    {
      // XXX trying to write outside of data area
      NS_ASSERT (false);
    }
}

void 
Buffer::Iterator::WriteU8 (uint8_t  data, uint32_t len)
{
  for (uint32_t i = 0; i < len; i++)
    {
      WriteU8 (data);
    }
}

uint8_t  
Buffer::Iterator::ReadU8 (void)
{
  if (m_current < m_dataStart)
    {
      // XXX trying to read from outside of data area
      NS_ASSERT (false);
    }
  else if (m_current < m_zeroStart)
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
  else if (m_current < m_dataEnd)
    {
      uint8_t data = m_data[m_current - (m_zeroEnd-m_zeroStart)];
      m_current++;
      return data;
    }
  else 
    {
      // XXX trying to read from outside of data area
      NS_ASSERT (false);
    }
  // to quiet compiler.
  return 0;
}

#endif /* BUFFER_USE_INLINE */

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

#if 0
  buffer = Buffer (10);  
  ENSURE_WRITTEN_BYTES (buffer, 10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
  buffer.Begin ().WriteU8 (1);
  ENSURE_WRITTEN_BYTES (buffer, 10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
#endif

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

  buffer = Buffer (5);
  buffer.AddAtEnd (2);
  i = buffer.End ();
  i.Prev (2);
  i.WriteU8 (0);
  i.WriteU8 (0x66);
  ENSURE_WRITTEN_BYTES (buffer, 7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66);
  Buffer frag0 = buffer.CreateFragment (0, 2);
  ENSURE_WRITTEN_BYTES (frag0, 2, 0x00, 0x00);
  Buffer frag1 = buffer.CreateFragment (2, 5);
  ENSURE_WRITTEN_BYTES (frag1, 5, 0x00, 0x00, 0x00, 0x00, 0x66);
  frag0.AddAtEnd (frag1);
  ENSURE_WRITTEN_BYTES (buffer, 7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66);
  ENSURE_WRITTEN_BYTES (frag0, 7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66);

  return result;
}



static BufferTest gBufferTest;

} // namespace ns3

#endif /* RUN_SELF_TESTS */


