/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#ifndef TIMER_IMPL_H
#define TIMER_IMPL_H

#include "simulator.h"
#include "ns3/type-traits.h"
#include "ns3/fatal-error.h"
#include "ns3/int-to-type.h"

namespace ns3 {

class TimerImpl
{
public:  
  virtual ~TimerImpl () {}

  template <typename T1>
  void SetArgs (T1 a1);
  template <typename T1, typename T2>
  void SetArgs (T1 a1, T2 a2);
  template <typename T1, typename T2, typename T3>
  void SetArgs (T1 a1, T2 a2, T3 a3);
  template <typename T1, typename T2, typename T3, 
            typename T4>
  void SetArgs (T1 a1, T2 a2, T3 a3, T4 a4);
  template <typename T1, typename T2, typename T3, 
            typename T4, typename T5>
  void SetArgs (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
  template <typename T1, typename T2, typename T3, 
            typename T4, typename T5, typename T6>
  void SetArgs (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);

  virtual EventId Schedule (const Time &delay) = 0;
  virtual void Invoke (void) = 0;
};


template <typename T1>
struct TimerImplOne : public TimerImpl
{
  virtual void SetArguments (T1 a1) = 0;
};
template <typename T1, typename T2>
struct TimerImplTwo : public TimerImpl
{
  virtual void SetArguments (T1 a1,T2 a2) = 0;
};
template <typename T1, typename T2, typename T3>
struct TimerImplThree : public TimerImpl
{
  virtual void SetArguments (T1 a1,T2 a2,T3 a3) = 0;
};
template <typename T1, typename T2, typename  T3, typename T4>
struct TimerImplFour : public TimerImpl
{
  virtual void SetArguments (T1 a1,T2 a2,T3 a3, T4 a4) = 0;
};
template <typename T1, typename T2, typename  T3, typename T4, typename T5>
struct TimerImplFive : public TimerImpl
{
  virtual void SetArguments (T1 a1,T2 a2,T3 a3, T4 a4, T5 a5) = 0;
};
template <typename T1, typename T2, typename  T3, typename T4, typename T5, typename T6>
struct TimerImplSix : public TimerImpl
{
  virtual void SetArguments (T1 a1,T2 a2,T3 a3, T4 a4, T5 a5, T6 a6) = 0;
};



template <typename T>
struct TimerTraits
{
  typedef typename TypeTraits<typename TypeTraits<T>::ReferencedType>::NonConstType StoredType;
  typedef const StoredType &ParameterType;
};

template <typename FN>
TimerImpl *
MakeTimerImpl (FN fn)
{
  NS_ASSERT (TypeTraits<FN>::IsFunctionPointer);
  return MakeTimerImpl (IntToType<TypeTraits<FN>::FunctionPointerTraits::nArgs> (), fn);
}

template <typename FN>
TimerImpl *
MakeTimerImpl (IntToType<0>, FN fn)
{
  struct FnTimerImplZero : public TimerImpl
  {
    FnTimerImplZero (FN fn) 
      : m_fn (fn) {}
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_fn);
    }
    virtual void Invoke (void) {
      m_fn ();
    }
    FN m_fn;
  } *function = new FnTimerImplZero (fn);
  return function;
}

template <typename FN>
TimerImpl *
MakeTimerImpl (IntToType<1>, FN fn)
{
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg1Type T1;
  typedef typename TimerTraits<T1>::ParameterType T1Parameter;
  typedef typename TimerTraits<T1>::StoredType T1Stored;

  struct FnTimerImplOne : public TimerImplOne<T1Parameter>
  {
    FnTimerImplOne (FN fn) 
      : m_fn (fn) {}
    virtual void SetArguments (T1Parameter a1) {
      m_a1 = a1;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_fn, m_a1);
    }
    virtual void Invoke (void) {
      m_fn (m_a1);
    }
    FN m_fn;
    T1Stored m_a1;
  } *function = new FnTimerImplOne (fn);
  return function;
}

