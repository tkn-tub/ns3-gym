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
#include "scheduler-factory.h"
#include "ns3/assert.h"
#include "ns3/fatal-error.h"
#include "ns3/default-value.h"

namespace ns3 {

SchedulerFactory::~SchedulerFactory ()
{}

Scheduler *
SchedulerFactory::Create (void) const
{
  return DoCreate ();
}

Scheduler *
SchedulerFactory::CreateDefault (void)
{
  NS_ASSERT_MSG (!GetList ()->empty (), "No Scheduler factory registered");
  std::string defaultValue = GetDefault ()->GetValue ();
  for (List::const_iterator i = GetList ()->begin ();
       i != GetList ()->end (); i++)
    {
      if (i->second == defaultValue)
        {
          return i->first->Create ();
        }
    }
  NS_ASSERT (false);
  // quiet compiler
  return 0;
}

Scheduler *
SchedulerFactory::Create (const std::string &name)
{
  for (List::iterator i = GetList ()->begin ();
       i != GetList ()->end (); i++)
    {
      if (i->second == name)
        {
          return i->first->Create ();
        }
    }
  NS_ASSERT_MSG (false, "Tried to create non-existant scheduler: " << name);
  // quiet compiler.
  return 0;
}

void 
SchedulerFactory::AddDefault (const SchedulerFactory *factory,
                              const std::string &name)
{
  GetDefault ()->AddDefaultValue (name);
  GetList ()->push_back (std::make_pair (factory, name));
}


void 
SchedulerFactory::Add (const SchedulerFactory *factory,
                       const std::string &name)
{
  GetDefault ()->AddPossibleValue (name);
  GetList ()->push_back (std::make_pair (factory, name));
}

StringEnumDefaultValue *
SchedulerFactory::GetDefault (void)
{
  static StringEnumDefaultValue value ("Scheduler", "Event Scheduler algorithm");
  return &value;
}

SchedulerFactory::List *
SchedulerFactory::GetList (void)
{
  static SchedulerFactory::List list;
  return &list;
}


}; // namespace ns3
