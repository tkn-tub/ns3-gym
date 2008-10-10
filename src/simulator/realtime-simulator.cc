/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
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
 */

#include "simulator.h"
#include "realtime-simulator.h"
#include "realtime-simulator-impl.h"
#include "event-impl.h"

#include "ns3/assert.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("RealtimeSimulatorExtension");

namespace ns3 {

RealtimeSimulatorImpl *
RealtimeSimulatorExtension::GetRealtimeImpl (void)
{
  RealtimeSimulatorImpl *impl = dynamic_cast<RealtimeSimulatorImpl *>(Simulator::GetImpl ());
  NS_ASSERT_MSG (impl, 
                 "RealtimeSimulatorExtension::GetImpl (): Underlying simulator implementation not realtime");
  return impl;
}

Time
RealtimeSimulatorExtension::RealNow (void)
{
  return GetRealtimeImpl ()->RealNow ();
}

EventId
RealtimeSimulatorExtension::ScheduleReal (Time const &time, const Ptr<EventImpl> &ev)
{
  NS_LOG_FUNCTION (time << ev);
  return GetRealtimeImpl ()->ScheduleReal (time, ev);
}

EventId
RealtimeSimulatorExtension::ScheduleRealNow (const Ptr<EventImpl> &ev)
{
  NS_LOG_FUNCTION (ev);
  return GetRealtimeImpl ()->ScheduleRealNow (ev);
}

EventId
RealtimeSimulatorExtension::ScheduleReal (Time const &time, void (*f) (void))
{
  NS_LOG_FUNCTION (time << f);
  return ScheduleReal (time, Simulator::MakeEvent (f));
}

EventId
RealtimeSimulatorExtension::ScheduleRealNow (void (*f) (void))
{
  NS_LOG_FUNCTION (f);
  return ScheduleRealNow (Simulator::MakeEvent (f));
}

} // namespace ns3
