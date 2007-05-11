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
 * \brief smart pointer class similar to boost::shared_ptr
 *
 * This smart-pointer class is supposed to be used to manage
 * heap-allocated objects: when it decides it does not need
 * the object it references, it invokes operator delete on it.
 * This implementation allows you to manipulate the smart pointer
 * as if it was a normal pointer: you can compare it with zero,
 * compare it against other pointers, etc. However, the only 
 * operation we are careful to avoid is the conversion back to
 * raw pointers: if you need to convert back, you need to invoke
 * the Ptr<T>::Remove method which returns a raw pointer and
 * makes the smart pointer forget about the raw pointer.
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

  /**
   * \return the pointer managed by this smart pointer.
   *
   * The underlying refcount is not incremented prior
   * to returning to the caller so the caller is not
   * responsible for calling Unref himself.
   */
  T * Peek () const;

  /**
   * \return the pointer managed by this smart pointer.
   *
   * The underlying refcount is incremented prior
   * to returning to the caller so the caller is
   * responsible for calling Unref himself.
   */
  T * Get () const;
  T *operator -> () const;
  T *operator -> ();
  // allow if (!sp)
  bool operator! ();
  // allow if (sp)
  operator Tester * () const;
  // allow if (sp == 0)
  template <typename T1, typename T2>
  inline friend bool operator == (Ptr<T1> const &lhs, T2 const *rhs);
  // allow if (0 == sp)
  template <typename T1, typename T2>
  inline friend bool operator == (T1 const *lhs, Ptr<T2> &rhs);
  // allow if (sp != 0)
  template <typename T1, typename T2>
  inline friend bool operator != (Ptr<T1> const &lhs, T2 const *rhs);
  // allow if (0 != sp)
  template <typename T1, typename T2>
  inline friend bool operator != (T1 const *lhs, Ptr<T2> &rhs);

  // allow if (sp0 == sp1)
  template <typename T1, typename T2>
  inline friend bool operator == (Ptr<T1> const &lhs, Ptr<T2> const &rhs);
  // allow if (sp0 != sp1)
  template <typename T1, typename T2>
  inline friend bool operator != (Ptr<T1> const &lhs, Ptr<T2> const &rhs);

  template <typename T1, typename T2>
  inline friend Ptr<T1> const_pointer_cast (Ptr<T2> const&p);


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
  : m_ptr (o.Peek ())
{
  Acquire ();
}
template <typename T>
template <typename U>
Ptr<T>::Ptr (Ptr<U> const &o)
  : m_ptr (o.Peek ())
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
Ptr<T>::Peek () const
{
  return m_ptr;
}

template <typename T>
T * 
Ptr<T>::Get () const
{
  Acquire ();
  return m_ptr;
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

// non-member friend functions.
template <typename T1, typename T2>
bool 
operator == (Ptr<T1> const &lhs, T2 const *rhs)
{
  return lhs.m_ptr == rhs;
}
template <typename T1, typename T2>
bool 
operator == (T1 const *lhs, Ptr<T2> &rhs)
{
  return lhs == rhs.m_ptr;
}
template <typename T1, typename T2>
bool 
operator != (Ptr<T1> const &lhs, T2 const *rhs)
{
  return lhs.m_ptr != rhs;
}
template <typename T1, typename T2>
bool 
operator != (T1 const *lhs, Ptr<T2> &rhs)
{
  return lhs != rhs.m_ptr;
}

template <typename T1, typename T2>
bool 
operator == (Ptr<T1> const &lhs, Ptr<T2> const &rhs)
{
  return lhs.Peek () == rhs.Peek ();
}
template <typename T1, typename T2>
bool 
operator != (Ptr<T1> const &lhs, Ptr<T2> const &rhs)
{
  return lhs.Peek () != rhs.Peek ();
}


template <typename T1, typename T2>
Ptr<T1>
const_pointer_cast (Ptr<T2> const&p)
{
  return Ptr<T1> (const_cast<T1 *> (p.m_ptr));
}


}; // namespace ns3

#endif /* PTR_H */