template <typename FN>
TimerImpl *
MakeTimerImpl (IntToType<2>, FN fn)
{
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg1Type T1;
  typedef typename TimerTraits<T1>::ParameterType T1Parameter;
  typedef typename TimerTraits<T1>::StoredType T1Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg2Type T2;
  typedef typename TimerTraits<T2>::ParameterType T2Parameter;
  typedef typename TimerTraits<T2>::StoredType T2Stored;

  struct FnTimerImplTwo : public TimerImplTwo<T1Parameter,T2Parameter>
  {
    FnTimerImplTwo (FN fn) 
      : m_fn (fn) {}
    virtual void SetArguments (T1Parameter a1, T2Parameter a2) {
      m_a1 = a1;
      m_a2 = a2;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_fn, m_a1, m_a2);
    }
    virtual void Invoke (void) {
      m_fn (m_a1, m_a2);
    }
    FN m_fn;
    T1Stored m_a1;
    T2Stored m_a2;
  } *function = new FnTimerImplTwo (fn);
  return function;
}

template <typename FN>
TimerImpl *
MakeTimerImpl (IntToType<3>, FN fn)
{
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg1Type T1;
  typedef typename TimerTraits<T1>::ParameterType T1Parameter;
  typedef typename TimerTraits<T1>::StoredType T1Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg2Type T2;
  typedef typename TimerTraits<T2>::ParameterType T2Parameter;
  typedef typename TimerTraits<T2>::StoredType T2Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg3Type T3;
  typedef typename TimerTraits<T3>::ParameterType T3Parameter;
  typedef typename TimerTraits<T3>::StoredType T3Stored;

  struct FnTimerImplThree : public TimerImplThree<T1Parameter,T2Parameter,T3Parameter>
  {
    FnTimerImplThree (FN fn) 
      : m_fn (fn) {}
    virtual void SetArguments (T1Parameter a1, T2Parameter a2, T3Parameter a3) {
      m_a1 = a1;
      m_a2 = a2;
      m_a3 = a3;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_fn, m_a1, m_a2, m_a3);
    }
    virtual void Invoke (void) {
      m_fn (m_a1, m_a2, m_a3);
    }
    FN m_fn;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
  } *function = new FnTimerImplThree (fn);
  return function;
}

template <typename FN>
TimerImpl *
MakeTimerImpl (IntToType<4>, FN fn)
{
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg1Type T1;
  typedef typename TimerTraits<T1>::ParameterType T1Parameter;
  typedef typename TimerTraits<T1>::StoredType T1Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg2Type T2;
  typedef typename TimerTraits<T2>::ParameterType T2Parameter;
  typedef typename TimerTraits<T2>::StoredType T2Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg3Type T3;
  typedef typename TimerTraits<T3>::ParameterType T3Parameter;
  typedef typename TimerTraits<T3>::StoredType T3Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg4Type T4;
  typedef typename TimerTraits<T4>::ParameterType T4Parameter;
  typedef typename TimerTraits<T4>::StoredType T4Stored;

  struct FnTimerImplFour : public TimerImplFour<T1Parameter,T2Parameter,T3Parameter,T4Parameter>
  {
    FnTimerImplFour (FN fn) 
      : m_fn (fn) {}
    virtual void SetArguments (T1Parameter a1, T2Parameter a2, T3Parameter a3, T4Parameter a4) {
      m_a1 = a1;
      m_a2 = a2;
      m_a3 = a3;
      m_a4 = a4;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_fn, m_a1, m_a2, m_a3, m_a4);
    }
    virtual void Invoke (void) {
      m_fn (m_a1, m_a2, m_a3, m_a4);
    }
    FN m_fn;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
    T4Stored m_a4;
  } *function = new FnTimerImplFour (fn);
  return function;
}

template <typename FN>
TimerImpl *
MakeTimerImpl (IntToType<5>, FN fn)
{
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg1Type T1;
  typedef typename TimerTraits<T1>::ParameterType T1Parameter;
  typedef typename TimerTraits<T1>::StoredType T1Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg2Type T2;
  typedef typename TimerTraits<T2>::ParameterType T2Parameter;
  typedef typename TimerTraits<T2>::StoredType T2Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg3Type T3;
  typedef typename TimerTraits<T3>::ParameterType T3Parameter;
  typedef typename TimerTraits<T3>::StoredType T3Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg4Type T4;
  typedef typename TimerTraits<T4>::ParameterType T4Parameter;
  typedef typename TimerTraits<T4>::StoredType T4Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg5Type T5;
  typedef typename TimerTraits<T5>::ParameterType T5Parameter;
  typedef typename TimerTraits<T5>::StoredType T5Stored;

  struct FnTimerImplFive : public TimerImplFive<T1Parameter,T2Parameter,T3Parameter,T4Parameter,T5Parameter>
  {
    FnTimerImplFive (FN fn) 
      : m_fn (fn) {}
    virtual void SetArguments (T1Parameter a1, T2Parameter a2, T3Parameter a3, T4Parameter a4, T5Parameter a5) {
      m_a1 = a1;
      m_a2 = a2;
      m_a3 = a3;
      m_a4 = a4;
      m_a5 = a5;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_fn, m_a1, m_a2, m_a3, m_a4, m_a5);
    }
    virtual void Invoke (void) {
      m_fn (m_a1, m_a2, m_a3, m_a4, m_a5);
    }
    FN m_fn;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
    T4Stored m_a4;
    T5Stored m_a5;
  } *function = new FnTimerImplFive (fn);
  return function;
}

