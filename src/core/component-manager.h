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
#ifndef NS_COMPONENT_MANAGER_H
#define NS_COMPONENT_MANAGER_H

#include <string>
#include <vector>
#include <stdint.h>
#include "callback.h"
#include "object.h"
#include "fatal-error.h"
#include "ptr.h"
#include "empty.h"
#include "default-value.h"

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
  std::string GetName (void) const;
protected:
  ClassId (std::string name);
private:
  ClassId (uint32_t classId);
  friend class ComponentManager;
  friend ClassId AllocateClassId (std::string name);
  friend bool operator == (const ClassId &a, const ClassId &b);
  uint32_t m_classId;
};

template <typename T, typename T1 = empty, typename T2 = empty>
class MakeClassId;


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
  static ClassId LookupByName (std::string name, bool *ok);
  /**
   * \param iid interface id to lookup
   * \returns the list of ClassId which can be used to
   *          create objects which support the requested 
   *          interface.
   *
   * Note that this method will not necessarily return the
   * complete list of objects which support a given interface
   * since dynamic aggregation of objects is not under
   * the control of this class.
   */
  static std::vector<ClassId> LookupByInterfaceId (InterfaceId iid);

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

private:
  friend void RegisterCallback (ClassId classId, CallbackBase *callback, 
                                   std::vector<const InterfaceId *> supportedInterfaces);
  static void Register (ClassId classId, CallbackBase *callback, 
                        std::vector<const InterfaceId *> supportedInterfaces);

  template <typename T1, typename T2,
            typename T3, typename T4,
            typename T5>
  static Callback<Ptr<Object>,T1,T2,T3,T4,T5> DoGetCallback (ClassId classId);

  struct ClassIdEntry {
    ClassIdEntry (ClassId classId);
    ClassId m_classId;
    CallbackBase *m_callback;
    std::vector<const InterfaceId *> m_supportedInterfaces;
  };

  typedef std::vector<struct ClassIdEntry> List;
  static List *GetList (void);
  static CallbackBase *Lookup (ClassId classId);
};

class ClassIdDefaultValue : public DefaultValueBase
{
public:
  ClassIdDefaultValue (std::string name, 
                       std::string help,
                       const InterfaceId &iid,
                       std::string defaultValue);
  ClassId GetValue (void) const;
  void SetValue (ClassId classId);
  void SetValue (std::string name);
private:
  virtual bool DoParseValue (const std::string &value);
  virtual std::string DoGetType (void) const;
  virtual std::string DoGetDefaultValue (void) const;
  std::string m_defaultName;
  std::string m_name;
  const InterfaceId *m_interfaceId;
};

} // namespace ns3 


namespace {
  // anonymous namespace for implementation code.
template <typename T, typename T1 = ns3::empty, typename T2 = ns3::empty>
struct ObjectMaker;

template <typename T>
struct ObjectMaker<T,ns3::empty,ns3::empty> {
  static ns3::Ptr<ns3::Object> 
  MakeObject (void) {
    return ns3::MakeNewObject<T> ();
  }
};

template <typename T, typename T1>
struct ObjectMaker<T,T1,ns3::empty> {
  static ns3::Ptr<ns3::Object> 
  MakeObject (T1 a1) {
    return ns3::MakeNewObject<T> (a1);
  }
};

template <typename T, typename T1, typename T2>
struct ObjectMaker {
  static ns3::Ptr<ns3::Object> 
  MakeObject (T1 a1, T2 a2) {
    return ns3::MakeNewObject<T> (a1, a2);
  }
};

} // anonymous namespace

namespace ns3 {
  
void RegisterCallback (ClassId classId, ns3::CallbackBase *callback, 
                       std::vector<const InterfaceId *> supportedInterfaces);


template <typename T, typename T1, typename T2>
class MakeClassId : public ClassId
{
private:
  typedef ObjectMaker<T,T1,T2> MakerType;
  static Callback<Ptr<Object>,T1,T2> m_callback;
  void Register (const InterfaceId *array [], uint32_t n) {
    std::vector<const InterfaceId *> supportedInterfaces;
    for (uint32_t i = 0; i < n; i++)
      {
        supportedInterfaces.push_back (array[i]);
      }
    RegisterCallback (*this, &m_callback, supportedInterfaces);
  }
public:
  MakeClassId (std::string name) : ClassId (name) {
    const InterfaceId *array[] = {};
    Register (array, sizeof (array)/sizeof(InterfaceId *));
  }
  MakeClassId (std::string name, 
               const InterfaceId &iid) : ClassId (name) {
    const InterfaceId *array[] = {&iid};
    Register (array, sizeof (array)/sizeof(InterfaceId *));
  }
  MakeClassId (std::string name, 
               const InterfaceId &iid0, 
               const InterfaceId iid1) : ClassId (name) {
    const InterfaceId *array[] = {&iid0, &iid1};
    Register (array, sizeof (array)/sizeof(InterfaceId *));
  }
  MakeClassId (std::string name, 
               const InterfaceId &iid0, 
               const InterfaceId &iid1,
               const InterfaceId &iid2) : ClassId (name) {
    const InterfaceId *array[] = {&iid0, &iid1, &iid2};
    Register (array, sizeof (array)/sizeof(InterfaceId *));
  }
  MakeClassId (std::string name, 
               const InterfaceId &iid0, 
               const InterfaceId &iid1,
               const InterfaceId &iid2,
               const InterfaceId &iid3) : ClassId (name) {
    const InterfaceId *array[] = {&iid0, &iid1, &iid2, &iid3};
    Register (array, sizeof (array)/sizeof(InterfaceId *));
  }
  MakeClassId (std::string name, 
               const InterfaceId &iid0, 
               const InterfaceId &iid1,
               const InterfaceId &iid2,
               const InterfaceId &iid3,
               const InterfaceId &iid4) : ClassId (name) {
    const InterfaceId *array[] = {&iid0, &iid1, iid2, &iid3, &iid4};
    Register (array, sizeof (array)/sizeof(InterfaceId *));
  }
};

template <typename T, typename T1, typename T2>
Callback<Ptr<Object>,T1,T2> MakeClassId<T,T1,T2>::m_callback = MakeCallback (&MakeClassId::MakerType::MakeObject);



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
  Callback<Ptr<Object>,T1,T2,T3,T4,T5> reference;
  reference.Assign (*callback);
  return reference;
}


template <typename T1>
Ptr<Object>
ComponentManager::Create (ClassId classId, T1 a1)
{
  Callback<Ptr<Object>,T1> callback = DoGetCallback<T1,empty,empty,empty,empty> (classId);
  return callback (a1);
}

template <typename T1, typename T2>
Ptr<Object> 
ComponentManager::Create (ClassId classId, T1 a1, T2 a2)
{
  Callback<Ptr<Object>,T1,T2> callback = DoGetCallback<T1,T2,empty,empty,empty> (classId);
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

} // namespace ns3

#endif /* NS_COMPONENT_MANAGER_H */
