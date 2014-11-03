/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
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
 */

#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include <stdint.h>
#include "nstime.h"
#include "object.h"

namespace ns3 {

/**
 * @brief Base class used for synchronizing the simulation events to some
 * real time "wall clock."
 *
 * The simulation clock is maintained as a 64-bit integer in a unit specified
 * by the user through the TimeStepPrecision::Set function. This means that
 * it is not possible to specify event expiration times with anything better
 * than this user-specified accuracy.  We use this clock for the simulation
 * time.
 *
 * The real-time clock is maintained as a 64-bit integer count of nanoseconds.
 *
 * The synchronization between the simulation clock and the real-time clock
 * is maintained using a combination of sleep-waiting, busy-waiting and a
 * feedback loop.
 */
class Synchronizer : public Object 
{
public:
  static TypeId GetTypeId (void);

  Synchronizer ();
  virtual ~Synchronizer ();

/**
 * @brief Return true if this synchronizer is actually synchronizing to a
 * realtime clock.  The simulator sometimes needs to know this.
 * @returns True if locked with realtime, false if not.
 */
  bool Realtime (void);

/**
 * @brief Retrieve the value of the origin of the underlying normalized wall
 * clock time in simulator timestep units.
 *
 * @returns The normalized wall clock time (in simulator timestep units).
 * @see TimeStepPrecision::Get
 * @see Synchronizer::SetOrigin
 */
  uint64_t GetCurrentRealtime (void);

/**
 * @brief Establish a correspondence between a simulation time and the
 * synchronizer real time.
 *
 * This method is expected to be called at the "instant" before simulation
 * begins.  At this point, simulation time = 0, and a 
 * set = 0 in this method.  We then associate this time with the current
 * value of the real time clock that will be used to actually perform the
 * synchronization.
 *
 * Subclasses are expected to implement the corresponding DoSetOrigin pure
 * virtual method to do the actual real-time-clock-specific work of making the 
 * correspondence mentioned above.
 *
 * @param ts The simulation time we should use as the origin (in simulator
 * timestep units).
 * @see TimeStepPrecision::Get
 * @see TimeStepPrecision::DoSetOrigin
 */
  void SetOrigin (uint64_t ts);

/**
 * @brief Retrieve the value of the origin of the simulation time in 
 * simulator timestep units.
 *
 * @returns The simulation time used as the origin (in simulator timestep
 * units).
 * @see TimeStepPrecision::Get
 * @see Synchronizer::SetOrigin
 */
  uint64_t GetOrigin (void);

/**
 * @brief Retrieve the difference between the real time clock used to 
 * synchronize the simulation and the simulation time (in simulator timestep
 * units).
 *
 * @param ts Simulation timestep from the simulator interpreted as current time
 * in the simulator.
 * @returns Simulation timestep (in simulator timestep units) minus origin 
 * time (stored internally in nanosecond units).
 * @see TimeStepPrecision::Get
 * @see Synchronizer::SetOrigin
 * @see Synchronizer::DoGetDrift
 */
  int64_t GetDrift (uint64_t ts);

/**
 * @brief Wait until the real time is in sync with the specified simulation
 * time or until the synchronizer is Sigalled.
 *
 * This is where the real work of synchronization is done.  The Time passed
 * in as a parameter is the simulation time.  The job of Synchronize is to
 * translate from simulation time to synchronizer time (in a perfect world
 * this is the same time) and then figure out how long in real-time it needs
 * to wait until that synchronizer / simulation time comes around.
 *
 * Subclasses are expected to implement the corresponding DoSynchronize pure
 * virtual method to do the actual real-time-clock-specific work of waiting 
 * (either busy-waiting or sleeping, or some combination thereof) until the
 * requested simulation time.
 *
 * @param tsCurrent The current simulation time (in simulator timestep units).
 * @param tsDelay The simulation time we need to wait for (in simulator
 * timestep units).
 * @returns True if the function ran to completion, false if it was interrupted
 * by a Signal.
 * @see TimeStepPrecision::Get
 * @see Synchronizer::DoSynchronize
 * @see Synchronizer::Signal
 */
  bool Synchronize (uint64_t tsCurrent, uint64_t tsDelay);

/**
 * @brief Tell a possible simulator thread waiting in the Synchronize method
 * that an event has happened which demands a reevaluation of the wait time.
 * This will cause the thread to wake and return to the simulator proper
 * where it can get its bearings.
 *
 * @see Synchronizer::Synchronize
 * @see Synchronizer::DoSignal
 */
  void Signal (void);

/**
 * @brief Set the condition variable that tells a possible simulator thread 
 * waiting in the Synchronize method that an event has happened which demands
 * a reevaluation of the wait time.
 *
 * @see Synchronizer::Signal
 */
  void SetCondition (bool);

/**
 * @brief Ask the synchronizer to remember what time it is.  Typically used
 * with EventEnd to determine the real execution time of a simulation event.
 *
 * @see Synchronizer::EventEnd
 * @see TimeStepPrecision::Get
 */
  void EventStart (void);

/**
 * @brief Ask the synchronizer to return the time step between the instant
 * remembered during EventStart and now.  Used in conjunction with EventStart
 * to determine the real execution time of a simulation event.
 *
 * @see Synchronizer::EventStart
 * @see TimeStepPrecision::Get
 */
  uint64_t EventEnd (void);

protected:
/**
 * @brief Establish a correspondence between a simulation time and a 
 * wall-clock (real) time.
 *
 * There are three timelines involved here:  the simulation time, the 
 * (absolute) wall-clock time and the (relative) synchronizer real time.
 * Calling this method makes a correspondence between the origin of the
 * synchronizer time and the current wall-clock time.
 *
 * This method is expected to be called at the "instant" before simulation
 * begins.  At this point, simulation time = 0, and synchronizer time is
 * set = 0 in this method.  We then associate this time with the current
 * value of the real time clock that will be used to actually perform the
 * synchronization.
 *
 * Subclasses are expected to implement this method to do the actual 
 * real-time-clock-specific work of making the correspondence mentioned above.
 * for example, this is where the differences between Time parameters and
 * parameters to clock_nanosleep would be dealt with. 
 *
 * @param ns The simulation time we need to use as the origin (normalized to
 * nanosecond units).
 * @see Synchronizer::SetOrigin
 * @see TimeStepPrecision::Get
 */
  virtual void DoSetOrigin (uint64_t ns) = 0;

/**
 * @brief Return true if this synchronizer is actually synchronizing to a
 * realtime clock.  The simulator sometimes needs to know this.
 *
 * Subclasses are expected to implement this method to tell the outside world
 * whether or not they are synchronizing to a realtime clock.
 *
 * @returns True if locked with realtime, false if not.
 */
  virtual bool DoRealtime (void) = 0;

/**
 * @brief Retrieve the value of the origin of the underlying normalized wall
 * clock time in simulator timestep units.
 *
 * Subclasses are expected to implement this method to do the actual
 * real-time-clock-specific work of getting the current time.
 *
 * @returns The normalized wall clock time (in nanosecond units).
 * @see TimeStepPrecision::Get
 * @see Synchronizer::SetOrigin
 */
  virtual uint64_t DoGetCurrentRealtime (void) = 0;

/**
 * @brief Wait until the real time is in sync with the specified simulation
 * time.
 *
 * This is where the real work of synchronization is done.  The Time passed
 * in as a parameter is the simulation time.  The job of Synchronize is to
 * translate from simulation time to synchronizer time (in a perfect world
 * this is the same time) and then figure out how long in real-time it needs
 * to wait until that synchronizer / simulation time comes around.
 *
 * Subclasses are expected to implement this method to do the actual
 * real-time-clock-specific work of waiting (either busy-waiting or sleeping,
 * or some combination) until the requested simulation time.
 *
 * @param nsCurrent The current simulation time (normalized to nanosecond
 * units).
 * @param nsDelay The simulation time we need to wait for (normalized to 
 * nanosecond units).
 * @returns True if the function ran to completion, false if it was interrupted
 * by a Signal.
 * @see Synchronizer::Synchronize
 * @see TimeStepPrecision::Get
 * @see Synchronizer::Signal
 */
  virtual bool DoSynchronize (uint64_t nsCurrent, uint64_t nsDelay) = 0;

/**
 * @brief Declaration of the method used to tell a possible simulator thread 
 * waiting in the DoSynchronize method that an event has happened which
 * demands a reevaluation of the wait time.
 *
 * @see Synchronizer::Signal
 */
  virtual void DoSignal (void) = 0;

/**
 * @brief Declaration of the method used to set the condition variable that 
 * tells a possible simulator thread waiting in the Synchronize method that an
 * event has happened which demands a reevaluation of the wait time.
 *
 * @see Synchronizer::SetCondition
 */
  virtual void DoSetCondition (bool) = 0;

/**
 * @brief Declaration of method used to retrieve drift between the real time
 * clock used to synchronize the simulation and the current simulation time.
 *
 * @param ns Simulation timestep from the simulator normalized to nanosecond 
 * steps.
 * @returns Drift in nanosecond units.
 * @see TimeStepPrecision::Get
 * @see Synchronizer::SetOrigin
 * @see Synchronizer::GetDrift
 */
  virtual int64_t DoGetDrift (uint64_t ns) = 0;

  virtual void DoEventStart (void) = 0;
  virtual uint64_t DoEventEnd (void) = 0;

  uint64_t m_realtimeOriginNano;
  uint64_t m_simOriginNano;

private:
/**
 * @brief Convert a simulator time step (which can be steps of time in a 
 * user-specified unit) to a normalized time step in nanosecond units.
 *
 * @param ts The simulation time step to be normalized.
 * @returns The simulation time step normalized to nanosecond units.
 * @see TimeStepPrecision::Get
 */
  uint64_t TimeStepToNanosecond (uint64_t ts);

/**
 * @brief Convert a normalized nanosecond count into a simulator time step
 * (which can be steps of time in a user-specified unit).
 *
 * @param ns The nanosecond count step to be converted
 * @returns The simulation time step to be interpreted in appropriate units.
 * @see TimeStepPrecision::Get
 */
  uint64_t NanosecondToTimeStep (uint64_t ns);
};

} // namespace ns3

#endif /* SYNCHRONIZER_H */
