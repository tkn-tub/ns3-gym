/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA, Gustavo Carneiro
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
 * Authors: Gustavo Carneiro <gjcarneiro@gmail.com>,
 *          Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <string>
#include "ptr.h"
#include "trace-resolver.h"
#include "callback.h"
#include "value.h"
#include "object-base.h"

#define NS_OBJECT_ENSURE_REGISTERED(type)       \
  static struct X##type##RegistrationClass      \
  {                                             \
    X##type##RegistrationClass () {             \
      ns3::TypeId tid = type::GetTypeId ();     \
      tid.GetParent ();                         \
    }                                           \
} x_##type##RegistrationVariable


namespace ns3 {

class TraceContext;
class CallbackBase;
class Object;
class Accessor;
class Value;
class Parameters;

/**
 * \brief a unique identifier for an interface.
 *
 * This class records a lot of meta-information about a 
 * subclass of the Object base class:
 *  - the base class of the subclass
 *  - the set of accessible constructors in the subclass
 *  - the set of 'parameters' accessible in the subclass
 */
class TypeId
{
public:
  enum {
    ATTR_GET = 1<<0,
    ATTR_SET = 1<<1,
    ATTR_CONSTRUCT = 1<<2,
    ATTR_SGC = ATTR_GET | ATTR_SET | ATTR_CONSTRUCT,
  };

  /**
   * \param name the name of the requested interface
   * \returns the unique id associated with the requested
   *          name. 
   *
   * This method cannot fail: it will crash if the input 
   * name is not a valid interface name.
   */
  static TypeId LookupByName (std::string name);

  /**
   * \returns the number of TypeId instances constructed
   */
  static uint32_t GetRegisteredN (void);
  /**
   * \param i index
   * \returns the TypeId instance whose index is \i.
   */
  static TypeId GetRegistered (uint32_t i);

  /**
   * \param name the name of the interface to construct.
   *
   * No two instances can share the same name.
   */
  TypeId (std::string name);

  /**
   * \returns the parent of this TypeId
   *
   * This method cannot fail: it will crash if the input
   * id is not a valid interface id.
   */
  TypeId GetParent (void) const;

  /**
   * \returns the name of the group associated to this TypeId.
   */
  std::string GetGroupName (void) const;
  /**
   * \returns the fully-qualified C++ typename of this TypeId.
   */
  std::string GetTypeName (void) const;

  /**
   * \returns the name of this interface.
   */
  std::string GetName (void) const;

  /**
   * \returns true if this TypeId has a constructor
   */
  bool HasConstructor (void) const;

  /**
   * \returns the number of parameters associated to this TypeId
   */
  uint32_t GetParametersN (void) const;
  /**
   * \param i index into parameter array
   * \returns the name associated to the parameter whose
   *          index is \i i.
   */
  std::string GetParameterName (uint32_t i) const;
  /**
   * \param i index into parameter array
   * \returns the full name associated to the parameter whose
   *          index is \i i.
   */
  std::string GetParameterFullName (uint32_t i) const;

  Attribute GetParameterInitialValue (uint32_t i) const;

  Ptr<Object> CreateObject (const Parameters &parameters) const;


  Ptr<Object> CreateObject (void) const;
  template <typename T1>
  Ptr<Object> CreateObject (T1 a1) const;
  template <typename T1, typename T2>
  Ptr<Object> CreateObject (T1 a1, T2 a2) const;


  /**
   * \param tid the TypeId of the base class.
   * \return this TypeId instance.
   *
   * Record in this TypeId which TypeId is the TypeId
   * of the base class of the subclass.
   */
  TypeId SetParent (TypeId tid);
  /**
   * \return this TypeId instance.
   *
   * Record in this TypeId which TypeId is the TypeId
   * of the base class of the subclass.
   */
  template <typename T>
  TypeId SetParent (void);

  /**
   * \param groupName the name of the group this TypeId belongs to.
   * \returns this TypeId instance.
   *
   * The group name is purely an advisory information used to 
   * group together types according to a user-specific grouping
   * scheme.
   */
  TypeId SetGroupName (std::string groupName);
  
