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
#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "nstime.h"
#include "event-id.h"

namespace ns3 {

class TimerImpl;

class Watchdog 
{
public:
  Watchdog ();
  ~Watchdog ();

  void Ping (Time delay);

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

private:
  void Expire (void);
  TimerImpl *m_impl;
  EventId m_event;
  Time m_end;
};

} // namespace ns3

#include "timer-impl.h"

namespace ns3 {


template <typename FN>
void 
Watchdog::SetFunction (FN fn)
{
  delete m_impl;
  m_impl = MakeTimerImpl (fn);
}
template <typename MEM_PTR, typename OBJ_PTR>
void 
Watchdog::SetFunction (MEM_PTR memPtr, OBJ_PTR objPtr)
{
  delete m_impl;
  m_impl = MakeTimerImpl (memPtr, objPtr);
}

template <typename T1>
void 
Watchdog::SetArguments (T1 a1)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Watchdog before setting its function.");
      return;
    }
  m_impl->SetArgs (a1);
}
template <typename T1, typename T2>
void 
Watchdog::SetArguments (T1 a1, T2 a2)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Watchdog before setting its function.");
      return;
    }
  m_impl->SetArgs (a1, a2);
}

template <typename T1, typename T2, typename T3>
void 
Watchdog::SetArguments (T1 a1, T2 a2, T3 a3)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Watchdog before setting its function.");
      return;
    }
  m_impl->SetArgs (a1, a2, a3);
}

template <typename T1, typename T2, typename T3, typename T4>
void 
Watchdog::SetArguments (T1 a1, T2 a2, T3 a3, T4 a4)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Watchdog before setting its function.");
      return;
    }
  m_impl->SetArgs (a1, a2, a3, a4);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void 
Watchdog::SetArguments (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Watchdog before setting its function.");
      return;
    }
  m_impl->SetArgs (a1, a2, a3, a4, a5);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void 
Watchdog::SetArguments (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  if (m_impl == 0)
    {
      NS_FATAL_ERROR ("You cannot set the arguments of a Watchdog before setting its function.");
      return;
    }
  m_impl->SetArgs (a1, a2, a3, a4, a5, a6);
}

} // namespace ns3


#endif /* WATCHDOG_H */
