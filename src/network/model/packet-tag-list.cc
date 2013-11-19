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

/**
\file   packet-tag-list.cc
\brief  Implements a linked list of Packet tags, including copy-on-write semantics.
*/

#include "packet-tag-list.h"
#include "tag-buffer.h"
#include "tag.h"
#include "ns3/fatal-error.h"
#include "ns3/log.h"
#include <cstring>

NS_LOG_COMPONENT_DEFINE ("PacketTagList")
  ;

namespace ns3 {

bool
PacketTagList::COWTraverse (Tag & tag, PacketTagList::COWWriter Writer)
{
  TypeId tid = tag.GetInstanceTypeId ();
  NS_LOG_FUNCTION (this << tid);
  NS_LOG_INFO     ("looking for " << tid);

  // trivial case when list is empty
  if (m_next == 0)
    {
      return false;
    }

  bool found = false;

  struct TagData ** prevNext = &m_next; // previous node's next pointer
  struct TagData  * cur      =  m_next; // cursor to current node
  struct TagData  * it = 0;             // utility

  // Search from the head of the list until we find tid or a merge
  while (cur != 0)
    {
      if (cur->count > 1)
        {
          // found merge
          NS_LOG_INFO ("found initial merge before tid");
          break;
        }
      else if (cur->tid == tid)
        {
          NS_LOG_INFO ("found tid before initial merge, calling writer");
          found = (this->*Writer)(tag, true, cur, prevNext);
          break;
        }
      else
        {
          // no merge or tid found yet, move on
          prevNext = &cur->next;
          cur      =  cur->next;
        }
    }  // while !found && !cow

  // did we find it or run out of tags?
  if (cur == 0 || found)
    {
      NS_LOG_INFO ("returning after header with found: " << found);
      return found;
    }

  // From here on out, we have to copy the list
  // until we find tid, then link past it

  // Before we do all that work, let's make sure tid really exists
  for (it = cur; it != 0; it = it->next)
    {
      if (it->tid == tid)
        {
          break;
        }
    }
  if (it == 0)
    {
      // got to end of list without finding tid
      NS_LOG_INFO ("tid not found after first merge");
      return found;
    }

  // At this point cur is a merge, but untested for tid
  NS_ASSERT (cur != 0);
  NS_ASSERT (cur->count > 1);

  /*
     Walk the remainder of the list, copying, until we find tid
     As we put a copy of the cur node onto our list,
     we move the merge point down the list.

     Starting position                  End position
       T1 is a merge                     T1.count decremented
                                         T2 is a merge
                                         T1' is a copy of T1

          other                             other
               \                                 \
      Prev  ->  T1  ->  T2  -> ...                T1  ->  T2  -> ...
           /   /                                         /|
      pNext cur                         Prev  ->  T1' --/ |
                                                     /    |
                                                pNext   cur

     When we reach tid, we link past it, decrement count, and we're done.
  */

  // Should normally check for null cur pointer,
  // but since we know tid exists, we'll skip this test
  while ( /* cur && */ cur->tid != tid)
    {
      NS_ASSERT (cur != 0);
      NS_ASSERT (cur->count > 1);
      cur->count--;                       // unmerge cur
      struct TagData * copy = new struct TagData ();
      copy->tid = cur->tid;
      copy->count = 1;
      memcpy (copy->data, cur->data, TagData::MAX_SIZE);
      copy->next = cur->next;             // merge into tail
      copy->next->count++;                // mark new merge
      *prevNext = copy;                   // point prior list at copy
      prevNext = &copy->next;             // advance
      cur      =  copy->next;
    }
  // Sanity check:
  NS_ASSERT (cur != 0);                 // cur should be non-zero
  NS_ASSERT (cur->tid == tid);          // cur->tid should be tid
  NS_ASSERT (cur->count > 1);           // cur should be a merge

  // link around tid, removing it from our list
  found = (this->*Writer)(tag, false, cur, prevNext);
  return found;

}

bool
PacketTagList::Remove (Tag & tag)
{
  return COWTraverse (tag, &PacketTagList::RemoveWriter);
}

// COWWriter implementing Remove
bool
PacketTagList::RemoveWriter (Tag & tag, bool preMerge,
                             struct PacketTagList::TagData  * cur,
                             struct PacketTagList::TagData ** prevNext)
{
  NS_LOG_FUNCTION_NOARGS ();

  // found tid
  bool found = true;
  tag.Deserialize (TagBuffer (cur->data,
                              cur->data + TagData::MAX_SIZE));
  *prevNext = cur->next;            // link around cur

  if (preMerge)
    {
      // found tid before first merge, so delete cur
      delete cur;
    }
  else
    {
      // cur is always a merge at this point
      // unmerge cur, since we linked around it already
      cur->count--;
      if (cur->next != 0)
        {
          // there's a next, so make it a merge
          cur->next->count++;
        }
    }
  return found;
}

bool
PacketTagList::Replace (Tag & tag)
{
  bool found = COWTraverse (tag, &PacketTagList::ReplaceWriter);
  if (!found)
    {
      Add (tag);
    }
  return found;
}

// COWWriter implementing Replace
bool
PacketTagList::ReplaceWriter (Tag & tag, bool preMerge,
                              struct PacketTagList::TagData  * cur,
                              struct PacketTagList::TagData ** prevNext)
{
  NS_LOG_FUNCTION_NOARGS ();

  // found tid
  bool found = true;
  if (preMerge)
    {
      // found tid before first merge, so just rewrite
      tag.Serialize (TagBuffer (cur->data,
                                cur->data + tag.GetSerializedSize ()));
    }
  else
    {
      // cur is always a merge at this point
      // need to copy, replace, and link past cur
      cur->count--;                     // unmerge cur
      struct TagData * copy = new struct TagData ();
      copy->tid = tag.GetInstanceTypeId ();
      copy->count = 1;
      tag.Serialize (TagBuffer (copy->data,
                                copy->data + tag.GetSerializedSize ()));
      copy->next = cur->next;           // merge into tail
      if (copy->next != 0)
        {
          copy->next->count++;          // mark new merge
        }
      *prevNext = copy;                 // point prior list at copy
    }
  return found;
}

void 
PacketTagList::Add (const Tag &tag) const
{
  NS_LOG_FUNCTION (this << tag.GetInstanceTypeId ());
  // ensure this id was not yet added
  for (struct TagData *cur = m_next; cur != 0; cur = cur->next) 
    {
      NS_ASSERT (cur->tid != tag.GetInstanceTypeId ());
    }
  struct TagData * head = new struct TagData ();
  head->count = 1;
  head->next = 0;
  head->tid = tag.GetInstanceTypeId ();
  head->next = m_next;
  NS_ASSERT (tag.GetSerializedSize () <= TagData::MAX_SIZE);
  tag.Serialize (TagBuffer (head->data, head->data + tag.GetSerializedSize ()));

  const_cast<PacketTagList *> (this)->m_next = head;
}

bool
PacketTagList::Peek (Tag &tag) const
{
  NS_LOG_FUNCTION (this << tag.GetInstanceTypeId ());
  TypeId tid = tag.GetInstanceTypeId ();
  for (struct TagData *cur = m_next; cur != 0; cur = cur->next) 
    {
      if (cur->tid == tid) 
        {
          /* found tag */
          tag.Deserialize (TagBuffer (cur->data, cur->data + TagData::MAX_SIZE));
          return true;
        }
    }
  /* no tag found */
  return false;
}

const struct PacketTagList::TagData *
PacketTagList::Head (void) const
{
  return m_next;
}

} /* namespace ns3 */

