#ifndef PARAM_SPEC_HELPER_H
#define PARAM_SPEC_HELPER_H

namespace ns3 {

template <typename BASE, typename V, typename T1>
Ptr<ParamSpec>
MakeParamSpecHelper (T1 a1);

template <typename BASE, typename V, typename T1, typename T2>
Ptr<ParamSpec>
MakeParamSpecHelper (T1 a1, T2 a2);

template <typename BASE, typename V, typename T1, typename CHECKER>
Ptr<ParamSpec>
MakeParamSpecHelperWithChecker (T1 a1, CHECKER checker);

template <typename BASE, typename V, typename T1, typename T2, typename CHECKER>
Ptr<ParamSpec>
MakeParamSpecHelperWithChecker (T1 a1, T2 a2, CHECKER checker);

} // namespace ns3

/***************************************************************
 *        The implementation of the above functions.
 ***************************************************************/

#include "type-traits.h"

namespace ns3 {

template <typename BASE, typename T, typename U, typename CHECKER>
class ParamSpecHelper : public BASE
{
public:
  ParamSpecHelper (CHECKER checker)
    : m_checker (checker) {}

  virtual bool Set (ObjectBase * object, PValue val) const {
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

  virtual bool Get (const ObjectBase * object, PValue val) const {
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

  virtual bool Check (PValue value) const {
    const U *val = value.DynCast<const U*> ();
    if (val == 0)
      {
	return false;
      }
    return m_checker.Check (val->Get ());
  }


private:
  virtual bool DoSet (T *object, const U *v) const = 0;
  virtual bool DoGet (const T *object, U *v) const = 0;
  CHECKER m_checker;
};

template <typename T>
class ParamSpecHelperSimpleChecker
{
public:
  bool Check (const T &value) const {return true;}
};

template <typename BASE, typename V, typename T, typename U, typename CHECKER>
Ptr<ParamSpec>
DoMakeParamSpecHelperOne (U T::*memberVariable, CHECKER checker)
{
  class MemberVariable : public ParamSpecHelper<BASE,T,V,CHECKER>
    {
    public:
      MemberVariable (U T::*memberVariable, CHECKER checker)
	: ParamSpecHelper<BASE,T,V,CHECKER> (checker),
	m_memberVariable (memberVariable)
	{}
    private:
      virtual bool DoSet (T *object, const V *v) const {
	(object->*m_memberVariable) = v->Get ();
	return true;
      }
      virtual bool DoGet (const T *object, V *v) const {
	v->Set (object->*m_memberVariable);
	return true;
      }
      
      U T::*m_memberVariable;
    };
  return Ptr<ParamSpec> (new MemberVariable (memberVariable, checker), false);
}
template <typename BASE, typename V, typename T, typename U>
Ptr<ParamSpec>
DoMakeParamSpecHelperOne (U T::*memberVariable)
{
  return DoMakeParamSpecHelperOne<BASE,V> (memberVariable, ParamSpecHelperSimpleChecker<U> ());
}


template <typename BASE, typename V, typename T, typename U, typename CHECKER>
Ptr<ParamSpec>
DoMakeParamSpecHelperOne (U (T::*getter) (void) const, 
			  CHECKER checker)
{
  class MemberMethod : public ParamSpecHelper<BASE,T,V,CHECKER>
    {
    public:
      MemberMethod (U (T::*getter) (void) const,
		    CHECKER checker)
	: ParamSpecHelper<BASE,T,V,CHECKER> (checker),
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
  return Ptr<ParamSpec> (new MemberMethod (getter, checker), false);
}

template <typename BASE, typename V, typename T, typename U>
Ptr<ParamSpec>
DoMakeParamSpecHelperOne (U (T::*getter) (void) const)
{
  return DoMakeParamSpecHelperOne<BASE,V> (getter, ParamSpecHelperSimpleChecker<U> ());
}



template <typename BASE, typename V, typename T, typename U, typename CHECKER>
Ptr<ParamSpec>
DoMakeParamSpecHelperOne (void (T::*setter) (U), 
			  CHECKER checker)
{
  class MemberMethod : public ParamSpecHelper<BASE,T,V,CHECKER>
    {
    public:
      MemberMethod (void (T::*setter) (U), 
		    CHECKER checker)
	: ParamSpecHelper<BASE,T,V,CHECKER> (checker),
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
  return Ptr<ParamSpec> (new MemberMethod (setter, checker), false);
}

template <typename BASE, typename V, typename T, typename U>
Ptr<ParamSpec>
DoMakeParamSpecHelperOne (void (T::*setter) (U))
{
  return DoMakeParamSpecHelperOne<BASE,V> (setter,
					 ParamSpecHelperSimpleChecker<typename TypeTraits<U>::ReferencedType> ());
}


template <typename BASE, typename W, typename T, typename U, typename V, typename CHECKER>
Ptr<ParamSpec>
DoMakeParamSpecHelperTwo (void (T::*setter) (U), 
			  V (T::*getter) (void) const, 
			  CHECKER checker = ParamSpecHelperSimpleChecker<V> ())
{
  class MemberMethod : public ParamSpecHelper<BASE,T,W,CHECKER>
    {
    public:
      MemberMethod (void (T::*setter) (U), 
		    V (T::*getter) (void) const,
		    CHECKER checker)
	: ParamSpecHelper<BASE,T,W,CHECKER> (checker),
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
  return Ptr<ParamSpec> (new MemberMethod (setter, getter, checker), false);
}

template <typename BASE, typename W, typename T, typename U, typename V>
Ptr<ParamSpec>
DoMakeParamSpecHelperTwo (void (T::*setter) (U), 
		      V (T::*getter) (void) const)
{
  return DoMakeParamSpecHelperTwo<BASE,W> (setter, getter, ParamSpecHelperSimpleChecker<V> ());
}

template <typename BASE, typename W, typename T, typename U, typename V, typename CHECKER>
Ptr<ParamSpec>
DoMakeParamSpecHelperTwo (V (T::*getter) (void) const, 
			  void (T::*setter) (U), 
			  CHECKER checker)
{
  return DoMakeParamSpecHelperTwo<BASE,W> (setter, getter, checker);
}

template <typename BASE, typename W, typename T, typename U, typename V, typename CHECKER>
Ptr<ParamSpec>
DoMakeParamSpecHelperTwo (V (T::*getter) (void) const, 
			  void (T::*setter) (U))
{
  return DoMakeParamSpecHelperTwo<BASE,W> (setter, getter);
}

template <typename BASE, typename V, typename T1>
Ptr<ParamSpec>
MakeParamSpecHelper (T1 a1)
{
  return DoMakeParamSpecHelperOne<BASE,V> (a1);
}

template <typename BASE, typename V, typename T1, typename T2>
Ptr<ParamSpec>
MakeParamSpecHelper (T1 a1, T2 a2)
{
  return DoMakeParamSpecHelperTwo<BASE,V> (a1, a2);
}

template <typename BASE, typename V, typename T1, typename CHECKER>
Ptr<ParamSpec>
MakeParamSpecHelperWithChecker (T1 a1, CHECKER checker)
{
  return DoMakeParamSpecHelperOne<BASE,V> (a1, checker);
}

template <typename BASE, typename V, typename T1, typename T2, typename CHECKER>
Ptr<ParamSpec>
MakeParamSpecHelperWithChecker (T1 a1, T2 a2, CHECKER checker)
{
  return DoMakeParamSpecHelperTwo<BASE,V> (a1, a2, checker);
}

} // namespace ns3

#endif /* PARAM_SPEC_HELPER_H */