  /**
   * \param typeName the fully-qualified C++ typename of this TypeId.
   * \returns this TypeId instance.
   */
  TypeId SetTypeName (std::string typeName);

  /**
   * \returns this TypeId instance
   *
   * Record in this TypeId the fact that the default constructor
   * is accessible.
   */
  template <typename T>
  TypeId AddConstructor (void);

  template <typename T, typename T1>
  TypeId AddConstructor (void);

  template <typename T, typename T1, typename T2>
  TypeId AddConstructor (void);

  /**
   * \param name the name of the new parameter
   * \param help some help text which describes the purpose of this
   *        parameter
   * \param param an instance of the associated Accessor subclass
   * \returns this TypeId instance
   *
   * Record in this TypeId the fact that a new parameter exists.
   */
  TypeId AddParameter (std::string name,
                       std::string help, 
                       Attribute initialValue,
                       Ptr<const Accessor> spec,
                       Ptr<const AttributeChecker> checker);

  /**
   * \param name the name of the new parameter
   * \param help some help text which describes the purpose of this
   *        parameter
   * \param flags flags which describe how this parameter can be read and/or written.
   * \param param an instance of the associated Accessor subclass
   * \returns this TypeId instance
   *
   * Record in this TypeId the fact that a new parameter exists.
   */
  TypeId AddParameter (std::string name,
                       std::string help, 
                       uint32_t flags,
                       Attribute initialValue,
                       Ptr<const Accessor> spec,
                       Ptr<const AttributeChecker> checker);

  // construct an invalid TypeId.
  TypeId ();
  ~TypeId ();
private:
  friend class Object;
  friend class Parameters;
  friend bool operator == (TypeId a, TypeId b);
  friend bool operator != (TypeId a, TypeId b);

  struct ParameterInfo {
    Ptr<const Accessor> spec;
    Attribute initialValue;
    uint32_t flags;
    Ptr<const AttributeChecker> checker;
  };

  /**
   * \param name the name of the requested parameter
   * \returns the Accessor associated to the requested parameter
   */
  bool LookupParameterByName (std::string name, struct ParameterInfo *info) const;
  /**
   * \param i the position of the requested parameter
   * \returns the Accessor associated to the requested parameter
   */
  bool LookupParameterByPosition (uint32_t i, struct ParameterInfo *info) const;
  /**
   * \param fullName the full name of the requested parameter
   * \returns the Accessor associated to the requested parameter
   */
  static bool LookupParameterByFullName (std::string fullName, struct ParameterInfo *info);

  explicit TypeId (uint16_t tid);
  void DoAddConstructor (CallbackBase callback, uint32_t nArguments);
  CallbackBase LookupConstructor (uint32_t nArguments) const;
  Ptr<const Accessor> GetParameterAccessor (uint32_t i) const;
  uint32_t GetParameterFlags (uint32_t i) const;
  Ptr<const AttributeChecker> GetParameterChecker (uint32_t i) const;
  
  uint16_t m_tid;
};


/**
 * \brief a container of parameters to be used during object's construction
 *        and in ns3::Object::Set.
 *
 */
class Parameters
{
public:
  Parameters ();
  Parameters (const Parameters &o);
  Parameters &operator = (const Parameters &o);
  ~Parameters ();
  /**
   * \param name the name of the parameter to set
   * \param value the value to set
   *
   * This method checks that a parameter with the requested
   * name exists and that the value specified is an acceptable
   * value of that parameter. If any of these checks fails,
   * the program terminates with a message.
   */
  bool Set (std::string name, Attribute value);

  void SetWithTid (TypeId tid, std::string name, Attribute value);
  void SetWithTid (TypeId tid, uint32_t position, Attribute value);

  /**
   * Clear the content of this instance.
   */
  void Reset (void);

