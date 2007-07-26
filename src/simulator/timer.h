/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INESC Porto
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
 * Author: Gustavo Carneiro  <gjc@inescporto.pt>
 */
#ifndef TIMER_H
#define TIMER_H

#include "simulator.h"
#include "ns3/callback.h"
#include "nstime.h"

namespace ns3 {

template <typename T1=empty, typename T2=empty,
          typename T3=empty, typename T4=empty,
          typename T5=empty>
class Timer
{
public:
  typedef Callback<void,T1,T2,T3,T4,T5> CallbackType;

protected:
  Time m_interval;
  CallbackType m_callback;
  EventId m_eventId;
  T1 m_t1;
  T2 m_t2;
  T3 m_t3;
  T4 m_t4;
  T5 m_t5;


public:

  /**
   * \brief create a new timer object
   *
   * Constructs a new timer object with the given interval and callback.
   * The timer will not actually start until Schedule() is called.
   *
   * \param interval The time interval after which the timer will expire
   * \param callback The callback object that will be invoked when the
   * timer expires
   *
   */
  Timer (Time const &interval, CallbackType callback)
    : m_interval (interval), m_callback (callback)
  {
  }

  Timer (CallbackType callback)
    : m_callback (callback)
  {
  }

  bool IsRunning () const
  {
    return m_eventId.IsRunning ();
  }

  /**
   * \brief start the timer
   *
   * Calling Schedule() on a timer essentially starts running
   * it. After the configured interval, the timer will expire and the
   * registered callback will be invoked.
   */
  void Schedule ()
  {
    if (m_eventId.IsRunning ())
      Simulator::Cancel (m_eventId);
    m_eventId = Simulator::Schedule (m_interval, &Timer::Expired, this);
  }
  void Schedule (T1 v1)
  {
    if (m_eventId.IsRunning ())
      Simulator::Cancel (m_eventId);
    m_eventId = Simulator::Schedule (m_interval, &Timer::Expired, this,
                                     v1);
  }
  void Schedule (T1 v1, T2 v2)
  {
    if (m_eventId.IsRunning ())
      Simulator::Cancel (m_eventId);
    m_eventId = Simulator::Schedule (m_interval, &Timer::Expired, this,
                                     v1, v2);
  }
  void Schedule (T1 v1, T2 v2, T3 v3)
  {
    if (m_eventId.IsRunning ())
      Simulator::Cancel (m_eventId);
    m_eventId = Simulator::Schedule (m_interval, &Timer::Expired, this,
                                     v1, v2, v3);
  }
  void Schedule (T1 v1, T2 v2, T3 v3, T4 v4)
  {
    if (m_eventId.IsRunning ())
      Simulator::Cancel (m_eventId);
    m_eventId = Simulator::Schedule (m_interval, &Timer::Expired, this,
                                     v1, v2, v3, v4);
  }
  void Schedule (T1 v1, T2 v2, T3 v3, T4 v4, T5 v5)
  {
    if (m_eventId.IsRunning ())
      Simulator::Cancel (m_eventId);
    m_eventId = Simulator::Schedule (m_interval, &Timer::Expired, this,
                                     v1, v2, v3, v4, v5);
  }

  /**
   * \brief changes the time interval for future Schedule() calls
   *
   * \param interval The time interval after which the timer will expire
   */
  void SetInterval (Time const &interval)
  {
    m_interval = interval;
  }

  /**
   * \brief cancel the timer
   *
   * Cancels the timer; after calling Cancel () the registered
   * callback will no longer be invoked, unless Schedule() is called
   * again.  May be called even if the timer is not running.
   */
  void Cancel ()
  {
    if (m_eventId.IsRunning ())
      Simulator::Cancel (m_eventId);
  }

  ~Timer ()
  {
    Cancel ();
  }

  /**
   * \brief artificially expire the timer
   *
   * Expires the timer at the current time.  This will result in the
   * registered callback to be invoked and the previously schedule
   * timeout, if any, to be cancelled.
   */
  void Expire ()
  {
    Cancel ();
    Expired ();
  }
  void Expire (T1 v1)
  {
    Cancel ();
    Expired (v1);
  }
  void Expire (T1 v1, T2 v2)
  {
    Cancel ();
    Expired (v1, v2);
  }
  void Expire (T1 v1, T2 v2, T3 v3)
  {
    Cancel ();
    Expired (v1, v2, v3);
  }
  void Expire (T1 v1, T2 v2, T3 v3, T4 v4)
  {
    Cancel ();
    Expired (v1, v2, v3, v4);
  }
  void Expire (T1 v1, T2 v2, T3 v3, T4 v4, T5 v5)
  {
    Cancel ();
    Expired (v1, v2, v3, v4, v5);
  }

private:
  void Expired ()
  {
    m_callback ();
  }
  void Expired (T1 v1)
  {
    m_callback (v1);
  }
  void Expired (T1 v1, T2 v2)
  {
    m_callback (v1, v2);
  }
  void Expired (T1 v1, T2 v2, T3 v3)
  {
    m_callback (v1, v2, v3);
  }
  void Expired (T1 v1, T2 v2, T3 v3, T4 v4)
  {
    m_callback (v1, v2, v3, v4);
  }
  void Expired (T1 v1, T2 v2, T3 v3, T4 v4, T5 v5)
  {
    m_callback (v1, v2, v3, v4, v5);
  }

};


}; // namespace ns3

#endif /* TIMER_H */
