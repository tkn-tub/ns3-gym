#ifndef PARAM_SPEC_HELPER_H
#define PARAM_SPEC_HELPER_H

namespace ns3 {

template <typename BASE, typename V, typename T1>
Ptr<const AttributeAccessor>
MakeAccessorHelper (T1 a1);

template <typename BASE, typename V, typename T1, typename T2>
Ptr<const AttributeAccessor>
MakeAccessorHelper (T1 a1, T2 a2);

} // namespace ns3

/***************************************************************
 *        The implementation of the above functions.
 ***************************************************************/

#include "type-traits.h"

namespace ns3 {

template <typename BASE, typename T, typename U>
class AccessorHelper : public BASE
{
public:
  AccessorHelper () {}

  virtual bool Set (ObjectBase * object, Attribute val) const {
    const U *value = val.DynCast<const U*> ();
    if (value == 0)
      {
	return false;
      }
    T *obj = dynamic_cast<T *> (object);
    if (obj == 0)
      {
	return false;
      }
    return DoSet (obj, value);
  }

  virtual bool Get (const ObjectBase * object, Attribute val) const {
    U *value = val.DynCast<U*> ();
    if (value == 0)
      {
	return false;
      }
    const T *obj = dynamic_cast<const T *> (object);
    if (obj == 0)
      {
	return false;
      }
    return DoGet (obj, value);
  }


private:
  virtual bool DoSet (T *object, const U *v) const = 0;
  virtual bool DoGet (const T *object, U *v) const = 0;
};

template <typename BASE, typename V, typename T, typename U>
Ptr<const AttributeAccessor>
DoMakeAccessorHelperOne (U T::*memberVariable)
{
  class MemberVariable : public AccessorHelper<BASE,T,V>
    {
    public:
      MemberVariable (U T::*memberVariable)
	: AccessorHelper<BASE,T,V> (),
	m_memberVariable (memberVariable)
	{}
    private:
      virtual bool DoSet (T *object, const V *v) const {
	(object->*m_memberVariable) = U (v->Get ());
	return true;
      }
      virtual bool DoGet (const T *object, V *v) const {
	v->Set (object->*m_memberVariable);
	return true;
      }
      
      U T::*m_memberVariable;
    };
  return Ptr<const AttributeAccessor> (new MemberVariable (memberVariable), false);
}

template <typename BASE, typename V, typename T, typename U>
Ptr<const AttributeAccessor>
DoMakeAccessorHelperOne (U (T::*getter) (void) const)
{
  class MemberMethod : public AccessorHelper<BASE,T,V>
    {
    public:
      MemberMethod (U (T::*getter) (void) const)
	: AccessorHelper<BASE,T,V> (),
	m_getter (getter)
	{}
    private:
      virtual bool DoSet (T *object, const V *v) const {
	return false;
      }
      virtual bool DoGet (const T *object, V *v) const {
	v->Set ((object->*m_getter) ());
	return true;
      }
      U (T::*m_getter) (void) const;
    };
  return Ptr<const AttributeAccessor> (new MemberMethod (getter), false);
}


template <typename BASE, typename V, typename T, typename U>
Ptr<const AttributeAccessor>
DoMakeAccessorHelperOne (void (T::*setter) (U))
{
  class MemberMethod : public AccessorHelper<BASE,T,V>
    {
    public:
      MemberMethod (void (T::*setter) (U))
	: AccessorHelper<BASE,T,V> (),
	m_setter (setter)
	{}
    private:
      virtual bool DoSet (T *object, const V *v) const {
	(object->*m_setter) (v->Get ());
	return true;
      }
      virtual void DoGet (const T *object, V *v) const {
	return false;
      }
      void (T::*m_setter) (U);
    };
  return Ptr<const AttributeAccessor> (new MemberMethod (setter), false);
}

template <typename BASE, typename W, typename T, typename U, typename V>
Ptr<const AttributeAccessor>
DoMakeAccessorHelperTwo (void (T::*setter) (U), 
			  V (T::*getter) (void) const)
{
  class MemberMethod : public AccessorHelper<BASE,T,W>
    {
    public:
      MemberMethod (void (T::*setter) (U), 
		    V (T::*getter) (void) const)
	: AccessorHelper<BASE,T,W> (),
	m_setter (setter),
	m_getter (getter)
	{}
    private:
      virtual bool DoSet (T *object, const W *v) const {
	(object->*m_setter) (v->Get ());
	return true;
      }
      virtual bool DoGet (const T *object, W *v) const {
	v->Set ((object->*m_getter) ());
	return true;
      }
      void (T::*m_setter) (U);
      V (T::*m_getter) (void) const;
    };
  return Ptr<const AttributeAccessor> (new MemberMethod (setter, getter), false);
}

template <typename BASE, typename W, typename T, typename U, typename V>
Ptr<const AttributeAccessor>
DoMakeAccessorHelperTwo (V (T::*getter) (void) const, 
			  void (T::*setter) (U))
{
  return DoMakeAccessorHelperTwo<BASE,W> (setter, getter);
}

template <typename BASE, typename V, typename T1>
Ptr<const AttributeAccessor>
MakeAccessorHelper (T1 a1)
{
  return DoMakeAccessorHelperOne<BASE,V> (a1);
}

template <typename BASE, typename V, typename T1, typename T2>
Ptr<const AttributeAccessor>
MakeAccessorHelper (T1 a1, T2 a2)
{
  return DoMakeAccessorHelperTwo<BASE,V> (a1, a2);
}

} // namespace ns3

#endif /* PARAM_SPEC_HELPER_H */
