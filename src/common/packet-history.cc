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
#include <list>
#include "ns3/assert.h"
#include "ns3/fatal-error.h"
#include "packet-history.h"
#include "chunk.h"
#include "buffer.h"

namespace {

class ItemList
{
public:
  void InitPayload (uint32_t packetUid, uint32_t size);
  void AddHeader (uint32_t type, uint32_t size);
  void AddTrailer (uint32_t type, uint32_t size);

  void RemHeader (uint32_t type, uint32_t size);
  void RemTrailer (uint32_t type, uint32_t size);

  void RemAtStart (uint32_t toRemove);
  void RemAtEnd (uint32_t toRemove);

  void AddAtEnd (ItemList const *other);

  void Print (std::ostream &os, ns3::Buffer buffer, const ns3::PacketPrinter &printer) const;

private:
  enum Type {
    PAYLOAD,
    HEADER,
    TRAILER,
  };
  struct Item
  {
    enum ItemList::Type m_type;
    uint32_t m_size;
    uint32_t m_chunkType;
    uint32_t m_fragmentStart;
    uint32_t m_fragmentEnd;
    uint32_t m_packetUid;
  };
  std::list<Item> m_itemList;
};

void 
ItemList::InitPayload (uint32_t packetUid, uint32_t size)
{
  NS_ASSERT (m_itemList.empty ());
  struct Item item;
  item.m_type = ItemList::PAYLOAD;
  item.m_chunkType = 0;
  item.m_size = size;
  item.m_fragmentStart = 0;
  item.m_fragmentEnd = item.m_size;
  item.m_packetUid = packetUid;
  m_itemList.push_back (item);
}

void 
ItemList::AddHeader (uint32_t type, uint32_t size)
{
  struct Item item;
  item.m_type = ItemList::HEADER;
  item.m_chunkType = type;
  item.m_size = size;
  item.m_fragmentStart = 0;
  item.m_fragmentEnd = size;
  item.m_packetUid = m_itemList.front ().m_packetUid;
  m_itemList.push_front (item);
}

void 
ItemList::AddTrailer (uint32_t type, uint32_t size)
{
  struct Item item;
  item.m_type = ItemList::TRAILER;
  item.m_chunkType = type;
  item.m_size = size;
  item.m_fragmentStart = 0;
  item.m_fragmentEnd = size;
  item.m_packetUid = m_itemList.back ().m_packetUid;
  m_itemList.push_back (item);
}

void 
ItemList::RemHeader (uint32_t type, uint32_t size)
{
  struct Item item = m_itemList.front ();
  if (item.m_type != ItemList::HEADER ||
      item.m_size != size ||
      item.m_chunkType != type)
    {
      NS_FATAL_ERROR ("Removing Unexpected header");
    }
  else if (item.m_fragmentStart != 0 ||
           item.m_fragmentEnd != item.m_size)
    {
      NS_FATAL_ERROR ("Removing non-complete header");
    }
  m_itemList.pop_front ();
}
void 
ItemList::RemTrailer (uint32_t type, uint32_t size)
{
  struct Item item = m_itemList.back ();
  if (item.m_type != ItemList::TRAILER ||
      item.m_size != size ||
      item.m_chunkType != type)
    {
      NS_FATAL_ERROR ("Removing Unexpected trailer");
    }
  else if (item.m_fragmentStart != 0 ||
           item.m_fragmentEnd != item.m_size)
    {
      NS_FATAL_ERROR ("Removing non-complete trailer");
    }
  m_itemList.pop_back ();
}

void 
ItemList::RemAtStart (uint32_t toRemove)
{
  uint32_t leftToRemove = toRemove;
  while (!m_itemList.empty () && leftToRemove > 0)
    {
      struct Item &item = m_itemList.front ();
      if (item.m_fragmentEnd - item.m_fragmentStart <= leftToRemove)
        {
          m_itemList.pop_front ();
          leftToRemove -= item.m_size;
        }
      else
        {
          item.m_fragmentStart += leftToRemove;
          leftToRemove = 0;
          NS_ASSERT (item.m_size >= item.m_fragmentEnd - item.m_fragmentStart &&
                     item.m_fragmentStart <= item.m_fragmentEnd);
        }
    }
  NS_ASSERT (leftToRemove == 0);
}
void 
ItemList::RemAtEnd (uint32_t toRemove)
{
  uint32_t leftToRemove = toRemove;
  while (!m_itemList.empty () && leftToRemove > 0)
    {
      struct Item &item = m_itemList.back ();
      if (item.m_fragmentEnd - item.m_fragmentStart <= leftToRemove)
        {
          m_itemList.pop_back ();
          leftToRemove -= item.m_size;
        }
      else
        {
          item.m_fragmentEnd -= leftToRemove;
          leftToRemove = 0;
        }
      NS_ASSERT (item.m_size >= item.m_fragmentEnd - item.m_fragmentStart &&
                 item.m_fragmentStart <= item.m_fragmentEnd &&
                 item.m_fragmentEnd <= item.m_size);
    }
  NS_ASSERT (leftToRemove == 0);
}

void 
ItemList::AddAtEnd (ItemList const *other)
{
  for (std::list<ItemList::Item>::const_iterator i = other->m_itemList.begin (); 
       i != other->m_itemList.end (); i++)
    {
      const ItemList::Item &item = *i;
      ItemList::Item &last = m_itemList.back ();
      if (item.m_packetUid == last.m_packetUid &&
          item.m_type == last.m_type &&
          item.m_chunkType == last.m_chunkType &&
          item.m_size == last.m_size &&
          last.m_fragmentEnd != last.m_size && 
          item.m_fragmentStart == last.m_fragmentEnd)
        {
          last.m_fragmentEnd = item.m_fragmentEnd;
        }
      else
        {
          m_itemList.push_back (item);
        }
    }
}

void 
ItemList::Print (std::ostream &os, ns3::Buffer buffer, const ns3::PacketPrinter &printer) const
{
  NS_ASSERT (!m_itemList.empty ());
  uint32_t totalSize = 0;
  for (std::list<ItemList::Item>::const_iterator i = m_itemList.begin (); 
       i != m_itemList.end (); i++)
    {
      ItemList::Item item = *i;
      totalSize += item.m_fragmentEnd - item.m_fragmentStart;
    }
  NS_ASSERT (totalSize == buffer.GetSize ());
  uint32_t offset = 0;
  for (std::list<ItemList::Item>::const_iterator i = m_itemList.begin (); 
       i != m_itemList.end (); i++)
    {
      ItemList::Item item = *i;
      if (item.m_type == ItemList::PAYLOAD)
        {
          printer.PrintPayload (os, item.m_packetUid, item.m_size, item.m_fragmentStart, item.m_fragmentEnd);
        }
      else if (item.m_fragmentStart != 0 || 
               item.m_fragmentEnd != item.m_size)
        {
          printer.PrintChunkFragment (item.m_chunkType, os, item.m_packetUid, item.m_size, 
                                      item.m_fragmentStart, item.m_fragmentEnd);
        }
      else if (item.m_type == ItemList::HEADER)
        {
          ns3::Buffer::Iterator j = buffer.Begin ();
          j.Next (offset);
          printer.PrintChunk (item.m_chunkType, j, os, item.m_packetUid, item.m_size);
        }
      else if (item.m_type == ItemList::TRAILER)
        {
          ns3::Buffer::Iterator j = buffer.End ();
          j.Prev (totalSize - (offset + item.m_size));
          printer.PrintChunk (item.m_chunkType, j, os, item.m_packetUid, item.m_size);
        }
      else 
        {
          NS_ASSERT (false);
        }
      offset += item.m_fragmentEnd - item.m_fragmentStart;
    }
}

} // anonymous namespace

