#include "tag-list.h"
#include <vector>

#define USE_FREE_LIST 1
#define FREE_LIST_SIZE 1000

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
  struct Item item = Item (TagBuffer (m_current, m_end));
  item.tid.SetUid (item.buf.ReadU32 ());
  item.size = item.buf.ReadU32 ();
  item.start = item.buf.ReadU32 ();
  item.end = item.buf.ReadU32 ();
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
      struct Item item = Item (TagBuffer (m_current, m_end));
      item.tid.SetUid (item.buf.ReadU32 ());
      item.size = item.buf.ReadU32 ();
      item.start = item.buf.ReadU32 ();
      item.end = item.buf.ReadU32 ();
      if (item.start > m_offsetEnd || item.end < m_offsetStart)
	{
	  m_current += 4 + 4 + 4 + 4 + item.size;
	}
      else
	{
	  break;
	}
    }
}
TagList::Iterator::Iterator (uint8_t *start, uint8_t *end, uint32_t offsetStart, uint32_t offsetEnd)
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
{}
TagList::TagList (const TagList &o)
  : m_used (o.m_used),
    m_data (o.m_data)
{
  if (m_data != 0)
    {
      m_data->count++;
    }
}
TagList &
TagList::operator = (const TagList &o)
{
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
  Deallocate (m_data);
  m_data = 0;
  m_used = 0;
}

TagBuffer
TagList::Add (TypeId tid, uint32_t bufferSize, uint32_t start, uint32_t end)
{
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
  TagList::Iterator i = o.Begin (0, 0xffffffff);
  while (i.HasNext ())
    {
      TagList::Iterator::Item item = i.Next ();
      TagBuffer buf = Add (item.tid, item.size, item.start, item.end);
      buf.CopyFrom (item.buf);
    }
}

void 
TagList::Remove (const Iterator &i)
{
  if (m_data == 0)
    {
      return;
    } 
  // XXX
}

void 
TagList::RemoveAll (void)
{
  Deallocate (m_data);
  m_data = 0;
  m_used = 0;
}

TagList::Iterator 
TagList::Begin (uint32_t offsetStart, uint32_t offsetEnd) const
{
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
TagList::IsDirtyAtEnd (uint32_t appendOffset)
{
  TagList::Iterator i = Begin (0, 0xffffffff);
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
TagList::IsDirtyAtStart (uint32_t prependOffset)
{
  TagList::Iterator i = Begin (0, 0xffffffff);
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
TagList::AddAtEnd (int32_t adjustment, uint32_t appendOffset)
{
  if (adjustment == 0 && !IsDirtyAtEnd (appendOffset))
    {
      return;
    }
  TagList list;
  TagList::Iterator i = Begin (0, 0xffffffff);
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
TagList::AddAtStart (int32_t adjustment, uint32_t prependOffset)
{
  if (adjustment == 0 && !IsDirtyAtStart (prependOffset))
    {
      return;
    }
  TagList list;
  TagList::Iterator i = Begin (0, 0xffffffff);
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
