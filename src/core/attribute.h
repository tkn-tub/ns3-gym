/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <stdint.h>
#include "ptr.h"

namespace ns3 {

class AttributeAccessor;
class AttributeChecker;
class Attribute;
class ObjectBase;

/**
 * \brief Hold a value for an Attribute.
 *
 * Instances of this class are usually created by Attribute::Create<> and
 * should always be wrapped into an Attribute object.
 * Most subclasses of this base class are implemented by the 
 * ATTRIBUTE_HELPER_* macros.
 */
class AttributeValue
{
public:
  AttributeValue ();
  AttributeValue (const AttributeValue &o);
  AttributeValue &operator = (const AttributeValue &o);
  virtual ~AttributeValue ();

  /**
   * \returns a deep copy of this class, wrapped into an Attribute object.
   */
  virtual Attribute Copy (void) const = 0;
  /**
   * \param checker the checker associated to the attribute
   * \returns a string representation of this value.
   *
   * In most cases, this method will not make any use of the checker argument.
   * However, in a very limited set of cases, the checker argument is needed to
   * perform proper serialization. A nice example of code which needs it is
   * the EnumValue::SerializeToString code.
   */
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const = 0;
  /**
   * \param value a string representation of the value
   * \param checker a pointer to the checker associated to the attribute.
   * \returns true if the input string was correctly-formatted and could be
   *          successfully deserialized, false otherwise.
   *
   * Upon return of this function, this AttributeValue instance contains
   * the deserialized value.
   * In most cases, this method will not make any use of the checker argument.
   * However, in a very limited set of cases, the checker argument is needed to
   * perform proper serialization. A nice example of code which needs it is
   * the EnumValue::SerializeToString code.
   */
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker) = 0;
private:
  friend class Attribute;
  uint32_t m_count;
};

/**
 * \brief an opaque wrapper around a value to set or retrieved
 *        from an attribute.
 *
 * This class is really a smart pointer to an instance of AttributeValue.
 * Of course, the question is "why not use a Ptr<AttributeValue>" ?. The 
 * answer is long and complicated but the crux of the issue is that if we
 * do not reproduce the smart pointer code in this class, we cannot provide
 * transparent handling of Ptr<T> values through the attribute system.
 */
class Attribute
{
public:
  Attribute ();
  Attribute (const Attribute &o);
  Attribute &operator = (const Attribute &o);
  ~Attribute ();

  /**
   * Forward to AttributeValue::Copy
   */
  Attribute Copy (void) const;
  /**
   * Forward to AttributeValue::SerializeToString
   */
  std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  /**
   * Forward to AttributeValue::DeserializeFromString
   */
  bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

  /**
   * \returns a new Attribute object which wraps an instance of the requested
   * subclass of AttributeValue.
   */
  template <typename T>
  static Attribute Create (void);
  /**
   * \param a1 a value to pass through to the constructor of the class T.
   * \returns a new Attribute object which wraps an instance of the requested
   * subclass of AttributeValue.
   */
  template <typename T, typename T1>
  static Attribute Create (T1 a1);

  /**
   * This method performs a dynamic_cast on the underlying AttributeValue.
   * This method is typically used to implement conversion operators
   * from the type Attribute. In most cases, these conversion operators
   * will be generated for you by the ATTRIBUTE_HELPER_* macros.
   * \returns the casted pointer.
   */
  template <typename T>
  T DynCast (void) const;

  /**
   * \param pointer a pointer to convert into an Attribute.
   */
  template <typename T>
  Attribute (Ptr<T> pointer);
  /**
   * \returns a pointer converted from this Attribute instance.
   */
  template <typename T>
  operator Ptr<T> ();

private:
  Attribute (AttributeValue *value);
  AttributeValue *m_value;
};

/**
 * \brief allow setting and getting the value of an attribute.
 *
 * The goal of this class is to hide from the user how an attribute
 * is actually set or get to or from a class instance. Implementations
 * of this base class are usually provided through the MakeAccessorHelper
 * template functions, hidden behind an ATTRIBUTE_HELPER_* macro.
 */
class AttributeAccessor
{
public:
  AttributeAccessor ();
  void Ref (void) const;
  void Unref (void) const;
  virtual ~AttributeAccessor ();

  /**
   * \param object the object instance to set the value in
   * \param value the value to set
   * \returns true if the value could be set successfully, false otherwise.
   *
   * This method expects that the caller has checked that the input value is
   * valid with AttributeChecker::Check.
   */
  virtual bool Set (ObjectBase * object, Attribute value) const = 0;
  /**
   * \param object the object instance to get the value from
   * \param attribute a pointer to where the value should be set.
   * \returns true if the value could be read successfully, and
   *          stored in the input value, false otherwise.
   *
   * This method expects that the caller has checked that the input value is
   * valid with AttributeChecker::Check.
   */
  virtual bool Get (const ObjectBase * object, Attribute attribute) const = 0;
private:
  mutable uint32_t m_count;
};

