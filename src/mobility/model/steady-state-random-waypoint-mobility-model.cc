/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Denis Fakhriev <fakhriev@iitp.ru>
 */
#include <cmath>
#include "ns3/simulator.h"
#include "ns3/double.h"
#include "steady-state-random-waypoint-mobility-model.h"
#include "ns3/test.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SteadyStateRandomWaypointMobilityModel);

TypeId
SteadyStateRandomWaypointMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SteadyStateRandomWaypointMobilityModel")
    .SetParent<MobilityModel> ()
    .SetGroupName ("Mobility")
    .AddConstructor<SteadyStateRandomWaypointMobilityModel> ()
    .AddAttribute ("MinSpeed",
                   "Minimum speed value, [m/s]",
                   DoubleValue (0.3),
                   MakeDoubleAccessor (&SteadyStateRandomWaypointMobilityModel::m_minSpeed),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxSpeed",
                   "Maximum speed value, [m/s]",
                   DoubleValue (0.7),
                   MakeDoubleAccessor (&SteadyStateRandomWaypointMobilityModel::m_maxSpeed),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MinPause",
                   "Minimum pause value, [s]",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&SteadyStateRandomWaypointMobilityModel::m_minPause),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxPause",
                   "Maximum pause value, [s]",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&SteadyStateRandomWaypointMobilityModel::m_maxPause),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MinX",
                   "Minimum X value of traveling region, [m]",
                   DoubleValue (1),
                   MakeDoubleAccessor (&SteadyStateRandomWaypointMobilityModel::m_minX),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxX",
                   "Maximum X value of traveling region, [m]",
                   DoubleValue (1),
                   MakeDoubleAccessor (&SteadyStateRandomWaypointMobilityModel::m_maxX),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MinY",
                   "Minimum Y value of traveling region, [m]",
                   DoubleValue (1),
                   MakeDoubleAccessor (&SteadyStateRandomWaypointMobilityModel::m_minY),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxY",
                   "Maximum Y value of traveling region, [m]",
                   DoubleValue (1),
                   MakeDoubleAccessor (&SteadyStateRandomWaypointMobilityModel::m_maxY),
                   MakeDoubleChecker<double> ());

  return tid;
}

SteadyStateRandomWaypointMobilityModel::SteadyStateRandomWaypointMobilityModel () :
  alreadyStarted (false)
{
}

void
SteadyStateRandomWaypointMobilityModel::DoStart (void)
{
  SteadyStateStart ();
  MobilityModel::DoStart ();
}

void
SteadyStateRandomWaypointMobilityModel::SteadyStateStart (void)
{
  alreadyStarted = true;
  NS_ASSERT (m_minSpeed >= 1e-6);
  NS_ASSERT (m_minSpeed <= m_maxSpeed);
  m_speed = UniformVariable (m_minSpeed, m_maxSpeed);
  NS_ASSERT (m_minX < m_maxX);
  NS_ASSERT (m_minY < m_maxY);
  m_position = CreateObject<RandomRectanglePositionAllocator> ();
  m_position->SetX (UniformVariable (m_minX, m_maxX));
  m_position->SetY (UniformVariable (m_minY, m_maxY));
  NS_ASSERT (m_minPause <= m_maxPause);
  m_pause = UniformVariable (m_minPause, m_maxPause);

  m_helper.Update ();
  m_helper.Pause ();

  // calculate the steady-state probability that a node is initially paused
  double expectedPauseTime = (m_minPause + m_maxPause)/2;
  double a = m_maxX - m_minX;
  double b = m_maxY - m_minY;
  double v0 = m_minSpeed;
  double v1 = m_maxSpeed;
  double log1 = b*b / a*std::log (std::sqrt ((a*a)/(b*b) + 1) + a/b);
  double log2 = a*a / b*std::log (std::sqrt ((b*b)/(a*a) + 1) + b/a);
  double expectedTravelTime = 1.0/6.0*(log1 + log2);
  expectedTravelTime += 1.0/15.0*((a*a*a)/(b*b) + (b*b*b)/(a*a)) -
    1.0/15.0*std::sqrt (a*a + b*b)*((a*a)/(b*b) + (b*b)/(a*a) - 3);
  if (v0 == v1)
    {
      expectedTravelTime /= v0;
    }
  else
    {
      expectedTravelTime *= std::log (v1/v0)/(v1 - v0);
    }
  double probabilityPaused = expectedPauseTime/(expectedPauseTime + expectedTravelTime);
  NS_ASSERT (probabilityPaused >= 0 && probabilityPaused <= 1);

  UniformVariable u_r;
  double u = u_r.GetValue (0, 1);
  if (u < probabilityPaused) // node initially paused
    {
      m_helper.SetPosition (m_position->GetNext ());
      u = u_r.GetValue (0, 1);
      Time pause;
      if (m_minPause != m_maxPause)
        {
          if (u < (2*m_minPause/(m_minPause + m_maxPause)))
            {
              pause = Seconds (u*(m_minPause + m_maxPause)/2);
            }
          else
            {
              // there is an error in equation 20 in the Tech. Report MCS-03-04
              // this error is corrected in the TMC 2004 paper and below
              pause = Seconds (m_maxPause - sqrt ((1 - u)*(m_maxPause*m_maxPause - m_minPause*m_minPause)));
            }
        }
      else // if pause is constant
        {
          pause = Seconds (u*expectedPauseTime);
        }
      NS_ASSERT (!m_event.IsRunning ());
      m_event = Simulator::Schedule (pause, &SteadyStateRandomWaypointMobilityModel::BeginWalk, this);
    }
  else // node initially moving
    {
      UniformVariable x1_r, y1_r, x2_r, y2_r;
      double x1, x2, y1, y2;
      double r = 0;
      double u1 = 1;
      while (u1 >= r)
        {
          x1 = x1_r.GetValue (0, a);
          y1 = y1_r.GetValue (0, b);
          x2 = x2_r.GetValue (0, a);
          y2 = y2_r.GetValue (0, b);
          u1 = u_r.GetValue (0, 1);
          r = std::sqrt (((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))/(a*a + b*b));
          NS_ASSERT (r <= 1);
        }
      double u2 = u_r.GetValue (0, 1);
      m_helper.SetPosition (Vector (m_minX + u2*x1 + (1 - u2)*x2, m_minY + u2*y1 + (1 - u2)*y2, 0));
      NS_ASSERT (!m_event.IsRunning ());
      m_event = Simulator::ScheduleNow (&SteadyStateRandomWaypointMobilityModel::SteadyStateBeginWalk, this, 
                                        Vector (m_minX + x2, m_minY + y2, 0));
    }
  NotifyCourseChange ();
}

