#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <stdint.h>
#include "ptr.h"
#include "object-base.h"

namespace ns3 {

class ParamSpec;
class PValue;

class Value
{
public:
  Value ();
  Value (const Value &o);
  Value &operator = (const Value &o);
  virtual ~Value ();

  virtual PValue Copy (void) const = 0;
  virtual std::string SerializeToString (Ptr<const ParamSpec> spec) const = 0;
  virtual bool DeserializeFromString (std::string value, Ptr<const ParamSpec> spec) = 0;
private:
  friend class PValue;
  uint32_t m_count;
};

class PValue
{
public:
  PValue ();
  PValue (const PValue &o);
  PValue &operator = (const PValue &o);
  ~PValue ();

  PValue Copy (void) const;
  std::string SerializeToString (Ptr<const ParamSpec> spec) const;
  bool DeserializeFromString (std::string value, Ptr<const ParamSpec> spec);

  template <typename T>
  static PValue Create (void);
  template <typename T, typename T1>
  static PValue Create (T1 a1);

  template <typename T>
  T DynCast (void) const;

  template <typename T>
  PValue (Ptr<T> pointer);
  template <typename T>
  operator Ptr<T> ();

private:
  PValue (Value *value);
  Value *m_value;
};

class ParamSpec : public ObjectBase
{
public:
  ParamSpec ();
  void Ref (void) const;
  void Unref (void) const;
  virtual ~ParamSpec ();

  /**
   * \param object the object instance to set the value in
   * \param value the value to set
   * \returns true if the value is valid and it could be set
   *          successfully, false otherwise.
   */
  virtual bool Set (ObjectBase * object, PValue value) const = 0;
  virtual bool Get (const ObjectBase * object, PValue value) const = 0;
  virtual bool Check (PValue value) const = 0;
  virtual PValue GetInitialValue (void) const = 0;
  virtual PValue CreateValue (void) const = 0;
private:
  mutable uint32_t m_count;
};


template <typename T, typename U>
Ptr<const ParamSpec>
MakePtrParamSpec (Ptr<U> T::*memberVariable);

template <typename T, typename U>
Ptr<const ParamSpec>
MakePtrParamSpec (void (T::*setter) (Ptr<U>));
template <typename T, typename U>
Ptr<const ParamSpec>
MakePtrParamSpec (Ptr<U> (T::*getter) (void) const);


} // namespace ns3

namespace ns3 {

/********************************************************
 *   The class used to access the pointer stored in a
 *   PtrValue<T> Value instance.
 ********************************************************/

class PtrValueBase : public Value
{
public:
  virtual ObjectBase *PeekObjectBase (void) const = 0;
  virtual bool SetObjectBase (ObjectBase *object) = 0;
  virtual std::string SerializeToString (Ptr<const ParamSpec> spec) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const ParamSpec> spec);
};

/********************************************************
 *        Store the content of a Ptr<T> in a Value
 ********************************************************/

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
  virtual PValue Copy (void) const {
    return PValue::Create<PtrValue<T> > (*this);
  }
private:
  Ptr<T> m_pointer;
};

/********************************************************
 *        The implementation of the PValue 
 *          class template methods.
 ********************************************************/


template <typename T>
PValue 
PValue::Create (void)
{
  return PValue (new T ());
}
template <typename T, typename T1>
PValue 
PValue::Create (T1 a1)
{
  return PValue (new T (a1));
}

template <typename T>
T
PValue::DynCast (void) const
{
  return dynamic_cast<T> (m_value);
}

template <typename T>
PValue::PValue (Ptr<T> pointer)
  : m_value (new PtrValue<T> (pointer))
{}
template <typename T>
PValue::operator Ptr<T> ()
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

/********************************************************
 *              The ParamSpec associated to 
 *               PtrValue<T>
 ********************************************************/

template <typename T, typename U>
class PtrParamSpec : public ParamSpec
{
public:
  virtual ~PtrParamSpec () {}
  virtual bool Set (ObjectBase * object, PValue val) const {
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
    virtual bool Get (const ObjectBase * object, PValue val) const {
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
    virtual bool Check (PValue val) const {
      const PtrValueBase *value = val.DynCast<const PtrValueBase *> ();
      if (value == 0)
        {
          return false;
        }
      if (value->PeekObjectBase () == 0)
        {
          return true;
        }
      U *ptr = dynamic_cast<U*> (value->PeekObjectBase ());
      if (ptr == 0)
        {
          return false;
        }
      return true;
    }
    virtual PValue GetInitialValue (void) const {
      return CreateValue ();
    }
    virtual PValue CreateValue (void) const {
      return PValue::Create<PtrValue<U> > (Ptr<U> (0));
    }
private:
  virtual void DoSet (T *object, Ptr<U> value) const = 0;
  virtual Ptr<U> DoGet (const T *object) const = 0;
};


template <typename T, typename U>
Ptr<const ParamSpec>
MakePtrParamSpec (Ptr<U> T::*memberVariable)
{
  struct MemberVariable : public PtrParamSpec<T,U>
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
  return Ptr<const ParamSpec> (spec, false);
}

template <typename T, typename U>
Ptr<const ParamSpec>
MakePtrParamSpec (void (T::*setter) (Ptr<U>))
{
  struct MemberMethod : public PtrParamSpec<T,U>
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
  return Ptr<const ParamSpec> (spec, false);
}

template <typename T, typename U>
Ptr<const ParamSpec>
MakePtrParamSpec (Ptr<U> (T::*getter) (void) const)
{
  struct MemberMethod : public PtrParamSpec<T,U>
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
  return Ptr<const ParamSpec> (spec, false);
}

} // namespace ns3

#endif /* VALUE_H */
