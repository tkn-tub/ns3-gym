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
#ifndef SINGLE_EVENT_H
#define SINGLE_EVENT_H

#include <list>
#include "ns3/event-id.h"
#include "ns3/simulator.h"

namespace ns3 {

/**
 * \brief An object that tracks a single event
 *
 * A SingleEvent acts as a smart proxy for an EventId. It
 * automatically cancels the underlying EventId when it is replaced
 * for a new EventId, or when the SingleEvent is destroyed.  It is
 * useful in situations where only one type of an event can possibly
 * be scheduled at any single time (e.g. a single timer instance), and
 * when the event should be limited to the lifetime of a container
 * object.
 */
class SingleEvent
{
public:

  SingleEvent ();
  ~SingleEvent ();

  /**
   * \brief Tracks a newly scheduled event.
   *
   * Tracks a newly scheduled event.  Any previous event that may have
   * been scheduled is first cancelled.
   */
  void operator = (const EventId &event);

  /**
   * This method is syntactic sugar for the ns3::Simulator::cancel
   * method.
   */
  void Cancel (void);
  /**
   * This method is syntactic sugar for the ns3::Simulator::isExpired
   * method.
   * \returns true if the event has expired, false otherwise.
   */
  bool IsExpired (void) const;
  bool IsRunning (void) const;

private:

  EventId m_event;
};

}; // namespace ns3

#endif /* SINGLE_EVENT_H */
