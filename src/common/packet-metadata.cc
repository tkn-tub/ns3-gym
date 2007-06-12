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
#include "packet-metadata.h"
#include "chunk.h"
#include "buffer.h"

NS_DEBUG_COMPONENT_DEFINE ("PacketMetadata");

namespace ns3 {

bool PacketMetadata::m_enable = false;
uint32_t PacketMetadata::m_maxSize = 0;
uint16_t PacketMetadata::m_chunkUid = 0;
PacketMetadata::DataFreeList PacketMetadata::m_freeList;
bool g_optOne = false;

void 
PacketMetadata::Enable (void)
{
  m_enable = true;
}

void 
PacketMetadata::SetOptOne (bool optOne)
{
  g_optOne = optOne;
}

void
PacketMetadata::ReserveCopy (uint32_t size)
{
  struct PacketMetadata::Data *newData = PacketMetadata::Create (m_used + size);
  memcpy (newData->m_data, m_data->m_data, m_used);
  newData->m_dirtyEnd = m_used;
  m_data->m_count--;
  if (m_data->m_count == 0) 
    {
      PacketMetadata::Recycle (m_data);
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
PacketMetadata::Reserve (uint32_t size)
{
  NS_ASSERT (m_data != 0);
  if (m_data->m_size >= m_used + size &&
      (m_head == 0xffff ||
       m_data->m_count == 1 ||
       m_data->m_dirtyEnd == m_used))
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
PacketMetadata::GetUleb128Size (uint32_t value) const
{
  if (value < 0x80)
    {
      return 1;
    }
  if (value < 0x4000)
    {
      return 2;
    }
  if (value < 0x200000)
    {
      return 3;
    }
  if (value < 0x10000000)
    {
      return 4;
    }
  return 5;
}
uint32_t
PacketMetadata::ReadUleb128 (const uint8_t **pBuffer) const
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
  result |= (byte & (~0x80)) << 7;
  if (!(byte & 0x80))
    {
      *pBuffer = buffer + 2;
      return result;
    }
  byte = buffer[2];
  result |= (byte & (~0x80)) << 14;
  if (!(byte & 0x80))
    {
      *pBuffer = buffer + 3;
      return result;
    }
  byte = buffer[3];
  result |= (byte & (~0x80)) << 21;
  if (!(byte & 0x80))
    {
      *pBuffer = buffer + 4;
      return result;
    }
  byte = buffer[4];
  result |= (byte & (~0x80)) << 28;
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
PacketMetadata::Append16 (uint16_t value, uint8_t *buffer)
{
  buffer[0] = value & 0xff;
  value >>= 8;
  buffer[1] = value;
}
bool
PacketMetadata::TryToAppendFast (uint32_t value, uint8_t **pBuffer, uint8_t *end)
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
PacketMetadata::TryToAppend16 (uint16_t value,  uint8_t **pBuffer, uint8_t *end)
{
  uint8_t *start = *pBuffer;
  if (start + 1 < end)
    {
      start[0] = value & 0xff;
      start[1] = value >> 8;
      *pBuffer = start + 2;
      return true;
    }
  return false;
}
bool
PacketMetadata::TryToAppend32 (uint32_t value,  uint8_t **pBuffer, uint8_t *end)
{
  uint8_t *start = *pBuffer;
  if (start + 3 < end)
    {
      start[0] = value & 0xff;
      start[1] = (value >> 8) & 0xff;
      start[2] = (value >> 16) & 0xff;
      start[3] = (value >> 24) & 0xff;
      *pBuffer = start + 4;
      return true;
    }
  return false;
}
bool
PacketMetadata::TryToAppend (uint32_t value, uint8_t **pBuffer, uint8_t *end)
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
      byte = value & (~0x80);
      start[1] = 0x80 | byte;
      value >>= 7;
      byte = value & (~0x80);
      start[2] = value;
      *pBuffer = start + 3;
      return true;
    }
  if (value < 0x10000000 && start + 3 < end)
    {
      uint8_t byte = value & (~0x80);
      start[0] = 0x80 | byte;
      value >>= 7;
      byte = value & (~0x80);
      start[1] = 0x80 | byte;
      value >>= 7;
      byte = value & (~0x80);
      start[2] = 0x80 | byte;
      value >>= 7;
      start[3] = value;
      *pBuffer = start + 4;
      return true;
    }
  if (start + 4 < end)
    {
      uint8_t byte = value & (~0x80);
      start[0] = 0x80 | byte;
      value >>= 7;
      byte = value & (~0x80);
      start[1] = 0x80 | byte;
      value >>= 7;
      byte = value & (~0x80);
      start[2] = 0x80 | byte;
      value >>= 7;
      byte = value & (~0x80);
      start[3] = 0x80 | byte;
      value >>= 7;
      start[4] = value;
      *pBuffer = start + 5;
      return true;
    }
  return false;
}

void
PacketMetadata::AppendValueExtra (uint32_t value, uint8_t *buffer)
{
  if (value < 0x200000)
    {
      uint8_t byte = value & (~0x80);
      buffer[0] = 0x80 | byte;
      value >>= 7;
      byte = value & (~0x80);
      buffer[1] = 0x80 | byte;
      value >>= 7;
      byte = value & (~0x80);
      buffer[2] = value;
      return;
    }
  if (value < 0x10000000)
    {
      uint8_t byte = value & (~0x80);
      buffer[0] = 0x80 | byte;
      value >>= 7;
      byte = value & (~0x80);
      buffer[1] = 0x80 | byte;
      value >>= 7;
      byte = value & (~0x80);
      buffer[2] = 0x80 | byte;
      value >>= 7;
      buffer[3] = value;
      return;
    }
  {
    uint8_t byte = value & (~0x80);
    buffer[0] = 0x80 | byte;
    value >>= 7;
    byte = value & (~0x80);
    buffer[1] = 0x80 | byte;
    value >>= 7;
    byte = value & (~0x80);
    buffer[2] = 0x80 | byte;
    value >>= 7;
    byte = value & (~0x80);
    buffer[3] = 0x80 | byte;
    value >>= 7;
    buffer[4] = value;
  }
}

void
PacketMetadata::AppendValue (uint32_t value, uint8_t *buffer)
{
  if (value < 0x80)
    {
      buffer[0] = value;
      return;
    }
  if (value < 0x4000)
    {
      uint8_t byte = value & (~0x80);
      buffer[0] = 0x80 | byte;
      value >>= 7;
      buffer[1] = value;
      return;
    }
  AppendValueExtra (value, buffer);
}

void
PacketMetadata::UpdateTail (uint16_t written)
{
  if (m_head == 0xffff)
    {
      NS_ASSERT (m_tail == 0xffff);
      m_head = m_used;
      m_tail = m_used;
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


void
PacketMetadata::UpdateHead (uint16_t written)
{
  if (m_head == 0xffff)
    {
      NS_ASSERT (m_tail == 0xffff);
      m_head = m_used;
      m_tail = m_used;
    } 
  else
    {
      NS_ASSERT (m_head != 0xffff);
      // overwrite the prev field of the previous head of the list.
      uint8_t *previousHead = &m_data->m_data[m_head + 2];
      Append16 (m_used, previousHead);
      // update the head of list to the new node.
      m_head = m_used;
    }
  NS_ASSERT (m_tail != 0xffff);
  NS_ASSERT (m_head != 0xffff);
  m_used += written;
  m_data->m_dirtyEnd = m_used;
}

uint16_t
PacketMetadata::AddSmall (const struct PacketMetadata::SmallItem *item)
{
  NS_ASSERT (m_data != 0);
  if (g_optOne)
    {
      uint32_t typeUidSize = GetUleb128Size (item->typeUid);
      uint32_t sizeSize = GetUleb128Size (item->size);
      uint32_t n = typeUidSize + sizeSize + 2 + 2 + 2;
    restart:
      if (m_used + n <= m_data->m_size &&
      (m_head == 0xffff ||
       m_data->m_count == 1 ||
       m_used == m_data->m_dirtyEnd))
        {
          uint8_t *buffer = &m_data->m_data[m_used];
          Append16 (item->next, buffer);
          buffer += 2;
          Append16 (item->prev, buffer);
          buffer += 2;
          AppendValue (item->typeUid, buffer);
          buffer += typeUidSize;
          AppendValue (item->size, buffer);
          buffer += sizeSize;
          Append16 (item->chunkUid, buffer);
        }
      else
        {
          ReserveCopy (n);
          goto restart;
        }
      return n;
    }
 append:
  uint8_t *start = &m_data->m_data[m_used];
  uint8_t *end = &m_data->m_data[m_data->m_size];
  if (end - start >= 8 &&
      (m_head == 0xffff ||
       m_data->m_count == 1 ||
       m_used == m_data->m_dirtyEnd))
    {
      uint8_t *buffer = start;

      Append16 (item->next, buffer);
      buffer += 2;
      Append16 (item->prev, buffer);
      buffer += 2;
      if (TryToAppendFast (item->typeUid, &buffer, end) &&
          TryToAppendFast (item->size, &buffer, end) &&
          TryToAppend16 (item->chunkUid, &buffer, end))
        {
          uintptr_t written = buffer - start;
          NS_ASSERT (written <= 0xffff);
          return written;
        }
    }
  uint32_t n = GetUleb128Size (item->typeUid);
  n += GetUleb128Size (item->size);
  n += 2;
  n += 2 + 2;
  Reserve (n);
  goto append;
}

uint16_t
PacketMetadata::AddBig (uint32_t next, uint32_t prev, 
                       const PacketMetadata::SmallItem *item, 
                       const PacketMetadata::ExtraItem *extraItem)
{
  NS_ASSERT (m_data != 0);
  uint32_t typeUid = ((item->typeUid & 0x1) == 0x1)?item->typeUid:item->typeUid+1;
 append:
  uint8_t *start = &m_data->m_data[m_used];
  uint8_t *end = &m_data->m_data[m_data->m_size];
  if (end - start >= 14 &&
      (m_head == 0xffff ||
       m_data->m_count == 1 ||
       m_used == m_data->m_dirtyEnd))
    {
      uint8_t *buffer = start;

      Append16 (next, buffer);
      buffer += 2;
      Append16 (prev, buffer);
      buffer += 2;
      if (TryToAppend (typeUid, &buffer, end) &&
          TryToAppend (item->size, &buffer, end) &&
          TryToAppend16 (item->chunkUid, &buffer, end) &&
          TryToAppend (extraItem->fragmentStart, &buffer, end) &&
          TryToAppend (extraItem->fragmentEnd, &buffer, end) &&
          TryToAppend32 (extraItem->packetUid, &buffer, end))
        {
          uintptr_t written = buffer - start;
          NS_ASSERT (written <= 0xffff);
          return written;
        }
    }

  uint32_t n = GetUleb128Size (typeUid);
  n += GetUleb128Size (item->size);
  n += 2;
  n += GetUleb128Size (extraItem->fragmentStart);
  n += GetUleb128Size (extraItem->fragmentEnd);
  n += 4;
  n += 2 + 2;
  ReserveCopy (n);
  goto append;
}

void
PacketMetadata::ReplaceTail (PacketMetadata::SmallItem *item, 
                            PacketMetadata::ExtraItem *extraItem,
                            uint32_t available)
{
  NS_ASSERT (m_data != 0);  
  if (available >= 14 &&
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
          TryToAppend16 (item->chunkUid, &buffer, end) &&
          TryToAppend (extraItem->fragmentStart, &buffer, end) &&
          TryToAppend (extraItem->fragmentEnd, &buffer, end) &&
          TryToAppend32 (extraItem->packetUid, &buffer, end))
        {
          m_used = buffer - &m_data->m_data[0];
          m_data->m_dirtyEnd = m_used;
          return;
        }
    }
  
  // create a copy of the packet.
  PacketMetadata h (m_packetUid, 0);
  uint16_t current = m_head;
  while (current != 0xffff && current != m_tail)
    {
      struct PacketMetadata::SmallItem tmpItem;
      PacketMetadata::ExtraItem tmpExtraItem;
      ReadItems (current, &tmpItem, &tmpExtraItem);
      uint16_t written = h.AddBig (0xffff, h.m_tail, 
                                   &tmpItem, &tmpExtraItem);
      h.UpdateTail (written);
    }
  // append new tail.
  uint16_t written = h.AddBig (0xffff, h.m_tail, item, extraItem);
  h.UpdateTail (written);

  *this = h;
}

uint32_t
PacketMetadata::ReadItems (uint16_t current, 
                          struct PacketMetadata::SmallItem *item,
                          struct PacketMetadata::ExtraItem *extraItem) const
{
  const uint8_t *buffer = &m_data->m_data[current];
  item->next = buffer[0];
  item->next |= (buffer[1]) << 8;
  item->prev = buffer[2];
  item->prev |= (buffer[3]) << 8;
  buffer += 4;
  item->typeUid = ReadUleb128 (&buffer);
  item->size = ReadUleb128 (&buffer);
  item->chunkUid = buffer[0];
  item->chunkUid |= (buffer[1]) << 8;
  buffer += 2;

  bool isExtra = (item->typeUid & 0x1) == 0x1;
  if (isExtra)
    {
      extraItem->fragmentStart = ReadUleb128 (&buffer);
      extraItem->fragmentEnd = ReadUleb128 (&buffer);
      extraItem->packetUid = buffer[0];
      extraItem->packetUid |= buffer[1] << 8;
      extraItem->packetUid |= buffer[2] << 16;
      extraItem->packetUid |= buffer[3] << 24;
      buffer += 4;
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

struct PacketMetadata::Data *
PacketMetadata::Create (uint32_t size)
{
  NS_DEBUG ("create size="<<size<<", max="<<m_maxSize);
  if (size > m_maxSize)
    {
      m_maxSize = size;
    }
  while (!m_freeList.empty ()) 
    {
      struct PacketMetadata::Data *data = m_freeList.back ();
      m_freeList.pop_back ();
      if (data->m_size >= size) 
        {
          NS_DEBUG ("create found size="<<data->m_size);
          data->m_count = 1;
          return data;
        }
      PacketMetadata::Deallocate (data);
      NS_DEBUG ("create dealloc size="<<data->m_size);
    }
  NS_DEBUG ("create alloc size="<<m_maxSize);
  return PacketMetadata::Allocate (m_maxSize);
}

void
PacketMetadata::Recycle (struct PacketMetadata::Data *data)
{
  NS_DEBUG ("recycle size="<<data->m_size<<", list="<<m_freeList.size ());
  NS_ASSERT (data->m_count == 0);
  if (m_freeList.size () > 1000 ||
      data->m_size < m_maxSize) 
    {
      PacketMetadata::Deallocate (data);
    } 
  else 
    {
      m_freeList.push_back (data);
    }
}

struct PacketMetadata::Data *
PacketMetadata::Allocate (uint32_t n)
{
  uint32_t size = sizeof (struct Data);
  if (n <= 10)
    {
      n = 10;
    }
  size += n - 10;
  uint8_t *buf = new uint8_t [size];
  struct PacketMetadata::Data *data = (struct PacketMetadata::Data *)buf;
  data->m_size = n;
  data->m_count = 1;
  data->m_dirtyEnd = 0;
  return data;
}
void 
PacketMetadata::Deallocate (struct PacketMetadata::Data *data)
{
  uint8_t *buf = (uint8_t *)data;
  delete [] buf;
}


PacketMetadata 
PacketMetadata::CreateFragment (uint32_t start, uint32_t end) const
{
  PacketMetadata fragment = *this;
  fragment.RemoveAtStart (start);
  fragment.RemoveAtEnd (end);
  return fragment;
}

void 
PacketMetadata::DoAddHeader (uint32_t uid, uint32_t size)
{
  if (!m_enable)
    {
      return;
    }
  struct PacketMetadata::SmallItem item;
  item.next = m_head;
  item.prev = 0xffff;
  item.typeUid = uid;
  item.size = size;
  item.chunkUid = m_chunkUid;
  m_chunkUid++;
  uint16_t written = AddSmall (&item);
  UpdateHead (written);
}
void 
PacketMetadata::DoRemoveHeader (uint32_t uid, uint32_t size)
{
  if (!m_enable) 
    {
      return;
    }
  struct PacketMetadata::SmallItem item;
  struct PacketMetadata::ExtraItem extraItem;
  ReadItems (m_head, &item, &extraItem);
  if ((item.typeUid & 0xfffffffe) != uid ||
      item.size != size)
    {
      NS_FATAL_ERROR ("Removing unexpected header.");
    }
  else if (item.typeUid != uid &&
           (extraItem.fragmentStart != 0 ||
            extraItem.fragmentEnd != size))
    {
      NS_FATAL_ERROR ("Removing incomplete header.");
    }
  m_head = item.next;
  if (m_head > m_tail)
    {
      m_used = m_head;
    }
}
void 
PacketMetadata::DoAddTrailer (uint32_t uid, uint32_t size)
{
  if (!m_enable)
    {
      return;
    }
  struct PacketMetadata::SmallItem item;
  item.next = 0xffff;
  item.prev = m_tail;
  item.typeUid = uid;
  item.size = size;
  item.chunkUid = m_chunkUid;
  m_chunkUid++;
  uint16_t written = AddSmall (&item);
  UpdateTail (written);
}
void 
PacketMetadata::DoRemoveTrailer (uint32_t uid, uint32_t size)
{
  if (!m_enable) 
    {
      return;
    }
  struct PacketMetadata::SmallItem item;
  struct PacketMetadata::ExtraItem extraItem;
  ReadItems (m_tail, &item, &extraItem);
  if ((item.typeUid & 0xfffffffe) != uid ||
      item.size != size)
    {
      NS_FATAL_ERROR ("Removing unexpected trailer.");
    }
  else if (item.typeUid != uid &&
           (extraItem.fragmentStart != 0 ||
            extraItem.fragmentEnd != size))
    {
      NS_FATAL_ERROR ("Removing incomplete trailer.");
    }
  m_tail = item.prev;
  if (m_tail > m_head)
    {
      m_used = m_tail;
    }
}
void
PacketMetadata::AddAtEnd (PacketMetadata const&o)
{
  if (!m_enable) 
    {
      return;
    }
  if (m_tail == 0xffff)
    {
      *this = o;
      return;
    }
  NS_ASSERT (m_head != 0xffff && m_tail != 0xffff);

  uint16_t lastTail;
  lastTail = m_tail;
  struct PacketMetadata::SmallItem lastItem;
  PacketMetadata::ExtraItem lastExtraItem;
  uint32_t lastTailSize = ReadItems (m_tail, &lastItem, &lastExtraItem);
  if (m_tail + lastTailSize == m_used &&
      m_used == m_data->m_dirtyEnd)
    {
      lastTailSize = m_data->m_size - m_tail;
    }

  uint16_t current = o.m_head;
  while (current != 0xffff)
    {
      struct PacketMetadata::SmallItem item;
      PacketMetadata::ExtraItem extraItem;
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
          uint16_t written = AddBig (0xffff, m_tail, &item, &extraItem);
          UpdateTail (written);
        }
      if (current == o.m_tail)
        {
          break;
        }
      current = item.next;
    }
}
void
PacketMetadata::AddPaddingAtEnd (uint32_t end)
{
  if (!m_enable)
    {
      return;
    }
}
void 
PacketMetadata::RemoveAtStart (uint32_t start)
{
  if (!m_enable) 
    {
      return;
    }
  NS_ASSERT (m_data != 0);
  uint32_t leftToRemove = start;
  uint16_t current = m_head;
  while (current != 0xffff && leftToRemove > 0)
    {
      struct PacketMetadata::SmallItem item;
      PacketMetadata::ExtraItem extraItem;
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
          PacketMetadata fragment (m_packetUid, 0);
          extraItem.fragmentStart += leftToRemove;
          leftToRemove = 0;
          uint16_t written = fragment.AddBig (0xffff, fragment.m_tail,
                                              &item, &extraItem);
          fragment.UpdateTail (written);
          current = item.next;
          while (current != 0xffff)
            {
              ReadItems (current, &item, &extraItem);
              written = fragment.AddBig (0xffff, fragment.m_tail,
                                         &item, &extraItem);
              fragment.UpdateTail (written);
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
PacketMetadata::RemoveAtEnd (uint32_t end)
{
  if (!m_enable) 
    {
      return;
    }
  NS_ASSERT (m_data != 0);

  uint32_t leftToRemove = end;
  uint16_t current = m_tail;
  while (current != 0xffff && leftToRemove > 0)
    {
      struct PacketMetadata::SmallItem item;
      PacketMetadata::ExtraItem extraItem;
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
          PacketMetadata fragment (m_packetUid, 0);
          NS_ASSERT (extraItem.fragmentEnd > leftToRemove);
          extraItem.fragmentEnd -= leftToRemove;
          leftToRemove = 0;
          uint16_t written = fragment.AddBig (fragment.m_head, 0xffff,
                                              &item, &extraItem);
          fragment.UpdateHead (written);
          current = item.prev;
          while (current != 0xffff)
            {
              ReadItems (current, &item, &extraItem);
              written = fragment.AddBig (fragment.m_head, 0xffff,
                                         &item, &extraItem);
              fragment.UpdateHead (written);
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
PacketMetadata::PrintDefault (std::ostream &os, Buffer buffer) const
{
  Print (os, buffer, PacketPrinter::GetDefault ());
}

uint32_t
PacketMetadata::DoPrint (struct PacketMetadata::SmallItem *item, uint32_t current,
                        Buffer data, uint32_t offset, const PacketPrinter &printer,
                        std::ostream &os) const
{
  PacketMetadata::ExtraItem extraItem;
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
PacketMetadata::GetTotalSize (void) const
{
  uint32_t totalSize = 0;
  uint16_t current = m_head;
  uint16_t tail = m_tail;
  while (current != 0xffff)
    {
      struct PacketMetadata::SmallItem item;
      PacketMetadata::ExtraItem extraItem;
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

uint32_t 
PacketMetadata::GetUid (void) const
{
  return m_packetUid;
}

void
PacketMetadata::Print (std::ostream &os, Buffer data, const PacketPrinter &printer) const
{
  if (!m_enable) 
    {
      return;
    }
  NS_ASSERT (m_data != 0);
  NS_ASSERT (GetTotalSize () == data.GetSize ());
  if (printer.m_forward)
    {
      uint32_t tail = m_tail;
      uint32_t head = m_head;
      uint32_t current = head;
      uint32_t offset = 0;
      while (current != 0xffff)
        {
          struct PacketMetadata::SmallItem item;
          uint32_t realSize = DoPrint (&item, current, data, offset, printer, os);
          offset += realSize;
          if (current == tail)
            {
              break;
            }
          if (item.next != 0xffff)
            {
              os << printer.m_separator;
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
          struct PacketMetadata::SmallItem item;
          uint32_t realSize = DoPrint (&item, current, data, offset, printer, os);
          offset -= realSize;
          if (current == tail)
            {
              break;
            }
          if (item.prev != 0xffff)
            {
              os << printer.m_separator;
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



class PacketMetadataTest : public Test {
public:
  PacketMetadataTest ();
  virtual ~PacketMetadataTest ();
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

PacketMetadataTest::PacketMetadataTest ()
  : Test ("PacketMetadata")
{
  m_printer.AddPayloadPrinter (MakeCallback (&PacketMetadataTest::PrintPayload, this));
  m_printer.AddDefaultPrinter (MakeCallback (&PacketMetadataTest::PrintDefault, this));
}

PacketMetadataTest::~PacketMetadataTest ()
{}

template <int N>
void 
PacketMetadataTest::RegisterHeader (void)
{
  static bool registered = false;
  if (!registered)
    {
      m_printer.AddHeaderPrinter (MakeCallback (&PacketMetadataTest::PrintHeader<N>, this),
                                  MakeCallback (&PacketMetadataTest::PrintFragment, this));
      registered = true;
    }
}

template <int N>
void 
PacketMetadataTest::RegisterTrailer (void)
{
  static bool registered = false;
  if (!registered)
    {
      m_printer.AddTrailerPrinter (MakeCallback (&PacketMetadataTest::PrintTrailer<N>, this),
                                   MakeCallback (&PacketMetadataTest::PrintFragment, this));
      registered = true;
    }
}


template <int N>
void 
PacketMetadataTest::PrintHeader (std::ostream &os, uint32_t packetUid, uint32_t size, 
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
PacketMetadataTest::PrintTrailer (std::ostream &os, uint32_t packetUid, uint32_t size, 
                                 const HistoryTrailer<N> *trailer)
{
  if (!trailer->IsOk ())
    {
      m_trailerError = true;
    }
  m_prints.push_back (N);
}
void 
PacketMetadataTest::PrintFragment (std::ostream &os,uint32_t packetUid,
                                  uint32_t size,std::string & name, 
                                  struct PacketPrinter::FragmentInformation info)
{
  m_prints.push_back (info.end - info.start);
}
void 
PacketMetadataTest::PrintDefault (std::ostream& os,uint32_t packetUid,
                     uint32_t size,std::string& name,
                     struct PacketPrinter::FragmentInformation info)
{
  NS_ASSERT (false);
}
void 
PacketMetadataTest::PrintPayload (std::ostream &os,uint32_t packetUid,
                                 uint32_t size,
                                 struct PacketPrinter::FragmentInformation info)
{
  m_prints.push_back (info.end - info.start);
}


void 
PacketMetadataTest::CleanupPrints (void)
{
  m_prints.clear ();
}

bool 
PacketMetadataTest::CheckHistory (Packet p, char *file, int line, uint32_t n, ...)
{
  m_headerError = false;
  m_trailerError = false;
  va_list ap;
  p.Print (std::cerr, m_printer);
  va_start (ap, n);
  if (m_headerError)
    {
      std::cout << "PacketMetadata header error. file=" << file 
                << ", line=" << line << std::endl;
      return false;
    }
  if (m_trailerError)
    {
      std::cout << "PacketMetadata trailer error. file=" << file 
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
  std::cout << "PacketMetadata error. file="<< file 
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
PacketMetadataTest::RunTests (void)
{
  bool ok = true;

  PacketMetadata::Enable ();

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


  p = Packet (2000);
  CHECK_HISTORY (p, 1, 2000);
  


  return ok;
}

static PacketMetadataTest g_packetHistoryTest;

}//namespace ns3
