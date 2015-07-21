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

/**
 * \file
 * \ingroup scheduler
 * \ingroup events
 * ns3::Scheduler abstract base class, ns3::Scheduler::Event and
 * ns3::Scheduler::EventKey declarations.
 */

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
  /**
   *  Register this type.
   *  \return The object TypeId.
   */
  static TypeId GetTypeId (void);

  /**
   * \ingroup events
   * Structure for sorting and comparing Events.
   */
  struct EventKey
  {
    uint64_t m_ts;         /**< Event time stamp. */
    uint32_t m_uid;        /**< Event unique id. */
    uint32_t m_context;    /**< Event context. */
  };
  /**
   * \ingroup events
   * Scheduler event.
   *
   * An Event consists of an EventKey, used for maintaining the schedule,
   * and an EventImpl which is the actual implementation.
   */
  struct Event
  {
    EventImpl *impl;       /**< Pointer to the event implementation. */
    EventKey key;          /**< Key for sorting and ordering Events. */
  };

  /** Destructor. */
  virtual ~Scheduler () = 0;

  /**
   * Insert a new Event in the schedule.
   *
   * \param ev Event to store in the event list
   */
  virtual void Insert (const Event &ev) = 0;
  /**
   * Test if the schedule is empty.
   *
   * \returns true if the event list is empty and false otherwise.
   */
  virtual bool IsEmpty (void) const = 0;
  /**
   * Get a pointer to the next event.
   *
   * This method cannot be invoked if the list is empty.
   *
   * \returns a pointer to the next earliest event. The caller
   *      takes ownership of the returned pointer.
   */
  virtual Event PeekNext (void) const = 0;
  /**
   * Remove the earliest event from the event list.
   *
   * This method cannot be invoked if the list is empty.
   *
   * \return The Event.
   */
  virtual Event RemoveNext (void) = 0;
  /**
   * Remove a specific event from the event list.
   *
   * This method cannot be invoked if the list is empty.
   *
   * \param ev the event to remove
   */
  virtual void Remove (const Event &ev) = 0;
};

/**
 * \ingroup Events
 * Compare (less than) two events by EventKey.
 *
 * Note the invariants which this function must provide:
 * - irreflexibility: f (x,x) is false
 * - antisymmetry: f(x,y) = !f(y,x)
 * - transitivity: f(x,y) and f(y,z) => f(x,z)
 *
 * \param [in] a The first event.
 * \param [in] b The second event.
 * \returns \c true if \c a < \c b
 */
inline bool operator < (const Scheduler::EventKey &a,
                        const Scheduler::EventKey &b)
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

/**
 * \ingroup Events
 * Compare (not equal) two events by EventKey.
 *
 * \param [in] a The first event.
 * \param [in] b The second event.
 * \returns \c true if \c a != \c b
 */
inline bool operator != (const Scheduler::EventKey &a,
                         const Scheduler::EventKey &b)
{
  return a.m_uid != b.m_uid;
}

/**
 * Compare (greater than) two events by EventKey.
 *
 * \param [in] a The first event.
 * \param [in] b The second event.
 * \returns \c true if \c a > \c b
 */
inline bool operator > (const Scheduler::EventKey &a,
                        const Scheduler::EventKey &b)
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

/**
 * Compare (less than) two events by Event.
 *
 * \param [in] a The first event.
 * \param [in] b The second event.
 * \returns \c true if \c a < \c b
 */
inline bool operator < (const Scheduler::Event &a,
                        const Scheduler::Event &b)
{
  return a.key < b.key;
}


} // namespace ns3


#endif /* SCHEDULER_H */
