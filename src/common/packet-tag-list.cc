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
#include "packet-tag-list.h"
#include "tag-buffer.h"
#include "tag.h"
#include "ns3/fatal-error.h"
#include "ns3/log.h"
#include <string.h>

NS_LOG_COMPONENT_DEFINE ("PacketTagList");

namespace ns3 {

#ifdef USE_FREE_LIST

struct PacketTagList::TagData *PacketTagList::g_free = 0;
uint32_t PacketTagList::g_nfree = 0;

struct PacketTagList::TagData *
PacketTagList::AllocData (void) const
{
  struct PacketTagList::TagData *retval;
  if (g_free != 0) 
    {
      retval = g_free;
      g_free = g_free->m_next;
      g_nfree--;
    } 
  else 
    {
      retval = new struct PacketTagList::TagData ();
    }
  return retval;
}

void
PacketTagList::FreeData (struct TagData *data) const
{
  if (g_nfree > 1000) 
    {
      delete data;
      return;
    }
  g_nfree++;
  data->next = g_free;
  data->tid = TypeId ();
  g_free = data;
}
#else
struct PacketTagList::TagData *
PacketTagList::AllocData (void) const
{
  struct PacketTagList::TagData *retval;
  retval = new struct PacketTagList::TagData ();
  return retval;
}

void
PacketTagList::FreeData (struct TagData *data) const
{
  delete data;
}
#endif

bool
PacketTagList::Remove (Tag &tag)
{
  TypeId tid = tag.GetInstanceTypeId ();
  bool found = false;
  for (struct TagData *cur = m_next; cur != 0; cur = cur->next) 
    {
      if (cur->tid == tid) 
        {
          found = true;
          tag.Deserialize (TagBuffer (cur->data, cur->data+PACKET_TAG_MAX_SIZE));
        }
    }
  if (!found) 
    {
      return false;
    }
  struct TagData *start = 0;
  struct TagData **prevNext = &start;
  for (struct TagData *cur = m_next; cur != 0; cur = cur->next) 
    {
      if (cur->tid == tid) 
        {
          /**
           * XXX
           * Note: I believe that we could optimize this to
           * avoid copying each TagData located after the target id
           * and just link the already-copied list to the next tag.
           */
          continue;
        }
      struct TagData *copy = AllocData ();
      copy->tid = cur->tid;
      copy->count = 1;
      copy->next = 0;
      memcpy (copy->data, cur->data, PACKET_TAG_MAX_SIZE);
      *prevNext = copy;
      prevNext = &copy->next;
    }
  *prevNext = 0;
  RemoveAll ();
  m_next = start;
  return true;
}

void 
PacketTagList::Add (const Tag &tag) const
{
  // ensure this id was not yet added
  for (struct TagData *cur = m_next; cur != 0; cur = cur->next) 
    {
      NS_ASSERT (cur->tid != tag.GetInstanceTypeId ());
    }
  struct TagData *head = AllocData ();
  head->count = 1;
  head->next = 0;
  head->tid = tag.GetInstanceTypeId ();
  head->next = m_next;
  NS_ASSERT (tag.GetSerializedSize () < PACKET_TAG_MAX_SIZE);
  tag.Serialize (TagBuffer (head->data, head->data+tag.GetSerializedSize ()));

  const_cast<PacketTagList *> (this)->m_next = head;
}

bool
PacketTagList::Peek (Tag &tag) const
{
  TypeId tid = tag.GetInstanceTypeId ();
  for (struct TagData *cur = m_next; cur != 0; cur = cur->next) 
    {
      if (cur->tid == tid) 
        {
          /* found tag */
          tag.Deserialize (TagBuffer (cur->data, cur->data+PACKET_TAG_MAX_SIZE));
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

} // namespace ns3

