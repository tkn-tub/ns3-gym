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
#ifndef TAG_REGISTRY_H
#define TAG_REGISTRY_H

#include <string>
#include <stdint.h>
#include "buffer.h"

namespace ns3 {

/**
 * \brief a registry of all existing tag types.
 * \internal
 *
 * This class is used to give polymorphic access to the methods
 * exported by a tag. It also is used to associate a single
 * reliable uid to each unique type. 
 */
class TagRegistry
{
public:
  template <typename T>
  static uint32_t Register (std::string uidString);
  static std::string GetUidString (uint32_t uid);
  static uint32_t GetUidFromUidString (std::string uidString);
  static void Destruct (uint32_t uid, uint8_t *data);
  static void Print (uint32_t uid, uint8_t *data, std::ostream &os);
  static uint32_t GetSerializedSize (uint32_t uid, uint8_t *data);
  static void Serialize (uint32_t uid, uint8_t *data, Buffer::Iterator start);
  static uint32_t Deserialize (uint32_t uid, uint8_t *data, Buffer::Iterator start);
private:
  typedef void (*DestructCb) (uint8_t *);
  typedef void (*PrintCb) (uint8_t *, std::ostream &);
  typedef uint32_t (*GetSerializedSizeCb) (uint8_t *);
  typedef void (*SerializeCb) (uint8_t *, Buffer::Iterator);
  typedef uint32_t (*DeserializeCb) (uint8_t *, Buffer::Iterator);
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
  static void DoDestruct (uint8_t *data);
  template <typename T>
  static void DoPrint (uint8_t *data, std::ostream &os);
  template <typename T>
  static uint32_t DoGetSerializedSize (uint8_t *data);
  template <typename T>
  static void DoSerialize (uint8_t *data, Buffer::Iterator start);
  template <typename T>
  static uint32_t DoDeserialize (uint8_t *data, Buffer::Iterator start);

  static TagInfoVector *GetInfo (void);
};

} // namespace ns3

namespace ns3 {

template <typename T>
void 
TagRegistry::DoDestruct (uint8_t *data)
{
  T *tag = reinterpret_cast<T *> (data);
  tag->~T ();  
}
template <typename T>
void 
TagRegistry::DoPrint (uint8_t *data, std::ostream &os)
{
  T *tag = reinterpret_cast<T *> (data);
  tag->Print (os);
}
template <typename T>
uint32_t 
TagRegistry::DoGetSerializedSize (uint8_t *data)
{
  T *tag = reinterpret_cast<T *> (data);
  return tag->GetSerializedSize ();
}
template <typename T>
void 
TagRegistry::DoSerialize (uint8_t *data, Buffer::Iterator start)
{
  T *tag = reinterpret_cast<T *> (data);
  tag->Serialize (start);
}
template <typename T>
uint32_t 
TagRegistry::DoDeserialize (uint8_t *data, Buffer::Iterator start)
{
  T *tag = reinterpret_cast<T *> (data);
  return tag->Deserialize (start);
}

template <typename T>
uint32_t 
TagRegistry::Register (std::string uidString)
{
  TagInfoVector *vec = GetInfo ();
  uint32_t j = 0;
  for (TagInfoVector::iterator i = vec->begin (); i != vec->end (); i++)
    {
      if (i->uidString == uidString)
        {
          return j;
        }
      j++;
    }
  TagInfo info;
  info.uidString = uidString;
  info.destruct = &TagRegistry::DoDestruct<T>;
  info.print = &TagRegistry::DoPrint<T>;
  info.getSerializedSize = &TagRegistry::DoGetSerializedSize<T>;
  info.serialize = &TagRegistry::DoSerialize<T>;
  info.deserialize = &TagRegistry::DoDeserialize<T>;
  vec->push_back (info);
  uint32_t uid = vec->size ();
  return uid;
}

} // namespace ns3

#endif /* TAG_REGISTRY_H */
