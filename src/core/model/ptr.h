/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef PTR_H
#define PTR_H

#include <iostream>
#include <stdint.h>
#include "assert.h"

/**
 * \file
 * \ingroup ptr
 * Smart pointer implementation.
 */

namespace ns3 {

/**
 * \ingroup core
 * \defgroup ptr Smart Pointer
 * \brief Heap memory management.
 *
 * See \ref ns3::Ptr for implementation details.
 *
 * See \ref main-ptr.cc for example usage.
 */
/**
 * \ingroup ptr
 *
 * \brief Smart pointer class similar to \c boost::intrusive_ptr.
 *
 * This smart-pointer class assumes that the underlying
 * type provides a pair of \c Ref and \c Unref methods which are
 * expected to increment and decrement the internal reference count
 * of the object instance.  You can add \c Ref and \c Unref
 * to a class simply by inheriting from ns3::SimpleRefCount.
 *
 * This implementation allows you to manipulate the smart pointer
 * as if it was a normal pointer: you can compare it with zero,
 * compare it against other pointers, assign zero to it, etc.
 *
 * It is possible to extract the raw pointer from this
 * smart pointer with the GetPointer() and PeekPointer() methods.
 *
 * If you want to store a \c new object into a smart pointer,
 * we recommend you to use the Create() template functions
 * to create the object and store it in a smart pointer to avoid
 * memory leaks. These functions are really small convenience
 * functions and their goal is just is save you a small
 * bit of typing.
 *
 * \tparam T The underlying type.
 */
template <typename T>
class Ptr 
{
private:

  /** The pointer. */
  T *m_ptr;

  /** Helper to test for null pointer. */
  class Tester {
  private:
    /** Disable delete (by virtue that this is unimplemented). */
    void operator delete (void *);
  };
  
  /** Interoperate with const instances. */
  friend class Ptr<const T>;
  
  /**
   * Get a permanent pointer to the underlying object.
   *
   * The underlying refcount is incremented prior
   * to returning to the caller so the caller is
   * responsible for calling Unref himself.
   *
   * \param p smart pointer
   * \return the pointer managed by this smart pointer.
   */
  template <typename U>
  friend U *GetPointer (const Ptr<U> &p);
  /**
   * Get a temporary pointer to the underlying object.
   *
   * The underlying refcount is not incremented prior
   * to returning to the caller so the caller is not
   * responsible for calling Unref himself.
   *
   * \param p smart pointer
   * \return the pointer managed by this smart pointer.
   */
  template <typename U>
  friend U *PeekPointer (const Ptr<U> &p);

