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
#include "object.h"
#include "fatal-error.h"
#include "ptr.h"

namespace ns3 {

/**
 * \brief Unique Identifier for class constructors.
 *
 * Instances of this type must be allocated through
 * the ns3::Ns3UnknownManager::RegisterConstructor methods
 */
class ClassId
{
public:
  /**
   * \returns the symbolic name associated to this class id
   *
   * This name is the name which was associated to this class id
   * by the ns3::Ns3UnknownManager::RegisterConstructor methods.
   * This name is also the name which is expected to be input
   * to ns3::UnknownManager::LookupByName.
   */
  std::string GetName (void);
private:
  ClassId (std::string name);
  ClassId (uint32_t classId);
  friend class ComponentManager;
  friend bool operator == (const ClassId &a, const ClassId &b);
  uint32_t m_classId;
};

/**
 * \brief Create any Interface
 *
 * This class keeps track of a set of ClassId, each
 * of which uniquely identifies the constructor of an
 * object which derives from the Interface base class.
 * This class can also create an instance of any of
 * the objects tracked through any of their tracked
 * constructor/ClassId.
 */
class ComponentManager
{
public:
  /**
   * \param name the symbolic name to lookup
   * \returns the ClassId associated to the input name.
   */
  static ClassId LookupByName (std::string name);

  /**
   * \param classId class id of the constructor to invoke.
   * \return a pointer to the instance created.
   *
   * Create an instance of the object identified by its
   * ClassId. This method invokes the default constructor.
   */
  static Ptr<Object> Create (ClassId classId);

  /**
   * \param classId class id of the constructor to invoke.
   * \param a1 argument to pass to the constructor.
   * \return a pointer to the instance created.
   *
   * Create an instance of the object identified by its
   * ClassId.
   */
  template <typename T1>
  static Ptr<Object> Create (ClassId classId, T1 a1);

  /**
   * \param classId class id of the constructor to invoke.
   * \param a1 first argument to pass to the constructor.
   * \param a2 second argument to pass to the constructor.
   * \return a pointer to the instance created.
   *
   * Create an instance of the object identified by its
   * ClassId.
   */
  template <typename T1, typename T2>
  static Ptr<Object> Create (ClassId classId, T1 a1, T2 a2);

  /**
   * \param classId class id of the constructor to invoke.
   * \param iid interface id to query for
   * \return a pointer to the instance created.
   *
   * Create an instance of the object identified by its
   * ClassId, call QueryInterface on it, and return the 
   * result.
   */
  template <typename T>
  static Ptr<T> Create (ClassId classId, InterfaceId iid);

  template <typename T, typename T1>
  static Ptr<T> Create (ClassId classId, InterfaceId iid, T1 a1);

  template <typename T, typename T1, typename T2>
  static Ptr<T> Create (ClassId classId, InterfaceId iid, T1 a1, T2 a2);

  /**
   * \param name the symbolic name to associate to this
   *        constructor
   * \returns a ClassId which uniquely identifies this constructor.
   */
  template <typename T>
  static ClassId RegisterConstructor (std::string name)
  {
    static Callback<Ptr<Object> > callback = 
      MakeCallback (&ComponentManager::MakeObjectZero<T>);
    return ComponentManager::Register (name, &callback);
  }

  /**
   * \param name the symbolic name to associate to this
   *        constructor
   * \returns a ClassId which uniquely identifies this constructor.
   */
  template <typename T, typename T1>
  static ClassId RegisterConstructor (std::string name)
  {
    static Callback<Ptr<Object> ,T1> callback = MakeCallback (&ComponentManager::MakeObjectOne<T,T1>);
    return ComponentManager::Register (name, &callback);
  }

  /**
   * \param name the symbolic name to associate to this
   *        constructor
   * \returns a ClassId which uniquely identifies this constructor.
   */
  template <typename T, typename T1, typename T2>
  static ClassId RegisterConstructor (std::string name)
  {
    static Callback<Ptr<Object>,T1,T2> callback = MakeCallback (&ComponentManager::MakeObjectTwo<T,T1,T2>);
    return ComponentManager::Register (name, &callback);
  }
private:
  static ClassId Register (std::string name, CallbackBase *callback);

  template <typename T1, typename T2,
            typename T3, typename T4,
            typename T5>
  static Callback<Ptr<Object>,T1,T2,T3,T4,T5> DoGetCallback (ClassId classId);

  template <typename T>
  static Ptr<Object> MakeObjectZero (void);

  template <typename T, typename T1>
  static Ptr<Object> MakeObjectOne (T1 a1);

  template <typename T, typename T1, typename T2>
  static Ptr<Object> MakeObjectTwo (T1 a1, T2 a2);

  typedef std::vector<std::pair<ClassId, CallbackBase *> > List;
  static List *GetList (void);
  static CallbackBase *Lookup (ClassId classId);
};

} // namespace ns3 


namespace ns3 {

template <typename T1, typename T2,
          typename T3, typename T4,
          typename T5>
Callback<Ptr<Object>,T1,T2,T3,T4,T5>
ComponentManager::DoGetCallback (ClassId classId)
{
  CallbackBase *callback = Lookup (classId);
  if (callback == 0)
    {
      NS_FATAL_ERROR ("Invalid Class Id.");
    }
  Callback<Ptr<Object>, T1,T2,T3,T4,T5> reference;
  reference.Assign (*callback);
  return reference;
}


template <typename T1>
Ptr<Object>
ComponentManager::Create (ClassId classId, T1 a1)
{
  Callback<Ptr<Object>, T1> callback = DoGetCallback<T1,empty,empty,empty,empty> (classId);
  return callback (a1);
}

template <typename T1, typename T2>
Ptr<Object> 
ComponentManager::Create (ClassId classId, T1 a1, T2 a2)
{
  Callback<Ptr<Object> , T1,T2> callback = DoGetCallback<T1,T2,empty,empty,empty> (classId);
  return callback (a1, a2);
}

template <typename T>
Ptr<T>
ComponentManager::Create (ClassId classId, InterfaceId iid)
{
  Ptr<Object> obj = Create (classId);
  Ptr<T> i = obj->QueryInterface<T> (iid);
  return i;
}

template <typename T, typename T1>
Ptr<T>
ComponentManager::Create (ClassId classId, InterfaceId iid, T1 a1)
{
  Ptr<Object> obj = Create (classId, a1);
  Ptr<T> i = obj->QueryInterface<T> (iid);
  return i;
}

template <typename T, typename T1, typename T2>
Ptr<T>
ComponentManager::Create (ClassId classId, InterfaceId iid, T1 a1, T2 a2)
{
  Ptr<Object> obj = Create (classId, a1, a2);
  Ptr<T> i = obj->QueryInterface<T> (iid);
  return i;
}


template <typename T>
Ptr<Object> 
ComponentManager::MakeObjectZero (void)
{
  return MakeNewObject<T> ();
}
template <typename T, typename T1>
Ptr<Object> 
ComponentManager::MakeObjectOne (T1 a1)
{
  return MakeNewObject<T> (a1);
}
template <typename T, typename T1, typename T2>
Ptr<Object> 
ComponentManager::MakeObjectTwo (T1 a1, T2 a2)
{
  return MakeNewObject<T> (a1, a2);
}

} // namespace ns3

#endif /* NS_UNKNOWN_MANAGER_H */
