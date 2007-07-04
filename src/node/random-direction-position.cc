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
#include "random-direction-position.h"

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
  : m_xMin (g_rectangle.GetMinX ()),
    m_xMax (g_rectangle.GetMaxX ()),
    m_yMin (g_rectangle.GetMinY ()),
    m_yMax (g_rectangle.GetMaxY ()),
    m_speedVariable (g_speedVariable.GetCopy ()),
    m_pauseVariable (g_pauseVariable.GetCopy ())
{}
RandomDirectionParameters::RandomDirectionParameters (double xMin, double xMax, double yMin, double yMax)
  : m_xMin (xMin),
    m_xMax (xMax),
    m_yMin (yMin),
    m_yMax (yMax),
    m_speedVariable (g_speedVariable.GetCopy ()),
    m_pauseVariable (g_pauseVariable.GetCopy ())
{}
RandomDirectionParameters::RandomDirectionParameters (double xMin, double xMax, double yMin, double yMax,
						      const RandomVariable &speedVariable,
						      const RandomVariable &pauseVariable)
  : m_xMin (xMin),
    m_xMax (xMax),
    m_yMin (yMin),
    m_yMax (yMax),
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
RandomDirectionParameters::SetBounds (double xMin, double xMax, double yMin, double yMax)
{
  m_xMin = xMin;
  m_yMin = yMin;
  m_xMax = xMax;
  m_yMax = yMax;
}

