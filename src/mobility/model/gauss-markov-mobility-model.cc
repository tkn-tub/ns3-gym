/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 Dan Broyles
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
 * Author: Dan Broyles <dbroyl01@ku.edu>
 */
#include <cmath>
#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "ns3/double.h"
#include "ns3/pointer.h"
#include "gauss-markov-mobility-model.h"
#include "position-allocator.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (GaussMarkovMobilityModel);

TypeId
GaussMarkovMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::GaussMarkovMobilityModel")
    .SetParent<MobilityModel> ()
    .SetGroupName ("Mobility")
    .AddConstructor<GaussMarkovMobilityModel> ()
    .AddAttribute ("Bounds",
                   "Bounds of the area to cruise.",
                   BoxValue (Box (-100.0, 100.0, -100.0, 100.0, 0.0, 100.0)),
                   MakeBoxAccessor (&GaussMarkovMobilityModel::m_bounds),
                   MakeBoxChecker ())
    .AddAttribute ("TimeStep",
                   "Change current direction and speed after moving for this time.",
                   TimeValue (Seconds (1.0)),
                   MakeTimeAccessor (&GaussMarkovMobilityModel::m_timeStep),
                   MakeTimeChecker ())
    .AddAttribute ("Alpha",
                   "A constant representing the tunable parameter in the Gauss-Markov model.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&GaussMarkovMobilityModel::m_alpha),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MeanVelocity",
                   "A random variable used to assign the average velocity.",
                   RandomVariableValue (UniformVariable (0.0, 1.0)),
                   MakeRandomVariableAccessor (&GaussMarkovMobilityModel::m_rndMeanVelocity),
                   MakeRandomVariableChecker ())
    .AddAttribute ("MeanDirection",
                   "A random variable used to assign the average direction.",
                   RandomVariableValue (UniformVariable (0.0, 6.283185307)),
                   MakeRandomVariableAccessor (&GaussMarkovMobilityModel::m_rndMeanDirection),
                   MakeRandomVariableChecker ())
    .AddAttribute ("MeanPitch",
                   "A random variable used to assign the average pitch.",
                   RandomVariableValue (ConstantVariable (0.0)),
                   MakeRandomVariableAccessor (&GaussMarkovMobilityModel::m_rndMeanPitch),
                   MakeRandomVariableChecker ())
    .AddAttribute ("NormalVelocity",
                   "A gaussian random variable used to calculate the next velocity value.",
                   RandomVariableValue (NormalVariable (0.0, 1.0, 10.0)), // Defaults to zero mean, and std dev = 1, and bound to +-10 of the mean
                   MakeRandomVariableAccessor (&GaussMarkovMobilityModel::m_normalVelocity),
                   MakeRandomVariableChecker ())
    .AddAttribute ("NormalDirection",
                   "A gaussian random variable used to calculate the next direction value.",
                   RandomVariableValue (NormalVariable (0.0, 1.0, 10.0)),
                   MakeRandomVariableAccessor (&GaussMarkovMobilityModel::m_normalDirection),
                   MakeRandomVariableChecker ())
    .AddAttribute ("NormalPitch",
                   "A gaussian random variable used to calculate the next pitch value.",
                   RandomVariableValue (NormalVariable (0.0, 1.0, 10.0)),
                   MakeRandomVariableAccessor (&GaussMarkovMobilityModel::m_normalPitch),
                   MakeRandomVariableChecker ());

  return tid;
}

GaussMarkovMobilityModel::GaussMarkovMobilityModel ()
{
  m_meanVelocity = 0.0;
  m_meanDirection = 0.0;
  m_meanPitch = 0.0;
  m_event = Simulator::ScheduleNow (&GaussMarkovMobilityModel::Start, this);
  m_helper.Unpause ();
}

