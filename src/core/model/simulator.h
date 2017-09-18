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
 * @file
 * @ingroup simulator
 * ns3::Simulator declaration.
 */

namespace ns3 {

class SimulatorImpl;
class Scheduler;

/**
 * @ingroup core
 * @defgroup simulator Simulator
 * @brief Control the virtual time and the execution of simulation events. 
 */
/**
 * @ingroup simulator
 *
 * @brief Control the scheduling of simulation events. 
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
 * is shown in sample-simulator.cc:
 * @include src/core/examples/sample-simulator.cc
 */
class Simulator 
{
public:
  /**
   * @param [in] impl A new simulator implementation.
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
   * @brief Get the SimulatorImpl singleton.
   *
   * @internal
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
   
   * @return The SimulatorImpl singleton.
   */
  static Ptr<SimulatorImpl> GetImplementation (void);

  /**
   * @brief Set the scheduler type with an ObjectFactory.
   * @param [in] schedulerFactory The configured ObjectFactory.
   *
   * The event scheduler can be set at any time: the events scheduled
   * in the previous scheduler will be transfered to the new scheduler
   * before we start to use it.
   */
  static void SetScheduler (ObjectFactory schedulerFactory);

  /**
   * Execute the events scheduled with ScheduleDestroy().
   *
   * This method is typically invoked at the end of a simulation
   * to avoid false-positive reports by a leak checker.
   * After this method has been invoked, it is actually possible
   * to restart a new simulation with a set of calls to Simulator::Run,
   * Simulator::Schedule and Simulator::ScheduleWithContext.
   */
  static void Destroy (void);

  /**
   * Check if the simulation should finish.
   *
   * Reasons to finish are because there are
   * no more events lefts to be scheduled, or if simulation
   * time has already reached the "stop time" (see Simulator::Stop()).
   *
   * @return @c true if no more events or stop time reached.
   */
  static bool IsFinished (void);

  /**
   * Run the simulation.
   *
   * The simulation will run until one of:
   *   - No events are present anymore
   *   - The user called Simulator::Stop
   *   - The user called Simulator::Stop with a stop time and the
   *     expiration time of the next event to be processed
   *     is greater than or equal to the stop time.
   */
  static void Run (void);

  /**
   * Tell the Simulator the calling event should be the last one
   * executed.
   *
   * If a running event invokes this method, it will be the last
   * event executed by the Simulator::Run method before
   * returning to the caller.
   */
  static void Stop (void);

  /**
   * Schedule the time delay until the Simulator should stop.
   *
   * Force the Simulator::Run method to return to the caller when the
   * expiration time of the next event to be processed is greater than
   * or equal to the stop time.  The stop time is relative to the
   * current simulation time.
   * @param [in] delay The stop time, relative to the current time.
   */
  static void Stop (const Time &delay);

  /**
   * Get the current simulation context.
   *
   * The simulation context is the ns-3 notion of a Logical Process.
   * Events in a single context should only modify state associated with
   * that context. Events for objects in other contexts should be
   * scheduled with ScheduleWithContext() to track the context switches.
   * In other words, events in different contexts should be mutually
   * thread safe, by not modify overlapping model state.
   *
   * In circumstances where the context can't be determined, such as
   * during object initialization, the \c enum value \c NO_CONTEXT
   * should be used.
   *
   * @return The current simulation context
   */
  static uint32_t GetContext (void);

  /** Context enum values. */
  enum {
    /**
     * Flag for events not associated with any particular context.
     */
    NO_CONTEXT = 0xffffffff
  };
  
  /**
   * @name Schedule events (in the same context) to run at a future time.
   */
  /** @{ */
  /**
   * Schedule an event to expire after @p delay.
   * This can be thought of as scheduling an event
   * for the current simulation time plus the @p delay  passed as a
   * parameter.
   *
   * When the event expires (when it becomes due to be run), the 
   * input method will be invoked on the input object.
   *
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @param [in] delay The relative expiration time of the event.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @returns The id for the scheduled event.
   */
  template <typename MEM, typename OBJ>
  static EventId Schedule (Time const &delay, MEM mem_ptr, OBJ obj);

  /**
   * @see Schedule(const Time&,MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @param [in] delay The relative expiration time of the event.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @returns The id for the scheduled event.
   */
  template <typename MEM, typename OBJ, typename T1>
  static EventId Schedule (Time const &delay, MEM mem_ptr, OBJ obj, T1 a1);

