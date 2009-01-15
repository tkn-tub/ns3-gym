/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 1994 Regents of the University of California.
 * Copyright (c) 2009 INRIA
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
 * Authors: 
 *  David Wetherall <djw@juniper.lcs.mit.edu>: originally, in ns-2, back in 1997, 
 *  Mathieu Lacage <mathieu.lacage@sophia.inria.fr>: port to ns-3
 */

#ifndef NS2_CALENDAR_SCHEDULER_H
#define NS2_CALENDAR_SCHEDULER_H

#include "scheduler.h"
#include <stdint.h>
#include <list>

namespace ns3 {

class EventImpl;

/**
 * \ingroup scheduler
 * \brief a calendar queue event scheduler
 *
 * This event scheduler is a copy/paste of the ns2.29 calendar scheduler.
 */
class Ns2CalendarScheduler : public Scheduler 
{
public:
  static TypeId GetTypeId (void);

  Ns2CalendarScheduler ();
  virtual ~Ns2CalendarScheduler ();

  virtual void Insert (const Event &ev);
  virtual bool IsEmpty (void) const;
  virtual Event PeekNext (void) const;
  virtual Event RemoveNext (void);
  virtual void Remove (const Event &ev);

private:
};

} // namespace ns3

#endif /* NS2_CALENDAR_SCHEDULER_H */
