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
#include "ns3/assert.h"
#include <string>

namespace ns3 {

/**
 * \brief represent a single tag
 * \internal
 *
 * This class is used to give polymorphic access to the methods
 * exported by a tag. It also is used to associate a single
 * reliable uid to each unique type. The tricky part here is
 * that we have to deal correctly with a single type
 * being registered multiple times in AllocateUid so,
 * AllocateUid must first check in the list of existing
 * types if there is already a type registered with the
 * same string returned by T::GetUid.
 *
 * It is important to note that, for example, this code
 * will never be triggered on ELF systems (linux, and
 * a lot of unixes) because the ELF runtime ensures that
 * there exist a single instance of a template instanciation
 * even if multiple instanciations of that template are
 * present in memory at runtime.
 */
class Tag
{
public:
  template <typename T>
  static uint32_t GetUid (void);
  static std::string GetUidString (uint32_t uid);
  static uint32_t GetUidFromUidString (std::string uidString);
  static void Destruct (uint32_t uid, uint8_t data[Tags::SIZE]);
  static void Print (uint32_t uid, uint8_t data[Tags::SIZE], std::ostream &os);
  static uint32_t GetSerializedSize (uint32_t uid, uint8_t data[Tags::SIZE]);
  static void Serialize (uint32_t uid, uint8_t data[Tags::SIZE], Buffer::Iterator start);
  static uint32_t Deserialize (uint32_t uid, uint8_t data[Tags::SIZE], Buffer::Iterator start);
private:
  typedef void (*DestructCb) (uint8_t [Tags::SIZE]);
  typedef void (*PrintCb) (uint8_t [Tags::SIZE], std::ostream &);
  typedef uint32_t (*GetSerializedSizeCb) (uint8_t [Tags::SIZE]);
  typedef void (*SerializeCb) (uint8_t [Tags::SIZE], Buffer::Iterator);
  typedef uint32_t (*DeserializeCb) (uint8_t [Tags::SIZE], Buffer::Iterator);
  struct TagInfo
  {
    std::string uidString;
    DestructCb destruct;
    PrintCb print;
    GetSerializedSizeCb getSerializedSize;
    SerializeCb serialize;
    DeserializeCb deserialize;
  };
  typedef std::vector<struct TagInfo> TagInfoVector;

  template <typename T>
  static void DoDestruct (uint8_t data[Tags::SIZE]);
  template <typename T>
  static void DoPrint (uint8_t data[Tags::SIZE], std::ostream &os);
  template <typename T>
  static uint32_t DoGetSerializedSize (uint8_t data[Tags::SIZE]);
  template <typename T>
  static void DoSerialize (uint8_t data[Tags::SIZE], Buffer::Iterator start);
  template <typename T>
  static uint32_t DoDeserialize (uint8_t data[Tags::SIZE], Buffer::Iterator start);
  template <typename T>
  static uint32_t AllocateUid (void);

  static TagInfoVector *GetInfo (void);
};

template <typename T>
void 
Tag::DoDestruct (uint8_t data[Tags::SIZE])
{
  T *tag = reinterpret_cast<T *> (data);
  tag->~T ();  
}
template <typename T>
void 
Tag::DoPrint (uint8_t data[Tags::SIZE], std::ostream &os)
{
  T *tag = reinterpret_cast<T *> (data);
  tag->Print (os);
}
template <typename T>
uint32_t 
Tag::DoGetSerializedSize (uint8_t data[Tags::SIZE])
{
  T *tag = reinterpret_cast<T *> (data);
  return tag->GetSerializedSize ();
}
template <typename T>
void 
Tag::DoSerialize (uint8_t data[Tags::SIZE], Buffer::Iterator start)
{
  T *tag = reinterpret_cast<T *> (data);
  tag->Serialize (start);
}
template <typename T>
uint32_t 
Tag::DoDeserialize (uint8_t data[Tags::SIZE], Buffer::Iterator start)
{
  T *tag = reinterpret_cast<T *> (data);
  return tag->Deserialize (start);
}

template <typename T>
uint32_t 
Tag::GetUid (void)
{
  static uint32_t uid = AllocateUid<T> ();
  return uid;
}

template <typename T>
uint32_t 
Tag::AllocateUid (void)
{
  TagInfoVector *vec = GetInfo ();
  uint32_t j = 0;
  for (TagInfoVector::iterator i = vec->begin (); i != vec->end (); i++)
    {
      if (i->uidString == T::GetUid ())
        {
          return j;
        }
      j++;
    }
  TagInfo info;
  info.uidString = T::GetUid ();
  info.destruct = &Tag::DoDestruct<T>;
  info.print = &Tag::DoPrint<T>;
  info.getSerializedSize = &Tag::DoGetSerializedSize<T>;
  info.serialize = &Tag::DoSerialize<T>;
  info.deserialize = &Tag::DoDeserialize<T>;
  vec->push_back (info);
  uint32_t uid = vec->size ();
  return uid;
}

template <typename T>
void 
Tags::Add (T const&tag)
{
  NS_ASSERT (sizeof (T) <= Tags::SIZE);
  // ensure this id was not yet added
  for (struct TagData *cur = m_next; cur != 0; cur = cur->m_next) 
    {
      NS_ASSERT (cur->m_id != Tag::GetUid<T> ());
    }
  struct TagData *newStart = AllocData ();
  newStart->m_count = 1;
  newStart->m_next = 0;
  newStart->m_id = Tag::GetUid<T> ();
  void *buf = &newStart->m_data;
  new (buf) T (tag);
  newStart->m_next = m_next;
  m_next = newStart;
}

template <typename T>
bool
Tags::Remove (T &tag)
{
  NS_ASSERT (sizeof (T) <= Tags::SIZE);
  return Remove (Tag::GetUid<T> ());
}

template <typename T>
bool
Tags::Peek (T &tag) const
{
  NS_ASSERT (sizeof (T) <= Tags::SIZE);
  for (struct TagData *cur = m_next; cur != 0; cur = cur->m_next) 
    {
      if (cur->m_id == Tag::GetUid<T> ()) 
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
          Tag::Destruct (prev->m_id, prev->m_data);
          FreeData (prev);
        }
      prev = cur;
    }
  if (prev != 0) 
    {
      Tag::Destruct (prev->m_id, prev->m_data);
      FreeData (prev);
    }
  m_next = 0;
}


}; // namespace ns3

#endif /* TAGS_H */