/**
 * \brief Represent the type of an attribute
 *
 * Each type of attribute has an associated unique AttributeChecker
 * subclass. The type of the subclass can be safely used by users
 * to infer the type of the associated attribute. i.e., we expect
 * binding authors to use the checker associated to an attribute
 * to detect the type of the associated attribute.
 *
 * Most subclasses of this base class are implemented by the 
 * ATTRIBUTE_HELPER_* macros.
 */
class AttributeChecker
{
public:
  AttributeChecker ();
  void Ref (void) const;
  void Unref (void) const;
  virtual ~AttributeChecker ();
  /**
   * \param value a pointer to the value to check
   * \returns true if the input value is both of the right type
   *          and if its value is within the requested range. Returns
   *          false otherwise.
   */
  virtual bool Check (Attribute value) const = 0;
  virtual std::string GetType (void) const = 0;
  virtual bool HasTypeConstraints (void) const = 0;
  virtual std::string GetTypeConstraints (void) const = 0;
  /**
   * \returns a new instance of an AttributeValue (wrapper in an Attribute 
   *          instance) which matches the type of the underlying attribute.
   *
   * This method is typically used to create a temporary variable prior
   * to calling Attribute::DeserializeFromString.
   */
  virtual Attribute Create (void) const = 0;
private:
  mutable uint32_t m_count;
};

template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (Ptr<U> T::*memberVariable);

template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (void (T::*setter) (Ptr<U>));
template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (Ptr<U> (T::*getter) (void) const);
template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (void (T::*setter) (Ptr<U>),
                 Ptr<U> (T::*getter) (void) const);
template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (Ptr<U> (T::*getter) (void) const,
                 void (T::*setter) (Ptr<U>));



class PtrChecker : public AttributeChecker {};

template <typename T>
Ptr<AttributeChecker> MakePtrChecker (void);



} // namespace ns3

namespace ns3 {

/********************************************************
 *   The class used to access the pointer stored in a
 *   PtrValue<T> AttributeValue instance.
 ********************************************************/

class PtrValueBase : public AttributeValue
{
public:
  virtual ObjectBase *PeekObjectBase (void) const = 0;
  virtual bool SetObjectBase (ObjectBase *object) = 0;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);
};

/********************************************************
 *        Store the content of a Ptr<T> in a AttributeValue
 ********************************************************/

namespace internal {

template <typename T>
class PtrValue : public PtrValueBase
{
public:
  PtrValue () 
    : m_pointer () {}
  PtrValue (Ptr<T> pointer) 
    : m_pointer (pointer) {}

  virtual ObjectBase *PeekObjectBase (void) const {
    return PeekPointer (m_pointer);
  }
  virtual bool SetObjectBase (ObjectBase *object) {
    T *ptr = dynamic_cast<T *> (object);
    if (ptr == 0)
      {
	return false;
      }
    m_pointer = ptr;
    return true;
  }
  virtual Attribute Copy (void) const {
    return Attribute::Create<PtrValue<T> > (*this);
  }
private:
  Ptr<T> m_pointer;
};

template <typename T>
class APtrChecker : public PtrChecker
{
  virtual bool Check (Attribute val) const {
    const PtrValueBase *value = val.DynCast<const PtrValueBase *> ();
    if (value == 0)
      {
	return false;
      }
    if (value->PeekObjectBase () == 0)
      {
	return true;
      }
    T *ptr = dynamic_cast<T*> (value->PeekObjectBase ());
    if (ptr == 0)
      {
	return false;
      }
    return true;
  }
  virtual std::string GetType (void) const {
    // XXX: we should be able to return better information
    return "Ptr<>";
  }
  virtual bool HasTypeConstraints (void) const {
    return false;
  }
  virtual std::string GetTypeConstraints (void) const {
    return "";
  }
  virtual Attribute Create (void) const {
    return Attribute::Create<PtrValue<T> > ();
  }
};

/********************************************************
 *              The Accessor associated to 
 *               PtrValue<T>
 ********************************************************/

template <typename T, typename U>
class PtrAccessor : public AttributeAccessor
{
public:
  virtual ~PtrAccessor () {}
  virtual bool Set (ObjectBase * object, Attribute val) const {
      T *obj = dynamic_cast<T *> (object);
      if (obj == 0)
        {
          return false;
        }
      const PtrValueBase *value = val.DynCast<const PtrValueBase *> ();
      if (value == 0)
        {
          return false;
        }
      Ptr<U> ptr = dynamic_cast<U*> (value->PeekObjectBase ());
      if (ptr == 0)
        {
          return false;
        }
      DoSet (obj, ptr);
      return true;
    }
  virtual bool Get (const ObjectBase * object, Attribute val) const {
      const T *obj = dynamic_cast<const T *> (object);
      if (obj == 0)
        {
          return false;
        }
      PtrValueBase *value = val.DynCast<PtrValueBase *> ();
      if (value == 0)
        {
          return false;
        }
      return value->SetObjectBase (PeekPointer (DoGet (obj)));
    }
private:
  virtual void DoSet (T *object, Ptr<U> value) const = 0;
  virtual Ptr<U> DoGet (const T *object) const = 0;
};

} // namespace internal

