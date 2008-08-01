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

#ifndef DEFAULT_SIMULATOR_IMPL_H
#define DEFAULT_SIMULATOR_IMPL_H

#include "simulator-impl.h"

#include "scheduler.h"
#include "event-impl.h"

#include "ns3/ptr.h"
#include "ns3/assert.h"
#include "ns3/log.h"

#include <list>
#include <fstream>

namespace ns3 {

  class DefaultSimulatorImpl : public SimulatorImpl
{
public:
  static TypeId GetTypeId (void);

  DefaultSimulatorImpl ();
  ~DefaultSimulatorImpl ();

  void Destroy ();

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
  void Run (void);
  Time Now (void) const;
  Time GetDelayLeft (const EventId &id) const;
  Time GetMaximumSimulationTime (void) const;

  void SetScheduler (Ptr<Scheduler> scheduler);
  Ptr<Scheduler> GetScheduler (void) const;

private:
  void ProcessOneEvent (void);
  uint64_t NextTs (void) const;

  typedef std::list<EventId> DestroyEvents;
  DestroyEvents m_destroyEvents;
  uint64_t m_stopAt;
  bool m_stop;
  Ptr<Scheduler> m_events;
  uint32_t m_uid;
  uint32_t m_currentUid;
  uint64_t m_currentTs;
  // number of events that have been inserted but not yet scheduled,
  // not counting the "destroy" events; this is used for validation
  int m_unscheduledEvents;
};

} // namespace ns3

#endif /* DEFAULT_SIMULATOR_IMPL_H */
