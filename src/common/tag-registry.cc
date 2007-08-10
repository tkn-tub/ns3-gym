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
#include "tag-registry.h"
#include "ns3/fatal-error.h"

namespace ns3 {

TagRegistry::TagInfoVector *
TagRegistry::GetInfo (void)
{
  static TagRegistry::TagInfoVector vector;
  return &vector;
}

std::string
TagRegistry::GetUidString (uint32_t uid)
{
  TagInfo info = (*GetInfo ())[uid - 1];
  return info.uidString;
}
uint32_t 
TagRegistry::GetUidFromUidString (std::string uidString)
{
  TagInfoVector *vec = GetInfo ();
  uint32_t uid = 1;
  for (TagInfoVector::iterator i = vec->begin (); i != vec->end (); i++)
    {
      if (i->uidString == uidString)
        {
          return uid;
        }
      uid++;
    }
  NS_FATAL_ERROR ("We are trying to deserialize an un-registered type. This can't work.");
  return 0;
}

void 
TagRegistry::Destruct (uint32_t uid, uint8_t *data)
{
  TagInfo info = (*GetInfo ())[uid - 1];
  info.destruct (data);
}
void 
TagRegistry::Print (uint32_t uid, uint8_t *data, std::ostream &os)
{
  TagInfo info = (*GetInfo ())[uid - 1];
  info.print (data, os);
}
uint32_t
TagRegistry::GetSerializedSize (uint32_t uid, uint8_t *data)
{
  TagInfo info = (*GetInfo ())[uid - 1];
  return info.getSerializedSize (data);
}
void 
TagRegistry::Serialize (uint32_t uid, uint8_t *data, Buffer::Iterator start)
{
  TagInfo info = (*GetInfo ())[uid - 1];
  info.serialize (data, start);
}
uint32_t 
TagRegistry::Deserialize (uint32_t uid, uint8_t *data, Buffer::Iterator start)
{
  TagInfo info = (*GetInfo ())[uid - 1];
  return info.deserialize (data, start);
}

} // namespace ns3
