#ifndef TIMER_H
#define TIMER_H

#include "ns3/fatal-error.h"
#include "nstime.h"
#include "event-id.h"
#include "ns3/int-to-type.h"

namespace ns3 {

class TimerImpl;

/**
 * \brief a simple Timer class
 *
 * A timer is used to hold together a delay, a function to invoke
 * when the delay expires, and a set of arguments to pass to the function
 * when the delay expires.
 *
 * A timer can also be used to enforce a set of predefined event lifetime
 * management policies. These policies are specified at construction time
 * and cannot be changed after.
 */
class Timer 
{
public:
  enum SchedulePolicy {
    /**
     * This policy cancels the event before scheduling a new event 
     * for each call to Timer::Schedule.
     */
    CANCEL_ON_SCHEDULE = (1<<0),
    /**
     * This policy removes the event from the simulation event list 
     * before scheduling a new event for each call to Timer::Schedule. 
     */
    REMOVE_ON_SCHEDULE = (1<<1),
    /**
     * This policy enforces a check before each call to Timer::Schedule
     * to verify that the timer has already expired.
     */
    CHECK_ON_SCHEDULE = (1<<2),
  };
  enum DestroyPolicy {
    /**
     * This policy cancels the event from the destructor of the Timer
     * to verify that the event has already expired.
     */
    CANCEL_ON_DESTROY = (1<<3),
    /**
     * This policy removes the event from the simulation event list
     * when the destructor of the Timer is invoked.
     */
    REMOVE_ON_DESTROY = (1<<4),
    /**
     * This policy enforces a check from the destructor of the Timer
     * to verify that the timer has already expired.
     */
    CHECK_ON_DESTROY = (1<<5)
  };
  enum GarbageCollectPolicy {
    /**
     * Every event scheduled with this policy is kept track of by an
     * event garbage collector which makes sure that all events
     * of timers with a GARBAGE_COLLECT policy are cancelled at the
     * end of the simulation.
     */
    GARBAGE_COLLECT = (1<<6)
  };
  /**
   * create a timer with a default event lifetime management policy:
   *  - CHECK_ON_SCHEDULE
   *  - CHECK_ON_DESTROY
   */
  Timer ();
  /**
   * \param scheduleFlags the event lifetime management policies to use for schedule events
   * \param destroyFlags the event lifetime management policies to use for destroy events
   */
  Timer (enum SchedulePolicy schedulePolicy, 
	 enum DestroyPolicy destroyPolicy);
  /**
   * \param policy the garbage collect policy. Only one
   *        value is possible.
   */
  Timer (enum GarbageCollectPolicy policy);
  ~Timer ();

  /**
   * \param fn the function
   *
   * Store this function in this Timer for later use by Timer::Schedule.
   */
  template <typename FN>
  void SetFunction (FN fn);

  /**
   * \param memPtr the member function pointer
   * \param objPtr the pointer to object
   *
   * Store this function and object in this Timer for later use by Timer::Schedule.
   */
  template <typename MEM_PTR, typename OBJ_PTR>
  void SetFunction (MEM_PTR memPtr, OBJ_PTR objPtr);


