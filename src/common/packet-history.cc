/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
#include <utility>
#include "ns3/assert.h"
#include "ns3/fatal-error.h"
#include "packet-history.h"
#include "chunk.h"
#include "buffer.h"

namespace ns3 {

bool PacketHistory::m_enable = false;
PacketHistory::ChunkFactories PacketHistory::m_factories;
uint32_t PacketHistory::m_maxSize = 0;
PacketHistory::DataFreeList PacketHistory::m_freeList;

void 
PacketHistory::Enable (void)
{
  m_enable = true;
}


PacketHistory::PacketHistory ()
  : m_data (0),
    m_end (0),
    m_n (0),
    m_aggregated (false)
{
  Construct (0, 0);
}
PacketHistory::PacketHistory (uint32_t uid, uint32_t size)
  : m_data (0),
    m_end (0),
    m_n (0),
    m_aggregated (false)
{
  Construct (uid, size);
}
void
PacketHistory::Construct (uint32_t uid, uint32_t size)
{
  if (m_enable) 
    {
      m_data = PacketHistory::Create (size);
      AppendOneCommand (PacketHistory::INIT_UID,
                        uid);
    }
}
PacketHistory::PacketHistory (PacketHistory const &o)
  : m_data (o.m_data),
    m_end (o.m_end),
    m_n (o.m_n),
    m_aggregated (o.m_aggregated)
{
  if (m_data != 0) 
    {
      m_data->m_count++;
    }
}
PacketHistory &
PacketHistory::operator = (PacketHistory const& o)
{
  if (m_data == o.m_data) 
    {
      // self assignment
      return *this;
    }
  if (m_data != 0) 
    {
      m_data->m_count--;
      if (m_data->m_count == 0) 
        {
          PacketHistory::Recycle (m_data);
        }
    }
  m_data = o.m_data;
  m_end = o.m_end;
  m_n = o.m_n;
  m_aggregated = o.m_aggregated;
  if (m_data != 0) 
    {
      m_data->m_count++;
    }
  return *this;
}
PacketHistory::~PacketHistory ()
{
  if (m_data != 0) 
    {
      m_data->m_count--;
      if (m_data->m_count == 0) 
        {
          PacketHistory::Recycle (m_data);
        }
    }
}


struct PacketHistory::CommandData *
PacketHistory::Create (uint32_t size)
{
  if (size > m_maxSize)
    {
      m_maxSize = size;
    }
  while (!m_freeList.empty ()) 
    {
      struct PacketHistory::CommandData *data = m_freeList.back ();
      m_freeList.pop_back ();
      if (data->m_size >= size) 
        {
          data->m_count = 1;
          return data;
        }
      PacketHistory::Deallocate (data);
    }
  return PacketHistory::Allocate (size);
}

void
PacketHistory::Recycle (struct CommandData *data)
{
  NS_ASSERT (data->m_count == 0);
  if (m_freeList.size () > 1000 ||
      data->m_size < m_maxSize) 
    {
      PacketHistory::Deallocate (data);
    } 
  else 
    {
      m_freeList.push_back (data);
    }
}

struct PacketHistory::CommandData *
PacketHistory::Allocate (uint32_t n)
{
  uint32_t size = sizeof (struct CommandData);
  if (n <= 4)
    {
      n = 4;
    }
  size += (n-4) * (4 + 1);
  uint8_t *buf = new uint8_t [size];
  struct CommandData *data = (struct CommandData *)buf;
  data->m_size = n;
  data->m_count = 1;
  return data;
}
void 
PacketHistory::Deallocate (struct CommandData *data)
{
  uint8_t *buf = (uint8_t *)data;
  delete [] buf;
}

void
PacketHistory::AppendOneCommand (uint32_t type, uint32_t data0, uint32_t data1)
{
  NS_ASSERT (m_data != 0);
  uint32_t n = GetUleb128Size (type);
  n += GetUleb128Size (data0);
  n += GetUleb128Size (data1);

  if (m_data->m_size > m_end + n)
    {
      if (m_data->m_count == 1 ||
          m_data->m_dirtyEnd == m_end)
        {
          AppendValue (data1);
          AppendValue (data0);
          AppendValue (type);
          m_n++;
          return;
        }
      else
        {
          uint8_t *buffer = &(m_data->m_data[m_end]);
          uint32_t lastType = ReadReverseValue (&buffer);
          if (lastType == type)
            {
              uint32_t lastData = ReadReverseValue (&buffer);
              if (lastData == data0)
                {
                  lastData = ReadReverseValue (&buffer);
                  if (lastData == data1)
                    {
                      return;
                    }
                }
            }
        }
    }
  Reserve (n);
  AppendValue (data1);
  AppendValue (data0);
  AppendValue (type);
  m_n++;
}

void
PacketHistory::AppendOneCommand (uint32_t type, uint32_t data)
{
  NS_ASSERT (m_data != 0);
  uint32_t n = GetUleb128Size (data);
  n += GetUleb128Size (type);

  if (m_data->m_size > m_end + n)
    {
      if (m_data->m_count == 1 ||
          m_data->m_dirtyEnd == m_end)
        {
          AppendValue (data);
          AppendValue (type);
          m_n++;
          return;
        }
      else
        {
          uint8_t *buffer = &(m_data->m_data[m_end]);
          uint32_t lastType = ReadReverseValue (&buffer);
          if (lastType == type)
            {
              uint32_t lastData = ReadReverseValue (&buffer);
              if (lastData == data)
                {
                  return;
                }
            }
        }
    }
  Reserve (n);
  AppendValue (data);
  AppendValue (type);
  m_n++;
}
void
PacketHistory::ReserveCopy (uint32_t size)
{
  struct CommandData *newData = PacketHistory::Create (m_end + size);
  memcpy (newData->m_data, m_data->m_data, m_end);
  newData->m_dirtyEnd = m_end;
  m_data->m_count--;
  if (m_data->m_count == 0) 
    {
      PacketHistory::Recycle (m_data);
    }
  m_data = newData;
}
void
PacketHistory::Reserve (uint32_t size)
{
  NS_ASSERT (m_data != 0);
  if (m_data->m_size > m_end + size &&
      (m_data->m_count == 1 ||
       m_data->m_dirtyEnd == m_end))
    {
      /* enough room, not dirty. */
    }
  else 
    {
      /* (enough room and dirty) or (not enough room) */
      ReserveCopy (size);
    }
}

uint32_t 
PacketHistory::GetUleb128Size (uint32_t value) const
{
  uint32_t n = 0;
  uint32_t tmp = value;
  do {
    tmp >>= 7;
    n++;
  } while (tmp != 0);
  return n;
}

uint32_t 
PacketHistory::GetReverseUleb128Size (uint8_t *buffer) const
{
  uint8_t *cur = buffer;
  uint8_t byte = *cur;
  if (byte & 0x80)
    {
      NS_FATAL_ERROR ("The last byte should have a zero high bit");
    }
  cur--;
  while ((byte & 0x80) && (buffer - cur) < 5)
    {
      cur--;
      byte = *cur;
    }
  uint32_t n = buffer - cur;
  return n;
}

void
PacketHistory::AppendValue (uint32_t value)
{
  uint32_t n = 0;
  uint8_t *buffer = &m_data->m_data[m_end];
  do {
    uint8_t byte = value & (~0x80);
    value >>= 7;
    if (value != 0)
      {
        /* more bytes to come */
        byte |= 0x80;
      }
    n++;
    *buffer = byte;
    buffer++;
  } while (value != 0);
  m_end += n;
  if (m_end > m_data->m_dirtyEnd)
    {
      m_data->m_dirtyEnd = m_end;
    }
}

uint32_t
PacketHistory::ReadValue (uint8_t *buffer, uint32_t *n) const
{
  uint32_t result = 0;
  uint8_t shift, byte;
  result = 0;
  shift = 0;
  do {
    byte = *buffer;
    buffer++;
    result |= (byte & (~0x80))<<shift;
    shift += 7;
    (*n)++;
  } while (byte & 0x80 && 
           /* a LEB128 unsigned number is at most 5 bytes long. */
           shift < (7*5)); 
  if (byte & 0x80)
    {
      /* This means that the LEB128 number was not valid.
       * ie: the last (5th) byte did not have the high-order bit zeroed.
       */
      result = -1;
    }
  return result;
}

uint32_t
PacketHistory::ReadReverseValue (uint8_t **pBuffer) const
{
  uint32_t n = GetReverseUleb128Size (*pBuffer);
  NS_ASSERT (n > 0);
  uint8_t *buffer = *pBuffer - n + 1;
  uint32_t read = 0;
  uint32_t result = ReadValue (buffer, &read);
  NS_ASSERT (read == n);
  *pBuffer = *pBuffer - n;
  return result;
}

uint32_t
PacketHistory::ReadForwardValue (uint8_t **pBuffer) const
{
  uint32_t read = 0;
  uint32_t result = ReadValue (*pBuffer, &read);
  *pBuffer = *pBuffer + read;
  return result;
}

PacketHistory 
PacketHistory::CreateFragment (uint32_t start, uint32_t end) const
{
  PacketHistory fragment = *this;
  fragment.RemoveAtStart (start);
  fragment.RemoveAtEnd (end);
  return fragment;
}

void 
PacketHistory::AddHeader (uint32_t uid, Chunk const & header, uint32_t size)
{
  if (m_enable) 
    {
      AppendOneCommand (PacketHistory::ADD_HEADER, uid, size);
    }  
}
void 
PacketHistory::RemoveHeader (uint32_t uid, Chunk const & header, uint32_t size)
{
  if (m_enable) 
    {
      AppendOneCommand (PacketHistory::REM_HEADER, uid, size);
    }
}
void 
PacketHistory::AddTrailer (uint32_t uid, Chunk const & trailer, uint32_t size)
{
  if (m_enable) 
    {
      AppendOneCommand (PacketHistory::ADD_TRAILER, uid, size);
    }
}
void 
PacketHistory::RemoveTrailer (uint32_t uid, Chunk const & trailer, uint32_t size)
{
  if (m_enable) 
    {
      AppendOneCommand (PacketHistory::REM_TRAILER, uid, size);
    }
}
void
PacketHistory::AddAtEnd (PacketHistory const&o)
{
  if (m_enable) 
    {
      m_aggregated = true;
      uint32_t n = GetUleb128Size (PacketHistory::ADD_AT_END);
      n += o.m_end;
      Reserve (n);
      memcpy (&m_data->m_data[m_end], o.m_data->m_data, o.m_end);
      m_end += o.m_end;
      if (m_end > m_data->m_dirtyEnd)
        {
          m_data->m_dirtyEnd = m_end;
        }
      AppendOneCommand (PacketHistory::ADD_AT_END, 0);
    }
}
void
PacketHistory::AddPaddingAtEnd (uint32_t end)
{
  if (m_enable)
    {
      AppendOneCommand (PacketHistory::PADDING_AT_END, end);
    }
}
void 
PacketHistory::RemoveAtStart (uint32_t start)
{
  if (m_enable) 
    {
      AppendOneCommand (PacketHistory::REM_AT_START, start);
    }
}
void 
PacketHistory::RemoveAtEnd (uint32_t end)
{
  if (m_enable) 
    {
      AppendOneCommand (PacketHistory::REM_AT_END, end);
    }
}

void 
PacketHistory::PrintSimple (std::ostream &os, Buffer buffer) const
{
  Buffer original = buffer;
  HeadersToPrint headersToPrint;
  TrailersToPrint trailersToPrint;
  uint8_t *dataBuffer = &m_data->m_data[m_end] - 1;
  int32_t start = 0;
  int32_t end = buffer.GetSize ();
  int32_t curStart = start;
  int32_t curEnd = end;
  for (uint32_t i = 0; i < m_n; i++)
    {
      uint32_t type = ReadReverseValue (&dataBuffer);
      uint32_t data = ReadReverseValue (&dataBuffer);
      switch (type)
        {
        case PacketHistory::INIT_UID:
          break;
        case PacketHistory::INIT_SIZE:
          std::cout << "init size=" << data << std::endl;
          break;
        case PacketHistory::ADD_HEADER: {
          int32_t size = ReadReverseValue (&dataBuffer);
          if (curStart == start)
            {
              if (start + size < end)
                {
                  headersToPrint.push_back (std::make_pair (data, curStart));
                }
              curStart += size;
              start += size;
            }
          else if (curStart + size <= start)
            {
              // header lies entirely outside of data area.
              curStart += size;
            }
          else if (curStart < start)
            {
              // header lies partly inside and outside of data area
              // potentially, because we fragmented the packet in the middle
              // of this header.
              curStart += size;
            }
          else
            {
              // header lies entirely inside data area but there is some 
              // data at the start of the data area which does not belong
              // to this header. Potentially, because we fragmented
              // the packet in the middle of a previous header.
              NS_ASSERT (curStart > start);
              // we print the content of the header anyway because we can.
              if (start + size < end)
                {
                  headersToPrint.push_back (std::make_pair (data, curStart));
                }
              curStart += size;
              start = curStart;
            }
        } break;
        case PacketHistory::REM_HEADER: {
          int32_t size = ReadReverseValue (&dataBuffer);
          if (curStart <= start)
            {
              // header lies entirely outside of data area.
              curStart -= size;
            }
          else
            {
              NS_ASSERT (false);
            }
        } break;
        case PacketHistory::ADD_TRAILER: {
          int32_t size = ReadReverseValue (&dataBuffer);
          if (curEnd == end)
            {
              if (end - size >= start)
                {
                  // trailer lies exactly at the end of the data area
                  trailersToPrint.push_back (std::make_pair (data, buffer.GetSize () - curEnd));
                }
              curEnd -= size;
              end -= size;
            }
          else if (curEnd - size >= end)
            {
              // trailer lies entirely outside of data area.
              curEnd -= size;
            }
          else if (curEnd > end)
            {
              // header lies partly inside and partly outside of
              // data area, potentially because of fragmentation.
              curEnd -= size;
            }
          else
            {
              // header lies entirely inside data area
              NS_ASSERT (curEnd < end);
              if (end - size >= start)
                {
                  trailersToPrint.push_back (std::make_pair (data, buffer.GetSize () - curEnd));
                }
              curEnd -= size;
              end = curEnd;

            }
        } break;
        case PacketHistory::REM_TRAILER: {
          int32_t size = ReadReverseValue (&dataBuffer);
          if (curEnd >= end)
            {
              curEnd += size;
            }
          else
            {
              NS_ASSERT (false);
            }
        } break;
        case PacketHistory::REM_AT_START:
          curStart -= data;
          break;
        case PacketHistory::REM_AT_END:
          curEnd += data;
          break;
        }
    }
  for (HeadersToPrint::iterator j = headersToPrint.begin (); 
       j != headersToPrint.end (); j++)
    {
      uint32_t uid = j->first;
      uint32_t offset = j->second;
      Buffer tmp = original;
      tmp.RemoveAtStart (offset);
      Chunk *chunk = CreateStatic (uid);
      chunk->Deserialize (tmp.Begin ());
      chunk->Print (os);
    }
  for (TrailersToPrint::reverse_iterator j = trailersToPrint.rbegin (); 
       j != trailersToPrint.rend (); j++)
    {
      uint32_t uid = j->first;
      uint32_t offset = j->second;
      Buffer tmp = original;
      tmp.RemoveAtEnd (offset);
      Chunk *chunk = CreateStatic (uid);
      chunk->Deserialize (tmp.End ());
      chunk->Print (os);
    }
}

void 
PacketHistory::PrintComplex (std::ostream &os, Buffer buffer) const
{
  // we need to build a linked list of the different fragments 
  // which are stored in this packet.
  
}

void 
PacketHistory::PrintDefault (std::ostream &os, Buffer buffer) const
{
  if (!m_enable) 
    {
      return;
    }

  if (m_aggregated)
    {
      PrintComplex (os, buffer);
    }
  else
    {
      PrintSimple (os, buffer);
    }
}


Chunk *
PacketHistory::CreateStatic (uint32_t uid)
{
  for (ChunkFactoriesI i = m_factories.begin (); i != m_factories.end (); i++) 
    {
      if (i->first == uid) 
        {
          return (*i->second) ();
        }
    }
  NS_ASSERT_MSG (false, "cannot be reached");
  /* quiet compiler */
  return 0;
}
uint32_t 
PacketHistory::RegisterChunkFactory (Chunk *(*createStatic) (void))
{
  static uint32_t uid = 0;
  uid++;
  for (ChunkFactoriesI i = m_factories.begin (); i != m_factories.end (); i++) 
    {
      NS_ASSERT (i->first != uid);
    }
  m_factories.push_back (std::make_pair (uid, createStatic));
  return uid;
}

}; // namespace ns3

