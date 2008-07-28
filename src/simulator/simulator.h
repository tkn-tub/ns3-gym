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
#include "nstime.h"
#include "scheduler.h"

#include "ns3/type-traits.h"
#include "ns3/deprecated.h"

#include <stdint.h>
#include <string>

namespace ns3 {

class SimulatorImpl;

/**
 * \ingroup simulator
 *
 * \brief Control the scheduling of simulation events. 
 *
 * The internal simulation clock is maintained
 * as a 64-bit integer in a unit specified by the user
 * through the TimeStepPrecision::Set function. This means that it is
 * not possible to specify event expiration times with anything better
 * than this user-specified accuracy. Events whose expiration time is
 * the same modulo this accuracy are scheduled in FIFO order: the 
 * first event inserted in the scheduling queue is scheduled to 
 * expire first.
 * 
 * A simple example of how to use the Simulator class to schedule events
 * is shown below:
 * \include samples/main-simulator.cc
 */
class Simulator 
{
public:
  /**
   * \param impl a new simulator implementation
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
   * \param scheduler a new event scheduler
   *
   * The event scheduler can be set at any time: the events scheduled
   * in the previous scheduler will be transfered to the new scheduler
   * before we start to use it.
   */
  static void SetScheduler (Ptr<Scheduler> scheduler);

  static void EnableLogTo (char const *filename) NS_DEPRECATED;

  /**
   * Every event scheduled by the Simulator::insertAtDestroy method is
   * invoked. Then, we ensure that any memory allocated by the 
   * Simulator is freed.
   * This method is typically invoked at the end of a simulation
   * to avoid false-positive reports by a leak checker.
   * After this method has been invoked, it is actually possible
   * to restart a new simulation with a set of calls to Simulator::run
   * and Simulator::insert_*.
   */
  static void Destroy (void);

  /**
   * If there any any events lefts to be scheduled, return
   * true. Return false otherwise.
   */
  static bool IsFinished (void);
  /**
   * If Simulator::isFinished returns true, the behavior of this
   * method is undefined. Otherwise, it returns the microsecond-based
   * time of the next event expected to be scheduled.
   */
  static Time Next (void);

  /**
   * Run the simulation until one of:
   *   - no events are present anymore
   *   - the user called Simulator::stop
   *   - the user called Simulator::stopAtUs and the
   *     expiration time of the next event to be processed
   *     is greater than or equal to the stop time.
   */
  static void Run (void);

  /**
   * If an event invokes this method, it will be the last
   * event scheduled by the Simulator::run method before
   * returning to the caller.
   */
  static void Stop (void);

  /**
   * Force the Simulator::run method to return to the caller when the
   * expiration time of the next event to be processed is greater than
   * or equal to the stop time.  The stop time is relative to the
   * current simulation time.
   * @param time the stop time, relative to the current time.
   */
  static void Stop (Time const &time);

  /**
   * Schedule an event to expire when the time "now + time" 
   * is reached. When the event expires, the input method 
   * will be invoked on the input object.
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
  static EventId Schedule (Time const &time, void (*f) (void));

  /**
   * @param time the relative expiration time of the event.
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @returns an id for the scheduled event.
   */
  template <typename U1, typename T1>
  static EventId Schedule (Time const &time, void (*f) (U1), T1 a1);

  /**
   * @param time the relative expiration time of the event.
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @returns an id for the scheduled event.
   */
  template <typename U1, typename U2, typename T1, typename T2>
  static EventId Schedule (Time const &time, void (*f) (U1,U2), T1 a1, T2 a2);

