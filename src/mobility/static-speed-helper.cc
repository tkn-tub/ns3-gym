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
				      const Vector &speed)
  : m_position (position),
    m_speed (speed),
    m_paused (true)
{}
void 
StaticSpeedHelper::InitializePosition (const Vector &position)
{
  m_position = position;
  m_speed.x = 0.0;
  m_speed.y = 0.0;
  m_speed.z = 0.0;
  m_lastUpdate = Simulator::Now ();
  m_paused = true;
}

Vector
StaticSpeedHelper::GetCurrentPosition (void) const
{
  Update ();
  return m_position;
}

Vector 
StaticSpeedHelper::GetSpeed (void) const
{
  return m_paused? Vector (0.0, 0.0, 0.0) : m_speed;
}
void 
StaticSpeedHelper::SetSpeed (const Vector &speed)
{
  Update ();
  m_speed = speed;
}

void
StaticSpeedHelper::Update (void) const
{
  if (m_paused)
    {
      return;
    }
  Time now = Simulator::Now ();
  NS_ASSERT (m_lastUpdate <= now);
  Time deltaTime = now - m_lastUpdate;
  m_lastUpdate = now;
  double deltaS = deltaTime.GetSeconds ();
  m_position.x += m_speed.x * deltaS;
  m_position.y += m_speed.y * deltaS;
  m_position.z += m_speed.z * deltaS;
}

void 
StaticSpeedHelper::Reset (const Vector &speed)
{
  Update ();
  m_speed = speed;
  Unpause ();
}
void
StaticSpeedHelper::UpdateFull (const Rectangle &bounds) const
{
  Update ();
  m_position.x = std::min (bounds.xMax, m_position.x);
  m_position.x = std::max (bounds.xMin, m_position.x);
  m_position.y = std::min (bounds.yMax, m_position.y);
  m_position.y = std::max (bounds.yMin, m_position.y);
}

Vector 
StaticSpeedHelper::GetCurrentPosition (const Rectangle &bounds) const
{
  UpdateFull (bounds);
  return m_position;
}

void 
StaticSpeedHelper::Pause (void)
{
  Update ();
  m_paused = true;
}

void 
StaticSpeedHelper::Unpause (void)
{
  if (m_paused)
    {
      m_lastUpdate = Simulator::Now ();
      m_paused = false;
    }
}

} // namespace ns3