template <typename FN>
TimerImpl *
MakeTimerImpl (IntToType<6>, FN fn)
{
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg1Type T1;
  typedef typename TimerTraits<T1>::ParameterType T1Parameter;
  typedef typename TimerTraits<T1>::StoredType T1Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg2Type T2;
  typedef typename TimerTraits<T2>::ParameterType T2Parameter;
  typedef typename TimerTraits<T2>::StoredType T2Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg3Type T3;
  typedef typename TimerTraits<T3>::ParameterType T3Parameter;
  typedef typename TimerTraits<T3>::StoredType T3Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg4Type T4;
  typedef typename TimerTraits<T4>::ParameterType T4Parameter;
  typedef typename TimerTraits<T4>::StoredType T4Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg5Type T5;
  typedef typename TimerTraits<T5>::ParameterType T5Parameter;
  typedef typename TimerTraits<T5>::StoredType T5Stored;
  typedef typename TypeTraits<FN>::FunctionPointerTraits::Arg6Type T6;
  typedef typename TimerTraits<T6>::ParameterType T6Parameter;
  typedef typename TimerTraits<T6>::StoredType T6Stored;

  struct FnTimerImplSix : public TimerImplSix<T1Parameter,T2Parameter,T3Parameter,T4Parameter,T5Parameter,T6Parameter>
  {
    FnTimerImplSix (FN fn) 
      : m_fn (fn) {}
    virtual void SetArguments (T1Parameter a1, T2Parameter a2, T3Parameter a3, T4Parameter a4, T5Parameter a5, T6Parameter a6) {
      m_a1 = a1;
      m_a2 = a2;
      m_a3 = a3;
      m_a4 = a4;
      m_a5 = a5;
      m_a6 = a6;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_fn, m_a1, m_a2, m_a3, m_a4, m_a5, m_a6);
    }
    virtual void Invoke (void) {
      m_fn (m_a1, m_a2, m_a3, m_a4, m_a5, m_a6);
    }
    FN m_fn;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
    T4Stored m_a4;
    T5Stored m_a5;
    T6Stored m_a6;
  } *function = new FnTimerImplSix (fn);
  return function;
}


template <typename T>
struct TimerImplMemberTraits;


template <typename T>
struct TimerImplMemberTraits<T *>
{
  static T &GetReference (T *p) {
    return *p;
  }
};

template <typename MEM_PTR, typename OBJ_PTR>
TimerImpl *
MakeTimerImpl (MEM_PTR memPtr, OBJ_PTR objPtr)
{
  NS_ASSERT (TypeTraits<MEM_PTR>::IsPointerToMember);
  return MakeTimerImpl (IntToType<TypeTraits<MEM_PTR>::PointerToMemberTraits::nArgs> () , memPtr, objPtr);
}

template <typename MEM_PTR, typename OBJ_PTR>
TimerImpl *
MakeTimerImpl (IntToType<0>, MEM_PTR memPtr, OBJ_PTR objPtr)
{
  struct MemFnTimerImplZero : public TimerImpl
  {
    MemFnTimerImplZero (MEM_PTR memPtr, OBJ_PTR objPtr) 
      : m_memPtr (memPtr), m_objPtr (objPtr) {}
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_memPtr, m_objPtr);
    }
    virtual void Invoke (void) {
      (TimerImplMemberTraits<OBJ_PTR>::GetReference (m_objPtr).*m_memPtr) (); 
    }
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
  } *function = new MemFnTimerImplZero (memPtr, objPtr);
  return function;
}