  /**
   * @param time the relative expiration time of the event.
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @returns an id for the scheduled event.
   */
  template <typename U1, typename U2, typename U3, typename T1, typename T2, typename T3>
  static EventId Schedule (Time const &time, void (*f) (U1,U2,U3), T1 a1, T2 a2, T3 a3);

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
  static EventId Schedule (Time const &time, void (*f) (U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);

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
  static EventId Schedule (Time const &time, void (*f) (U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /**
   * Schedule an event to expire Now. All events scheduled to
   * to expire "Now" are scheduled FIFO, after all normal events
   * have expired. 
   *
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   */
  template <typename MEM, typename OBJ>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, 
            typename T1>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
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
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId ScheduleNow (MEM mem_ptr, OBJ obj, 
                              T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
  /**
   * @param f the function to invoke
   */
  static EventId ScheduleNow (void (*f) (void));

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   */
  template <typename U1,
            typename T1>
  static EventId ScheduleNow (void (*f) (U1), T1 a1);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   */
  template <typename U1, typename U2,
            typename T1, typename T2>
  static EventId ScheduleNow (void (*f) (U1,U2), T1 a1, T2 a2);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3,
            typename T1, typename T2, typename T3>
  static EventId ScheduleNow (void (*f) (U1,U2,U3), T1 a1, T2 a2, T3 a3);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3, typename U4,
            typename T1, typename T2, typename T3, typename T4>
  static EventId ScheduleNow (void (*f) (U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   * @param a5 the fifth argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5,
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId ScheduleNow (void (*f) (U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /**
   * Schedule an event to expire at Destroy time. All events 
   * scheduled to expire at "Destroy" time are scheduled FIFO, 
   * after all normal events have expired and only when 
   * Simulator::Destroy is invoked.
   *
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   */
  template <typename MEM, typename OBJ>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   */
  template <typename MEM, typename OBJ, 
            typename T1>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1);

  /**
   * @param mem_ptr member method pointer to invoke
   * @param obj the object on which to invoke the member method
   * @param a1 the first argument to pass to the invoked method
   * @param a2 the second argument to pass to the invoked method
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
   */
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId ScheduleDestroy (MEM mem_ptr, OBJ obj, 
                                  T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
  /**
   * @param f the function to invoke
   */
  static EventId ScheduleDestroy (void (*f) (void));

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   */
  template <typename U1,
            typename T1>
  static EventId ScheduleDestroy (void (*f) (U1), T1 a1);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   */
  template <typename U1, typename U2,
            typename T1, typename T2>
  static EventId ScheduleDestroy (void (*f) (U1,U2), T1 a1, T2 a2);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3,
            typename T1, typename T2, typename T3>
  static EventId ScheduleDestroy (void (*f) (U1,U2,U3), T1 a1, T2 a2, T3 a3);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3, typename U4,
            typename T1, typename T2, typename T3, typename T4>
  static EventId ScheduleDestroy (void (*f) (U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);

  /**
   * @param f the function to invoke
   * @param a1 the first argument to pass to the function to invoke
   * @param a2 the second argument to pass to the function to invoke
   * @param a3 the third argument to pass to the function to invoke
   * @param a4 the fourth argument to pass to the function to invoke
   * @param a5 the fifth argument to pass to the function to invoke
   */
  template <typename U1, typename U2, typename U3, typename U4, typename U5,
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static EventId ScheduleDestroy (void (*f) (U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  /**
   * Remove an event from the event list. 
   * This method has the same visible effect as the 
   * ns3::EventId::Cancel method
   * but its algorithmic complexity is much higher: it has often 
   * O(log(n)) complexity, sometimes O(n), sometimes worse.
   * Note that it is not possible to remove events which were scheduled
   * for the "destroy" time. Doing so will result in a program error (crash).
   *
   * @param id the event to remove from the list of scheduled events.
   */
  static void Remove (const EventId &id);

  /**
   * Set the cancel bit on this event: the event's associated function
   * will not be invoked when it expires. 
   * This method has the same visible effect as the 
   * ns3::Simulator::remove method but its algorithmic complexity is 
   * much lower: it has O(1) complexity.
   * This method has the exact same semantics as ns3::EventId::cancel.
   * Note that it is not possible to cancel events which were scheduled
   * for the "destroy" time. Doing so will result in a program error (crash).
   * 
   * @param id the event to cancel
   */
  static void Cancel (const EventId &id);

  /**
   * This method has O(1) complexity.
   * Note that it is not possible to test for the expiration of
   * events which were scheduled for the "destroy" time. Doing so
   * will result in a program error (crash).
   * An event is said to "expire" when it starts being scheduled
   * which means that if the code executed by the event calls
   * this function, it will get true.
   *
   * @param id the event to test for expiration
   * @returns true if the event has expired, false otherwise.
   */
  static bool IsExpired (const EventId &id);

  /**
   * Return the "current simulation time".
   */
  static Time Now (void);

  /**
   * \param id the event id to analyse
   * \returns the delay left until the input event id expires.
   *          if the event is not running, this method returns
   *          zero.
   */
  static Time GetDelayLeft (const EventId &id);

  /**
   * \returns the maximum simulation time at which an event 
   *          can be scheduled.
   *
   * The returned value will always be bigger than or equal to Simulator::Now.
   */
  static Time GetMaximumSimulationTime (void);

  /**
   * \param time delay until the event expires
   * \param event the event to schedule
   * \returns a unique identifier for the newly-scheduled event.
   *
   * This method will be typically used by language bindings
   * to delegate events to their own subclass of the EventImpl base class.
   */
  static EventId Schedule (Time const &time, const Ptr<EventImpl> &event);  

  /**
   * \param event the event to schedule
   * \returns a unique identifier for the newly-scheduled event.
   *
   * This method will be typically used by language bindings
   * to delegate events to their own subclass of the EventImpl base class.
   */
  static EventId ScheduleDestroy (const Ptr<EventImpl> &event);

  /**
   * \param event the event to schedule
   * \returns a unique identifier for the newly-scheduled event.
   *
   * This method will be typically used by language bindings
   * to delegate events to their own subclass of the EventImpl base class.
   */
  static EventId ScheduleNow (const Ptr<EventImpl> &event);
private:
  Simulator ();
  ~Simulator ();

  template <typename MEM, typename OBJ>
  static Ptr<EventImpl> MakeEvent (MEM mem_ptr, OBJ obj);
  template <typename MEM, typename OBJ, 
            typename T1>
  static Ptr<EventImpl> MakeEvent (MEM mem_ptr, OBJ obj, T1 a1);
  template <typename MEM, typename OBJ, 
            typename T1, typename T2>
  static Ptr<EventImpl> MakeEvent (MEM mem_ptr, OBJ obj, T1 a1, T2 a2);
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3>
  static Ptr<EventImpl> MakeEvent (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3);
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4>
  static Ptr<EventImpl> MakeEvent (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4);
  template <typename MEM, typename OBJ, 
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static Ptr<EventImpl> MakeEvent (MEM mem_ptr, OBJ obj, 
                                   T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
  static Ptr<EventImpl> MakeEvent (void (*f) (void));
  template <typename U1, 
            typename T1>
  static Ptr<EventImpl> MakeEvent (void (*f) (U1), T1 a1);
  template <typename U1, typename U2, 
            typename T1, typename T2>
  static Ptr<EventImpl> MakeEvent (void (*f) (U1,U2), T1 a1, T2 a2);
  template <typename U1, typename U2, typename U3,
            typename T1, typename T2, typename T3>
  static Ptr<EventImpl> MakeEvent (void (*f) (U1,U2,U3), T1 a1, T2 a2, T3 a3);
  template <typename U1, typename U2, typename U3, typename U4,
            typename T1, typename T2, typename T3, typename T4>
  static Ptr<EventImpl> MakeEvent (void (*f) (U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4);
  template <typename U1, typename U2, typename U3, typename U4, typename U5,
            typename T1, typename T2, typename T3, typename T4, typename T5>
  static Ptr<EventImpl> MakeEvent (void (*f) (U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);

  static SimulatorImpl *GetImpl (void);
  static Ptr<SimulatorImpl> m_impl;
  static std::string m_typeImpl;
  static std::string m_typeSched;
};

/**
 * \brief create an ns3::Time instance which contains the
 *        current simulation time.
 *
 * This is really a shortcut for the ns3::Simulator::Now method.
 * It is typically used as shown below to schedule an event
 * which expires at the absolute time "2 seconds":
 * \code
 * Simulator::Schedule (Seconds (2.0) - Now (), &my_function);
 * \endcode
 */
Time Now (void);

}; // namespace ns3


/********************************************************************
   Implementation of templates defined above
 ********************************************************************/

namespace ns3 {

template <typename T>
struct EventMemberImplObjTraits;

template <typename T>
struct EventMemberImplObjTraits<T *>
{
  static T &GetReference (T *p) {
    return *p;
  }
};

template <typename MEM, typename OBJ>
Ptr<EventImpl> Simulator::MakeEvent (MEM mem_ptr, OBJ obj) 
{
  // zero argument version
  class EventMemberImpl0 : public EventImpl {
  public:
    EventMemberImpl0 (OBJ obj, MEM function) 
      : m_obj (obj), 
        m_function (function)
    {}
    virtual ~EventMemberImpl0 () {}
  private:
    virtual void Notify (void) { 
      (EventMemberImplObjTraits<OBJ>::GetReference (m_obj).*m_function) (); 
    }
    OBJ m_obj;
    MEM m_function;
  } * ev = new EventMemberImpl0 (obj, mem_ptr);
  return Ptr<EventImpl> (ev, false);
}


template <typename MEM, typename OBJ, 
          typename T1>
Ptr<EventImpl> Simulator::MakeEvent (MEM mem_ptr, OBJ obj, T1 a1)
{
  // one argument version
  class EventMemberImpl1 : public EventImpl {
  public:
    EventMemberImpl1 (OBJ obj, MEM function, T1 a1) 
      : m_obj (obj), 
        m_function (function),
        m_a1 (a1)
    {}
  protected:
    virtual ~EventMemberImpl1 () {}
  private:
    virtual void Notify (void) { 
      (EventMemberImplObjTraits<OBJ>::GetReference (m_obj).*m_function) (m_a1);
    }
    OBJ m_obj;
    MEM m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
  } *ev = new EventMemberImpl1 (obj, mem_ptr, a1);
  return Ptr<EventImpl> (ev, false);
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2>
Ptr<EventImpl> Simulator::MakeEvent (MEM mem_ptr, OBJ obj, T1 a1, T2 a2) 
{
  // two argument version
  class EventMemberImpl2 : public EventImpl {
  public:
    EventMemberImpl2 (OBJ obj, MEM function, T1 a1, T2 a2) 
      : m_obj (obj), 
        m_function (function),
        m_a1 (a1),
        m_a2 (a2)
    { }
  protected:
    virtual ~EventMemberImpl2 () {}
  private:
    virtual void Notify (void) { 
      (EventMemberImplObjTraits<OBJ>::GetReference (m_obj).*m_function) (m_a1, m_a2);
    }
    OBJ m_obj;
    MEM m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
  } *ev = new EventMemberImpl2 (obj, mem_ptr, a1, a2);
  return Ptr<EventImpl> (ev, false);
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3>
Ptr<EventImpl> Simulator::MakeEvent (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3) 
{
  // three argument version
  class EventMemberImpl3 : public EventImpl {
  public:
    EventMemberImpl3 (OBJ obj, MEM function, T1 a1, T2 a2, T3 a3) 
      : m_obj (obj), 
        m_function (function),
        m_a1 (a1),
        m_a2 (a2),
        m_a3 (a3)
    { }
  protected:
    virtual ~EventMemberImpl3 () {}
  private:
    virtual void Notify (void) { 
      (EventMemberImplObjTraits<OBJ>::GetReference (m_obj).*m_function) (m_a1, m_a2, m_a3);
    }
    OBJ m_obj;
    MEM m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
    typename TypeTraits<T3>::ReferencedType m_a3;
  } *ev = new EventMemberImpl3 (obj, mem_ptr, a1, a2, a3);
  return Ptr<EventImpl> (ev, false);
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4>
Ptr<EventImpl> Simulator::MakeEvent (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4) 
{
  // four argument version
  class EventMemberImpl4 : public EventImpl {
  public:
    EventMemberImpl4 (OBJ obj, MEM function, T1 a1, T2 a2, T3 a3, T4 a4) 
      : m_obj (obj), 
        m_function (function),
        m_a1 (a1),
        m_a2 (a2),
        m_a3 (a3),
        m_a4 (a4)
    { }
  protected:
    virtual ~EventMemberImpl4 () {}
  private:
    virtual void Notify (void) { 
      (EventMemberImplObjTraits<OBJ>::GetReference (m_obj).*m_function) (m_a1, m_a2, m_a3, m_a4);
    }
    OBJ m_obj;
    MEM m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
    typename TypeTraits<T3>::ReferencedType m_a3;
    typename TypeTraits<T4>::ReferencedType m_a4;
  } *ev = new EventMemberImpl4 (obj, mem_ptr, a1, a2, a3, a4);
  return Ptr<EventImpl> (ev, false);
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4, typename T5>
Ptr<EventImpl> Simulator::MakeEvent (MEM mem_ptr, OBJ obj, 
                                     T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
  // five argument version
  class EventMemberImpl5 : public EventImpl {
  public:
    EventMemberImpl5 (OBJ obj, MEM function, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
      : m_obj (obj), 
        m_function (function),
        m_a1 (a1),
        m_a2 (a2),
        m_a3 (a3),
        m_a4 (a4),
        m_a5 (a5)
    { }
  protected:
    virtual ~EventMemberImpl5 () {}
  private:
    virtual void Notify (void) { 
      (EventMemberImplObjTraits<OBJ>::GetReference (m_obj).*m_function) (m_a1, m_a2, m_a3, m_a4, m_a5);
    }
    OBJ m_obj;
    MEM m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
    typename TypeTraits<T3>::ReferencedType m_a3;
    typename TypeTraits<T4>::ReferencedType m_a4;
    typename TypeTraits<T5>::ReferencedType m_a5;
  } *ev = new EventMemberImpl5 (obj, mem_ptr, a1, a2, a3, a4, a5);
  return Ptr<EventImpl> (ev, false);
}

template <typename U1, typename T1>
Ptr<EventImpl> Simulator::MakeEvent (void (*f) (U1), T1 a1) 
{
  // one arg version
  class EventFunctionImpl1 : public EventImpl {
  public:
    typedef void (*F)(U1);
      
    EventFunctionImpl1 (F function, T1 a1) 
      : m_function (function),
        m_a1 (a1)
    { }
  protected:
    virtual ~EventFunctionImpl1 () {}
  private:
    virtual void Notify (void) { 
      (*m_function) (m_a1);
    }
    F m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
  } *ev = new EventFunctionImpl1 (f, a1);
  return Ptr<EventImpl> (ev, false);
}

template <typename U1, typename U2, typename T1, typename T2>
Ptr<EventImpl> Simulator::MakeEvent (void (*f) (U1,U2), T1 a1, T2 a2) 
{
  // two arg version
  class EventFunctionImpl2 : public EventImpl {
  public:
    typedef void (*F)(U1, U2);
      
    EventFunctionImpl2 (F function, T1 a1, T2 a2) 
      : m_function (function),
        m_a1 (a1),
        m_a2 (a2)
    {}
  protected:
    virtual ~EventFunctionImpl2 () {}
  private:
    virtual void Notify (void) { 
      (*m_function) (m_a1, m_a2);
    }
    F m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
  } *ev = new EventFunctionImpl2 (f, a1, a2);
  return Ptr<EventImpl> (ev, false);
}

template <typename U1, typename U2, typename U3,
          typename T1, typename T2, typename T3>
Ptr<EventImpl> Simulator::MakeEvent (void (*f) (U1,U2,U3), T1 a1, T2 a2, T3 a3)
{
  // three arg version
  class EventFunctionImpl3 : public EventImpl {
  public:
    typedef void (*F)(U1, U2, U3);
      
    EventFunctionImpl3 (F function, T1 a1, T2 a2, T3 a3) 
      : m_function (function),
        m_a1 (a1),
        m_a2 (a2),
        m_a3 (a3)
    { }
  protected:
    virtual ~EventFunctionImpl3 () {}
  private:
    virtual void Notify (void) { 
      (*m_function) (m_a1, m_a2, m_a3);
    }
    F m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
    typename TypeTraits<T3>::ReferencedType m_a3;
  } *ev = new EventFunctionImpl3 (f, a1, a2, a3);
  return Ptr<EventImpl> (ev, false);
}

template <typename U1, typename U2, typename U3, typename U4,
          typename T1, typename T2, typename T3, typename T4>
Ptr<EventImpl> Simulator::MakeEvent (void (*f) (U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4) 
{
  // four arg version
  class EventFunctionImpl4 : public EventImpl {
  public:
    typedef void (*F)(U1, U2, U3, U4);
      
    EventFunctionImpl4 (F function, T1 a1, T2 a2, T3 a3, T4 a4) 
      : m_function (function),
        m_a1 (a1),
        m_a2 (a2),
        m_a3 (a3),
        m_a4 (a4)
    { }
  protected:
    virtual ~EventFunctionImpl4 () {}
  private:
    virtual void Notify (void) { 
      (*m_function) (m_a1, m_a2, m_a3, m_a4);
    }
    F m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
    typename TypeTraits<T3>::ReferencedType m_a3;
    typename TypeTraits<T4>::ReferencedType m_a4;
  } *ev = new EventFunctionImpl4 (f, a1, a2, a3, a4);
  return Ptr<EventImpl> (ev, false);
}

template <typename U1, typename U2, typename U3, typename U4, typename U5,
          typename T1, typename T2, typename T3, typename T4, typename T5>
Ptr<EventImpl> Simulator::MakeEvent (void (*f) (U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
  // five arg version
  class EventFunctionImpl5 : public EventImpl {
  public:
    typedef void (*F)(U1,U2,U3,U4,U5);
      
    EventFunctionImpl5 (F function, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
      : m_function (function),
        m_a1 (a1),
        m_a2 (a2),
        m_a3 (a3),
        m_a4 (a4),
        m_a5 (a5)
    {}
  protected:
    virtual ~EventFunctionImpl5 () {}
  private:
    virtual void Notify (void) { 
      (*m_function) (m_a1, m_a2, m_a3, m_a4, m_a5);
    }
    F m_function;
    typename TypeTraits<T1>::ReferencedType m_a1;
    typename TypeTraits<T2>::ReferencedType m_a2;
    typename TypeTraits<T3>::ReferencedType m_a3;
    typename TypeTraits<T4>::ReferencedType m_a4;
    typename TypeTraits<T5>::ReferencedType m_a5;
  } *ev = new EventFunctionImpl5 (f, a1, a2, a3, a4, a5);
  return Ptr<EventImpl> (ev, false);
}

template <typename MEM, typename OBJ>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj) 
{
  return Schedule (time, MakeEvent (mem_ptr, obj));
}


template <typename MEM, typename OBJ,
          typename T1>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj, T1 a1) 
{
  return Schedule (time, MakeEvent (mem_ptr, obj, a1));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2)
{
  return Schedule (time, MakeEvent (mem_ptr, obj, a1, a2));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3) 
{
  return Schedule (time, MakeEvent (mem_ptr, obj, a1, a2, a3));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4) 
{
  return Schedule (time, MakeEvent (mem_ptr, obj, a1, a2, a3, a4));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj, 
                             T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
  return Schedule (time, MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5));
}

template <typename U1, typename T1>
EventId Simulator::Schedule (Time const &time, void (*f) (U1), T1 a1) 
{
  return Schedule (time, MakeEvent (f, a1));
}

template <typename U1, typename U2, 
          typename T1, typename T2>
EventId Simulator::Schedule (Time const &time, void (*f) (U1,U2), T1 a1, T2 a2) 
{
  return Schedule (time, MakeEvent (f, a1, a2));
}

template <typename U1, typename U2, typename U3,
          typename T1, typename T2, typename T3>
EventId Simulator::Schedule (Time const &time, void (*f) (U1,U2,U3), T1 a1, T2 a2, T3 a3)
{
  return Schedule (time, MakeEvent (f, a1, a2, a3));
}

template <typename U1, typename U2, typename U3, typename U4,
          typename T1, typename T2, typename T3, typename T4>
EventId Simulator::Schedule (Time const &time, void (*f) (U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4) 
{
  return Schedule (time, MakeEvent (f, a1, a2, a3, a4));
}

template <typename U1, typename U2, typename U3, typename U4, typename U5,
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId Simulator::Schedule (Time const &time, void (*f) (U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
  return Schedule (time, MakeEvent (f, a1, a2, a3, a4, a5));
}




template <typename MEM, typename OBJ>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj) 
{
  return ScheduleNow (MakeEvent (mem_ptr, obj));
}


template <typename MEM, typename OBJ, 
          typename T1>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1) 
{
  return ScheduleNow (MakeEvent (mem_ptr, obj, a1));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1, T2 a2) 
{
  return ScheduleNow (MakeEvent (mem_ptr, obj, a1, a2));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3) 
{
  return ScheduleNow (MakeEvent (mem_ptr, obj, a1, a2, a3));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4) 
{
  return ScheduleNow (MakeEvent (mem_ptr, obj, a1, a2, a3, a4));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId
Simulator::ScheduleNow (MEM mem_ptr, OBJ obj, 
                        T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
  return ScheduleNow (MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5));
}

template <typename U1,
          typename T1>
EventId
Simulator::ScheduleNow (void (*f) (U1), T1 a1) 
{
  return ScheduleNow (MakeEvent (f, a1));
}

template <typename U1, typename U2,
          typename T1, typename T2>
EventId
Simulator::ScheduleNow (void (*f) (U1,U2), T1 a1, T2 a2) 
{
  return ScheduleNow (MakeEvent (f, a1, a2));
}

template <typename U1, typename U2, typename U3,
          typename T1, typename T2, typename T3>
EventId
Simulator::ScheduleNow (void (*f) (U1,U2,U3), T1 a1, T2 a2, T3 a3)
{
  return ScheduleNow (MakeEvent (f, a1, a2, a3));
}

template <typename U1, typename U2, typename U3, typename U4,
          typename T1, typename T2, typename T3, typename T4>
EventId
Simulator::ScheduleNow (void (*f) (U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4) 
{
  return ScheduleNow (MakeEvent (f, a1, a2, a3, a4));
}

template <typename U1, typename U2, typename U3, typename U4, typename U5,
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId
Simulator::ScheduleNow (void (*f) (U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
  return ScheduleNow (MakeEvent (f, a1, a2, a3, a4, a5));
}



template <typename MEM, typename OBJ>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj) 
{
  return ScheduleDestroy (MakeEvent (mem_ptr, obj));
}


template <typename MEM, typename OBJ, 
          typename T1>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1) 
{
  return ScheduleDestroy (MakeEvent (mem_ptr, obj, a1));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1, T2 a2) 
{
  return ScheduleDestroy (MakeEvent (mem_ptr, obj, a1, a2));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3) 
{
  return ScheduleDestroy (MakeEvent (mem_ptr, obj, a1, a2, a3));
}

template <typename MEM, typename OBJ,
          typename T1, typename T2, typename T3, typename T4>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj, T1 a1, T2 a2, T3 a3, T4 a4) 
{
  return ScheduleDestroy (MakeEvent (mem_ptr, obj, a1, a2, a3, a4));
}

template <typename MEM, typename OBJ, 
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId
Simulator::ScheduleDestroy (MEM mem_ptr, OBJ obj, 
                            T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
  return ScheduleDestroy (MakeEvent (mem_ptr, obj, a1, a2, a3, a4, a5));
}

template <typename U1,
          typename T1>
EventId
Simulator::ScheduleDestroy (void (*f) (U1), T1 a1) 
{
  return ScheduleDestroy (MakeEvent (f, a1));
}

template <typename U1, typename U2,
          typename T1, typename T2>
EventId
Simulator::ScheduleDestroy (void (*f) (U1,U2), T1 a1, T2 a2) 
{
  return ScheduleDestroy (MakeEvent (f, a1, a2));
}

template <typename U1, typename U2, typename U3,
          typename T1, typename T2, typename T3>
EventId
Simulator::ScheduleDestroy (void (*f) (U1,U2,U3), T1 a1, T2 a2, T3 a3)
{
  return ScheduleDestroy (MakeEvent (f, a1, a2, a3));
}

template <typename U1, typename U2, typename U3, typename U4,
          typename T1, typename T2, typename T3, typename T4>
EventId
Simulator::ScheduleDestroy (void (*f) (U1,U2,U3,U4), T1 a1, T2 a2, T3 a3, T4 a4) 
{
  return ScheduleDestroy (MakeEvent (f, a1, a2, a3, a4));
}

template <typename U1, typename U2, typename U3, typename U4, typename U5,
          typename T1, typename T2, typename T3, typename T4, typename T5>
EventId
Simulator::ScheduleDestroy (void (*f) (U1,U2,U3,U4,U5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
{
  return ScheduleDestroy (MakeEvent (f, a1, a2, a3, a4, a5));
}

}; // namespace ns3

#endif /* SIMULATOR_H */
