#include "mtag-list.h"

namespace ns3 {

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
  : m_buffer (0),
    m_size (0)
{}
TagList::TagList (const TagList &o)
  : m_size (o.m_size)
{
  m_buffer = new uint8_t [o.m_size] ();
  memcpy (m_buffer, o.m_buffer, o.m_size);
}
TagList &
TagList::operator = (const TagList &o)
{
  delete [] m_buffer;
  m_buffer = new uint8_t [o.m_size] ();
  memcpy (m_buffer, o.m_buffer, m_size);
  m_size = o.m_size;
  return *this;
}
TagList::~TagList ()
{
  delete [] m_buffer;
  m_buffer = 0;
  m_size = 0;
}

TagBuffer
TagList::Add (TypeId tid, uint32_t bufferSize, uint32_t start, uint32_t end)
{
  uint32_t newSize = m_size + bufferSize + 4 + 4 + 4 + 4;
  uint8_t *newBuffer = new uint8_t [newSize] ();
  memcpy (newBuffer, m_buffer, m_size);
  TagBuffer tag = TagBuffer (newBuffer + m_size, newBuffer + newSize);
  tag.WriteU32 (tid.GetUid ());
  tag.WriteU32 (bufferSize);
  tag.WriteU32 (start);
  tag.WriteU32 (end);
  delete [] m_buffer;
  m_buffer = newBuffer;
  m_size = newSize;
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
  // XXX: more complex to implement.
}

void 
TagList::RemoveAll (void)
{
  delete [] m_buffer;
  m_buffer = 0;
  m_size = 0;  
}

TagList::Iterator 
TagList::Begin (uint32_t offsetStart, uint32_t offsetEnd) const
{
  return Iterator (m_buffer, m_buffer + m_size, offsetStart, offsetEnd);
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

} // namespace ns3
