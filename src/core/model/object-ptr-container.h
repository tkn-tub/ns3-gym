/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA, Mathieu Lacage
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
 * Authors: Mathieu Lacage <mathieu.lacage@gmail.com>
 */
#ifndef OBJECT_PTR_CONTAINER_H
#define OBJECT_PTR_CONTAINER_H

#include <vector>
#include "object.h"
#include "ptr.h"
#include "attribute.h"

namespace ns3 {

/**
 * \ingroup object
 * 
 * \brief contain a vector of ns3::Object pointers.
 *
 * This class it used to get attribute access to an array of
 * ns3::Object pointers.
 */
class ObjectPtrContainerValue : public AttributeValue
{
public:
  typedef std::vector<Ptr<Object> >::const_iterator Iterator;

  ObjectPtrContainerValue ();

  /**
   * \returns an iterator to the first object contained in this vector
   */
  Iterator Begin (void) const;
  /**
   * \returns an iterator to the last object contained in this vector
   */
  Iterator End (void) const;
  /**
   * \returns the number of objects contained in this vector.
   */
  uint32_t GetN (void) const;
  /**
   * \param i the index of the requested object.
   * \returns the requested object
   */
  Ptr<Object> Get (uint32_t i) const;

  virtual Ptr<AttributeValue> Copy (void) const;
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);

private:
  friend class ObjectPtrContainerAccessor;
  std::vector<Ptr<Object> > m_objects;
};


template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectPtrContainerAccessor (Ptr<U> (T::*get)(INDEX) const,
				INDEX (T::*getN)(void) const);

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectPtrContainerAccessor (INDEX (T::*getN)(void) const,
				Ptr<U> (T::*get)(INDEX) const);

class ObjectPtrContainerChecker : public AttributeChecker
{
public:
  virtual TypeId GetItemTypeId (void) const = 0;
};

template <typename T>
Ptr<const AttributeChecker> MakeObjectPtrContainerChecker (void);

} // namespace ns3

namespace ns3 {

namespace internal {

template <typename T>
class AnObjectPtrContainerChecker : public ObjectPtrContainerChecker
{
public:
  virtual TypeId GetItemTypeId (void) const {
    return T::GetTypeId ();
  }
  virtual bool Check (const AttributeValue &value) const {
    return dynamic_cast<const ObjectPtrContainerValue *> (&value) != 0;
  }
  virtual std::string GetValueTypeName (void) const {
    return "ns3::ObjectPtrContainerValue";
  }
  virtual bool HasUnderlyingTypeInformation (void) const {
    return true;
  }
  virtual std::string GetUnderlyingTypeInformation (void) const {
    return "ns3::Ptr< " + T::GetTypeId ().GetName () + " >";
  }
  virtual Ptr<AttributeValue> Create (void) const {
    return ns3::Create<ObjectPtrContainerValue> ();
  }
  virtual bool Copy (const AttributeValue &source, AttributeValue &destination) const {
    const ObjectPtrContainerValue *src = dynamic_cast<const ObjectPtrContainerValue *> (&source);
    ObjectPtrContainerValue *dst = dynamic_cast<ObjectPtrContainerValue *> (&destination);
    if (src == 0 || dst == 0)
      {
        return false;
      }
    *dst = *src;
    return true;
  }
};

} // namespace internal


class ObjectPtrContainerAccessor : public AttributeAccessor
{
public:
  virtual bool Set (ObjectBase * object, const AttributeValue &value) const;
  virtual bool Get (const ObjectBase * object, AttributeValue &value) const;
  virtual bool HasGetter (void) const;
  virtual bool HasSetter (void) const;
private:
  virtual bool DoGetN (const ObjectBase *object, uint32_t *n) const = 0;
  virtual Ptr<Object> DoGet (const ObjectBase *object, uint32_t i) const = 0;
};

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectPtrContainerAccessor (Ptr<U> (T::*get)(INDEX) const,
				INDEX (T::*getN)(void) const)
{
  struct MemberGetters : public ObjectPtrContainerAccessor
  {
    virtual bool DoGetN (const ObjectBase *object, uint32_t *n) const {
      const T *obj = dynamic_cast<const T *> (object);
      if (obj == 0)
        {
          return false;
        }
      *n = (obj->*m_getN)();
      return true;
    }
    virtual Ptr<Object> DoGet (const ObjectBase *object, uint32_t i) const {
      const T *obj = static_cast<const T *> (object);
      return (obj->*m_get)(i);
    }
    Ptr<U> (T::*m_get)(INDEX) const;
    INDEX (T::*m_getN)(void) const;
  } *spec = new MemberGetters ();
  spec->m_get = get;
  spec->m_getN = getN;
  return Ptr<const AttributeAccessor> (spec, false);
}

template <typename T, typename U, typename INDEX>
Ptr<const AttributeAccessor>
MakeObjectPtrContainerAccessor (INDEX (T::*getN)(void) const,
				Ptr<U> (T::*get)(INDEX) const)
{
  return MakeObjectPtrContainerAccessor (get, getN);
}

template <typename T>
Ptr<const AttributeChecker> MakeObjectPtrContainerChecker (void)
{
  return Create<internal::AnObjectPtrContainerChecker<T> > ();
}


} // namespace ns3

#endif /* OBJECT_PTR_CONTAINER_H */
