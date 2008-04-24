#include "mtag-list.h"

namespace ns3 {

MtagList::Iterator::Item::Item (MtagBuffer buf_)
    : buf (buf_)
{}

bool 
MtagList::Iterator::HasNext (void) const
{
  return m_current < m_end;
}
struct MtagList::Iterator::Item 
MtagList::Iterator::Next (void)
{
  NS_ASSERT (HasNext ());
  struct Item item = Item (MtagBuffer (m_current, m_end));
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
MtagList::Iterator::PrepareForNext (void)
{
  while (m_current < m_end)
    {
      struct Item item = Item (MtagBuffer (m_current, m_end));
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
MtagList::Iterator::Iterator (uint8_t *start, uint8_t *end, uint32_t offsetStart, uint32_t offsetEnd)
  : m_current (start),
    m_end (end),
    m_offsetStart (offsetStart),
    m_offsetEnd (offsetEnd)
{
  PrepareForNext ();
}

uint32_t 
MtagList::Iterator::GetOffsetStart (void) const
{
  return m_offsetStart;
}


MtagList::MtagList ()
  : m_buffer (0),
    m_size (0)
{}
MtagList::MtagList (const MtagList &o)
  : m_size (o.m_size)
{
  m_buffer = new uint8_t [o.m_size] ();
  memcpy (m_buffer, o.m_buffer, o.m_size);
}
MtagList &
MtagList::operator = (const MtagList &o)
{
  delete [] m_buffer;
  m_buffer = new uint8_t [o.m_size] ();
  memcpy (m_buffer, o.m_buffer, m_size);
  m_size = o.m_size;
  return *this;
}
MtagList::~MtagList ()
{
  delete [] m_buffer;
  m_buffer = 0;
  m_size = 0;
}

MtagBuffer
MtagList::Add (TypeId tid, uint32_t bufferSize, uint32_t start, uint32_t end)
{
  uint32_t newSize = m_size + bufferSize + 4 + 4 + 4 + 4;
  uint8_t *newBuffer = new uint8_t [newSize] ();
  memcpy (newBuffer, m_buffer, m_size);
  MtagBuffer tag = MtagBuffer (newBuffer + m_size, newBuffer + newSize);
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
MtagList::Add (const MtagList &o)
{
  MtagList::Iterator i = o.Begin (0, 0xffffffff);
  while (i.HasNext ())
    {
      MtagList::Iterator::Item item = i.Next ();
      MtagBuffer buf = Add (item.tid, item.size, item.start, item.end);
      buf.CopyFrom (item.buf);
    }
}

void 
MtagList::Remove (const Iterator &i)
{
  // XXX: more complex to implement.
}

void 
MtagList::RemoveAll (void)
{
  delete [] m_buffer;
  m_buffer = 0;
  m_size = 0;  
}

MtagList::Iterator 
MtagList::Begin (uint32_t offsetStart, uint32_t offsetEnd) const
{
  return Iterator (m_buffer, m_buffer + m_size, offsetStart, offsetEnd);
}

bool 
MtagList::IsDirtyAtEnd (uint32_t appendOffset)
{
  MtagList::Iterator i = Begin (0, 0xffffffff);
  while (i.HasNext ())
    {
      MtagList::Iterator::Item item = i.Next ();
      if (item.end > appendOffset)
	{
	  return true;
	}
    }
  return false;
}

bool 
MtagList::IsDirtyAtStart (uint32_t prependOffset)
{
  MtagList::Iterator i = Begin (0, 0xffffffff);
  while (i.HasNext ())
    {
      MtagList::Iterator::Item item = i.Next ();
      if (item.start < prependOffset)
	{
	  return true;
	}
    }
  return false;
}

void 
MtagList::AddAtEnd (int32_t adjustment, uint32_t appendOffset)
{
  if (adjustment == 0 && !IsDirtyAtEnd (appendOffset))
    {
      return;
    }
  MtagList list;
  MtagList::Iterator i = Begin (0, 0xffffffff);
  while (i.HasNext ())
    {
      MtagList::Iterator::Item item = i.Next ();
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
      MtagBuffer buf = list.Add (item.tid, item.size, item.start, item.end);
      buf.CopyFrom (item.buf);
    }
  *this = list;  
}

void 
MtagList::AddAtStart (int32_t adjustment, uint32_t prependOffset)
{
  if (adjustment == 0 && !IsDirtyAtStart (prependOffset))
    {
      return;
    }
  MtagList list;
  MtagList::Iterator i = Begin (0, 0xffffffff);
  while (i.HasNext ())
    {
      MtagList::Iterator::Item item = i.Next ();
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
      MtagBuffer buf = list.Add (item.tid, item.size, item.start, item.end);
      buf.CopyFrom (item.buf);
    }
  *this = list;    
}

} // namespace ns3
