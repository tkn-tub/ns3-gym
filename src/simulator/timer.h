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

private:
  template <typename FN>
  void DoSetFunction (IntToType<0>, FN fn);
  template <typename FN>
  void DoSetFunction (IntToType<1>, FN fn);
  template <typename MEM_PTR, typename OBJ_PTR>
  void DoSetFunction (IntToType<0>, MEM_PTR memPtr, OBJ_PTR objPtr);
  template <typename MEM_PTR, typename OBJ_PTR>
  void DoSetFunction (IntToType<1>, MEM_PTR memPtr, OBJ_PTR objPtr);

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


template <typename T1>
void 
Timer::SetArguments (T1 a1)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Timer before setting its function.");
      return;
    }
  struct TimerImplOne<typename TimerTraits<T1>::ParameterType> *impl =
    dynamic_cast<struct TimerImplOne<typename TimerTraits<T1>::ParameterType> *> (m_impl);
  if (impl == 0)
    {
      NS_FATAL_ERROR ("You tried to set Timer arguments incompatible with its function.");
      return;
    }
  impl->SetArguments (a1);
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

} // namespace ns3

#endif /* TIMER_H */
