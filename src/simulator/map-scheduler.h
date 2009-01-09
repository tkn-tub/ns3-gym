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

#ifndef MAP_SCHEDULER_H
#define MAP_SCHEDULER_H

#include "scheduler.h"
#include <stdint.h>
#include <map>
#include <utility>

namespace ns3 {

/**
 * \ingroup scheduler
 * \brief a std::map event scheduler
 *
 * This class implements the an event scheduler using an std::map 
 * data structure.
 */
class MapScheduler : public Scheduler 
{
public:
  static TypeId GetTypeId (void);

  MapScheduler ();
  virtual ~MapScheduler ();

  virtual void Insert (const Event &ev);
  virtual bool IsEmpty (void) const;
  virtual Event PeekNext (void) const;
  virtual Event RemoveNext (void);
  virtual void Remove (const Event &ev);
private:

  typedef std::map<Scheduler::EventKey, EventImpl*> EventMap;
  typedef std::map<Scheduler::EventKey, EventImpl*>::iterator EventMapI;
  typedef std::map<Scheduler::EventKey, EventImpl*>::const_iterator EventMapCI;


  EventMap m_list;
};

} // namespace ns3

#endif /* MAP_SCHEDULER_H */
