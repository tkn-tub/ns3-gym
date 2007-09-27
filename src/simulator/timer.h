#ifndef TIMER_H
#define TIMER_H

#include "ns3/fatal-error.h"
#include "nstime.h"
#include "event-id.h"

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
  enum {
    /**
     * This policy enforces a check before each call to Timer::Schedule
     * to verify that the timer has already expired. This policy
     * is incompatible with CANCEL_ON_SCHEDULE REMOVE_ON_SCHEDULE, and,
     * GARBAGE_COLLECT.
     */
    CHECK_ON_SCHEDULE = (1<<0),
    /**
     * This policy enforces a check from the destructor of the Timer
     * to verify that the timer has already expired. This policy is
     * incompatible with CANCEL_ON_DESTROY, REMOVE_ON_DESTROY, and
     * GARBAGE_COLLECT.
     */
    CHECK_ON_DESTROY = (1<<1),
    /**
     * This policy cancels the event before scheduling a new event 
     * for each call to Timer::Schedule. This policy
     * is incompatible with CHECK_ON_SCHEDULE, REMOVE_ON_SCHEDULE, and,
     * GARBAGE_COLLECT.
     */
    CANCEL_ON_SCHEDULE = (1<<2),
    /**
     * This policy cancels the event from the destructor of the Timer
     * to verify that the event has already expired. This policy is
     * incompatible with CHECK_ON_DESTROY, REMOVE_ON_DESTROY, and
     * GARBAGE_COLLECT.
     */
    CANCEL_ON_DESTROY = (1<<3),
    /**
     * This policy removes the event from the simulation event list 
     * before scheduling a new event for each call to Timer::Schedule. 
     * This policy is incompatible with CHECK_ON_SCHEDULE,
     * CANCEL_ON_SCHEDULE, and, GARBAGE_COLLECT.
     */
    REMOVE_ON_SCHEDULE = (1<<4),
    /**
     * This policy removes the event from the simulation event list
     * when the destructor of the Timer is invoked. This policy is
     * incompatible with CHECK_ON_DESTROY, CANCEL_ON_DESTROY, and
     * GARBAGE_COLLECT.
     */
    REMOVE_ON_DESTROY = (1<<5),
    /**
     * This policy is incompatible with all other policies. Event
     * event scheduled with this policy is kept track of by an
     * event garbage collector which makes sure that all events
     * of timers with a GARBAGE_COLLECT policy are cancelled at the
     * end of the simulation.
     */
    GARBAGE_COLLECT = (1<<6),
  };
  /**
   * create a timer with a default event lifetime management policy:
   *  - CHECK_ON_SCHEDULE
   *  - CHECK_ON_DESTROY
   */
  Timer ();
  /**
   * \param flags the event lifetime management policies to use
   *
   * The set of flag combinations allowed is:
   *  - none
   *  - GARBAGE_COLLECT
   *  - one of CANCEL_ON_DESTROY, REMOVE_ON_DESTROY, or, CHECK_ON_DESTROY
   *  - one of CANCEL_ON_SCHEDULE, REMOVE_ON_SCHEDULE, or, CHECK_ON_SCHEDULE
   *  - one of CANCEL_ON_DESTROY, REMOVE_ON_DESTROY, or, CHECK_ON_DESTROY ored
   *    with one of CANCEL_ON_SCHEDULE, REMOVE_ON_SCHEDULE, or, CHECK_ON_SCHEDULE.
   */
  Timer (int flags);
  ~Timer ();

  /**
   * \param fn the function
   *
   * Store this function in this Timer for later use by Timer::Schedule.
   */
  void SetFunction (void (*fn) (void));
  /**
   * \param fn the function
   * \param a1 the first argument
   *
   * Store this function and this argument in this Timer for later use by 
   * Timer::Schedule.
   */
  template <typename U1, typename T1>
  void SetFunction (void (*fn) (U1), T1 a1);
  /**
   * \param fn the function
   * \param a1 the first argument
   * \param a2 the second argument
   *
   * Store this function and these arguments in this Timer for later use by 
   * Timer::Schedule.
   */
  template <typename U1, typename U2,
            typename T1, typename T2>
  void SetFunction (void (*fn) (U1, U2), T1 a1, T2 a2);
  /**
   * \param fn the function
   * \param a1 the first argument
   * \param a2 the second argument
   * \param a3 the third argument
   *
   * Store this function and these arguments in this Timer for later use by 
   * Timer::Schedule.
   */
  template <typename U1, typename U2, typename U3,
            typename T1, typename T2, typename T3>
  void SetFunction (void (*fn) (U1, U2, U3), T1 a1, T2 a2, T3 a3);

  /**
   * \param memPtr the member function pointer
   * \param objPtr the pointer to object
   *
   * Store this function and object in this Timer for later use by Timer::Schedule.
   */
  template <typename MEM_PTR, typename OBJ_PTR>
  void SetFunction (MEM_PTR memPtr, OBJ_PTR objPtr);
  /**
   * \param memPtr the member function pointer
   * \param objPtr the pointer to object
   * \param a1 the first argument
   *
   * Store this function and this argument in this Timer for later use by 
   * Timer::Schedule.
   */
  template <typename MEM_PTR, typename OBJ_PTR,
            typename T1>
  void SetFunction (MEM_PTR memPtr, OBJ_PTR objPtr, T1 a1);
  /**
   * \param memPtr the member function pointer
   * \param objPtr the pointer to object
   * \param a1 the first argument
   * \param a2 the second argument
   *
   * Store this function and these arguments in this Timer for later use by 
   * Timer::Schedule.
   */
  template <typename MEM_PTR, typename OBJ_PTR,
            typename T1, typename T2>
  void SetFunction (MEM_PTR memPtr, OBJ_PTR objPtr, T1 a1, T2 a2);
  /**
   * \param memPtr the member function pointer
   * \param objPtr the pointer to object
   * \param a1 the first argument
   * \param a2 the second argument
   * \param a3 the third argument
   *
   * Store this function and these arguments in this Timer for later use by 
   * Timer::Schedule.
   */
  template <typename MEM_PTR, typename OBJ_PTR,
            typename T1, typename T2, typename T3>
  void SetFunction (MEM_PTR memPtr, OBJ_PTR objPtr, T1 a1, T2 a2, T3 a3);


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


