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

#ifndef CALLBACK_H
#define CALLBACK_H

#include "reference-list.h"

namespace ns3 {

/***
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
class empty {};

class CallbackImplBase {
public:
  virtual ~CallbackImplBase () {}
  virtual bool IsEqual (CallbackImplBase const *other) const = 0;
};

// declare the CallbackImpl class
template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
class CallbackImpl;
// define CallbackImpl for 0 params
template <typename R>
class CallbackImpl<R,empty,empty,empty,empty,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (void) = 0;
};
// define CallbackImpl for 1 params
template <typename R, typename T1>
class CallbackImpl<R,T1,empty,empty,empty,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1) = 0;
};
// define CallbackImpl for 2 params
template <typename R, typename T1, typename T2>
class CallbackImpl<R,T1,T2,empty,empty,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1, T2) = 0;
};
// define CallbackImpl for 3 params
template <typename R, typename T1, typename T2, typename T3>
class CallbackImpl<R,T1,T2,T3,empty,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1, T2, T3) = 0;
};
// define CallbackImpl for 4 params
template <typename R, typename T1, typename T2, typename T3, typename T4>
class CallbackImpl<R,T1,T2,T3,T4,empty> : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1, T2, T3, T4) = 0;
};
// define CallbackImpl for 5 params
template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
class CallbackImpl : public CallbackImplBase {
public:
  virtual ~CallbackImpl () {}
  virtual R operator() (T1, T2, T3, T4, T5) = 0;
};


// an impl for Functors:
template <typename T, typename R, typename T1, typename T2, typename T3, typename T4,typename T5>
class FunctorCallbackImpl : public CallbackImpl<R,T1,T2,T3,T4,T5> {
public:
  FunctorCallbackImpl (T const &functor)
      : m_functor (functor) {}
  virtual ~FunctorCallbackImpl () {}
  R operator() (void) {
      return m_functor ();
  }
  R operator() (T1 a1) {
      return m_functor (a1);
  }
  R operator() (T1 a1,T2 a2) {
      return m_functor (a1,a2);
  }
  R operator() (T1 a1,T2 a2,T3 a3) {
      return m_functor (a1,a2,a3);
  }
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4) {
      return m_functor (a1,a2,a3,a4);
  }
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5) {
      return m_functor (a1,a2,a3,a4,a5);
  }
  virtual bool IsEqual (CallbackImplBase const *other) const {
    FunctorCallbackImpl<T,R,T1,T2,T3,T4,T5> const *otherDerived = 
      dynamic_cast<FunctorCallbackImpl<T,R,T1,T2,T3,T4,T5> const *> (other);
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
  T m_functor;
};

// an impl for pointer to member functions
template <typename OBJ_PTR, typename MEM_PTR, typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
class MemPtrCallbackImpl : public CallbackImpl<R,T1,T2,T3,T4,T5> {
public:
  MemPtrCallbackImpl (OBJ_PTR const&objPtr, MEM_PTR mem_ptr)
      : m_objPtr (objPtr), m_memPtr (mem_ptr) {}
  virtual ~MemPtrCallbackImpl () {}
  R operator() (void) {
      return ((*m_objPtr).*m_memPtr) ();
  }
  R operator() (T1 a1) {
      return ((*m_objPtr).*m_memPtr) (a1);
  }
  R operator() (T1 a1,T2 a2) {
      return ((*m_objPtr).*m_memPtr) (a1,a2);
  }
  R operator() (T1 a1,T2 a2,T3 a3) {
      return ((*m_objPtr).*m_memPtr) (a1,a2,a3);
  }
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4) {
      return ((*m_objPtr).*m_memPtr) (a1,a2,a3,a4);
  }
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5) {
      return ((*m_objPtr).*m_memPtr) (a1,a2,a3,a4,a5);
  }
  virtual bool IsEqual (CallbackImplBase const *other) const {
    MemPtrCallbackImpl<OBJ_PTR,MEM_PTR,R,T1,T2,T3,T4,T5> const *otherDerived = 
      dynamic_cast<MemPtrCallbackImpl<OBJ_PTR,MEM_PTR,R,T1,T2,T3,T4,T5> const *> (other);
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
  OBJ_PTR const m_objPtr;
  MEM_PTR m_memPtr;
};

class CallbackBase {
public:
  virtual ~CallbackBase () {}
  virtual CallbackImplBase *PeekImpl (void) const = 0;
};

/**
 * \brief Callback template class
 *
 * This class template implements the Functor Design Pattern.
 * It is used to declare the type of a Callback:
 *  - the first non-optional template argument represents
 *    the return type of the callback.
 *  - the second optional template argument represents
 *    the type of the first argument to the callback.
 *  - the third optional template argument represents
 *    the type of the second argument to the callback.
 *  - the fourth optional template argument represents
 *    the type of the third argument to the callback.
 *  - the fifth optional template argument represents
 *    the type of the fourth argument to the callback.
 *  - the sixth optional template argument represents
 *    the type of the fifth argument to the callback.
 *
 * Callback instances are built with the \ref MakeCallback
 * template functions. Callback instances have POD semantics:
 * the memory they allocate is managed automatically, without
 * user intervention which allows you to pass around Callback
 * instances by value.
 *
 * Sample code which shows how to use this class template 
 * as well as the function templates \ref MakeCallback :
 * \include samples/main-callback.cc
 */

template<typename R, 
   typename T1 = empty, typename T2 = empty, 
   typename T3 = empty, typename T4 = empty,
   typename T5 = empty>
class Callback : public CallbackBase {
public:
  // There are two dummy args below to ensure that this constructor is
  // always properly disambiguited by the c++ compiler
  template <typename FUNCTOR>
  Callback (FUNCTOR const &functor, bool, bool) 
      : m_impl (new FunctorCallbackImpl<FUNCTOR,R,T1,T2,T3,T4,T5> (functor))
  {}