  /**
   * \param a1 the first argument
   *
   * Store this argument in this Timer for later use by Timer::Schedule.
   */
  template <typename T1>
  void SetArguments (T1 a1);
  /**
   * \param a1 the first argument
   * \param a2 the second argument
   *
   * Store these arguments in this Timer for later use by Timer::Schedule.
   */
  template <typename T1, typename T2>
  void SetArguments (T1 a1, T2 a2);
  /**
   * \param a1 the first argument
   * \param a2 the second argument
   * \param a3 the third argument
   *
   * Store these arguments in this Timer for later use by Timer::Schedule.
   */
  template <typename T1, typename T2, typename T3>
  void SetArguments (T1 a1, T2 a2, T3 a3);
  /**
   * \param a1 the first argument
   * \param a2 the second argument
   * \param a3 the third argument
   * \param a4 the fourth argument
   *
   * Store these arguments in this Timer for later use by Timer::Schedule.
   */
  template <typename T1, typename T2, typename T3, typename T4>
  void SetArguments (T1 a1, T2 a2, T3 a3, T4 a4);
  /**
   * \param a1 the first argument
   * \param a2 the second argument
   * \param a3 the third argument
   * \param a4 the fourth argument
   * \param a5 the fifth argument
   *
   * Store these arguments in this Timer for later use by Timer::Schedule.
   */
  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  void SetArguments (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
  /**
   * \param a1 the first argument
   * \param a2 the second argument
   * \param a3 the third argument
   * \param a4 the fourth argument
   * \param a5 the fifth argument
   * \param a6 the sixth argument
   *
   * Store these arguments in this Timer for later use by Timer::Schedule.
   */
  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  void SetArguments (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);

  /**
   * \param delay the delay
   *
   * The next call to Schedule will schedule the timer with this delay.
   */
  void SetDelay (const Time &delay);
  /**
   * \returns the currently-configured delay for the next Schedule.
   */
  Time GetDelay (void) const;
  /**
   * Cancel the currently-running event if there is one. Do nothing
   * otherwise.
   */
  void Cancel (void);
  /**
   * Remove from the simulation event-list the currently-running event 
   * if there is one. Do nothing otherwise.
   */
  void Remove (void);
  /**
   * \return true if there is no currently-running event, false otherwise.
   */
  bool IsExpired (void) const;
  /**
   * \return true if there is a currently-running event, false otherwise.
   */
  bool IsRunning (void) const;
  /**
   * Schedule a new event using the currently-configured delay, function, 
   * and arguments.
   */
  void Schedule (void);
  /**
   * \param delay the delay to use
   *
   * Schedule a new event using the specified delay (ignore the delay set by 
   * Timer::SetDelay), function, and arguments.
   */
  void Schedule (Time delay);

private:
  template <typename FN>
  void DoSetFunction (IntToType<0>, FN fn);
  template <typename FN>
  void DoSetFunction (IntToType<1>, FN fn);
  template <typename FN>
  void DoSetFunction (IntToType<2>, FN fn);
  template <typename FN>
  void DoSetFunction (IntToType<3>, FN fn);
  template <typename FN>
  void DoSetFunction (IntToType<4>, FN fn);
  template <typename FN>
  void DoSetFunction (IntToType<5>, FN fn);
  template <typename FN>
  void DoSetFunction (IntToType<6>, FN fn);

  template <typename MEM_PTR, typename OBJ_PTR>
  void DoSetFunction (IntToType<0>, MEM_PTR memPtr, OBJ_PTR objPtr);
  template <typename MEM_PTR, typename OBJ_PTR>
  void DoSetFunction (IntToType<1>, MEM_PTR memPtr, OBJ_PTR objPtr);
  template <typename MEM_PTR, typename OBJ_PTR>
  void DoSetFunction (IntToType<2>, MEM_PTR memPtr, OBJ_PTR objPtr);
  template <typename MEM_PTR, typename OBJ_PTR>
  void DoSetFunction (IntToType<3>, MEM_PTR memPtr, OBJ_PTR objPtr);
  template <typename MEM_PTR, typename OBJ_PTR>
  void DoSetFunction (IntToType<4>, MEM_PTR memPtr, OBJ_PTR objPtr);
  template <typename MEM_PTR, typename OBJ_PTR>
  void DoSetFunction (IntToType<5>, MEM_PTR memPtr, OBJ_PTR objPtr);
  template <typename MEM_PTR, typename OBJ_PTR>
  void DoSetFunction (IntToType<6>, MEM_PTR memPtr, OBJ_PTR objPtr);

  int m_flags;
  Time m_delay;
  EventId m_event;
  TimerImpl *m_impl;
};

} // namespace ns3


// The actual implementation.
#include "simulator.h"
#include "ns3/type-traits.h"

namespace ns3 {


template <typename T>
struct TimerTraits;
template <typename T>
struct TimerTraits
{
  typedef typename TypeTraits<typename TypeTraits<T>::ReferencedType>::NonConstType StoredType;
  typedef const StoredType &ParameterType;
};

class TimerImpl
{
public:
  virtual ~TimerImpl () {}
  virtual EventId Schedule (const Time &delay) = 0;
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


template <typename FN>
void 
Timer::SetFunction (FN fn)
{
  NS_ASSERT (TypeTraits<FN>::IsFunctionPointer);
  DoSetFunction (IntToType<TypeTraits<FN>::FunctionPointerTraits::nArgs> (), fn);
}

template <typename FN>
void 
Timer::DoSetFunction (IntToType<0>, FN fn)
{
  struct FnTimerImplZero : public TimerImpl
  {
    FnTimerImplZero (FN fn) 
      : m_fn (fn) {}
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_fn);
    }
    FN m_fn;
  } *function = new FnTimerImplZero (fn);
  delete m_impl;
  m_impl = function;
}

template <typename FN>
void 
Timer::DoSetFunction (IntToType<1>, FN fn)
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
    FN m_fn;
    T1Stored m_a1;
  } *function = new FnTimerImplOne (fn);
  delete m_impl;
  m_impl = function;  
}

