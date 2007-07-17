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
#include "random-direction-mobility-model.h"

namespace ns3 {

const double RandomDirectionMobilityModel::PI = 3.1415;
const InterfaceId RandomDirectionMobilityModel::iid = 
  MakeInterfaceId ("RandomDirectionMobilityModel", MobilityModel::iid);
const ClassId RandomDirectionMobilityModel::cid = 
  MakeClassId<RandomDirectionMobilityModel,double,double> ("RandomDirectionMobilityModel",
						      RandomDirectionMobilityModel::iid);


static RandomVariableDefaultValue 
  g_speedVariable ("RandomDirectionSpeed",
		   "A random variable to control the speed of a RandomDirection mobility model.",
		   "Uniform:1:2");

static RandomVariableDefaultValue
  g_pauseVariable ("RandomDirectionPause",
		   "A random variable to control the duration of of the pause of a RandomDiretion mobility model.",
		   "Constant:2");

static RectangleDefaultValue
  g_rectangle ("RandomDirectionArea",
	       "The bounding area for the RandomDirection model.",
	       -100, 100, -100, 100);


RandomDirectionParameters::RandomDirectionParameters ()
  : m_speedVariable (g_speedVariable.GetCopy ()),
    m_pauseVariable (g_pauseVariable.GetCopy ())
{
  m_bounds.xMin = g_rectangle.GetMinX ();
  m_bounds.xMax = g_rectangle.GetMaxX ();
  m_bounds.yMin = g_rectangle.GetMinY ();
  m_bounds.yMax = g_rectangle.GetMaxY ();
}
RandomDirectionParameters::RandomDirectionParameters (double xMin, double xMax, double yMin, double yMax)
  : m_speedVariable (g_speedVariable.GetCopy ()),
    m_pauseVariable (g_pauseVariable.GetCopy ())
{
  m_bounds.xMin = xMin;
  m_bounds.xMax = xMax;
  m_bounds.yMin = yMin;
  m_bounds.yMax = yMax;
}
RandomDirectionParameters::RandomDirectionParameters (double xMin, double xMax, double yMin, double yMax,
						      const RandomVariable &speedVariable,
						      const RandomVariable &pauseVariable)
  : m_speedVariable (speedVariable.Copy ()),
    m_pauseVariable (pauseVariable.Copy ())
{
  m_bounds.xMin = xMin;
  m_bounds.xMax = xMax;
  m_bounds.yMin = yMin;
  m_bounds.yMax = yMax;
}

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
RandomDirectionParameters::SetBounds (double xMin, double xMax, double yMin, double yMax)
{
  m_bounds.xMin = xMin;
  m_bounds.yMin = yMin;
  m_bounds.xMax = xMax;
  m_bounds.yMax = yMax;
}

Ptr<RandomDirectionParameters> 
RandomDirectionMobilityModel::GetDefaultParameters (void)
{
  static Ptr<RandomDirectionParameters> parameters = Create<RandomDirectionParameters> ();
  if (parameters->m_bounds.xMin != g_rectangle.GetMinX () ||
      parameters->m_bounds.yMin != g_rectangle.GetMinY () ||
      parameters->m_bounds.xMax != g_rectangle.GetMaxX () ||
      parameters->m_bounds.yMax != g_rectangle.GetMaxY () ||
      g_speedVariable.IsDirty () ||
      g_pauseVariable.IsDirty ())
    {
      parameters = Create<RandomDirectionParameters> ();
      g_speedVariable.ClearDirtyFlag ();
      g_pauseVariable.ClearDirtyFlag ();
    }
  return parameters;
}


RandomDirectionMobilityModel::RandomDirectionMobilityModel ()
  : m_parameters (GetDefaultParameters ())
{
  SetInterfaceId (RandomDirectionMobilityModel::iid);
  InitializeDirectionAndSpeed ();
}
RandomDirectionMobilityModel::RandomDirectionMobilityModel (double x, double y)
  : m_parameters (GetDefaultParameters ())
{
  SetInterfaceId (RandomDirectionMobilityModel::iid);
  InitializeDirectionAndSpeed ();
}
RandomDirectionMobilityModel::RandomDirectionMobilityModel (Ptr<RandomDirectionParameters> parameters)
  : m_parameters (parameters)
{
  SetInterfaceId (RandomDirectionMobilityModel::iid);
  InitializeDirectionAndSpeed ();
}
RandomDirectionMobilityModel::RandomDirectionMobilityModel (Ptr<RandomDirectionParameters> parameters,
                                                            double x, double y)
  : m_parameters (parameters)
{
  SetInterfaceId (RandomDirectionMobilityModel::iid);
  InitializeDirectionAndSpeed ();
}
void 
RandomDirectionMobilityModel::DoDispose (void)
{
  m_parameters = 0;
  // chain up.
  MobilityModel::DoDispose ();
}
void
RandomDirectionMobilityModel::InitializeDirectionAndSpeed (void)
{
  double direction = UniformVariable::GetSingleValue (0, 2 * PI);
  SetDirectionAndSpeed (direction);
}
void
RandomDirectionMobilityModel::SetDirectionAndSpeed (double direction)
{
  double speed = m_parameters->m_speedVariable->GetValue ();
  Time pause = Seconds (m_parameters->m_pauseVariable->GetValue ());
  Time delay = m_helper.Reset (m_parameters->m_bounds,
                               direction, speed,
                               pause);
  m_event = Simulator::Schedule (delay + pause,
				 &RandomDirectionMobilityModel::ResetDirectionAndSpeed, this);
}
void
RandomDirectionMobilityModel::ResetDirectionAndSpeed (void)
{
  double direction = UniformVariable::GetSingleValue (0, PI);
  
  switch (m_helper.GetSide (m_parameters->m_bounds))
    {
    case MobilityModelHelper::RIGHT:
      direction += PI / 2;
      break;
    case MobilityModelHelper::LEFT:
      direction += - PI / 2;
      break;
    case MobilityModelHelper::TOP:
      direction += PI;
      break;
    case MobilityModelHelper::BOTTOM:
      direction += 0.0;
      break;
    case MobilityModelHelper::NONE:
      NS_ASSERT (false);
      break;
    }
  SetDirectionAndSpeed (direction);
  NotifyCourseChange ();
}
Position
RandomDirectionMobilityModel::DoGet (void) const
{
  Position2D position = m_helper.GetCurrentPosition (m_parameters->m_bounds);
  return Position (position.x, position.y, 0.0);
}
void
RandomDirectionMobilityModel::DoSet (const Position &position)
{
  Position2D pos (position.x, position.y);
  m_helper.InitializePosition (pos);
  Simulator::Remove (m_event);
  InitializeDirectionAndSpeed ();
  NotifyCourseChange ();
}



} // namespace ns3
