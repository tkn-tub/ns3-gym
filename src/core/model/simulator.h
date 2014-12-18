/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "event-id.h"
#include "event-impl.h"
#include "make-event.h"
#include "nstime.h"

#include "object-factory.h"

#include <stdint.h>
#include <string>

/**
 * \file
 * \ingroup simulator
 * ns3::Simulator declaration.
 */

namespace ns3 {

class SimulatorImpl;
class Scheduler;

/**
 * \ingroup core
 * \defgroup simulator Simulator
 * \brief Control the virtual time and the execution of simulation events. 
 */
/**
 * \ingroup simulator
 *
 * \brief Control the scheduling of simulation events. 
 *
 * The internal simulation clock is maintained
 * as a 64-bit integer in a unit specified by the user
 * through the Time::SetResolution function. This means that it is
 * not possible to specify event expiration times with anything better
 * than this user-specified accuracy. Events whose expiration time is
 * the same modulo this accuracy are scheduled in FIFO order: the 
 * first event inserted in the scheduling queue is scheduled to 
 * expire first.
 * 
 * A simple example of how to use the Simulator class to schedule events
 * is shown in sample-simulator.cc ::
 * \include src/core/examples/sample-simulator.cc
 *
 * \todo Define what the simulation or event context means.
 */
class Simulator 
{
public:
  /**
   * \param impl A new simulator implementation.
   *
   * The simulator provides a mechanism to swap out different implementations.
   * For example, the default implementation is a single-threaded simulator
   * that performs no realtime synchronization.  By calling this method, you
   * can substitute in a new simulator implementation that might be multi-
   * threaded and synchronize events to a realtime clock.
   *
   * The simulator implementation can be set when the simulator is not 
   * running.
   */
  static void SetImplementation (Ptr<SimulatorImpl> impl);

  /**
   * \brief Get the SimulatorImpl singleton.
   *
   * \internal
   * If the SimulatorImpl singleton hasn't been created yet,
   * this function does so.  At the same time it also creates
   * the Scheduler.  Both of these respect the global values
   * which may have been set from the command line or through
   * the Config system.
   *
   * As a side effect we also call LogSetTimePrinter() and
   * LogSetNodePrinter() with the default implementations
   * since we can't really do any logging until we have
   * a SimulatorImpl and Scheduler.
   
   * \return The SimulatorImpl singleton.
   */
  static Ptr<SimulatorImpl> GetImplementation (void);

  /**
   * \param schedulerFactory a new event scheduler factory
   *
   * The event scheduler can be set at any time: the events scheduled
   * in the previous scheduler will be transfered to the new scheduler
   * before we start to use it.
   */
  static void SetScheduler (ObjectFactory schedulerFactory);

  /**  \copydoc SimulatorImpl::Destroy   */
  static void Destroy (void);

  /** \copydoc SimulatorImpl::IsFinished */
  static bool IsFinished (void);

  /** \copydoc SimulatorImpl::Run */
  static void Run (void);

  /** \copydoc SimulatorImpl::Stop(void) */
  static void Stop (void);

  /** \copydoc SimulatorImpl::Stop(Time const &) */
  static void Stop (Time const &time);

  /**
   * \name Schedule events (in the same context) to run at a future time.
   */
  /** @{ */
  /**
   * Schedule an event to expire at the relative time "time"
   * is reached.  This can be thought of as scheduling an event
   * for the current simulation time plus the Time passed as a
   * parameter
   *
   * When the event expires (when it becomes due to be run), the 
   * input method will be invoked on the input object.
   *
   * @param time the relative expiration time of the event.
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @returns an id for the scheduled event.
   */
  template <typename MEM, typename OBJ>
  static EventId Schedule (Time const &time, MEM mem_ptr, OBJ obj);

  /**
   * @param time the relative expiration time of the event.
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @returns an id for the scheduled event.
   */
  template <typename MEM, typename OBJ, typename T1>
  static EventId Schedule (Time const &time, MEM mem_ptr, OBJ obj, T1 a1);

  /**
   * @param time the relative expiration time of the event.
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @returns an id for the scheduled event.
   */
  template <typename MEM, typename OBJ, typename T1, typename T2>
  static EventId Schedule (Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2);

