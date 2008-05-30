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
#ifndef TIMER_H
#define TIMER_H

#include "ns3/fatal-error.h"
#include "nstime.h"
#include "event-id.h"
#include "ns3/int-to-type.h"

namespace ns3 {

class TimerImpl;

/**
 * \ingroup simulator
 *
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
  /**
   * The policy to use to manager the internal timer when and
   * instance of the Timer class is destroyed.
   */
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
  enum State {
    RUNNING,
    EXPIRED,
    SUSPENDED,
  };
  /**
   * create a timer with a default event lifetime management policy:
   *  - CHECK_ON_DESTROY
   */
  Timer ();
  /**
   * \param destroyPolicy the event lifetime management policies to use for destroy events
   */
  Timer (enum DestroyPolicy destroyPolicy);
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
   * \returns the amount of time left until this timer expires.
   *
   * This method returns zero if the timer is in EXPIRED state.
   */
  Time GetDelayLeft (void) const;
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
   * \returns true if this timer was suspended and not yet resumed, false
   *          otherwise.
   */
  bool IsSuspended (void) const;
  /**
   * \returns the current state of the timer.
   */
  enum Timer::State GetState (void) const;
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

  /**
   * Cancel the timer and save the amount of time left until it was
   * set to expire.
   * Calling Suspend on a non-running timer is an error.
   */
  void Suspend (void);
  /**
   * Restart the timer to expire within the amount of time left saved
   * during Suspend.
   * Calling Resume without a prior call to Suspend is an error.
   */
  void Resume (void);

private:

  enum {
    TIMER_SUSPENDED = (1<<7)
  };

  int m_flags;
  Time m_delay;
  EventId m_event;
  TimerImpl *m_impl;
  Time m_delayLeft;
};

} // namespace ns3

#include "timer-impl.h"

namespace ns3 {


template <typename FN>
void 
Timer::SetFunction (FN fn)
{
  delete m_impl;
  m_impl = MakeTimerImpl (fn);
}
template <typename MEM_PTR, typename OBJ_PTR>
void 
Timer::SetFunction (MEM_PTR memPtr, OBJ_PTR objPtr)
{
  delete m_impl;
  m_impl = MakeTimerImpl (memPtr, objPtr);
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
  m_impl->SetArgs (a1);
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
  m_impl->SetArgs (a1, a2);
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
  m_impl->SetArgs (a1, a2, a3);
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
  m_impl->SetArgs (a1, a2, a3, a4);
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
  m_impl->SetArgs (a1, a2, a3, a4, a5);
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
  m_impl->SetArgs (a1, a2, a3, a4, a5, a6);
}

} // namespace ns3

#endif /* TIMER_H */
