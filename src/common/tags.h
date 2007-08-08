/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#ifndef TAGS_H
#define TAGS_H

#include <stdint.h>
#include <ostream>
#include <vector>
#include "buffer.h"

namespace ns3 {

template <typename T>
class TagPrettyPrinter;

/**
 * \ingroup constants
 * \brief Tag maximum size
 * The maximum size (in bytes) of a Tag is stored
 * in this constant.
 */
#define TAGS_MAX_SIZE 16

class Tags {
public:
  inline Tags ();
  inline Tags (Tags const &o);
  inline Tags &operator = (Tags const &o);
  inline ~Tags ();

  template <typename T>
  void Add (T const&tag);

  template <typename T>
  bool Remove (T &tag);

  template <typename T>
  bool Peek (T &tag) const;

  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator i, uint32_t size) const;
  uint32_t Deserialize (Buffer::Iterator i);

  inline void RemoveAll (void);

  enum {
      SIZE = TAGS_MAX_SIZE
  };
private:
  struct TagData {
      uint8_t m_data[Tags::SIZE];
      struct TagData *m_next;
      uint32_t m_id;
      uint32_t m_count;
  };

  bool Remove (uint32_t id);
  struct Tags::TagData *AllocData (void);
  void FreeData (struct TagData *data);

  static struct Tags::TagData *gFree;
  static uint32_t gN_free;

  struct TagData *m_next;
};

} // namespace ns3



/**************************************************************
   An implementation of the templates defined above
 *************************************************************/
#include "tag-registry.h"
#include "tag.h"
#include "ns3/assert.h"
#include <string>

namespace ns3 {

template <typename T>
void 
Tags::Add (T const&tag)
{
  const Tag *parent;
  // if the following assignment fails, it is because the
  // input to this function is not a subclass of the Tag class.
  parent = &tag;

  NS_ASSERT (sizeof (T) <= Tags::SIZE);
  // ensure this id was not yet added
  for (struct TagData *cur = m_next; cur != 0; cur = cur->m_next) 
    {
      NS_ASSERT (cur->m_id != T::GetUid ());
    }
  struct TagData *newStart = AllocData ();
  newStart->m_count = 1;
  newStart->m_next = 0;
  newStart->m_id = T::GetUid ();
  void *buf = &newStart->m_data;
  new (buf) T (tag);
  newStart->m_next = m_next;
  m_next = newStart;
}

template <typename T>
bool
Tags::Remove (T &tag)
{
  Tag *parent;
  // if the following assignment fails, it is because the
  // input to this function is not a subclass of the Tag class.
  parent = &tag;
  NS_ASSERT (sizeof (T) <= Tags::SIZE);
  return Remove (T::GetUid ());
}

template <typename T>
bool
Tags::Peek (T &tag) const
{
  Tag *parent;
  // if the following assignment fails, it is because the
  // input to this function is not a subclass of the Tag class.
  parent = &tag;
  NS_ASSERT (sizeof (T) <= Tags::SIZE);
  for (struct TagData *cur = m_next; cur != 0; cur = cur->m_next) 
    {
      if (cur->m_id == T::GetUid ()) 
        {
          /* found tag */
          T *data = reinterpret_cast<T *> (&cur->m_data);
          tag = T (*data);
          return true;
        }
    }
  /* no tag found */
  return false;
}

Tags::Tags ()
  : m_next ()
{}

Tags::Tags (Tags const &o)
  : m_next (o.m_next)
{
  if (m_next != 0) 
    {
      m_next->m_count++;
    }
}

Tags &
Tags::operator = (Tags const &o)
{
  // self assignment
  if (m_next == o.m_next) 
    {
      return *this;
    }
  RemoveAll ();
  m_next = o.m_next;
  if (m_next != 0) 
    {
      m_next->m_count++;
    }
  return *this;
}

Tags::~Tags ()
{
  RemoveAll ();
}

void
Tags::RemoveAll (void)
{
  struct TagData *prev = 0;
  for (struct TagData *cur = m_next; cur != 0; cur = cur->m_next) 
    {
      cur->m_count--;
      if (cur->m_count > 0) 
        {
          break;
        }
      if (prev != 0) 
        {
          TagRegistry::Destruct (prev->m_id, prev->m_data);
          FreeData (prev);
        }
      prev = cur;
    }
  if (prev != 0) 
    {
      TagRegistry::Destruct (prev->m_id, prev->m_data);
      FreeData (prev);
    }
  m_next = 0;
}


}; // namespace ns3

#endif /* TAGS_H */
