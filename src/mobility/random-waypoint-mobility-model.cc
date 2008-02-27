/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include <cmath>
#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "ns3/random-variable-default-value.h"
#include "ns3/type-id-default-value.h"
#include "random-waypoint-mobility-model.h"
#include "position-allocator.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RandomWaypointMobilityModel);

TypeId
RandomWaypointMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("RandomWaypointMobilityModel")
    .SetParent<MobilityModel> ()
    .SetGroupName ("Mobility")
    .AddConstructor<RandomWaypointMobilityModel> ()
    .AddAttribute ("speed",
                   "A random variable used to pick the speed of a random waypoint model.",
                   UniformVariable (0.3, 0.7),
                   MakeRandomVariableAccessor (&RandomWaypointMobilityModel::m_speed),
                   MakeRandomVariableChecker ())
    .AddAttribute ("pause",
                   "A random variable used to pick the pause of a random waypoint model.",
                   ConstantVariable (2.0),
                   MakeRandomVariableAccessor (&RandomWaypointMobilityModel::m_pause),
                   MakeRandomVariableChecker ())
    .AddAttribute ("position",
                   "The position model used to pick a destination point.",
                   Ptr<PositionAllocator> (0),
                   MakePtrAccessor (&RandomWaypointMobilityModel::m_position),
                   MakePtrChecker<PositionAllocator> ());
  
  return tid;
}

RandomWaypointMobilityModel::RandomWaypointMobilityModel ()
{
  Simulator::ScheduleNow (&RandomWaypointMobilityModel::Start, this);
}

void
RandomWaypointMobilityModel::BeginWalk (void)
{
  Vector m_current = m_helper.GetCurrentPosition ();
  Vector destination = m_position->GetNext ();
  double speed = m_speed.GetValue ();
  double dx = (destination.x - m_current.x);
  double dy = (destination.y - m_current.y);
  double dz = (destination.z - m_current.z);
  double k = speed / std::sqrt (dx*dx + dy*dy + dz*dz);

  m_helper.Reset (Vector (k*dx, k*dy, k*dz));
  Time travelDelay = Seconds (CalculateDistance (destination, m_current) / speed);
  m_event = Simulator::Schedule (travelDelay,
				 &RandomWaypointMobilityModel::Start, this);
  NotifyCourseChange ();
}

void
RandomWaypointMobilityModel::Start (void)
{
  Time pause = Seconds (m_pause.GetValue ());
  m_helper.Pause ();
  NotifyCourseChange ();
  m_event = Simulator::Schedule (pause, &RandomWaypointMobilityModel::BeginWalk, this);
}

Vector
RandomWaypointMobilityModel::DoGetPosition (void) const
{
  return m_helper.GetCurrentPosition ();
}
void 
RandomWaypointMobilityModel::DoSetPosition (const Vector &position)
{
  m_helper.InitializePosition (position);
  Simulator::Remove (m_event);
  Simulator::ScheduleNow (&RandomWaypointMobilityModel::Start, this);
}
Vector
RandomWaypointMobilityModel::DoGetVelocity (void) const
{
  return m_helper.GetVelocity ();
}


} // namespace ns3
