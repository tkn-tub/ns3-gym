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

#ifndef CALLBACK_H
#define CALLBACK_H

#include "ptr.h"
#include "fatal-error.h"
#include "empty.h"
#include "type-traits.h"
#include "attribute.h"
#include "attribute-helper.h"
#include "simple-ref-count.h"
#include <typeinfo>

namespace ns3 {

/**
 * \ingroup core
 * \defgroup callback Callbacks
 */
/**
 * \ingroup callback
 */
/**@{*/
/**
 * Trait class to convert a pointer into a reference,
 * used by MemPtrCallBackImpl
 */
template <typename T>
struct CallbackTraits;

/**
 * Trait class to convert a pointer into a reference,
 * used by MemPtrCallBackImpl
 */
template <typename T>
struct CallbackTraits<T *>
{
  /**
   * \param p object pointer
   * \return a reference to the object pointed to by p
   */
  static T & GetReference (T * const p)
  {
    return *p;
  }
};
/**@}*/

/**
 * \ingroup callback
 * \defgroup callbackimpl Callback Implementation
 * CallbackImpl classes
 */
/**
 * \ingroup callbackimpl
 * Abstract base class for CallbackImpl
 * Provides reference counting and equality test.
 */
class CallbackImplBase : public SimpleRefCount<CallbackImplBase>
{
public:
  /** Virtual destructor */
  virtual ~CallbackImplBase () {}
  /**
   * Equality test
   *
   * \param other Callback Ptr
   * \return true if we are equal
   */
  virtual bool IsEqual (Ptr<const CallbackImplBase> other) const = 0;
};

/**
 * \ingroup callbackimpl
 * The unqualified CallbackImpl class
 */
template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class CallbackImpl;
  
/**
 * \ingroup callbackimpl
 * CallbackImpl classes with varying numbers of argument types
 */
/**@{*/
/** CallbackImpl class with no arguments. */
template <typename R>
class CallbackImpl<R,empty,empty,empty,empty,empty,empty,empty,empty,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (void) = 0;      //!< Abstract operator
};
/** CallbackImpl class with one argument. */
template <typename R, typename T1>
class CallbackImpl<R,T1,empty,empty,empty,empty,empty,empty,empty,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1) = 0;        //!< Abstract operator
};
/** CallbackImpl class with two arguments. */
template <typename R, typename T1, typename T2>
class CallbackImpl<R,T1,T2,empty,empty,empty,empty,empty,empty,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1, T2) = 0;    //!< Abstract operator
};
/** CallbackImpl class with three arguments. */
template <typename R, typename T1, typename T2, typename T3>
class CallbackImpl<R,T1,T2,T3,empty,empty,empty,empty,empty,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1, T2, T3) = 0;  //!< Abstract operator
};
/** CallbackImpl class with four arguments. */
template <typename R, typename T1, typename T2, typename T3, typename T4>
class CallbackImpl<R,T1,T2,T3,T4,empty,empty,empty,empty,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1, T2, T3, T4) = 0;  //!< Abstract operator
};
/** CallbackImpl class with five arguments. */
template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
class CallbackImpl<R,T1,T2,T3,T4,T5,empty,empty,empty,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1, T2, T3, T4, T5) = 0;  //!< Abstract operator
};
/** CallbackImpl class with six arguments. */
template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class CallbackImpl<R,T1,T2,T3,T4,T5,T6,empty,empty,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1, T2, T3, T4, T5, T6) = 0;  //!< Abstract operator
};
/** CallbackImpl class with seven arguments. */
template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,empty,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1, T2, T3, T4, T5, T6, T7) = 0;  //!< Abstract operator
};
/** CallbackImpl class with eight arguments. */
template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,T8,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1, T2, T3, T4, T5, T6, T7, T8) = 0;  //!< Abstract operator
};
/** CallbackImpl class with nine arguments. */
template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class CallbackImpl : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1, T2, T3, T4, T5, T6, T7, T8, T9) = 0;  //!< Abstract operator
};
/**@}*/


/**
 * \ingroup callback
 * CallbackImpl with functors
 */
template <typename T, typename R, typename T1, typename T2, typename T3, typename T4,typename T5, typename T6, typename T7, typename T8, typename T9>
class FunctorCallbackImpl : public CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> {
public:
  /**
   * Construct from a functor
   *
   * \param functor the functor 
   */
  FunctorCallbackImpl (T const &functor)
    : m_functor (functor) {}
  virtual ~FunctorCallbackImpl () {}
  /**
   * Functor with varying numbers of arguments
   * @{
   */
  /** \return Callback value */
  R operator() (void) {
    return m_functor ();
  }
  /**
   * \param a1 first argument
   * \return Callback value
   */
  R operator() (T1 a1) {
    return m_functor (a1);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2) {
    return m_functor (a1,a2);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3) {
    return m_functor (a1,a2,a3);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4) {
    return m_functor (a1,a2,a3,a4);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5) {
    return m_functor (a1,a2,a3,a4,a5);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6) {
    return m_functor (a1,a2,a3,a4,a5,a6);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \param a7 seventh argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6,T7 a7) {
    return m_functor (a1,a2,a3,a4,a5,a6,a7);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \param a7 seventh argument
   * \param a8 eighth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6,T7 a7,T8 a8) {
    return m_functor (a1,a2,a3,a4,a5,a6,a7,a8);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \param a7 seventh argument
   * \param a8 eighth argument
   * \param a9 ninth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6,T7 a7,T8 a8,T9 a9) {
    return m_functor (a1,a2,a3,a4,a5,a6,a7,a8,a9);
  }
  /**@}*/
  /**
   * Equality test.
   *
   * \param other CallbackImpl Ptr
   * \return true if this and other have the same functor
   */
  virtual bool IsEqual (Ptr<const CallbackImplBase> other) const {
    FunctorCallbackImpl<T,R,T1,T2,T3,T4,T5,T6,T7,T8,T9> const *otherDerived = 
      dynamic_cast<FunctorCallbackImpl<T,R,T1,T2,T3,T4,T5,T6,T7,T8,T9> const *> (PeekPointer (other));
    if (otherDerived == 0)
      {
        return false;
      }
    else if (otherDerived->m_functor != m_functor)
      {
        return false;
      }
    return true;
  }
private:
  T m_functor;                          //!< the functor
};

/**
 * \ingroup callback
 * CallbackImpl for pointer to member functions
 */
