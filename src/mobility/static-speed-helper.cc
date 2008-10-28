/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
#include "ns3/rectangle.h"
#include "static-speed-helper.h"

namespace ns3 {

StaticSpeedHelper::StaticSpeedHelper ()
{}
StaticSpeedHelper::StaticSpeedHelper (const Vector &position)
  : m_position (position)
{}
StaticSpeedHelper::StaticSpeedHelper (const Vector &position,
				      const Vector &vel)
  : m_position (position),
    m_velocity (vel),
    m_paused (true)
{}
void 
StaticSpeedHelper::SetPosition (const Vector &position)
{
  m_position = position;
  m_velocity = Vector (0.0, 0.0, 0.0);
  m_lastUpdate = Simulator::Now ();
}

Vector
StaticSpeedHelper::GetCurrentPosition (void) const
{
  return m_position;
}

Vector 
StaticSpeedHelper::GetVelocity (void) const
{
  return m_paused? Vector (0.0, 0.0, 0.0) : m_velocity;
}
void 
StaticSpeedHelper::SetVelocity (const Vector &vel)
{
  m_velocity = vel;
  m_lastUpdate = Simulator::Now ();
}

void
StaticSpeedHelper::Update (void) const
{
  Time now = Simulator::Now ();
  NS_ASSERT (m_lastUpdate <= now);
  Time deltaTime = now - m_lastUpdate;
  m_lastUpdate = now;
  if (m_paused)
    {
      return;
    }
  double deltaS = deltaTime.GetSeconds ();
  m_position.x += m_velocity.x * deltaS;
  m_position.y += m_velocity.y * deltaS;
  m_position.z += m_velocity.z * deltaS;
}

void
StaticSpeedHelper::UpdateWithBounds (const Rectangle &bounds) const
{
  Update ();
  m_position.x = std::min (bounds.xMax, m_position.x);
  m_position.x = std::max (bounds.xMin, m_position.x);
  m_position.y = std::min (bounds.yMax, m_position.y);
  m_position.y = std::max (bounds.yMin, m_position.y);
}

void 
StaticSpeedHelper::Pause (void)
{
  m_paused = true;
}

void 
StaticSpeedHelper::Unpause (void)
{
  m_paused = false;
}

} // namespace ns3
