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
#include "ns3/time-default-value.h"
#include "ns3/simulator.h"
#include <algorithm>
#include <cmath>
#include "random-mobility-model.h"

namespace ns3 {

const double RandomMobilityModel::PI = 3.1415;
const InterfaceId RandomMobilityModel::iid = 
  MakeInterfaceId ("RandomMobilityModel", MobilityModel::iid);
const ClassId RandomMobilityModel::cid = 
  MakeClassId<RandomMobilityModel,double,double> ("RandomMobilityModel",
                                                  RandomMobilityModel::iid);


static RandomVariableDefaultValue 
  g_speedVariable ("RandomMobilityModelSpeed",
		   "A random variable to control the speed of a random mobility model.",
		   "Uniform:1:2");

static RectangleDefaultValue
  g_rectangle ("RandomMobilityModelArea",
	       "The bounding area for the Random model.",
	       -100, 100, -100, 100);

static IntegerDefaultValue<double>
  g_walkDistance ("RandomWalkMaxDistance",
                  "The maximum distance (meters) an object moves before changing speed/direction for a "
                  "random walk model",
                  2.0);

static IntegerDefaultValue<double>
  g_waypointDistance ("RandomWaypointMaxDistance",
                      "The maximum distance (meters) an object moves before changing speed/direction for a "
                      "random waypoint model",
                      2.0);

static TimeDefaultValue
  g_walkDelay ("RandomWalkMaxDelay",
               "The maximum delay an object moves before changing speed/direction for "
               "a random walk model.",
               Seconds (1.0));

static TimeDefaultValue
  g_waypointDelay ("RandomWaypointMaxDelay",
                   "The maximum delay an object moves before changing speed/direction for "
                   "a random waypoint model.",
                   Seconds (1.0));


static RandomVariableDefaultValue
  g_directionPauseVariable ("RandomDirectionMobilityModelPause",
                            "A random variable to control the duration of the "
                            "pause of a random direction mobility model.",
                            "Uniform:2:4");

static RandomVariableDefaultValue
  g_waypointPauseVariable ("RandomWaypointMobilityModelPause",
                           "A random variable to control the duration of the "
                           "pause of a random waypoint mobility model.",
                           "Uniform:2:4");

static EnumDefaultValue<enum RandomMobilityModelParameters::Type>
  g_typeVariable ("RandomMobilityModelType",
                  "The type of mobility model to use.",
                  RandomMobilityModelParameters::WALK_DISTANCE, "WalkDistance",
                  RandomMobilityModelParameters::WALK_DELAY, "WalkDelay",
                  RandomMobilityModelParameters::WAYPOINT_DISTANCE, "WaypointDistance",
                  RandomMobilityModelParameters::WAYPOINT_DELAY, "WaypointDelay",
                  RandomMobilityModelParameters::DIRECTION, "Direction",
                  RandomMobilityModelParameters::BOUNDLESS, "Boundless",
                  0, (void*)0);


RandomMobilityModelParameters::RandomMobilityModelParameters ()
  : m_type (g_typeVariable.GetValue ()),
    m_speedVariable (g_speedVariable.GetCopy ())
{
  m_bounds.xMin = g_rectangle.GetMinX ();
  m_bounds.xMax = g_rectangle.GetMaxX ();
  m_bounds.yMin = g_rectangle.GetMinY ();
  m_bounds.yMax = g_rectangle.GetMaxY ();
  Initialize ();
}
RandomMobilityModelParameters::RandomMobilityModelParameters (double xMin, double xMax, double yMin, double yMax)
  : m_type (g_typeVariable.GetValue ()),
    m_speedVariable (g_speedVariable.GetCopy ())
{
  m_bounds.xMin = xMin;
  m_bounds.xMax = xMax;
  m_bounds.yMin = yMin;
  m_bounds.yMax = yMax;
  Initialize ();
}
RandomMobilityModelParameters::RandomMobilityModelParameters (double xMin, double xMax, double yMin, double yMax,
                                                              const RandomVariable &speedVariable)
  : m_type (g_typeVariable.GetValue ()),
    m_speedVariable (speedVariable.Copy ())
{
  m_bounds.xMin = xMin;
  m_bounds.xMax = xMax;
  m_bounds.yMin = yMin;
  m_bounds.yMax = yMax;
  Initialize ();
}

void
RandomMobilityModelParameters::Initialize (void)
{
  switch (m_type)
    {
    case RandomMobilityModelParameters::WAYPOINT_DISTANCE:
      m_maxDistance = g_waypointDistance.GetValue ();
      m_pauseVariable = g_waypointPauseVariable.GetCopy ();
      break;
    case RandomMobilityModelParameters::WAYPOINT_DELAY:
      m_maxDelay = g_waypointDelay.GetValue ();
      m_pauseVariable = g_waypointPauseVariable.GetCopy ();
      break;
    case RandomMobilityModelParameters::WALK_DISTANCE:
      m_maxDistance = g_walkDistance.GetValue ();
      break;
    case RandomMobilityModelParameters::WALK_DELAY:
      m_maxDelay = g_walkDelay.GetValue ();
      break;
    case RandomMobilityModelParameters::DIRECTION:
      m_pauseVariable = g_directionPauseVariable.GetCopy ();
      break;
    case RandomMobilityModelParameters::BOUNDLESS:
      break;
    }
}

RandomMobilityModelParameters::~RandomMobilityModelParameters ()
{
  delete m_speedVariable;
  delete m_pauseVariable;
  m_speedVariable = 0;
  m_pauseVariable = 0;
}

void 
RandomMobilityModelParameters::SetSpeed (const RandomVariable &speedVariable)
{
  delete m_speedVariable;
  m_speedVariable = speedVariable.Copy ();
}
void 
RandomMobilityModelParameters::SetWalkModelDistance (double maxDistance)
{
  m_type = RandomMobilityModelParameters::WALK_DISTANCE;
  m_maxDistance = maxDistance;
}
void 
RandomMobilityModelParameters::SetWalkModelDelay (Time maxDelay)
{
  m_type = RandomMobilityModelParameters::WALK_DELAY;
  m_maxDelay = maxDelay;
}
void 
RandomMobilityModelParameters::SetWaypointModelDistance (const RandomVariable &pauseVariable, 
                                                         double maxDistance)
{
  m_type = RandomMobilityModelParameters::WAYPOINT_DISTANCE;
  delete m_pauseVariable;
  m_pauseVariable = pauseVariable.Copy ();
  m_maxDistance = maxDistance;
}
void 
RandomMobilityModelParameters::SetWaypointModelDelay (const RandomVariable &pauseVariable, Time maxDelay)
{
  m_type = RandomMobilityModelParameters::WAYPOINT_DELAY;
  delete m_pauseVariable;
  m_pauseVariable = pauseVariable.Copy ();
  m_maxDelay = maxDelay;
}

void 
RandomMobilityModelParameters::SetDirectionModel (const RandomVariable &pauseVariable)
{
  m_type = RandomMobilityModelParameters::DIRECTION;
  delete m_pauseVariable;
  m_pauseVariable = pauseVariable.Copy ();  
}
void 
RandomMobilityModelParameters::SetBoundlessModel (void)
{
  m_type = RandomMobilityModelParameters::BOUNDLESS;
}

void 
RandomMobilityModelParameters::SetBounds (double xMin, double xMax, double yMin, double yMax)
{
  m_bounds.xMin = xMin;
  m_bounds.yMin = yMin;
  m_bounds.xMax = xMax;
  m_bounds.yMax = yMax;
}

Ptr<RandomMobilityModelParameters> 
RandomMobilityModel::GetDefaultParameters (void)
{
  static Ptr<RandomMobilityModelParameters> parameters = Create<RandomMobilityModelParameters> ();
  if (g_rectangle.IsDirty () ||
      g_typeVariable.IsDirty () ||
      g_speedVariable.IsDirty () ||
      g_waypointPauseVariable.IsDirty () ||
      g_directionPauseVariable.IsDirty () ||
      g_walkDistance.IsDirty () ||
      g_waypointDistance.IsDirty () ||
      g_walkDelay.IsDirty () ||
      g_waypointDelay.IsDirty ())
    {
      parameters = Create<RandomMobilityModelParameters> ();
      g_rectangle.ClearDirtyFlag ();
      g_typeVariable.ClearDirtyFlag ();
      g_speedVariable.ClearDirtyFlag ();
      g_waypointPauseVariable.ClearDirtyFlag ();
      g_directionPauseVariable.ClearDirtyFlag ();
      g_walkDelay.ClearDirtyFlag ();
      g_waypointDelay.ClearDirtyFlag ();
      g_walkDistance.ClearDirtyFlag ();
      g_waypointDistance.ClearDirtyFlag ();
    }
  return parameters;
}


RandomMobilityModel::RandomMobilityModel ()
  : m_parameters (GetDefaultParameters ())
{
  SetInterfaceId (RandomMobilityModel::iid);
  InitializeDirectionAndSpeed ();
}
RandomMobilityModel::RandomMobilityModel (double x, double y)
  : m_parameters (GetDefaultParameters ())
{
  SetInterfaceId (RandomMobilityModel::iid);
  InitializeDirectionAndSpeed ();
}
RandomMobilityModel::RandomMobilityModel (Ptr<RandomMobilityModelParameters> parameters)
  : m_parameters (parameters)
{
  SetInterfaceId (RandomMobilityModel::iid);
  InitializeDirectionAndSpeed ();
}
RandomMobilityModel::RandomMobilityModel (Ptr<RandomMobilityModelParameters> parameters,
                                          double x, double y)
  : m_parameters (parameters)
{
  SetInterfaceId (RandomMobilityModel::iid);
  InitializeDirectionAndSpeed ();
}
void 
RandomMobilityModel::DoDispose (void)
{
  m_parameters = 0;
  // chain up.
  MobilityModel::DoDispose ();
}
void
RandomMobilityModel::InitializeDirectionAndSpeed (void)
{
  double direction = UniformVariable::GetSingleValue (0, 2 * PI);
  SetDirectionAndSpeed (direction);
}
void
RandomMobilityModel::SetDirectionAndSpeed (double direction)
{
  double speed = m_parameters->m_speedVariable->GetValue ();
  Time pause;
  Time delay;
  switch (m_parameters->m_type)
    {
    case RandomMobilityModelParameters::WALK_DISTANCE:
      {
        pause = MicroSeconds (0);
        Time delay = m_helper.Reset (m_parameters->m_bounds,
                                     direction, speed,
                                     m_parameters->m_maxDistance,
                                     pause);
      } break;
    case RandomMobilityModelParameters::WALK_DELAY:
      {
        pause = MicroSeconds (0);
        Time delay = m_helper.Reset (m_parameters->m_bounds,
                                     direction, speed,
                                     m_parameters->m_maxDelay,
                                     pause);
      } break;
    case RandomMobilityModelParameters::WAYPOINT_DISTANCE:
      {
        pause = Seconds (m_parameters->m_pauseVariable->GetValue ());
        Time delay = m_helper.Reset (m_parameters->m_bounds,
                                     direction, speed,
                                     m_parameters->m_maxDistance,
                                     pause);
      } break;
      break;
    case RandomMobilityModelParameters::WAYPOINT_DELAY:
      {
        pause = Seconds (m_parameters->m_pauseVariable->GetValue ());
        Time delay = m_helper.Reset (m_parameters->m_bounds,
                                     direction, speed,
                                     m_parameters->m_maxDelay,
                                     pause);
      } break;
    case RandomMobilityModelParameters::DIRECTION:
      {
        pause = Seconds (m_parameters->m_pauseVariable->GetValue ());
        Time delay = m_helper.Reset (m_parameters->m_bounds,
                                     direction, speed,
                                     pause);
      } break;
    case RandomMobilityModelParameters::BOUNDLESS:
      break;
    }
  m_event = Simulator::Schedule (delay + pause,
                                 &RandomMobilityModel::ResetDirectionAndSpeed, this);  
}
void
RandomMobilityModel::ResetDirectionAndSpeed (void)
{
  double direction;
  switch (m_parameters->m_type)
    {
    default:
      //XXX
      break;
    case RandomMobilityModelParameters::WALK_DISTANCE:
      {
        switch (m_helper.GetSide (m_parameters->m_bounds))
          {
          case MobilityModelHelper::RIGHT:
            
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
      } break;
    case RandomMobilityModelParameters::DIRECTION:
      {
        direction = UniformVariable::GetSingleValue (0, PI);
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
      } break;
    }
  SetDirectionAndSpeed (direction);
  NotifyCourseChange ();
}
Position
RandomMobilityModel::DoGet (void) const
{
  Position2D position = m_helper.GetCurrentPosition (m_parameters->m_bounds);
  return Position (position.x, position.y, 0.0);
}
void
RandomMobilityModel::DoSet (const Position &position)
{
  Position2D pos (position.x, position.y);
  m_helper.InitializePosition (pos);
  Simulator::Remove (m_event);
  InitializeDirectionAndSpeed ();
  NotifyCourseChange ();
}



} // namespace ns3
