/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Gustavo Carneiro
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
 * Author: Gustavo Carneiro <gjcarneiro@gmail.com> <gjc@inescporto.pt>
 */
#include <Python.h>
#include "visual-simulator-impl.h"
#include "ns3/default-simulator-impl.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("VisualSimulatorImpl");

namespace ns3 {



NS_OBJECT_ENSURE_REGISTERED (VisualSimulatorImpl);

namespace
{
ObjectFactory
GetDefaultSimulatorImplFactory ()
{
  ObjectFactory factory;
  factory.SetTypeId (DefaultSimulatorImpl::GetTypeId ());
  return factory;
}
}


TypeId
VisualSimulatorImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::VisualSimulatorImpl")
    .SetParent<SimulatorImpl> ()
    .AddConstructor<VisualSimulatorImpl> ()
    .AddAttribute ("SimulatorImplFactory",
                   "Factory for the underlying simulator implementation used by the visualizer.",
                   ObjectFactoryValue (GetDefaultSimulatorImplFactory ()),
                   MakeObjectFactoryAccessor (&VisualSimulatorImpl::m_simulatorImplFactory),
                   MakeObjectFactoryChecker ())
  ;
  return tid;
}


VisualSimulatorImpl::VisualSimulatorImpl ()
{
}

VisualSimulatorImpl::~VisualSimulatorImpl ()
{
}

void
VisualSimulatorImpl::DoDispose (void)
{
  if (m_simulator)
    {
      m_simulator->Dispose ();
      m_simulator = NULL;
    }
  SimulatorImpl::DoDispose ();
}

void
VisualSimulatorImpl::NotifyConstructionCompleted ()
{
  m_simulator = m_simulatorImplFactory.Create<SimulatorImpl> ();
}


void
VisualSimulatorImpl::Destroy ()
{
  m_simulator->Destroy ();
}

void
VisualSimulatorImpl::SetScheduler (ObjectFactory schedulerFactory)
{
  m_simulator->SetScheduler (schedulerFactory);
}

// System ID for non-distributed simulation is always zero
uint32_t 
VisualSimulatorImpl::GetSystemId (void) const
{
  return m_simulator->GetSystemId ();
}

bool 
VisualSimulatorImpl::IsFinished (void) const
{
  return m_simulator->IsFinished ();
}

Time
VisualSimulatorImpl::Next (void) const
{
  return m_simulator->Next ();
}

void
VisualSimulatorImpl::Run (void)
{
  if (!Py_IsInitialized ()) 
    {
      const char *argv[] = { "python", NULL};
      Py_Initialize ();
      PySys_SetArgv (1, (char**) argv);
    }
  PyRun_SimpleString (
    "import visualizer\n"
    "visualizer.start();\n"
    );
}

void
VisualSimulatorImpl::RunOneEvent (void)
{
  m_simulator->RunOneEvent ();
}

void 
VisualSimulatorImpl::Stop (void)
{
  m_simulator->Stop ();
}

void 
VisualSimulatorImpl::Stop (Time const &time)
{
  m_simulator->Stop (time);
}

//
// Schedule an event for a _relative_ time in the future.
//
EventId
VisualSimulatorImpl::Schedule (Time const &time, EventImpl *event)
{
  return m_simulator->Schedule (time, event);
}

void
VisualSimulatorImpl::ScheduleWithContext (uint32_t context, Time const &time, EventImpl *event)
{
  m_simulator->ScheduleWithContext (context, time, event);
}

EventId
VisualSimulatorImpl::ScheduleNow (EventImpl *event)
{
  return m_simulator->ScheduleNow (event);
}

EventId
VisualSimulatorImpl::ScheduleDestroy (EventImpl *event)
{
  return m_simulator->ScheduleDestroy (event);
}

Time
VisualSimulatorImpl::Now (void) const
{
  return m_simulator->Now ();
}

Time 
VisualSimulatorImpl::GetDelayLeft (const EventId &id) const
{
  return m_simulator->GetDelayLeft (id);
}

void
VisualSimulatorImpl::Remove (const EventId &id)
{
  m_simulator->Remove (id);
}

void
VisualSimulatorImpl::Cancel (const EventId &id)
{
  m_simulator->Cancel (id);
}

bool
VisualSimulatorImpl::IsExpired (const EventId &ev) const
{
  return m_simulator->IsExpired (ev);
}

Time 
VisualSimulatorImpl::GetMaximumSimulationTime (void) const
{
  return m_simulator->GetMaximumSimulationTime ();
}

uint32_t
VisualSimulatorImpl::GetContext (void) const
{
  return m_simulator->GetContext ();
}

void
VisualSimulatorImpl::RunRealSimulator (void)
{
  m_simulator->Run ();
}


} // namespace ns3


