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

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "object.h"

namespace ns3 {

class EventImpl;

/**
 * \ingroup core
 * \defgroup scheduler Scheduler and Events
 * \brief Manage the event list by creating and scheduling events.
 */
/**
 * \ingroup scheduler
 * \defgroup events Events
 */
/**
 * \ingroup scheduler
 * \brief Maintain the event list
 *
 * This base class specifies the interface used to maintain the
 * event list. If you want to provide a new event list scheduler,
 * you need to create a subclass of this base class and implement
 * all the pure virtual methods defined here.
 *
 * The only tricky aspect of this API is the memory management of
 * the EventImpl pointer which is a member of the Event data structure.
 * The lifetime of this pointer is assumed to always be longer than
 * the lifetime of the Scheduler class which means that the caller
 * is responsible for ensuring that this invariant holds through
 * calling EventId::Ref and SimpleRefCount::Unref at the right time.
 * Typically, EventId::Ref is called before Insert and SimpleRefCount::Unref is called
 * after a call to one of the Remove methods.
 */
class Scheduler : public Object
{
public:
  static TypeId GetTypeId (void);

  /** \ingroup events */
  struct EventKey
  {
    uint64_t m_ts;
    uint32_t m_uid;
    uint32_t m_context;
  };
  /** \ingroup events */
  struct Event
  {
    EventImpl *impl;
    EventKey key;
  };

  virtual ~Scheduler () = 0;

  /**
   * \param ev event to store in the event list
   */
  virtual void Insert (const Event &ev) = 0;
  /**
   * \returns true if the event list is empty and false otherwise.
   */
  virtual bool IsEmpty (void) const = 0;
  /**
   * \returns a pointer to the next earliest event. The caller
   *      takes ownership of the returned pointer.
   *
   * This method cannot be invoked if the list is empty.
   */
  virtual Event PeekNext (void) const = 0;
  /**
   * This method cannot be invoked if the list is empty.
   * Remove the next earliest event from the event list.
   */
  virtual Event RemoveNext (void) = 0;
  /**
   * \param ev the event to remove
   *
   * This methods cannot be invoked if the list is empty.
   */
  virtual void Remove (const Event &ev) = 0;
};

/* Note the invariants which this function must provide:
 * - irreflexibility: f (x,x) is false)
 * - antisymmetry: f(x,y) = !f(y,x)
 * - transitivity: f(x,y) and f(y,z) => f(x,z)
 */
inline bool operator < (const Scheduler::EventKey &a, const Scheduler::EventKey &b)
{
  if (a.m_ts < b.m_ts)
    {
      return true;
    }
  else if (a.m_ts == b.m_ts
           && a.m_uid < b.m_uid)
    {
      return true;
    }
  else
    {
      return false;
    }
}
inline bool operator != (const Scheduler::EventKey &a, const Scheduler::EventKey &b)
{
  return a.m_uid != b.m_uid;
}
inline bool operator > (const Scheduler::EventKey &a, const Scheduler::EventKey &b)
{
  if (a.m_ts > b.m_ts)
    {
      return true;
    }
  else if (a.m_ts == b.m_ts
           && a.m_uid > b.m_uid)
    {
      return true;
    }
  else
    {
      return false;
    }
}



inline bool operator < (const Scheduler::Event &a, const Scheduler::Event &b)
{
  return a.key < b.key;
}


} // namespace ns3


#endif /* SCHEDULER_H */
