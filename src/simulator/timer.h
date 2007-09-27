#ifndef TIMER_H
#define TIMER_H

#include "ns3/fatal-error.h"
#include "nstime.h"
#include "event-id.h"

namespace ns3 {

class TimerImpl;

class Timer 
{
public:
  enum {
    CHECK_ON_SCHEDULE = (1<<0),
    CHECK_ON_DESTROY = (1<<1),
    CANCEL_ON_SCHEDULE = (1<<2),
    CANCEL_ON_DESTROY = (1<<3),
    REMOVE_ON_SCHEDULE = (1<<4),
    REMOVE_ON_DESTROY = (1<<5),
    GARBAGE_COLLECT = (1<<6),
  };
  Timer ();
  Timer (int flags);
  ~Timer ();

  void SetFunction (void (*fn) (void));
  template <typename U1, typename T1>
  void SetFunction (void (*fn) (U1), T1 a1);
  template <typename U1, typename U2,
            typename T1, typename T2>
  void SetFunction (void (*fn) (U1, U2), T1 a1, T2 a2);
  template <typename U1, typename U2, typename U3,
            typename T1, typename T2, typename T3>
  void SetFunction (void (*fn) (U1, U2, U3), T1 a1, T2 a2, T3 a3);

  template <typename T1>
  void SetArguments (T1 a1);
  template <typename T1, typename T2>
  void SetArguments (T1 a1, T2 a2);
  template <typename T1, typename T2, typename T3>
  void SetArguments (T1 a1, T2 a2, T3 a3);

  void SetDelay (const Time &time);
  Time GetDelay (void) const;
  void Cancel (void);
  void Remove (void);
  bool IsExpired (void) const;
  bool IsRunning (void) const;

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




} // namespace ns3

#endif /* TIMER_H */