  /**
   * @see Schedule(const Time&,MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @param [in] delay The relative expiration time of the event.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @returns The id for the scheduled event.
   */
  template <typename MEM, typename OBJ, typename T1, typename T2>
  static EventId Schedule (Time const &delay, MEM mem_ptr, OBJ obj, T1 a1, T2 a2);

  /**
   * @see Schedule(const Time&,MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @tparam T3 @deduced Type of third argument.
   * @param [in] delay The relative expiration time of the event.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @param [in] a3 The third argument to pass to the invoked method
   * @returns The id for the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3>
  static EventId Schedule (Time const &delay, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3);

  /**
   * @see Schedule(const Time&,MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @tparam T3 @deduced Type of third argument.
   * @tparam T4 @deduced Type of fourth argument.
   * @param [in] delay The relative expiration time of the event.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @param [in] a3 The third argument to pass to the invoked method
   * @param [in] a4 The fourth argument to pass to the invoked method
   * @returns The id for the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4>
  static EventId Schedule (Time const &delay, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * @see Schedule(const Time&,MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @tparam T3 @deduced Type of third argument.
   * @tparam T4 @deduced Type of fourth argument.
   * @tparam T5 @deduced Type of fifth argument.
   * @param [in] delay The relative expiration time of the event.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @param [in] a3 The third argument to pass to the invoked method
   * @param [in] a4 The fourth argument to pass to the invoked method
   * @param [in] a5 The fifth argument to pass to the invoked method
   * @returns The id for the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId Schedule (Time const &delay, MEM mem_ptr, OBJ obj, 
                           T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /**
   * @param time the relative expiration time of the event.
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @param a4 the fourth argument to pass to the invoked method
   * @param a5 the fifth argument to pass to the invoked method
   * @param a6 the sixth argument to pass to the invoked method
   * @returns an id for the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  static EventId Schedule (Time const &time, MEM mem_ptr, OBJ obj, 
                           T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);

  /**
   * Schedule an event to expire after @p delay.
   * This can be thought of as scheduling an event
   * for the current simulation time plus the @p delay  passed as a
   * parameter.
   *
   * When the event expires (when it becomes due to be run), the
   * function will be invoked with any supplied arguments.
   * @param [in] delay The relative expiration time of the event.
   * @param [in] f The function to invoke
   * @returns The id for the scheduled event.
   */
  static EventId Schedule (Time const &delay, void (*f)(void));

  /**
   * @see Schedule(const Time&,(*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @param [in] delay The relative expiration time of the event.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke.
   * @returns The id for the scheduled event.
   */
  template <typename U1, typename T1>
  static EventId Schedule (Time const &delay, void (*f)(U1), T1 a1);

  /**
   * @see Schedule(const Time&,(*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @param [in] delay The relative expiration time of the event.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @returns The id for the scheduled event.
   */
  template <typename U1, typename U2,
            typename T1, typename T2>
  static EventId Schedule (Time const &delay, void (*f)(U1,U2), T1 a1, T2 a2);

  /**
   * @see Schedule(const Time&,void(*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam U3 @deduced Formal type of the third argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @tparam T3 @deduced Actual type of the third argument.
   * @param [in] delay The relative expiration time of the event.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @param [in] a3 The third argument to pass to the function to invoke
   * @returns The id for the scheduled event.
   */
  template <typename U1, typename U2, typename U3,
            typename T1, typename T2, typename T3>
  static EventId Schedule (Time const &delay, void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3);

  /**
   * @see Schedule(const Time&,(*)(void))
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam U3 @deduced Formal type of the third argument to the function.
   * @tparam U4 @deduced Formal type of the fourth argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @tparam T3 @deduced Actual type of the third argument.
   * @tparam T4 @deduced Actual type of the fourth argument.
   * @param [in] delay The relative expiration time of the event.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @param [in] a3 The third argument to pass to the function to invoke
   * @param [in] a4 The fourth argument to pass to the function to invoke
   * @returns The id for the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4, 
            typename T1, typename T2, typename T3, typename T4>
  static EventId Schedule (Time const &delay, void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * @see Schedule(const Time&,void(*)(void))
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam U3 @deduced Formal type of the third argument to the function.
   * @tparam U4 @deduced Formal type of the fourth argument to the function.
   * @tparam U5 @deduced Formal type of the fifth argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @tparam T3 @deduced Actual type of the third argument.
   * @tparam T4 @deduced Actual type of the fourth argument.
   * @tparam T5 @deduced Actual type of the fifth argument.
   * @param [in] delay The relative expiration time of the event.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @param [in] a3 The third argument to pass to the function to invoke
   * @param [in] a4 The fourth argument to pass to the function to invoke
   * @param [in] a5 The fifth argument to pass to the function to invoke
   * @returns The id for the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5,
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId Schedule (Time const &delay, void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /**
   * @param time the relative expiration time of the event.
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   * @param a5 the fifth argument to pass to the function to invoke
   * @param a6 the sixth argument to pass to the function to invoke
   * @returns an id for the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6,
            typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  static EventId Schedule (Time const &time, void (*f)(U1,U2,U3,U4,U5,U6), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);


  /** @} */  // Schedule events (in the same context) to run at a future time.