void
GaussMarkovMobilityModel::Start (void)
{
  if (m_meanVelocity == 0.0)
    {
      //Initialize the mean velocity, direction, and pitch variables
      m_meanVelocity = m_rndMeanVelocity.GetValue ();
      m_meanDirection = m_rndMeanDirection.GetValue ();
      m_meanPitch = m_rndMeanPitch.GetValue ();
      double cosD = std::cos (m_meanDirection);
      double cosP = std::cos (m_meanPitch);
      double sinD = std::sin (m_meanDirection);
      double sinP = std::sin (m_meanPitch);
      //Initialize the starting velocity, direction, and pitch to be identical to the mean ones
      m_Velocity = m_meanVelocity;
      m_Direction = m_meanDirection;
      m_Pitch = m_meanPitch;
      //Set the velocity vector to give to the constant velocity helper
      m_helper.SetVelocity (Vector (m_Velocity*cosD*cosP, m_Velocity*sinD*cosP, m_Velocity*sinP));
    }
  m_helper.Update ();

  //Get the next values from the gaussian distributions for velocity, direction, and pitch
  double rv = m_normalVelocity.GetValue ();
  double rd = m_normalDirection.GetValue ();
  double rp = m_normalPitch.GetValue ();

  //Calculate the NEW velocity, direction, and pitch values using the Gauss-Markov formula:
  //newVal = alpha*oldVal + (1-alpha)*meanVal + sqrt(1-alpha^2)*rv
  //where rv is a random number from a normal (gaussian) distribution
  double one_minus_alpha = 1 - m_alpha;
  double sqrt_alpha = std::sqrt (1 - m_alpha*m_alpha);
  m_Velocity  = m_alpha * m_Velocity  + one_minus_alpha * m_meanVelocity  + sqrt_alpha * rv;
  m_Direction = m_alpha * m_Direction + one_minus_alpha * m_meanDirection + sqrt_alpha * rd;
  m_Pitch     = m_alpha * m_Pitch     + one_minus_alpha * m_meanPitch     + sqrt_alpha * rp;

  //Calculate the linear velocity vector to give to the constant velocity helper
  double cosDir = std::cos (m_Direction);
  double cosPit = std::cos (m_Pitch);
  double sinDir = std::sin (m_Direction);
  double sinPit = std::sin (m_Pitch);
  double vx = m_Velocity * cosDir * cosPit;
  double vy = m_Velocity * sinDir * cosPit;
  double vz = m_Velocity * sinPit;
  m_helper.SetVelocity (Vector (vx, vy, vz));

  m_helper.Unpause ();

  DoWalk (m_timeStep);
}

void
GaussMarkovMobilityModel::DoWalk (Time delayLeft)
{
  m_helper.UpdateWithBounds (m_bounds);
  Vector position = m_helper.GetCurrentPosition ();
  Vector speed = m_helper.GetVelocity ();
  Vector nextPosition = position;
  nextPosition.x += speed.x * delayLeft.GetSeconds ();
  nextPosition.y += speed.y * delayLeft.GetSeconds ();
  nextPosition.z += speed.z * delayLeft.GetSeconds ();
  if (delayLeft.GetSeconds () < 0.0) delayLeft = Seconds (1.0);

  // Make sure that the position by the next time step is still within the boundary.
  // If out of bounds, then alter the velocity vector and average direction to keep the position in bounds
  if (m_bounds.IsInside (nextPosition))
    {
      m_event = Simulator::Schedule (delayLeft, &GaussMarkovMobilityModel::Start, this);
    }
  else
    {
      if (nextPosition.x > m_bounds.xMax || nextPosition.x < m_bounds.xMin) 
        {
          speed.x = -speed.x;
          m_meanDirection = 3.14159265 - m_meanDirection;
        }

      if (nextPosition.y > m_bounds.yMax || nextPosition.y < m_bounds.yMin) 
        {
          speed.y = -speed.y;
          m_meanDirection = -m_meanDirection;
        }

      if (nextPosition.z > m_bounds.zMax || nextPosition.z < m_bounds.zMin) 
        {
          speed.z = -speed.z;
          m_meanPitch = -m_meanPitch;
        }

      m_Direction = m_meanDirection;
      m_Pitch = m_meanPitch;
      m_helper.SetVelocity (speed);
      m_helper.Unpause ();
      m_event = Simulator::Schedule (delayLeft, &GaussMarkovMobilityModel::Start, this);
    }
  NotifyCourseChange ();
}

void
GaussMarkovMobilityModel::DoDispose (void)
{
  // chain up
  MobilityModel::DoDispose ();
}

Vector
GaussMarkovMobilityModel::DoGetPosition (void) const
{
  m_helper.Update ();
  return m_helper.GetCurrentPosition ();
}
void 
GaussMarkovMobilityModel::DoSetPosition (const Vector &position)
{
  m_helper.SetPosition (position);
  Simulator::Remove (m_event);
  m_event = Simulator::ScheduleNow (&GaussMarkovMobilityModel::Start, this);
}
Vector
GaussMarkovMobilityModel::DoGetVelocity (void) const
{
  return m_helper.GetVelocity ();
}


} // namespace ns3
