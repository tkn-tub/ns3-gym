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
#ifndef EVENT_ID_H
#define EVENT_ID_H

#include <stdint.h>
#include "ns3/ptr.h"
#include "event-impl.h"

namespace ns3 {

class EventImpl;

/**
 * \ingroup simulator
 * \brief an identifier for simulation events.
 *
 * Each EventId identifies a unique event scheduled with one
 * of the many Simulator::Schedule methods. This EventId can
 * be used to Cancel or Remove events after they are scheduled
 * with Simulator::Cancel or Simulator::Remove.
 *
 * The important thing to remember about this class is that
 * every variable of this type is _always_ in a valid state, 
 * even when it has not been assigned an EventId coming from a Schedule
 * method: calling Cancel, IsRunning, IsExpired or passing around
 * instances of this object will not result in crashes or memory leaks.
 */
class EventId {
public:
  EventId ();
  // internal.
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
  bool IsExpired (void) const;
  /**
   * This method is syntactic sugar for the ns3::Simulator::isExpired
   * method.
   * \returns true if the event has not expired, false otherwise.
   */
  bool IsRunning (void) const;
public:
  /* The following methods are semi-private
   * they are supposed to be invoked only by
   * subclasses of the Scheduler base class.
   */
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