  /**
   * @name Schedule events (in a different context) to run now or at a future time.
   *
   * See @ref main-test-sync.cc for example usage.
   */
  /** @{ */
  /**
   * Schedule an event with the given context.
   * A context of 0xffffffff means no context is specified.
   * This method is thread-safe: it can be called from any thread.
   *
   * @see Schedule(const Time&,MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @param [in] context User-specified context parameter
   * @param [in] delay The relative expiration time of the event.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   */
  template <typename MEM, typename OBJ>
  static void ScheduleWithContext (uint32_t context, Time const &delay, MEM mem_ptr, OBJ obj);

  /**
   * @see ScheduleWithContext(uint32_t,const Time&,MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @param [in] context User-specified context parameter
   * @param [in] delay The relative expiration time of the event.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, typename T1>
  static void ScheduleWithContext (uint32_t context, Time const &delay, MEM mem_ptr, OBJ obj, T1 a1);

  /**
   * @see ScheduleWithContext(uint32_t,const Time&,MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @param [in] context User-specified context parameter
   * @param [in] delay The relative expiration time of the event.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, typename T1, typename T2>
  static void ScheduleWithContext (uint32_t context, Time const &delay, MEM mem_ptr, OBJ obj, T1 a1, T2 a2);

  /**
   * @see ScheduleWithContext(uint32_t,const Time&,MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @tparam T3 @deduced Type of third argument.
   * @param [in] context User-specified context parameter
   * @param [in] delay The relative expiration time of the event.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @param [in] a3 The third argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3>
  static void ScheduleWithContext (uint32_t context, Time const &delay, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3);

  /**
   * @see ScheduleWithContext(uint32_t,const Time&,MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @tparam T3 @deduced Type of third argument.
   * @tparam T4 @deduced Type of fourth argument.
   * @param [in] context User-specified context parameter
   * @param [in] delay The relative expiration time of the event.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @param [in] a3 The third argument to pass to the invoked method
   * @param [in] a4 The fourth argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4>
  static void ScheduleWithContext (uint32_t context, Time const &delay, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * @see ScheduleWithContext(uint32_t,const Time&,MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @tparam T3 @deduced Type of third argument.
   * @tparam T4 @deduced Type of fourth argument.
   * @tparam T5 @deduced Type of fifth argument.
   * @param [in] context User-specified context parameter
   * @param [in] delay The relative expiration time of the event.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @param [in] a3 The third argument to pass to the invoked method
   * @param [in] a4 The fourth argument to pass to the invoked method
   * @param [in] a5 The fifth argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static void ScheduleWithContext (uint32_t context, Time const &delay, MEM mem_ptr, OBJ obj, 
                                   T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

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
   * @param a6 the sixth argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  static void ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj, 
                                   T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);
  
  /**
   * Schedule an event with the given context.
   * A context of 0xffffffff means no context is specified.
   * This method is thread-safe: it can be called from any thread.
   *
   * When the event expires (when it becomes due to be run), the
   * function will be invoked with any supplied arguments.
   *
   * This method is thread-safe: it can be called from any thread.
   * @param [in] context User-specified context parameter
   * @param [in] delay The relative expiration time of the event.
   * @param [in] f The function to invoke
   */
  static void ScheduleWithContext (uint32_t context, Time const &delay, void (*f)(void));

  /**
   * @see ScheduleWithContext(uint32_t,const Time&,(*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @param [in] context User-specified context parameter
   * @param [in] delay The relative expiration time of the event.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   */
  template <typename U1,
            typename T1>
  static void ScheduleWithContext (uint32_t context, Time const &delay, void (*f)(U1), T1 a1);

  /**
   * @see ScheduleWithContext(uint32_t,const Time&,(*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @param [in] context User-specified context parameter
   * @param [in] delay The relative expiration time of the event.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   */
  template <typename U1, typename U2,
            typename T1, typename T2>
  static void ScheduleWithContext (uint32_t context, Time const &delay, void (*f)(U1,U2), T1 a1, T2 a2);

