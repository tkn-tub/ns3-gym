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
#include "attribute.h"
#include "object-base.h"
#include "attribute-list.h"


namespace ns3 {

class Object;
class AttributeAccessor;
class AttributeValue;
class AttributeList;
class TraceSourceAccessor;

/**
 * \brief a base class which provides memory management and object aggregation
 *
 */
class Object : public ObjectBase
{
public:
  static TypeId GetTypeId (void);

  /**
   * \brief Iterate over the objects aggregated to an ns3::Object.
   *
   * This iterator does not allow you to iterate over the initial
   * object used to call Object::GetAggregateIterator. 
   *
   * Note: this is a java-style iterator.
   */
  class AggregateIterator
  {
  public:
    AggregateIterator ();

    /**
     * \returns true if HasNext can be called and return a non-null
     *          pointer, false otherwise.
     */
    bool HasNext (void) const;

    /**
     * \returns the next aggregated object.
     */
    Ptr<const Object> Next (void);
  private:
    friend class Object;
    AggregateIterator (Ptr<const Object> first);
    Ptr<const Object> m_first;
    Ptr<const Object> m_current;
  };

  Object ();
  virtual ~Object ();

  /*
   * Implement the GetInstanceTypeId method defined in ObjectBase.
   */
  virtual TypeId GetInstanceTypeId (void) const;

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
   * instance.
   *
   * This method is typically used to break reference cycles.
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
   * \returns an iterator to the first object aggregated to this
   *          object.
   *
   * If no objects are aggregated to this object, then, the returned
   * iterator will be empty and AggregateIterator::HasNext will
   * always return false.
   */
  AggregateIterator GetAggregateIterator (void) const;

protected:
  /**
   * This method is called by Object::Dispose or by the object's 
   * destructor, whichever comes first.
   *
   * Subclasses are expected to implement their real destruction
   * code in an overriden version of this method and chain
   * up to their parent's implementation once they are done.
   * i.e., for simplicity, the destructor of every subclass should
   * be empty and its content should be moved to the associated
   * DoDispose method.
   */
  virtual void DoDispose (void);
  /**
   * \param o the object to copy.
   *
   * Allow subclasses to implement a copy constructor.
   * While it is technically possible to implement a copy
   * constructor in a subclass, we strongly discourage you
   * to do so. If you really want to do it anyway, you have
   * to understand that this copy constructor will _not_
   * copy aggregated objects. i.e., if your object instance
   * is already aggregated to another object and if you invoke
   * this copy constructor, the new object instance will be
   * a pristine standlone object instance not aggregated to
   * any other object. It is thus _your_ responsability
   * as a caller of this method to do what needs to be done
   * (if it is needed) to ensure that the object stays in a
   * valid state.
   */
  Object (const Object &o);
private:

  template <typename T>
  friend Ptr<T> CreateObject (const AttributeList &attributes);
  template <typename T>
  friend Ptr<T> CopyObject (Ptr<T> object);
  template <typename T>
  friend Ptr<T> CopyObject (Ptr<const T> object);

  friend class ObjectFactory;
  friend class AggregateIterator;

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
   * Invoked from ns3::ObjectFactory::Create and ns3::CreateObject only.
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
  /**
   * A pointer to the next aggregate object. This is a circular
   * linked list of aggregated objects: the last one points
   * back to the first one. If an object is not aggregated to
   * any other object, the value of this field is equal to the
   * value of the 'this' pointer.
   */
  Object *m_next;
};

/**
 * \param object a pointer to the object to copy.
 * \returns a copy of the input object.
 *
 * This method invoke the copy constructor of the input object
 * and returns the new instance.
 */
template <typename T>
Ptr<T> CopyObject (Ptr<const T> object);
template <typename T>
Ptr<T> CopyObject (Ptr<T> object);


/**
 * \param attributes a list of attributes to set on the 
 *        object during construction.
 * \returns a pointer to a newly allocated object.
 *
 * This allocates an object on the heap and initializes
 * it with a set of attributes.
 */
template <typename T>
Ptr<T> CreateObject (const AttributeList &attributes);

/**
 * \param n1 name of attribute
 * \param v1 value of attribute
 * \param n2 name of attribute
 * \param v2 value of attribute
 * \param n3 name of attribute
 * \param v3 value of attribute
 * \param n4 name of attribute
 * \param v4 value of attribute
 * \param n5 name of attribute
 * \param v5 value of attribute
 * \param n6 name of attribute
 * \param v6 value of attribute
 * \param n7 name of attribute
 * \param v7 value of attribute
 * \param n8 name of attribute
 * \param v8 value of attribute
 * \param n9 name of attribute
 * \param v9 value of attribute
 * \returns a pointer to a newly allocated object.
 *
 * This allocates an object on the heap and initializes
 * it with a set of attributes.
 */
template <typename T>
Ptr<T> 
CreateObject (std::string n1 = "", const AttributeValue & v1 = EmptyAttributeValue (),
              std::string n2 = "", const AttributeValue & v2 = EmptyAttributeValue (),
              std::string n3 = "", const AttributeValue & v3 = EmptyAttributeValue (),
              std::string n4 = "", const AttributeValue & v4 = EmptyAttributeValue (),
              std::string n5 = "", const AttributeValue & v5 = EmptyAttributeValue (),
              std::string n6 = "", const AttributeValue & v6 = EmptyAttributeValue (),
              std::string n7 = "", const AttributeValue & v7 = EmptyAttributeValue (),
              std::string n8 = "", const AttributeValue & v8 = EmptyAttributeValue (),
              std::string n9 = "", const AttributeValue & v9 = EmptyAttributeValue ());
  


} // namespace ns3

namespace ns3 {

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
Ptr<T> CopyObject (Ptr<T> object)
{
  Ptr<T> p = Ptr<T> (new T (*PeekPointer (object)), false);
  NS_ASSERT (p->m_tid == object->m_tid);
  return p;
}

template <typename T>
Ptr<T> CopyObject (Ptr<const T> object)
{
  Ptr<T> p = Ptr<T> (new T (*PeekPointer (object)), false);
  NS_ASSERT (p->m_tid == object->m_tid);
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
CreateObject (std::string n1 = "", const AttributeValue & v1 = EmptyAttributeValue (),
              std::string n2 = "", const AttributeValue & v2 = EmptyAttributeValue (),
              std::string n3 = "", const AttributeValue & v3 = EmptyAttributeValue (),
              std::string n4 = "", const AttributeValue & v4 = EmptyAttributeValue (),
              std::string n5 = "", const AttributeValue & v5 = EmptyAttributeValue (),
              std::string n6 = "", const AttributeValue & v6 = EmptyAttributeValue (),
              std::string n7 = "", const AttributeValue & v7 = EmptyAttributeValue (),
              std::string n8 = "", const AttributeValue & v8 = EmptyAttributeValue (),
              std::string n9 = "", const AttributeValue & v9 = EmptyAttributeValue ())
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