  /**
   * @param time the relative expiration time of the event.
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @returns an id for the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3>
  static EventId Schedule (Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3);

  /**
   * @param time the relative expiration time of the event.
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @param a4 the fourth argument to pass to the invoked method
   * @returns an id for the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4>
  static EventId Schedule (Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * @param time the relative expiration time of the event.
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @param a4 the fourth argument to pass to the invoked method
   * @param a5 the fifth argument to pass to the invoked method
   * @returns an id for the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId Schedule (Time const &time, MEM mem_ptr, OBJ obj, 
                           T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
  /**
   * @param time the relative expiration time of the event.
   * @param f the function to invoke
   * @returns an id for the scheduled event.
   */
  static EventId Schedule (Time const &time, void (*f)(void));

  /**
   * @param time the relative expiration time of the event.
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @returns an id for the scheduled event.
   */
  template <typename U1, typename T1>
  static EventId Schedule (Time const &time, void (*f)(U1), T1 a1);

  /**
   * @param time the relative expiration time of the event.
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @returns an id for the scheduled event.
   */
  template <typename U1, typename U2, typename T1, typename T2>
  static EventId Schedule (Time const &time, void (*f)(U1,U2), T1 a1, T2 a2);

  /**
   * @param time the relative expiration time of the event.
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @returns an id for the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename T1, typename T2, typename T3>
  static EventId Schedule (Time const &time, void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3);

  /**
   * @param time the relative expiration time of the event.
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   * @returns an id for the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4, 
            typename T1, typename T2, typename T3, typename T4>
  static EventId Schedule (Time const &time, void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * @param time the relative expiration time of the event.
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   * @param a5 the fifth argument to pass to the function to invoke
   * @returns an id for the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5,
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId Schedule (Time const &time, void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /** @} */

  /**
   * \name Schedule events (in a different context) to run at a particular time.
   *
   * See \ref main-test-sync.cc for example usage.
   */
  /** @{ */
  /**
   * Schedule an event with the given context.
   * A context of 0xffffffff means no context is specified.
   * This method is thread-safe: it can be called from any thread.
   *
   * @param time the relative expiration time of the event.
   * @param context user-specified context parameter
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   */
  template <typename MEM, typename OBJ>
  static void ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj);

  /**
   * This method is thread-safe: it can be called from any thread.
   *
   * @param time the relative expiration time of the event.
   * @param context user-specified context parameter
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, typename T1>
  static void ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj, T1 a1);

  /**
   * This method is thread-safe: it can be called from any thread.
   *
   * @param time the relative expiration time of the event.
   * @param context user-specified context parameter
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, typename T1, typename T2>
  static void ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2);

  /**
   * This method is thread-safe: it can be called from any thread.
   *
   * @param time the relative expiration time of the event.
   * @param context user-specified context parameter
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3>
  static void ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3);

  /**
   * This method is thread-safe: it can be called from any thread.
   *
   * @param time the relative expiration time of the event.
   * @param context user-specified context parameter
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @param a4 the fourth argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4>
  static void ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * This method is thread-safe: it can be called from any thread.
   *
   * @param time the relative expiration time of the event.
   * @param context user-specified context parameter
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @param a4 the fourth argument to pass to the invoked method
   * @param a5 the fifth argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static void ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj, 
                                   T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
  /**
   * This method is thread-safe: it can be called from any thread.
   *
   * @param time the relative expiration time of the event.
   * @param context user-specified context parameter
   * @param f the function to invoke
   */
  static void ScheduleWithContext (uint32_t context, Time const &time, void (*f)(void));

  /**
   * This method is thread-safe: it can be called from any thread.
   *
   * @param time the relative expiration time of the event.
   * @param context user-specified context parameter
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   */
  template <typename U1, typename T1>
  static void ScheduleWithContext (uint32_t context, Time const &time, void (*f)(U1), T1 a1);

  /**
   * This method is thread-safe: it can be called from any thread.
   *
   * @param time the relative expiration time of the event.
   * @param context user-specified context parameter
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename T1, typename T2>
  static void ScheduleWithContext (uint32_t context, Time const &time, void (*f)(U1,U2), T1 a1, T2 a2);

  /**
   * This method is thread-safe: it can be called from any thread.
   *
   * @param time the relative expiration time of the event.
   * @param context user-specified context parameter
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3, typename T1, typename T2, typename T3>
  static void ScheduleWithContext (uint32_t context, Time const &time, void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3);

  /**
   * This method is thread-safe: it can be called from any thread.
   *
   * @param time the relative expiration time of the event.
   * @param context user-specified context parameter
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3, typename U4, 
            typename T1, typename T2, typename T3, typename T4>
  static void ScheduleWithContext (uint32_t context, Time const &time, void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * This method is thread-safe: it can be called from any thread.
   *
   * @param time the relative expiration time of the event.
   * @param context user-specified context parameter
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   * @param a5 the fifth argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5,
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static void ScheduleWithContext (uint32_t context, Time const &time, void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /** @} */
  
