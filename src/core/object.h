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
#include <vector>
#include "ptr.h"
#include "callback.h"
#include "attribute.h"
#include "object-base.h"
#include "attribute-helper.h"

/**
 * This macro should be invoked once for every class which
 * defines a new GetTypeId method.
 */
#define NS_OBJECT_ENSURE_REGISTERED(type)       \
  static struct X##type##RegistrationClass      \
  {                                             \
    X##type##RegistrationClass () {             \
      ns3::TypeId tid = type::GetTypeId ();     \
      tid.GetParent ();                         \
    }                                           \
} x_##type##RegistrationVariable


namespace ns3 {

class CallbackBase;
class Object;
class AttributeAccessor;
class AttributeValue;
class AttributeList;
class TraceSourceAccessor;

/**
 * \brief a unique identifier for an interface.
 *
 * This class records a lot of meta-information about a 
 * subclass of the Object base class:
 *  - the base class of the subclass
 *  - the set of accessible constructors in the subclass
 *  - the set of 'attributes' accessible in the subclass
 */
class TypeId
{
public:
  enum {
    /**
     * The attribute can be read
     */
    ATTR_GET = 1<<0,
    /**
     * The attribute can be written
     */
    ATTR_SET = 1<<1,
    /**
     * The attribute can be written at construction-time.
     */
    ATTR_CONSTRUCT = 1<<2,
    /**
     * The attribute can be read, and written at any time.
     */
    ATTR_SGC = ATTR_GET | ATTR_SET | ATTR_CONSTRUCT,
  };

  /**
   * \param name the name of the requested TypeId
   * \returns the unique id associated with the requested
   *          name. 
   *
   * This method cannot fail: it will crash if the input 
   * name is not a valid TypeId name.
   */
  static TypeId LookupByName (std::string name);
  /**
   * \param name the name of the requested TypeId
   * \param tid a pointer to the TypeId instance where the 
   *        result of this function should be stored.
   * \returns true if the requested name was found, false otherwise.
   */
  static bool LookupByNameFailSafe (std::string name, TypeId *tid);

  /**
   * \returns the number of TypeId instances registered.
   */
  static uint32_t GetRegisteredN (void);
  /**
   * \param i index
   * \returns the TypeId instance whose index is i.
   */
  static TypeId GetRegistered (uint32_t i);

  /**
   * \param name the name of the interface to construct.
   *
   * No two instances can share the same name. The name is expected to be
   * the full c++ typename of associated c++ object.
   */
  TypeId (const char * name);

  /**
   * \returns the parent of this TypeId
   *
   * This method cannot fail. It will return itself
   * if this TypeId has no parent. i.e., it is at the top
   * of the TypeId hierarchy. Currently, this is the
   * case for the TypeId associated to the Object class 
   * only.
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
   * \returns the number of attributes associated to this TypeId
   */
  uint32_t GetAttributeListN (void) const;
  /**
   * \param i index into attribute array
   * \returns the name associated to the attribute whose
   *          index is i.
   */
  std::string GetAttributeName (uint32_t i) const;
  std::string GetAttributeHelp (uint32_t i) const;
  /**
   * \param i index into attribute array
   * \returns the full name associated to the attribute whose
   *          index is i.
   */
  std::string GetAttributeFullName (uint32_t i) const;

  Attribute GetAttributeInitialValue (uint32_t i) const;
  uint32_t GetAttributeFlags (uint32_t i) const;
  Ptr<const AttributeChecker> GetAttributeChecker (uint32_t i) const;


  uint32_t GetTraceSourceN (void) const;
  std::string GetTraceSourceName (uint32_t i) const;
  std::string GetTraceSourceHelp (uint32_t i) const;
  Ptr<const TraceSourceAccessor> GetTraceSourceAccessor (uint32_t i) const;

  Ptr<Object> CreateObject (const AttributeList &attributes) const;
  Ptr<Object> CreateObject (void) const;

  bool MustHideFromDocumentation (void) const;

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

  /**
   * \param name the name of the new attribute
   * \param help some help text which describes the purpose of this
   *        attribute.
   * \param initialValue the initial value for this attribute.
   * \param accessor an instance of the associated AttributeAccessor subclass.
   * \param checker an instance of the associated AttributeChecker subclass.
   * \returns this TypeId instance
   *
   * Record in this TypeId the fact that a new attribute exists.
   */
  TypeId AddAttribute (std::string name,
                       std::string help, 
                       Attribute initialValue,
                       Ptr<const AttributeAccessor> accessor,
                       Ptr<const AttributeChecker> checker);

  /**
   * \param name the name of the new attribute
   * \param help some help text which describes the purpose of this
   *        attribute
   * \param flags flags which describe how this attribute can be read and/or written.
   * \param initialValue the initial value for this attribute.
   * \param accessor an instance of the associated AttributeAccessor subclass.
   * \param checker an instance of the associated AttributeChecker subclass.
   * \returns this TypeId instance
   *
   * Record in this TypeId the fact that a new attribute exists.
   */
  TypeId AddAttribute (std::string name,
                       std::string help, 
                       uint32_t flags,
                       Attribute initialValue,
                       Ptr<const AttributeAccessor> accessor,
                       Ptr<const AttributeChecker> checker);

