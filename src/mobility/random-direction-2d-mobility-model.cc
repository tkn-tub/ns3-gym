/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "ns3/random-variable-default-value.h"
#include "ns3/rectangle-default-value.h"
#include "ns3/simulator.h"
#include <algorithm>
#include <cmath>
#include "random-direction-2d-mobility-model.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("RandomDirection2dMobilityModel");

namespace ns3 {

const double RandomDirection2dMobilityModel::PI = 3.14159265358979323846;
const ClassId RandomDirection2dMobilityModel::cid = 
  MakeClassId<RandomDirection2dMobilityModel> ("RandomDirection2dMobilityModel",
                                               MobilityModel::iid);


static RandomVariableDefaultValue 
  g_speedVariable ("RandomDirection2dSpeed",
		   "A random variable to control the speed of a RandomDirection2d mobility model.",
		   "Uniform:1:2");

static RandomVariableDefaultValue
  g_pauseVariable ("RandomDirection2dPause",
		   "A random variable to control the duration "
                   "of the pause of a RandomDiretion mobility model.",
		   "Constant:2");

static RectangleDefaultValue
  g_bounds ("RandomDirection2dArea",
	       "The bounding area for the RandomDirection2d model.",
	       -100, 100, -100, 100);


RandomDirection2dMobilityModelParameters::RandomDirection2dMobilityModelParameters ()
  : m_bounds (g_bounds.GetValue ()),
    m_speedVariable (g_speedVariable.GetCopy ()),
    m_pauseVariable (g_pauseVariable.GetCopy ())
    
{}
RandomDirection2dMobilityModelParameters::RandomDirection2dMobilityModelParameters 
(const Rectangle &bounds,
 const RandomVariable &speedVariable,
 const RandomVariable &pauseVariable)
  : m_bounds (bounds),
    m_speedVariable (speedVariable.Copy ()),
    m_pauseVariable (pauseVariable.Copy ())
{}

RandomDirection2dMobilityModelParameters::~RandomDirection2dMobilityModelParameters ()
{
  delete m_speedVariable;
  delete m_pauseVariable;
  m_speedVariable = 0;
  m_pauseVariable = 0;
}

void 
RandomDirection2dMobilityModelParameters::SetSpeed (const RandomVariable &speedVariable)
{
  delete m_speedVariable;
  m_speedVariable = speedVariable.Copy ();
}
void 
RandomDirection2dMobilityModelParameters::SetPause (const RandomVariable &pauseVariable)
{
  delete m_pauseVariable;
  m_pauseVariable = pauseVariable.Copy ();
}
void 
RandomDirection2dMobilityModelParameters::SetBounds (const Rectangle &bounds)
{
  m_bounds = bounds;
}

Ptr<RandomDirection2dMobilityModelParameters> 
RandomDirection2dMobilityModelParameters::GetCurrent (void)
{
  static Ptr<RandomDirection2dMobilityModelParameters> parameters = 0;
  if (parameters == 0 ||
      g_bounds.IsDirty () ||
      g_speedVariable.IsDirty () ||
      g_pauseVariable.IsDirty ())
    {
      parameters = Create<RandomDirection2dMobilityModelParameters> ();
      g_bounds.ClearDirtyFlag ();
      g_speedVariable.ClearDirtyFlag ();
      g_pauseVariable.ClearDirtyFlag ();
    }
  return parameters;
}


RandomDirection2dMobilityModel::RandomDirection2dMobilityModel ()
  : m_parameters (RandomDirection2dMobilityModelParameters::GetCurrent ())
{
  SetInterfaceId (RandomDirection2dMobilityModel::iid);
  m_event = Simulator::ScheduleNow (&RandomDirection2dMobilityModel::Start, this);
}
RandomDirection2dMobilityModel::RandomDirection2dMobilityModel 
(Ptr<RandomDirection2dMobilityModelParameters> parameters)
  : m_parameters (parameters)
{
  SetInterfaceId (RandomDirection2dMobilityModel::iid);
  m_event = Simulator::ScheduleNow (&RandomDirection2dMobilityModel::Start, this);
}
void 
RandomDirection2dMobilityModel::DoDispose (void)
{
  m_parameters = 0;
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
RandomDirection2dMobilityModel::SetDirectionAndSpeed (double direction)
{
  NS_LOG_FUNCTION;
  double speed = m_parameters->m_speedVariable->GetValue ();
  const Speed vector (std::cos (direction) * speed,
                      std::sin (direction) * speed,
                      0.0);
  Time pause = Seconds (m_parameters->m_pauseVariable->GetValue ());
  Position position = m_helper.GetCurrentPosition (m_parameters->m_bounds);
  m_helper.Reset (vector, pause);
  Position next = m_parameters->m_bounds.CalculateIntersection (position, vector);
  Time delay = Seconds (CalculateDistance (position, next) / speed);
  m_event = Simulator::Schedule (delay + pause,
				 &RandomDirection2dMobilityModel::ResetDirectionAndSpeed, this);
  NotifyCourseChange ();
}
void
RandomDirection2dMobilityModel::ResetDirectionAndSpeed (void)
{
  double direction = UniformVariable::GetSingleValue (0, PI);
  
  Position position = m_helper.GetCurrentPosition (m_parameters->m_bounds);
  switch (m_parameters->m_bounds.GetClosestSide (position))
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
Position
RandomDirection2dMobilityModel::DoGet (void) const
{
  return m_helper.GetCurrentPosition (m_parameters->m_bounds);
}
void
RandomDirection2dMobilityModel::DoSet (const Position &position)
{
  m_helper.InitializePosition (position);
  Simulator::Remove (m_event);
  m_event = Simulator::ScheduleNow (&RandomDirection2dMobilityModel::Start, this);
}
Speed 
RandomDirection2dMobilityModel::DoGetSpeed (void) const
{
  return m_helper.GetSpeed ();
}



} // namespace ns3