  /**
   * \name Schedule events (in the same context) to run now.
   */
  /** @{ */
  /**
   * Schedule an event to expire Now. All events scheduled to
   * to expire "Now" are scheduled FIFO, after all normal events
   * have expired. 
   *
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1, T2 a2);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @param a4 the fourth argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, 
                              T1 a1, T2 a2, T3 a3, T4 a4);
  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @param a4 the fourth argument to pass to the invoked method
   * @param a5 the fifth argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, 
                              T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
  /**
   * @param f the function to invoke
   * @return The EventId of the scheduled event.
   */
  static EventId ScheduleNow (void (*f)(void));

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1,
            typename T1>
  static EventId ScheduleNow (void (*f)(U1), T1 a1);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2,
            typename T1, typename T2>
  static EventId ScheduleNow (void (*f)(U1,U2), T1 a1, T2 a2);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3,
            typename T1, typename T2, typename T3>
  static EventId ScheduleNow (void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4,
            typename T1, typename T2, typename T3, typename T4>
  static EventId ScheduleNow (void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   * @param a5 the fifth argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5,
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId ScheduleNow (void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /** @} */

  /**
   * \name Schedule events to run at the end of the simulation.
   */
  /** @{ */
  /**
   * Schedule an event to expire at Destroy time. All events 
   * scheduled to expire at "Destroy" time are scheduled FIFO, 
   * after all normal events have expired and only when 
   * Simulator::Destroy is invoked.
   *
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ,
            typename T1, typename T2>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1, T2 a2);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @param a4 the fourth argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, 
                                  T1 a1, T2 a2, T3 a3, T4 a4);
  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @param a4 the fourth argument to pass to the invoked method
   * @param a5 the fifth argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, 
                                  T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
  /**
   * @param f the function to invoke
   * @return The EventId of the scheduled event.
   */
  static EventId ScheduleDestroy (void (*f)(void));

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1,
            typename T1>
  static EventId ScheduleDestroy (void (*f)(U1), T1 a1);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2,
            typename T1, typename T2>
  static EventId ScheduleDestroy (void (*f)(U1,U2), T1 a1, T2 a2);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3,
            typename T1, typename T2, typename T3>
  static EventId ScheduleDestroy (void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4,
            typename T1, typename T2, typename T3, typename T4>
  static EventId ScheduleDestroy (void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   * @param a5 the fifth argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5,
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId ScheduleDestroy (void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /** @} */

  /** \copydoc SimulatorImpl::Remove */
  static void Remove (const EventId &id);

  /** \copydoc SimulatorImpl::Cancel */
  static void Cancel (const EventId &id);

  /** \copydoc SimulatorImpl::IsExpired */
  static bool IsExpired (const EventId &id);

  /** \copydoc SimulatorImpl::Now */
  static Time Now (void);

  /** \copydoc SimulatorImpl::GetDelayLeft */
  static Time GetDelayLeft (const EventId &id);

  /** \copydoc SimulatorImpl::GetMaximumSimulationTime */
  static Time GetMaximumSimulationTime (void);

  /** \copydoc SimulatorImpl::GetContext */
  static uint32_t GetContext (void);

  /** \copydoc SimulatorImpl::Schedule */
  static EventId Schedule (Time const &time, const Ptr<EventImpl> &event);

  /** \copydoc SimulatorImpl::ScheduleWithContext
   * This method is thread-safe: it can be called from any thread.
   */
  static void ScheduleWithContext (uint32_t context, const Time &time, EventImpl *event);

  /** \copydoc SimulatorImpl::ScheduleDestroy */
  static EventId ScheduleDestroy (const Ptr<EventImpl> &event);

  /** \copydoc SimulatorImpl::ScheduleNow */
  static EventId ScheduleNow (const Ptr<EventImpl> &event);

  /** \copydoc SimulatorImpl::GetSystemId */
  static uint32_t GetSystemId (void);
  
private:
  /** Default constructor. */
  Simulator ();
  /** Destructor. */
  ~Simulator ();

  /**
   * Implementation of the various Schedule methods.
   * \param [in] time Delay until the event should execute.
   * \param [in] event The event to execute.
   * \return The EventId.
   */
  static EventId DoSchedule (Time const &time, EventImpl *event);
  /**
   * Implementation of the various ScheduleNow methods.
   * \param [in] event The event to execute.
   * \return The EventId.
   */
  static EventId DoScheduleNow (EventImpl *event);
  /**
   * Implementation of the various ScheduleDestroy methods.
   * \param [in] event The event to execute.
   * \return The EventId.
   */
  static EventId DoScheduleDestroy (EventImpl *event);
};

/**
 * \ingroup simulator
 * \brief create an ns3::Time instance which contains the
 *        current simulation time.
 *
 * This is really a shortcut for the ns3::Simulator::Now method.
 * It is typically used as shown below to schedule an event
 * which expires at the absolute time "2 seconds":
 * \code
 *   Simulator::Schedule (Seconds (2.0) - Now (), &my_function);
 * \endcode
 * \return The current simulation time.
 */
Time Now (void);

} // namespace ns3


/********************************************************************
 *  Implementation of the templates declared above.
 ********************************************************************/

namespace ns3 {

template <typename MEM, typename OBJ>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj) 
{
  return DoSchedule (time, MakeEvent (mem_ptr, obj));
}


template <typename MEM, typename OBJ,
          typename T1>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj, T1 a1) 
{
  return DoSchedule (time, MakeEvent (mem_ptr, obj, a1));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2)
{
  return DoSchedule (time, MakeEvent (mem_ptr, obj, a1, a2));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3) 
{
  return DoSchedule (time, MakeEvent (mem_ptr, obj, a1, a2, a3));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4) 
{
  return DoSchedule (time, MakeEvent (mem_ptr, obj, a1, a2, a3, a4));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj, 
                             T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
  return DoSchedule (time, MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5));
}

template <typename U1, typename T1>
EventId Simulator::Schedule (Time const &time, void (*f)(U1), T1 a1)
{
  return DoSchedule (time, MakeEvent (f, a1));
}

template <typename U1, typename U2, 
          typename T1, typename T2>
EventId Simulator::Schedule (Time const &time, void (*f)(U1,U2), T1 a1, T2 a2)
{
  return DoSchedule (time, MakeEvent (f, a1, a2));
}

template <typename U1, typename U2, typename U3,
          typename T1, typename T2, typename T3>
EventId Simulator::Schedule (Time const &time, void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3)
{
  return DoSchedule (time, MakeEvent (f, a1, a2, a3));
}

template <typename U1, typename U2, typename U3, typename U4,
          typename T1, typename T2, typename T3, typename T4>
EventId Simulator::Schedule (Time const &time, void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4)
{
  return DoSchedule (time, MakeEvent (f, a1, a2, a3, a4));
}

template <typename U1, typename U2, typename U3, typename U4, typename U5,
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId Simulator::Schedule (Time const &time, void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  return DoSchedule (time, MakeEvent (f, a1, a2, a3, a4, a5));
}




template <typename MEM, typename OBJ>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj)
{
  ScheduleWithContext (context, time, MakeEvent (mem_ptr, obj));
}


template <typename MEM, typename OBJ,
          typename T1>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj, T1 a1)
{
  return ScheduleWithContext (context, time, MakeEvent (mem_ptr, obj, a1));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2)
{
  return ScheduleWithContext (context, time, MakeEvent (mem_ptr, obj, a1, a2));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3)
{
  return ScheduleWithContext (context, time, MakeEvent (mem_ptr, obj, a1, a2, a3));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3, typename T4>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4)
{
  return ScheduleWithContext (context, time, MakeEvent (mem_ptr, obj, a1, a2, a3, a4));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3, typename T4, typename T5>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj,
                                     T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  return ScheduleWithContext (context, time, MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5));
}

