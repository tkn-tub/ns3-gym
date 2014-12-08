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
#include "attribute-construction-list.h"
#include "simple-ref-count.h"

/**
 * \file
 * \ingroup object
 * ns3::Object class declaration, which is the root of the Object hierarchy
 * and Aggregation.
 */

namespace ns3 {

class Object;
class AttributeAccessor;
class AttributeValue;
class TraceSourceAccessor;

/**
 * \ingroup core
 * \defgroup object Object
 * \brief Base classes which provide memory management and object aggregation.
 */
  
/**
 * \ingroup object
 * \ingroup ptr
 * Standard Object deleter, used by SimpleRefCount
 * to delete an Object when the reference count drops to zero.
 */
struct ObjectDeleter
{
  /**
   * Smart pointer deleter implementation for Object.
   *
   * Delete implementation, forwards to the Object::DoDelete()
   * method.
   *
   * \param [in] object The Object to delete.
   */
  inline static void Delete (Object *object);
};

/**
 * \ingroup object
 * \brief A base class which provides memory management and object aggregation
 *
 * The memory management scheme is based on reference-counting with
 * dispose-like functionality to break the reference cycles.
 * The reference count is incremented and decremented with
 * the methods Ref() and Unref(). If a reference cycle is
 * present, the user is responsible for breaking it
 * by calling Dispose() in a single location. This will
 * eventually trigger the invocation of DoDispose() on itself and
 * all its aggregates. The DoDispose() method is always automatically
 * invoked from the Unref() method before destroying the Object,
 * even if the user did not call Dispose() directly.
 */
class Object : public SimpleRefCount<Object, ObjectBase, ObjectDeleter>
{
public:
  /**
   * \brief Register this type.
   * \return The Object TypeId.
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Iterate over the Objects aggregated to an ns3::Object.
   *
   * This iterator does not allow you to iterate over the parent
   * Object used to call Object::GetAggregateIterator. 
   *
   * \note This is a java-style iterator.
   */
  class AggregateIterator
  {
public:
    /** Default constructor, which has no Object. */
    AggregateIterator ();

    /**
     * Check if there are more Aggregates to iterate over.
     *
     * \returns \c true if Next() can be called and return a non-null
     *          pointer, \c false otherwise.
     */
    bool HasNext (void) const;

    /**
     * Get the next Aggregated Object.
     *
     * \returns The next aggregated Object.
     */
    Ptr<const Object> Next (void);
private:
    friend class Object;
    /**
     * Construct from an Object.
     *
     * This is private, with Object as friend, so only Objects can create
     * useful AggregateIterators.
     *
     * \param [in] object The Object whose Aggregates should be iterated over.
     */
    AggregateIterator (Ptr<const Object> object);
    Ptr<const Object> m_object;                    //!< Parent Object.
    uint32_t m_current;                            //!< Current position in parent's aggegrates.
  };

  /** Constructor. */
  Object ();
  /** Destructor. */
  virtual ~Object ();

  /**
   * Implement the GetInstanceTypeId method defined in ObjectBase.
   */
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * Get a pointer to the requested aggregated Object.
   *
   * \returns A pointer to the requested Object, or zero
   *          if it could not be found.
   */
  template <typename T>
  inline Ptr<T> GetObject (void) const;
  /**
   * Get a pointer to the requested aggregated Object.
   * 
   * \param tid The TypeId of the requested Object.
   * \returns A pointer to the requested Object, or zero
   *          if it could not be found.
   */
  template <typename T>
  Ptr<T> GetObject (TypeId tid) const;
  /**
   * Dispose of this Object.
   *
   * Run the DoDispose() methods of this Object and all the
   * Objects aggregated to it.  
   * After calling this method, this Object is expected to be
   * totally unusable except for the Ref() and Unref() methods.
   *
   * \note You can call Dispose() many times on the same Object or
   * different Objects aggregated together, and DoDispose() will be
   * called only once for each aggregated Object.
   *
   * This method is typically used to break reference cycles.
   */
  void Dispose (void);
  /**
   * Aggregate two Objects together.
   *
   * \param other The other Object pointer
   *
   * This method aggregates the two Objects together: after this
   * method returns, it becomes possible to call GetObject()
   * on one to get the other, and vice-versa. 
   *
   * This method calls the virtual method NotifyNewAggregates() to
   * notify all aggregated Objects that they have been aggregated
   * together.
   *
   * \sa NotifyNewAggregate()
   */
  void AggregateObject (Ptr<Object> other);

