#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <stdint.h>
#include "ptr.h"
#include "object-base.h"

namespace ns3 {

class AttributeAccessor;
class AttributeChecker;
class Attribute;

class AttributeValue
{
public:
  AttributeValue ();
  AttributeValue (const AttributeValue &o);
  AttributeValue &operator = (const AttributeValue &o);
  virtual ~AttributeValue ();

  virtual Attribute Copy (void) const = 0;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const = 0;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker) = 0;
private:
  friend class Attribute;
  uint32_t m_count;
};

class Attribute
{
public:
  Attribute ();
  Attribute (const Attribute &o);
  Attribute &operator = (const Attribute &o);
  ~Attribute ();

  Attribute Copy (void) const;
  std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

  template <typename T>
  static Attribute Create (void);
  template <typename T, typename T1>
  static Attribute Create (T1 a1);

  template <typename T>
  T DynCast (void) const;

  template <typename T>
  Attribute (Ptr<T> pointer);
  template <typename T>
  operator Ptr<T> ();

private:
  Attribute (AttributeValue *value);
  AttributeValue *m_value;
};

class AttributeAccessor : public ObjectBase
{
public:
  AttributeAccessor ();
  void Ref (void) const;
  void Unref (void) const;
  virtual ~AttributeAccessor ();

  /**
   * \param object the object instance to set the value in
   * \param value the value to set
   * \returns true if the value is valid and it could be set
   *          successfully, false otherwise.
   */
  virtual bool Set (ObjectBase * object, Attribute value) const = 0;
  virtual bool Get (const ObjectBase * object, Attribute value) const = 0;
private:
  mutable uint32_t m_count;
};

class AttributeChecker : public ObjectBase
{
public:
  AttributeChecker ();
  void Ref (void) const;
  void Unref (void) const;
  virtual ~AttributeChecker ();
  virtual bool Check (Attribute value) const = 0;
private:
  mutable uint32_t m_count;
};

template <typename T, typename BASE>
Ptr<AttributeChecker>
MakeSimpleAttributeChecker (void);

template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (Ptr<U> T::*memberVariable);

template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (void (T::*setter) (Ptr<U>));
template <typename T, typename U>
Ptr<const AttributeAccessor>
MakePtrAccessor (Ptr<U> (T::*getter) (void) const);


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
    virtual MemberVariable *Copy (void) const {
      return new MemberVariable (*this);
    }
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
    virtual MemberMethod *Copy (void) const {
      return new MemberMethod (*this);
    }
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
    void (T::*m_getter) (Ptr<U>);
    virtual MemberMethod *Copy (void) const {
      return new MemberMethod (*this);
    }
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


template <typename T>
Ptr<AttributeChecker>
MakePtrChecker (void)
{
  return Create<internal::APtrChecker<T> > ();
}

template <typename T, typename BASE>
Ptr<AttributeChecker>
MakeSimpleAttributeChecker (void)
{
  struct SimpleAttributeChecker : public BASE
  {
    virtual bool Check (Attribute value) const {
      return value.DynCast<const T *> () != 0;
    }
  } *checker = new SimpleAttributeChecker ();
  return Ptr<AttributeChecker> (checker, false);
}


} // namespace ns3

#endif /* ATTRIBUTE_H */
