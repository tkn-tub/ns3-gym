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
#ifndef ATTRIBUTE_ACCESSOR_HELPER_H
#define ATTRIBUTE_ACCESSOR_HELPER_H

#include "attribute.h"

namespace ns3 {

/**
 * \ingroup AttributeHelper
 */
template <typename V, typename T1>
inline
Ptr<const AttributeAccessor>
MakeAccessorHelper (T1 a1);

/**
 * \ingroup AttributeHelper
 */
template <typename V, typename T1, typename T2>
inline
Ptr<const AttributeAccessor>
MakeAccessorHelper (T1 a1, T2 a2);

} // namespace ns3

/***************************************************************
 *        The implementation of the above functions.
 ***************************************************************/

#include "type-traits.h"

namespace ns3 {

/**
 * \ingroup AttributeHelper
 */
template <typename T>
struct AccessorTrait
{
  typedef typename TypeTraits<typename TypeTraits<T>::ReferencedType>::NonConstType Result;
};

/**
 * \ingroup AttributeHelper
 */
template <typename T, typename U>
class AccessorHelper : public AttributeAccessor
{
public:
  AccessorHelper () {}

  virtual bool Set (ObjectBase * object, const AttributeValue & val) const {
    const U *value = dynamic_cast<const U *> (&val);
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

  virtual bool Get (const ObjectBase * object, AttributeValue &val) const {
    U *value = dynamic_cast<U *> (&val);
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

template <typename V, typename T, typename U>
inline
Ptr<const AttributeAccessor>
DoMakeAccessorHelperOne (U T::*memberVariable)
{
  class MemberVariable : public AccessorHelper<T,V>
  {
public:
    MemberVariable (U T::*memberVariable)
      : AccessorHelper<T,V> (),
        m_memberVariable (memberVariable)
    {}
private:
    virtual bool DoSet (T *object, const V *v) const {
      typename AccessorTrait<U>::Result tmp;
      bool ok = v->GetAccessor (tmp);
      if (!ok)
        {
          return false;
        }
      (object->*m_memberVariable) = tmp;
      return true;
    }
    virtual bool DoGet (const T *object, V *v) const {
      v->Set (object->*m_memberVariable);
      return true;
    }
    virtual bool HasGetter (void) const {
      return true;
    }
    virtual bool HasSetter (void) const {
      return true;
    }

    U T::*m_memberVariable;
  };
  return Ptr<const AttributeAccessor> (new MemberVariable (memberVariable), false);
}

template <typename V, typename T, typename U>
inline
Ptr<const AttributeAccessor>
DoMakeAccessorHelperOne (U (T::*getter)(void) const)
{
  class MemberMethod : public AccessorHelper<T,V>
  {
public:
    MemberMethod (U (T::*getter)(void) const)
      : AccessorHelper<T,V> (),
        m_getter (getter)
    {}
private:
    virtual bool DoSet (T *object, const V *v) const {
      return false;
    }
    virtual bool DoGet (const T *object, V *v) const {
      v->Set ((object->*m_getter)());
      return true;
    }
    virtual bool HasGetter (void) const {
      return true;
    }
    virtual bool HasSetter (void) const {
      return false;
    }
    U (T::*m_getter)(void) const;
  };
  return Ptr<const AttributeAccessor> (new MemberMethod (getter), false);
}


template <typename V, typename T, typename U>
inline
Ptr<const AttributeAccessor>
DoMakeAccessorHelperOne (void (T::*setter)(U))
{
  class MemberMethod : public AccessorHelper<T,V>
  {
public:
    MemberMethod (void (T::*setter)(U))
      : AccessorHelper<T,V> (),
        m_setter (setter)
    {}
private:
    virtual bool DoSet (T *object, const V *v) const {
      typename AccessorTrait<U>::Result tmp;
      bool ok = v->GetAccessor (tmp);
      if (!ok)
        {
          return false;
        }
      (object->*m_setter)(tmp);
      return true;
    }
    virtual bool DoGet (const T *object, V *v) const {
      return false;
    }
    virtual bool HasGetter (void) const {
      return false;
    }
    virtual bool HasSetter (void) const {
      return true;
    }
    void (T::*m_setter)(U);
  };
  return Ptr<const AttributeAccessor> (new MemberMethod (setter), false);
}

template <typename W, typename T, typename U, typename V>
inline
Ptr<const AttributeAccessor>
DoMakeAccessorHelperTwo (void (T::*setter)(U),
                         V (T::*getter)(void) const)
{
  class MemberMethod : public AccessorHelper<T,W>
  {
public:
    MemberMethod (void (T::*setter)(U),
                  V (T::*getter)(void) const)
      : AccessorHelper<T,W> (),
        m_setter (setter),
        m_getter (getter)
    {}
private:
    virtual bool DoSet (T *object, const W *v) const {
      typename AccessorTrait<U>::Result tmp;
      bool ok = v->GetAccessor (tmp);
      if (!ok)
        {
          return false;
        }
      (object->*m_setter)(tmp);
      return true;
    }
    virtual bool DoGet (const T *object, W *v) const {
      v->Set ((object->*m_getter)());
      return true;
    }
    virtual bool HasGetter (void) const {
      return true;
    }
    virtual bool HasSetter (void) const {
      return true;
    }
    void (T::*m_setter)(U);
    V (T::*m_getter)(void) const;
  };
  return Ptr<const AttributeAccessor> (new MemberMethod (setter, getter), false);
}

template <typename W, typename T, typename U, typename V>
inline
Ptr<const AttributeAccessor>
DoMakeAccessorHelperTwo (V (T::*getter)(void) const,
                         void (T::*setter)(U))
{
  return DoMakeAccessorHelperTwo<W> (setter, getter);
}

template <typename W, typename T, typename U, typename V>
inline
Ptr<const AttributeAccessor>
DoMakeAccessorHelperTwo (bool (T::*setter)(U),
                         V (T::*getter)(void) const)
{
  class MemberMethod : public AccessorHelper<T,W>
  {
public:
    MemberMethod (bool (T::*setter)(U),
                  V (T::*getter)(void) const)
      : AccessorHelper<T,W> (),
        m_setter (setter),
        m_getter (getter)
    {}
private:
    virtual bool DoSet (T *object, const W *v) const {
      typename AccessorTrait<U>::Result tmp;
      bool ok = v->GetAccessor (tmp);
      if (!ok)
        {
          return false;
        }
      ok = (object->*m_setter)(tmp);
      return ok;
    }
    virtual bool DoGet (const T *object, W *v) const {
      v->Set ((object->*m_getter)());
      return true;
    }
    virtual bool HasGetter (void) const {
      return true;
    }
    virtual bool HasSetter (void) const {
      return true;
    }
    bool (T::*m_setter)(U);
    V (T::*m_getter)(void) const;
  };
  return Ptr<const AttributeAccessor> (new MemberMethod (setter, getter), false);
}

template <typename W, typename T, typename U, typename V>
inline
Ptr<const AttributeAccessor>
DoMakeAccessorHelperTwo (bool (T::*getter)(void) const,
                         void (T::*setter)(U))
{
  return DoMakeAccessorHelperTwo<W> (setter, getter);
}

template <typename V, typename T1>
inline
Ptr<const AttributeAccessor>
MakeAccessorHelper (T1 a1)
{
  return DoMakeAccessorHelperOne<V> (a1);
}

template <typename V, typename T1, typename T2>
inline
Ptr<const AttributeAccessor>
MakeAccessorHelper (T1 a1, T2 a2)
{
  return DoMakeAccessorHelperTwo<V> (a1, a2);
}

} // namespace ns3

#endif /* ATTRIBUTE_ACCESSOR_HELPER_H */
