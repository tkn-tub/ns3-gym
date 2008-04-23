/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
#include "ns3/log.h"
#include "packet-metadata.h"
#include "buffer.h"
#include "header.h"
#include "trailer.h"

NS_LOG_COMPONENT_DEFINE ("PacketMetadata");

namespace ns3 {

bool PacketMetadata::m_enable = false;
bool PacketMetadata::m_metadataSkipped = false;
uint32_t PacketMetadata::m_maxSize = 0;
uint16_t PacketMetadata::m_chunkUid = 0;
PacketMetadata::DataFreeList PacketMetadata::m_freeList;

PacketMetadata::DataFreeList::~DataFreeList ()
{
  for (iterator i = begin (); i != end (); i++)
    {
      PacketMetadata::Deallocate (*i);
    }
  PacketMetadata::m_enable = false;
}

void 
PacketMetadata::Enable (void)
{
  NS_ASSERT_MSG (!m_metadataSkipped,
                 "Error: attempting to enable the packet metadata "
                 "subsystem too late in the simulation, which is not allowed.\n"
                 "A common cause for this problem is to enable ASCII tracing "
                 "after sending any packets.  One way to fix this problem is "
                 "to call ns3::PacketMetadata::Enable () near the beginning of"
                 " the program, before any packets are sent.");
  m_enable = true;
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
  NS_ASSERT (written >= 8);
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
  NS_ASSERT (written >= 8);
  m_used += written;
  m_data->m_dirtyEnd = m_used;
}

uint16_t
PacketMetadata::AddSmall (const struct PacketMetadata::SmallItem *item)
{
  NS_ASSERT (m_data != 0);
  NS_ASSERT (m_used != item->prev && m_used != item->next);
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

uint16_t
PacketMetadata::AddBig (uint32_t next, uint32_t prev, 
                       const PacketMetadata::SmallItem *item, 
                       const PacketMetadata::ExtraItem *extraItem)
{
  NS_ASSERT (m_data != 0);
  uint32_t typeUid = ((item->typeUid & 0x1) == 0x1)?item->typeUid:item->typeUid+1;
  NS_ASSERT (m_used != prev && m_used != next);
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
          NS_ASSERT (written >= 14);
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
  NS_LOG_LOGIC ("create size="<<size<<", max="<<m_maxSize);
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
          NS_LOG_LOGIC ("create found size="<<data->m_size);
          data->m_count = 1;
          return data;
        }
      PacketMetadata::Deallocate (data);
      NS_LOG_LOGIC ("create dealloc size="<<data->m_size);
    }
  NS_LOG_LOGIC ("create alloc size="<<m_maxSize);
  return PacketMetadata::Allocate (m_maxSize);
}

