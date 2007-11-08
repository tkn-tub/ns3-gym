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
#include "ns3/component-manager.h"
#include "random-waypoint-mobility-model.h"
#include "random-position.h"

namespace ns3 {

static RandomVariableDefaultValue
g_speed ("RandomWaypointSpeed",
	 "A random variable used to pick the speed of a random waypoint model.",
	 "Uniform:0.3:0.7");

static RandomVariableDefaultValue
g_pause ("RandomWaypointPause",
	 "A random variable used to pick the pause of a random waypoint model.",
	 "Constant:2");

static ClassIdDefaultValue
g_position ("RandomWaypointPosition",
	    "A random position model used to pick the next waypoint position.",
	    RandomPosition::iid,
	    "RandomRectanglePosition");

const ClassId RandomWaypointMobilityModel::cid = 
  MakeClassId<RandomWaypointMobilityModel> ("RandomWaypointMobilityModel", MobilityModel::iid);

RandomWaypointMobilityModelParameters::RandomWaypointMobilityModelParameters ()
  : m_speed (g_speed.GetCopy ()),
    m_pause (g_pause.GetCopy ())
{
  m_position = ComponentManager::Create<RandomPosition> (g_position.GetValue (), 
							 RandomPosition::iid);
}
RandomWaypointMobilityModelParameters::RandomWaypointMobilityModelParameters (Ptr<RandomPosition> randomPosition,
									      const RandomVariable &speed,
									      const RandomVariable &pause)
  : m_speed (speed.Copy ()),
    m_pause (pause.Copy ()),
    m_position (randomPosition)
{}
void 
RandomWaypointMobilityModelParameters::SetWaypointPositionModel (Ptr<RandomPosition> randomPosition)
{
  m_position = randomPosition;
}
void 
RandomWaypointMobilityModelParameters::SetSpeed (const RandomVariable &speed)
{
  delete m_speed;
  m_speed = speed.Copy ();
}
void 
RandomWaypointMobilityModelParameters::SetPause (const RandomVariable &pause)
{
  delete m_pause;
  m_pause = pause.Copy ();
}
void 
RandomWaypointMobilityModelParameters::DoDispose (void)
{
  m_position = 0;
  delete m_pause;
  delete m_speed;
  m_pause = 0;
  m_speed = 0;  
}

Ptr<RandomWaypointMobilityModelParameters>
RandomWaypointMobilityModelParameters::GetCurrent (void)
{
  static Ptr<RandomWaypointMobilityModelParameters> parameters = 0;
  if (parameters == 0 ||
      g_position.IsDirty () ||
      g_pause.IsDirty () ||
      g_speed.IsDirty ())
    {
      parameters = Create<RandomWaypointMobilityModelParameters> ();
    }
  return parameters;
}

RandomWaypointMobilityModel::RandomWaypointMobilityModel ()
  : m_parameters (RandomWaypointMobilityModelParameters::GetCurrent ())
{
  Simulator::ScheduleNow (&RandomWaypointMobilityModel::Start, this);
}

RandomWaypointMobilityModel::RandomWaypointMobilityModel (Ptr<RandomWaypointMobilityModelParameters> parameters)
  : m_parameters (parameters)
{
  Simulator::ScheduleNow (&RandomWaypointMobilityModel::Start, this);
  NotifyCourseChange ();
}

void
RandomWaypointMobilityModel::BeginWalk (void)
{
  Vector m_current = m_helper.GetCurrentPosition ();
  Vector destination = m_parameters->m_position->Get ();
  double speed = m_parameters->m_speed->GetValue ();
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
  Time pause = Seconds (m_parameters->m_pause->GetValue ());
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
RandomWaypointMobilityModel::DoGetSpeed (void) const
{
  return m_helper.GetSpeed ();
}


} // namespace ns3
