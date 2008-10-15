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
#include "ns3/system-mutex.h"

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

  virtual void Destroy ();
  virtual bool IsFinished (void) const;
  virtual Time Next (void) const;
  virtual void Stop (void);
  virtual void Stop (Time const &time);
  virtual EventId Schedule (Time const &time, EventImpl *event);
  virtual EventId ScheduleNow (EventImpl *event);
  virtual EventId ScheduleDestroy (EventImpl *event);
  virtual void Remove (const EventId &ev);
  virtual void Cancel (const EventId &ev);
  virtual bool IsExpired (const EventId &ev) const;
  virtual void Run (void);
  virtual void RunOneEvent (void);
  virtual Time Now (void) const;
  virtual Time GetDelayLeft (const EventId &id) const;
  virtual Time GetMaximumSimulationTime (void) const;
  virtual void SetScheduler (Ptr<Scheduler> scheduler);
  virtual Ptr<Scheduler> GetScheduler (void) const;
  EventId ScheduleRealtime (Time const &time, const Ptr<EventImpl> &event);
  EventId ScheduleRealtimeNow (const Ptr<EventImpl> &event);
  Time RealtimeNow (void) const;

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
