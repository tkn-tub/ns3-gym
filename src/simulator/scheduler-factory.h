/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#ifndef SCHEDULER_FACTORY_H
#define SCHEDULER_FACTORY_H

#include <list>

namespace ns3 {

class Scheduler;
class StringEnumDefaultValue;

/**
 * \brief a base class to create event schedulers
 *
 * If you want to make the core simulation engine use a new
 * event scheduler without editing the code of the simulator,
 * you need to create a subclass of this base class and implement
 * the ns3::SchedulerFactory::realCreate method.
 */
class SchedulerFactory {
public:
  virtual ~SchedulerFactory ();
  /**
   * \returns a newly-created scheduler.
   */
  Scheduler *Create (void) const;
  /**
   * \returns a newly-created scheduler.
   *
   * Return a "default" scheduler.
   */
  static Scheduler *CreateDefault (void);
  /**
   * \param name of scheduler to create.
   * \returns a newly-created scheduler.
   *
   * Create a scheduler registered under the specified name.
   */
  static Scheduler *Create (const std::string &name);
protected:
  static void Add (const SchedulerFactory *factory,
                   const std::string &name);  
  static void AddDefault (const SchedulerFactory *factory,
                          const std::string &name);  
private:
  typedef std::list<std::pair<const SchedulerFactory *, std::string> > List;
  static SchedulerFactory::List *GetList (void);
  static StringEnumDefaultValue *GetDefault (void);
  /**
   * \returns a newly-created scheduler. The caller takes 
   *      ownership of the returned pointer.
   *
   * This method must be implemented by subclasses.
   */
  virtual Scheduler *DoCreate (void) const = 0;
};

}; // namespace ns3

#endif /* SCHEDULER_FACTORY_H */
