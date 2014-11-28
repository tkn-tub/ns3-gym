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
#include "ns3/core-config.h"
#include "simulator.h"
#include "simulator-impl.h"
#include "scheduler.h"
#include "map-scheduler.h"
#include "event-impl.h"

#include "ptr.h"
#include "string.h"
#include "object-factory.h"
#include "global-value.h"
#include "assert.h"
#include "log.h"

#include <cmath>
#include <fstream>
#include <list>
#include <vector>
#include <iostream>

namespace ns3 {

// Note:  Logging in this file is largely avoided due to the
// number of calls that are made to these functions and the possibility
// of causing recursions leading to stack overflow
NS_LOG_COMPONENT_DEFINE ("Simulator");

static GlobalValue g_simTypeImpl = GlobalValue ("SimulatorImplementationType",
                                                "The object class to use as the simulator implementation",
                                                StringValue ("ns3::DefaultSimulatorImpl"),
                                                MakeStringChecker ());

static GlobalValue g_schedTypeImpl = GlobalValue ("SchedulerType",
                                                  "The object class to use as the scheduler implementation",
                                                  TypeIdValue (MapScheduler::GetTypeId ()),
                                                  MakeTypeIdChecker ());

static void
TimePrinter (std::ostream &os)
{
  os << Simulator::Now ().GetSeconds () << "s";
}

static void
NodePrinter (std::ostream &os)
{
  if (Simulator::GetContext () == 0xffffffff)
    {
      os << "-1";
    }
  else
    {
      os << Simulator::GetContext ();
    }
}

static SimulatorImpl **PeekImpl (void)
{
  static SimulatorImpl *impl = 0;
  return &impl;
}

static SimulatorImpl * GetImpl (void)
{
  SimulatorImpl **pimpl = PeekImpl ();
  /* Please, don't include any calls to logging macros in this function
   * or pay the price, that is, stack explosions.
   */
  if (*pimpl == 0)
    {
      {
        ObjectFactory factory;
        StringValue s;

        g_simTypeImpl.GetValue (s);
        factory.SetTypeId (s.Get ());
        *pimpl = GetPointer (factory.Create<SimulatorImpl> ());
      }
      {
        ObjectFactory factory;
        StringValue s;
        g_schedTypeImpl.GetValue (s);
        factory.SetTypeId (s.Get ());
        (*pimpl)->SetScheduler (factory);
      }

//
// Note: we call LogSetTimePrinter _after_ creating the implementation
// object because the act of creation can trigger calls to the logging 
// framework which would call the TimePrinter function which would call 
// Simulator::Now which would call Simulator::GetImpl, and, thus, get us 
// in an infinite recursion until the stack explodes.
//
      LogSetTimePrinter (&TimePrinter);
      LogSetNodePrinter (&NodePrinter);
    }
  return *pimpl;
}

void
Simulator::Destroy (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  SimulatorImpl **pimpl = PeekImpl (); 
  if (*pimpl == 0)
    {
      return;
    }
  /* Note: we have to call LogSetTimePrinter (0) below because if we do not do
   * this, and restart a simulation after this call to Destroy, (which is 
   * legal), Simulator::GetImpl will trigger again an infinite recursion until
   * the stack explodes.
   */
  LogSetTimePrinter (0);
  LogSetNodePrinter (0);
  (*pimpl)->Destroy ();
  (*pimpl)->Unref ();
  *pimpl = 0;
}

void
Simulator::SetScheduler (ObjectFactory schedulerFactory)
{
  NS_LOG_FUNCTION (schedulerFactory);
  GetImpl ()->SetScheduler (schedulerFactory);
}

bool 
Simulator::IsFinished (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return GetImpl ()->IsFinished ();
}

void 
Simulator::Run (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Time::ClearMarkedTimes ();
  GetImpl ()->Run ();
}

void 
Simulator::Stop (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_LOGIC ("stop");
  GetImpl ()->Stop ();
}

void 
Simulator::Stop (Time const &time)
{
  NS_LOG_FUNCTION (time);
  GetImpl ()->Stop (time);
}

Time
Simulator::Now (void)
{
  /* Please, don't include any calls to logging macros in this function
   * or pay the price, that is, stack explosions.
   */
  return GetImpl ()->Now ();
}

Time
Simulator::GetDelayLeft (const EventId &id)
{
  NS_LOG_FUNCTION (&id);
  return GetImpl ()->GetDelayLeft (id);
}

EventId
Simulator::Schedule (Time const &time, const Ptr<EventImpl> &ev)
{
  return DoSchedule (time, GetPointer (ev));
}

EventId
Simulator::ScheduleNow (const Ptr<EventImpl> &ev)
{
  return DoScheduleNow (GetPointer (ev));
}
void
Simulator::ScheduleWithContext (uint32_t context, const Time &time, EventImpl *impl)
{
  return GetImpl ()->ScheduleWithContext (context, time, impl);
}
EventId
Simulator::ScheduleDestroy (const Ptr<EventImpl> &ev)
{
  return DoScheduleDestroy (GetPointer (ev));
}
EventId 
Simulator::DoSchedule (Time const &time, EventImpl *impl)
{
  return GetImpl ()->Schedule (time, impl);
}
EventId 
Simulator::DoScheduleNow (EventImpl *impl)
{
  return GetImpl ()->ScheduleNow (impl);
}
EventId 
Simulator::DoScheduleDestroy (EventImpl *impl)
{
  return GetImpl ()->ScheduleDestroy (impl);
}


EventId
Simulator::Schedule (Time const &time, void (*f)(void))
{
  return DoSchedule (time, MakeEvent (f));
}

void
Simulator::ScheduleWithContext (uint32_t context, Time const &time, void (*f)(void))
{
  return ScheduleWithContext (context, time, MakeEvent (f));
}

EventId
Simulator::ScheduleNow (void (*f)(void))
{
  return DoScheduleNow (MakeEvent (f));
}

EventId
Simulator::ScheduleDestroy (void (*f)(void))
{
  return DoScheduleDestroy (MakeEvent (f));
}

void
Simulator::Remove (const EventId &ev)
{
  if (*PeekImpl () == 0)
    {
      return;
    }
  return GetImpl ()->Remove (ev);
}

void
Simulator::Cancel (const EventId &ev)
{
  if (*PeekImpl () == 0)
    {
      return;
    }
  return GetImpl ()->Cancel (ev);
}

bool 
Simulator::IsExpired (const EventId &id)
{
  if (*PeekImpl () == 0)
    {
      return true;
    }
  return GetImpl ()->IsExpired (id);
}

Time Now (void)
{
  return Time (Simulator::Now ());
}

Time 
Simulator::GetMaximumSimulationTime (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return GetImpl ()->GetMaximumSimulationTime ();
}

uint32_t
Simulator::GetContext (void)
{
  return GetImpl ()->GetContext ();
}

uint32_t
Simulator::GetSystemId (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (*PeekImpl () != 0)
    {
      return GetImpl ()->GetSystemId ();
    }
  else
    {
      return 0;
    }
}

void
Simulator::SetImplementation (Ptr<SimulatorImpl> impl)
{
  NS_LOG_FUNCTION (impl);
  if (*PeekImpl () != 0)
    {
      NS_FATAL_ERROR ("It is not possible to set the implementation after calling any Simulator:: function. Call Simulator::SetImplementation earlier or after Simulator::Destroy.");
    }
  *PeekImpl () = GetPointer (impl);
  // Set the default scheduler
  ObjectFactory factory;
  StringValue s;
  g_schedTypeImpl.GetValue (s);
  factory.SetTypeId (s.Get ());
  impl->SetScheduler (factory);
//
// Note: we call LogSetTimePrinter _after_ creating the implementation
// object because the act of creation can trigger calls to the logging 
// framework which would call the TimePrinter function which would call 
// Simulator::Now which would call Simulator::GetImpl, and, thus, get us 
// in an infinite recursion until the stack explodes.
//
  LogSetTimePrinter (&TimePrinter);
  LogSetNodePrinter (&NodePrinter);
}
Ptr<SimulatorImpl>
Simulator::GetImplementation (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return GetImpl ();
}



} // namespace ns3