void
SteadyStateRandomWaypointMobilityModel::SteadyStateBeginWalk (const Vector &destination)
{
  m_helper.Update ();
  Vector m_current = m_helper.GetCurrentPosition ();
  NS_ASSERT (m_minX <= m_current.x && m_current.x <= m_maxX);
  NS_ASSERT (m_minY <= m_current.y && m_current.y <= m_maxY);
  NS_ASSERT (m_minX <= destination.x && destination.x <= m_maxX);
  NS_ASSERT (m_minY <= destination.y && destination.y <= m_maxY);
  UniformVariable u_r;
  double u = u_r.GetValue (0, 1);
  double speed = std::pow (m_maxSpeed, u)/std::pow (m_minSpeed, u - 1);
  double dx = (destination.x - m_current.x);
  double dy = (destination.y - m_current.y);
  double dz = (destination.z - m_current.z);
  double k = speed / std::sqrt (dx*dx + dy*dy + dz*dz);

  m_helper.SetVelocity (Vector (k*dx, k*dy, k*dz));
  m_helper.Unpause ();
  Time travelDelay = Seconds (CalculateDistance (destination, m_current) / speed);
  m_event = Simulator::Schedule (travelDelay, 
                                 &SteadyStateRandomWaypointMobilityModel::Start, this);
  NotifyCourseChange ();
}

void
SteadyStateRandomWaypointMobilityModel::BeginWalk (void)
{
  m_helper.Update ();
  Vector m_current = m_helper.GetCurrentPosition ();
  NS_ASSERT (m_minX <= m_current.x && m_current.x <= m_maxX);
  NS_ASSERT (m_minY <= m_current.y && m_current.y <= m_maxY);
  Vector destination = m_position->GetNext ();
  double speed = m_speed.GetValue ();
  double dx = (destination.x - m_current.x);
  double dy = (destination.y - m_current.y);
  double dz = (destination.z - m_current.z);
  double k = speed / std::sqrt (dx*dx + dy*dy + dz*dz);

  m_helper.SetVelocity (Vector (k*dx, k*dy, k*dz));
  m_helper.Unpause ();
  Time travelDelay = Seconds (CalculateDistance (destination, m_current) / speed);
  m_event = Simulator::Schedule (travelDelay,
                                 &SteadyStateRandomWaypointMobilityModel::Start, this);
  NotifyCourseChange ();
}

void
SteadyStateRandomWaypointMobilityModel::Start (void)
{
  m_helper.Update ();
  m_helper.Pause ();
  Time pause = Seconds (m_pause.GetValue ());
  m_event = Simulator::Schedule (pause, &SteadyStateRandomWaypointMobilityModel::BeginWalk, this);
  NotifyCourseChange ();
}

Vector
SteadyStateRandomWaypointMobilityModel::DoGetPosition (void) const
{
  m_helper.Update ();
  return m_helper.GetCurrentPosition ();
}
void 
SteadyStateRandomWaypointMobilityModel::DoSetPosition (const Vector &position)
{
  if (alreadyStarted)
    {
      m_helper.SetPosition (position);
      Simulator::Remove (m_event);
      m_event = Simulator::ScheduleNow (&SteadyStateRandomWaypointMobilityModel::Start, this);
    }
}
Vector
SteadyStateRandomWaypointMobilityModel::DoGetVelocity (void) const
{
  return m_helper.GetVelocity ();
}

} // namespace ns3