template <typename MEM_PTR, typename OBJ_PTR>
TimerImpl *
MakeTimerImpl (IntToType<1>, MEM_PTR memPtr, OBJ_PTR objPtr)
{
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg1Type T1;
  typedef typename TimerTraits<T1>::ParameterType T1Parameter;
  typedef typename TimerTraits<T1>::StoredType T1Stored;
  
  struct MemFnTimerImplOne : public TimerImplOne<T1Parameter>
  {
    MemFnTimerImplOne (MEM_PTR memPtr, OBJ_PTR objPtr) 
      : m_memPtr (memPtr), m_objPtr (objPtr) {}
    virtual void SetArguments (T1Parameter a1) {
      m_a1 = a1;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_memPtr, m_objPtr, m_a1);
    }
    virtual void Invoke (void) {
      (TimerImplMemberTraits<OBJ_PTR>::GetReference (m_objPtr).*m_memPtr) (m_a1); 
    }
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    T1Stored m_a1;
  } *function = new MemFnTimerImplOne (memPtr, objPtr);
  return function;
}

template <typename MEM_PTR, typename OBJ_PTR>
TimerImpl *
MakeTimerImpl (IntToType<2>, MEM_PTR memPtr, OBJ_PTR objPtr)
{
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg1Type T1;
  typedef typename TimerTraits<T1>::ParameterType T1Parameter;
  typedef typename TimerTraits<T1>::StoredType T1Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg2Type T2;
  typedef typename TimerTraits<T2>::ParameterType T2Parameter;
  typedef typename TimerTraits<T2>::StoredType T2Stored;
  
  struct MemFnTimerImplTwo : public TimerImplTwo<T1Parameter,T2Parameter>
  {
    MemFnTimerImplTwo (MEM_PTR memPtr, OBJ_PTR objPtr) 
      : m_memPtr (memPtr), m_objPtr (objPtr) {}
    virtual void SetArguments (T1Parameter a1, T2Parameter a2) {
      m_a1 = a1;
      m_a2 = a2;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_memPtr, m_objPtr, m_a1, m_a2);
    }
    virtual void Invoke (void) {
      (TimerImplMemberTraits<OBJ_PTR>::GetReference (m_objPtr).*m_memPtr) (m_a1, m_a2); 
    }
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    T1Stored m_a1;
    T2Stored m_a2;
  } *function = new MemFnTimerImplTwo (memPtr, objPtr);
  return function;
}

template <typename MEM_PTR, typename OBJ_PTR>
TimerImpl *
MakeTimerImpl (IntToType<3>, MEM_PTR memPtr, OBJ_PTR objPtr)
{
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg1Type T1;
  typedef typename TimerTraits<T1>::ParameterType T1Parameter;
  typedef typename TimerTraits<T1>::StoredType T1Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg2Type T2;
  typedef typename TimerTraits<T2>::ParameterType T2Parameter;
  typedef typename TimerTraits<T2>::StoredType T2Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg3Type T3;
  typedef typename TimerTraits<T3>::ParameterType T3Parameter;
  typedef typename TimerTraits<T3>::StoredType T3Stored;
  
  struct MemFnTimerImplThree : public TimerImplThree<T1Parameter,T2Parameter,T3Parameter>
  {
    MemFnTimerImplThree (MEM_PTR memPtr, OBJ_PTR objPtr) 
      : m_memPtr (memPtr), m_objPtr (objPtr) {}
    virtual void SetArguments (T1Parameter a1, T2Parameter a2, T3Parameter a3) {
      m_a1 = a1;
      m_a2 = a2;
      m_a3 = a3;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_memPtr, m_objPtr, m_a1, m_a2, m_a3);
    }
    virtual void Invoke (void) {
      (TimerImplMemberTraits<OBJ_PTR>::GetReference (m_objPtr).*m_memPtr) (m_a1, m_a2, m_a3); 
    }
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
  } *function = new MemFnTimerImplThree (memPtr, objPtr);
  return function;
}

