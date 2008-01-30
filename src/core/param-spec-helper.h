#ifndef PARAM_SPEC_HELPER_H
#define PARAM_SPEC_HELPER_H

namespace ns3 {

template <typename T, typename U, typename V>
class ParamSpecHelper : public ParamSpec
{
public:
  ParamSpecHelper (V initialValue);
  virtual bool Set (ObjectBase * object, Ptr<const Value> parameter) const;
  virtual bool Get (const ObjectBase * object, Ptr<Value> parameter) const;
  virtual bool Check (Ptr<const Value> parameter) const;
  virtual Ptr<Value> CreateInitialValue (void) const;

private:
  virtual void DoSet (T *object, const V &v) const = 0;
  virtual V DoGet (const T *object) const = 0;
  virtual bool DoCheck (const V &v) const;
  V m_initialValue;
};

template <typename T, typename U, typename V>
class MemberVariableParamSpecHelper : public ParamSpecHelper<T,U,V>
{
public:
  MemberVariableParamSpecHelper (V T::*memberVariable, V initialValue);
private:
  virtual void DoSet (T *object, const V &v) const;
  virtual V DoGet (const T *object) const;

  V T::*m_memberVariable;
};

template <typename T, typename U, typename V>
class MemberMethodParamSpecHelper : public ParamSpecHelper<T,U,V>
{
public:
  MemberMethodParamSpecHelper (void (T::*setter) (V),
			       V (T::*getter) (void) const,
			       V initialValue);
private:
  virtual void DoSet (T *object, const V &v) const;
  virtual V DoGet (const T *object) const;

  void (T::*m_setter) (V);
  V (T::*m_getter) (void) const;
};

template <typename T, typename U, typename V>
class MemberMethodSetterParamSpecHelper : public ParamSpecHelper<T,U,V>
{
public:
  MemberMethodSetterParamSpecHelper (void (T::*setter) (V),
				     V initialValue);
private:
  virtual void DoSet (T *object, const V &v) const;
  virtual V DoGet (const T *object) const;

  void (T::*m_setter) (V);
};

template <typename T, typename U, typename V>
class MemberMethodGetterParamSpecHelper : public ParamSpecHelper<T,U,V>
{
public:
  MemberMethodGetterParamSpecHelper (V (T::*getter) (void) const);
private:
  virtual void DoSet (T *object, const V &v) const;
  virtual V DoGet (const T *object) const;

  V (T::*m_getter) (void) const;
};

} // namespace ns3


/***************************************************************
 *        The implementation of the above class templates.
 ***************************************************************/

namespace ns3 {

template <typename T, typename U, typename V>
ParamSpecHelper<T,U,V>::ParamSpecHelper (V initialValue)
  : m_initialValue (initialValue)
{}
template <typename T, typename U, typename V>
bool 
ParamSpecHelper<T,U,V>::Set (ObjectBase * object, Ptr<const Value> parameter) const
{
  const U *value = dynamic_cast<const U*> (PeekPointer (parameter));
  if (value == 0)
    {
      return false;
    }
  T *obj = dynamic_cast<T *> (object);
  if (obj == 0)
    {
      return false;
    }
  V v = value->Get ();
  DoSet (obj, v);
  return true;
}
template <typename T, typename U, typename V>
bool 
ParamSpecHelper<T,U,V>::Get (const ObjectBase * object, Ptr<Value> parameter) const
{
  U *value = dynamic_cast<U*> (PeekPointer (parameter));
  if (value == 0)
    {
      return false;
    }
  const T *obj = dynamic_cast<const T *> (object);
  if (obj == 0)
    {
      return false;
    }
  V v = DoGet (obj);
  value->Set (v);
  return true;
}
template <typename T, typename U, typename V>
bool 
ParamSpecHelper<T,U,V>::Check (Ptr<const Value> parameter) const
{
  const U *value = dynamic_cast<const U*> (PeekPointer (parameter));
  if (value == 0)
    {
      return false;
    }
  V v = value->Get ();
  DoCheck (v);
  return true;
}
template <typename T, typename U, typename V>
bool
ParamSpecHelper<T,U,V>::DoCheck (const V &v) const
{
  return true;
}
template <typename T, typename U, typename V>
Ptr<Value>
ParamSpecHelper<T,U,V>::CreateInitialValue (void) const
{
  return Create<U> (m_initialValue);
}


template <typename T, typename U, typename V>
MemberVariableParamSpecHelper<T,U,V>::MemberVariableParamSpecHelper (V T::*memberVariable, V initialValue)
  : ParamSpecHelper<T,U,V> (initialValue),
  m_memberVariable (memberVariable)
{}
template <typename T, typename U, typename V>
void 
MemberVariableParamSpecHelper<T,U,V>::DoSet (T *object, const V &v) const
{
  (object->*m_memberVariable) = v;
}
template <typename T, typename U, typename V>
V 
MemberVariableParamSpecHelper<T,U,V>::DoGet (const T *object) const
{
  return (object->*m_memberVariable);
}

template <typename T, typename U, typename V>
MemberMethodParamSpecHelper<T,U,V>::MemberMethodParamSpecHelper (void (T::*setter) (V),
								 V (T::*getter) (void) const,
								 V initialValue)
  : ParamSpecHelper<T,U,V> (initialValue),
  m_setter (setter),
  m_getter (getter)
{}
template <typename T, typename U, typename V>
void 
MemberMethodParamSpecHelper<T,U,V>::DoSet (T *object, const V &v) const
{
  (object->*m_setter) (v);
}
template <typename T, typename U, typename V>
V 
MemberMethodParamSpecHelper<T,U,V>::DoGet (const T *object) const
{
  return (object->*m_getter) ();
}

template <typename T, typename U, typename V>
MemberMethodSetterParamSpecHelper<T,U,V>::MemberMethodSetterParamSpecHelper (void (T::*setter) (V),
									     V initialValue)
  : ParamSpecHelper<T,U,V> (initialValue),
  m_setter (setter)
{}
template <typename T, typename U, typename V>
void 
MemberMethodSetterParamSpecHelper<T,U,V>::DoSet (T *object, const V &v) const
{
  (object->m_setter) (v);
}
template <typename T, typename U, typename V>
V 
MemberMethodSetterParamSpecHelper<T,U,V>::DoGet (const T *object) const
{
  return V ();
}

template <typename T, typename U, typename V>
MemberMethodGetterParamSpecHelper<T,U,V>::MemberMethodGetterParamSpecHelper (V (T::*getter) (void) const)
  : m_getter (getter)
{}
template <typename T, typename U, typename V>
void 
MemberMethodGetterParamSpecHelper<T,U,V>::DoSet (T *object, const V &v) const
{}
template <typename T, typename U, typename V>
V 
MemberMethodGetterParamSpecHelper<T,U,V>::DoGet (const T *object) const
{
  return (object->*m_getter) ();
}


} // namespace ns3

#endif /* PARAM_SPEC_HELPER_H */