namespace ns3 {

bool PacketHistory::m_enable = false;
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
      AppendOneCommand (PacketHistory::INIT,
                        size, uid);
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
          AppendValue (type);
          AppendValue (data0);
          AppendValue (data1);
          m_n++;
          return;
        }
      else
        {
          uint8_t *buffer = &(m_data->m_data[m_end]);
          uint32_t lastType = ReadForwardValue (&buffer);
          if (lastType == type)
            {
              uint32_t lastData = ReadForwardValue (&buffer);
              if (lastData == data0)
                {
                  lastData = ReadForwardValue (&buffer);
                  if (lastData == data1)
                    {
                      return;
                    }
                }
            }
        }
    }
  Reserve (n);
  AppendValue (type);
  AppendValue (data0);
  AppendValue (data1);
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
          AppendValue (type);
          AppendValue (data);
          m_n++;
          return;
        }
      else
        {
          uint8_t *buffer = &(m_data->m_data[m_end]);
          uint32_t lastType = ReadForwardValue (&buffer);
          if (lastType == type)
            {
              uint32_t lastData = ReadForwardValue (&buffer);
              if (lastData == data)
                {
                  return;
                }
            }
        }
    }
  Reserve (n);
  AppendValue (type);
  AppendValue (data);
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
      result = 0xffffffff;
    }
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
      n += GetUleb128Size (o.m_end); 
      n += o.m_end;
      Reserve (n);
      memcpy (&m_data->m_data[m_end], o.m_data->m_data, o.m_end);
      m_end += o.m_end;
      if (m_end > m_data->m_dirtyEnd)
        {
          m_data->m_dirtyEnd = m_end;
        }
      AppendOneCommand (PacketHistory::ADD_AT_END, o.m_end);
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
PacketHistory::PrintComplex (std::ostream &os, Buffer buffer, const PacketPrinter &printer) const
{
  // we need to build a linked list of the different fragments 
  // which are stored in this packet.
  uint8_t *dataBuffer = &m_data->m_data[0];
  ItemList itemList;
  BuildItemList (&itemList, dataBuffer, m_end);
  itemList.Print (os, buffer, printer);
}

