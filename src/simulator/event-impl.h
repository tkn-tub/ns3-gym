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
 * \brief a simulation event
 *
 * Each subclass of this base class represents a simulation event. The
 * EventImpl::Invoke method will be invoked by the simulation engine
 * when the time associated to this event expires. This class is
 * obviously (there are Ref and Unref methods) reference-counted and
 * most subclasses are usually created by one of the many Simulator::Schedule
 * methods.
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

protected:
  virtual void Notify (void) = 0;

private:
  bool m_cancel;
  mutable uint32_t m_count;
};

}; // namespace ns3

namespace ns3 {

void
EventImpl::Ref (void) const
{
  m_count++;
}

void
EventImpl::Unref (void) const
{
  uint32_t c;
  c = --m_count;
  if (c == 0)
    {
      delete this;
    }
}

} // namespace ns3

#endif /* EVENT_IMPL_H */