template <typename U1, typename T1>
void 
Timer::SetFunction (void (*fn) (U1), T1 a1)
{
  struct FnTimerImplOne : public TimerImplOne<typename TimerTraits<U1>::ParameterType>
  {
    typedef void (*FN) (U1);
    FnTimerImplOne (FN fn) 
      : m_fn (fn) {}
    virtual void SetArguments (typename TimerTraits<U1>::ParameterType a1) {
      m_a1 = a1;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_fn, m_a1);
    }
    FN m_fn;
    typename TimerTraits<T1>::StoredType m_a1;
  } *function = new FnTimerImplOne (fn);
  function->SetArguments (a1);
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



template <typename MEM_PTR, typename OBJ_PTR, 
         typename T1>
void 
Timer::SetFunction (MEM_PTR memPtr, OBJ_PTR objPtr, T1 a1)
{
  struct MemFnTimerImplZero : public TimerImpl
  {
    MemFnTimerImplZero (MEM_PTR memPtr, OBJ_PTR objPtr) 
      : m_memPtr (memPtr), m_objPtr (objPtr) {}
    virtual void SetArguments (typename TimerTraits<T1>::ParameterType a1) {
      m_a1 = a1;
    }
    virtual EventId Schedule (const Time &delay) {
      return Simulator::Schedule (delay, m_memPtr, m_objPtr, m_a1);
    }
    MEM_PTR m_memPtr;
    OBJ_PTR m_objPtr;
    typename TimerTraits<T1>::StoredType m_a1;
  } *function = new MemFnTimerImplZero (memPtr, objPtr);
  function->SetArguments (a1);
  delete m_impl;
  m_impl = function;    
}




} // namespace ns3

#endif /* TIMER_H */
