/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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

/**
 * \ingroup scheduler
 */
class MapScheduler : public Scheduler {
public:
  MapScheduler ();
  virtual ~MapScheduler ();

  virtual void Insert (const EventId &id);
  virtual bool IsEmpty (void) const;
  virtual EventId PeekNext (void) const;
  virtual EventId RemoveNext (void);
  virtual bool Remove (const EventId &ev);
private:

  class EventKeyCompare {
  public:
    bool operator () (struct EventKey const&a, struct EventKey const&b);
  };

  typedef std::map<Scheduler::EventKey, EventImpl*, MapScheduler::EventKeyCompare> EventMap;
  typedef std::map<Scheduler::EventKey, EventImpl*, MapScheduler::EventKeyCompare>::iterator EventMapI;
  typedef std::map<Scheduler::EventKey, EventImpl*, MapScheduler::EventKeyCompare>::const_iterator EventMapCI;


  EventMap m_list;
};

}; // namespace ns3


#endif /* SCHEDULER_MAP_H */