/********************************************************
 *        The implementation of the Attribute 
 *          class template methods.
 ********************************************************/

template <typename T>
Attribute 
Attribute::Create (void)
{
  return Attribute (new T ());
}
template <typename T, typename T1>
Attribute 
Attribute::Create (T1 a1)
{
  return Attribute (new T (a1));
}

template <typename T>
T
Attribute::DynCast (void) const
{
  return dynamic_cast<T> (m_value);
}

template <typename T>
Attribute::Attribute (Ptr<T> pointer)
  : m_value (new internal::PtrValue<T> (pointer))
{}
template <typename T>
Attribute::operator Ptr<T> ()
{
  PtrValueBase *value = DynCast<PtrValueBase *> ();
  if (value == 0)
    {
      return 0;
    }
  ObjectBase *objectBase = value->PeekObjectBase ();
  T *obj = dynamic_cast<T *> (objectBase);
  if (obj == 0)
    {
      return 0;
    }
  return obj;
}



template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (Ptr<U> T::*memberVariable)
{
  struct MemberVariable : public internal::PtrAccessor<T,U>
  {
    Ptr<U> T::*m_memberVariable;
    virtual void DoSet (T *object, Ptr<U> value) const {
      (object->*m_memberVariable) = value;
    }
    virtual Ptr<U> DoGet (const T *object) const {
      return object->*m_memberVariable;
    }
  } *spec = new MemberVariable ();
  spec->m_memberVariable = memberVariable;
  return Ptr<const AttributeAccessor> (spec, false);
}

template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (void (T::*setter) (Ptr<U>))
{
  struct MemberMethod : public internal::PtrAccessor<T,U>
  {
    void (T::*m_setter) (Ptr<U>);
    virtual void DoSet (T *object, Ptr<U> value) const {
      (object->*m_setter) (value);
    }
    virtual Ptr<U> DoGet (const T *object) const {
      return 0;
      //return (object->*m_getter) ();
    }
  } *spec = new MemberMethod ();
  spec->m_setter = setter;
  return Ptr<const AttributeAccessor> (spec, false);
}

template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (Ptr<U> (T::*getter) (void) const)
{
  struct MemberMethod : public internal::PtrAccessor<T,U>
  {
    Ptr<U> (T::*m_getter) (void) const;
    virtual void DoSet (T *object, Ptr<U> value) const {
      //(object->*m_setter) (value);
    }
    virtual Ptr<U> DoGet (const T *object) const {
      return (object->*m_getter) ();
    }
  } *spec = new MemberMethod ();
  spec->m_getter = getter;
  return Ptr<const AttributeAccessor> (spec, false);
}
template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (void (T::*setter) (Ptr<U>),
                 Ptr<U> (T::*getter) (void) const)
{
  return MakePtrAccessor (getter, setter);
}
template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (Ptr<U> (T::*getter) (void) const,
                 void (T::*setter) (Ptr<U>))
{
  struct MemberMethod : public internal::PtrAccessor<T,U>
  {
    void (T::*m_setter) (Ptr<U>);
    Ptr<U> (T::*m_getter) (void) const;
    virtual void DoSet (T *object, Ptr<U> value) const {
      (object->*m_setter) (value);
    }
    virtual Ptr<U> DoGet (const T *object) const {
      return (object->*m_getter) ();
    }
  } *spec = new MemberMethod ();
  spec->m_setter = setter;
  spec->m_getter = getter;
  return Ptr<const AttributeAccessor> (spec, false);
}



template <typename T>
Ptr<AttributeChecker>
MakePtrChecker (void)
{
  return Create<internal::APtrChecker<T> > ();
}

} // namespace ns3

#endif /* ATTRIBUTE_H */
