/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INESC Porto
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
 * Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt>
 */
#ifndef EVENT_COLLECTOR_H
#define EVENT_COLLECTOR_H

#include <list>
#include "event-id.h"
#include "simulator.h"

namespace ns3 {

/**
 * \brief An object that tracks scheduled events and automatically
 * cancels them when it is destroyed.  It is useful in situations
 * where multiple instances of the same type of event can
 * simultaneously be scheduled, and when the events should be limited
 * to the lifetime of a container object.
 */
class EventCollector
{
public:

  EventCollector ();

  /**
   * \brief Tracks a new event
   */
  void Track (EventId event);

  ~EventCollector ();

private:

  std::list<EventId>::size_type m_nextCleanupSize;
  std::list<EventId> m_events;

  void Cleanup ();
  void Grow ();
  void Shrink ();
};

}; // namespace ns3

#endif /* EVENT_COLLECTOR_H */
