/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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
#ifndef EVENT_ID_H
#define EVENT_ID_H

#include <stdint.h>
#include "ns3/ptr.h"
#include "event-impl.h"

namespace ns3 {

class EventImpl;

/**
 * \brief an identifier for simulation events.
 */
class EventId {
public:
  EventId ();
  EventId (const Ptr<EventImpl> &impl, uint64_t ts, uint32_t uid);
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
  bool IsExpired (void);
  bool IsRunning (void);
public:
  /* The following methods are semi-private
   * they are supposed to be invoked only by
   * subclasses of the Scheduler base class.
   */
  Ptr<EventImpl> GetEventImpl (void) const;
  EventImpl *PeekEventImpl (void) const;
  uint64_t GetTs (void) const;
  uint32_t GetUid (void) const;
private:
  friend bool operator == (const EventId &a, const EventId &b);
  Ptr<EventImpl> m_eventImpl;
  uint64_t m_ts;
  uint32_t m_uid;
};

bool operator == (const EventId &a, const EventId &b);
bool operator != (const EventId &a, const EventId &b);

}; // namespace ns3

#endif /* EVENT_ID_H */
