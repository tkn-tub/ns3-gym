/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 * All rights reserved.
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

#ifndef SCHEDULER_MAP_H
#define SCHEDULER_MAP_H

#include "scheduler.h"
#include <stdint.h>
#include <map>
#include <utility>

namespace ns3 {

class EventImpl;

class SchedulerMap : public Scheduler {
public:
  SchedulerMap ();
  virtual ~SchedulerMap ();

private:
  virtual EventId RealInsert (EventImpl *event, Scheduler::EventKey key);
  virtual bool RealIsEmpty (void) const;
  virtual EventImpl *RealPeekNext (void) const;
  virtual Scheduler::EventKey RealPeekNextKey (void) const;
  virtual void RealRemoveNext (void);
  virtual EventImpl *RealRemove (EventId ev, Scheduler::EventKey *key);
  virtual bool RealIsValid (EventId id);

  typedef std::map<Scheduler::EventKey, EventImpl*, Scheduler::EventKeyCompare> EventMap;
  typedef std::map<Scheduler::EventKey, EventImpl*, Scheduler::EventKeyCompare>::iterator EventMapI;
  typedef std::map<Scheduler::EventKey, EventImpl*, Scheduler::EventKeyCompare>::const_iterator EventMapCI;

  void StoreInEvent (EventImpl *ev, EventMapI i) const;
  SchedulerMap::EventMapI GetFromEvent (EventImpl *ev) const;

  EventMap m_list;
  uint32_t m_uid;
};

}; // namespace ns3


#endif /* SCHEDULER_MAP_H */