  /**
   * \returns the global parameter container
   *
   * The global parameter container can be used to specify
   * a set of parameter values without having to re-specify
   * them for each object when it is created. This container
   * is checked only during object construction and 
   * it is always checked last, after any per-object
   * container is checked.
   */
  static Parameters *GetGlobal (void);

  std::string SerializeToString (void) const;
  bool DeserializeFromString (std::string value);
private:
  friend class Object;
  struct Param {
    Ptr<const AttributeChecker> checker;
    Attribute value;
  };
  typedef std::vector<struct Param> Params;
  typedef Params::iterator Iterator;
  typedef Params::const_iterator CIterator;



  bool DoSet (struct TypeId::ParameterInfo *info, Attribute param);
  void DoSetOne (Ptr<const AttributeChecker> checker, Attribute param);
  std::string LookupParameterFullNameByChecker (Ptr<const AttributeChecker> checker) const;

  Params m_parameters;
};


/**
 * \brief a base class which provides memory management and object aggregation
 *
 */
class Object : public ObjectBase
{
public:
  static TypeId GetTypeId (void);

  Object ();
  virtual ~Object ();

  /**
   * \param name the name of the parameter to set
   * \param value the name of the parameter to set
   *
   * Set a single parameter.
   */
  bool Set (std::string name, Attribute value);
  /**
   * \param name the name of the parameter to read
   * \param value a reference to the string where the value of the 
   *        parameter should be stored.
   * \returns true if the requested parameter was found, false otherwise.
   */
  bool Get (std::string name, std::string &value) const;
  /**
   * \param name the name of the parameter to read
   * \param value a reference to the object where the value of the 
   *        parameter should be stored.
   * \returns true if the requested parameter was found, false otherwise.
   */
  Attribute Get (std::string name) const;

  /**
   * Increment the reference count. This method should not be called
   * by user code. Object instances are expected to be used in conjunction
   * of the Ptr template which would make calling Ref unecessary and 
   * dangerous.
   */
  inline void Ref (void) const;
  /**
   * Decrement the reference count. This method should not be called
   * by user code. Object instances are expected to be used in conjunction
   * of the Ptr template which would make calling Ref unecessary and 
   * dangerous.
   */
  inline void Unref (void) const;
  /**
   * \returns a pointer to the requested interface or zero if it could not be found.
   */
  template <typename T>
  Ptr<T> GetObject (void) const;
  /**
   * \param tid the interface id of the requested interface
   * \returns a pointer to the requested interface or zero if it could not be found.
   */
  template <typename T>
  Ptr<T> GetObject (TypeId tid) const;
  /**
   * Run the DoDispose methods of this object and all the
   * objects aggregated to it.
   * After calling this method, the object is expected to be
   * totally unusable except for the Ref and Unref methods.
   * It is an error to call Dispose twice on the same object 
   * instance
   */
  void Dispose (void);
  /**
   * \param other another object pointer
   *
   * This method aggregates the two objects together: after this
   * method returns, it becomes possible to call GetObject
   * on one to get the other, and vice-versa. 
   */
  void AggregateObject (Ptr<Object> other);

  /**
   * \param path the path to match for the callback
   * \param cb callback to connect
   *
   * Connect the input callback to all trace sources which
   * match the input path.
   *
   */
  void TraceConnect (std::string path, const CallbackBase &cb) const;
  /**
   * \param path the path to match for the callback
   * \param cb callback to disconnect
   *
   * Disconnect the input callback from all trace sources which
   * match the input path.
   */
  void TraceDisconnect (std::string path, const CallbackBase &cb) const;
  /**
   * \returns the trace resolver associated to this object.
   *
   * This method should be rarely called by users.
   */
  virtual Ptr<TraceResolver> GetTraceResolver (void) const;
protected:
  /**
   * This method is called by Object::Dispose.
   * Subclasses are expected to override this method and chain
   * up to their parent's implementation once they are done.
   */
  virtual void DoDispose (void);
  virtual void NotifyConstructionCompleted (void);
private:
  friend class TypeIdTraceResolver;

  template <typename T>
  friend Ptr<T> CreateObject (const Parameters &parameters);

