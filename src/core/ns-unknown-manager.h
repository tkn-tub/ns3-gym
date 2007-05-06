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
#ifndef NS_UNKNOWN_MANAGER_H
#define NS_UNKNOWN_MANAGER_H

#include <string>
#include <vector>
#include <stdint.h>
#include "callback.h"
#include "ns-unknown.h"

namespace ns3 {

class ClassId
{
public:
  ClassId (std::string name);
private:
  friend bool operator == (const ClassId &a, const ClassId &b);
  uint32_t m_classId;
};

template <typename T>
ClassId
MakeClassId (std::string name);

template <typename T, typename T1>
ClassId
MakeClassId (std::string name);

class NsUnknownManager
{
public:
  static NsUnknown *Create (ClassId classId);

  template <typename T1>
  static NsUnknown *Create (ClassId classId, T1 a1);

  template <typename T1, typename T2>
  static NsUnknown *Create (ClassId classId, T1 a1, T2 a2);

  template <typename T>
  static T *Create (ClassId classId, Iid iid);

  template <typename T, typename T1>
  static T *Create (ClassId classId, Iid iid, T1 a1);

  template <typename T, typename T1, typename T2>
  static T *Create (ClassId classId, Iid iid, T1 a1, T2 a2);

  static void Register (ClassId classId, CallbackBase *callback);
private:
  typedef std::vector<std::pair<ClassId, CallbackBase *> > List;
  static List *GetList (void);
  static CallbackBase *Lookup (ClassId classId);
};

template <typename T>
NsUnknown *
MakeNsUnknownObjectZero (void)
{
  return new T ();
}
template <typename T, typename T1>
NsUnknown *
MakeNsUnknownObjectOne (T1 a1)
{
  return new T (a1);
}


template <typename T>
ClassId
MakeClassId (std::string name)
{
  ClassId classId = ClassId (name);
  static Callback<NsUnknown *> callback = MakeCallback (&MakeNsUnknownObjectZero<T>);
  NsUnknownManager::Register (classId, &callback);
  return classId;
}

template <typename T, typename T1>
ClassId
MakeClassId (std::string name)
{
  ClassId classId = ClassId (name);
  static Callback<NsUnknown *, T1> callback = MakeCallback (&MakeNsUnknownObjectOne<T,T1>);
  NsUnknownManager::Register (classId, &callback);
  return classId;
}


template <typename T1>
NsUnknown *
NsUnknownManager::Create (ClassId classId, T1 a1)
{
  CallbackBase *callback = Lookup (classId);
  if (callback == 0)
    {
      return 0;
    }
  Callback<NsUnknown *, T1> reference;
  if (reference.CheckType (*callback))
    {
      reference = *static_cast<Callback<NsUnknown *,T1> *> (callback);
      return reference (a1);
    }
  return 0;
}

template <typename T1, typename T2>
NsUnknown *
NsUnknownManager::Create (ClassId classId, T1 a1, T2 a2)
{
  return 0;
}

template <typename T>
T *
NsUnknownManager::Create (ClassId classId, Iid iid)
{
  NsUnknown *obj = Create (classId);
  T *i = obj->QueryInterface<T> (iid);
  return i;
}

template <typename T, typename T1>
T *
NsUnknownManager::Create (ClassId classId, Iid iid, T1 a1)
{
  NsUnknown *obj = Create (classId, a1);
  T *i = obj->QueryInterface<T> (iid);
  return i;
}

template <typename T, typename T1, typename T2>
T *
NsUnknownManager::Create (ClassId classId, Iid iid, T1 a1, T2 a2)
{
  NsUnknown *obj = Create (classId, a1, a2);
  T *i = obj->QueryInterface<T> (iid);
  return i;
}



} // namespace ns3

#endif /* NS_UNKNOWN_MANAGER_H */
