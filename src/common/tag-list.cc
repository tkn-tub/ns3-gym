/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
#include "tag-list.h"
#include "ns3/log.h"
#include <vector>

NS_LOG_COMPONENT_DEFINE ("TagList");

#define USE_FREE_LIST 1
#define FREE_LIST_SIZE 1000
#define OFFSET_MAX (2147483647)

namespace ns3 {

struct TagListData {
  uint32_t size;
  uint32_t count;
  uint32_t dirty;
  uint8_t data[4];
};

#ifdef USE_FREE_LIST
static class TagListDataFreeList : public std::vector<struct TagListData *>
{
public:
  ~TagListDataFreeList ();
} g_freeList;
static uint32_t g_maxSize = 0;

TagListDataFreeList::~TagListDataFreeList ()
{
  for (TagListDataFreeList::iterator i = begin ();
       i != end (); i++)
    {
      uint8_t *buffer = (uint8_t *)(*i);
      delete [] buffer;
    }
}
#endif /* USE_FREE_LIST */

TagList::Iterator::Item::Item (TagBuffer buf_)
    : buf (buf_)
{}

bool 
TagList::Iterator::HasNext (void) const
{
  return m_current < m_end;
}
struct TagList::Iterator::Item 
TagList::Iterator::Next (void)
{
  NS_ASSERT (HasNext ());
  struct Item item = Item (TagBuffer (m_current+16, m_end));
  item.tid.SetUid (m_nextTid);
  item.size = m_nextSize;
  item.start = m_nextStart;
  item.end = m_nextEnd;
  item.start = std::max (item.start, m_offsetStart);
  item.end = std::min (item.end, m_offsetEnd);
  m_current += 4 + 4 + 4 + 4 + item.size;
  item.buf.TrimAtEnd (m_end - m_current);
  PrepareForNext ();
  return item;
}
void
TagList::Iterator::PrepareForNext (void)
{
  while (m_current < m_end)
    {
      TagBuffer buf = TagBuffer (m_current, m_end);
      m_nextTid = buf.ReadU32 ();
      m_nextSize = buf.ReadU32 ();
      m_nextStart = buf.ReadU32 ();
      m_nextEnd = buf.ReadU32 ();
      if (m_nextStart > m_offsetEnd || m_nextEnd < m_offsetStart)
	{
	  m_current += 4 + 4 + 4 + 4 + m_nextSize;
	}
      else
	{
	  break;
	}
    }
}
TagList::Iterator::Iterator (uint8_t *start, uint8_t *end, int32_t offsetStart, int32_t offsetEnd)
  : m_current (start),
    m_end (end),
    m_offsetStart (offsetStart),
    m_offsetEnd (offsetEnd)
{
  PrepareForNext ();
}

uint32_t 
TagList::Iterator::GetOffsetStart (void) const
{
  return m_offsetStart;
}


TagList::TagList ()
  : m_used (0),
    m_data (0)
{
  NS_LOG_FUNCTION (this);
}
TagList::TagList (const TagList &o)
  : m_used (o.m_used),
    m_data (o.m_data)
{
  NS_LOG_FUNCTION (this << &o);
  if (m_data != 0)
    {
      m_data->count++;
    }
}
TagList &
TagList::operator = (const TagList &o)
{
  NS_LOG_FUNCTION (this << &o);
  if (this == &o)
    {
      return *this;
    }

  Deallocate (m_data);
  m_data = o.m_data;
  m_used = o.m_used;
  if (m_data != 0)
    {
      m_data->count++;
    }
  return *this;
}
TagList::~TagList ()
{
  NS_LOG_FUNCTION (this);
  Deallocate (m_data);
  m_data = 0;
  m_used = 0;
}

TagBuffer
TagList::Add (TypeId tid, uint32_t bufferSize, int32_t start, int32_t end)
{
  NS_LOG_FUNCTION (this << tid << bufferSize << start << end);
  uint32_t spaceNeeded = m_used + bufferSize + 4 + 4 + 4 + 4;
  NS_ASSERT (m_used <= spaceNeeded);
  if (m_data == 0)
    {
      m_data = Allocate (spaceNeeded);
      m_used = 0;
    } 
  else if (m_data->size < spaceNeeded ||
	   (m_data->count != 1 && m_data->dirty != m_used))
    {
      struct TagListData *newData = Allocate (spaceNeeded);
      memcpy (&newData->data, &m_data->data, m_used);
      Deallocate (m_data);
      m_data = newData;
    }
  TagBuffer tag = TagBuffer (&m_data->data[m_used], 
			     &m_data->data[spaceNeeded]);
  tag.WriteU32 (tid.GetUid ());
  tag.WriteU32 (bufferSize);
  tag.WriteU32 (start);
  tag.WriteU32 (end);
  m_used = spaceNeeded;
  m_data->dirty = m_used;
  return tag;
}

void 
TagList::Add (const TagList &o)
{
  NS_LOG_FUNCTION (this << &o);
  TagList::Iterator i = o.BeginAll ();
  while (i.HasNext ())
    {
      TagList::Iterator::Item item = i.Next ();
      TagBuffer buf = Add (item.tid, item.size, item.start, item.end);
      buf.CopyFrom (item.buf);
    }
}

void 
TagList::RemoveAll (void)
{
  NS_LOG_FUNCTION (this);
  Deallocate (m_data);
  m_data = 0;
  m_used = 0;
}

TagList::Iterator 
TagList::BeginAll (void) const
{
  NS_LOG_FUNCTION (this);
  // I am not totally sure but I might need to use 
  // INT32_MIN instead of zero below.
  return Begin (0, OFFSET_MAX);
}

TagList::Iterator 
TagList::Begin (int32_t offsetStart, int32_t offsetEnd) const
{
  NS_LOG_FUNCTION (this << offsetStart << offsetEnd);
  if (m_data == 0)
    {
      return Iterator (0, 0, offsetStart, offsetEnd);
    }
  else
    {
      return Iterator (m_data->data, &m_data->data[m_used], offsetStart, offsetEnd);
    }
}

bool 
TagList::IsDirtyAtEnd (int32_t appendOffset)
{
  NS_LOG_FUNCTION (this << appendOffset);
  TagList::Iterator i = BeginAll ();
  while (i.HasNext ())
    {
      TagList::Iterator::Item item = i.Next ();
      if (item.end > appendOffset)
	{
	  return true;
	}
    }
  return false;
}

bool 
TagList::IsDirtyAtStart (int32_t prependOffset)
{
  NS_LOG_FUNCTION (this << prependOffset);
  TagList::Iterator i = BeginAll ();
  while (i.HasNext ())
    {
      TagList::Iterator::Item item = i.Next ();
      if (item.start < prependOffset)
	{
	  return true;
	}
    }
  return false;
}

void 
TagList::AddAtEnd (int32_t adjustment, int32_t appendOffset)
{
  NS_LOG_FUNCTION (this << adjustment << appendOffset);
  if (adjustment == 0 && !IsDirtyAtEnd (appendOffset))
    {
      return;
    }
  TagList list;
  TagList::Iterator i = BeginAll ();
  while (i.HasNext ())
    {
      TagList::Iterator::Item item = i.Next ();
      item.start += adjustment;
      item.end += adjustment;

      if (item.start > appendOffset)
	{
	  continue;
	}
      else if (item.start < appendOffset && item.end > appendOffset)
	{
	  item.end = appendOffset;
	}
      else
	{
	  // nothing to do.
	}
      TagBuffer buf = list.Add (item.tid, item.size, item.start, item.end);
      buf.CopyFrom (item.buf);
    }
  *this = list;  
}

void 
TagList::AddAtStart (int32_t adjustment, int32_t prependOffset)
{
  NS_LOG_FUNCTION (this << adjustment << prependOffset);
  if (adjustment == 0 && !IsDirtyAtStart (prependOffset))
    {
      return;
    }
  TagList list;
  TagList::Iterator i = BeginAll ();
  while (i.HasNext ())
    {
      TagList::Iterator::Item item = i.Next ();
      item.start += adjustment;
      item.end += adjustment;

      if (item.end < prependOffset)
	{
	  continue;
	}
      else if (item.end > prependOffset && item.start < prependOffset)
	{
	  item.start = prependOffset;
	}
      else
	{
	  // nothing to do.
	}
      TagBuffer buf = list.Add (item.tid, item.size, item.start, item.end);
      buf.CopyFrom (item.buf);
    }
  *this = list;
}

#ifdef USE_FREE_LIST

struct TagListData *
TagList::Allocate (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  while (!g_freeList.empty ())
    {
      struct TagListData *data = g_freeList.back ();
      g_freeList.pop_back ();
      NS_ASSERT (data != 0);
      if (data->size >= size)
	{
	  data->count = 1;
	  data->dirty = 0;
	  return data;
	}
      uint8_t *buffer = (uint8_t *)data;
      delete [] buffer;
    }
  uint8_t *buffer = new uint8_t [std::max (size, g_maxSize) + sizeof (struct TagListData) - 4];
  struct TagListData *data = (struct TagListData *)buffer;
  data->count = 1;
  data->size = size;
  data->dirty = 0;
  return data;
}

void 
TagList::Deallocate (struct TagListData *data)
{
  NS_LOG_FUNCTION (this << data);
  if (data == 0)
    {
      return;
    }
  g_maxSize = std::max (g_maxSize, data->size);
  data->count--;
  if (data->count == 0)
    {
      if (g_freeList.size () > FREE_LIST_SIZE ||
	  data->size < g_maxSize)
	{
	  uint8_t *buffer = (uint8_t *)data;
	  delete [] buffer;
	}
      else
	{
	  g_freeList.push_back (data);
	}
    }
}

#else /* USE_FREE_LIST */

struct TagListData *
TagList::Allocate (uint32_t size)
{
  NS_LOG_FUNCTION (this << size);
  uint8_t *buffer = new uint8_t [size + sizeof (struct TagListData) - 4];
  struct TagListData *data = (struct TagListData *)buffer;
  data->count = 1;
  data->size = size;
  data->dirty = 0;
  return data;
}

void 
TagList::Deallocate (struct TagListData *data)
{
  NS_LOG_FUNCTION (this << data);
  if (data == 0)
    {
      return;
    }
  data->count--;
  if (data->count == 0)
    {
      uint8_t *buffer = (uint8_t *)data;
      delete [] buffer;
    }
}

#endif /* USE_FREE_LIST */


} // namespace ns3