template <typename MEM_PTR, typename OBJ_PTR>
TimerImpl *
MakeTimerImpl (IntToType<4>, MEM_PTR memPtr, OBJ_PTR objPtr)
{
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg1Type T1;
  typedef typename TimerTraits<T1>::ParameterType T1Parameter;
  typedef typename TimerTraits<T1>::StoredType T1Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg2Type T2;
  typedef typename TimerTraits<T2>::ParameterType T2Parameter;
  typedef typename TimerTraits<T2>::StoredType T2Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg3Type T3;
  typedef typename TimerTraits<T3>::ParameterType T3Parameter;
  typedef typename TimerTraits<T3>::StoredType T3Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg4Type T4;
  typedef typename TimerTraits<T4>::ParameterType T4Parameter;
  typedef typename TimerTraits<T4>::StoredType T4Stored;
  
  struct MemFnTimerImplFour : public TimerImplFour<T1Parameter,T2Parameter,T3Parameter,T4Parameter>
  {
    MemFnTimerImplFour (MEM_PTR memPtr, OBJ_PTR objPtr) 
      : m_memPtr (memPtr), m_objPtr (objPtr) {}
    virtual void SetArguments (T1Parameter a1, T2Parameter a2, T3Parameter a3, T4Parameter a4) {
      m_a1 = a1;
      m_a2 = a2;
      m_a3 = a3;
      m_a4 = a4;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_memPtr, m_objPtr, m_a1, m_a2, m_a3, m_a4);
    }
    virtual void Invoke (void) {
      (TimerImplMemberTraits<OBJ_PTR>::GetReference (m_objPtr).*m_memPtr) (m_a1, m_a2, m_a3, m_a4); 
    }
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
    T4Stored m_a4;
  } *function = new MemFnTimerImplFour (memPtr, objPtr);
  return function;
}

template <typename MEM_PTR, typename OBJ_PTR>
TimerImpl *
MakeTimerImpl (IntToType<5>, MEM_PTR memPtr, OBJ_PTR objPtr)
{
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg1Type T1;
  typedef typename TimerTraits<T1>::ParameterType T1Parameter;
  typedef typename TimerTraits<T1>::StoredType T1Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg2Type T2;
  typedef typename TimerTraits<T2>::ParameterType T2Parameter;
  typedef typename TimerTraits<T2>::StoredType T2Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg3Type T3;
  typedef typename TimerTraits<T3>::ParameterType T3Parameter;
  typedef typename TimerTraits<T3>::StoredType T3Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg4Type T4;
  typedef typename TimerTraits<T4>::ParameterType T4Parameter;
  typedef typename TimerTraits<T4>::StoredType T4Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg5Type T5;
  typedef typename TimerTraits<T5>::ParameterType T5Parameter;
  typedef typename TimerTraits<T5>::StoredType T5Stored;
  
  struct MemFnTimerImplFive : public TimerImplFive<T1Parameter,T2Parameter,T3Parameter,T4Parameter,T5Parameter>
  {
    MemFnTimerImplFive (MEM_PTR memPtr, OBJ_PTR objPtr) 
      : m_memPtr (memPtr), m_objPtr (objPtr) {}
    virtual void SetArguments (T1Parameter a1, T2Parameter a2, T3Parameter a3, T4Parameter a4,T5Parameter a5) {
      m_a1 = a1;
      m_a2 = a2;
      m_a3 = a3;
      m_a4 = a4;
      m_a5 = a5;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_memPtr, m_objPtr, m_a1, m_a2, m_a3, m_a4, m_a5);
    }
    virtual void Invoke (void) {
      (TimerImplMemberTraits<OBJ_PTR>::GetReference (m_objPtr).*m_memPtr) (m_a1, m_a2, m_a3, m_a4, m_a5); 
    }
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
    T4Stored m_a4;
    T5Stored m_a5;
  } *function = new MemFnTimerImplFive (memPtr, objPtr);
  return function;
}

