/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
 * All rights reserved.
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
#include "random-walk-2d-mobility-model.h"
#include "ns3/enum.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include <cmath>

NS_LOG_COMPONENT_DEFINE ("RandomWalk2d");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RandomWalk2dMobilityModel);

TypeId
RandomWalk2dMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("RandomWalk2dMobilityModel")
    .SetParent<MobilityModel> ()
    .SetGroupName ("Mobility")
    .AddConstructor<RandomWalk2dMobilityModel> ()
    .AddAttribute ("Bounds",
                   "Bounds of the area to cruise.",
                   Rectangle (0.0, 0.0, 100.0, 100.0),
                   MakeRectangleAccessor (&RandomWalk2dMobilityModel::m_bounds),
                   MakeRectangleChecker ())
    .AddAttribute ("Time",
                   "Change current direction and speed after moving for this delay.",
                   Seconds (1.0),
                   MakeTimeAccessor (&RandomWalk2dMobilityModel::m_modeTime),
                   MakeTimeChecker ())
    .AddAttribute ("Distance",
                   "Change current direction and speed after moving for this distance.",
                   Seconds (1.0),
                   MakeTimeAccessor (&RandomWalk2dMobilityModel::m_modeTime),
                   MakeTimeChecker ())
    .AddAttribute ("Mode",
                   "The mode indicates the condition used to "
                   "change the current speed and direction",
                   Enum (RandomWalk2dMobilityModel::MODE_DISTANCE),
                   MakeEnumAccessor (&RandomWalk2dMobilityModel::m_mode),
                   MakeEnumChecker (RandomWalk2dMobilityModel::MODE_DISTANCE, "Distance",
                                    RandomWalk2dMobilityModel::MODE_TIME, "Time"))
    .AddAttribute ("Direction",
                   "A random variable used to pick the direction (gradients).",
                   UniformVariable (0.0, 6.283184),
                   MakeRandomVariableAccessor (&RandomWalk2dMobilityModel::m_direction),
                   MakeRandomVariableChecker ())
    .AddAttribute ("Speed",
                   "A random variable used to pick the speed (m/s).",
                   UniformVariable (2.0, 4.0),
                   MakeRandomVariableAccessor (&RandomWalk2dMobilityModel::m_speed),
                   MakeRandomVariableChecker ());
  return tid;
}

RandomWalk2dMobilityModel::RandomWalk2dMobilityModel ()
{
  m_event = Simulator::ScheduleNow (&RandomWalk2dMobilityModel::Start, this);
}

void
RandomWalk2dMobilityModel::Start (void)
{
  double speed = m_speed.GetValue ();
  double direction = m_direction.GetValue ();
  Vector vector (std::cos (direction) * speed,
                 std::sin (direction) * speed,
                 0.0);
  m_helper.Reset (vector);

  Time delayLeft;
  if (m_mode == RandomWalk2dMobilityModel::MODE_TIME)
    {
      delayLeft = m_modeTime;
    }
  else
    {
      delayLeft = Seconds (m_modeDistance / speed); 
    }
  DoWalk (delayLeft);
}

void
RandomWalk2dMobilityModel::DoWalk (Time delayLeft)
{
  Vector position = m_helper.GetCurrentPosition ();
  Vector speed = m_helper.GetVelocity ();
  Vector nextPosition = position;
  nextPosition.x += speed.x * delayLeft.GetSeconds ();
  nextPosition.y += speed.y * delayLeft.GetSeconds ();
  if (m_bounds.IsInside (nextPosition))
    {
      m_event = Simulator::Schedule (delayLeft, &RandomWalk2dMobilityModel::Start, this);
    }
  else
    {
      nextPosition = m_bounds.CalculateIntersection (position, speed);
      Time delay = Seconds ((nextPosition.x - position.x) / speed.x);
      m_event = Simulator::Schedule (delay, &RandomWalk2dMobilityModel::Rebound, this,
                                     delayLeft - delay);      
    }  
  NotifyCourseChange ();
}

void
RandomWalk2dMobilityModel::Rebound (Time delayLeft)
{
  Vector position = m_helper.GetCurrentPosition (m_bounds);
  Vector speed = m_helper.GetVelocity ();
  switch (m_bounds.GetClosestSide (position))
    {
    case Rectangle::RIGHT:
    case Rectangle::LEFT:
      speed.x = - speed.x;
      break;
    case Rectangle::TOP:
    case Rectangle::BOTTOM:
      speed.y = - speed.y;
      break;
    }
  m_helper.Reset (speed);
  DoWalk (delayLeft);
}

void
RandomWalk2dMobilityModel::DoDispose (void)
{
  // chain up
  MobilityModel::DoDispose ();
}
Vector
RandomWalk2dMobilityModel::DoGetPosition (void) const
{
  return m_helper.GetCurrentPosition (m_bounds);
}
void
RandomWalk2dMobilityModel::DoSetPosition (const Vector &position)
{
  NS_ASSERT (m_bounds.IsInside (position));
  m_helper.InitializePosition (position);
  Simulator::Remove (m_event);
  m_event = Simulator::ScheduleNow (&RandomWalk2dMobilityModel::Start, this);
}
Vector
RandomWalk2dMobilityModel::DoGetVelocity (void) const
{
  return m_helper.GetVelocity ();
}



} // namespace ns3