  /**
   * @see ScheduleWithContext(uint32_t,const Time&,(*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam U3 @deduced Formal type of the third argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @tparam T3 @deduced Actual type of the third argument.
   * @param [in] context User-specified context parameter
   * @param [in] delay The relative expiration time of the event.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @param [in] a3 The third argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3,
            typename T1, typename T2, typename T3>
  static void ScheduleWithContext (uint32_t context, Time const &delay, void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3);

  /**
   * @see ScheduleWithContext(uint32_t,const Time&,(*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam U3 @deduced Formal type of the third argument to the function.
   * @tparam U4 @deduced Formal type of the fourth argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @tparam T3 @deduced Actual type of the third argument.
   * @tparam T4 @deduced Actual type of the fourth argument.
   * @param [in] context User-specified context parameter
   * @param [in] delay The relative expiration time of the event.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @param [in] a3 The third argument to pass to the function to invoke
   * @param [in] a4 The fourth argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3, typename U4, 
            typename T1, typename T2, typename T3, typename T4>
  static void ScheduleWithContext (uint32_t context, Time const &delay, void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * @see ScheduleWithContext(uint32_t,const Time&,(*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam U3 @deduced Formal type of the third argument to the function.
   * @tparam U4 @deduced Formal type of the fourth argument to the function.
   * @tparam U5 @deduced Formal type of the fifth argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @tparam T3 @deduced Actual type of the third argument.
   * @tparam T4 @deduced Actual type of the fourth argument.
   * @tparam T5 @deduced Actual type of the fifth argument.
   * @param [in] context User-specified context parameter
   * @param [in] delay The relative expiration time of the event.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @param [in] a3 The third argument to pass to the function to invoke
   * @param [in] a4 The fourth argument to pass to the function to invoke
   * @param [in] a5 The fifth argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5,
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static void ScheduleWithContext (uint32_t context, Time const &delay, void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

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
   * @param a6 the sixth argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6,
            typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  static void ScheduleWithContext (uint32_t context, Time const &time, void (*f)(U1,U2,U3,U4,U5,U6), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);

  /** @} */  // Schedule events (in a different context) to run now or at a future time.
  
  /**
   * @name Schedule events (in the same context) to run now.
   */
  /** @{ */
  /**
   * Schedule an event to expire Now. All events scheduled to
   * to expire "Now" are scheduled FIFO, after all normal events
   * have expired. 
   *
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj);

  /**
   * @see ScheduleNow(MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1);

  /**
   * @see ScheduleNow(MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1, T2 a2);

  /**
   * @see ScheduleNow(MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @tparam T3 @deduced Type of third argument.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @param [in] a3 The third argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3);

  /**
   * @see ScheduleNow(MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @tparam T3 @deduced Type of third argument.
   * @tparam T4 @deduced Type of fourth argument.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @param [in] a3 The third argument to pass to the invoked method
   * @param [in] a4 The fourth argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, 
                              T1 a1, T2 a2, T3 a3, T4 a4);
  /**
   * @see ScheduleNow(MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @tparam T3 @deduced Type of third argument.
   * @tparam T4 @deduced Type of fourth argument.
   * @tparam T5 @deduced Type of fifth argument.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @param [in] a3 The third argument to pass to the invoked method
   * @param [in] a4 The fourth argument to pass to the invoked method
   * @param [in] a5 The fifth argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, 
                              T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @param a4 the fourth argument to pass to the invoked method
   * @param a5 the fifth argument to pass to the invoked method
   * @param a6 the sixth argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, 
                              T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);

  /**
   * @copybrief ScheduleNow(MEM,OBJ)
   *
   * When the event expires (when it becomes due to be run), the
   * function will be invoked with any supplied arguments.
   * @param [in] f The function to invoke
   * @return The EventId of the scheduled event.
   */
  static EventId ScheduleNow (void (*f)(void));