template <typename MEM_PTR, typename OBJ_PTR>
TimerImpl *
MakeTimerImpl (IntToType<6>, MEM_PTR memPtr, OBJ_PTR objPtr)
{
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg1Type T1;
  typedef typename TimerTraits<T1>::ParameterType T1Parameter;
  typedef typename TimerTraits<T1>::StoredType T1Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg2Type T2;
  typedef typename TimerTraits<T2>::ParameterType T2Parameter;
  typedef typename TimerTraits<T2>::StoredType T2Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg3Type T3;
  typedef typename TimerTraits<T3>::ParameterType T3Parameter;
  typedef typename TimerTraits<T3>::StoredType T3Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg4Type T4;
  typedef typename TimerTraits<T4>::ParameterType T4Parameter;
  typedef typename TimerTraits<T4>::StoredType T4Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg5Type T5;
  typedef typename TimerTraits<T5>::ParameterType T5Parameter;
  typedef typename TimerTraits<T5>::StoredType T5Stored;
  typedef typename TypeTraits<MEM_PTR>::PointerToMemberTraits::Arg6Type T6;
  typedef typename TimerTraits<T6>::ParameterType T6Parameter;
  typedef typename TimerTraits<T6>::StoredType T6Stored;
  
  struct MemFnTimerImplSix : public TimerImplSix<T1Parameter,T2Parameter,T3Parameter,T4Parameter,T5Parameter,T6Parameter>
  {
    MemFnTimerImplSix (MEM_PTR memPtr, OBJ_PTR objPtr) 
      : m_memPtr (memPtr), m_objPtr (objPtr) {}
    virtual void SetArguments (T1Parameter a1, T2Parameter a2, T3Parameter a3, T4Parameter a4,T5Parameter a5,T6Parameter a6) {
      m_a1 = a1;
      m_a2 = a2;
      m_a3 = a3;
      m_a4 = a4;
      m_a5 = a5;
      m_a6 = a6;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_memPtr, m_objPtr, m_a1, m_a2, m_a3, m_a4, m_a5, m_a6);
    }
    virtual void Invoke (void) {
      (TimerImplMemberTraits<OBJ_PTR>::GetReference (m_objPtr).*m_memPtr) (m_a1, m_a2, m_a3, m_a4, m_a5, m_a6); 
    }
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
    T4Stored m_a4;
    T5Stored m_a5;
    T6Stored m_a6;
  } *function = new MemFnTimerImplSix (memPtr, objPtr);
  return function;
}


template <typename T1>
void
TimerImpl::SetArgs (T1 a1)
{
  typedef struct TimerImplOne<
    typename TimerTraits<T1>::ParameterType
    > TimerImplBase;
  TimerImplBase *impl = dynamic_cast<TimerImplBase *> (this);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1);
}

template <typename T1, typename T2>
void
TimerImpl::SetArgs (T1 a1, T2 a2)
{
  typedef struct TimerImplTwo<
    typename TimerTraits<T1>::ParameterType,
    typename TimerTraits<T2>::ParameterType
    > TimerImplBase;
  TimerImplBase *impl = dynamic_cast<TimerImplBase *> (this);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1, a2);
}

template <typename T1, typename T2, typename T3>
void 
TimerImpl::SetArgs (T1 a1, T2 a2, T3 a3)
{
  typedef struct TimerImplThree<
    typename TimerTraits<T1>::ParameterType,
    typename TimerTraits<T2>::ParameterType,
    typename TimerTraits<T3>::ParameterType
    > TimerImplBase;
  TimerImplBase *impl = dynamic_cast<TimerImplBase *> (this);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1, a2, a3);
}

template <typename T1, typename T2, typename T3, typename T4>
void 
TimerImpl::SetArgs (T1 a1, T2 a2, T3 a3, T4 a4)
{
  typedef struct TimerImplFour<
    typename TimerTraits<T1>::ParameterType,
    typename TimerTraits<T2>::ParameterType,
    typename TimerTraits<T3>::ParameterType,
    typename TimerTraits<T4>::ParameterType
    > TimerImplBase;
  TimerImplBase *impl = dynamic_cast<TimerImplBase *> (this);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1, a2, a3, a4);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void 
TimerImpl::SetArgs (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  typedef struct TimerImplFive<
    typename TimerTraits<T1>::ParameterType,
    typename TimerTraits<T2>::ParameterType,
    typename TimerTraits<T3>::ParameterType,
    typename TimerTraits<T4>::ParameterType,
    typename TimerTraits<T5>::ParameterType
    > TimerImplBase;
  TimerImplBase *impl = dynamic_cast<TimerImplBase *> (this);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1, a2, a3, a4, a5);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void 
TimerImpl::SetArgs (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  typedef struct TimerImplSix<
    typename TimerTraits<T1>::ParameterType,
    typename TimerTraits<T2>::ParameterType,
    typename TimerTraits<T3>::ParameterType,
    typename TimerTraits<T4>::ParameterType,
    typename TimerTraits<T5>::ParameterType,
    typename TimerTraits<T6>::ParameterType
    > TimerImplBase;
  TimerImplBase *impl = dynamic_cast<TimerImplBase *> (this);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1, a2, a3, a4, a5, a6);
}

} // namespace ns3

#endif /* TIMER_IMPL_H */