void 
PacketHistory::BuildItemList (ItemList *list, uint8_t *buffer, uint32_t size) const
{
  // we need to build a linked list of the different fragments 
  // which are stored in this packet.
  uint8_t *dataBuffer = buffer;
  for (uint32_t i = 0; i < m_n; i++)
    {
      uint32_t type = ReadForwardValue (&dataBuffer);
      uint32_t data = ReadForwardValue (&dataBuffer);
      switch (type)
        {
        case INIT: {
          uint32_t uid = ReadForwardValue (&dataBuffer);
          list->InitPayload (uid, data);
        } break;
        case ADD_HEADER: {
          uint32_t size = ReadForwardValue (&dataBuffer);
          list->AddHeader (data, size);
        } break;
        case REM_HEADER: {
          uint32_t size = ReadForwardValue (&dataBuffer);
          list->RemHeader (data, size);
        } break;
        case ADD_TRAILER: {
          uint32_t size = ReadForwardValue (&dataBuffer);
          list->AddTrailer (data, size);
        } break;
        case REM_TRAILER: {
          uint32_t size = ReadForwardValue (&dataBuffer);
          list->RemTrailer (data, size);
        } break;
        case ADD_AT_END: {
          ItemList other;
          BuildItemList (&other, dataBuffer, data);
          list->AddAtEnd (&other);
        } break;
        case REM_AT_START: {
          list->RemAtStart (data);
        } break;
        case REM_AT_END: {
          list->RemAtEnd (data);
        } break;
        case PADDING_AT_END:
          break;
        }
    }  
}

void 
PacketHistory::PrintDefault (std::ostream &os, Buffer buffer) const
{
  Print (os, buffer, PacketPrinter::GetDefault ());
}

void
PacketHistory::Print (std::ostream &os, Buffer buffer, const PacketPrinter &printer) const
{
  if (!m_enable) 
    {
      return;
    }

  PrintComplex (os, buffer, printer);
}



}; // namespace ns3

#include <stdarg.h>
#include <iostream>
#include <sstream>
#include "ns3/test.h"
#include "header.h"
#include "trailer.h"
#include "packet.h"

