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
#include "object.h"
#include "object-factory.h"
#include "ptr.h"

namespace ns3 {

class Scheduler;

/**
 * \ingroup simulator
 *
 * The SimulatorImpl base class.
 *
 * \todo Define what the simulation or event context means.
 */
class SimulatorImpl : public Object
{
public:
  
  /**
   *  Register this type.
   *  \return The object TypeId.
   */
  static TypeId GetTypeId (void);

  /**
   * Execute the events scheduled with ScheduleDestroy().
   *
   * This method is typically invoked at the end of a simulation
   * to avoid false-positive reports by a leak checker.
   * After this method has been invoked, it is actually possible
   * to restart a new simulation with a set of calls to Simulator::Run,
   * Simulator::Schedule and Simulator::ScheduleWithContext.
   */
  virtual void Destroy () = 0;
  /**
   * Check if the simulation should finish.
   *
   * Reasons to finish are because there are
   * no more events lefts to be scheduled, or if simulation
   * time has already reached the "stop time" (see Simulator::Stop()).
   *
   * \return \c true if no more events or stop time reached.
   */
  virtual bool IsFinished (void) const = 0;
  /**
   * Tell the Simulator the calling event should be the last one
   * executed.
   *
   * If a running event invokes this method, it will be the last
   * event executed by the Simulator::Run method before
   * returning to the caller.
   */
  virtual void Stop (void) = 0;
  /**
   * Schedule the time delay until the Simulator should stop.
   *
   * Force the Simulator::Run method to return to the caller when the
   * expiration time of the next event to be processed is greater than
   * or equal to the stop time.  The stop time is relative to the
   * current simulation time.
   * \param time The stop time, relative to the current time.
   */
  virtual void Stop (Time const &time) = 0;
  /**
   * Schedule a future event execution (in the same context).
   *
   * \param time Delay until the event expires.
   * \param event The event to schedule.
   * \returns A unique identifier for the newly-scheduled event.
   */
  virtual EventId Schedule (Time const &time, EventImpl *event) = 0;
  /**
   * Schedule a future event execution (in a different context).
   *
   * \param time Delay until the event expires.
   * \param context Event context.
   * \param event The event to schedule.
   * \returns A unique identifier for the newly-scheduled event.
   */
  virtual void ScheduleWithContext (uint32_t context, Time const &time, EventImpl *event) = 0;
  /**
   * Schedule an event to run at the current virtual time.
   *
   * \param event The event to schedule.
   * \returns A unique identifier for the newly-scheduled event.
   */
  virtual EventId ScheduleNow (EventImpl *event) = 0;
  /**
   * Schedule an event to run at the end of the simulation, after
   * the Stop() time or condition has been reached.
   *
   * \param event The event to schedule.
   * \returns A unique identifier for the newly-scheduled event.
   */
  virtual EventId ScheduleDestroy (EventImpl *event) = 0;
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
   * \param id The event to remove from the list of scheduled events.
   */
  virtual void Remove (const EventId &id) = 0;
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
   * \param id the event to cancel
   */
  virtual void Cancel (const EventId &id) = 0;
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
   * \param id The event to test for expiration.
   * \returns \c true if the event has expired, false otherwise.
   */
  virtual bool IsExpired (const EventId &id) const = 0;
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
  virtual void Run (void) = 0;
  /**
   * Return the current simulation virtual time.
   *
   * \returns The current virtual time.
   */
  virtual Time Now (void) const = 0;
  /**
   * Get the remaining time until this event will execute.
   *
   * \param id The event id to analyse.
   * \return The delay left until the input event id expires.
   *          if the event is not running, this method returns
   *          zero.
   */
  virtual Time GetDelayLeft (const EventId &id) const = 0;
  /**
   * Get the maximum representable simulation time.
   *
   * \return The maximum simulation time at which an event 
   *          can be scheduled.
   *
   * The returned value will always be bigger than or equal to Simulator::Now.
   */
  virtual Time GetMaximumSimulationTime (void) const = 0;
  /**
   * Set the Scheduler to be used to manage the event list.
   *
   * \param schedulerFactory A new event scheduler factory.
   *
   * The event scheduler can be set at any time: the events scheduled
   * in the previous scheduler will be transfered to the new scheduler
   * before we start to use it.
   */
  virtual void SetScheduler (ObjectFactory schedulerFactory) = 0;
  /**
   * Get the system id of this simulator.
   *
   * The system id is the identifier for this simulator instance
   * in a distributed simulation.  For MPI this is the MPI rank.
   * \return The system id for this simulator.
   */
  virtual uint32_t GetSystemId () const = 0; 
  /**
   * Get the current simulation context.
   *
   * \return The current simulation context
   */
  virtual uint32_t GetContext (void) const = 0;
};

} // namespace ns3

#endif /* SIMULATOR_IMPL_H */