template <typename OBJ_PTR, typename MEM_PTR, typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class MemPtrCallbackImpl : public CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> {
public:
  /**
   * Construct from an object pointer and member function pointer
   *
   * \param objPtr the object pointer
   * \param memPtr the object class member function
   */
  MemPtrCallbackImpl (OBJ_PTR const&objPtr, MEM_PTR memPtr)
    : m_objPtr (objPtr), m_memPtr (memPtr) {}
  virtual ~MemPtrCallbackImpl () {}
  /**
   * Functor with varying numbers of arguments
   * @{
   */
  /** \return Callback value */
  R operator() (void) {
    return ((CallbackTraits<OBJ_PTR>::GetReference (m_objPtr)).*m_memPtr)();
  }
  /**
   * \param a1 first argument
   * \return Callback value
   */
  R operator() (T1 a1) {
    return ((CallbackTraits<OBJ_PTR>::GetReference (m_objPtr)).*m_memPtr)(a1);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2) {
    return ((CallbackTraits<OBJ_PTR>::GetReference (m_objPtr)).*m_memPtr)(a1, a2);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3) {
    return ((CallbackTraits<OBJ_PTR>::GetReference (m_objPtr)).*m_memPtr)(a1, a2, a3);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4) {
    return ((CallbackTraits<OBJ_PTR>::GetReference (m_objPtr)).*m_memPtr)(a1, a2, a3, a4);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5) {
    return ((CallbackTraits<OBJ_PTR>::GetReference (m_objPtr)).*m_memPtr)(a1, a2, a3, a4, a5);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6) {
    return ((CallbackTraits<OBJ_PTR>::GetReference (m_objPtr)).*m_memPtr)(a1, a2, a3, a4, a5, a6);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \param a7 seventh argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6,T7 a7) {
    return ((CallbackTraits<OBJ_PTR>::GetReference (m_objPtr)).*m_memPtr)(a1, a2, a3, a4, a5, a6, a7);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \param a7 seventh argument
   * \param a8 eighth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6,T7 a7,T8 a8) {
    return ((CallbackTraits<OBJ_PTR>::GetReference (m_objPtr)).*m_memPtr)(a1, a2, a3, a4, a5, a6, a7, a8);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \param a7 seventh argument
   * \param a8 eighth argument
   * \param a9 ninth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6,T7 a7,T8 a8, T9 a9) {
    return ((CallbackTraits<OBJ_PTR>::GetReference (m_objPtr)).*m_memPtr)(a1, a2, a3, a4, a5, a6, a7, a8, a9);
  }
  /**@}*/
  /**
   * Equality test.
   *
   * \param other Callback Ptr
   * \return true if we have the same object and member function
   */
  virtual bool IsEqual (Ptr<const CallbackImplBase> other) const {
    MemPtrCallbackImpl<OBJ_PTR,MEM_PTR,R,T1,T2,T3,T4,T5,T6,T7,T8,T9> const *otherDerived = 
      dynamic_cast<MemPtrCallbackImpl<OBJ_PTR,MEM_PTR,R,T1,T2,T3,T4,T5,T6,T7,T8,T9> const *> (PeekPointer (other));
    if (otherDerived == 0)
      {
        return false;
      }
    else if (otherDerived->m_objPtr != m_objPtr ||
             otherDerived->m_memPtr != m_memPtr)
      {
        return false;
      }
    return true;
  }
private:
  OBJ_PTR const m_objPtr;               //!< the object pointer
  MEM_PTR m_memPtr;                     //!< the member function pointer
};

/**
 * \ingroup callback
 * CallbackImpl for functors with first argument bound at construction
 */
template <typename T, typename R, typename TX, typename T1, typename T2, typename T3, typename T4,typename T5, typename T6, typename T7, typename T8>
class BoundFunctorCallbackImpl : public CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,T8,empty> {
public:
  /**
   * Construct from functor and a bound argument
   * \param functor the functor 
   * \param a the argument to bind
   */
  template <typename FUNCTOR, typename ARG>
  BoundFunctorCallbackImpl (FUNCTOR functor, ARG a)
    : m_functor (functor), m_a (a) {}
  virtual ~BoundFunctorCallbackImpl () {}
  /**
   * Functor with varying numbers of arguments
   * @{
   */
  /** \return Callback value */
  R operator() (void) {
    return m_functor (m_a);
  }
  /**
   * \param a1 first argument
   * \return Callback value
   */
  R operator() (T1 a1) {
    return m_functor (m_a,a1);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2) {
    return m_functor (m_a,a1,a2);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3) {
    return m_functor (m_a,a1,a2,a3);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4) {
    return m_functor (m_a,a1,a2,a3,a4);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5) {
    return m_functor (m_a,a1,a2,a3,a4,a5);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6) {
    return m_functor (m_a,a1,a2,a3,a4,a5,a6);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \param a7 seventh argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6,T7 a7) {
    return m_functor (m_a,a1,a2,a3,a4,a5,a6,a7);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \param a7 seventh argument
   * \param a8 eighth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6,T7 a7,T8 a8) {
    return m_functor (m_a,a1,a2,a3,a4,a5,a6,a7,a8);
  }
  /**@}*/
  /**
   * Equality test.
   *
   * \param other Callback Ptr
   * \return true if we have the same functor and bound arguments
   */
  virtual bool IsEqual (Ptr<const CallbackImplBase> other) const {
    BoundFunctorCallbackImpl<T,R,TX,T1,T2,T3,T4,T5,T6,T7,T8> const *otherDerived = 
      dynamic_cast<BoundFunctorCallbackImpl<T,R,TX,T1,T2,T3,T4,T5,T6,T7,T8> const *> (PeekPointer (other));
    if (otherDerived == 0)
      {
        return false;
      }
    else if (otherDerived->m_functor != m_functor ||
             otherDerived->m_a != m_a)
      {
        return false;
      }
    return true;
  }
private:
  T m_functor;                          //!< The functor
  typename TypeTraits<TX>::ReferencedType m_a;  //!< the bound argument
};

/**
 * \ingroup callback
 * CallbackImpl for functors with first two arguments bound at construction
 */
template <typename T, typename R, typename TX1, typename TX2, typename T1, typename T2, typename T3, typename T4,typename T5, typename T6, typename T7>
class TwoBoundFunctorCallbackImpl : public CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,empty,empty> {
public:
  /**
   * Construct from functor and two arguments
   * \param functor the functor
   * \param arg1 the first argument to bind
   * \param arg2 the second argument to bind
   */
  template <typename FUNCTOR, typename ARG1, typename ARG2>
  TwoBoundFunctorCallbackImpl (FUNCTOR functor, ARG1 arg1, ARG2 arg2)
    : m_functor (functor), m_a1 (arg1), m_a2 (arg2) {}
  virtual ~TwoBoundFunctorCallbackImpl () {}
  /**
   * Functor with varying numbers of arguments
   * @{
   */
  /** \return Callback value */
  R operator() (void) {
    return m_functor (m_a1,m_a2);
  }
  /**
   * \param a1 first argument
   * \return Callback value
   */
  R operator() (T1 a1) {
    return m_functor (m_a1,m_a2,a1);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2) {
    return m_functor (m_a1,m_a2,a1,a2);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3) {
    return m_functor (m_a1,m_a2,a1,a2,a3);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4) {
    return m_functor (m_a1,m_a2,a1,a2,a3,a4);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5) {
    return m_functor (m_a1,m_a2,a1,a2,a3,a4,a5);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6) {
    return m_functor (m_a1,m_a2,a1,a2,a3,a4,a5,a6);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \param a7 seventh argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6,T7 a7) {
    return m_functor (m_a1,m_a2,a1,a2,a3,a4,a5,a6,a7);
  }
  /**@}*/
  /**
   * Equality test.
   *
   * \param other Callback Ptr
   * \return true if we have the same functor and bound arguments
   */
  virtual bool IsEqual (Ptr<const CallbackImplBase> other) const {
    TwoBoundFunctorCallbackImpl<T,R,TX1,TX2,T1,T2,T3,T4,T5,T6,T7> const *otherDerived = 
      dynamic_cast<TwoBoundFunctorCallbackImpl<T,R,TX1,TX2,T1,T2,T3,T4,T5,T6,T7> const *> (PeekPointer (other));
    if (otherDerived == 0)
      {
        return false;
      }
    else if (otherDerived->m_functor != m_functor ||
             otherDerived->m_a1 != m_a1 || otherDerived->m_a2 != m_a2)
      {
        return false;
      }
    return true;
  }
private:
  T m_functor;                                    //!< The functor
  typename TypeTraits<TX1>::ReferencedType m_a1;  //!< first bound argument
  typename TypeTraits<TX2>::ReferencedType m_a2;  //!< second bound argument
};

