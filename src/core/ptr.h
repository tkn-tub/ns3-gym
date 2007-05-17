/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
 * All rights reserved.
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

#include <stdint.h>
#include "assert.h"

namespace ns3 {

/**
 * \brief smart pointer class similar to boost::intrusive_ptr
 *
 * This smart-pointer class assumes that the underlying
 * type provides a pair of Ref and Unref methods which are
 * expected to increment and decrement the internal refcount
 * of the object instance.
 *
 * This implementation allows you to manipulate the smart pointer
 * as if it was a normal pointer: you can compare it with zero,
 * compare it against other pointers, assign zero to it, etc.
 *
 * It is possible to extract the raw pointer from this
 * smart pointer with the GetPointer and PeekPointer methods.
 *
 * If you want to store a newed object into a smart pointer,
 * we recommend you to use the MakeNewObject template functions
 * to create the object and store it in a smart pointer to avoid
 * memory leaks. These functions are really small conveniance
 * functions and their goal is just is save you a small
 * bit of typing.
 */
template <typename T>
class Ptr 
{
private:
  T *m_ptr;
  class Tester {
  private:
    void operator delete (void *);
  };
  friend class Ptr<const T>;
  template <typename U>
  friend U *GetPointer (const Ptr<U> &p);
  template <typename U>
  friend U *PeekPointer (const Ptr<U> &p);

  void Acquire (void) const;
public:
  /**
   * Create an empty smart pointer
   */
  Ptr ();
  /**
   * \param ptr raw pointer to manage
   *
   * Create a smart pointer which points to the
   * input raw pointer. This method takes ownershipt
   * of the input raw pointer. That is, the smart pointer
   * becomes responsible for calling delete on the
   * raw pointer when needed.
   */
  Ptr (T *ptr);
  Ptr (Ptr const&o);
  // allow conversions from T to T const.
  template <typename U>
  Ptr (Ptr<U> const &o);
  ~Ptr () ;
  Ptr<T> &operator = (Ptr const& o);

  T *operator -> () const;
  T *operator -> ();
  // allow if (!sp)
  bool operator! ();
  // allow if (sp)
  // disable delete sp
  operator Tester * () const;
};

template <typename T>
Ptr<T> MakeNewObject (void);

template <typename T, typename T1>
Ptr<T> MakeNewObject (T1 a1);

template <typename T, typename T1, typename T2>
Ptr<T> MakeNewObject (T1 a1, T2 a2);

template <typename T, typename T1, typename T2, typename T3>
Ptr<T> MakeNewObject (T1 a1, T2 a2, T3 a3);

template <typename T, typename T1, typename T2, typename T3, typename T4>
Ptr<T> MakeNewObject (T1 a1, T2 a2, T3 a3, T4 a4);

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
Ptr<T> MakeNewObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
Ptr<T> MakeNewObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
Ptr<T> MakeNewObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7);

/**
 * \relates Ptr
 * \return the pointer managed by this smart pointer.
 *
 * The underlying refcount is not incremented prior
 * to returning to the caller so the caller is not
 * responsible for calling Unref himself.
 */
template <typename T>
T * PeekPointer (const Ptr<T> &p);

/**
 * \relates Ptr
 * \return the pointer managed by this smart pointer.
 *
 * The underlying refcount is incremented prior
 * to returning to the caller so the caller is
 * responsible for calling Unref himself.
 */
template <typename T>
T * GetPointer (const Ptr<T> &p);


// allow if (sp == 0)
template <typename T1, typename T2>
bool operator == (Ptr<T1> const &lhs, T2 const *rhs);

// allow if (0 == sp)
template <typename T1, typename T2>
bool operator == (T1 const *lhs, Ptr<T2> &rhs);

// allow if (sp != 0)
template <typename T1, typename T2>
bool operator != (Ptr<T1> const &lhs, T2 const *rhs);

// allow if (0 != sp)
template <typename T1, typename T2>
bool operator != (T1 const *lhs, Ptr<T2> &rhs);

// allow if (sp0 == sp1)
template <typename T1, typename T2>
bool operator == (Ptr<T1> const &lhs, Ptr<T2> const &rhs);

// allow if (sp0 != sp1)
template <typename T1, typename T2>
bool operator != (Ptr<T1> const &lhs, Ptr<T2> const &rhs);

template <typename T1, typename T2>
Ptr<T1> const_pointer_cast (Ptr<T2> const&p);

} // namespace ns3


namespace ns3 {

  /*************************************************
   *  friend non-member function implementations
   ************************************************/

template <typename T>
Ptr<T> MakeNewObject (void)
{
  T *obj = new T ();
  Ptr<T> p = obj;
  obj->Unref ();
  return p;
}

template <typename T, typename T1>
Ptr<T> MakeNewObject (T1 a1)
{
  T *obj = new T (a1);
  Ptr<T> p = obj;
  obj->Unref ();
  return p;
}

template <typename T, typename T1, typename T2>
Ptr<T> MakeNewObject (T1 a1, T2 a2)
{
  T *obj = new T (a1, a2);
  Ptr<T> p = obj;
  obj->Unref ();
  return p;
}

template <typename T, typename T1, typename T2, typename T3>
Ptr<T> MakeNewObject (T1 a1, T2 a2, T3 a3)
{
  T *obj = new T (a1, a2, a3);
  Ptr<T> p = obj;
  obj->Unref ();
  return p;
}

template <typename T, typename T1, typename T2, typename T3, typename T4>
Ptr<T> MakeNewObject (T1 a1, T2 a2, T3 a3, T4 a4)
{
  T *obj = new T (a1, a2, a3, a4);
  Ptr<T> p = obj;
  obj->Unref ();
  return p;
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
Ptr<T> MakeNewObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  T *obj = new T (a1, a2, a3, a4, a5);
  Ptr<T> p = obj;
  obj->Unref ();
  return p;
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
Ptr<T> MakeNewObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  T *obj = new T (a1, a2, a3, a4, a5, a6);
  Ptr<T> p = obj;
  obj->Unref ();
  return p;
}

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
Ptr<T> MakeNewObject (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7)
{
  T *obj = new T (a1, a2, a3, a4, a5, a6, a7);
  Ptr<T> p = obj;
  obj->Unref ();
  return p;
}

template <typename T>
T * PeekPointer (const Ptr<T> &p)
{
  return p.m_ptr;
}

template <typename T>
T * GetPointer (const Ptr<T> &p)
{
  p.Acquire ();
  return p.m_ptr;
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


template <typename T1, typename T2>
Ptr<T1>
const_pointer_cast (Ptr<T2> const&p)
{
  return Ptr<T1> (const_cast<T1 *> (PeekPointer (p)));
}


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
{}

template <typename T>
Ptr<T>::Ptr (T *ptr) 
  : m_ptr (ptr)
{
  Acquire ();
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
      m_ptr->Unref();
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
      m_ptr->Unref();
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


}; // namespace ns3

#endif /* PTR_H */
