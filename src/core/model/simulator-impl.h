/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

#ifndef SIMULATOR_IMPL_H
#define SIMULATOR_IMPL_H

#include "event-impl.h"
#include "event-id.h"
#include "nstime.h"
#include "ns3/object.h"
#include "ns3/object-factory.h"
#include "ns3/ptr.h"

namespace ns3 {

class Scheduler;

class SimulatorImpl : public Object
{
public:
  static TypeId GetTypeId (void);

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
  virtual void Destroy () = 0;
  /**
   * If there are no more events lefts to be scheduled, or if simulation
   * time has already reached the "stop time" (see Simulator::Stop()),
   * return true. Return false otherwise.
   */
  virtual bool IsFinished (void) const = 0;
  /**
   * If Simulator::IsFinished returns true, the behavior of this
   * method is undefined. Otherwise, it returns the microsecond-based
   * time of the next event expected to be scheduled.
   */
  virtual Time Next (void) const = 0;
  /**
   * If an event invokes this method, it will be the last
   * event scheduled by the Simulator::run method before
   * returning to the caller.
   */
  virtual void Stop (void) = 0;
  /**
   * Force the Simulator::run method to return to the caller when the
   * expiration time of the next event to be processed is greater than
   * or equal to the stop time.  The stop time is relative to the
   * current simulation time.
   * @param time the stop time, relative to the current time.
   */
  virtual void Stop (Time const &time) = 0;
  /**
   * \param time delay until the event expires
   * \param event the event to schedule
   * \returns a unique identifier for the newly-scheduled event.
   *
   * This method will be typically used by language bindings
   * to delegate events to their own subclass of the EventImpl base class.
   */
  virtual EventId Schedule (Time const &time, EventImpl *event) = 0;
  /**
   * \param time delay until the event expires
   * \param context event context
   * \param event the event to schedule
   * \returns a unique identifier for the newly-scheduled event.
   *
   * This method will be typically used by language bindings
   * to delegate events to their own subclass of the EventImpl base class.
   */
  virtual void ScheduleWithContext (uint32_t context, Time const &time, EventImpl *event) = 0;
  /**
   * \param event the event to schedule
   * \returns a unique identifier for the newly-scheduled event.
   *
   * This method will be typically used by language bindings
   * to delegate events to their own subclass of the EventImpl base class.
   */
  virtual EventId ScheduleNow (EventImpl *event) = 0;
  /**
   * \param event the event to schedule
   * \returns a unique identifier for the newly-scheduled event.
   *
   * This method will be typically used by language bindings
   * to delegate events to their own subclass of the EventImpl base class.
   */
  virtual EventId ScheduleDestroy (EventImpl *event) = 0;
  /**
   * Remove an event from the event list. 
   * This method has the same visible effect as the 
   * ns3::EventId::Cancel method
   * but its algorithmic complexity is much higher: it has often 
   * O(log(n)) complexity, sometimes O(n), sometimes worse.
   * Note that it is not possible to remove events which were scheduled
   * for the "destroy" time. Doing so will result in a program error (crash).
   *
   * @param ev the event to remove from the list of scheduled events.
   */
  virtual void Remove (const EventId &ev) = 0;
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
   * @param ev the event to cancel
   */
  virtual void Cancel (const EventId &ev) = 0;
  /**
   * This method has O(1) complexity.
   * Note that it is not possible to test for the expiration of
   * events which were scheduled for the "destroy" time. Doing so
   * will result in a program error (crash).
   * An event is said to "expire" when it starts being scheduled
   * which means that if the code executed by the event calls
   * this function, it will get true.
   *
   * @param ev the event to test for expiration
   * @returns true if the event has expired, false otherwise.
   */
  virtual bool IsExpired (const EventId &ev) const = 0;
  /**
   * Run the simulation until one of:
   *   - no events are present anymore
   *   - the user called Simulator::stop
   *   - the user called Simulator::stopAtUs and the
   *     expiration time of the next event to be processed
   *     is greater than or equal to the stop time.
   */
  virtual void Run (void) = 0;
  /**
   * Process only the next simulation event, then return immediately.
   */
  virtual void RunOneEvent (void) = 0;
  /**
   * Return the "current simulation time".
   */
  virtual Time Now (void) const = 0;
  /**
   * \param id the event id to analyse
   * \return the delay left until the input event id expires.
   *          if the event is not running, this method returns
   *          zero.
   */
  virtual Time GetDelayLeft (const EventId &id) const = 0;
  /**
   * \return the maximum simulation time at which an event 
   *          can be scheduled.
   *
   * The returned value will always be bigger than or equal to Simulator::Now.
   */
  virtual Time GetMaximumSimulationTime (void) const = 0;
  /**
   * \param schedulerFactory a new event scheduler factory
   *
   * The event scheduler can be set at any time: the events scheduled
   * in the previous scheduler will be transfered to the new scheduler
   * before we start to use it.
   */
  virtual void SetScheduler (ObjectFactory schedulerFactory) = 0;
  /**
   * \return the system id for this simulator; used for 
   *          MPI or other distributed simulations
   */
  virtual uint32_t GetSystemId () const = 0; 
  /**
   * \return the current simulation context
   */
  virtual uint32_t GetContext (void) const = 0;
};

} // namespace ns3

#endif /* SIMULATOR_IMPL_H */