/**
 * \ingroup callback
 * CallbackImpl for functors with first three arguments bound at construction
 */
template <typename T, typename R, typename TX1, typename TX2, typename TX3, typename T1, typename T2, typename T3, typename T4,typename T5, typename T6>
class ThreeBoundFunctorCallbackImpl : public CallbackImpl<R,T1,T2,T3,T4,T5,T6,empty,empty,empty> {
public:
  /**
   * Construct from functor and three arguments
   * \param functor the functor
   * \param arg1 the first argument to bind
   * \param arg2 the second argument to bind
   * \param arg3 the third argument to bind
   */
  template <typename FUNCTOR, typename ARG1, typename ARG2, typename ARG3>
  ThreeBoundFunctorCallbackImpl (FUNCTOR functor, ARG1 arg1, ARG2 arg2, ARG3 arg3)
    : m_functor (functor), m_a1 (arg1), m_a2 (arg2), m_a3 (arg3) {}
  virtual ~ThreeBoundFunctorCallbackImpl () {}
  /**
   * Functor with varying numbers of arguments
   * @{
   */
  /** \return Callback value */
  R operator() (void) {
    return m_functor (m_a1,m_a2,m_a3);
  }
  /**
   * \param a1 first argument
   * \return Callback value
   */
  R operator() (T1 a1) {
    return m_functor (m_a1,m_a2,m_a3,a1);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2) {
    return m_functor (m_a1,m_a2,m_a3,a1,a2);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3) {
    return m_functor (m_a1,m_a2,m_a3,a1,a2,a3);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4) {
    return m_functor (m_a1,m_a2,m_a3,a1,a2,a3,a4);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5) {
    return m_functor (m_a1,m_a2,m_a3,a1,a2,a3,a4,a5);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \return Callback value
   */
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5,T6 a6) {
    return m_functor (m_a1,m_a2,m_a3,a1,a2,a3,a4,a5,a6);
  }
  /**@}*/
  /**
   * Equality test.
   *
   * \param other Callback Ptr
   * \return true if we have the same functor and bound arguments
   */
  virtual bool IsEqual (Ptr<const CallbackImplBase> other) const {
    ThreeBoundFunctorCallbackImpl<T,R,TX1,TX2,TX3,T1,T2,T3,T4,T5,T6> const *otherDerived = 
      dynamic_cast<ThreeBoundFunctorCallbackImpl<T,R,TX1,TX2,TX3,T1,T2,T3,T4,T5,T6> const *> (PeekPointer (other));
    if (otherDerived == 0)
      {
        return false;
      }
    else if (otherDerived->m_functor != m_functor ||
             otherDerived->m_a1 != m_a1 || otherDerived->m_a2 != m_a2 || otherDerived->m_a3 != m_a3)
      {
        return false;
      }
    return true;
  }
private:
  T m_functor;                                    //!< The functor      
  typename TypeTraits<TX1>::ReferencedType m_a1;  //!< first bound argument 
  typename TypeTraits<TX2>::ReferencedType m_a2;  //!< second bound argument
  typename TypeTraits<TX3>::ReferencedType m_a3;  //!< third bound argument
};

/**
 * \ingroup callback
 * Base class for Callback class.
 * Provides pimpl abstraction.
 */
class CallbackBase {
public:
  CallbackBase () : m_impl () {}
  /** \return the impl pointer */
  Ptr<CallbackImplBase> GetImpl (void) const { return m_impl; }
protected:
  /**
   * Construct from a pimpl
   * \param impl the CallbackImplBase Ptr
   */
  CallbackBase (Ptr<CallbackImplBase> impl) : m_impl (impl) {}
  Ptr<CallbackImplBase> m_impl;         //!< the pimpl

  /**
   * \param mangled the mangled string
   * \return the demangled form of mangled
   */
  static std::string Demangle (const std::string& mangled);
};

/**
 * \ingroup callback
 * \brief Callback template class
 *
 * This class template implements the Functor Design Pattern.
 * It is used to declare the type of a Callback:
 *  - the first non-optional template argument represents
 *    the return type of the callback.
 *  - the remaining (optional) template arguments represent
 *    the type of the subsequent arguments to the callback.
 *  - up to nine arguments are supported.
 *
 * Callback instances are built with the \ref MakeCallback
 * template functions. Callback instances have POD semantics:
 * the memory they allocate is managed automatically, without
 * user intervention which allows you to pass around Callback
 * instances by value.
 *
 * Sample code which shows how to use this class template 
 * as well as the function templates \ref MakeCallback :
 * \include src/core/examples/main-callback.cc
 *
 * \internal
 * This code was originally written based on the techniques 
 * described in http://www.codeproject.com/cpp/TTLFunction.asp
 * It was subsequently rewritten to follow the architecture
 * outlined in "Modern C++ Design" by Andrei Alexandrescu in 
 * chapter 5, "Generalized Functors".
 *
 * This code uses:
 *   - default template parameters to saves users from having to
 *     specify empty parameters when the number of parameters
 *     is smaller than the maximum supported number
 *   - the pimpl idiom: the Callback class is passed around by 
 *     value and delegates the crux of the work to its pimpl
 *     pointer.
 *   - two pimpl implementations which derive from CallbackImpl
 *     FunctorCallbackImpl can be used with any functor-type
 *     while MemPtrCallbackImpl can be used with pointers to
 *     member functions.
 *   - a reference list implementation to implement the Callback's
 *     value semantics.
 *
 * This code most notably departs from the alexandrescu 
 * implementation in that it does not use type lists to specify
 * and pass around the types of the callback arguments.
 * Of course, it also does not use copy-destruction semantics
 * and relies on a reference list rather than autoPtr to hold
 * the pointer.
 */
template<typename R, 
         typename T1 = empty, typename T2 = empty, 
         typename T3 = empty, typename T4 = empty,
         typename T5 = empty, typename T6 = empty,
         typename T7 = empty, typename T8 = empty,
         typename T9 = empty>
class Callback : public CallbackBase {
public:
  Callback () {}

  /**
   * Construct a functor call back, supporting operator() calls
   *
   * \param functor the functor to run on this callback
   *
   * \internal
   * There are two dummy args below to ensure that this constructor is
   * always properly disambiguated by the c++ compiler.
   */
  template <typename FUNCTOR>
  Callback (FUNCTOR const &functor, bool, bool) 
    : CallbackBase (Create<FunctorCallbackImpl<FUNCTOR,R,T1,T2,T3,T4,T5,T6,T7,T8,T9> > (functor))
  {}

  /**
   * Construct a member function pointer call back.
   *
   * \param objPtr pointer to the object
   * \param memPtr  pointer to the member function
   */
  template <typename OBJ_PTR, typename MEM_PTR>
  Callback (OBJ_PTR const &objPtr, MEM_PTR memPtr)
    : CallbackBase (Create<MemPtrCallbackImpl<OBJ_PTR,MEM_PTR,R,T1,T2,T3,T4,T5,T6,T7,T8,T9> > (objPtr, memPtr))
  {}

  /**
   * Construct from a CallbackImpl pointer
   *
   * \param impl the CallbackImpl Ptr
   */
  Callback (Ptr<CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> > const &impl)
    : CallbackBase (impl)
  {}

