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

/**
 * \file
 * \ingroup timer
 * ns3::Watchdog timer class declaration.
 */

namespace ns3 {

class TimerImpl;

/**
 * \ingroup timer
 * \brief A very simple watchdog operating in virtual time.
 *
 * The watchdog timer is started by calling Ping with a delay value.
 * Once started the timer cannot be suspended, cancelled or shortened.
 * It _can_ be lengthened (delayed) by calling Ping again:  if the new
 * expire time (current simulation time plus the new delay)
 * is greater than the old expire time the timer will be extended
 * to the new expire time.
 *
 * Typical usage would be to periodically Ping the Watchdog, extending
 * it's execution time.  If the owning process fails to Ping before
 * the Watchdog expires, the registered function will be invoked.
 *
 * If you don't ping the watchdog sufficiently often, it triggers its
 * listening function.
 *
 * \see Timer for a more sophisticated general purpose timer.
 */
class Watchdog 
{
public:
  /** Constructor. */
  Watchdog ();
  /** Destructor. */
  ~Watchdog ();

  /**
   * Delay the timer.
   *
   * \param [in] delay The watchdog delay
   *
   * After a call to this method, the watchdog will not be triggered
   * until the delay specified has been expired. This operation is
   * sometimes named "re-arming" a watchdog in some operating systems.
   */
  void Ping (Time delay);

  /**
   * Set the function to execute when the timer expires.
   *
   * \param [in] fn The function
   *
   * Store this function in this Timer for later use by Timer::Schedule.
   */
  template <typename FN>
  void SetFunction (FN fn);

  /**
   * Set the function to execute when the timer expires.
   *
   * \tparam MEM_PTR \deduced Class method function type.
   * \tparam OBJ_PTR \deduced Class type containing the function.
   * \param [in] memPtr The member function pointer
   * \param [in] objPtr The pointer to object
   *
   * Store this function and object in this Timer for later use by Timer::Schedule.
   */
  template <typename MEM_PTR, typename OBJ_PTR>
  void SetFunction (MEM_PTR memPtr, OBJ_PTR objPtr);


  /**
   * Set the arguments to be used when invoking the expire function.
   */
  /**@{*/
  /**
   * \tparam T1 \deduced Type of the first argument.
   * \param [in] a1 The first argument
   */
  template <typename T1>
  void SetArguments (T1 a1);
  /**
   * \tparam T1 \deduced Type of the first argument.
   * \tparam T2 \deduced Type of the second argument.
   * \param [in] a1 the first argument
   * \param [in] a2 the second argument
   */
  template <typename T1, typename T2>
  void SetArguments (T1 a1, T2 a2);
  /**
   * \tparam T1 \deduced Type of the first argument.
   * \tparam T2 \deduced Type of the second argument.
   * \tparam T3 \deduced Type of the third argument.
   * \param [in] a1 the first argument
   * \param [in] a2 the second argument
   * \param [in] a3 the third argument
   */
  template <typename T1, typename T2, typename T3>
  void SetArguments (T1 a1, T2 a2, T3 a3);
  /**
   * \tparam T1 \deduced Type of the first argument.
   * \tparam T2 \deduced Type of the second argument.
   * \tparam T3 \deduced Type of the third argument.
   * \tparam T4 \deduced Type of the fourth argument.
   * \param [in] a1 the first argument
   * \param [in] a2 the second argument
   * \param [in] a3 the third argument
   * \param [in] a4 the fourth argument
   */
  template <typename T1, typename T2, typename T3, typename T4>
  void SetArguments (T1 a1, T2 a2, T3 a3, T4 a4);
  /**
   * \tparam T1 \deduced Type of the first argument.
   * \tparam T2 \deduced Type of the second argument.
   * \tparam T3 \deduced Type of the third argument.
   * \tparam T4 \deduced Type of the fourth argument.
   * \tparam T5 \deduced Type of the fifth argument.
   * \param [in] a1 the first argument
   * \param [in] a2 the second argument
   * \param [in] a3 the third argument
   * \param [in] a4 the fourth argument
   * \param [in] a5 the fifth argument
   */
  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  void SetArguments (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
  /**
   * \tparam T1 \deduced Type of the first argument.
   * \tparam T2 \deduced Type of the second argument.
   * \tparam T3 \deduced Type of the third argument.
   * \tparam T4 \deduced Type of the fourth argument.
   * \tparam T5 \deduced Type of the fifth argument.
   * \tparam T6 \deduced Type of the sixth argument.
   * \param [in] a1 the first argument
   * \param [in] a2 the second argument
   * \param [in] a3 the third argument
   * \param [in] a4 the fourth argument
   * \param [in] a5 the fifth argument
   * \param [in] a6 the sixth argument
   */
  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  void SetArguments (T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);
  /**@}*/

private:
  /** Internal callback invoked when the timer expires. */
  void Expire (void);
  /**
   * The timer implementation, which contains the bound callback
   * function and arguments.
   */
  TimerImpl *m_impl;
  /** The future event scheduled to expire the timer. */
  EventId m_event;
  /** The absolute time when the timer will expire. */
  Time m_end;
};

} // namespace ns3


/********************************************************************
 *  Implementation of the templates declared above.
 ********************************************************************/

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
