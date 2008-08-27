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

#ifndef REALTIME_SIMULATOR_IMPL_H
#define REALTIME_SIMULATOR_IMPL_H

#include "simulator-impl.h"

#include "scheduler.h"
#include "synchronizer.h"
#include "event-impl.h"

#include "ns3/ptr.h"
#include "ns3/assert.h"
#include "ns3/log.h"

#include <list>
#include <fstream>

namespace ns3 {

class RealtimeEventLock : public EventLock
{
public:
  void Lock (void);
  void Unlock (void);
private:
  SystemMutex m_eventMutex;
};

class RealtimeSimulatorImpl : public SimulatorImpl
{
public:
  static TypeId GetTypeId (void);

  /**
   * Enumeration of the types of packets supported in the class.
   *
   */
  enum SynchronizationMode {
    SYNC_BEST_EFFORT, /** Make a best effort to keep synced to real-time */
    SYNC_HARD_LIMIT, /** Keep to real-time within a tolerance or die trying */
  };

  RealtimeSimulatorImpl ();
  ~RealtimeSimulatorImpl ();

  void Destroy ();

  void EnableLogTo (char const *filename);

  bool IsFinished (void) const;
  Time Next (void) const;
  void Stop (void);
  void Stop (Time const &time);
  EventId Schedule (Time const &time, const Ptr<EventImpl> &event);
  EventId ScheduleNow (const Ptr<EventImpl> &event);
  EventId ScheduleDestroy (const Ptr<EventImpl> &event);
  void Remove (const EventId &ev);
  void Cancel (const EventId &ev);
  bool IsExpired (const EventId &ev) const;
  virtual void RunOneEvent (void);
  void Run (void);
  Time Now (void) const;
  Time GetDelayLeft (const EventId &id) const;
  Time GetMaximumSimulationTime (void) const;

  void SetScheduler (Ptr<Scheduler> scheduler);
  Ptr<Scheduler> GetScheduler (void) const;

  void SetSynchronizationMode (RealtimeSimulatorImpl::SynchronizationMode mode);
  RealtimeSimulatorImpl::SynchronizationMode GetSynchronizationMode (void) const;

  void SetHardLimit (Time limit);
  Time GetHardLimit (void) const;

private:
  bool Running (void) const;
  bool Realtime (void) const;

  void ProcessOneEvent (void);
  uint64_t NextTs (void) const;

  typedef std::list<EventId> DestroyEvents;
  DestroyEvents m_destroyEvents;
  uint64_t m_stopAt;
  bool m_stop;
  bool m_running;

  // The following variables are protected using the m_mutex
  Ptr<Scheduler> m_events;
  int m_unscheduledEvents;
  uint32_t m_uid;
  uint32_t m_currentUid;
  uint64_t m_currentTs;

  mutable SystemMutex m_mutex;
  RealtimeEventLock m_eventLock;

  Ptr<Synchronizer> m_synchronizer;

#ifdef PERMIT_WALLCLOCK_SIMULATION_TIME
  /*
   * In calls to Simulator::Now we have a basic choice to make.  We can either
   * report back the time the simulator thinks it should be, or we can report 
   * the time it actually is.
   *
   * The synchronizer will make an attempt to cause these two numbers to be as
   * close as possible to each other, but they will never be exactly the same.
   * We give the client a choice in this respect.  
   *
   * If the client sets m_reportSimulatedTime to true, the behavior will be that 
   * the simulator runs as close as possible to real time, but reports back to the
   * client that it is running at exactly real time, and consuming no real time 
   * as each event executes.  This allows for deterministic execution times and
   * repeatable trace files.
   *
   * If the client sets m_reportSimulatedTime to false, the behavior will be that 
   * the simulator runs as close as possible to real time, and reports back to the
   * client the real wall-clock time whenever it asks.  Real time will be consumed
   * as each event executes.  This allows for non-deterministic execution times and
   * real variations in event executions.  Simulation time will be influenced by
   * variations in host process scheduling, for example.
   */
  bool m_reportSimulatedTime;
#endif

  /**
   * The policy to use if the simulation cannot keep synchronized to real-time.
   */
  SynchronizationMode m_synchronizationMode;

  /**
   * The maximum allowable drift from real-time in SYNC_HARD_LIMIT mode.
   */
  Time m_hardLimit;
};

} // namespace ns3

#endif /* REALTIME_SIMULATOR_IMPL_H */