template <typename U1, typename T1>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, void (*f)(U1), T1 a1)
{
  return ScheduleWithContext (context, time, MakeEvent (f, a1));
}

template <typename U1, typename U2,
          typename T1, typename T2>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, void (*f)(U1,U2), T1 a1, T2 a2)
{
  return ScheduleWithContext (context, time, MakeEvent (f, a1, a2));
}

template <typename U1, typename U2, typename U3,
          typename T1, typename T2, typename T3>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3)
{
  return ScheduleWithContext (context, time, MakeEvent (f, a1, a2, a3));
}

template <typename U1, typename U2, typename U3, typename U4,
          typename T1, typename T2, typename T3, typename T4>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4)
{
  return ScheduleWithContext (context, time, MakeEvent (f, a1, a2, a3, a4));
}

template <typename U1, typename U2, typename U3, typename U4, typename U5,
          typename T1, typename T2, typename T3, typename T4, typename T5>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  return ScheduleWithContext (context, time, MakeEvent (f, a1, a2, a3, a4, a5));
}




template <typename MEM, typename OBJ>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj) 
{
  return DoScheduleNow (MakeEvent (mem_ptr, obj));
}


template <typename MEM, typename OBJ, 
          typename T1>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1) 
{
  return DoScheduleNow (MakeEvent (mem_ptr, obj, a1));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1, T2 a2) 
{
  return DoScheduleNow (MakeEvent (mem_ptr, obj, a1, a2));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3) 
{
  return DoScheduleNow (MakeEvent (mem_ptr, obj, a1, a2, a3));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4) 
{
  return DoScheduleNow (MakeEvent (mem_ptr, obj, a1, a2, a3, a4));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj, 
                        T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
  return DoScheduleNow (MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5));
}

