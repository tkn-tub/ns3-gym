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
#include "ns3/default-value.h"
#include "ns3/time-default-value.h"
#include "ns3/rectangle-default-value.h"
#include "ns3/random-variable-default-value.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include <cmath>

NS_LOG_COMPONENT_DEFINE ("RandomWalk2d");

namespace ns3 {

const ClassId RandomWalk2dMobilityModel::cid = 
  MakeClassId<RandomWalk2dMobilityModel> ("RandomWalk2dMobilityModel", RandomWalk2dMobilityModel::iid);


static EnumDefaultValue<RandomWalk2dMobilityModelParameters::Mode> 
g_mode ("RandomWalk2dMode",
        "The mode indicates the condition used to "
        "change the current speed and direction",
        RandomWalk2dMobilityModelParameters::MODE_DISTANCE, "Distance",
        RandomWalk2dMobilityModelParameters::MODE_TIME, "Time",
        0, (void*)0);

static NumericDefaultValue<double>
g_modeDistance ("RandomWalk2dDistance",
                "Change current direction and speed after moving this distance.",
                2.0);

static TimeDefaultValue
g_modeTime ("RandomWalk2dTime",
             "Change current direction and speed after moving for this delay.",
             Seconds (1.0));

static RandomVariableDefaultValue
g_speed ("RandomWalk2dSpeed",
         "A random variable used to pick the speed.",
         "Uniform:2:4");
static RandomVariableDefaultValue
g_direction ("RandomWalk2dDirection",
             "A random variable used to pick the direction (gradients).",
             "Uniform:0.0:6.283184");

static RectangleDefaultValue
g_rectangle ("RandomWalk2dBounds",
             "Bounds of the area to cruise.",
             0.0, 0.0, 100.0, 100.0);

RandomWalk2dMobilityModelParameters::RandomWalk2dMobilityModelParameters ()
  : m_mode (g_mode.GetValue ()),
    m_modeDistance (g_modeDistance.GetValue ()),
    m_modeTime (g_modeTime.GetValue ()),
    m_speed (g_speed.GetCopy ()),
    m_direction (g_direction.GetCopy ()),
    m_bounds (g_rectangle.GetValue ())
{}

RandomWalk2dMobilityModelParameters::~RandomWalk2dMobilityModelParameters ()
{
  delete m_speed;
  delete m_direction;
  m_speed = 0;
  m_direction = 0;
}

void 
RandomWalk2dMobilityModelParameters::SetSpeed (const RandomVariable &speed)
{
  delete m_speed;
  m_speed = speed.Copy ();
}
void 
RandomWalk2dMobilityModelParameters::SetDirection (const RandomVariable &direction)
{
  delete m_direction;
  m_direction = direction.Copy ();
}
void 
RandomWalk2dMobilityModelParameters::SetModeDistance (double distance)
{
  m_mode = RandomWalk2dMobilityModelParameters::MODE_DISTANCE;
  m_modeDistance = distance;
}
void 
RandomWalk2dMobilityModelParameters::SetModeTime (Time time)
{
  m_mode = RandomWalk2dMobilityModelParameters::MODE_TIME;
  m_modeTime = time;
}
void 
RandomWalk2dMobilityModelParameters::SetBounds (const Rectangle &bounds)
{
  m_bounds = bounds;
}

Ptr<RandomWalk2dMobilityModelParameters> 
RandomWalk2dMobilityModelParameters::GetCurrent (void)
{
  static Ptr<RandomWalk2dMobilityModelParameters> parameters = 0;
  if (parameters == 0 ||
      g_speed.IsDirty () ||
      g_direction.IsDirty () ||
      g_mode.IsDirty () ||
      g_modeDistance.IsDirty () ||
      g_modeTime.IsDirty () ||
      g_rectangle.IsDirty ())
    {
      parameters = Create<RandomWalk2dMobilityModelParameters> ();
    }
  return parameters;
}

RandomWalk2dMobilityModel::RandomWalk2dMobilityModel ()
  : m_parameters (RandomWalk2dMobilityModelParameters::GetCurrent ())
{
  SetInterfaceId (RandomWalk2dMobilityModel::iid);
  m_event = Simulator::ScheduleNow (&RandomWalk2dMobilityModel::Start, this);
}

void
RandomWalk2dMobilityModel::Start (void)
{
  double speed = m_parameters->m_speed->GetValue ();
  double direction = m_parameters->m_direction->GetValue ();
  Speed vector (std::cos (direction) * speed,
                std::sin (direction) * speed,
                0.0);
  m_helper.Reset (vector);

  Time delayLeft;
  if (m_parameters->m_mode == RandomWalk2dMobilityModelParameters::MODE_TIME)
    {
      delayLeft = m_parameters->m_modeTime;
    }
  else
    {
      delayLeft = Seconds (m_parameters->m_modeDistance / speed); 
    }
  DoWalk (delayLeft);
}

void
RandomWalk2dMobilityModel::DoWalk (Time delayLeft)
{
  Position position = m_helper.GetCurrentPosition ();
  Speed speed = m_helper.GetSpeed ();
  Position nextPosition = position;
  nextPosition.x += speed.dx * delayLeft.GetSeconds ();
  nextPosition.y += speed.dy * delayLeft.GetSeconds ();
  if (m_parameters->m_bounds.IsInside (nextPosition))
    {
      m_event = Simulator::Schedule (delayLeft, &RandomWalk2dMobilityModel::Start, this);
    }
  else
    {
      nextPosition = m_parameters->m_bounds.CalculateIntersection (position, speed);
      Time delay = Seconds ((nextPosition.x - position.x) / speed.dx);
      m_event = Simulator::Schedule (delay, &RandomWalk2dMobilityModel::Rebound, this,
                                     delayLeft - delay);      
    }  
  NotifyCourseChange ();
}

void
RandomWalk2dMobilityModel::Rebound (Time delayLeft)
{
  Position position = m_helper.GetCurrentPosition (m_parameters->m_bounds);
  Speed speed = m_helper.GetSpeed ();
  switch (m_parameters->m_bounds.GetClosestSide (position))
    {
    case Rectangle::RIGHT:
    case Rectangle::LEFT:
      speed.dx = - speed.dx;
      break;
    case Rectangle::TOP:
    case Rectangle::BOTTOM:
      speed.dy = - speed.dy;
      break;
    }
  m_helper.Reset (speed);
  DoWalk (delayLeft);
}

void
RandomWalk2dMobilityModel::DoDispose (void)
{
  m_parameters = 0;
  // chain up
  MobilityModel::DoDispose ();
}
Position
RandomWalk2dMobilityModel::DoGetPosition (void) const
{
  return m_helper.GetCurrentPosition (m_parameters->m_bounds);
}
void
RandomWalk2dMobilityModel::DoSetPosition (const Position &position)
{
  NS_ASSERT (m_parameters->m_bounds.IsInside (position));
  m_helper.InitializePosition (position);
  Simulator::Remove (m_event);
  m_event = Simulator::ScheduleNow (&RandomWalk2dMobilityModel::Start, this);
}
Speed 
RandomWalk2dMobilityModel::DoGetSpeed (void) const
{
  return m_helper.GetSpeed ();
}



} // namespace ns3
