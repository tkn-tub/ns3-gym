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

namespace ns3 {

const double RandomDirectionMobilityModel::PI = 3.1415;
const InterfaceId RandomDirectionMobilityModel::iid = 
  MakeInterfaceId ("RandomDirectionMobilityModel", MobilityModel::iid);
const ClassId RandomDirectionMobilityModel::cid = 
  MakeClassId<RandomDirectionMobilityModel> ("RandomDirectionMobilityModel",
                                             RandomDirectionMobilityModel::iid);


static RandomVariableDefaultValue 
  g_speedVariable ("RandomDirectionSpeed",
		   "A random variable to control the speed of a RandomDirection mobility model.",
		   "Uniform:1:2");

static RandomVariableDefaultValue
  g_pauseVariable ("RandomDirectionPause",
		   "A random variable to control the duration "
                   "of the pause of a RandomDiretion mobility model.",
		   "Constant:2");

static RectangleDefaultValue
  g_bounds ("RandomDirectionArea",
	       "The bounding area for the RandomDirection model.",
	       -100, 100, -100, 100);


RandomDirectionParameters::RandomDirectionParameters ()
  : m_bounds (g_bounds.GetValue ()),
    m_speedVariable (g_speedVariable.GetCopy ()),
    m_pauseVariable (g_pauseVariable.GetCopy ())
    
{}
RandomDirectionParameters::RandomDirectionParameters (const Rectangle &bounds,
						      const RandomVariable &speedVariable,
						      const RandomVariable &pauseVariable)
  : m_bounds (bounds),
    m_speedVariable (speedVariable.Copy ()),
    m_pauseVariable (pauseVariable.Copy ())
{}

RandomDirectionParameters::~RandomDirectionParameters ()
{
  delete m_speedVariable;
  delete m_pauseVariable;
  m_speedVariable = 0;
  m_pauseVariable = 0;
}

void 
RandomDirectionParameters::SetSpeed (const RandomVariable &speedVariable)
{
  delete m_speedVariable;
  m_speedVariable = speedVariable.Copy ();
}
void 
RandomDirectionParameters::SetPause (const RandomVariable &pauseVariable)
{
  delete m_pauseVariable;
  m_pauseVariable = pauseVariable.Copy ();
}
void 
RandomDirectionParameters::SetBounds (const Rectangle &bounds)
{
  m_bounds = bounds;
}

Ptr<RandomDirectionParameters> 
RandomDirectionParameters::GetCurrent (void)
{
  static Ptr<RandomDirectionParameters> parameters = 0;
  if (parameters == 0 ||
      g_bounds.IsDirty () ||
      g_speedVariable.IsDirty () ||
      g_pauseVariable.IsDirty ())
    {
      parameters = Create<RandomDirectionParameters> ();
      g_bounds.ClearDirtyFlag ();
      g_speedVariable.ClearDirtyFlag ();
      g_pauseVariable.ClearDirtyFlag ();
    }
  return parameters;
}


RandomDirectionMobilityModel::RandomDirectionMobilityModel ()
  : m_parameters (RandomDirectionParameters::GetCurrent ())
{
  SetInterfaceId (RandomDirectionMobilityModel::iid);
  m_event = Simulator::ScheduleNow (&RandomDirectionMobilityModel::Start, this);
}
RandomDirectionMobilityModel::RandomDirectionMobilityModel (Ptr<RandomDirectionParameters> parameters)
  : m_parameters (parameters)
{
  SetInterfaceId (RandomDirectionMobilityModel::iid);
  m_event = Simulator::ScheduleNow (&RandomDirectionMobilityModel::Start, this);
}
void 
RandomDirectionMobilityModel::DoDispose (void)
{
  m_parameters = 0;
  // chain up.
  MobilityModel::DoDispose ();
}
void
RandomDirectionMobilityModel::Start (void)
{
  double direction = UniformVariable::GetSingleValue (0, 2 * PI);
  SetDirectionAndSpeed (direction);
}
void
RandomDirectionMobilityModel::SetDirectionAndSpeed (double direction)
{
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
				 &RandomDirectionMobilityModel::ResetDirectionAndSpeed, this);
  NotifyCourseChange ();
}
void
RandomDirectionMobilityModel::ResetDirectionAndSpeed (void)
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
RandomDirectionMobilityModel::DoGet (void) const
{
  return m_helper.GetCurrentPosition (m_parameters->m_bounds);
}
void
RandomDirectionMobilityModel::DoSet (const Position &position)
{
  m_helper.InitializePosition (position);
  Simulator::Remove (m_event);
  m_event = Simulator::ScheduleNow (&RandomDirectionMobilityModel::Start, this);
}
Speed 
RandomDirectionMobilityModel::DoGetSpeed (void) const
{
  return m_helper.GetSpeed ();
}



} // namespace ns3
