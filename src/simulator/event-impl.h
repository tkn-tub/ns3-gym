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
#include "ns3/object-base.h"

namespace ns3 {

class EventImpl
{
public:
  EventImpl ();
  inline void Ref (void) const;
  inline void Unref (void) const;
  virtual ~EventImpl () = 0;
  void Invoke (void);
  void Cancel (void);
  bool IsCancelled (void);
protected:
  virtual void Notify (void) = 0;
private:
  friend class Event;
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
  m_count--;
  if (m_count == 0)
    {
      delete this;
    }
}

} // namespace ns3

#endif /* EVENT_IMPL_H */
