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
#include "ns3/debug.h"
#include "packet-history.h"
#include "chunk.h"
#include "buffer.h"

NS_DEBUG_COMPONENT_DEFINE ("PacketHistory");

#define USE_ULEB 1

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
    uint32_t m_chunkUid;
  };
  std::list<Item> m_itemList;
  uint32_t m_packetUid;
  uint32_t m_chunkUid;
};


void 
ItemList::InitPayload (uint32_t packetUid, uint32_t size)
{
  NS_ASSERT (m_itemList.empty ());
  m_packetUid = packetUid;
  m_chunkUid = 1;
  if (size > 0)
    {
      struct Item item;
      item.m_type = ItemList::PAYLOAD;
      item.m_chunkType = 0;
      item.m_size = size;
      item.m_fragmentStart = 0;
      item.m_fragmentEnd = item.m_size;
      item.m_packetUid = m_packetUid;
      item.m_chunkUid = m_chunkUid;
      m_itemList.push_back (item);
    }
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
  item.m_packetUid = m_packetUid;
  item.m_chunkUid = m_chunkUid;
  m_itemList.push_front (item);
  m_chunkUid++;
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
  item.m_packetUid = m_packetUid;
  item.m_chunkUid = m_chunkUid;
  m_itemList.push_back (item);
  m_chunkUid++;
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
      uint32_t itemRealSize = item.m_fragmentEnd - item.m_fragmentStart;
      if (itemRealSize <= leftToRemove)
        {
          m_itemList.pop_front ();
          leftToRemove -= itemRealSize;
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
      uint32_t itemRealSize = item.m_fragmentEnd - item.m_fragmentStart;
      if (itemRealSize <= leftToRemove)
        {
          m_itemList.pop_back ();
          leftToRemove -= itemRealSize;
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
  ItemList::Item &last = m_itemList.back ();
  for (std::list<ItemList::Item>::const_iterator i = other->m_itemList.begin (); 
       i != other->m_itemList.end (); i++)
    {
      const ItemList::Item &item = *i;
      if (item.m_packetUid == last.m_packetUid &&
          item.m_type == last.m_type &&
          item.m_chunkType == last.m_chunkType &&
          item.m_size == last.m_size &&
          last.m_fragmentEnd != last.m_size && 
          item.m_fragmentStart == last.m_fragmentEnd &&
          item.m_chunkUid == last.m_chunkUid)
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
uint16_t PacketHistory::m_chunkUid = 0;
PacketHistory::DataFreeList PacketHistory::m_freeList;
uint32_t g_nAllocs = 0;
uint32_t g_nDeAllocs = 0;
uint32_t g_nRecycle = 0;
uint32_t g_nCreate = 0;
  uint32_t g_one = 0;
  uint32_t g_two = 0;
  uint32_t g_three = 0;
  uint32_t g_four = 0;
  uint32_t g_five = 0;

void 
PacketHistory::PrintStats (void)
{
  std::cout << "allocs="<<g_nAllocs<<", deallocs="<<g_nDeAllocs
            <<", recycle="<<g_nRecycle<<", create="<<g_nCreate<<std::endl;
  std::cout << "one="<<g_one<<", two="<<g_two<<", three="<<g_three<<std::endl;
  //std::cout << "four="<<g_four<<", five="<<g_five<<std::endl;
  g_nAllocs = 0;
  g_nDeAllocs = 0;
  g_nRecycle = 0;
  g_nCreate = 0;
  g_one = 0;
  g_two = 0;
  g_three = 0;
  g_four = 0;
  g_five = 0;
#if 0
  PacketHistory h (0, 0);
  for (uint32_t i = 0; i < 0xffffff; i++)
    {
      if (h.GetUleb128Size (i) == 4)
        {
          std::cout << i << std::endl;
          break;
        }
    }
#endif
}

void 
PacketHistory::Enable (void)
{
  m_enable = true;
}

PacketHistory::PacketHistory (uint32_t uid, uint32_t size)
  : m_data (0),
    m_head (0xffff),
    m_tail (0xffff),
    m_used (0),
    m_packetUid (uid)
{
  if (size > 0)
    {
      uint16_t written = AddSmall (true, 0, size);
      Update (true, written);
    }
}
PacketHistory::PacketHistory (PacketHistory const &o)
  : m_data (o.m_data),
    m_head (o.m_head),
    m_tail (o.m_tail),
    m_used (o.m_used),
    m_packetUid (o.m_packetUid)
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
  m_head = o.m_head;
  m_tail = o.m_tail;
  m_used = o.m_used;
  m_packetUid = o.m_packetUid;
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

void
PacketHistory::ReserveCopy (uint32_t size)
{
  struct PacketHistory::Data *newData = PacketHistory::Create (m_used + size);
  memcpy (newData->m_data, m_data->m_data, m_used);
  newData->m_dirtyEnd = m_used;
  m_data->m_count--;
  if (m_data->m_count == 0) 
    {
      PacketHistory::Recycle (m_data);
    }
  m_data = newData;
  if (m_head != 0xffff)
    {
      uint8_t *start;
      NS_ASSERT (m_tail != 0xffff);
      // clear the next field of the tail
      start = &m_data->m_data[m_tail];
      Append16 (0xffff, start);
      // clear the prev field of the head
      start = &m_data->m_data[m_head] + 2;
      Append16 (0xffff, start);
    }
}
void
PacketHistory::Reserve (uint32_t size)
{
  NS_ASSERT (m_data != 0);
  if (m_data->m_size >= m_used + size &&
      (m_data->m_count == 1 ||
       m_data->m_dirtyEnd == m_used))
    {
      /* enough room, not dirty. */
      g_four++;
    }
  else 
    {
      /* (enough room and dirty) or (not enough room) */
      ReserveCopy (size);
      g_five++;
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
PacketHistory::ReadUleb128 (const uint8_t **pBuffer) const
{
  const uint8_t *buffer = *pBuffer;
  uint32_t result = 0;
  uint8_t byte;
  result = 0;
  byte = buffer[0];
  result = (byte & (~0x80));
  if (!(byte & 0x80))
    {
      *pBuffer = buffer + 1;
      return result;
    }
  byte = buffer[1];
  result = (byte & (~0x80)) << 7;
  if (!(byte & 0x80))
    {
      *pBuffer = buffer + 2;
      return result;
    }
  byte = buffer[2];
  result = (byte & (~0x80)) << 14;
  if (!(byte & 0x80))
    {
      *pBuffer = buffer + 3;
      return result;
    }
  byte = buffer[3];
  result = (byte & (~0x80)) << 21;
  if (!(byte & 0x80))
    {
      *pBuffer = buffer + 4;
      return result;
    }
  byte = buffer[4];
  result = (byte & (~0x80)) << 28;
  if (!(byte & 0x80))
    {
      *pBuffer = buffer + 5;
      return result;
    }
  /* This means that the LEB128 number was not valid.
   * ie: the last (5th) byte did not have the high-order bit zeroed.
   */
  NS_ASSERT (false);
  return 0;
}

void
PacketHistory::Append16 (uint16_t value, uint8_t *buffer)
{
  buffer[0] = value & 0xff;
  value >>= 8;
  buffer[1] = value;
}
bool
PacketHistory::TryToAppendFast (uint32_t value, uint8_t **pBuffer, uint8_t *end)
{
  uint8_t *start = *pBuffer;
  if (value < 0x80 && start < end)
    {
      start[0] = value;
      *pBuffer = start + 1;
      return true;
    }
  if (value < 0x4000 && start + 1 < end)
    {
      uint8_t byte = value & (~0x80);
      start[0] = 0x80 | byte;
      value >>= 7;
      start[1] = value;
      *pBuffer = start + 2;
      return true;
    }
  return false;
}
bool
PacketHistory::TryToAppend (uint32_t value, uint8_t **pBuffer, uint8_t *end)
{
  uint8_t *start = *pBuffer;
  if (value < 0x80 && start < end)
    {
      start[0] = value;
      *pBuffer = start + 1;
      return true;
    }
  if (value < 0x4000 && start + 1 < end)
    {
      uint8_t byte = value & (~0x80);
      start[0] = 0x80 | byte;
      value >>= 7;
      start[1] = value;
      *pBuffer = start + 2;
      return true;
    }
  if (value < 0x200000 && start + 2 < end)
    {
      uint8_t byte = value & (~0x80);
      start[0] = 0x80 | byte;
      value >>= 7;
      start[1] = 0x80 | byte;
      value >>= 7;
      start[2] = value;
      *pBuffer = start + 3;
      return true;
    }
  if (start + 3 < end)
    {
      uint8_t byte = value & (~0x80);
      start[0] = 0x80 | byte;
      value >>= 7;
      start[1] = 0x80 | byte;
      value >>= 7;
      start[2] = 0x80 | byte;
      value >>= 7;
      start[3] = value;
      *pBuffer = start + 4;
      return true;
    }
  return false;
}

void
PacketHistory::Update (bool atStart, uint16_t written)
{
  if (m_head == 0xffff)
    {
      NS_ASSERT (m_tail == 0xffff);
      m_head = m_used;
      m_tail = m_used;
    } 
  else if (atStart)
    {
      NS_ASSERT (m_head != 0xffff);
      // overwrite the prev field of the previous head of the list.
      uint8_t *previousHead = &m_data->m_data[m_head] + 2;
      Append16 (m_used, previousHead);
      // update the head of list to the new node.
      m_head = m_used;
    }
  else
    {
      NS_ASSERT (m_tail != 0xffff);
      // overwrite the next field of the previous tail of the list.
      uint8_t *previousTail = &m_data->m_data[m_tail];
      Append16 (m_used, previousTail);
      // update the tail of the list to the new node.
      m_tail = m_used;
    }
  NS_ASSERT (m_tail != 0xffff);
  NS_ASSERT (m_head != 0xffff);
  m_used += written;
  m_data->m_dirtyEnd = m_used;
}

uint16_t
PacketHistory::AddSmall (bool atStart,
                         uint32_t typeUid, uint32_t size)
{
  if (m_data == 0)
    {
      m_data = PacketHistory::Create (10);
      memset (m_data->m_data, 0xff, 4);
    }
  NS_ASSERT (m_data != 0);
  uint16_t chunkUid = m_chunkUid;
  m_chunkUid++;
 append:
  uint8_t *start = &m_data->m_data[m_used];
  uint8_t *end = &m_data->m_data[m_data->m_size];
  if (end - start >= 7 &&
      (m_head == 0xffff ||
       m_data->m_count == 1 ||
       m_used == m_data->m_dirtyEnd))
    {
      uint8_t *buffer = start;
      uint16_t next, prev;
      if (atStart)
        {
          next = m_head;
          prev = 0xffff;
        }
      else
        {
          next = 0xffff;
          prev = m_tail;
        }

      Append16 (next, buffer);
      buffer += 2;
      Append16 (prev, buffer);
      buffer += 2;
      if (TryToAppendFast (typeUid, &buffer, end) &&
          TryToAppendFast (size, &buffer, end) &&
          TryToAppend (chunkUid, &buffer, end))
        {
          uintptr_t written = buffer - start;
          NS_ASSERT (written <= 0xffff);
          g_one++;
          return written;
        }
    }
  g_two++;
  uint32_t n = GetUleb128Size (typeUid);
  n += GetUleb128Size (size);
  n += GetUleb128Size (chunkUid);
  n += 2 + 2;
  ReserveCopy (n);
  goto append;
}

uint16_t
PacketHistory::AddBig (bool atStart,
                       const PacketHistory::SmallItem *item, 
                       const PacketHistory::ExtraItem *extraItem)
{
  if (m_data == 0)
    {
      m_data = PacketHistory::Create (10);
      memset (m_data->m_data, 0xff, 4);
    }
  NS_ASSERT (m_data != 0);
  uint32_t typeUid = ((item->typeUid & 0x1) == 0x1)?item->typeUid:item->typeUid+1;
 append:
  uint8_t *start = &m_data->m_data[m_used];
  uint8_t *end = &m_data->m_data[m_data->m_size];
  if (end - start >= 10 &&
      (m_head == 0xffff ||
       m_data->m_count == 1 ||
       m_used == m_data->m_dirtyEnd))
    {
      uint8_t *buffer = start;
      uint16_t next, prev;
      if (atStart)
        {
          next = m_head;
          prev = 0xffff;
        }
      else
        {
          next = 0xffff;
          prev = m_tail;
        }

      Append16 (next, buffer);
      buffer += 2;
      Append16 (prev, buffer);
      buffer += 2;
      if (TryToAppend (typeUid, &buffer, end) &&
          TryToAppend (item->size, &buffer, end) &&
          TryToAppend (item->chunkUid, &buffer, end) &&
          TryToAppend (extraItem->fragmentStart, &buffer, end) &&
          TryToAppend (extraItem->fragmentEnd, &buffer, end) &&
          TryToAppend (extraItem->packetUid, &buffer, end))
        {
          uintptr_t written = buffer - start;
          NS_ASSERT (written <= 0xffff);
          return written;
        }
    }
  
  uint32_t n = GetUleb128Size (typeUid);
  n += GetUleb128Size (item->size);
  n += GetUleb128Size (item->chunkUid);
  n += GetUleb128Size (extraItem->fragmentStart);
  n += GetUleb128Size (extraItem->fragmentEnd);
  n += GetUleb128Size (extraItem->packetUid);
  n += 2 + 2;
  ReserveCopy (n);
  goto append;
}

void
PacketHistory::ReplaceTail (const PacketHistory::SmallItem *item, 
                            const PacketHistory::ExtraItem *extraItem,
                            uint32_t available)
{
  NS_ASSERT (m_data != 0);  
  if (available >= 10 &&
      m_data->m_count == 1)
    {
      uint8_t *buffer = &m_data->m_data[m_tail];
      uint8_t *end = buffer + available;

      Append16 (item->next, buffer);
      buffer += 2;
      Append16 (item->prev, buffer);
      buffer += 2;
      if (TryToAppend (item->typeUid, &buffer, end) &&
          TryToAppend (item->size, &buffer, end) &&
          TryToAppend (item->chunkUid, &buffer, end) &&
          TryToAppend (extraItem->fragmentStart, &buffer, end) &&
          TryToAppend (extraItem->fragmentEnd, &buffer, end) &&
          TryToAppend (extraItem->packetUid, &buffer, end))
        {
          m_used = buffer - &m_data->m_data[0];
          m_data->m_dirtyEnd = m_used;
          return;
        }
    }
  
  // create a copy of the packet.
  PacketHistory h (m_packetUid, 0);
  uint16_t current = m_head;
  while (current != 0xffff && current != m_tail)
    {
      struct PacketHistory::SmallItem tmpItem;
      PacketHistory::ExtraItem tmpExtraItem;
      ReadItems (current, &tmpItem, &tmpExtraItem);
      uint16_t written = h.AddBig (false, &tmpItem, &tmpExtraItem);
      h.Update (false, written);
    }
  // append new tail.
  uint16_t written = h.AddBig (false, item, extraItem);
  h.Update (false, written);

  *this = h;
}



void
PacketHistory::ReadSmall (struct PacketHistory::SmallItem *item, const uint8_t **pBuffer) const
{
  const uint8_t *buffer = *pBuffer;
  item->next = buffer[0];
  item->next |= (buffer[1]) << 8;
  item->prev = buffer[2];
  item->prev |= (buffer[3]) << 8;
  *pBuffer = *pBuffer + 4;
  item->typeUid = ReadUleb128 (pBuffer);
  item->size = ReadUleb128 (pBuffer);
  item->chunkUid = ReadUleb128 (pBuffer);
}

uint32_t
PacketHistory::ReadItems (uint16_t current, 
                          struct PacketHistory::SmallItem *item,
                          struct PacketHistory::ExtraItem *extraItem) const
{
  const uint8_t *buffer = &m_data->m_data[current];
  ReadSmall (item, &buffer);
  bool isExtra = (item->typeUid & 0x1) == 0x1;
  if (isExtra)
    {
      ReadExtra (extraItem, &buffer);
    }
  else
    {
      extraItem->fragmentStart = 0;
      extraItem->fragmentEnd = item->size;
      extraItem->packetUid = m_packetUid;
    }
  NS_ASSERT (buffer <= &m_data->m_data[m_data->m_size]);
  return buffer - &m_data->m_data[current];
}


void
PacketHistory::ReadExtra (struct PacketHistory::ExtraItem *item, const uint8_t **pBuffer) const
{
  item->fragmentStart = ReadUleb128 (pBuffer);
  item->fragmentEnd = ReadUleb128 (pBuffer);
  item->packetUid = ReadUleb128 (pBuffer);
}


struct PacketHistory::Data *
PacketHistory::Create (uint32_t size)
{
  g_nCreate++;
  NS_DEBUG ("create size="<<size<<", max="<<m_maxSize);
  if (size > m_maxSize)
    {
      m_maxSize = size;
    }
  while (!m_freeList.empty ()) 
    {
      struct PacketHistory::Data *data = m_freeList.back ();
      m_freeList.pop_back ();
      if (data->m_size >= size) 
        {
          NS_DEBUG ("create found size="<<data->m_size);
          data->m_count = 1;
          return data;
        }
      PacketHistory::Deallocate (data);
      NS_DEBUG ("create dealloc size="<<data->m_size);
    }
  NS_DEBUG ("create alloc size="<<m_maxSize);
  return PacketHistory::Allocate (m_maxSize);
}

void
PacketHistory::Recycle (struct PacketHistory::Data *data)
{
  g_nRecycle++;
  NS_DEBUG ("recycle size="<<data->m_size<<", list="<<m_freeList.size ());
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

struct PacketHistory::Data *
PacketHistory::Allocate (uint32_t n)
{
  g_nAllocs++;
  uint32_t size = sizeof (struct Data);
  if (n <= 10)
    {
      n = 10;
    }
  size += n - 10;
  uint8_t *buf = new uint8_t [size];
  struct PacketHistory::Data *data = (struct PacketHistory::Data *)buf;
  data->m_size = n;
  data->m_count = 1;
  data->m_dirtyEnd = 0;
  return data;
}
void 
PacketHistory::Deallocate (struct PacketHistory::Data *data)
{
  g_nDeAllocs++;
  uint8_t *buf = (uint8_t *)data;
  delete [] buf;
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
  if (!m_enable)
    {
      return;
    }
  uint16_t written = AddSmall (true, uid, size);
  Update (true, written);
}
void 
PacketHistory::RemoveHeader (uint32_t uid, Chunk const & header, uint32_t size)
{
  if (!m_enable) 
    {
      return;
    }
  if (m_data == 0)
    {
      NS_FATAL_ERROR ("Removing header from empty packet.");
    }
  struct PacketHistory::SmallItem item;
  const uint8_t *buffer = &m_data->m_data[m_head];
  ReadSmall (&item, &buffer);
  NS_ASSERT (buffer <= &m_data->m_data[m_data->m_size]);
  if ((item.typeUid & 0xfffffffe) != uid ||
      item.size != size)
    {
      NS_FATAL_ERROR ("Removing unexpected header.");
    }
  else if (item.typeUid != uid)
    {
      // this is a "big" item
      struct PacketHistory::ExtraItem extraItem;
      ReadExtra (&extraItem, &buffer);
      NS_ASSERT (buffer <= &m_data->m_data[m_data->m_size]);
      if (extraItem.fragmentStart != 0 ||
          extraItem.fragmentEnd != size)
        {
          NS_FATAL_ERROR ("Removing incomplete header.");
        }
    }
  m_head = item.next;
  if (m_head > m_tail)
    {
      m_used = m_head;
    }
}
void 
PacketHistory::AddTrailer (uint32_t uid, Chunk const & trailer, uint32_t size)
{
  if (!m_enable)
    {
      return;
    }
  uint16_t written = AddSmall (false, uid, size);
  Update (false, written);
}
void 
PacketHistory::RemoveTrailer (uint32_t uid, Chunk const & trailer, uint32_t size)
{
  if (!m_enable) 
    {
      return;
    }
  if (m_data == 0)
    {
      NS_FATAL_ERROR ("Removing trailer from empty packet.");
    }
  struct PacketHistory::SmallItem item;
  const uint8_t *buffer = &m_data->m_data[m_tail];
  ReadSmall (&item, &buffer);
  NS_ASSERT (buffer <= &m_data->m_data[m_data->m_size]);
  if ((item.typeUid & 0xfffffffe) != uid ||
      item.size != size)
    {
      NS_FATAL_ERROR ("Removing unexpected trailer.");
    }
  else if (item.typeUid != uid)
    {
      // this is a "big" item
      struct PacketHistory::ExtraItem extraItem;
      ReadExtra (&extraItem, &buffer);
      NS_ASSERT (buffer <= &m_data->m_data[m_data->m_size]);
      if (extraItem.fragmentStart != 0 ||
          extraItem.fragmentEnd != size)
        {
          NS_FATAL_ERROR ("Removing incomplete trailer.");
        }
    }
  m_tail = item.prev;
  if (m_tail > m_head)
    {
      m_used = m_tail;
    }
}
void
PacketHistory::AddAtEnd (PacketHistory const&o)
{
  if (!m_enable) 
    {
      return;
    }
  if (m_data == 0 || m_tail == 0xffff)
    {
      *this = o;
      return;
    }
  NS_ASSERT (m_head != 0xffff && m_tail != 0xffff);

  uint16_t lastTail;
  lastTail = m_tail;
  struct PacketHistory::SmallItem lastItem;
  PacketHistory::ExtraItem lastExtraItem;
  uint32_t lastTailSize = ReadItems (m_tail, &lastItem, &lastExtraItem);
  if (m_tail + lastTailSize == m_used &&
      m_used == m_data->m_dirtyEnd)
    {
      lastTailSize = m_data->m_size - m_tail;
    }

  uint16_t current = o.m_head;
  while (current != 0xffff)
    {
      struct PacketHistory::SmallItem item;
      PacketHistory::ExtraItem extraItem;
      o.ReadItems (current, &item, &extraItem);
      if (extraItem.packetUid == lastExtraItem.packetUid &&
          item.typeUid == lastItem.typeUid &&
          item.chunkUid == lastItem.chunkUid &&
          item.size == lastItem.size &&
          extraItem.fragmentStart == lastExtraItem.fragmentEnd)
        {
          // replace previous tail.
          lastExtraItem.fragmentEnd = extraItem.fragmentEnd;
          NS_ASSERT (m_tail == lastTail);
          ReplaceTail (&lastItem, &lastExtraItem, lastTailSize);
        }
      else
        {
          // append the extra items.
          uint16_t written = AddBig (false, &item, &extraItem);
          Update (false, written);
        }
      if (current == o.m_tail)
        {
          break;
        }
      current = item.next;
    }
}
void
PacketHistory::AddPaddingAtEnd (uint32_t end)
{
  if (!m_enable)
    {
      return;
    }
}
void 
PacketHistory::RemoveAtStart (uint32_t start)
{
  if (!m_enable) 
    {
      return;
    }
  if (m_data == 0)
    {
      NS_FATAL_ERROR ("Removing data from start of empty packet.");
    }

  uint32_t leftToRemove = start;
  uint16_t current = m_head;
  while (current != 0xffff && leftToRemove > 0)
    {
      struct PacketHistory::SmallItem item;
      PacketHistory::ExtraItem extraItem;
      ReadItems (current, &item, &extraItem);
      uint32_t itemRealSize = extraItem.fragmentEnd - extraItem.fragmentStart;
      if (itemRealSize <= leftToRemove)
        {
          // remove from list.
          m_head = item.next;
          leftToRemove -= itemRealSize;
        }
      else
        {
          // fragment the list item.
          PacketHistory fragment (m_packetUid, 0);
          extraItem.fragmentStart += leftToRemove;
          leftToRemove = 0;
          uint16_t written = fragment.AddBig (false, &item, &extraItem);
          Update (false, written);
          current = item.next;
          while (current != 0xffff)
            {
              ReadItems (current, &item, &extraItem);
              written = fragment.AddBig (false, &item, &extraItem);
              Update (false, written);
              if (current == m_tail)
                {
                  break;
                }
              current = item.next;
            }
          *this = fragment;
        }
      NS_ASSERT (item.size >= extraItem.fragmentEnd - extraItem.fragmentStart &&
                 extraItem.fragmentStart <= extraItem.fragmentEnd);
      if (current == m_tail)
        {
          break;
        }
      current = item.next;
    }
  NS_ASSERT (leftToRemove == 0);
}
void 
PacketHistory::RemoveAtEnd (uint32_t end)
{
  if (!m_enable) 
    {
      return;
    }
  if (m_data == 0)
    {
      NS_FATAL_ERROR ("Removing data from start of empty packet.");
    }

  uint32_t leftToRemove = end;
  uint16_t current = m_tail;
  while (current != 0xffff && leftToRemove > 0)
    {
      struct PacketHistory::SmallItem item;
      PacketHistory::ExtraItem extraItem;
      ReadItems (current, &item, &extraItem);
      uint32_t itemRealSize = extraItem.fragmentEnd - extraItem.fragmentStart;
      if (itemRealSize <= leftToRemove)
        {
          // remove from list.
          m_tail = item.prev;
          leftToRemove -= itemRealSize;
        }
      else
        {
          // fragment the list item.
          PacketHistory fragment (m_packetUid, 0);
          NS_ASSERT (extraItem.fragmentEnd > leftToRemove);
          extraItem.fragmentEnd -= leftToRemove;
          leftToRemove = 0;
          uint16_t written = fragment.AddBig (true, &item, &extraItem);
          Update (true, written);
          current = item.prev;
          while (current != 0xffff)
            {
              ReadItems (current, &item, &extraItem);
              written = fragment.AddBig (true, &item, &extraItem);
              Update (true, written);
              if (current == m_head)
                {
                  break;
                }
              current = item.prev;
            }
          *this = fragment;
        }
      NS_ASSERT (item.size >= extraItem.fragmentEnd - extraItem.fragmentStart &&
                 extraItem.fragmentStart <= extraItem.fragmentEnd);
      if (current == m_head)
        {
          break;
        }
      current = item.prev;
    }
  NS_ASSERT (leftToRemove == 0);
}

void 
PacketHistory::PrintDefault (std::ostream &os, Buffer buffer) const
{
  Print (os, buffer, PacketPrinter::GetDefault ());
}

uint32_t
PacketHistory::DoPrint (struct PacketHistory::SmallItem *item, uint32_t current,
                        Buffer data, uint32_t offset, const PacketPrinter &printer,
                        std::ostream &os) const
{
  PacketHistory::ExtraItem extraItem;
  ReadItems (current, item, &extraItem);
  uint32_t uid = item->typeUid & 0xfffffffe;
  if (uid == 0)
    {
      // payload.
      printer.PrintPayload (os, extraItem.packetUid, item->size, 
                            extraItem.fragmentStart, 
                            extraItem.fragmentEnd);
    }
  else if (extraItem.fragmentStart != 0 ||
           extraItem.fragmentEnd != item->size)
    {
      printer.PrintChunkFragment (uid, os, extraItem.packetUid, item->size, 
                                  extraItem.fragmentStart, extraItem.fragmentEnd);
    }
  else if (PacketPrinter::IsHeader (uid))
    {
      ns3::Buffer::Iterator j = data.Begin ();
      j.Next (offset);
      printer.PrintChunk (uid, j, os, extraItem.packetUid, item->size);
    }
  else if (PacketPrinter::IsTrailer (uid))
    {
      ns3::Buffer::Iterator j = data.End ();
      j.Prev (data.GetSize () - (offset + item->size));
      printer.PrintChunk (uid, j, os, extraItem.packetUid, item->size);
    }
  else 
    {
      NS_ASSERT (false);
    }
  return extraItem.fragmentEnd - extraItem.fragmentStart;
}

uint32_t
PacketHistory::GetTotalSize (void) const
{
  uint32_t totalSize = 0;
  uint16_t current = m_head;
  uint16_t tail = m_tail;
  while (current != 0xffff)
    {
      struct PacketHistory::SmallItem item;
      PacketHistory::ExtraItem extraItem;
      ReadItems (current, &item, &extraItem);
      totalSize += extraItem.fragmentEnd - extraItem.fragmentStart;
      if (current == tail)
        {
          break;
        }
      current = item.next;
    }
  return totalSize;
}

void
PacketHistory::Print (std::ostream &os, Buffer data, const PacketPrinter &printer) const
{
  if (!m_enable) 
    {
      return;
    }
  if (m_data == 0)
    {
      return;
    }
  NS_ASSERT (GetTotalSize () == data.GetSize ());
  if (printer.m_forward)
    {
      uint32_t tail = m_tail;
      uint32_t head = m_head;
      uint32_t current = head;
      uint32_t offset = 0;
      while (current != 0xffff)
        {
          struct PacketHistory::SmallItem item;
          uint32_t realSize = DoPrint (&item, current, data, offset, printer, os);
          offset += realSize;
          if (current == tail)
            {
              break;
            }
          current = item.next;
        }
    }
  else
    {
      uint32_t head = m_head;
      uint32_t tail = m_tail;
      uint32_t current = head;
      uint32_t offset = 0;
      while (current != 0xffff)
        {
          struct PacketHistory::SmallItem item;
          uint32_t realSize = DoPrint (&item, current, data, offset, printer, os);
          offset -= realSize;
          if (current == tail)
            {
              break;
            }
          current = item.prev;
        }
    }
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
  start.Prev (N);
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
      m_printer.AddHeaderPrinter (MakeCallback (&PacketHistoryTest::PrintHeader<N>, this),
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
      m_printer.AddTrailerPrinter (MakeCallback (&PacketHistoryTest::PrintTrailer<N>, this),
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
  p1.RemoveAtStart (1);
  CHECK_HISTORY (p1, 4, 
                 1, 1, 10, 4);
  p1.RemoveAtStart (1);
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

  p = Packet (10);
  ADD_HEADER (p, 10);
  ADD_TRAILER (p, 6);
  p.RemoveAtEnd (18);
  ADD_TRAILER (p, 5);
  ADD_HEADER (p, 3);
  CHECK_HISTORY (p, 3, 3, 8, 5);
  p.RemoveAtStart (12);
  CHECK_HISTORY (p, 1, 4);
  p.RemoveAtEnd (2);
  CHECK_HISTORY (p, 1, 2);
  ADD_HEADER (p, 10);
  CHECK_HISTORY (p, 2, 10, 2);
  p.RemoveAtEnd (5);
  CHECK_HISTORY (p, 1, 7);

  Packet p2 = Packet (0);
  Packet p3 = Packet (0);

  p = Packet (40);
  ADD_HEADER (p, 5);
  ADD_HEADER (p, 8);
  CHECK_HISTORY (p, 3, 8, 5, 40);
  p1 = p.CreateFragment (0, 5);
  p2 = p.CreateFragment (5, 5);
  p3 = p.CreateFragment (10, 43);
  CHECK_HISTORY (p1, 1, 5);
  CHECK_HISTORY (p2, 2, 3, 2);
  CHECK_HISTORY (p3, 2, 3, 40);
  p1.AddAtEnd (p2);
  CHECK_HISTORY (p1, 2, 8, 2);
  CHECK_HISTORY (p2, 2, 3, 2);
  p1.AddAtEnd (p3);
  CHECK_HISTORY (p1, 3, 8, 5, 40);
  CHECK_HISTORY (p2, 2, 3, 2);
  CHECK_HISTORY (p3, 2, 3, 40);
  p1 = p.CreateFragment (0, 5);
  CHECK_HISTORY (p1, 1, 5);

  p3 = Packet (50);
  ADD_HEADER (p3, 8);
  CHECK_HISTORY (p3, 2, 8, 50);
  CHECK_HISTORY (p1, 1, 5);
  p1.AddAtEnd (p3);
  CHECK_HISTORY (p1, 3, 5, 8, 50);
  ADD_HEADER (p1, 5);
  CHECK_HISTORY (p1, 4, 5, 5, 8, 50);
  ADD_TRAILER (p1, 2);
  CHECK_HISTORY (p1, 5, 5, 5, 8, 50, 2);
  REM_HEADER (p1, 5);
  CHECK_HISTORY (p1, 4, 5, 8, 50, 2);
  p1.RemoveAtEnd (60);
  CHECK_HISTORY (p1, 1, 5);
  p1.AddAtEnd (p2);
  CHECK_HISTORY (p1, 2, 8, 2);
  CHECK_HISTORY (p2, 2, 3, 2);

  p3 = Packet (40);
  ADD_HEADER (p3, 5);
  ADD_HEADER (p3, 5);
  CHECK_HISTORY (p3, 3, 5, 5, 40);
  p1 = p3.CreateFragment (0, 5);
  p2 = p3.CreateFragment (5, 5);
  CHECK_HISTORY (p1, 1, 5);
  CHECK_HISTORY (p2, 1, 5);
  p1.AddAtEnd (p2);
  CHECK_HISTORY (p1, 2, 5, 5);

  p = Packet (0);
  CHECK_HISTORY (p, 0);

  p3 = Packet (0);
  ADD_HEADER (p3, 5);
  ADD_HEADER (p3, 5);
  CHECK_HISTORY (p3, 2, 5, 5);
  p1 = p3.CreateFragment (0, 4);
  p2 = p3.CreateFragment (9, 1);
  CHECK_HISTORY (p1, 1, 4);
  CHECK_HISTORY (p2, 1, 1);
  p1.AddAtEnd (p2);
  CHECK_HISTORY (p1, 2, 4, 1);
  
  


  return ok;
}

static PacketHistoryTest g_packetHistoryTest;

}//namespace ns3