  template <typename OBJ_PTR, typename MEM_PTR>
  Callback (OBJ_PTR const &objPtr, MEM_PTR mem_ptr)
      : m_impl (new MemPtrCallbackImpl<OBJ_PTR,MEM_PTR,R,T1,T2,T3,T4,T5> (objPtr, mem_ptr))
  {}

  Callback (ReferenceList<CallbackImpl<R,T1,T2,T3,T4,T5> *> const &impl)
      : m_impl (impl)
  {}

  bool IsNull (void) {
      return (m_impl.Get () == 0)?true:false;
  }

  Callback () : m_impl () {}
  R operator() (void) const {
      return (*(m_impl.Get ())) ();
  }
  R operator() (T1 a1) const {
      return (*(m_impl.Get ())) (a1);
  }
  R operator() (T1 a1, T2 a2) const {
      return (*(m_impl).Get ()) (a1,a2);
  }
  R operator() (T1 a1, T2 a2, T3 a3) const {
      return (*(m_impl).Get ()) (a1,a2,a3);
  }
  R operator() (T1 a1, T2 a2, T3 a3, T4 a4) const {
      return (*(m_impl).Get ()) (a1,a2,a3,a4);
  }
  R operator() (T1 a1, T2 a2, T3 a3, T4 a4,T5 a5) const {
      return (*(m_impl).Get ()) (a1,a2,a3,a4,a5);
  }

  bool IsEqual (CallbackBase const &other) {
    return PeekImpl ()->IsEqual (other.PeekImpl ());
  }

