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
#include "ns3/ptr.h"

namespace ns3 {

class Scheduler;

class SimulatorImpl : public Object
{
public:
  virtual void Destroy () = 0;
  virtual bool IsFinished (void) const = 0;
  virtual Time Next (void) const = 0;
  virtual void Stop (void) = 0;
  virtual EventId Schedule (Time const &time, EventImpl *event) = 0;
  virtual EventId ScheduleNow (EventImpl *event) = 0;
  virtual EventId ScheduleDestroy (EventImpl *event) = 0;
  virtual void Remove (const EventId &ev) = 0;
  virtual void Cancel (const EventId &ev) = 0;
  virtual bool IsExpired (const EventId &ev) const = 0;
  virtual void Run (void) = 0;
  virtual void RunOneEvent (void) = 0;
  virtual Time Now (void) const = 0;
  virtual Time GetDelayLeft (const EventId &id) const = 0;
  virtual Time GetMaximumSimulationTime (void) const = 0;
  virtual void SetScheduler (Ptr<Scheduler> scheduler) = 0;
  virtual Ptr<Scheduler> GetScheduler (void) const = 0;
};

} // namespace ns3

#endif /* SIMULATOR_IMPL_H */