  /** Mark this as a a reference by incrementing the reference count. */
  inline void Acquire (void) const;
  
public:
  /** Create an empty smart pointer */
  Ptr ();
  /**
   * Create a smart pointer which points to the object pointed to by
   * the input raw pointer ptr. This method creates its own reference
   * to the pointed object. The caller is responsible for Unref()'ing
   * its own reference, and the smart pointer will eventually do the
   * same, so that object is deleted if no more references to it
   * remain.
   *
   * \param ptr raw pointer to manage
   */
  Ptr (T *ptr);
  /**
   * Create a smart pointer which points to the object pointed to by
   * the input raw pointer ptr.
   *
   * \param ptr raw pointer to manage
   * \param ref if set to true, this method calls Ref, otherwise,
   *        it does not call Ref.
   */
  Ptr (T *ptr, bool ref);
  /**
   * Copy by referencing the same underlying object.
   *
   * \param [in] o The other Ptr instance.
   */
  Ptr (Ptr const&o);
  /**
   * Copy, removing \c const qualifier.
   *
   * \tparam U The underlying type of the \c const object.
   * \param [in] o The Ptr to copy.
   */
  template <typename U>
  Ptr (Ptr<U> const &o); 
  /** Destructor. */
  ~Ptr ();
  /**
   * Assignment operator by referencing the same underlying object.
   *
   * \param [in] o The other Ptr instance.
   * \return A reference to self.
   */
  Ptr<T> &operator = (Ptr const& o);
  /**
   * An rvalue member access.
   * \returns A pointer to the underlying object.
   */
  T *operator -> () const;
  /**
   * An lvalue member access.
   * \returns A pointer to the underlying object.
   */
  T *operator -> ();
  /**
   * A \c const dereference.
   * \returns A pointer to the underlying object.
   */
  const T &operator * () const;
  /**
   * A  dereference.
   * \returns A pointer to the underlying object.
   */
  T &operator * ();
  /**
   * Test for NULL pointer.
   *
   * This enables simple NULL pointer checks like
   * \code
   *   Ptr<..> p = ...;
   *   if (!p) ...
   * \endcode
   * \returns true if the underlying pointer is NULL.
   */
  bool operator! ();
  /**
   * Test for non-NULL pointer.
   *
   * This enables simple pointer checks like
   * \code
   *   Ptr<...> p = ...;
   *   if (p) ...
   * \endcode
   * This also disables deleting a Ptr
   */
  operator Tester * () const;
};

/**
 * \ingroup ptr
 * Create class instances by constructors with varying numbers
 * of arguments and return them by Ptr.
 *
 * These methods work for any class \c T.
 *
 * \see CreateObject for methods to create derivatives of ns3::Object
 */
/** @{ */
/**
 * \tparam T  The type of class object to create.
 * \return A Ptr to the newly created \c T.
 */
template <typename T>
Ptr<T> Create (void);

/**
 * \tparam T  The type of class object to create.
 * \tparam T1 The type of the first constructor argument.
 * \param  a1 The first constructor argument.
 * \return A Ptr to the newly created \c T.
 */
template <typename T,
          typename T1>
Ptr<T> Create (T1 a1);

/**
 * \tparam T  The type of class object to create.
 * \tparam T1 The type of the first constructor argument.
 * \tparam T2 The type of the second constructor argument.
 * \param  a1 The first constructor argument.
 * \param  a2 The second constructor argument.
 * \return A Ptr to the newly created \c T.
 */
template <typename T,
          typename T1, typename T2>
Ptr<T> Create (T1 a1, T2 a2);

/**
 * \tparam T  The type of class object to create.
 * \tparam T1 The type of the first constructor argument.
 * \tparam T2 The type of the second constructor argument.
 * \tparam T3 The type of the third constructor argument.
 * \param  a1 The first constructor argument.
 * \param  a2 The second constructor argument.
 * \param  a3 The third constructor argument.
 * \return A Ptr to the newly created \c T.
 */
template <typename T,
          typename T1, typename T2,
          typename T3>
Ptr<T> Create (T1 a1, T2 a2, T3 a3);

/**
 * \tparam T  The type of class object to create.
 * \tparam T1 The type of the first constructor argument.
 * \tparam T2 The type of the second constructor argument.
 * \tparam T3 The type of the third constructor argument.
 * \tparam T4 The type of the fourth constructor argument.
 * \param  a1 The first constructor argument.
 * \param  a2 The second constructor argument.
 * \param  a3 The third constructor argument.
 * \param  a4 The fourth constructor argument.
 * \return A Ptr to the newly created \c T.
 */
template <typename T,
          typename T1, typename T2,
          typename T3, typename T4>
Ptr<T> Create (T1 a1, T2 a2, T3 a3, T4 a4);

/**
 * \tparam T  The type of class object to create.
 * \tparam T1 The type of the first constructor argument.
 * \tparam T2 The type of the second constructor argument.
 * \tparam T3 The type of the third constructor argument.
 * \tparam T4 The type of the fourth constructor argument.
 * \tparam T5 The type of the fifth constructor argument.
 * \param  a1 The first constructor argument.
 * \param  a2 The second constructor argument.
 * \param  a3 The third constructor argument.
 * \param  a4 The fourth constructor argument.
 * \param  a5 The fifth constructor argument.
 * \return A Ptr to the newly created \c T.
 */
template <typename T,
          typename T1, typename T2,
          typename T3, typename T4,
          typename T5>
Ptr<T> Create (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

/**
 * \tparam T  The type of class object to create.
 * \tparam T1 The type of the first constructor argument.
 * \tparam T2 The type of the second constructor argument.
 * \tparam T3 The type of the third constructor argument.
 * \tparam T4 The type of the fourth constructor argument.
 * \tparam T5 The type of the fifth constructor argument.
 * \tparam T6 The type of the sixth constructor argument.
 * \param  a1 The first constructor argument.
 * \param  a2 The second constructor argument.
 * \param  a3 The third constructor argument.
 * \param  a4 The fourth constructor argument.
 * \param  a5 The fifth constructor argument.
 * \param  a6 The sixth constructor argument.
 * \return A Ptr to the newly created \c T.
 */
template <typename T,
          typename T1, typename T2,
          typename T3, typename T4,
          typename T5, typename T6>
Ptr<T> Create (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);

/**
 * \tparam T  The type of class object to create.
 * \tparam T1 The type of the first constructor argument.
 * \tparam T2 The type of the second constructor argument.
 * \tparam T3 The type of the third constructor argument.
 * \tparam T4 The type of the fourth constructor argument.
 * \tparam T5 The type of the fifth constructor argument.
 * \tparam T6 The type of the sixth constructor argument.
 * \tparam T7 The type of the seventh constructor argument.
 * \param  a1 The first constructor argument.
 * \param  a2 The second constructor argument.
 * \param  a3 The third constructor argument.
 * \param  a4 The fourth constructor argument.
 * \param  a5 The fifth constructor argument.
 * \param  a6 The sixth constructor argument.
 * \param  a7 The seventh constructor argument.
 * \return A Ptr to the newly created \c T.
 */
template <typename T,
          typename T1, typename T2,
          typename T3, typename T4,
          typename T5, typename T6,
          typename T7>
Ptr<T> Create (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7);
/** @}*/

/**
 * \ingroup ptr
 * Output streamer.
 * \param [in] os The output stream.
 * \param [in] p The Ptr.
 * \returns The stream.
 */
template <typename T>
std::ostream &operator << (std::ostream &os, const Ptr<T> &p);

/**
 * \ingroup ptr
 * Equality operator.
 *
 * This enables code such as
 * \code
 *   Ptr<...> p = ...;
 *   Ptr<...> q = ...;
 *   if (p == q) ...
 * \endcode
 *
 * Note that either \c p or \c q could also be ordinary pointers
 * to the underlying object.
 *
 * \tparam T1 Type of the object on the lhs.
 * \tparam T2 Type of the object on the rhs.
 * \param [in] lhs The left operand.
 * \param [in] rhs The right operand.
 * \return true if the operands point to the same underlying object.
 */
/** @{ */
template <typename T1, typename T2>
bool operator == (Ptr<T1> const &lhs, T2 const *rhs);

template <typename T1, typename T2>
bool operator == (T1 const *lhs, Ptr<T2> &rhs);

template <typename T1, typename T2>
bool operator == (Ptr<T1> const &lhs, Ptr<T2> const &rhs);
/**@}*/

/**
 * \ingroup ptr
 * Inequality operator.
 *
 * This enables code such as
 * \code
 *   Ptr<...> p = ...;
 *   Ptr<...> q = ...;
 *   if (p != q) ...
 * \endcode
 *
 * Note that either \c p or \c q could also be ordinary pointers
 * to the underlying object.
 *
 * \tparam T1 Type of the object on the lhs.
 * \tparam T2 Type of the object on the rhs.
 * \param [in] lhs The left operand.
 * \param [in] rhs The right operand.
 * \return true if the operands point to the same underlying object.
 */
/** @{ */
template <typename T1, typename T2>
bool operator != (Ptr<T1> const &lhs, T2 const *rhs);

template <typename T1, typename T2>
bool operator != (T1 const *lhs, Ptr<T2> &rhs);

template <typename T1, typename T2>
bool operator != (Ptr<T1> const &lhs, Ptr<T2> const &rhs);
/**@}*/


/**
 * \ingroup ptr
 * Comparison operator applied to the underlying pointers.
 *
 * \param [in] lhs The left operand.
 * \param [in] rhs The right operand.
 * \return The comparison on the underlying pointers.
 */
/** @{ */
template <typename T>
bool operator < (const Ptr<T> &lhs, const Ptr<T> &rhs);
template <typename T>
bool operator <= (const Ptr<T> &lhs, const Ptr<T> &rhs);
template <typename T>
bool operator > (const Ptr<T> &lhs, const Ptr<T> &rhs);
template <typename T>
bool operator >= (const Ptr<T> &lhs, const Ptr<T> &rhs);
/** @} */
  
/**
 * Return a copy of \c p with its stored pointer const casted from
 * \c T2 to \c T1.
 *
 * \tparam T1 The type to return in a Ptr.
 * \tparam T2 The type of the underlying object.
 * \param p The original \c const Ptr.
 * \return A non-const Ptr.
 */
template <typename T1, typename T2>
Ptr<T1> const_pointer_cast (Ptr<T2> const&p);

template <typename T>
struct CallbackTraits;

/**
 * \ingroup makecallbackmemptr
 *
 * Trait class to convert a pointer into a reference,
 * used by MemPtrCallBackImpl.
 *
 * This is the specialization for Ptr types.
 *
 * \tparam The base object type.
 */
template <typename T>
struct CallbackTraits<Ptr<T> >
{
  /**
   * \param p object pointer
   * \return a reference to the object pointed to by p
   */
  static T & GetReference (Ptr<T> const p)
  {
    return *PeekPointer (p);
  }
};

template <typename T>
struct EventMemberImplObjTraits;

/**
 * \ingroup makeeventmemptr
 * Helper for the MakeEvent functions which take a class method.
 *
 * This is the specialization for Ptr types.
 *
 * \tparam T The class type.
 */
template <typename T>
struct EventMemberImplObjTraits<Ptr<T> >
{
  /**
   * \param p object pointer
   * \return a reference to the object pointed to by p
   */
  static T &GetReference (Ptr<T> p) {
    return *PeekPointer (p);
  }
};



} // namespace ns3


namespace ns3 {

/*************************************************
 *  friend non-member function implementations
 ************************************************/

template <typename T>
Ptr<T> Create (void)
{
  return Ptr<T> (new T (), false);
}

template <typename T, typename T1>
Ptr<T> Create (T1 a1)
{
  return Ptr<T> (new T (a1), false);
}

template <typename T, typename T1, typename T2>
Ptr<T> Create (T1 a1, T2 a2)
{
  return Ptr<T> (new T (a1, a2), false);
}

template <typename T, typename T1, typename T2, typename T3>
Ptr<T> Create (T1 a1, T2 a2, T3 a3)
{
  return Ptr<T> (new T (a1, a2, a3), false);
}

template <typename T, typename T1, typename T2, typename T3, typename T4>
Ptr<T> Create (T1 a1, T2 a2, T3 a3, T4 a4)
{
  return Ptr<T> (new T (a1, a2, a3, a4), false);
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
Ptr<T> Create (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  return Ptr<T> (new T (a1, a2, a3, a4, a5), false);
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
Ptr<T> Create (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  return Ptr<T> (new T (a1, a2, a3, a4, a5, a6), false);
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
Ptr<T> Create (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7)
{
  return Ptr<T> (new T (a1, a2, a3, a4, a5, a6, a7), false);
}

template <typename U>
U * PeekPointer (const Ptr<U> &p)
{
  return p.m_ptr;
}

template <typename U>
U * GetPointer (const Ptr<U> &p)
{
  p.Acquire ();
  return p.m_ptr;
}

template <typename T>
std::ostream &operator << (std::ostream &os, const Ptr<T> &p)
{
  os << PeekPointer (p);
  return os;
}

template <typename T1, typename T2>
bool 
operator == (Ptr<T1> const &lhs, T2 const *rhs)
{
  return PeekPointer (lhs) == rhs;
}

template <typename T1, typename T2>
bool 
operator == (T1 const *lhs, Ptr<T2> &rhs)
{
  return lhs == PeekPointer (rhs);
}

template <typename T1, typename T2>
bool 
operator != (Ptr<T1> const &lhs, T2 const *rhs)
{
  return PeekPointer (lhs) != rhs;
}

template <typename T1, typename T2>
bool 
operator != (T1 const *lhs, Ptr<T2> &rhs)
{
  return lhs != PeekPointer (rhs);
}

template <typename T1, typename T2>
bool 
operator == (Ptr<T1> const &lhs, Ptr<T2> const &rhs)
{
  return PeekPointer (lhs) == PeekPointer (rhs);
}

template <typename T1, typename T2>
bool 
operator != (Ptr<T1> const &lhs, Ptr<T2> const &rhs)
{
  return PeekPointer (lhs) != PeekPointer (rhs);
}

template <typename T>
bool operator < (const Ptr<T> &lhs, const Ptr<T> &rhs)
{
  return PeekPointer<T> (lhs) < PeekPointer<T> (rhs);
}

template <typename T>
bool operator <= (const Ptr<T> &lhs, const Ptr<T> &rhs)
{
  return PeekPointer<T> (lhs) <= PeekPointer<T> (rhs);
}

template <typename T>
bool operator > (const Ptr<T> &lhs, const Ptr<T> &rhs)
{
  return PeekPointer<T> (lhs) > PeekPointer<T> (rhs);
}

template <typename T>
bool operator >= (const Ptr<T> &lhs, const Ptr<T> &rhs)
{
  return PeekPointer<T> (lhs) >= PeekPointer<T> (rhs);
}

/**
 * Cast a Ptr.
 *
 * \tparam T1 The desired type to cast to.
 * \tparam T2 The type of the original Ptr.
 * \param p The original Ptr.
 * \return The result of the cast.
 */
/** @{ */
template <typename T1, typename T2>
Ptr<T1>
ConstCast (Ptr<T2> const&p)
{
  return Ptr<T1> (const_cast<T1 *> (PeekPointer (p)));
}

template <typename T1, typename T2>
Ptr<T1>
DynamicCast (Ptr<T2> const&p)
{
  return Ptr<T1> (dynamic_cast<T1 *> (PeekPointer (p)));
}

template <typename T1, typename T2>
Ptr<T1>
StaticCast (Ptr<T2> const&p)
{
  return Ptr<T1> (static_cast<T1 *> (PeekPointer (p)));
}
/** @} */

/**
 * Return a deep copy of a Ptr.
 *
 * \param object The object Ptr to copy.
 * \returns The copy.
 */
/** @{ */
template <typename T>
Ptr<T> Copy (Ptr<T> object)
{
  Ptr<T> p = Ptr<T> (new T (*PeekPointer (object)), false);
  return p;
}

template <typename T>
Ptr<T> Copy (Ptr<const T> object)
{
  Ptr<T> p = Ptr<T> (new T (*PeekPointer (object)), false);
  return p;
}
/** @} */

/****************************************************
 *      Member method implementations.
 ***************************************************/

template <typename T>
void 
Ptr<T>::Acquire (void) const
{
  if (m_ptr != 0)
    {
      m_ptr->Ref ();
    }
}

template <typename T>
Ptr<T>::Ptr ()
  : m_ptr (0)
{
}

template <typename T>
Ptr<T>::Ptr (T *ptr)
  : m_ptr (ptr)
{
  Acquire ();
}

template <typename T>
Ptr<T>::Ptr (T *ptr, bool ref)
  : m_ptr (ptr)
{
  if (ref)
    {
      Acquire ();
    }
}

template <typename T>
Ptr<T>::Ptr (Ptr const&o) 
  : m_ptr (PeekPointer (o))
{
  Acquire ();
}
template <typename T>
template <typename U>
Ptr<T>::Ptr (Ptr<U> const &o)
  : m_ptr (PeekPointer (o))
{
  Acquire ();
}

template <typename T>
Ptr<T>::~Ptr () 
{
  if (m_ptr != 0) 
    {
      m_ptr->Unref ();
    }
}

template <typename T>
Ptr<T> &
Ptr<T>::operator = (Ptr const& o) 
{
  if (&o == this)
    {
      return *this;
    }
  if (m_ptr != 0) 
    {
      m_ptr->Unref ();
    }
  m_ptr = o.m_ptr;
  Acquire ();
  return *this;
}

template <typename T>
T *
Ptr<T>::operator -> () 
{
  return m_ptr;
}

template <typename T>
T *
Ptr<T>::operator -> () const
{
  return m_ptr;
}

template <typename T>
const T &
Ptr<T>::operator * () const
{
  return *m_ptr;
}

template <typename T>
T &
Ptr<T>::operator * ()
{
  return *m_ptr;
}

template <typename T>
bool 
Ptr<T>::operator! () 
{
  return m_ptr == 0;
}

template <typename T>
Ptr<T>::operator Tester * () const
{
  if (m_ptr == 0) 
    {
      return 0;
    }
  static Tester test;
  return &test;
}


} // namespace ns3

#endif /* PTR_H */