  template <typename T>
  friend Ptr<T> CreateObject (void);
  template <typename T, typename T1>
  friend Ptr<T> CreateObject (T1 a1);
  template <typename T, typename T1, typename T2>
  friend Ptr<T> CreateObject (T1 a1, T2 a2);
  template <typename T, typename T1, typename T2, typename T3>
  friend Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3);
  template <typename T, typename T1, typename T2, typename T3, typename T4>
  friend Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4);
  template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
  friend Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
  template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  friend Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);
  template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  friend Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7);


  bool DoSet (Ptr<const Accessor> spec, Attribute intialValue, 
              Ptr<const AttributeChecker> checker, Attribute value);
  Ptr<Object> DoGetObject (TypeId tid) const;
  void DoCollectSources (std::string path, const TraceContext &context, 
                         TraceResolver::SourceCollection *collection) const;
  void DoTraceAll (std::ostream &os, const TraceContext &context) const;
  bool Check (void) const;
  bool CheckLoose (void) const;
  /**
   * Attempt to delete this object. This method iterates
   * over all aggregated objects to check if they all 
   * have a zero refcount. If yes, the object and all
   * its aggregates are deleted. If not, nothing is done.
   */
  void MaybeDelete (void) const;
  /**
   * \param tid an TypeId
   *
   * Invoked from ns3::CreateObject only.
   * Initialize the m_tid member variable to
   * keep track of the type of this object instance.
   */
  void SetTypeId (TypeId tid);
  /**
   * \param parameters the parameter values used to initialize 
   *        the member variables of this object's instance.
   *
   * Invoked from ns3::CreateObject only.
   * Initialize all the member variables which were
   * registered with the associated TypeId.
   */
  void Construct (const Parameters &parameters);

  /**
   * The reference count for this object. Each aggregate
   * has an individual reference count. When the global
   * reference count (the sum of all reference counts) 
   * reaches zero, the object and all its aggregates is 
   * deleted.
   */
  mutable uint32_t m_count;
  /**
   * Identifies the type of this object instance.
   */
  TypeId m_tid;
  /**
   * Set to true when the DoDispose method of the object
   * has run, false otherwise.
   */
  bool m_disposed;
  mutable bool m_collecting;
  /**
   * A pointer to the next aggregate object. This is a circular
   * linked list of aggregated objects: the last one points
   * back to the first one. If an object is not aggregated to
   * any other object, the value of this field is equal to the
   * value of the 'this' pointer.
   */
  Object *m_next;
};

} // namespace ns3

