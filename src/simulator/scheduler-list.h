/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifndef SCHEDULER_LIST_H
#define SCHEDULER_LIST_H

#include "scheduler.h"
#include "event-id.h"
#include <list>
#include <utility>
#include <stdint.h>

namespace ns3 {

class EventImpl;

class SchedulerList : public Scheduler {
 public:
  SchedulerList ();
  virtual ~SchedulerList ();

  virtual void Insert (const EventId &id);
  virtual bool IsEmpty (void) const;
  virtual EventId PeekNext (void) const;
  virtual EventId RemoveNext (void);
  virtual bool Remove (const EventId &ev);

 private:
  inline bool IsLower (Scheduler::EventKey const*a, Scheduler::EventKey const*b) const;

  typedef std::list<std::pair<EventImpl*, EventKey> > Events;
  typedef std::list<std::pair<EventImpl*, EventKey> >::iterator EventsI;
  Events m_events;
};

}; // namespace ns3


#endif /* SCHEDULER_LIST_H */