namespace ns3 {

template <int N>
class HistoryHeader : public Header
{
public:
  HistoryHeader ();
  bool IsOk (void) const;
private:
  virtual std::string DoGetName (void) const;
  virtual void PrintTo (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void SerializeTo (Buffer::Iterator start) const;
  virtual uint32_t DeserializeFrom (Buffer::Iterator start);
  bool m_ok;
};

template <int N>
HistoryHeader<N>::HistoryHeader ()
  : m_ok (false)
{}

template <int N>
bool 
HistoryHeader<N>::IsOk (void) const
{
  return m_ok;
}

template <int N>
std::string 
HistoryHeader<N>::DoGetName (void) const
{
  std::ostringstream oss;
  oss << N;
  return oss.str ();
}

template <int N>
void 
HistoryHeader<N>::PrintTo (std::ostream &os) const
{
  NS_ASSERT (false);
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
  m_ok = true;
  for (int i = 0; i < N; i++)
    {
      if (start.ReadU8 () != N)
        {
          m_ok = false;
        }
    }
  return N;
}

template <int N>
class HistoryTrailer : public Trailer
{
public:
  HistoryTrailer ();
  bool IsOk (void) const;
private:
  virtual std::string DoGetName (void) const;
  virtual void PrintTo (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void SerializeTo (Buffer::Iterator start) const;
  virtual uint32_t DeserializeFrom (Buffer::Iterator start);
  bool m_ok;
};

template <int N>
HistoryTrailer<N>::HistoryTrailer ()
  : m_ok (false)
{}

template <int N>
bool
HistoryTrailer<N>::IsOk (void) const
{
  return m_ok;
}

template <int N>
std::string 
HistoryTrailer<N>::DoGetName (void) const
{
  std::ostringstream oss;
  oss << N;
  return oss.str ();
}
template <int N>
void 
HistoryTrailer<N>::PrintTo (std::ostream &os) const
{
  NS_ASSERT (false);
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
  m_ok = true;
  start.Prev (N);
  for (int i = 0; i < N; i++)
    {
      if (start.ReadU8 () != N)
        {
          m_ok = false;
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
private:
  template <int N>
  void PrintHeader (std::ostream &os, uint32_t packetUid, uint32_t size, const HistoryHeader<N> *header);
  template <int N>
  void PrintTrailer (std::ostream &os, uint32_t packetUid, uint32_t size, const HistoryTrailer<N> *trailer);
  void PrintFragment (std::ostream &os,uint32_t packetUid,
                      uint32_t size,std::string & name, 
                      struct PacketPrinter::FragmentInformation info);
  void PrintDefault (std::ostream& os,uint32_t packetUid,
                     uint32_t size,std::string& name,
                     struct PacketPrinter::FragmentInformation info);
  void PrintPayload (std::ostream &os,uint32_t packetUid,
                     uint32_t size,
                     struct PacketPrinter::FragmentInformation info);
  template <int N>
  void RegisterHeader (void);
  template <int N>
  void RegisterTrailer (void);
  void CleanupPrints (void);


  bool m_headerError;
  bool m_trailerError;
  std::list<int> m_prints;
  PacketPrinter m_printer;
};

PacketHistoryTest::PacketHistoryTest ()
  : Test ("PacketHistory")
{
  m_printer.AddPayloadPrinter (MakeCallback (&PacketHistoryTest::PrintPayload, this));
  m_printer.AddDefaultPrinter (MakeCallback (&PacketHistoryTest::PrintDefault, this));
}

PacketHistoryTest::~PacketHistoryTest ()
{}

template <int N>
void 
PacketHistoryTest::RegisterHeader (void)
{
  static bool registered = false;
  if (!registered)
    {
      m_printer.AddPrinter (MakeCallback (&PacketHistoryTest::PrintHeader<N>, this),
                            MakeCallback (&PacketHistoryTest::PrintFragment, this));
      registered = true;
    }
}

template <int N>
void 
PacketHistoryTest::RegisterTrailer (void)
{
  static bool registered = false;
  if (!registered)
    {
      m_printer.AddPrinter (MakeCallback (&PacketHistoryTest::PrintTrailer<N>, this),
                            MakeCallback (&PacketHistoryTest::PrintFragment, this));
      registered = true;
    }
}


template <int N>
void 
PacketHistoryTest::PrintHeader (std::ostream &os, uint32_t packetUid, uint32_t size, 
                                const HistoryHeader<N> *header)
{
  if (!header->IsOk ())
    {
      m_headerError = true;
    }
  m_prints.push_back (N);
}

template <int N>
void 
PacketHistoryTest::PrintTrailer (std::ostream &os, uint32_t packetUid, uint32_t size, 
                                 const HistoryTrailer<N> *trailer)
{
  if (!trailer->IsOk ())
    {
      m_trailerError = true;
    }
  m_prints.push_back (N);
}
void 
PacketHistoryTest::PrintFragment (std::ostream &os,uint32_t packetUid,
                                  uint32_t size,std::string & name, 
                                  struct PacketPrinter::FragmentInformation info)
{
  m_prints.push_back (info.end - info.start);
}
void 
PacketHistoryTest::PrintDefault (std::ostream& os,uint32_t packetUid,
                     uint32_t size,std::string& name,
                     struct PacketPrinter::FragmentInformation info)
{
  NS_ASSERT (false);
}
void 
PacketHistoryTest::PrintPayload (std::ostream &os,uint32_t packetUid,
                                 uint32_t size,
                                 struct PacketPrinter::FragmentInformation info)
{
  m_prints.push_back (info.end - info.start);
}


void 
PacketHistoryTest::CleanupPrints (void)
{
  m_prints.clear ();
}

bool 
PacketHistoryTest::CheckHistory (Packet p, char *file, int line, uint32_t n, ...)
{
  m_headerError = false;
  m_trailerError = false;
  va_list ap;
  p.Print (std::cerr, m_printer);
  va_start (ap, n);
  if (m_headerError)
    {
      std::cout << "PacketHistory header error. file=" << file 
                << ", line=" << line << std::endl;
      return false;
    }
  if (m_trailerError)
    {
      std::cout << "PacketHistory trailer error. file=" << file 
                << ", line=" << line << std::endl;
      return false;
    }
  if (n != m_prints.size ())
    {
      goto error;
    }
  for (std::list<int>::iterator i = m_prints.begin (); 
       i != m_prints.end (); i++)
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
  for (std::list<int>::iterator i = m_prints.begin (); 
       i != m_prints.end (); i++)
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
    RegisterHeader<n> ();                       \
    p.AddHeader (header);                       \
  }
#define ADD_TRAILER(p, n)                       \
  {                                             \
    HistoryTrailer<n> trailer;                  \
    RegisterTrailer<n> ();                      \
    p.AddTrailer (trailer);                     \
  }
#define REM_HEADER(p, n)                        \
  {                                             \
    HistoryHeader<n> header;                    \
    RegisterHeader<n> ();                       \
    p.RemoveHeader (header);                    \
  }
#define REM_TRAILER(p, n)                       \
  {                                             \
    HistoryTrailer<n> trailer;                  \
    RegisterTrailer<n> ();                      \
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
  CHECK_HISTORY (p, 2, 10, 100);

  p = Packet (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  CHECK_HISTORY (p, 4, 
                 3, 2, 1, 10);
  ADD_HEADER (p, 5);
  CHECK_HISTORY (p, 5, 
                 5, 3, 2, 1, 10);
  ADD_HEADER (p, 6);
  CHECK_HISTORY (p, 6, 
                 6, 5, 3, 2, 1, 10);

  p = Packet (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  REM_HEADER (p, 3);
  CHECK_HISTORY (p, 3, 
                 2, 1, 10);

  p = Packet (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  REM_HEADER (p, 3);
  REM_HEADER (p, 2);
  CHECK_HISTORY (p, 2, 
                 1, 10);

  p = Packet (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  REM_HEADER (p, 3);
  REM_HEADER (p, 2);
  REM_HEADER (p, 1);
  CHECK_HISTORY (p, 1, 10);

  p = Packet (10);
  ADD_HEADER (p, 1);
  ADD_HEADER (p, 2);
  ADD_HEADER (p, 3);
  p1 = p;
  REM_HEADER (p1, 3);
  REM_HEADER (p1, 2);
  REM_HEADER (p1, 1);
  CHECK_HISTORY (p1, 1, 10);
  CHECK_HISTORY (p, 4, 
                 3, 2, 1, 10);
  ADD_HEADER (p1, 1);
  ADD_HEADER (p1, 2);
  CHECK_HISTORY (p1, 3, 
                 2, 1, 10);
  CHECK_HISTORY (p, 4, 
                 3, 2, 1, 10);
  ADD_HEADER (p, 3);
  CHECK_HISTORY (p, 5, 
                 3, 3, 2, 1, 10);
  ADD_TRAILER (p, 4);
  CHECK_HISTORY (p, 6, 
                 3, 3, 2, 1, 10, 4);
  ADD_TRAILER (p, 5);
  CHECK_HISTORY (p, 7, 
                 3, 3, 2, 1, 10, 4, 5);
  REM_HEADER (p, 3);
  CHECK_HISTORY (p, 6, 
                 3, 2, 1, 10, 4, 5);
  REM_TRAILER (p, 5);
  CHECK_HISTORY (p, 5, 
                 3, 2, 1, 10, 4);
  p1 = p;
  REM_TRAILER (p, 4);
  CHECK_HISTORY (p, 4, 
                 3, 2, 1, 10);
  CHECK_HISTORY (p1, 5, 
                 3, 2, 1, 10, 4);
  p1.RemoveAtStart (3);
  CHECK_HISTORY (p1, 4, 
                 2, 1, 10, 4);
  p1.RemoveAtStart (2);
  CHECK_HISTORY (p1, 3, 
                 1, 10, 4);
  p1.RemoveAtEnd (4);
  CHECK_HISTORY (p1, 2, 
                 1, 10);
  p1.RemoveAtStart (1);
  CHECK_HISTORY (p1, 1, 10);

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
  CHECK_HISTORY (p, 5, 3, 10, 10, 6, 4);


  return ok;
}

static PacketHistoryTest g_packetHistoryTest;

}//namespace ns3
