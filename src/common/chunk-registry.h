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
#ifndef CHUNK_REGISTRY_H
#define CHUNK_REGISTRY_H

#include "buffer.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"
#include <string>

namespace ns3 {

/**
 * \brief this registry keeps track of all different
 * types of headers and trailers and assigns to each of them
 * a unique integer.
 * \internal
 */
class ChunkRegistry
{
public:
  template <typename T>
  static uint32_t GetHeaderUid (void);
  template <typename T>
  static uint32_t GetTrailerUid (void);

  static uint8_t *GetStaticInstance (uint32_t uid);
  static uint32_t Deserialize (uint32_t uid, uint8_t *instance, Buffer::Iterator i);
  static void Print (uint32_t uid, uint8_t *instance, std::ostream &os);
  static std::string GetName (uint32_t uid, uint8_t *instance);
  static bool IsHeader (uint32_t uid);
  static bool IsTrailer (uint32_t uid);
  static void InvokePrintCallback (uint32_t uid, uint8_t *instance, std::ostream &os,
				   uint32_t packetUid, uint32_t size, 
				   Ptr<CallbackImplBase> callback);
private:
  typedef uint8_t *(*GetStaticInstanceCb) (void);
  typedef uint32_t (*DeserializeCb) (uint8_t *, Buffer::Iterator);
  typedef void (*PrintCb) (uint8_t *,std::ostream &);
  typedef std::string (*GetNameCb) (uint8_t *);
  typedef void (*InvokePrintCallbackCb) (uint8_t *instance, std::ostream &os,
					 uint32_t packetUid, uint32_t size, 
					 Ptr<CallbackImplBase> callback);
  struct Info {
    std::string uidString;
    bool isHeader;
    GetStaticInstanceCb getStaticInstance;
    DeserializeCb deserialize;
    PrintCb print;
    GetNameCb getName;
    InvokePrintCallbackCb invokePrintCallback;
  };
  typedef std::vector<struct Info> InfoVector;
  static InfoVector *GetInfoVector (void);
  template <typename T>
  static uint8_t *DoGetStaticInstance (void);
  template <typename T>
  static uint32_t DoDeserialize (uint8_t *instance, Buffer::Iterator i);
  template <typename T>
  static void DoPrint (uint8_t *instance, std::ostream &os);
  template <typename T>
  static std::string DoGetName (uint8_t *instance);
  template <typename T>
  static void DoInvokePrintCallback (uint8_t *instance, std::ostream &os,
				     uint32_t packetUid, uint32_t size, 
				     Ptr<CallbackImplBase> callback);
  template <typename T>
  static uint32_t GetUid (bool isHeader);

};

} // namespace ns3

namespace ns3 {

template <typename T>
uint32_t 
ChunkRegistry::GetHeaderUid (void)
{
  return GetUid<T> (true);
}
template <typename T>
uint32_t 
ChunkRegistry::GetTrailerUid (void)
{
  return GetUid<T> (false);
}

template <typename T>
uint32_t 
ChunkRegistry::GetUid (bool isHeader)
{
  InfoVector *vec = GetInfoVector ();
  uint32_t uid = 1; 
  for (InfoVector::iterator i = vec->begin (); i != vec->end (); i++)
    {
      if (i->uidString == T::GetUid ())
	{
	  return uid;
	}
      uid++;
    }
  Info info;
  info.getStaticInstance = &ChunkRegistry::DoGetStaticInstance<T>;
  info.print = &ChunkRegistry::DoPrint<T>;
  info.getName = &ChunkRegistry::DoGetName<T>;
  info.deserialize = &ChunkRegistry::DoDeserialize<T>;
  info.invokePrintCallback = &ChunkRegistry::DoInvokePrintCallback<T>;
  info.uidString = T::GetUid ();
  info.isHeader = isHeader;
  vec->push_back (info);
  return vec->size ();
}

template <typename T>
uint8_t *
ChunkRegistry::DoGetStaticInstance ()
{
  static T instance;
  return reinterpret_cast<uint8_t *> (&instance);
}
template <typename T>
uint32_t 
ChunkRegistry::DoDeserialize (uint8_t *instance, Buffer::Iterator i)
{
  T *obj = reinterpret_cast<T *> (instance);
  return obj->Deserialize (i);
}
template <typename T>
void 
ChunkRegistry::DoPrint (uint8_t *instance, std::ostream &os)
{
  T *obj = reinterpret_cast<T *> (instance);
  obj->Print (os);
}
template <typename T>
std::string
ChunkRegistry::DoGetName (uint8_t *instance)
{
  T *obj = reinterpret_cast<T *> (instance);
  return obj->GetName ();
}
template <typename T>
void 
ChunkRegistry::DoInvokePrintCallback (uint8_t *instance, std::ostream &os,
				      uint32_t packetUid, uint32_t size, 
				      Ptr<CallbackImplBase> callback)
{
  T *obj = reinterpret_cast<T *> (instance);
  Callback<void,std::ostream&,uint32_t,uint32_t,const T*> cb;
  cb.Assign (callback);
  cb (os, packetUid, size, obj);
}



} // namespace ns3

#endif /* CHUNK_REGISTRY_H */