  /**
   * \param name the name of the new trace source
   * \param help some help text which describes the purpose of this
   *        trace source.
   * \param accessor a pointer to a TraceSourceAccessor which can be
   *        used to connect/disconnect sinks to this trace source.
   * \returns this TypeId instance.
   */
  TypeId AddTraceSource (std::string name,
                         std::string help,
                         Ptr<const TraceSourceAccessor> accessor);

  TypeId HideFromDocumentation (void);

  /**
   * \brief store together a set of attribute properties.
   */
  struct AttributeInfo {
    // The accessor associated to the attribute.
    Ptr<const AttributeAccessor> accessor;
    // The initial value associated to the attribute.
    Attribute initialValue;
    // The set of access control flags associated to the attribute.
    uint32_t flags;
    // The checker associated to the attribute.
    Ptr<const AttributeChecker> checker;
  };
  /**
   * \param name the name of the requested attribute
   * \param info a pointer to the TypeId::AttributeInfo data structure
   *        where the result value of this method will be stored.
   * \returns true if the requested attribute could be found, false otherwise.
   */
  bool LookupAttributeByName (std::string name, struct AttributeInfo *info) const;


  // construct an invalid TypeId.
  TypeId ();
  ~TypeId ();

  ATTRIBUTE_HELPER_HEADER_1 (TypeId);
private:
  friend class Object;
  friend class AttributeList;
  friend bool operator == (TypeId a, TypeId b);
  friend bool operator != (TypeId a, TypeId b);


  Ptr<const TraceSourceAccessor> LookupTraceSourceByName (std::string name) const;

  /**
   * \param fullName the full name of the requested attribute
   * \param info a pointer to the TypeId::AttributeInfo data structure
   *        where the result value of this method will be stored.
   * \returns the Accessor associated to the requested attribute
   */
  static bool LookupAttributeByFullName (std::string fullName, struct AttributeInfo *info);

  explicit TypeId (uint16_t tid);
  void DoAddConstructor (CallbackBase callback);
  CallbackBase LookupConstructor (void) const;
  Ptr<const AttributeAccessor> GetAttributeAccessor (uint32_t i) const;
  
  uint16_t m_tid;
};

std::ostream & operator << (std::ostream &os, TypeId tid);
std::istream & operator >> (std::istream &is, TypeId &tid);

ATTRIBUTE_HELPER_HEADER_2 (TypeId);

/**
 * \brief a container of attributes to be used during object's construction
 *        and in ns3::Object::Set.
 *
 */
class AttributeList
{
public:
  AttributeList ();
  AttributeList (const AttributeList &o);
  AttributeList &operator = (const AttributeList &o);
  ~AttributeList ();
  /**
   * \param name the full name of the attribute to set
   * \param value the value to set
   *
   * This method checks that a attribute with the requested
   * name exists and that the value specified is an acceptable
   * value of that attribute. If any of these checks fails,
   * the program terminates with a message.
   */
  void Set (std::string name, Attribute value);
  /**
   * \param name the full name of the attribute to set
   * \param value the value to set
   * \returns true if the requested attribute exists and could be
   *          stored, false otherwise.
   */
  bool SetFailSafe (std::string name, Attribute value);
  /**
   * \param tid the TypeId associated to this attribute
   * \param name the name (not full!) of the attribute
   * \param value the value to set
   *
   * This method checks that a attribute with the requested
   * name exists and that the value specified is an acceptable
   * value of that attribute. If any of these checks fails,
   * the program terminates with a message.
   */
  void SetWithTid (TypeId tid, std::string name, Attribute value);

  /**
   * Clear the content of this instance.
   */
  void Reset (void);

  /**
   * \returns the global attribute container
   *
   * The global attribute container can be used to specify
   * a set of attribute values without having to re-specify
   * them for each object when it is created. This container
   * is checked only during object construction and 
   * it is always checked last, after any per-object
   * container is checked.
   */
  static AttributeList *GetGlobal (void);

  // XXX: untested.
  std::string SerializeToString (void) const;
  bool DeserializeFromString (std::string value);
private:
  friend class Object;
  struct Attr {
    Ptr<const AttributeChecker> checker;
    Attribute value;
  };
  typedef std::vector<struct Attr> Attrs;
  typedef Attrs::iterator Iterator;
  typedef Attrs::const_iterator CIterator;



  bool DoSet (struct TypeId::AttributeInfo *info, Attribute param);
  void DoSetOne (Ptr<const AttributeChecker> checker, Attribute param);
  std::string LookupAttributeFullNameByChecker (Ptr<const AttributeChecker> checker) const;

