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
#include "des-metrics.h"

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
#include <iomanip>

/**
 * \file
 * \ingroup simulator
 * ns3::Simulator implementation, as well as implementation pointer,
 * global scheduler implementation, and default ns3::NodePrinter
 * and ns3::TimePrinter.
 */

namespace ns3 {

// Note:  Logging in this file is largely avoided due to the
// number of calls that are made to these functions and the possibility
// of causing recursions leading to stack overflow
NS_LOG_COMPONENT_DEFINE ("Simulator");

/**
 * \ingroup simulator
 * The specific simulator implementation to use.
 *
 * Must be derived from SimulatorImpl.
 */
static GlobalValue g_simTypeImpl = GlobalValue
  ("SimulatorImplementationType",
   "The object class to use as the simulator implementation",
   StringValue ("ns3::DefaultSimulatorImpl"),
   MakeStringChecker ());

/**
 * \ingroup scheduler
 * The specific event scheduler implementation to use.
 *
 * Must be derived from Scheduler.
 */
static GlobalValue g_schedTypeImpl = GlobalValue ("SchedulerType",
                                                  "The object class to use as the scheduler implementation",
                                                  TypeIdValue (MapScheduler::GetTypeId ()),
                                                  MakeTypeIdChecker ());

/**
 * \ingroup logging
 * Default TimePrinter implementation.
 *
 * \param [in,out] os The output stream to print the time on.
 */
static void
TimePrinter (std::ostream &os)
{
  std::ios_base::fmtflags ff = os.flags (); // Save stream flags
  std::streamsize oldPrecision = os.precision ();
  if (Time::GetResolution () == Time::NS)
    {
      os << std::fixed << std::setprecision (9) << Simulator::Now ().As (Time::S);
    }
  else if (Time::GetResolution () == Time::PS) 
    {
      os << std::fixed << std::setprecision (12) << Simulator::Now ().As (Time::S);
    }
  else if (Time::GetResolution () == Time::FS) 
    {
      os << std::fixed << std::setprecision (15) << Simulator::Now ().As (Time::S);
    }
  else if (Time::GetResolution () == Time::US) 
    {
      os << std::fixed << std::setprecision (6) << Simulator::Now ().As (Time::S);
    }
  else
    {
      // default C++ precision of 5
      os << std::fixed << std::setprecision (5) << Simulator::Now ().As (Time::S);
    }
  os << std::setprecision (oldPrecision);
  os.flags (ff); // Restore stream flags
}

/**
 * \ingroup logging
 * Default node id printer implementation.
 * 
 * \param [in,out] os The output stream to print the node id on.
 */
static void
NodePrinter (std::ostream &os)
{
  if (Simulator::GetContext () == Simulator::NO_CONTEXT)
    {
      os << "-1";
    }
  else
    {
      os << Simulator::GetContext ();
    }
}

/**
 * \ingroup simulator
 * \brief Get the static SimulatorImpl instance.
 * \return The SimulatorImpl instance pointer.
 */
static SimulatorImpl **PeekImpl (void)
{
  static SimulatorImpl *impl = 0;
  return &impl;
}

/**
 * \ingroup simulator
 * \brief Get the SimulatorImpl singleton.
 * \return The singleton pointer.
 * \see Simulator::GetImplementation()
 */
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
Simulator::Stop (Time const &delay)
{
  NS_LOG_FUNCTION (delay);
  GetImpl ()->Stop (delay);
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
Simulator::Schedule (Time const &delay, const Ptr<EventImpl> &event)
{
  return DoSchedule (delay, GetPointer (event));
}

EventId
Simulator::ScheduleNow (const Ptr<EventImpl> &ev)
{
  return DoScheduleNow (GetPointer (ev));
}
void
Simulator::ScheduleWithContext (uint32_t context, const Time &delay, EventImpl *impl)
{
#ifdef ENABLE_DES_METRICS
  DesMetrics::Get ()->TraceWithContext (context, Now (), delay);
#endif
  return GetImpl ()->ScheduleWithContext (context, delay, impl);
}
EventId
Simulator::ScheduleDestroy (const Ptr<EventImpl> &ev)
{
  return DoScheduleDestroy (GetPointer (ev));
}
EventId 
Simulator::DoSchedule (Time const &time, EventImpl *impl)
{
#ifdef ENABLE_DES_METRICS
  DesMetrics::Get ()->Trace (Now (), time);
#endif
  return GetImpl ()->Schedule (time, impl);
}
EventId 
Simulator::DoScheduleNow (EventImpl *impl)
{
#ifdef ENABLE_DES_METRICS
  DesMetrics::Get ()->Trace (Now (), Time (0));
#endif
  return GetImpl ()->ScheduleNow (impl);
}
EventId 
Simulator::DoScheduleDestroy (EventImpl *impl)
{
  return GetImpl ()->ScheduleDestroy (impl);
}


EventId
Simulator::Schedule (Time const &delay, void (*f)(void))
{
  return DoSchedule (delay, MakeEvent (f));
}

void
Simulator::ScheduleWithContext (uint32_t context, Time const &delay, void (*f)(void))
{
  return ScheduleWithContext (context, delay, MakeEvent (f));
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
Simulator::Remove (const EventId &id)
{
  if (*PeekImpl () == 0)
    {
      return;
    }
  return GetImpl ()->Remove (id);
}

void
Simulator::Cancel (const EventId &id)
{
  if (*PeekImpl () == 0)
    {
      return;
    }
  return GetImpl ()->Cancel (id);
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