template <typename FN>
void 
Timer::DoSetFunction (IntToType<2>, FN fn)
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
    FN m_fn;
    T1Stored m_a1;
    T2Stored m_a2;
  } *function = new FnTimerImplTwo (fn);
  delete m_impl;
  m_impl = function;  
}

template <typename FN>
void 
Timer::DoSetFunction (IntToType<3>, FN fn)
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
    FN m_fn;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
  } *function = new FnTimerImplThree (fn);
  delete m_impl;
  m_impl = function;  
}

template <typename FN>
void 
Timer::DoSetFunction (IntToType<4>, FN fn)
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
    FN m_fn;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
    T4Stored m_a4;
  } *function = new FnTimerImplFour (fn);
  delete m_impl;
  m_impl = function;  
}

template <typename FN>
void 
Timer::DoSetFunction (IntToType<5>, FN fn)
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
    FN m_fn;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
    T4Stored m_a4;
    T5Stored m_a5;
  } *function = new FnTimerImplFive (fn);
  delete m_impl;
  m_impl = function;  
}

template <typename FN>
void 
Timer::DoSetFunction (IntToType<6>, FN fn)
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
    FN m_fn;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
    T4Stored m_a4;
    T5Stored m_a5;
    T6Stored m_a6;
  } *function = new FnTimerImplSix (fn);
  delete m_impl;
  m_impl = function;  
}

template <typename MEM_PTR, typename OBJ_PTR>
void 
Timer::SetFunction (MEM_PTR memPtr, OBJ_PTR objPtr)
{
  NS_ASSERT (TypeTraits<MEM_PTR>::IsPointerToMember);
  DoSetFunction (IntToType<TypeTraits<MEM_PTR>::PointerToMemberTraits::nArgs> () , memPtr, objPtr);
}

template <typename MEM_PTR, typename OBJ_PTR>
void 
Timer::DoSetFunction (IntToType<0>, MEM_PTR memPtr, OBJ_PTR objPtr)
{
  struct MemFnTimerImplZero : public TimerImpl
  {
    MemFnTimerImplZero (MEM_PTR memPtr, OBJ_PTR objPtr) 
      : m_memPtr (memPtr), m_objPtr (objPtr) {}
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_memPtr, m_objPtr);
    }
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
  } *function = new MemFnTimerImplZero (memPtr, objPtr);
  delete m_impl;
  m_impl = function;    
}

template <typename MEM_PTR, typename OBJ_PTR>
void 
Timer::DoSetFunction (IntToType<1>, MEM_PTR memPtr, OBJ_PTR objPtr)
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
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    T1Stored m_a1;
  } *function = new MemFnTimerImplOne (memPtr, objPtr);
  delete m_impl;
  m_impl = function;    
}

template <typename MEM_PTR, typename OBJ_PTR>
void 
Timer::DoSetFunction (IntToType<2>, MEM_PTR memPtr, OBJ_PTR objPtr)
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
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    T1Stored m_a1;
    T2Stored m_a2;
  } *function = new MemFnTimerImplTwo (memPtr, objPtr);
  delete m_impl;
  m_impl = function;    
}

template <typename MEM_PTR, typename OBJ_PTR>
void 
Timer::DoSetFunction (IntToType<3>, MEM_PTR memPtr, OBJ_PTR objPtr)
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
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
  } *function = new MemFnTimerImplThree (memPtr, objPtr);
  delete m_impl;
  m_impl = function;    
}