Ptr<RandomDirectionParameters> 
RandomDirectionMobilityModel::GetDefaultParameters (void)
{
  static Ptr<RandomDirectionParameters> parameters = Create<RandomDirectionParameters> ();
  if (parameters->m_xMin != g_rectangle.GetMinX () ||
      parameters->m_yMin != g_rectangle.GetMinY () ||
      parameters->m_xMax != g_rectangle.GetMaxX () ||
      parameters->m_yMax != g_rectangle.GetMaxY () ||
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
  : m_parameters (GetDefaultParameters ()),
    m_x (0.0),
    m_y (0.0),
    m_dx (0.0),
    m_dy (0.0),
    m_prevTime (Simulator::Now ()),
    m_pauseStart (Simulator::Now ())
{
  SetInterfaceId (RandomDirectionMobilityModel::iid);
  InitializeDirectionAndSpeed ();
}
bool
RandomDirectionMobilityModel::CheckMobilityModel (void) const
{
  return 
    m_x <= m_parameters->m_xMax && 
    m_x >= m_parameters->m_xMin &&
    m_y <= m_parameters->m_yMax && 
    m_y >= m_parameters->m_yMin;
}

RandomDirectionMobilityModel::RandomDirectionMobilityModel (double x, double y)
  : m_parameters (GetDefaultParameters ()),
    m_x (x),
    m_y (y),
    m_dx (0.0),
    m_dy (0.0),
    m_prevTime (Simulator::Now ()),
    m_pauseStart (Simulator::Now ())
{
  SetInterfaceId (RandomDirectionMobilityModel::iid);
  NS_ASSERT (CheckMobilityModel ());
  InitializeDirectionAndSpeed ();
}
RandomDirectionMobilityModel::RandomDirectionMobilityModel (Ptr<RandomDirectionParameters> parameters)
  : m_parameters (parameters),
    m_x (0.0),
    m_y (0.0),
    m_dx (0.0),
    m_dy (0.0),
    m_prevTime (Simulator::Now ()),
    m_pauseStart (Simulator::Now ())
{
  SetInterfaceId (RandomDirectionMobilityModel::iid);
  InitializeDirectionAndSpeed ();
  NS_ASSERT (CheckMobilityModel ());
}
RandomDirectionMobilityModel::RandomDirectionMobilityModel (Ptr<RandomDirectionParameters> parameters,
						  double x, double y)
  : m_parameters (parameters),
    m_x (x),
    m_y (y),
    m_dx (0.0),
    m_dy (0.0),
    m_prevTime (Simulator::Now ()),
    m_pauseStart (Simulator::Now ())
{
  SetInterfaceId (RandomDirectionMobilityModel::iid);
  InitializeDirectionAndSpeed ();
  NS_ASSERT (CheckMobilityModel ());
}
void 
RandomDirectionMobilityModel::DoDispose (void)
{
  m_parameters = 0;
  // chain up.
  MobilityModel::DoDispose ();
}
enum RandomDirectionMobilityModel::Side
RandomDirectionMobilityModel::CalculateIntersection (double &x, double &y)
{
  double xMin = m_parameters->m_xMin;
  double xMax = m_parameters->m_xMax;
  double yMin = m_parameters->m_yMin;
  double yMax = m_parameters->m_yMax;
  double xMaxY = m_y + (xMax - m_x) / m_dx * m_dy;
  double xMinY = m_y + (xMin - m_x) / m_dx * m_dy;
  double yMaxX = m_x + (yMax - m_y) / m_dy * m_dx;
  double yMinX = m_x + (yMin - m_y) / m_dy * m_dx;
  bool xMaxOk = xMaxY <= yMax && xMaxY >= yMin;
  bool xMinOk = xMinY <= yMax && xMinY >= yMin;
  bool yMaxOk = yMaxX <= xMax && yMaxX >= xMin;
  bool yMinOk = yMinX <= xMax && yMinX >= xMin;
  if (xMaxOk && m_dx >= 0)
    {
      x = xMax;
      y = xMaxY;
      return RandomDirectionMobilityModel::RIGHT;
    }
  else if (xMinOk && m_dx <= 0)
    {
      x = xMin;
      y = xMinY;
      return RandomDirectionMobilityModel::LEFT;
    }
  else if (yMaxOk && m_dy >= 0)
    {
      x = yMaxX;
      y = yMax;
      return RandomDirectionMobilityModel::TOP;
    }
  else if (yMinOk && m_dy <= 0)
    {
      x = yMinX;
      y = yMin;
      return RandomDirectionMobilityModel::BOTTOM;
    }
  else
    {
      NS_ASSERT (false);
      // quiet compiler
      return RandomDirectionMobilityModel::RIGHT;
    }
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
  m_dx = std::cos (direction) * speed;
  m_dy = std::sin (direction) * speed;
  double x, y;
  m_side = CalculateIntersection (x, y);
  double deltaX = x - m_x;
  double deltaY = y - m_y;
  double distance = sqrt (deltaX * deltaX + deltaY * deltaY);
  double seconds = distance / speed;
  double pause = m_parameters->m_pauseVariable->GetValue ();
  m_pauseStart = Simulator::Now () + Seconds (seconds);
  m_prevTime = Simulator::Now ();
  m_event = Simulator::Schedule (Seconds (seconds + pause),
				 &RandomDirectionMobilityModel::ResetDirectionAndSpeed, this);
}
void
RandomDirectionMobilityModel::ResetDirectionAndSpeed (void)
{
  Update ();
  double direction = UniformVariable::GetSingleValue (0, PI);
  switch (m_side)
    {
    case RandomDirectionMobilityModel::RIGHT:
      direction += PI / 2;
      break;
    case RandomDirectionMobilityModel::LEFT:
      direction += - PI / 2;
      break;
    case RandomDirectionMobilityModel::TOP:
      direction += PI;
      break;
    case RandomDirectionMobilityModel::BOTTOM:
      direction += 0.0;
      break;
    }
  SetDirectionAndSpeed (direction);
  NotifyCourseChange ();
}
void
RandomDirectionMobilityModel::Update (void) const
{
  Time end = std::min (Simulator::Now (), m_pauseStart);
  if (m_prevTime >= end)
    {
      return;
    }
  Time deltaTime = end - m_prevTime;
  m_prevTime = Simulator::Now ();
  double deltaS = deltaTime.GetSeconds ();
  NS_ASSERT (CheckMobilityModel ());
  m_x += m_dx * deltaS;
  m_y += m_dy * deltaS;
  // round to closest boundaries.
  m_x = std::min (m_x, m_parameters->m_xMax);
  m_x = std::max (m_x, m_parameters->m_xMin);
  m_y = std::min (m_y, m_parameters->m_yMax);
  m_y = std::max (m_y, m_parameters->m_yMin);
  NS_ASSERT (CheckMobilityModel ());
}
void 
RandomDirectionMobilityModel::DoGet (double &x, double &y, double &z) const
{
  Update ();
  x = m_x;
  y = m_y;
  z = 0;
}
void
RandomDirectionMobilityModel::DoSet (double x, double y, double z)
{
  bool changed = false;
  if (m_x != x || m_y != y)
    {
      changed = true;
    }
  m_x = x;
  m_y = y;
  m_prevTime = Simulator::Now ();
  m_pauseStart = Simulator::Now ();
  Simulator::Remove (m_event);
  InitializeDirectionAndSpeed ();
  NotifyCourseChange ();
}



} // namespace ns3
