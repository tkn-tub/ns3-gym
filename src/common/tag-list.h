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
#ifndef TAG_LIST_H
#define TAG_LIST_H

#include <stdint.h>
#include "ns3/type-id.h"
#include "tag-buffer.h"

namespace ns3 {

struct TagListData;

class TagList
{
public:

  class Iterator
  {
  public:
    struct Item 
    {
      TypeId tid;
      uint32_t size;
      uint32_t start;
      uint32_t end;
      TagBuffer buf;
    private:
      friend class TagList;
      Item (TagBuffer buf);
    };
    bool HasNext (void) const;
    struct TagList::Iterator::Item Next (void);
    uint32_t GetOffsetStart (void) const;
  private:
    friend class TagList;
    Iterator (uint8_t *start, uint8_t *end, uint32_t offsetStart, uint32_t offsetEnd);
    void PrepareForNext (void);
    uint8_t *m_current;
    uint8_t *m_end;
    uint32_t m_offsetStart;
    uint32_t m_offsetEnd;
  };

  TagList ();
  TagList (const TagList &o);
  TagList &operator = (const TagList &o);
  ~TagList ();

  TagBuffer Add (TypeId tid, uint32_t bufferSize, uint32_t start, uint32_t end);

  void Add (const TagList &o);

  void Remove (const Iterator &i);
  void RemoveAll (void);

  TagList::Iterator Begin (uint32_t offsetStart, uint32_t offsetEnd) const;

  void AddAtEnd (int32_t adjustment, uint32_t appendOffset);
  void AddAtStart (int32_t adjustment, uint32_t prependOffset);

private:
  bool IsDirtyAtEnd (uint32_t appendOffset);
  bool IsDirtyAtStart (uint32_t prependOffset);

  struct TagListData *Allocate (uint32_t size);
  void Deallocate (struct TagListData *data);

  uint16_t m_used;
  struct TagListData *m_data;
};

} // namespace ns3

#endif /* TAG_LIST_H */