  /**
   * Bind the first arguments
   *
   * \param a argument to bind
   * \return the bound callback
   */
  template <typename T>
  Callback<R,T2,T3,T4,T5,T6,T7,T8,T9> Bind (T a) {
    Ptr<CallbackImpl<R,T2,T3,T4,T5,T6,T7,T8,T9,empty> > impl =
      Ptr<CallbackImpl<R,T2,T3,T4,T5,T6,T7,T8,T9,empty> > (
        new BoundFunctorCallbackImpl<
          Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9>,
          R,T1,T2,T3,T4,T5,T6,T7,T8,T9> (*this, a), false);
    return Callback<R,T2,T3,T4,T5,T6,T7,T8,T9> (impl);
  }

  /**
   * Bind the first two arguments
   *
   * \param a1 first argument to bind
   * \param a2 second argument to bind
   * \return the bound callback
   */
  template <typename TX1, typename TX2>
  Callback<R,T3,T4,T5,T6,T7,T8,T9> TwoBind (TX1 a1, TX2 a2) {
    Ptr<CallbackImpl<R,T3,T4,T5,T6,T7,T8,T9,empty,empty> > impl =
      Ptr<CallbackImpl<R,T3,T4,T5,T6,T7,T8,T9,empty,empty> > (
        new TwoBoundFunctorCallbackImpl<
          Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9>,
          R,T1,T2,T3,T4,T5,T6,T7,T8,T9> (*this, a1, a2), false);
    return Callback<R,T3,T4,T5,T6,T7,T8,T9> (impl);
  }

  /**
   * Bind the first three arguments
   *
   * \param a1 first argument to bind
   * \param a2 second argument to bind
   * \param a3 third argument to bind
   * \return the bound callback
   */
  template <typename TX1, typename TX2, typename TX3>
  Callback<R,T4,T5,T6,T7,T8,T9> ThreeBind (TX1 a1, TX2 a2, TX3 a3) {
    Ptr<CallbackImpl<R,T4,T5,T6,T7,T8,T9,empty,empty,empty> > impl =
      Ptr<CallbackImpl<R,T4,T5,T6,T7,T8,T9,empty,empty,empty> > (
        new ThreeBoundFunctorCallbackImpl<
          Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9>,
          R,T1,T2,T3,T4,T5,T6,T7,T8,T9> (*this, a1, a2, a3), false);
    return Callback<R,T4,T5,T6,T7,T8,T9> (impl);
  }

  /**
   * Check for null implementation
   *
   * \return true if I don't have an implementation
   */
  bool IsNull (void) const {
    return (DoPeekImpl () == 0) ? true : false;
  }
  /** Discard the implementation, set it to null */
  void Nullify (void) {
    m_impl = 0;
  }