  bool CheckType (CallbackBase const& other) {
    CallbackImplBase *otherBase = other.PeekImpl ();
    if (dynamic_cast<CallbackImpl<R,T1,T2,T3,T4,T5> *> (otherBase) != 0)
      {
        return true;
      }
    else
      {
        return false;
      }
  }
private:
  virtual CallbackImplBase *PeekImpl (void) const {
    return m_impl.Get ();
  }
  ReferenceList<CallbackImpl<R,T1,T2,T3,T4,T5>*> m_impl;
};

/**
 * \defgroup MakeCallback MakeCallback
 *
 */

/**
 * \ingroup MakeCallback
 * \param mem_ptr class method member pointer
 * \param objPtr class instance
 * \return a wrapper Callback
 * Build Callbacks for class method members which takes no arguments
 * and potentially return a value.
 */
template <typename OBJ, typename R>
Callback<R> MakeCallback (R (OBJ::*mem_ptr) (), OBJ *const objPtr) {
  return Callback<R> (objPtr, mem_ptr);
}
/**
 * \ingroup MakeCallback
 * \param mem_ptr class method member pointer
 * \param objPtr class instance
 * \return a wrapper Callback
 * Build Callbacks for class method members which takes one argument
 * and potentially return a value.
 */
template <typename OBJ, typename R, typename T1>
Callback<R,T1> MakeCallback (R (OBJ::*mem_ptr) (T1), OBJ *const objPtr) {
  return Callback<R,T1> (objPtr, mem_ptr);
}
/**
 * \ingroup MakeCallback
 * \param mem_ptr class method member pointer
 * \param objPtr class instance
 * \return a wrapper Callback
 * Build Callbacks for class method members which takes two arguments
 * and potentially return a value.
 */
template <typename OBJ, typename R, typename T1, typename T2>
Callback<R,T1,T2> MakeCallback (R (OBJ::*mem_ptr) (T1,T2), OBJ *const objPtr) {
  return Callback<R,T1,T2> (objPtr, mem_ptr);
}
/**
 * \ingroup MakeCallback
 * \param mem_ptr class method member pointer
 * \param objPtr class instance
 * \return a wrapper Callback
 * Build Callbacks for class method members which takes three arguments
 * and potentially return a value.
 */
template <typename OBJ, typename R, typename T1,typename T2, typename T3>
Callback<R,T1,T2,T3> MakeCallback (R (OBJ::*mem_ptr) (T1,T2,T3), OBJ *const objPtr) {
  return Callback<R,T1,T2,T3> (objPtr, mem_ptr);
}
/**
 * \ingroup MakeCallback
 * \param mem_ptr class method member pointer
 * \param objPtr class instance
 * \return a wrapper Callback
 * Build Callbacks for class method members which takes four arguments
 * and potentially return a value.
 */
template <typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4>
Callback<R,T1,T2,T3,T4> MakeCallback (R (OBJ::*mem_ptr) (T1,T2,T3,T4), OBJ *const objPtr) {
  return Callback<R,T1,T2,T3,T4> (objPtr, mem_ptr);
}
/**
 * \ingroup MakeCallback
 * \param mem_ptr class method member pointer
 * \param objPtr class instance
 * \return a wrapper Callback
 * Build Callbacks for class method members which takes five arguments
 * and potentially return a value.
 */
template <typename OBJ, typename R, typename T1, typename T2, typename T3, typename T4,typename T5>
Callback<R,T1,T2,T3,T4,T5> MakeCallback (R (OBJ::*mem_ptr) (T1,T2,T3,T4,T5), OBJ *const objPtr) {
  return Callback<R,T1,T2,T3,T4,T5> (objPtr, mem_ptr);
}

/**
 * \ingroup MakeCallback
 * \param fnPtr function pointer
 * \return a wrapper Callback
 * Build Callbacks for functions which takes no arguments
 * and potentially return a value.
 */
template <typename R>
Callback<R> MakeCallback (R (*fnPtr) ()) {
  return Callback<R> (fnPtr, true, true);
}
/**
 * \ingroup MakeCallback
 * \param fnPtr function pointer
 * \return a wrapper Callback
 * Build Callbacks for functions which takes one argument
 * and potentially return a value.
 */
template <typename R, typename T1>
Callback<R,T1> MakeCallback (R (*fnPtr) (T1)) {
  return Callback<R,T1> (fnPtr, true, true);
}
/**
 * \ingroup MakeCallback
 * \param fnPtr function pointer
 * \return a wrapper Callback
 * Build Callbacks for functions which takes two arguments
 * and potentially return a value.
 */
template <typename R, typename T1, typename T2>
Callback<R,T1,T2> MakeCallback (R (*fnPtr) (T1,T2)) {
  return Callback<R,T1,T2> (fnPtr, true, true);
}
/**
 * \ingroup MakeCallback
 * \param fnPtr function pointer
 * \return a wrapper Callback
 * Build Callbacks for functions which takes three arguments
 * and potentially return a value.
 */
template <typename R, typename T1, typename T2,typename T3>
Callback<R,T1,T2,T3> MakeCallback (R (*fnPtr) (T1,T2,T3)) {
  return Callback<R,T1,T2,T3> (fnPtr, true, true);
}
/**
 * \ingroup MakeCallback
 * \param fnPtr function pointer
 * \return a wrapper Callback
 * Build Callbacks for functions which takes four arguments
 * and potentially return a value.
 */
template <typename R, typename T1, typename T2,typename T3,typename T4>
Callback<R,T1,T2,T3,T4> MakeCallback (R (*fnPtr) (T1,T2,T3,T4)) {
  return Callback<R,T1,T2,T3,T4> (fnPtr, true, true);
}
/**
 * \ingroup MakeCallback
 * \param fnPtr function pointer
 * \return a wrapper Callback
 * Build Callbacks for functions which takes five arguments
 * and potentially return a value.
 */
template <typename R, typename T1, typename T2,typename T3,typename T4,typename T5>
Callback<R,T1,T2,T3,T4,T5> MakeCallback (R (*fnPtr) (T1,T2,T3,T4,T5)) {
  return Callback<R,T1,T2,T3,T4,T5> (fnPtr, true, true);
}



/**
 * \ingroup MakeCallback
 * \return a wrapper Callback
 * Build a null callback which takes no arguments
 * and potentially return a value.
 */
template <typename R>
Callback<R> MakeNullCallback (void) {
  return Callback<R> ();
}
/**
 * \ingroup MakeCallback
 * \overload Callback<R> MakeNullCallback (void)
 * \return a wrapper Callback
 * Build a null callback which takes one argument
 * and potentially return a value.
 */
template <typename R, typename T1>
Callback<R,T1> MakeNullCallback (void) {
  return Callback<R,T1> ();
}
/**
 * \ingroup MakeCallback
 * \overload Callback<R> MakeNullCallback (void)
 * \return a wrapper Callback
 * Build a null callback which takes two arguments
 * and potentially return a value.
 */
template <typename R, typename T1, typename T2>
Callback<R,T1,T2> MakeNullCallback (void) {
  return Callback<R,T1,T2> ();
}
/**
 * \ingroup MakeCallback
 * \overload Callback<R> MakeNullCallback (void)
 * \return a wrapper Callback
 * Build a null callback which takes three arguments
 * and potentially return a value.
 */
template <typename R, typename T1, typename T2,typename T3>
Callback<R,T1,T2,T3> MakeNullCallback (void) {
  return Callback<R,T1,T2,T3> ();
}
/**
 * \ingroup MakeCallback
 * \overload Callback<R> MakeNullCallback (void)
 * \return a wrapper Callback
 * Build a null callback which takes four arguments
 * and potentially return a value.
 */
template <typename R, typename T1, typename T2,typename T3,typename T4>
Callback<R,T1,T2,T3,T4> MakeNullCallback (void) {
  return Callback<R,T1,T2,T3,T4> ();
}
/**
 * \ingroup MakeCallback
 * \overload Callback<R> MakeNullCallback (void)
 * \return a wrapper Callback
 * Build a null callback which takes five arguments
 * and potentially return a value.
 */
template <typename R, typename T1, typename T2,typename T3,typename T4,typename T5>
Callback<R,T1,T2,T3,T4,T5> MakeNullCallback (void) {
  return Callback<R,T1,T2,T3,T4,T5> ();
}


/**
 * The following is experimental code. It works but we have
 * not yet determined whether or not it is really useful and whether
 * or not we really want to use it.
 */
// an impl for Bound Functors:
template <typename T, typename R, typename TX, typename T1, typename T2, typename T3, typename T4,typename T5>
class BoundFunctorCallbackImpl : public CallbackImpl<R,T1,T2,T3,T4,T5> {
public:
  BoundFunctorCallbackImpl (T const &functor, TX a)
      : m_functor (functor), m_a (a) {}
  virtual ~BoundFunctorCallbackImpl () {}
  R operator() (void) {
      return m_functor (m_a);
  }
  R operator() (T1 a1) {
      return m_functor (m_a,a1);
  }
  R operator() (T1 a1,T2 a2) {
      return m_functor (m_a,a1,a2);
  }
  R operator() (T1 a1,T2 a2,T3 a3) {
      return m_functor (m_a,a1,a2,a3);
  }
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4) {
      return m_functor (m_a,a1,a2,a3,a4);
  }
  R operator() (T1 a1,T2 a2,T3 a3,T4 a4,T5 a5) {
      return m_functor (m_a,a1,a2,a3,a4,a5);
  }
  virtual bool IsEqual (CallbackImplBase const *other) const {
    BoundFunctorCallbackImpl<T,R,TX,T1,T2,T3,T4,T5> const *otherDerived = 
      dynamic_cast<BoundFunctorCallbackImpl<T,R,TX,T1,T2,T3,T4,T5> const *> (other);
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
  T m_functor;
  TX m_a;
};

template <typename R, typename TX>
Callback<R> MakeBoundCallback (R (*fnPtr) (TX), TX a) {
  ReferenceList<CallbackImpl<R,empty,empty,empty,empty,empty>*> impl =
  ReferenceList<CallbackImpl<R,empty,empty,empty,empty,empty>*> (
  new BoundFunctorCallbackImpl<R (*) (TX),R,TX,empty,empty,empty,empty,empty> (fnPtr, a)
  );
  return Callback<R> (impl);
}

template <typename R, typename TX, typename T1>
Callback<R,T1> MakeBoundCallback (R (*fnPtr) (TX,T1), TX a) {
  ReferenceList<CallbackImpl<R,T1,empty,empty,empty,empty>*> impl =
  ReferenceList<CallbackImpl<R,T1,empty,empty,empty,empty>*> (
  new BoundFunctorCallbackImpl<R (*) (TX,T1),R,TX,T1,empty,empty,empty,empty> (fnPtr, a)
  );
  return Callback<R,T1> (impl);
}
template <typename R, typename TX, typename T1, typename T2>
Callback<R,T1,T2> MakeBoundCallback (R (*fnPtr) (TX,T1,T2), TX a) {
  ReferenceList<CallbackImpl<R,T1,T2,empty,empty,empty>*> impl =
  ReferenceList<CallbackImpl<R,T1,T2,empty,empty,empty>*> (
  new BoundFunctorCallbackImpl<R (*) (TX,T1,T2),R,TX,T1,T2,empty,empty,empty> (fnPtr, a)
  );
  return Callback<R,T1,T2> (impl);
}
template <typename R, typename TX, typename T1, typename T2,typename T3,typename T4>
Callback<R,T1,T2,T3,T4> MakeBoundCallback (R (*fnPtr) (TX,T1,T2,T3,T4), TX a) {
  ReferenceList<CallbackImpl<R,T1,T2,T3,T4,empty>*> impl =
  ReferenceList<CallbackImpl<R,T1,T2,T3,T4,empty>*> (
  new BoundFunctorCallbackImpl<R (*) (TX,T1,T2,T3,T4),R,TX,T1,T2,T3,T4,empty> (fnPtr, a)
  );
  return Callback<R,T1,T2,T3,T4> (impl);
}

template <typename R, typename TX, typename T1, typename T2,typename T3,typename T4,typename T5>
Callback<R,T1,T2,T3,T4,T5> MakeBoundCallback (R (*fnPtr) (TX,T1,T2,T3,T4,T5), TX a) {
  ReferenceList<CallbackImpl<R,T1,T2,T3,T4,T5>*> impl =
  ReferenceList<CallbackImpl<R,T1,T2,T3,T4,T5>*> (
  new BoundFunctorCallbackImpl<R (*) (TX,T1,T2,T3,T4,T5),R,TX,T1,T2,T3,T4,T5> (fnPtr, a)
  );
  return Callback<R,T1,T2,T3,T4,T5> (impl);
}


}; // namespace ns3


#endif /* CALLBACK_H */