  /**
   * Get an iterator to the Objects aggregated to this one.
   *
   * \returns An iterator to the first Object aggregated to this
   *          Object.
   *
   * If no Objects are aggregated to this Object, then, the returned
   * iterator will be empty and AggregateIterator::HasNext() will
   * always return \c false.
   */
  AggregateIterator GetAggregateIterator (void) const;

  /**
   * Invoke DoInitialize on all Objects aggregated to this one.
   *
   * This method calls the virtual DoInitialize() method on all the Objects
   * aggregated to this Object. DoInitialize() will be called only once over
   * the lifetime of an Object, just like DoDispose() is called only
   * once.
   *
   * \sa DoInitialize()
   */
  void Initialize (void);

protected:
  /**
   * Notify all Objects aggregated to this one of a new Object being
   * aggregated.
   *
   * This method is invoked whenever two sets of Objects are aggregated
   * together.  It is invoked exactly once for each Object in both sets.
   * This method can be overriden by subclasses who wish to be notified
   * of aggregation events. These subclasses must chain up to their
   * base class NotifyNewAggregate() method.
   *
   * It is safe to call GetObject() and AggregateObject() from within
   * this method.
   */
  virtual void NotifyNewAggregate (void);
  /**
   * Initialize() implementation.
   *
   * This method is called only once by Initialize(). If the user
   * calls Initialize() multiple times, DoInitialize() is called only the
   * first time.
   *
   * Subclasses are expected to override this method and chain up
   * to their parent's implementation once they are done. It is
   * safe to call GetObject() and AggregateObject() from within this method.
   */
  virtual void DoInitialize (void);
  /**
   * Destructor implementation.
   *
   * This method is called by Dispose() or by the Object's 
   * destructor, whichever comes first.
   *
   * Subclasses are expected to implement their real destruction
   * code in an overriden version of this method and chain
   * up to their parent's implementation once they are done.
   * _i.e_, for simplicity, the destructor of every subclass should
   * be empty and its content should be moved to the associated
   * DoDispose() method.
   *
   * It is safe to call GetObject() from within this method.
   */
  virtual void DoDispose (void);
  /**
   * Copy an Object.
   *
   * \param o the Object to copy.
   *
   * Allow subclasses to implement a copy constructor.
   *
   * While it is technically possible to implement a copy
   * constructor in a subclass, we strongly discourage you
   * from doing so. If you really want to do it anyway, you have
   * to understand that this copy constructor will _not_
   * copy aggregated Objects, _i.e_, if your Object instance
   * is already aggregated to another Object and if you invoke
   * this copy constructor, the new )bject instance will be
   * a pristine standalone Object instance not aggregated to
   * any other )bject. It is thus _your_ responsability
   * as a caller of this method to do what needs to be done
   * (if it is needed) to ensure that the Object stays in a
   * valid state.
   */
  Object (const Object &o);
  
private:

  /**
   * Copy an Object.
   *
   * \param object A pointer to the object to copy.
   * \returns A copy of the input object.
   *
   * This method invoke the copy constructor of the input object
   * and returns the new instance.
   */
  /**@{*/
  template <typename T>
  friend Ptr<T> CopyObject (Ptr<T> object);
  template <typename T>
  friend Ptr<T> CopyObject (Ptr<const T> object);
  /**@}*/
  
  /**
   * Set the TypeId and construct all Attributes of an Object.
   *
   * \tparam T The type of the derived object we are constructing.
   * \param [in] object The uninitialized object pointer.
   * \return The derived object.
   */
  template <typename T>
  friend Ptr<T> CompleteConstruct (T *object);

  friend class ObjectFactory;
  friend class AggregateIterator;
  friend struct ObjectDeleter;

  /**
   * The list of Objects aggregated to this one.
   *
   * This data structure uses a classic C-style trick to 
   * hold an array of variable size without performing
   * two memory allocations: the declaration of the structure
   * declares a one-element array but when we allocate
   * memory for this struct, we effectively allocate a larger
   * chunk of memory than the struct to allow space for a larger
   * variable sized buffer whose size is indicated by the element
   * \c n
   */
  struct Aggregates {
    /** The number of entries in \c buffer. */
    uint32_t n;
    /** The array of Objects. */
    Object *buffer[1];
  };

