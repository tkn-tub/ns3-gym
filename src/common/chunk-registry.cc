/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "chunk-registry.h"

namespace ns3 {

ChunkRegistry::InfoVector *
ChunkRegistry::GetInfoVector (void)
{
  static InfoVector vec;
  return &vec;
}

std::string 
ChunkRegistry::GetUidStringFromUid (uint32_t uid)
{
  InfoVector *vec = GetInfoVector ();
  NS_ASSERT (uid >= 1 && uid <= vec->size ());
  Info info = (*vec)[uid - 1];
  return info.uidString;
}
uint32_t 
ChunkRegistry::GetUidFromUidString (std::string uidString)
{
  uint32_t uid = 1;
  InfoVector *vec = GetInfoVector ();
  for (InfoVector::iterator i = vec->begin (); i != vec->end (); i++)
    {
      if (i->uidString == uidString)
	{
	  return uid;
	}
      uid++;
    }
  NS_FATAL_ERROR ("Trying to access a non-registered Header or Trailer: \"" << uidString << "\". "<<
		  "You could try calling NS_HEADER_ENSURE_REGISTER somewhere.");
  return 0;
}

uint8_t *
ChunkRegistry::GetStaticInstance (uint32_t uid)
{
  InfoVector *vec = GetInfoVector ();
  NS_ASSERT (uid >= 1 && uid <= vec->size ());
  Info info = (*vec)[uid - 1];
  return info.getStaticInstance ();
}
bool 
ChunkRegistry::IsHeader (uint32_t uid)
{
  InfoVector *vec = GetInfoVector ();
  NS_ASSERT (uid >= 1 && uid <= vec->size ());
  Info info = (*vec)[uid - 1];
  return info.isHeader;
}
bool 
ChunkRegistry::IsTrailer (uint32_t uid)
{
  return !IsHeader (uid);
}
uint32_t 
ChunkRegistry::Deserialize (uint32_t uid, uint8_t *instance, Buffer::Iterator i)
{
  InfoVector *vec = GetInfoVector ();
  NS_ASSERT (uid >= 1 && uid <= vec->size ());
  Info info = (*vec)[uid - 1];
  return info.deserialize (instance, i);
}
void 
ChunkRegistry::Print (uint32_t uid, uint8_t *instance, std::ostream &os)
{
  InfoVector *vec = GetInfoVector ();
  NS_ASSERT (uid >= 1 && uid <= vec->size ());
  Info info = (*vec)[uid - 1];
  return info.print (instance, os);
}
std::string
ChunkRegistry::GetName (uint32_t uid, uint8_t *instance)
{ 
  InfoVector *vec = GetInfoVector ();
  NS_ASSERT (uid >= 1 && uid <= vec->size ());
  Info info = (*vec)[uid - 1];
  return info.getName (instance);
}
void 
ChunkRegistry::InvokePrintCallback (uint32_t uid, uint8_t *instance, std::ostream &os,
				    uint32_t packetUid, uint32_t size, 
				    Ptr<CallbackImplBase> callback)
{
  InfoVector *vec = GetInfoVector ();
  NS_ASSERT (uid >= 1 && uid <= vec->size ());
  Info info = (*vec)[uid - 1];
  info.invokePrintCallback (instance, os, packetUid, size, callback);
}


} // namespace ns3