  Attrs m_attributes;
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
   * \param name the name of the attribute to set
   * \param value the name of the attribute to set
   *
   * Set a single attribute. This cannot fail: if the input is invalid,
   * it will crash immediately.
   */
  void SetAttribute (std::string name, Attribute value);
  /**
   * \param name the name of the attribute to set
   * \param value the name of the attribute to set
   * \returns true if the requested attribute exists and could be set, 
   * false otherwise.
   */
  bool SetAttributeFailSafe (std::string name, Attribute value);
  /**
   * \param name the name of the attribute to read
   * \param value a reference to the string where the value of the 
   *        attribute should be stored.
   * \returns true if the requested attribute was found, false otherwise.
   */
  bool GetAttribute (std::string name, std::string &value) const;
  /**
   * \param name the name of the attribute to read
   * \returns the attribute read.
   *
   * If the input attribute name does not exist, this method crashes.
   */
  Attribute GetAttribute (std::string name) const;

  bool TraceConnectWithoutContext (std::string name, const CallbackBase &cb);
  bool TraceConnectWithoutContext (std::string name, std::string context, const CallbackBase &cb);
  bool TraceDisconnectWithoutContext (std::string name, const CallbackBase &cb);
  bool TraceDisconnectWithoutContext (std::string name, std::string context, const CallbackBase &cb);

  TypeId GetRealTypeId (void) const;

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

protected:
  /**
   * This method is called by Object::Dispose.
   * Subclasses are expected to override this method and chain
   * up to their parent's implementation once they are done.
   */
  virtual void DoDispose (void);
  virtual void NotifyConstructionCompleted (void);
private:
  template <typename T>
  friend Ptr<T> CreateObject (const AttributeList &attributes);
  template <typename T>
  friend Ptr<T> CopyObject (Ptr<T> object);

  bool DoSet (Ptr<const AttributeAccessor> spec, Attribute intialValue, 
              Ptr<const AttributeChecker> checker, Attribute value);
  Ptr<Object> DoGetObject (TypeId tid) const;
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
   * \param attributes the attribute values used to initialize 
   *        the member variables of this object's instance.
   *
   * Invoked from ns3::CreateObject only.
   * Initialize all the member variables which were
   * registered with the associated TypeId.
   */
  void Construct (const AttributeList &attributes);

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
    static Ptr<Object> Create (const AttributeList &attributes) {
      return ns3::CreateObject<T> (attributes);
    }
  };
  CallbackBase cb = MakeCallback (&Maker::Create);
  DoAddConstructor (cb);
  return *this;
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
Ptr<T> CopyObject (Ptr<T> o)
{
  Ptr<T> p = Ptr<T> (new T (*PeekPointer (o)), false);
  p->SetTypeId (T::GetTypeId ());
  return p;
}


template <typename T>
Ptr<T> CreateObject (const AttributeList &attributes)
{
  Ptr<T> p = Ptr<T> (new T (), false);
  p->SetTypeId (T::GetTypeId ());
  p->Object::Construct (attributes);
  return p;  
}

template <typename T>
Ptr<T> 
CreateObject (std::string n1 = "", Attribute v1 = Attribute (),
              std::string n2 = "", Attribute v2 = Attribute (),
              std::string n3 = "", Attribute v3 = Attribute (),
              std::string n4 = "", Attribute v4 = Attribute (),
              std::string n5 = "", Attribute v5 = Attribute (),
              std::string n6 = "", Attribute v6 = Attribute (),
              std::string n7 = "", Attribute v7 = Attribute (),
              std::string n8 = "", Attribute v8 = Attribute (),
              std::string n9 = "", Attribute v9 = Attribute ())
  
{
  AttributeList attributes;
  if (n1 == "")
    {
      goto end;
    }
  attributes.SetWithTid (T::GetTypeId (), n1, v1);
  if (n2 == "")
    {
      goto end;
    }
  attributes.SetWithTid (T::GetTypeId (), n2, v2);
  if (n3 == "")
    {
      goto end;
    }
  attributes.SetWithTid (T::GetTypeId (), n3, v3);
  if (n4 == "")
    {
      goto end;
    }
  attributes.SetWithTid (T::GetTypeId (), n4, v4);
  if (n5 == "")
    {
      goto end;
    }
  attributes.SetWithTid (T::GetTypeId (), n5, v5);
  if (n6 == "")
    {
      goto end;
    }
  attributes.SetWithTid (T::GetTypeId (), n6, v6);
  if (n7 == "")
    {
      goto end;
    }
  attributes.SetWithTid (T::GetTypeId (), n7, v7);
  if (n8 == "")
    {
      goto end;
    }
  attributes.SetWithTid (T::GetTypeId (), n8, v8);
  if (n9 == "")
    {
      goto end;
    }
  attributes.SetWithTid (T::GetTypeId (), n9, v9);
 end:
  return CreateObject<T> (attributes);
}

} // namespace ns3

#endif /* OBJECT_H */