  /**
   * Find an Object of TypeId tid in the aggregates of this Object.
   *
   * \param tid The TypeId we're looking for
   * \return The matching Object, if it is found
   */
  Ptr<Object> DoGetObject (TypeId tid) const;
  /**
   * Verify that this Object is still live, by checking it's reference count.
   * \return \c true if the reference count is non zero.
   */
  bool Check (void) const;
  /**
   * Check if any aggregated Objects have non-zero reference counts.
   *
   * \return \c true if any of our aggregates have non zero reference count.
   *
   * In some cases, when an event is scheduled against a subclass of
   * Object, and if no one owns a reference directly to this Object, the
   * Object is alive, has a refcount of zero and the method run when the
   * event expires runs against the raw pointer, which means that we are
   * manipulating an Object with a refcount of zero.  So, instead we
   * check the aggregate reference count.
   */
  bool CheckLoose (void) const;
  /**
   * Set the TypeId of this Object.
   
   * \param tid The TypeId value to set.
   *
   * Invoked from ns3::CreateObject only.
   * Initialize the \c m_tid member variable to
   * keep track of the type of this Object instance.
   */
  void SetTypeId (TypeId tid);
  /**
   * Initialize all member variables registered as Attributes of this TypeId.
   *
   * \param attributes The attribute values used to initialize
   *        the member variables of this Object's instance.
   *
   * Invoked from ns3::ObjectFactory::Create and ns3::CreateObject only.
   * Initialize all the member variables which were
   * registered with the associated TypeId.
  */
  void Construct (const AttributeConstructionList &attributes);

  /**
   * Keep the list of aggregates in most-recently-used order
   *
   * \param aggregates The list of aggregated Objects.
   * \param i The most recently used entry in the list.
   */
  void UpdateSortedArray (struct Aggregates *aggregates, uint32_t i) const;
  /**
   * Attempt to delete this Object.
   *
   * This method iterates over all aggregated Objects to check if they all 
   * have a zero refcount. If yes, the Object and all
   * its aggregates are deleted. If not, nothing is done.
   */
  void DoDelete (void);

  /**
   * Identifies the type of this Object instance.
   */
  TypeId m_tid;
  /**
   * Set to \c true when the DoDispose() method of the Object has run,
   * \c false otherwise.
   */
  bool m_disposed;
  /**
   * Set to \c true once the DoInitialize() method has run,
   * \c false otherwise
   */
  bool m_initialized;
  /**
   * A pointer to an array of 'aggregates'.
   *
   * A pointer to each Object aggregated to this Object is stored in this 
   * array.  The array is shared by all aggregated Objects
   * so the size of the array is indirectly a reference count.
   */
  struct Aggregates * m_aggregates;
  /**
   * The number of times the Object was accessed with a
   * call to GetObject().
   *
   * This integer is used to implement a heuristic to sort
   * the array of aggregates in most-frequently accessed order.
   */
  uint32_t m_getObjectCount;
};

template <typename T>
Ptr<T> CopyObject (Ptr<const T> object);
template <typename T>
Ptr<T> CopyObject (Ptr<T> object);

} // namespace ns3