  /**
   * @see ScheduleNow(*)
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1,
            typename T1>
  static EventId ScheduleNow (void (*f)(U1), T1 a1);

  /**
   * @see ScheduleNow(*)
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2,
            typename T1, typename T2>
  static EventId ScheduleNow (void (*f)(U1,U2), T1 a1, T2 a2);

  /**
   * @see ScheduleNow(*)
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam U3 @deduced Formal type of the third argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @tparam T3 @deduced Actual type of the third argument.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @param [in] a3 The third argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3,
            typename T1, typename T2, typename T3>
  static EventId ScheduleNow (void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3);

  /**
   * @see ScheduleNow(*)
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam U3 @deduced Formal type of the third argument to the function.
   * @tparam U4 @deduced Formal type of the fourth argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @tparam T3 @deduced Actual type of the third argument.
   * @tparam T4 @deduced Actual type of the fourth argument.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @param [in] a3 The third argument to pass to the function to invoke
   * @param [in] a4 The fourth argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4,
            typename T1, typename T2, typename T3, typename T4>
  static EventId ScheduleNow (void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * @see ScheduleNow(*)
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam U3 @deduced Formal type of the third argument to the function.
   * @tparam U4 @deduced Formal type of the fourth argument to the function.
   * @tparam U5 @deduced Formal type of the fifth argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @tparam T3 @deduced Actual type of the third argument.
   * @tparam T4 @deduced Actual type of the fourth argument.
   * @tparam T5 @deduced Actual type of the fifth argument.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @param [in] a3 The third argument to pass to the function to invoke
   * @param [in] a4 The fourth argument to pass to the function to invoke
   * @param [in] a5 The fifth argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5,
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId ScheduleNow (void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   * @param a5 the fifth argument to pass to the function to invoke
   * @param a6 the sixth argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6,
            typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  static EventId ScheduleNow (void (*f)(U1,U2,U3,U4,U5,U6), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);

  /** @} */  // Schedule events (in the same context) to run now.

  /**
   * @name Schedule events to run at the end of the simulation, when Simulator:Destroy() is called.
   */
  /** @{ */
  /**
   * Schedule an event to expire when Simulator::Destroy is called.
   * All events scheduled to expire at "Destroy" time are scheduled FIFO, 
   * after all normal events have expired and only when 
   * Simulator::Destroy is invoked.
   *
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj);

  /**
   * @see ScheduleDestroy(MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1);

  /**
   * @see ScheduleDestroy(MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ,
            typename T1, typename T2>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1, T2 a2);

  /**
   * @see ScheduleDestroy(MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @tparam T3 @deduced Type of third argument.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @param [in] a3 The third argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3);

  /**
   * @see ScheduleDestroy(MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @tparam T3 @deduced Type of third argument.
   * @tparam T4 @deduced Type of fourth argument.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @param [in] a3 The third argument to pass to the invoked method
   * @param [in] a4 The fourth argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, 
                                  T1 a1, T2 a2, T3 a3, T4 a4);
  /**
   * @see ScheduleDestroy(MEM,OBJ)
   * @tparam MEM @deduced Class method function signature type.
   * @tparam OBJ @deduced Class type of the object.
   * @tparam T1 @deduced Type of first argument.
   * @tparam T2 @deduced Type of second argument.
   * @tparam T3 @deduced Type of third argument.
   * @tparam T4 @deduced Type of fourth argument.
   * @tparam T5 @deduced Type of fifth argument.
   * @param [in] mem_ptr Member method pointer to invoke
   * @param [in] obj The object on which to invoke the member method
   * @param [in] a1 The first argument to pass to the invoked method
   * @param [in] a2 The second argument to pass to the invoked method
   * @param [in] a3 The third argument to pass to the invoked method
   * @param [in] a4 The fourth argument to pass to the invoked method
   * @param [in] a5 The fifth argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, 
                                  T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
   * @param a3 the third argument to pass to the invoked method
   * @param a4 the fourth argument to pass to the invoked method
   * @param a5 the fifth argument to pass to the invoked method
   * @param a6 the sixth argument to pass to the invoked method
   * @return The EventId of the scheduled event.
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, 
                                  T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);

  /**
   * @copybrief ScheduleDestroy(MEM,OBJ)
   * When Simulator::Destroy() is called, the
   * function will be invoked with any supplied arguments.
   * @param [in] f The function to invoke
   * @return The EventId of the scheduled event.
   */
  static EventId ScheduleDestroy (void (*f)(void));