template <typename U1,
          typename T1>
EventId
Simulator::ScheduleNow (void (*f)(U1), T1 a1)
{
  return DoScheduleNow (MakeEvent (f, a1));
}

template <typename U1, typename U2,
          typename T1, typename T2>
EventId
Simulator::ScheduleNow (void (*f)(U1,U2), T1 a1, T2 a2)
{
  return DoScheduleNow (MakeEvent (f, a1, a2));
}

template <typename U1, typename U2, typename U3,
          typename T1, typename T2, typename T3>
EventId
Simulator::ScheduleNow (void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3)
{
  return DoScheduleNow (MakeEvent (f, a1, a2, a3));
}

template <typename U1, typename U2, typename U3, typename U4,
          typename T1, typename T2, typename T3, typename T4>
EventId
Simulator::ScheduleNow (void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4)
{
  return DoScheduleNow (MakeEvent (f, a1, a2, a3, a4));
}

template <typename U1, typename U2, typename U3, typename U4, typename U5,
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId
Simulator::ScheduleNow (void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  return DoScheduleNow (MakeEvent (f, a1, a2, a3, a4, a5));
}



template <typename MEM, typename OBJ>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj) 
{
  return DoScheduleDestroy (MakeEvent (mem_ptr, obj));
}


template <typename MEM, typename OBJ, 
          typename T1>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1) 
{
  return DoScheduleDestroy (MakeEvent (mem_ptr, obj, a1));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1, T2 a2) 
{
  return DoScheduleDestroy (MakeEvent (mem_ptr, obj, a1, a2));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3) 
{
  return DoScheduleDestroy (MakeEvent (mem_ptr, obj, a1, a2, a3));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3, typename T4>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4) 
{
  return DoScheduleDestroy (MakeEvent (mem_ptr, obj, a1, a2, a3, a4));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj, 
                            T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
  return DoScheduleDestroy (MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5));
}

template <typename U1,
          typename T1>
EventId
Simulator::ScheduleDestroy (void (*f)(U1), T1 a1)
{
  return DoScheduleDestroy (MakeEvent (f, a1));
}

template <typename U1, typename U2,
          typename T1, typename T2>
EventId
Simulator::ScheduleDestroy (void (*f)(U1,U2), T1 a1, T2 a2)
{
  return DoScheduleDestroy (MakeEvent (f, a1, a2));
}

template <typename U1, typename U2, typename U3,
          typename T1, typename T2, typename T3>
EventId
Simulator::ScheduleDestroy (void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3)
{
  return DoScheduleDestroy (MakeEvent (f, a1, a2, a3));
}

template <typename U1, typename U2, typename U3, typename U4,
          typename T1, typename T2, typename T3, typename T4>
EventId
Simulator::ScheduleDestroy (void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4)
{
  return DoScheduleDestroy (MakeEvent (f, a1, a2, a3, a4));
}

template <typename U1, typename U2, typename U3, typename U4, typename U5,
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId
Simulator::ScheduleDestroy (void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  return DoScheduleDestroy (MakeEvent (f, a1, a2, a3, a4, a5));
}

} // namespace ns3

#endif /* SIMULATOR_H */