namespace ns3 {


/*************************************************************************
 *   The Object implementation which depends on templates
 *************************************************************************/

void 
ObjectDeleter::Delete (Object *object)
{
  object->DoDelete ();
}

template <typename T>
Ptr<T> 
Object::GetObject () const
{
  // This is an optimization: if the cast works (which is likely),
  // things will be pretty fast.
  T *result = dynamic_cast<T *> (m_aggregates->buffer[0]);
  if (result != 0)
    {
      return Ptr<T> (result);
    }
  // if the cast does not work, we try to do a full type check.
  Ptr<Object> found = DoGetObject (T::GetTypeId ());
  if (found != 0)
    {
      return Ptr<T> (static_cast<T *> (PeekPointer (found)));
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
      return Ptr<T> (static_cast<T *> (PeekPointer (found)));
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
  NS_ASSERT (p->GetInstanceTypeId () == object->GetInstanceTypeId ());
  return p;
}

template <typename T>
Ptr<T> CopyObject (Ptr<const T> object)
{
  Ptr<T> p = Ptr<T> (new T (*PeekPointer (object)), false);
  NS_ASSERT (p->GetInstanceTypeId () == object->GetInstanceTypeId ());
  return p;
}

template <typename T>
Ptr<T> CompleteConstruct (T *object)
{
  object->SetTypeId (T::GetTypeId ());
  object->Object::Construct (AttributeConstructionList ());
  return Ptr<T> (object, false);
}

/** 
 * \ingroup object
 * @{
 */
/**
 * Create an object by type, with varying number of constructor parameters.
 *
 * \tparam T The type of the derived object to construct.
 * \return The derived object.
 */
template <typename T>
Ptr<T> CreateObject (void)
{
  return CompleteConstruct (new T ());
}
/**
 * \copybrief CreateObject()
 * \tparam T The type of the derived object to construct.
 * \tparam T1 The type of the constructor argument.
 * \param a1 The constructor argument
 * \return The derived object.
 */
template <typename T, typename T1>
Ptr<T> CreateObject (T1 a1)
{
  return CompleteConstruct (new T (a1));
}

/**
 * \copybrief CreateObject()
 * \tparam T The type of the derived object to construct.
 * \tparam T1 The type of the first constructor argument.
 * \tparam T2 The type of the second constructor argument.
 * \param a1 The constructor first argument
 * \param a2 The constructor second argument
 * \return The derived object.
 */
template <typename T, typename T1, typename T2>
Ptr<T> CreateObject (T1 a1, T2 a2)
{
  return CompleteConstruct (new T (a1,a2));
}

/**
 * \copybrief CreateObject()
 * \tparam T The type of the derived object to construct.
 * \tparam T1 The type of the first constructor argument.
 * \tparam T2 The type of the second constructor argument.
 * \tparam T3 The type of the third constructor argument.
 * \param a1 The constructor first argument
 * \param a2 The constructor second argument
 * \param a3 The constructor third argument
 * \return The derived object.
 */
template <typename T, typename T1, typename T2, typename T3>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3)
{
  return CompleteConstruct (new T (a1,a2,a3));
}

/**
 * \copybrief CreateObject()
 * \tparam T The type of the derived object to construct.
 * \tparam T1 The type of the first constructor argument.
 * \tparam T2 The type of the second constructor argument.
 * \tparam T3 The type of the third constructor argument.
 * \tparam T4 The type of the fourth constructor argument.
 * \param a1 The constructor first argument
 * \param a2 The constructor second argument
 * \param a3 The constructor third argument
 * \param a4 The constructor fourth argument
 * \return The derived object.
 */
template <typename T, typename T1, typename T2, typename T3, typename T4>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4)
{
  return CompleteConstruct (new T (a1,a2,a3,a4));
}

/**
 * \copybrief CreateObject()
 * \tparam T The type of the derived object to construct.
 * \tparam T1 The type of the first constructor argument.
 * \tparam T2 The type of the second constructor argument.
 * \tparam T3 The type of the third constructor argument.
 * \tparam T4 The type of the fourth constructor argument.
 * \tparam T5 The type of the fifth constructor argument.
 * \param a1 The constructor first argument
 * \param a2 The constructor second argument
 * \param a3 The constructor third argument
 * \param a4 The constructor fourth argument
 * \param a5 The constructor fifth argument
 * \return The derived object.
 */
template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  return CompleteConstruct (new T (a1,a2,a3,a4,a5));
}

/**
 * \copybrief CreateObject()
 * \tparam T The type of the derived object to construct.
 * \tparam T1 The type of the first constructor argument.
 * \tparam T2 The type of the second constructor argument.
 * \tparam T3 The type of the third constructor argument.
 * \tparam T4 The type of the fourth constructor argument.
 * \tparam T5 The type of the fifth constructor argument.
 * \tparam T6 The type of the sixth constructor argument.
 * \param a1 The constructor first argument
 * \param a2 The constructor second argument
 * \param a3 The constructor third argument
 * \param a4 The constructor fourth argument
 * \param a5 The constructor fifth argument
 * \param a6 The constructor sixth argument
 * \return The derived object.
 */
template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  return CompleteConstruct (new T (a1,a2,a3,a4,a5,a6));
}

/**
 * \copybrief CreateObject()
 * \tparam T The type of the derived object to construct.
 * \tparam T1 The type of the first constructor argument.
 * \tparam T2 The type of the second constructor argument.
 * \tparam T3 The type of the third constructor argument.
 * \tparam T4 The type of the fourth constructor argument.
 * \tparam T5 The type of the fifth constructor argument.
 * \tparam T6 The type of the sixth constructor argument.
 * \tparam T7 The type of the seventh constructor argument.
 * \param a1 The constructor first argument
 * \param a2 The constructor second argument
 * \param a3 The constructor third argument
 * \param a4 The constructor fourth argument
 * \param a5 The constructor fifth argument
 * \param a6 The constructor sixth argument
 * \param a7 The constructor seventh argument
 * \return The derived object.
 */
template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
Ptr<T> CreateObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7)
{
  return CompleteConstruct (new T (a1,a2,a3,a4,a5,a6,a7));
}
/**@}*/

} // namespace ns3

#endif /* OBJECT_H */