  /**
   * Functor with varying numbers of arguments
   * @{
   */
  /** \return Callback value */
  R operator() (void) const {
    return (*(DoPeekImpl ()))();
  }
  /**
   * \param a1 first argument
   * \return Callback value
   */
  R operator() (T1 a1) const {
    return (*(DoPeekImpl ()))(a1);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \return Callback value
   */
  R operator() (T1 a1, T2 a2) const {
    return (*(DoPeekImpl ()))(a1,a2);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \return Callback value
   */
  R operator() (T1 a1, T2 a2, T3 a3) const {
    return (*(DoPeekImpl ()))(a1,a2,a3);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \return Callback value
   */
  R operator() (T1 a1, T2 a2, T3 a3, T4 a4) const {
    return (*(DoPeekImpl ()))(a1,a2,a3,a4);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \return Callback value
   */
  R operator() (T1 a1, T2 a2, T3 a3, T4 a4,T5 a5) const {
    return (*(DoPeekImpl ()))(a1,a2,a3,a4,a5);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \return Callback value
   */
  R operator() (T1 a1, T2 a2, T3 a3, T4 a4,T5 a5,T6 a6) const {
    return (*(DoPeekImpl ()))(a1,a2,a3,a4,a5,a6);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \param a7 seventh argument
   * \return Callback value
   */
  R operator() (T1 a1, T2 a2, T3 a3, T4 a4,T5 a5,T6 a6,T7 a7) const {
    return (*(DoPeekImpl ()))(a1,a2,a3,a4,a5,a6,a7);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \param a7 seventh argument
   * \param a8 seventh argument
   * \return Callback value
   */
  R operator() (T1 a1, T2 a2, T3 a3, T4 a4,T5 a5,T6 a6,T7 a7,T8 a8) const {
    return (*(DoPeekImpl ()))(a1,a2,a3,a4,a5,a6,a7,a8);
  }
  /**
   * \param a1 first argument
   * \param a2 second argument
   * \param a3 third argument
   * \param a4 fourth argument
   * \param a5 fifth argument
   * \param a6 sixth argument
   * \param a7 seventh argument
   * \param a8 eighth argument
   * \param a9 ninth argument
   * \return Callback value
   */
  R operator() (T1 a1, T2 a2, T3 a3, T4 a4,T5 a5,T6 a6,T7 a7,T8 a8, T9 a9) const {
    return (*(DoPeekImpl ()))(a1,a2,a3,a4,a5,a6,a7,a8,a9);
  }
  /**@}*/

  /**
   * Equality test.
   *
   * \param other Callback
   * \return true if we are equal
   */
  bool IsEqual (const CallbackBase &other) const {
    return m_impl->IsEqual (other.GetImpl ());
  }

  /**
   * Check for compatible types
   *
   * \param other Callback Ptr
   * \return true if other can be dynamic_cast to my type
   */
  bool CheckType (const CallbackBase & other) const {
    return DoCheckType (other.GetImpl ());
  }
  /**
   * Adopt the other's implementation, if type compatible
   *
   * \param other Callback
   */
  void Assign (const CallbackBase &other) {
    DoAssign (other.GetImpl ());
  }
private:
  /** \return the pimpl pointer */
  CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> *DoPeekImpl (void) const {
    return static_cast<CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> *> (PeekPointer (m_impl));
  }
  /**
   * Check for compatible types
   *
   * \param other Callback Ptr
   * \return true if other can be dynamic_cast to my type
   */
  bool DoCheckType (Ptr<const CallbackImplBase> other) const {
    if (other != 0 && dynamic_cast<const CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> *> (PeekPointer (other)) != 0)
      {
        return true;
      }
    else if (other == 0)
      {
        return true;
      }
    else
      {
        return false;
      }
  }
  /**
   * Adopt the other's implementation, if type compatible
   *
   * \param other Callback Ptr to adopt from
   */
  void DoAssign (Ptr<const CallbackImplBase> other) {
    if (!DoCheckType (other))
      {
        NS_FATAL_ERROR ("Incompatible types. (feed to \"c++filt -t\" if needed)" << std::endl <<
                        "got=" << Demangle ( typeid (*other).name () ) << std::endl <<
                        "expected=" << Demangle ( typeid (CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> *).name () ));
      }
    m_impl = const_cast<CallbackImplBase *> (PeekPointer (other));
  }
};


/**
 * Inequality test.
 *
 * \param a Callback
 * \param b Callback
 *
 * \return true if the Callbacks are not equal
 */
template <typename R, typename T1, typename T2,
          typename T3, typename T4,
          typename T5, typename T6,
          typename T7, typename T8,
          typename T9>
bool operator != (Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> a, Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> b)
{
  return !a.IsEqual (b);
}

/**
 * \ingroup callback
 * \defgroup makecallbackmemptr MakeCallback from member function pointer
 *
 * Build Callbacks for class method members which take varying numbers of arguments
 * and potentially returning a value.
 */
/**
 * \ingroup makecallbackmemptr
 * @{
 */
/**
 * \param memPtr class method member pointer
 * \param objPtr class instance
 * \return a wrapper Callback
 * 
 * Build Callbacks for class method members which take varying numbers of arguments
 * and potentially returning a value.
 */     
template <typename T, typename OBJ, typename R>
Callback<R> MakeCallback (R (T::*memPtr)(void), OBJ objPtr) {
  return Callback<R> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R>
Callback<R> MakeCallback (R (T::*memPtr)() const, OBJ objPtr) {
  return Callback<R> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1>
Callback<R,T1> MakeCallback (R (T::*memPtr)(T1), OBJ objPtr) {
  return Callback<R,T1> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1>
Callback<R,T1> MakeCallback (R (T::*memPtr)(T1) const, OBJ objPtr) {
  return Callback<R,T1> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2>
Callback<R,T1,T2> MakeCallback (R (T::*memPtr)(T1,T2), OBJ objPtr) {
  return Callback<R,T1,T2> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2>
Callback<R,T1,T2> MakeCallback (R (T::*memPtr)(T1,T2) const, OBJ objPtr) {
  return Callback<R,T1,T2> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1,typename T2, typename T3>
Callback<R,T1,T2,T3> MakeCallback (R (T::*memPtr)(T1,T2,T3), OBJ objPtr) {
  return Callback<R,T1,T2,T3> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1,typename T2, typename T3>
Callback<R,T1,T2,T3> MakeCallback (R (T::*memPtr)(T1,T2,T3) const, OBJ objPtr) {
  return Callback<R,T1,T2,T3> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4>
Callback<R,T1,T2,T3,T4> MakeCallback (R (T::*memPtr)(T1,T2,T3,T4), OBJ objPtr) {
  return Callback<R,T1,T2,T3,T4> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4>
Callback<R,T1,T2,T3,T4> MakeCallback (R (T::*memPtr)(T1,T2,T3,T4) const, OBJ objPtr) {
  return Callback<R,T1,T2,T3,T4> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4,typename T5>
Callback<R,T1,T2,T3,T4,T5> MakeCallback (R (T::*memPtr)(T1,T2,T3,T4,T5), OBJ objPtr) {
  return Callback<R,T1,T2,T3,T4,T5> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4,typename T5>
Callback<R,T1,T2,T3,T4,T5> MakeCallback (R (T::*memPtr)(T1,T2,T3,T4,T5) const, OBJ objPtr) {
  return Callback<R,T1,T2,T3,T4,T5> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4,typename T5,typename T6>
Callback<R,T1,T2,T3,T4,T5,T6> MakeCallback (R (T::*memPtr)(T1,T2,T3,T4,T5,T6), OBJ objPtr) {
  return Callback<R,T1,T2,T3,T4,T5,T6> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4,typename T5, typename T6>
Callback<R,T1,T2,T3,T4,T5,T6> MakeCallback (R (T::*memPtr)(T1,T2,T3,T4,T5,T6) const, OBJ objPtr) {
  return Callback<R,T1,T2,T3,T4,T5,T6> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4,typename T5,typename T6, typename T7>
Callback<R,T1,T2,T3,T4,T5,T6,T7> MakeCallback (R (T::*memPtr)(T1,T2,T3,T4,T5,T6,T7), OBJ objPtr) {
  return Callback<R,T1,T2,T3,T4,T5,T6,T7> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4,typename T5, typename T6, typename T7>
Callback<R,T1,T2,T3,T4,T5,T6,T7> MakeCallback (R (T::*memPtr)(T1,T2,T3,T4,T5,T6,T7) const, OBJ objPtr) {
  return Callback<R,T1,T2,T3,T4,T5,T6,T7> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4,typename T5,typename T6, typename T7, typename T8>
Callback<R,T1,T2,T3,T4,T5,T6,T7,T8> MakeCallback (R (T::*memPtr)(T1,T2,T3,T4,T5,T6,T7,T8), OBJ objPtr) {
  return Callback<R,T1,T2,T3,T4,T5,T6,T7,T8> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4,typename T5, typename T6, typename T7, typename T8>
Callback<R,T1,T2,T3,T4,T5,T6,T7,T8> MakeCallback (R (T::*memPtr)(T1,T2,T3,T4,T5,T6,T7,T8) const, OBJ objPtr) {
  return Callback<R,T1,T2,T3,T4,T5,T6,T7,T8> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4,typename T5,typename T6, typename T7, typename T8, typename T9>
Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> MakeCallback (R (T::*memPtr)(T1,T2,T3,T4,T5,T6,T7,T8,T9), OBJ objPtr) {
  return Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> (objPtr, memPtr);
}
template <typename T, typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4,typename T5, typename T6, typename T7, typename T8, typename T9>
Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> MakeCallback (R (T::*memPtr)(T1,T2,T3,T4,T5,T6,T7,T8,T9) const, OBJ objPtr) {
  return Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> (objPtr, memPtr);
}
/**@}*/

/**
 * \ingroup callback
 * \defgroup makecallbackfnptr MakeCallback from function pointers
 *
 * Build Callbacks for functions which take varying numbers of arguments
 * and potentially returning a value.
 */
/**
 * \ingroup makecallbackfnptr
 * @{
 */
/**
 * \param fnPtr function pointer
 * \return a wrapper Callback
 * 
 * Build Callbacks for functions which take varying numbers of arguments
 * and potentially returning a value.
 */
template <typename R>
Callback<R> MakeCallback (R (*fnPtr)()) {
  return Callback<R> (fnPtr, true, true);
}
template <typename R, typename T1>
Callback<R,T1> MakeCallback (R (*fnPtr)(T1)) {
  return Callback<R,T1> (fnPtr, true, true);
}
template <typename R, typename T1, typename T2>
Callback<R,T1,T2> MakeCallback (R (*fnPtr)(T1,T2)) {
  return Callback<R,T1,T2> (fnPtr, true, true);
}
template <typename R, typename T1, typename T2,typename T3>
Callback<R,T1,T2,T3> MakeCallback (R (*fnPtr)(T1,T2,T3)) {
  return Callback<R,T1,T2,T3> (fnPtr, true, true);
}
template <typename R, typename T1, typename T2,typename T3,typename T4>
Callback<R,T1,T2,T3,T4> MakeCallback (R (*fnPtr)(T1,T2,T3,T4)) {
  return Callback<R,T1,T2,T3,T4> (fnPtr, true, true);
}
template <typename R, typename T1, typename T2,typename T3,typename T4,typename T5>
Callback<R,T1,T2,T3,T4,T5> MakeCallback (R (*fnPtr)(T1,T2,T3,T4,T5)) {
  return Callback<R,T1,T2,T3,T4,T5> (fnPtr, true, true);
}
template <typename R, typename T1, typename T2,typename T3,typename T4,typename T5,typename T6>
Callback<R,T1,T2,T3,T4,T5,T6> MakeCallback (R (*fnPtr)(T1,T2,T3,T4,T5,T6)) {
  return Callback<R,T1,T2,T3,T4,T5,T6> (fnPtr, true, true);
}
template <typename R, typename T1, typename T2,typename T3,typename T4,typename T5,typename T6, typename T7>
Callback<R,T1,T2,T3,T4,T5,T6,T7> MakeCallback (R (*fnPtr)(T1,T2,T3,T4,T5,T6,T7)) {
  return Callback<R,T1,T2,T3,T4,T5,T6,T7> (fnPtr, true, true);
}
template <typename R, typename T1, typename T2,typename T3,typename T4,typename T5,typename T6, typename T7, typename T8>
Callback<R,T1,T2,T3,T4,T5,T6,T7,T8> MakeCallback (R (*fnPtr)(T1,T2,T3,T4,T5,T6,T7,T8)) {
  return Callback<R,T1,T2,T3,T4,T5,T6,T7,T8> (fnPtr, true, true);
}
template <typename R, typename T1, typename T2,typename T3,typename T4,typename T5,typename T6, typename T7, typename T8, typename T9>
Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> MakeCallback (R (*fnPtr)(T1,T2,T3,T4,T5,T6,T7,T8,T9)) {
  return Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> (fnPtr, true, true);
}
/**@}*/

/**
 * \ingroup callback
 * \defgroup makenullcallback MakeCallback with no arguments
 */
/**
 * \ingroup makenullcallback
 * @{
 */
/**
 * \return a wrapper Callback
 *
 * Build null Callbacks which take no arguments,
 * for varying number of template arguments,
 * and potentially returning a value.
 */     
template <typename R>
Callback<R> MakeNullCallback (void) {
  return Callback<R> ();
}
template <typename R, typename T1>
Callback<R,T1> MakeNullCallback (void) {
  return Callback<R,T1> ();
}
template <typename R, typename T1, typename T2>
Callback<R,T1,T2> MakeNullCallback (void) {
  return Callback<R,T1,T2> ();
}
template <typename R, typename T1, typename T2,typename T3>
Callback<R,T1,T2,T3> MakeNullCallback (void) {
  return Callback<R,T1,T2,T3> ();
}
template <typename R, typename T1, typename T2,typename T3,typename T4>
Callback<R,T1,T2,T3,T4> MakeNullCallback (void) {
  return Callback<R,T1,T2,T3,T4> ();
}
template <typename R, typename T1, typename T2,typename T3,typename T4,typename T5>
Callback<R,T1,T2,T3,T4,T5> MakeNullCallback (void) {
  return Callback<R,T1,T2,T3,T4,T5> ();
}
template <typename R, typename T1, typename T2,typename T3,typename T4,typename T5,typename T6>
Callback<R,T1,T2,T3,T4,T5,T6> MakeNullCallback (void) {
  return Callback<R,T1,T2,T3,T4,T5,T6> ();
}
template <typename R, typename T1, typename T2,typename T3,typename T4,typename T5,typename T6, typename T7>
Callback<R,T1,T2,T3,T4,T5,T6,T7> MakeNullCallback (void) {
  return Callback<R,T1,T2,T3,T4,T5,T6,T7> ();
}
template <typename R, typename T1, typename T2,typename T3,typename T4,typename T5,typename T6, typename T7, typename T8>
Callback<R,T1,T2,T3,T4,T5,T6,T7,T8> MakeNullCallback (void) {
  return Callback<R,T1,T2,T3,T4,T5,T6,T7,T8> ();
}
template <typename R, typename T1, typename T2,typename T3,typename T4,typename T5,typename T6, typename T7, typename T8, typename T9>
Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> MakeNullCallback (void) {
  return Callback<R,T1,T2,T3,T4,T5,T6,T7,T8,T9> ();
}
/**@}*/


/**
 * \ingroup callback
 * \defgroup makeboundcallback MakeBoundCallback from functions bound with up to three arguments.
 */
  
/**
 * \ingroup makeboundcallback
 *
 * Build bound Callbacks which take varying numbers of arguments,
 * and potentially returning a value.
 *
 * \internal
 *
 * The following is experimental code. It works but we have
 * not yet determined whether or not it is really useful and whether
 * or not we really want to use it.
 *
 * @{
 */
/**
 * @{
 * \param fnPtr function pointer
 * \param a1 first bound argument
 * \return a bound Callback
 */   
template <typename R, typename TX, typename ARG>
Callback<R> MakeBoundCallback (R (*fnPtr)(TX), ARG a1) {
  Ptr<CallbackImpl<R,empty,empty,empty,empty,empty,empty,empty,empty,empty> > impl =
    Create<BoundFunctorCallbackImpl<R (*)(TX),R,TX,empty,empty,empty,empty,empty,empty,empty,empty> >(fnPtr, a1);
  return Callback<R> (impl);
}
template <typename R, typename TX, typename ARG, 
          typename T1>
Callback<R,T1> MakeBoundCallback (R (*fnPtr)(TX,T1), ARG a1) {
  Ptr<CallbackImpl<R,T1,empty,empty,empty,empty,empty,empty,empty,empty> > impl =
    Create<BoundFunctorCallbackImpl<R (*)(TX,T1),R,TX,T1,empty,empty,empty,empty,empty,empty,empty> > (fnPtr, a1);
  return Callback<R,T1> (impl);
}
template <typename R, typename TX, typename ARG, 
          typename T1, typename T2>
Callback<R,T1,T2> MakeBoundCallback (R (*fnPtr)(TX,T1,T2), ARG a1) {
  Ptr<CallbackImpl<R,T1,T2,empty,empty,empty,empty,empty,empty,empty> > impl =
    Create<BoundFunctorCallbackImpl<R (*)(TX,T1,T2),R,TX,T1,T2,empty,empty,empty,empty,empty,empty> > (fnPtr, a1);
  return Callback<R,T1,T2> (impl);
}
template <typename R, typename TX, typename ARG,
          typename T1, typename T2,typename T3>
Callback<R,T1,T2,T3> MakeBoundCallback (R (*fnPtr)(TX,T1,T2,T3), ARG a1) {
  Ptr<CallbackImpl<R,T1,T2,T3,empty,empty,empty,empty,empty,empty> > impl =
    Create<BoundFunctorCallbackImpl<R (*)(TX,T1,T2,T3),R,TX,T1,T2,T3,empty,empty,empty,empty,empty> > (fnPtr, a1);
  return Callback<R,T1,T2,T3> (impl);
}
template <typename R, typename TX, typename ARG,
          typename T1, typename T2,typename T3,typename T4>
Callback<R,T1,T2,T3,T4> MakeBoundCallback (R (*fnPtr)(TX,T1,T2,T3,T4), ARG a1) {
  Ptr<CallbackImpl<R,T1,T2,T3,T4,empty,empty,empty,empty,empty> > impl =
    Create<BoundFunctorCallbackImpl<R (*)(TX,T1,T2,T3,T4),R,TX,T1,T2,T3,T4,empty,empty,empty,empty> > (fnPtr, a1);
  return Callback<R,T1,T2,T3,T4> (impl);
}
template <typename R, typename TX, typename ARG,
          typename T1, typename T2,typename T3,typename T4,typename T5>
Callback<R,T1,T2,T3,T4,T5> MakeBoundCallback (R (*fnPtr)(TX,T1,T2,T3,T4,T5), ARG a1) {
  Ptr<CallbackImpl<R,T1,T2,T3,T4,T5,empty,empty,empty,empty> > impl =
    Create<BoundFunctorCallbackImpl<R (*)(TX,T1,T2,T3,T4,T5),R,TX,T1,T2,T3,T4,T5,empty,empty,empty> > (fnPtr, a1);
  return Callback<R,T1,T2,T3,T4,T5> (impl);
}
template <typename R, typename TX, typename ARG,
          typename T1, typename T2,typename T3,typename T4,typename T5, typename T6>
Callback<R,T1,T2,T3,T4,T5,T6> MakeBoundCallback (R (*fnPtr)(TX,T1,T2,T3,T4,T5,T6), ARG a1) {
  Ptr<CallbackImpl<R,T1,T2,T3,T4,T5,T6,empty,empty,empty> > impl =
    Create<BoundFunctorCallbackImpl<R (*)(TX,T1,T2,T3,T4,T5,T6),R,TX,T1,T2,T3,T4,T5,T6,empty,empty> > (fnPtr, a1);
  return Callback<R,T1,T2,T3,T4,T5,T6> (impl);
}
template <typename R, typename TX, typename ARG,
          typename T1, typename T2,typename T3,typename T4,typename T5, typename T6, typename T7>
Callback<R,T1,T2,T3,T4,T5,T6,T7> MakeBoundCallback (R (*fnPtr)(TX,T1,T2,T3,T4,T5,T6,T7), ARG a1) {
  Ptr<CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,empty,empty> > impl =
    Create<BoundFunctorCallbackImpl<R (*)(TX,T1,T2,T3,T4,T5,T6,T7),R,TX,T1,T2,T3,T4,T5,T6,T7,empty> > (fnPtr, a1);
  return Callback<R,T1,T2,T3,T4,T5,T6,T7> (impl);
}
template <typename R, typename TX, typename ARG,
          typename T1, typename T2,typename T3,typename T4,typename T5, typename T6, typename T7, typename T8>
Callback<R,T1,T2,T3,T4,T5,T6,T7,T8> MakeBoundCallback (R (*fnPtr)(TX,T1,T2,T3,T4,T5,T6,T7,T8), ARG a1) {
  Ptr<CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,T8,empty> > impl =
    Create<BoundFunctorCallbackImpl<R (*)(TX,T1,T2,T3,T4,T5,T6,T7,T8),R,TX,T1,T2,T3,T4,T5,T6,T7,T8> > (fnPtr, a1);
  return Callback<R,T1,T2,T3,T4,T5,T6,T7,T8> (impl);
}
/**@}*/

/**
 * \param fnPtr function pointer
 * \param a1 first bound argument
 * \param a2 second bound argument 
 * \return a bound Callback
 * @{
 */
template <typename R, typename TX1, typename TX2, typename ARG1, typename ARG2>
Callback<R> MakeBoundCallback (R (*fnPtr)(TX1,TX2), ARG1 a1, ARG2 a2) {
  Ptr<CallbackImpl<R,empty,empty,empty,empty,empty,empty,empty,empty,empty> > impl =
    Create<TwoBoundFunctorCallbackImpl<R (*)(TX1,TX2),R,TX1,TX2,empty,empty,empty,empty,empty,empty,empty> >(fnPtr, a1, a2);
  return Callback<R> (impl);
}
template <typename R, typename TX1, typename TX2, typename ARG1, typename ARG2,
          typename T1>
Callback<R,T1> MakeBoundCallback (R (*fnPtr)(TX1,TX2,T1), ARG1 a1, ARG2 a2) {
  Ptr<CallbackImpl<R,T1,empty,empty,empty,empty,empty,empty,empty,empty> > impl =
    Create<TwoBoundFunctorCallbackImpl<R (*)(TX1,TX2,T1),R,TX1,TX2,T1,empty,empty,empty,empty,empty,empty> > (fnPtr, a1, a2);
  return Callback<R,T1> (impl);
}
template <typename R, typename TX1, typename TX2, typename ARG1, typename ARG2,
          typename T1, typename T2>
Callback<R,T1,T2> MakeBoundCallback (R (*fnPtr)(TX1,TX2,T1,T2), ARG1 a1, ARG2 a2) {
  Ptr<CallbackImpl<R,T1,T2,empty,empty,empty,empty,empty,empty,empty> > impl =
    Create<TwoBoundFunctorCallbackImpl<R (*)(TX1,TX2,T1,T2),R,TX1,TX2,T1,T2,empty,empty,empty,empty,empty> > (fnPtr, a1, a2);
  return Callback<R,T1,T2> (impl);
}
template <typename R, typename TX1, typename TX2, typename ARG1, typename ARG2,
          typename T1, typename T2,typename T3>
Callback<R,T1,T2,T3> MakeBoundCallback (R (*fnPtr)(TX1,TX2,T1,T2,T3), ARG1 a1, ARG2 a2) {
  Ptr<CallbackImpl<R,T1,T2,T3,empty,empty,empty,empty,empty,empty> > impl =
    Create<TwoBoundFunctorCallbackImpl<R (*)(TX1,TX2,T1,T2,T3),R,TX1,TX2,T1,T2,T3,empty,empty,empty,empty> > (fnPtr, a1, a2);
  return Callback<R,T1,T2,T3> (impl);
}
template <typename R, typename TX1, typename TX2, typename ARG1, typename ARG2,
          typename T1, typename T2,typename T3,typename T4>
Callback<R,T1,T2,T3,T4> MakeBoundCallback (R (*fnPtr)(TX1,TX2,T1,T2,T3,T4), ARG1 a1, ARG2 a2) {
  Ptr<CallbackImpl<R,T1,T2,T3,T4,empty,empty,empty,empty,empty> > impl =
    Create<TwoBoundFunctorCallbackImpl<R (*)(TX1,TX2,T1,T2,T3,T4),R,TX1,TX2,T1,T2,T3,T4,empty,empty,empty> > (fnPtr, a1, a2);
  return Callback<R,T1,T2,T3,T4> (impl);
}
template <typename R, typename TX1, typename TX2, typename ARG1, typename ARG2,
          typename T1, typename T2,typename T3,typename T4,typename T5>
Callback<R,T1,T2,T3,T4,T5> MakeBoundCallback (R (*fnPtr)(TX1,TX2,T1,T2,T3,T4,T5), ARG1 a1, ARG2 a2) {
  Ptr<CallbackImpl<R,T1,T2,T3,T4,T5,empty,empty,empty,empty> > impl =
    Create<TwoBoundFunctorCallbackImpl<R (*)(TX1,TX2,T1,T2,T3,T4,T5),R,TX1,TX2,T1,T2,T3,T4,T5,empty,empty> > (fnPtr, a1, a2);
  return Callback<R,T1,T2,T3,T4,T5> (impl);
}
template <typename R, typename TX1, typename TX2, typename ARG1, typename ARG2,
          typename T1, typename T2,typename T3,typename T4,typename T5, typename T6>
Callback<R,T1,T2,T3,T4,T5,T6> MakeBoundCallback (R (*fnPtr)(TX1,TX2,T1,T2,T3,T4,T5,T6), ARG1 a1, ARG2 a2) {
  Ptr<CallbackImpl<R,T1,T2,T3,T4,T5,T6,empty,empty,empty> > impl =
    Create<TwoBoundFunctorCallbackImpl<R (*)(TX1,TX2,T1,T2,T3,T4,T5,T6),R,TX1,TX2,T1,T2,T3,T4,T5,T6,empty> > (fnPtr, a1, a2);
  return Callback<R,T1,T2,T3,T4,T5,T6> (impl);
}
template <typename R, typename TX1, typename TX2, typename ARG1, typename ARG2,
          typename T1, typename T2,typename T3,typename T4,typename T5, typename T6, typename T7>
Callback<R,T1,T2,T3,T4,T5,T6,T7> MakeBoundCallback (R (*fnPtr)(TX1,TX2,T1,T2,T3,T4,T5,T6,T7), ARG1 a1, ARG2 a2) {
  Ptr<CallbackImpl<R,T1,T2,T3,T4,T5,T6,T7,empty,empty> > impl =
    Create<TwoBoundFunctorCallbackImpl<R (*)(TX1,TX2,T1,T2,T3,T4,T5,T6,T7),R,TX1,TX2,T1,T2,T3,T4,T5,T6,T7> > (fnPtr, a1, a2);
  return Callback<R,T1,T2,T3,T4,T5,T6,T7> (impl);
}
/**@}*/

/**
 * \param a1 first bound argument
 * \param a2 second bound argument 
 * \param a3 third bound argument 
 * \param fnPtr function pointer
 * \return a bound Callback
 * @{
 */
template <typename R, typename TX1, typename TX2, typename TX3, typename ARG1, typename ARG2, typename ARG3>
Callback<R> MakeBoundCallback (R (*fnPtr)(TX1,TX2,TX3), ARG1 a1, ARG2 a2, ARG3 a3) {
  Ptr<CallbackImpl<R,empty,empty,empty,empty,empty,empty,empty,empty,empty> > impl =
    Create<ThreeBoundFunctorCallbackImpl<R (*)(TX1,TX2,TX3),R,TX1,TX2,TX3,empty,empty,empty,empty,empty,empty> >(fnPtr, a1, a2, a3);
  return Callback<R> (impl);
}
template <typename R, typename TX1, typename TX2, typename TX3, typename ARG1, typename ARG2, typename ARG3,
          typename T1>
Callback<R,T1> MakeBoundCallback (R (*fnPtr)(TX1,TX2,TX3,T1), ARG1 a1, ARG2 a2, ARG3 a3) {
  Ptr<CallbackImpl<R,T1,empty,empty,empty,empty,empty,empty,empty,empty> > impl =
    Create<ThreeBoundFunctorCallbackImpl<R (*)(TX1,TX2,TX3,T1),R,TX1,TX2,TX3,T1,empty,empty,empty,empty,empty> > (fnPtr, a1, a2, a3);
  return Callback<R,T1> (impl);
}
template <typename R, typename TX1, typename TX2, typename TX3, typename ARG1, typename ARG2, typename ARG3,
          typename T1, typename T2>
Callback<R,T1,T2> MakeBoundCallback (R (*fnPtr)(TX1,TX2,TX3,T1,T2), ARG1 a1, ARG2 a2, ARG3 a3) {
  Ptr<CallbackImpl<R,T1,T2,empty,empty,empty,empty,empty,empty,empty> > impl =
    Create<ThreeBoundFunctorCallbackImpl<R (*)(TX1,TX2,TX3,T1,T2),R,TX1,TX2,TX3,T1,T2,empty,empty,empty,empty> > (fnPtr, a1, a2, a3);
  return Callback<R,T1,T2> (impl);
}
template <typename R, typename TX1, typename TX2, typename TX3, typename ARG1, typename ARG2, typename ARG3,
          typename T1, typename T2,typename T3>
Callback<R,T1,T2,T3> MakeBoundCallback (R (*fnPtr)(TX1,TX2,TX3,T1,T2,T3), ARG1 a1, ARG2 a2, ARG3 a3) {
  Ptr<CallbackImpl<R,T1,T2,T3,empty,empty,empty,empty,empty,empty> > impl =
    Create<ThreeBoundFunctorCallbackImpl<R (*)(TX1,TX2,TX3,T1,T2,T3),R,TX1,TX2,TX3,T1,T2,T3,empty,empty,empty> > (fnPtr, a1, a2, a3);
  return Callback<R,T1,T2,T3> (impl);
}
template <typename R, typename TX1, typename TX2, typename TX3, typename ARG1, typename ARG2, typename ARG3,
          typename T1, typename T2,typename T3,typename T4>
Callback<R,T1,T2,T3,T4> MakeBoundCallback (R (*fnPtr)(TX1,TX2,TX3,T1,T2,T3,T4), ARG1 a1, ARG2 a2, ARG3 a3) {
  Ptr<CallbackImpl<R,T1,T2,T3,T4,empty,empty,empty,empty,empty> > impl =
    Create<ThreeBoundFunctorCallbackImpl<R (*)(TX1,TX2,TX3,T1,T2,T3,T4),R,TX1,TX2,TX3,T1,T2,T3,T4,empty,empty> > (fnPtr, a1, a2, a3);
  return Callback<R,T1,T2,T3,T4> (impl);
}
template <typename R, typename TX1, typename TX2, typename TX3, typename ARG1, typename ARG2, typename ARG3,
          typename T1, typename T2,typename T3,typename T4,typename T5>
Callback<R,T1,T2,T3,T4,T5> MakeBoundCallback (R (*fnPtr)(TX1,TX2,TX3,T1,T2,T3,T4,T5), ARG1 a1, ARG2 a2, ARG3 a3) {
  Ptr<CallbackImpl<R,T1,T2,T3,T4,T5,empty,empty,empty,empty> > impl =
    Create<ThreeBoundFunctorCallbackImpl<R (*)(TX1,TX2,TX3,T1,T2,T3,T4,T5),R,TX1,TX2,TX3,T1,T2,T3,T4,T5,empty> > (fnPtr, a1, a2, a3);
  return Callback<R,T1,T2,T3,T4,T5> (impl);
}
template <typename R, typename TX1, typename TX2, typename TX3, typename ARG1, typename ARG2, typename ARG3,
          typename T1, typename T2,typename T3,typename T4,typename T5, typename T6>
Callback<R,T1,T2,T3,T4,T5,T6> MakeBoundCallback (R (*fnPtr)(TX1,TX2,TX3,T1,T2,T3,T4,T5,T6), ARG1 a1, ARG2 a2, ARG3 a3) {
  Ptr<CallbackImpl<R,T1,T2,T3,T4,T5,T6,empty,empty,empty> > impl =
    Create<ThreeBoundFunctorCallbackImpl<R (*)(TX1,TX2,TX3,T1,T2,T3,T4,T5,T6),R,TX1,TX2,TX3,T1,T2,T3,T4,T5,T6> > (fnPtr, a1, a2, a3);
  return Callback<R,T1,T2,T3,T4,T5,T6> (impl);
}
/**@}*/

/**@}*/

} // namespace ns3

namespace ns3 {

/**
 * \ingroup callback
 * AttributeValue form of a Callback
 */
class CallbackValue : public AttributeValue
{
public:
  CallbackValue ();
  /**
   * Copy constructor
   * \param base Callback to copy
   */
  CallbackValue (const CallbackBase &base);  
  virtual ~CallbackValue ();
  /** \param base the Callbackbase to use */
  void Set (CallbackBase base);
  /**
   * Give value my callback, if type compatible
   *
   * \param value destination callback
   * \returns true if successful
   */
  template <typename T>
  bool GetAccessor (T &value) const;
  /** \return a copy of this CallBack */
  virtual Ptr<AttributeValue> Copy (void) const;
  /**
   * Serialize to string
   * \param checker the checker to validate with
   * \return serialize this pimpl
   */
  virtual std::string SerializeToString (Ptr<const AttributeChecker> checker) const;
  /**
   * Deserialize from string (not implemented)
   *
   * \param value source string
   * \param checker checker to validate with
   * \return true if successful
   */
  virtual bool DeserializeFromString (std::string value, Ptr<const AttributeChecker> checker);
private:
  CallbackBase m_value;                 //!< the CallbackBase
};

/** Attribute helpers @{ */
ATTRIBUTE_ACCESSOR_DEFINE (Callback);
ATTRIBUTE_CHECKER_DEFINE (Callback);
/**@}*/

} // namespace ns3

namespace ns3 {

template <typename T>
bool CallbackValue::GetAccessor (T &value) const
{
  if (value.CheckType (m_value))
    {
      value.Assign (m_value);
      return true;
    }
  return false;
}

} // namespace ns3


#endif /* CALLBACK_H */