  /**
   * @see ScheduleDestory((*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1,
            typename T1>
  static EventId ScheduleDestroy (void (*f)(U1), T1 a1);

  /**
   * @see ScheduleDestory((*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2,
            typename T1, typename T2>
  static EventId ScheduleDestroy (void (*f)(U1,U2), T1 a1, T2 a2);

  /**
   * @see ScheduleDestory((*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam U3 @deduced Formal type of the third argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @tparam T3 @deduced Actual type of the third argument.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @param [in] a3 The third argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3,
            typename T1, typename T2, typename T3>
  static EventId ScheduleDestroy (void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3);

  /**
   * @see ScheduleDestory((*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam U3 @deduced Formal type of the third argument to the function.
   * @tparam U4 @deduced Formal type of the fourth argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @tparam T3 @deduced Actual type of the third argument.
   * @tparam T4 @deduced Actual type of the fourth argument.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @param [in] a3 The third argument to pass to the function to invoke
   * @param [in] a4 The fourth argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4,
            typename T1, typename T2, typename T3, typename T4>
  static EventId ScheduleDestroy (void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * @see ScheduleDestory((*)())
   * @tparam U1 @deduced Formal type of the first argument to the function.
   * @tparam U2 @deduced Formal type of the second argument to the function.
   * @tparam U3 @deduced Formal type of the third argument to the function.
   * @tparam U4 @deduced Formal type of the fourth argument to the function.
   * @tparam U5 @deduced Formal type of the fifth argument to the function.
   * @tparam T1 @deduced Actual type of the first argument.
   * @tparam T2 @deduced Actual type of the second argument.
   * @tparam T3 @deduced Actual type of the third argument.
   * @tparam T4 @deduced Actual type of the fourth argument.
   * @tparam T5 @deduced Actual type of the fifth argument.
   * @param [in] f The function to invoke
   * @param [in] a1 The first argument to pass to the function to invoke
   * @param [in] a2 The second argument to pass to the function to invoke
   * @param [in] a3 The third argument to pass to the function to invoke
   * @param [in] a4 The fourth argument to pass to the function to invoke
   * @param [in] a5 The fifth argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5,
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId ScheduleDestroy (void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   * @param a5 the fifth argument to pass to the function to invoke
   * @param a6 the sixth argument to pass to the function to invoke
   * @return The EventId of the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6,
            typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  static EventId ScheduleDestroy (void (*f)(U1,U2,U3,U4,U5,U6), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);

  /** @} */  // Schedule events to run when Simulator:Destroy() is called.

  /**
   * Remove an event from the event list. 
   * 
   * This method has the same visible effect as the 
   * ns3::EventId::Cancel method
   * but its algorithmic complexity is much higher: it has often 
   * O(log(n)) complexity, sometimes O(n), sometimes worse.
   * Note that it is not possible to remove events which were scheduled
   * for the "destroy" time. Doing so will result in a program error (crash).
   *
   * @param [in] id The event to remove from the list of scheduled events.
   */
  static void Remove (const EventId &id);

  /**
   * Set the cancel bit on this event: the event's associated function
   * will not be invoked when it expires.
   *
   * This method has the same visible effect as the 
   * ns3::Simulator::Remove method but its algorithmic complexity is 
   * much lower: it has O(1) complexity.
   * This method has the exact same semantics as ns3::EventId::Cancel.
   * Note that it is not possible to cancel events which were scheduled
   * for the "destroy" time. Doing so will result in a program error (crash).
   * 
   * @param [in] id the event to cancel
   */
  static void Cancel (const EventId &id);

  /**
   * Check if an event has already run or been cancelled.
   *
   * This method has O(1) complexity.
   * Note that it is not possible to test for the expiration of
   * events which were scheduled for the "destroy" time. Doing so
   * will result in a program error (crash).
   * An event is said to "expire" when it starts being scheduled
   * which means that if the code executed by the event calls
   * this function, it will get true.
   *
   * @param [in] id The event to test for expiration.
   * @returns @c true if the event has expired, false otherwise.
   */
  static bool IsExpired (const EventId &id);

  /**
   * Return the current simulation virtual time.
   *
   * @returns The current virtual time.
   */
  static Time Now (void);

  /**
   * Get the remaining time until this event will execute.
   *
   * @param [in] id The event id to analyse.
   * @return The delay left until the input event id expires.
   *          if the event is not running, this method returns
   *          zero.
   */
  static Time GetDelayLeft (const EventId &id);

  /**
   * Get the maximum representable simulation time.
   *
   * @return The maximum simulation time at which an event 
   *          can be scheduled.
   *
   * The returned value will always be bigger than or equal to Simulator::Now.
   */
  static Time GetMaximumSimulationTime (void);

  /**
   * Schedule a future event execution (in the same context).
   *
   * @param [in] delay Delay until the event expires.
   * @param [in] event The event to schedule.
   * @returns A unique identifier for the newly-scheduled event.
   */
  static EventId Schedule (const Time &delay, const Ptr<EventImpl> &event);

  /**
   * Schedule a future event execution (in a different context).
   * This method is thread-safe: it can be called from any thread.
   *
   * @param [in] delay Delay until the event expires.
   * @param [in] context Event context.
   * @param [in] event The event to schedule.
   * @returns A unique identifier for the newly-scheduled event.
   */
  static void ScheduleWithContext (uint32_t context, const Time &delay, EventImpl *event);

