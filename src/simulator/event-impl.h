/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#ifndef EVENT_IMPL_H
#define EVENT_IMPL_H

#include <stdint.h>

namespace ns3 {

/**
 * \ingroup simulator
 * \brief Base class for locking functionality for events.
 *
 * This class provides a cheap way (from the perspective of the event) to lock 
 * an event for multithreaded access.  This is a bit of a hack, but it lets us
 * use a whole lot of existing mechanism in the multithreaded simulator case.
 *
 * Here's a taste of the problem.  It makes life extraordinarily easier in the 
 * case of interfacing real network devices to be able to have threads reading
 * from real sockets and doing ScheduleNow calls to inject packets into ns-3.
 * It is desirable to have Schedule* calls all work similarly as well.  That is
 * you don't want to have to do different calls when working from an "external"
 * thread than you do when working in the context of the main simulation thread.
 *
 * It turns out that basically all of the Schedule* calls return an EventId.
 * Each EventId holds a reference to the underlying event.  Clients (see the
 * Applications for examples) often schedule events and hold onto the EventId
 * in case they need to cancel the event later.  The EventImpl that underlies 
 * all of this is reference counted and sharing an unprotected reference 
 * counted object between threads is a bad thing.
 *
 * There were several possible solutions:
 *
 * - Put a mutex into each event (costs 40 bytes for the mutex and a minumum of 
 *   three system calls to use an event);
 * - Work on the inheritance diagram of EventImpl to make a 
 *   MultithreadedEventImpl and pull apart all of the MakeEvent functions to 
 *   teach them to make the right kind of event based on a flag returned by the
 *   simulator;
 * - Rework the entire event mechanism to use raw pointers and avoid the entire
 *   reference counting approach with its associated problems;
 * - Provide a cheap way to use a shared mutex (and avoid using the mutex in 
 *   cases where it is not required).
 *
 * The original prototype chose the first option since it was easy.  I am very
 * hesitant to rework the entire event mechanism or even pull apart the MakeEvent
 * code at this point.  We went with the last option even though it feels a bit
 * like a hack.
 *
 * The result is the EventLock class.  If you have a simulator implementation that 
 * is going to need multithreaded access to events, you need to inherit from 
 * EventLock and provide a object that does real locking.  Give the object to the 
 * EventImpl code by calling EventImpl::SetEventLock, take it back by calling
 * EventImpl::SetNoEventLock or provide a new one.  The EventImpl code takes no
 * responsibility for the object passed in.
 *
 * \see EventImpl
 */
class EventLock
{
public:
  virtual ~EventLock () {};
  virtual void Lock (void) = 0;
  virtual void Unlock (void) = 0;
};

/**
 * \ingroup simulator
 * \brief a simulation event
 *
 * Each subclass of this base class represents a simulation event. The
 * EventImpl::Invoke method will be invoked by the simulation engine
 * when the time associated to this event expires. This class is
 * obviously (there are Ref and Unref methods) reference-counted and
 * most subclasses are usually created by one of the many Simulator::Schedule
 * methods.
 *
 * \see EventLock
 */
class EventImpl
{
public:
  EventImpl ();
  inline void Ref (void) const;
  inline void Unref (void) const;
  virtual ~EventImpl () = 0;
  /**
   * Called by the simulation engine to notify the event that it has expired.
   */
  void Invoke (void);
  /**
   * Marks the event as 'canceled'. The event will not be removed from
   * the event list but the simulation engine will check its canceled status
   * before calling Invoke.
   */
  void Cancel (void);
  /**
   * \returns true if the event has been canceled.
   *
   * Invoked by the simulation engine before calling Invoke.
   */
  bool IsCancelled (void);

  /**
   * Provide a mutex with Lock and Unlock methods to the event implementation
   * so that it can do cheap (from the perspective of event memory usage)
   * critical sections (mutual exclusion) in the reference counting code.
   *
   * \param eventLock Pointer to the EventLock object used to contain the
   * underlying mutex.
   */
  static void SetEventLock (EventLock *eventLock);
  /**
   * Remove any reference the event implementation code may hold to to an 
   * existing EventLock and disable the event locking feature.
   *
   * \see SetEventLock
   */
  static void SetNoEventLock (void);

protected:
  virtual void Notify (void) = 0;

private:
  bool m_cancel;
  mutable uint32_t m_count;
  static EventLock *m_eventLock;
};

}; // namespace ns3

namespace ns3 {

void
EventImpl::Ref (void) const
{
  if (m_eventLock)
    {
      m_eventLock->Lock ();
      m_count++;
      m_eventLock->Unlock ();
    }
  else
    {
      m_count++;
    }
}

void
EventImpl::Unref (void) const
{
  uint32_t c;

  if (m_eventLock)
    {
      m_eventLock->Lock ();
      c = --m_count;
      m_eventLock->Unlock ();
    }
  else
    {
      c = --m_count;
    }

  if (c == 0)
    {
      delete this;
    }
}

} // namespace ns3

#endif /* EVENT_IMPL_H */
