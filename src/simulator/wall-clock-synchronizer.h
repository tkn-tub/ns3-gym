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

#ifndef WALL_CLOCK_CLOCK_SYNCHRONIZER_H
#define WALL_CLOCK_CLOCK_SYNCHRONIZER_H

#include "ns3/system-condition.h"
#include "synchronizer.h"

namespace ns3 {

/**
 * @brief Class used for synchronizing the simulation events to a real-time
 * "wall clock" using Posix Clock functions.
 *
 * Enable this synchronizer using:
 *
 *   DefaultValue::Bind ("Synchronizer", "WallClockSynchronizer");
 *
 * before calling any simulator functions.
 *
 * The simulation clock is maintained as a 64-bit integer in a unit specified
 * by the user through the TimeStepPrecision::Set function. This means that
 * it is not possible to specify event expiration times with anything better
 * than this user-specified accuracy.  
 *
 * There are a couple of more issues at this level.  Posix clocks provide
 * access to several clocks we could use as a wall clock.  We don't care about
 * time in the sense of 0430 CEST, we care about some piece of hardware that
 * ticks at some regular period.  The most accurate posix clock in this
 * respect is the CLOCK_PROCESS_CPUTIME_ID clock.  This is a high-resolution
 * register in the CPU.  For example, on Intel machines this corresponds to
 * the timestamp counter (TSC) register.  The resolution of this counter will
 * be on the order of nanoseconds.
 *
 * Now, just because we can measure time in nanoseconds doesn't mean we can
 * put our process to sleep to nanosecond resolution.  We are eventualy going
 * to use the function clock_nanosleep () to sleep until a simulation Time
 * specified by the caller. 
 *
 * MORE ON JIFFIES, SLEEP, PROCESSES, etc., as required
 *
 * Nanosleep takes a struct timespec as an input so we have to deal with
 * conversion between Time and struct timespec here.  They are both 
 * interpreted as elapsed times.
 */
class WallClockSynchronizer : public Synchronizer
{
public:
  WallClockSynchronizer ();
  virtual ~WallClockSynchronizer ();

  static const uint64_t US_PER_NS = (uint64_t)1000;
  static const uint64_t US_PER_SEC = (uint64_t)1000000;
  static const uint64_t NS_PER_SEC = (uint64_t)1000000000;

protected:
/**
 * @brief Return true if this synchronizer is actually synchronizing to a
 * realtime clock.  The simulator sometimes needs to know this.
 *
 * @internal
 *
 * Subclasses are expected to implement this method to tell the outside world
 * whether or not they are synchronizing to a realtime clock.
 *
 * @returns True if locked with realtime, false if not.
 */
  virtual bool DoRealtime (void);

/**
 * @brief Retrieve the value of the origin of the underlying normalized wall
 * clock time in nanosecond units.
 *
 * @internal
 *
 * Subclasses are expected to implement this method to do the actual
 * real-time-clock-specific work of getting the current time.
 *
 * @returns The normalized wall clock time (in nanosecond units).
 * @see TimeStepPrecision::Get
 * @see Synchronizer::SetOrigin
 */
  virtual uint64_t DoGetCurrentRealtime (void);

/**
 * @brief Establish a correspondence between a simulation time and a 
 * wall-clock (real) time.
 *
 * @internal
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
 */
  virtual void DoSetOrigin (uint64_t ns);

/**
 * @brief Declaration of method used to retrieve drift between the real time
 * clock used to synchronize the simulation and the current simulation time.
 *
 * @internal
 *
 * @param ns Simulation timestep from the simulator normalized to nanosecond 
 * steps.
 * @returns Drift in nanosecond units.
 * @see TimeStepPrecision::Get
 * @see Synchronizer::SetOrigin
 * @see Synchronizer::GetDrift
 */
  virtual int64_t DoGetDrift (uint64_t ns);

/**
 * @brief Wait until the real time is in sync with the specified simulation
 * time.
 *
 * @internal
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
 * @param ns The simulation time we need to wait for (normalized to nanosecond
 * units).
 * @see TimeStepPrecision::Get
 */
  virtual bool DoSynchronize (uint64_t nsCurrent, uint64_t nsDelay);
  virtual void DoSignal (void);
  virtual void DoSetCondition (bool cond);

  virtual void DoEventStart (void);
  virtual uint64_t DoEventEnd (void);

  bool SpinWait (uint64_t);
  bool SleepWait (uint64_t);

  uint64_t DriftCorrect (uint64_t nsNow, uint64_t nsDelay);

  uint64_t GetRealtime (void);
  uint64_t GetNormalizedRealtime (void);

  void NsToTimespec (int64_t ns, struct timespec *ts);
  void NsToTimeval (int64_t ns, struct timeval *tv);

  uint64_t TimespecToNs (struct timespec *ts);
  uint64_t TimevalToNs (struct timeval *tv);

  void TimespecAdd(
    struct timespec *ts1, 
    struct timespec *ts2, 
    struct timespec *result);

  void TimevalAdd (
    struct timeval *tv1, 
    struct timeval *tv2,
    struct timeval *result);

  uint64_t m_cpuTick;
  uint64_t m_realtimeTick;
  uint64_t m_jiffy;
  uint64_t m_nsEventStart;

  SystemCondition m_condition;
};

}; // namespace ns3

#endif /* WALL_CLOCK_SYNCHRONIZER_H */
