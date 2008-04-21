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
#include "ns3/simulator.h"
#include <algorithm>
#include <cmath>
#include "random-direction-2d-mobility-model.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("RandomDirection2dMobilityModel");

namespace ns3 {

const double RandomDirection2dMobilityModel::PI = 3.14159265358979323846;

NS_OBJECT_ENSURE_REGISTERED (RandomDirection2dMobilityModel);


TypeId
RandomDirection2dMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RandomDirection2dMobilityModel")
    .SetParent<MobilityModel> ()
    .SetGroupName ("Mobility")
    .AddConstructor<RandomDirection2dMobilityModel> ()
    .AddAttribute ("Bounds", "The 2d bounding area",
                   RectangleValue (Rectangle (-100, 100, -100, 100)),
                   MakeRectangleAccessor (&RandomDirection2dMobilityModel::m_bounds),
                   MakeRectangleChecker ())
    .AddAttribute ("Speed", "A random variable to control the speed (m/s).",
                   RandomVariableValue (UniformVariable (1.0, 2.0)),
                   MakeRandomVariableAccessor (&RandomDirection2dMobilityModel::m_speed),
                   MakeRandomVariableChecker ())
    .AddAttribute ("Pause", "A random variable to control the pause (s).",
                   RandomVariableValue (ConstantVariable (2.0)),
                   MakeRandomVariableAccessor (&RandomDirection2dMobilityModel::m_pause),
                   MakeRandomVariableChecker ())
    ;
  return tid;
}


RandomDirection2dMobilityModel::RandomDirection2dMobilityModel ()
{
  m_event = Simulator::ScheduleNow (&RandomDirection2dMobilityModel::Start, this);
}
void 
RandomDirection2dMobilityModel::DoDispose (void)
{
  // chain up.
  MobilityModel::DoDispose ();
}
void
RandomDirection2dMobilityModel::Start (void)
{
  double direction = UniformVariable::GetSingleValue (0, 2 * PI);
  SetDirectionAndSpeed (direction);
}

void
RandomDirection2dMobilityModel::BeginPause (void)
{
  Time pause = Seconds (m_pause.GetValue ());
  m_helper.Pause ();
  m_event = Simulator::Schedule (pause, &RandomDirection2dMobilityModel::ResetDirectionAndSpeed, this);
  NotifyCourseChange ();
}

void
RandomDirection2dMobilityModel::SetDirectionAndSpeed (double direction)
{
  NS_LOG_FUNCTION_NOARGS ();
  double speed = m_speed.GetValue ();
  const Vector vector (std::cos (direction) * speed,
                       std::sin (direction) * speed,
                       0.0);
  Vector position = m_helper.GetCurrentPosition (m_bounds);
  m_helper.Reset (vector);
  Vector next = m_bounds.CalculateIntersection (position, vector);
  Time delay = Seconds (CalculateDistance (position, next) / speed);
  m_event = Simulator::Schedule (delay,
				 &RandomDirection2dMobilityModel::BeginPause, this);
  NotifyCourseChange ();
}
void
RandomDirection2dMobilityModel::ResetDirectionAndSpeed (void)
{
  double direction = UniformVariable::GetSingleValue (0, PI);
  
  Vector position = m_helper.GetCurrentPosition (m_bounds);
  switch (m_bounds.GetClosestSide (position))
    {
    case Rectangle::RIGHT:
      direction += PI / 2;
      break;
    case Rectangle::LEFT:
      direction += - PI / 2;
      break;
    case Rectangle::TOP:
      direction += PI;
      break;
    case Rectangle::BOTTOM:
      direction += 0.0;
      break;
    }
  SetDirectionAndSpeed (direction);
}
Vector
RandomDirection2dMobilityModel::DoGetPosition (void) const
{
  return m_helper.GetCurrentPosition (m_bounds);
}
void
RandomDirection2dMobilityModel::DoSetPosition (const Vector &position)
{
  m_helper.InitializePosition (position);
  Simulator::Remove (m_event);
  m_event = Simulator::ScheduleNow (&RandomDirection2dMobilityModel::Start, this);
}
Vector
RandomDirection2dMobilityModel::DoGetVelocity (void) const
{
  return m_helper.GetVelocity ();
}



} // namespace ns3