  /**
   * Schedule an event to run at the end of the simulation, after
   * the Stop() time or condition has been reached.
   *
   * @param [in] event The event to schedule.
   * @returns A unique identifier for the newly-scheduled event.
   */
  static EventId ScheduleDestroy (const Ptr<EventImpl> &event);

  /**
   * Schedule an event to run at the current virtual time.
   *
   * @param [in] event The event to schedule.
   * @returns A unique identifier for the newly-scheduled event.
   */
  static EventId ScheduleNow (const Ptr<EventImpl> &event);

  /**
   * Get the system id of this simulator.
   *
   * The system id is the identifier for this simulator instance
   * in a distributed simulation.  For MPI this is the MPI rank.
   * @return The system id for this simulator.
   */
  static uint32_t GetSystemId (void);
  
private:
  /** Default constructor. */
  Simulator ();
  /** Destructor. */
  ~Simulator ();

  /**
   * Implementation of the various Schedule methods.
   * @param [in] delay Delay until the event should execute.
   * @param [in] event The event to execute.
   * @return The EventId.
   */
  static EventId DoSchedule (Time const &delay, EventImpl *event);
  /**
   * Implementation of the various ScheduleNow methods.
   * @param [in] event The event to execute.
   * @return The EventId.
   */
  static EventId DoScheduleNow (EventImpl *event);
  /**
   * Implementation of the various ScheduleDestroy methods.
   * @param [in] event The event to execute.
   * @return The EventId.
   */
  static EventId DoScheduleDestroy (EventImpl *event);
};

/**
 * @ingroup simulator
 * @brief create an ns3::Time instance which contains the
 *        current simulation time.
 *
 * This is really a shortcut for the ns3::Simulator::Now method.
 * It is typically used as shown below to schedule an event
 * which expires at the absolute time "2 seconds":
 * @code
 *   Simulator::Schedule (Seconds (2.0) - Now (), &my_function);
 * @endcode
 * @return The current simulation time.
 */
Time Now (void);

} // namespace ns3


/********************************************************************
 *  Implementation of the templates declared above.
 ********************************************************************/