#include <stdarg.h>
#include <iostream>
#include "ns3/test.h"
#include "header.h"
#include "trailer.h"
#include "packet.h"

namespace ns3 {

static std::list<int> g_prints;
static bool g_headerError;
static bool g_trailerError;

static void 
RecordPrint (int n)
{
  g_prints.push_back (n);
}

static void
RecordTrailerError (int n)
{
  g_trailerError = true;
}

static void
RecordHeaderError (int n)
{
  g_headerError = true;
}

static void
CleanupPrints (void)
{
  g_prints.erase (g_prints.begin (), g_prints.end ());
}

template <int N>
class HistoryHeader : public Header
{
private:
  virtual void PrintTo (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void SerializeTo (Buffer::Iterator start) const;
  virtual uint32_t DeserializeFrom (Buffer::Iterator start);
};

template <int N>
void 
HistoryHeader<N>::PrintTo (std::ostream &os) const
{
  RecordPrint (N);
}
template <int N>
uint32_t 
HistoryHeader<N>::GetSerializedSize (void) const
{
  return N;
}
template <int N>
void 
HistoryHeader<N>::SerializeTo (Buffer::Iterator start) const
{
  start.WriteU8 (N, N);
}
template <int N>
uint32_t
HistoryHeader<N>::DeserializeFrom (Buffer::Iterator start)
{
  for (int i = 0; i < N; i++)
    {
      if (start.ReadU8 () != N)
        {
          RecordHeaderError (N);
        }
    }
  return N;
}

template <int N>
class HistoryTrailer : public Trailer
{
private:
  virtual void PrintTo (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void SerializeTo (Buffer::Iterator start) const;
  virtual uint32_t DeserializeFrom (Buffer::Iterator start);
};

template <int N>
void 
HistoryTrailer<N>::PrintTo (std::ostream &os) const
{
  RecordPrint (N);
}
template <int N>
uint32_t 
HistoryTrailer<N>::GetSerializedSize (void) const
{
  return N;
}
template <int N>
void 
HistoryTrailer<N>::SerializeTo (Buffer::Iterator start) const
{
  start.WriteU8 (N, N);
}
template <int N>
uint32_t
HistoryTrailer<N>::DeserializeFrom (Buffer::Iterator start)
{
  start.Prev (N);
  for (int i = 0; i < N; i++)
    {
      if (start.ReadU8 () != N)
        {
          RecordTrailerError (N);
        }
    }
  return N;
}



class PacketHistoryTest : public Test {
public:
  PacketHistoryTest ();
  virtual ~PacketHistoryTest ();
  bool CheckHistory (Packet p, char *file, int line, uint32_t n, ...);
  virtual bool RunTests (void);
};

PacketHistoryTest::PacketHistoryTest ()
  : Test ("PacketHistory")
{}

PacketHistoryTest::~PacketHistoryTest ()
{}

bool 
PacketHistoryTest::CheckHistory (Packet p, char *file, int line, uint32_t n, ...)
{
  g_headerError = false;
  g_trailerError = false;
  va_list ap;
  p.Print (std::cerr);
  va_start (ap, n);
  if (g_headerError)
    {
      std::cout << "PacketHistory header error. file=" << file 
                << ", line=" << line << std::endl;
      return false;
    }
  if (g_trailerError)
    {
      std::cout << "PacketHistory trailer error. file=" << file 
                << ", line=" << line << std::endl;
      return false;
    }
  if (n != g_prints.size ())
    {
      goto error;
    }
  for (std::list<int>::iterator i = g_prints.begin (); 
       i != g_prints.end (); i++)
    {
      int v = va_arg (ap, int);
      if (v != *i)
        {
          va_end (ap);
          goto error;
        }
    }
  va_end (ap);
  return true;
 error:
  std::cout << "PacketHistory error. file="<< file 
            << ", line=" << line << ", got:\"";
  for (std::list<int>::iterator i = g_prints.begin (); 
       i != g_prints.end (); i++)
    {
      std::cout << *i << ", ";
    }
  std::cout << "\", expected: \"";
  va_start (ap, n);
  for (uint32_t j = 0; j < n; j++)
    {
      int v = va_arg (ap, int);
      std::cout << v << ", ";
    }
  va_end (ap);
  std::cout << "\"" << std::endl;
  return false;
}

#define ADD_HEADER(p, n)                        \
  {                                             \
    HistoryHeader<n> header;                    \
    p.AddHeader (header);                       \
  }
#define ADD_TRAILER(p, n)                       \
  {                                             \
    HistoryTrailer<n> trailer;                  \
    p.AddTrailer (trailer);                     \
  }
#define REM_HEADER(p, n)                        \
  {                                             \
    HistoryHeader<n> header;                    \
    p.RemoveHeader (header);                    \
  }
#define REM_TRAILER(p, n)                       \
  {                                             \
    HistoryTrailer<n> trailer;                  \
    p.RemoveTrailer (trailer);                  \
  }
#define CHECK_HISTORY(p, ...)                   \
  {                                             \
    if (!CheckHistory (p, __FILE__,             \
                      __LINE__, __VA_ARGS__))   \
      {                                         \
        ok = false;                             \
      }                                         \
    CleanupPrints ();                           \
  }

bool
PacketHistoryTest::RunTests (void)
{
  bool ok = true;

  PacketHistory::Enable ();

  Packet p = Packet (0);
  Packet p1 = Packet (0);

  p = Packet (10);
  ADD_TRAILER (p, 100);
  CHECK_HISTORY (p, 1, 100);

  p = Packet (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  CHECK_HISTORY (p, 3, 
                 3, 2, 1);
  ADD_HEADER (p, 5);
  CHECK_HISTORY (p, 4, 
                 5, 3, 2, 1);
  ADD_HEADER (p, 6);
  CHECK_HISTORY (p, 5, 
                 6, 5, 3, 2, 1);

  p = Packet (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  REM_HEADER (p, 3);
  CHECK_HISTORY (p, 2, 
                 2, 1);

  p = Packet (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  REM_HEADER (p, 3);
  REM_HEADER (p, 2);
  CHECK_HISTORY (p, 1, 
                 1);

  p = Packet (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  REM_HEADER (p, 3);
  REM_HEADER (p, 2);
  REM_HEADER (p, 1);
  CHECK_HISTORY (p, 0);

  p = Packet (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  p1 = p;
  REM_HEADER (p1, 3);
  REM_HEADER (p1, 2);
  REM_HEADER (p1, 1);
  CHECK_HISTORY (p1, 0);
  CHECK_HISTORY (p, 3, 
                 3, 2, 1);
  ADD_HEADER (p1, 1);
  ADD_HEADER (p1, 2);
  CHECK_HISTORY (p1, 2, 
                 2, 1);
  CHECK_HISTORY (p, 3, 
                 3, 2, 1);
  ADD_HEADER (p, 3);
  CHECK_HISTORY (p, 4, 
                 3, 3, 2, 1);
  ADD_TRAILER (p, 4);
  CHECK_HISTORY (p, 5, 
                 3, 3, 2, 1, 4);
  ADD_TRAILER (p, 5);
  CHECK_HISTORY (p, 6, 
                 3, 3, 2, 1, 4, 5);
  REM_HEADER (p, 3);
  CHECK_HISTORY (p, 5, 
                 3, 2, 1, 4, 5);
  REM_TRAILER (p, 5);
  CHECK_HISTORY (p, 4, 
                 3, 2, 1, 4);
  p1 = p;
  REM_TRAILER (p, 4);
  CHECK_HISTORY (p, 3, 
                 3, 2, 1);
  CHECK_HISTORY (p1, 4, 
                 3, 2, 1, 4);
  p1.RemoveAtStart (3);
  CHECK_HISTORY (p1, 3, 
                 2, 1, 4);
  p1.RemoveAtStart (2);
  CHECK_HISTORY (p1, 2, 
                 1, 4);
  p1.RemoveAtEnd (4);
  CHECK_HISTORY (p1, 1, 
                 1);
  p1.RemoveAtStart (1);
  CHECK_HISTORY (p1, 0);

  p = Packet (10);
  ADD_HEADER (p, 8);
  ADD_TRAILER (p, 8);
  ADD_TRAILER (p, 8);
  p.RemoveAtStart (8+10+8);
  CHECK_HISTORY (p, 1, 8);


  p = Packet (10);
  ADD_HEADER (p, 10);
  ADD_HEADER (p, 8);
  ADD_TRAILER (p, 6);
  ADD_TRAILER (p, 7);
  ADD_TRAILER (p, 9);
  p.RemoveAtStart (5);
  p.RemoveAtEnd (12);
  CHECK_HISTORY (p, 2, 10, 6);


  return ok;
}

static PacketHistoryTest g_packetHistoryTest;

}//namespace ns3