namespace ns3 {

/*************************************************************************
 *   The TypeId implementation which depends on templates
 *************************************************************************/

template <typename T>
TypeId 
TypeId::SetParent (void)
{
  return SetParent (T::GetTypeId ());
}

template <typename T>
TypeId 
TypeId::AddConstructor (void)
{
  struct Maker {
    static Ptr<Object> Create (const Parameters &parameters) {
      return ns3::CreateObject<T> (parameters);
    }
  };
  CallbackBase cb = MakeCallback (&Maker::Create);
  DoAddConstructor (cb, 0);
  return *this;
}
template <typename T, typename T1>
TypeId 
TypeId::AddConstructor (void)
{
  struct Maker {
    static Ptr<Object> Create (T1 a1) {
      return ns3::CreateObject<T,T1> (a1);
    }
  };
  CallbackBase cb = MakeCallback (&Maker::Create);
  DoAddConstructor (cb, 1);
  return *this;
}
template <typename T, typename T1, typename T2>
TypeId 
TypeId::AddConstructor (void)
{
  struct Maker {
    static Ptr<Object> Create (T1 a1, T2 a2) {
      return ns3::CreateObject<T,T1,T2> (a1, a2);
    }
  };
  CallbackBase cb = MakeCallback (&Maker::Create);
  DoAddConstructor (cb, 2);
  return *this;
}
template <typename T1>
Ptr<Object> 
TypeId::CreateObject (T1 a1) const
{
  CallbackBase cb = LookupConstructor (1);
  Callback<Ptr<Object>,T1> realCb;
  realCb.Assign (cb);
  Ptr<Object> object = realCb (a1);
  return object;
}
template <typename T1, typename T2>
Ptr<Object> 
TypeId::CreateObject (T1 a1, T2 a2) const
{
  CallbackBase cb = LookupConstructor (2);
  Callback<Ptr<Object>,T1,T2> realCb;
  realCb.Assign (cb);
  Ptr<Object> object = realCb (a1,a2);
  return object;
}

/*************************************************************************
 *   The Object implementation which depends on templates
 *************************************************************************/

void
Object::Ref (void) const
{
  m_count++;
}
void
Object::Unref (void) const
{
  NS_ASSERT (Check ());
  m_count--;
  if (m_count == 0)
    {
      MaybeDelete ();
    }
}

template <typename T>
Ptr<T> 
Object::GetObject () const
{
  Ptr<Object> found = DoGetObject (T::GetTypeId ());
  if (found != 0)
    {
      return Ptr<T> (dynamic_cast<T *> (PeekPointer (found)));
    }
  return 0;
}

template <typename T>
Ptr<T> 
Object::GetObject (TypeId tid) const
{
  Ptr<Object> found = DoGetObject (tid);
  if (found != 0)
    {
      return Ptr<T> (dynamic_cast<T *> (PeekPointer (found)));
    }
  return 0;
}

/*************************************************************************
 *   The helper functions which need templates.
 *************************************************************************/


template <typename T>
Ptr<T> CreateObject (const Parameters &parameters)
{
  Ptr<T> p = Ptr<T> (new T (), false);
  p->SetTypeId (T::GetTypeId ());
  p->Object::Construct (parameters);
  return p;  
}

template <typename T>
Ptr<T> CreateObject (void)
{
  Ptr<T> p = Ptr<T> (new T (), false);
  p->SetTypeId (T::GetTypeId ());
  p->Object::Construct (Parameters ());
  return p;
}

template <typename T, typename T1>
Ptr<T> CreateObject (T1 a1)
{
  Ptr<T> p = Ptr<T> (new T (a1), false);
  p->SetTypeId (T::GetTypeId ());
  p->Object::Construct (Parameters ());
  return p;
}

template <typename T, typename T1, typename T2>
Ptr<T> CreateObject (T1 a1, T2 a2)
{
  Ptr<T> p = Ptr<T> (new T (a1, a2), false);
  p->SetTypeId (T::GetTypeId ());
  p->Object::Construct (Parameters ());
  return p;
}
template <typename T, typename T1, typename T2, typename T3>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3)
{
  Ptr<T> p = Ptr<T> (new T (a1, a2, a3), false);
  p->SetTypeId (T::GetTypeId ());
  return p;
}

template <typename T, typename T1, typename T2, typename T3, typename T4>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4)
{
  Ptr<T> p = Ptr<T> (new T (a1, a2, a3, a4), false);
  p->SetTypeId (T::GetTypeId ());
  return p;
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  Ptr<T> p = Ptr<T> (new T (a1, a2, a3, a4, a5), false);
  p->SetTypeId (T::GetTypeId ());
  return p;
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  Ptr<T> p = Ptr<T> (new T (a1, a2, a3, a4, a5, a6), false);
  p->SetTypeId (T::GetTypeId ());
  return p;
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7)
{
  Ptr<T> p = Ptr<T> (new T (a1, a2, a3, a4, a5, a6, a7), false);
  p->SetTypeId (T::GetTypeId ());
  return p;
}


template <typename T>
Ptr<T> 
CreateObjectWith (std::string n1, Attribute v1,
                  std::string n2 = "", Attribute v2 = Attribute ())
              
{
  Parameters parameters;
  parameters.SetWithTid (T::GetTypeId (), n1, v1);
  parameters.SetWithTid (T::GetTypeId (), n2, v2);
  return CreateObject<T> (parameters);
}

} // namespace ns3

#endif /* OBJECT_H */