template <typename MEM_PTR, typename OBJ_PTR>
void 
Timer::DoSetFunction (IntToType<4>, MEM_PTR memPtr, OBJ_PTR objPtr)
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
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
    T4Stored m_a4;
  } *function = new MemFnTimerImplFour (memPtr, objPtr);
  delete m_impl;
  m_impl = function;    
}

template <typename MEM_PTR, typename OBJ_PTR>
void 
Timer::DoSetFunction (IntToType<5>, MEM_PTR memPtr, OBJ_PTR objPtr)
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
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
    T4Stored m_a4;
    T5Stored m_a5;
  } *function = new MemFnTimerImplFive (memPtr, objPtr);
  delete m_impl;
  m_impl = function;    
}

template <typename MEM_PTR, typename OBJ_PTR>
void 
Timer::DoSetFunction (IntToType<6>, MEM_PTR memPtr, OBJ_PTR objPtr)
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
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    T1Stored m_a1;
    T2Stored m_a2;
    T3Stored m_a3;
    T4Stored m_a4;
    T5Stored m_a5;
    T6Stored m_a6;
  } *function = new MemFnTimerImplSix (memPtr, objPtr);
  delete m_impl;
  m_impl = function;    
}


template <typename T1>
void 
Timer::SetArguments (T1 a1)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Timer before setting its function.");
      return;
    }
  typedef struct TimerImplOne<
    typename TimerTraits<T1>::ParameterType
    > TimerImplBase;
  TimerImplBase *impl = dynamic_cast<TimerImplBase *> (m_impl);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1);
}

template <typename T1, typename T2>
void 
Timer::SetArguments (T1 a1, T2 a2)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Timer before setting its function.");
      return;
    }
  typedef struct TimerImplTwo<
    typename TimerTraits<T1>::ParameterType,
    typename TimerTraits<T2>::ParameterType
    > TimerImplBase;
  TimerImplBase *impl = dynamic_cast<TimerImplBase *> (m_impl);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1, a2);
}

template <typename T1, typename T2, typename T3>
void 
Timer::SetArguments (T1 a1, T2 a2, T3 a3)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Timer before setting its function.");
      return;
    }
  typedef struct TimerImplThree<
    typename TimerTraits<T1>::ParameterType,
    typename TimerTraits<T2>::ParameterType,
    typename TimerTraits<T3>::ParameterType
    > TimerImplBase;
  TimerImplBase *impl = dynamic_cast<TimerImplBase *> (m_impl);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1, a2, a3);
}

template <typename T1, typename T2, typename T3, typename T4>
void 
Timer::SetArguments (T1 a1, T2 a2, T3 a3, T4 a4)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Timer before setting its function.");
      return;
    }
  typedef struct TimerImplFour<
    typename TimerTraits<T1>::ParameterType,
    typename TimerTraits<T2>::ParameterType,
    typename TimerTraits<T3>::ParameterType,
    typename TimerTraits<T4>::ParameterType
    > TimerImplBase;
  TimerImplBase *impl = dynamic_cast<TimerImplBase *> (m_impl);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1, a2, a3, a4);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void 
Timer::SetArguments (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Timer before setting its function.");
      return;
    }
  typedef struct TimerImplFive<
    typename TimerTraits<T1>::ParameterType,
    typename TimerTraits<T2>::ParameterType,
    typename TimerTraits<T3>::ParameterType,
    typename TimerTraits<T4>::ParameterType,
    typename TimerTraits<T5>::ParameterType
    > TimerImplBase;
  TimerImplBase *impl = dynamic_cast<TimerImplBase *> (m_impl);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1, a2, a3, a4, a5);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void 
Timer::SetArguments (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Timer before setting its function.");
      return;
    }
  typedef struct TimerImplSix<
    typename TimerTraits<T1>::ParameterType,
    typename TimerTraits<T2>::ParameterType,
    typename TimerTraits<T3>::ParameterType,
    typename TimerTraits<T4>::ParameterType,
    typename TimerTraits<T5>::ParameterType,
    typename TimerTraits<T6>::ParameterType
    > TimerImplBase;
  TimerImplBase *impl = dynamic_cast<TimerImplBase *> (m_impl);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1, a2, a3, a4, a5, a6);
}



} // namespace ns3

#endif /* TIMER_H */