void
PacketMetadata::Recycle (struct PacketMetadata::Data *data)
{
  if (!m_enable)
    {
      PacketMetadata::Deallocate (data);
      return;
    } 
  NS_LOG_LOGIC ("recycle size="<<data->m_size<<", list="<<m_freeList.size ());
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
PacketMetadata::AddHeader (const Header &header, uint32_t size)
{
  uint32_t uid = header.GetInstanceTypeId ().GetUid () << 1;
  DoAddHeader (uid, size);
}
void
PacketMetadata::DoAddHeader (uint32_t uid, uint32_t size)
{
  if (!m_enable)
    {
      m_metadataSkipped = true;
      return;
    }
  NS_LOG_FUNCTION ("uid=" << uid << "size=" << size << "");

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
PacketMetadata::RemoveHeader (const Header &header, uint32_t size)
{
  uint32_t uid = header.GetInstanceTypeId ().GetUid () << 1;
  if (!m_enable) 
    {
      m_metadataSkipped = true;
      return;
    }
  NS_LOG_FUNCTION ("(uid=" << uid << ", size=" << size << ")");
  struct PacketMetadata::SmallItem item;
  struct PacketMetadata::ExtraItem extraItem;
  uint32_t read = ReadItems (m_head, &item, &extraItem);
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
  if (m_head + read == m_used)
    {
      m_used = m_head;
    }
  if (m_head == m_tail)
    {
      m_head = 0xffff;
      m_tail = 0xffff;
    }
  else
    {
      m_head = item.next;
    }
}
void 
PacketMetadata::AddTrailer (const Trailer &trailer, uint32_t size)
{
  uint32_t uid = trailer.GetInstanceTypeId ().GetUid () << 1;
  if (!m_enable)
    {
      m_metadataSkipped = true;
      return;
    }
  NS_LOG_FUNCTION ("(uid=" << uid << ", size=" << size << ")");
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
PacketMetadata::RemoveTrailer (const Trailer &trailer, uint32_t size)
{
  uint32_t uid = trailer.GetInstanceTypeId ().GetUid () << 1;
  if (!m_enable) 
    {
      m_metadataSkipped = true;
      return;
    }
  NS_LOG_FUNCTION ("(uid=" << uid << ", size=" << size << ")");
  struct PacketMetadata::SmallItem item;
  struct PacketMetadata::ExtraItem extraItem;
  uint32_t read = ReadItems (m_tail, &item, &extraItem);
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
  if (m_tail + read == m_used)
    {
      m_used = m_tail;
    }  
  if (m_head == m_tail)
    {
      m_head = 0xffff;
      m_tail = 0xffff;
    }
  else
    {
      m_tail = item.prev;
    }
}
void
PacketMetadata::AddAtEnd (PacketMetadata const&o)
{
  if (!m_enable) 
    {
      m_metadataSkipped = true;
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
          // XXX This call might be wrong. 
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
      m_metadataSkipped = true;
      return;
    }
}
void 
PacketMetadata::RemoveAtStart (uint32_t start)
{
  if (!m_enable) 
    {
      m_metadataSkipped = true;
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
          if (m_head == m_tail)
            {
              m_head = 0xffff;
              m_tail = 0xffff;
            }
          else
            {
              m_head = item.next;
            }
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
      m_metadataSkipped = true;
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
          if (m_head == m_tail)
            {
              m_head = 0xffff;
              m_tail = 0xffff;
            }
          else
            {
              m_tail = item.prev;
            }
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
      NS_ASSERT (current != item.next);
      current = item.next;
    }
  return totalSize;
}

uint32_t 
PacketMetadata::GetUid (void) const
{
  return m_packetUid;
}
PacketMetadata::ItemIterator 
PacketMetadata::BeginItem (Buffer buffer) const
{
  return ItemIterator (this, buffer);
}
PacketMetadata::ItemIterator::ItemIterator (const PacketMetadata *metadata, Buffer buffer)
  : m_metadata (metadata),
    m_buffer (buffer),
    m_current (metadata->m_head),
    m_offset (0),
    m_hasReadTail (false)
{}
bool 
PacketMetadata::ItemIterator::HasNext (void) const
{
  if (m_current == 0xffff)
    {
      return false;
    }
  if (m_hasReadTail)
    {
      return false;
    }
  return true;
}
PacketMetadata::Item
PacketMetadata::ItemIterator::Next (void)
{
  struct PacketMetadata::Item item;
  struct PacketMetadata::SmallItem smallItem;
  struct PacketMetadata::ExtraItem extraItem;
  m_metadata->ReadItems (m_current, &smallItem, &extraItem);
  if (m_current == m_metadata->m_tail)
    {
      m_hasReadTail = true;
    }
  m_current = smallItem.next;
  uint32_t uid = (smallItem.typeUid & 0xfffffffe) >> 1;
  item.tid.SetUid (uid);
  item.currentTrimedFromStart = extraItem.fragmentStart;
  item.currentTrimedFromEnd = extraItem.fragmentEnd - smallItem.size;
  item.currentSize = extraItem.fragmentEnd - extraItem.fragmentStart;
  if (extraItem.fragmentStart != 0 || extraItem.fragmentEnd != smallItem.size)
    {
      item.isFragment = true;
    }
  else
    {
      item.isFragment = false;
    }
  TypeId tid;
  tid.SetUid (uid);
  if (uid == 0)
    {
      item.type = PacketMetadata::Item::PAYLOAD;
    }
  else if (tid.IsChildOf (Header::GetTypeId ()))
    {
      item.type = PacketMetadata::Item::HEADER;
      if (!item.isFragment)
        {
          ns3::Buffer::Iterator j = m_buffer.Begin ();
          j.Next (m_offset);
          item.current = j;
        }
    }
  else if (tid.IsChildOf (Trailer::GetTypeId ()))
    {
      item.type = PacketMetadata::Item::TRAILER;
      if (!item.isFragment)
        {
          ns3::Buffer::Iterator j = m_buffer.End ();
          j.Prev (m_buffer.GetSize () - (m_offset + smallItem.size));
          item.current = j;
        }
    }
  else 
    {
      NS_ASSERT (false);
    }
  m_offset += extraItem.fragmentEnd - extraItem.fragmentStart;
  return item;
}

uint32_t 
PacketMetadata::GetSerializedSize (void) const
{
  uint32_t totalSize = 0;
  totalSize += 4;
  if (!m_enable)
    {
      return totalSize;
    }
  struct PacketMetadata::SmallItem item;
  struct PacketMetadata::ExtraItem extraItem;
  uint32_t current = m_head;
  while (current != 0xffff)
    {
      ReadItems (current, &item, &extraItem);
      uint32_t uid = (item.typeUid & 0xfffffffe) >> 1;
      if (uid == 0)
        {
          totalSize += 4;
        }
      else
        {
          TypeId tid;
          tid.SetUid (uid);
          totalSize += 4 + tid.GetName ().size ();
        }
      totalSize += 1 + 4 + 2 + 4 + 4 + 4;
      if (current == m_tail)
        {
          break;
        }
      NS_ASSERT (current != item.next);
      current = item.next;
    }
  return totalSize;
}
void 
PacketMetadata::Serialize (Buffer::Iterator i, uint32_t size) const
{
  uint32_t bytesWritten = 0;
  i.WriteU32 (size);
  bytesWritten += 4;
  struct PacketMetadata::SmallItem item;
  struct PacketMetadata::ExtraItem extraItem;
  uint32_t current = m_head;
  while (current != 0xffff)
    {
      ReadItems (current, &item, &extraItem);
      NS_LOG_LOGIC ("bytesWritten=" << bytesWritten << ", typeUid="<<
        item.typeUid << ", size="<<item.size<<", chunkUid="<<item.chunkUid<<
        ", fragmentStart="<<extraItem.fragmentStart<<", fragmentEnd="<<
        extraItem.fragmentEnd<< ", packetUid="<<extraItem.packetUid);
      uint32_t uid = (item.typeUid & 0xfffffffe) >> 1;
      if (uid != 0)
        {
          TypeId tid;
          tid.SetUid (uid);
          std::string uidString = tid.GetName ();
          i.WriteU32 (uidString.size ());
          bytesWritten += 4;
          i.Write ((uint8_t *)uidString.c_str (), uidString.size ());
          bytesWritten += uidString.size ();
        }
      else
        {
          i.WriteU32 (0);
          bytesWritten += 4;
        }
      uint8_t isBig = item.typeUid & 0x1;
      i.WriteU8 (isBig);
      bytesWritten += 1;
      i.WriteU32 (item.size);
      bytesWritten += 4;
      i.WriteU16 (item.chunkUid);
      bytesWritten += 2;
      i.WriteU32 (extraItem.fragmentStart);
      bytesWritten += 4;
      i.WriteU32 (extraItem.fragmentEnd);
      bytesWritten += 4;
      i.WriteU32 (extraItem.packetUid);
      bytesWritten += 4;
      if (current == m_tail)
        {
          break;
        }
      
      NS_ASSERT (current != item.next);
      current = item.next;
    }
  NS_ASSERT (bytesWritten == size);
}
uint32_t 
PacketMetadata::Deserialize (Buffer::Iterator i)
{
  struct PacketMetadata::SmallItem item;
  struct PacketMetadata::ExtraItem extraItem;
  uint32_t totalSize = i.ReadU32 ();
  uint32_t size = totalSize;
  size -= 4;
  while (size > 0)
    {
      uint32_t uidStringSize = i.ReadU32 ();
      size -= 4;
      uint32_t uid;
      if (uidStringSize == 0)
        {
          // uid zero for payload.
          uid = 0;
        }
      else
        {
          std::string uidString;
          for (uint32_t j = 0; j < uidStringSize; j++)
            {
              uidString.push_back (i.ReadU8 ());
              size --;
            }
          TypeId tid = TypeId::LookupByName (uidString);
          uid = tid.GetUid ();
        }
      uint8_t isBig = i.ReadU8 ();
      size --;
      item.typeUid = (uid << 1) | isBig;
      item.size = i.ReadU32 ();
      size -= 4;
      item.chunkUid = i.ReadU16 ();
      size -= 2;
      extraItem.fragmentStart = i.ReadU32 ();
      size -= 4;
      extraItem.fragmentEnd = i.ReadU32 ();
      size -= 4;
      extraItem.packetUid = i.ReadU32 ();
      size -= 4;
      NS_LOG_LOGIC ("size=" << size << ", typeUid="<<item.typeUid <<
        ", size="<<item.size<<", chunkUid="<<item.chunkUid<<
        ", fragmentStart="<<extraItem.fragmentStart<<", fragmentEnd="<<
        extraItem.fragmentEnd<< ", packetUid="<<extraItem.packetUid);
      uint32_t tmp = AddBig (0xffff, m_tail, &item, &extraItem);
      UpdateTail (tmp);
    }
  NS_ASSERT (size == 0);
  return totalSize;
}


} // namespace ns3

