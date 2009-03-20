/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#include "ns3/type-id.h"

namespace ns3 {

class Tag;

/**
 * \ingroup constants
 * \brief Tag maximum size
 * The maximum size (in bytes) of a Tag is stored
 * in this constant.
 */
#define TAGS_MAX_SIZE 16

class PacketTagList {
public:
  inline PacketTagList ();
  inline PacketTagList (PacketTagList const &o);
  inline PacketTagList &operator = (PacketTagList const &o);
  inline ~PacketTagList ();

  void Add (Tag const&tag) const;
  bool Remove (Tag &tag);
  bool Peek (Tag &tag) const;
  inline void RemoveAll (void);

  void Print (std::ostream &os, std::string separator) const;


  enum {
    SIZE = TAGS_MAX_SIZE
  };
private:
  struct TagData {
    uint8_t data[PacketTagList::SIZE];
    struct TagData *next;
    TypeId tid;
    uint32_t count;
  };

  bool Remove (TypeId tid);
  struct PacketTagList::TagData *AllocData (void) const;
  void FreeData (struct TagData *data) const;

  static struct PacketTagList::TagData *g_free;
  static uint32_t g_nfree;

  struct TagData *m_next;
};

} // namespace ns3



namespace ns3 {

PacketTagList::PacketTagList ()
  : m_next ()
{}

PacketTagList::PacketTagList (PacketTagList const &o)
  : m_next (o.m_next)
{
  if (m_next != 0) 
    {
      m_next->count++;
    }
}

PacketTagList &
PacketTagList::operator = (PacketTagList const &o)
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
      m_next->count++;
    }
  return *this;
}

PacketTagList::~PacketTagList ()
{
  RemoveAll ();
}

void
PacketTagList::RemoveAll (void)
{
  struct TagData *prev = 0;
  for (struct TagData *cur = m_next; cur != 0; cur = cur->next) 
    {
      cur->count--;
      if (cur->count > 0) 
        {
          break;
        }
      if (prev != 0) 
        {
          FreeData (prev);
        }
      prev = cur;
    }
  if (prev != 0) 
    {
      FreeData (prev);
    }
  m_next = 0;
}

} // namespace ns3

#endif /* TAGS_H */