namespace ns3 {

// Doxygen has trouble with static template functions in a class:
// it treats the in-class declaration as different from the
// out of class definition, so makes two entries in the member list.  Ugh
  
template <typename MEM, typename OBJ>
EventId Simulator::Schedule (Time const &delay, MEM mem_ptr, OBJ obj) 
{
  return DoSchedule (delay, MakeEvent (mem_ptr, obj));
}


template <typename MEM, typename OBJ,
          typename T1>
EventId Simulator::Schedule (Time const &delay, MEM mem_ptr, OBJ obj, T1 a1) 
{
  return DoSchedule (delay, MakeEvent (mem_ptr, obj, a1));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2>
EventId Simulator::Schedule (Time const &delay, MEM mem_ptr, OBJ obj, T1 a1, T2 a2)
{
  return DoSchedule (delay, MakeEvent (mem_ptr, obj, a1, a2));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3>
EventId Simulator::Schedule (Time const &delay, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3) 
{
  return DoSchedule (delay, MakeEvent (mem_ptr, obj, a1, a2, a3));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4>
EventId Simulator::Schedule (Time const &delay, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4) 
{
  return DoSchedule (delay, MakeEvent (mem_ptr, obj, a1, a2, a3, a4));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId Simulator::Schedule (Time const &delay, MEM mem_ptr, OBJ obj, 
                             T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
  return DoSchedule (delay, MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj, 
                             T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) 
{
  return DoSchedule (time, MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5, a6));
}

template <typename U1,
          typename T1>
EventId Simulator::Schedule (Time const &delay, void (*f)(U1), T1 a1)
{
  return DoSchedule (delay, MakeEvent (f, a1));
}

template <typename U1, typename U2, 
          typename T1, typename T2>
EventId Simulator::Schedule (Time const &delay, void (*f)(U1,U2), T1 a1, T2 a2)
{
  return DoSchedule (delay, MakeEvent (f, a1, a2));
}

template <typename U1, typename U2, typename U3,
          typename T1, typename T2, typename T3>
EventId Simulator::Schedule (Time const &delay, void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3)
{
  return DoSchedule (delay, MakeEvent (f, a1, a2, a3));
}

template <typename U1, typename U2, typename U3, typename U4,
          typename T1, typename T2, typename T3, typename T4>
EventId Simulator::Schedule (Time const &delay, void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4)
{
  return DoSchedule (delay, MakeEvent (f, a1, a2, a3, a4));
}

template <typename U1, typename U2, typename U3, typename U4, typename U5,
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId Simulator::Schedule (Time const &delay, void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  return DoSchedule (delay, MakeEvent (f, a1, a2, a3, a4, a5));
}

template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6,
          typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
EventId Simulator::Schedule (Time const &time, void (*f)(U1,U2,U3,U4,U5,U6), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  return DoSchedule (time, MakeEvent (f, a1, a2, a3, a4, a5, a6));
}


template <typename MEM, typename OBJ>
void Simulator::ScheduleWithContext (uint32_t context, Time const &delay, MEM mem_ptr, OBJ obj)
{
  ScheduleWithContext (context, delay, MakeEvent (mem_ptr, obj));
}


template <typename MEM, typename OBJ,
          typename T1>
void Simulator::ScheduleWithContext (uint32_t context, Time const &delay, MEM mem_ptr, OBJ obj, T1 a1)
{
  return ScheduleWithContext (context, delay, MakeEvent (mem_ptr, obj, a1));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2>
void Simulator::ScheduleWithContext (uint32_t context, Time const &delay, MEM mem_ptr, OBJ obj, T1 a1, T2 a2)
{
  return ScheduleWithContext (context, delay, MakeEvent (mem_ptr, obj, a1, a2));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3>
void Simulator::ScheduleWithContext (uint32_t context, Time const &delay, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3)
{
  return ScheduleWithContext (context, delay, MakeEvent (mem_ptr, obj, a1, a2, a3));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3, typename T4>
void Simulator::ScheduleWithContext (uint32_t context, Time const &delay, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4)
{
  return ScheduleWithContext (context, delay, MakeEvent (mem_ptr, obj, a1, a2, a3, a4));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3, typename T4, typename T5>
void Simulator::ScheduleWithContext (uint32_t context, Time const &delay, MEM mem_ptr, OBJ obj,
                                     T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  return ScheduleWithContext (context, delay, MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj,
                                     T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  return ScheduleWithContext (context, time, MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5, a6));
}

template <typename U1,
          typename T1>
void Simulator::ScheduleWithContext (uint32_t context, Time const &delay, void (*f)(U1), T1 a1)
{
  return ScheduleWithContext (context, delay, MakeEvent (f, a1));
}

template <typename U1, typename U2,
          typename T1, typename T2>
void Simulator::ScheduleWithContext (uint32_t context, Time const &delay, void (*f)(U1,U2), T1 a1, T2 a2)
{
  return ScheduleWithContext (context, delay, MakeEvent (f, a1, a2));
}

template <typename U1, typename U2, typename U3,
          typename T1, typename T2, typename T3>
void Simulator::ScheduleWithContext (uint32_t context, Time const &delay, void (*f)(U1,U2,U3), T1 a1, T2 a2, T3 a3)
{
  return ScheduleWithContext (context, delay, MakeEvent (f, a1, a2, a3));
}

template <typename U1, typename U2, typename U3, typename U4,
          typename T1, typename T2, typename T3, typename T4>
void Simulator::ScheduleWithContext (uint32_t context, Time const &delay, void (*f)(U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4)
{
  return ScheduleWithContext (context, delay, MakeEvent (f, a1, a2, a3, a4));
}

template <typename U1, typename U2, typename U3, typename U4, typename U5,
          typename T1, typename T2, typename T3, typename T4, typename T5>
void Simulator::ScheduleWithContext (uint32_t context, Time const &delay, void (*f)(U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
{
  return ScheduleWithContext (context, delay, MakeEvent (f, a1, a2, a3, a4, a5));
}

template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6,
          typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void Simulator::ScheduleWithContext (uint32_t context, Time const &time, void (*f)(U1,U2,U3,U4,U5,U6), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  return ScheduleWithContext (context, time, MakeEvent (f, a1, a2, a3, a4, a5, a6));
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

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj, 
                        T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) 
{
  return DoScheduleNow (MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5, a6));
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

template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6,
          typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
EventId
Simulator::ScheduleNow (void (*f)(U1,U2,U3,U4,U5,U6), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  return DoScheduleNow (MakeEvent (f, a1, a2, a3, a4, a5, a6));
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

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj, 
                            T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) 
{
  return DoScheduleDestroy (MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5, a6));
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

template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6,
          typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
EventId
Simulator::ScheduleDestroy (void (*f)(U1,U2,U3,U4,U5,U6), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
{
  return DoScheduleDestroy (MakeEvent (f, a1, a2, a3, a4, a5, a6));
}

} // namespace ns3

#endif /* SIMULATOR_H */
