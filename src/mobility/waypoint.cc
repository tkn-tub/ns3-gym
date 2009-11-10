/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 Phillip Sitbon
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
 * Author: Phillip Sitbon <phillip@sitbon.net>
 */
#include "waypoint.h"

namespace ns3 {

ATTRIBUTE_HELPER_CPP (Waypoint);

Waypoint::Waypoint (const Time &waypointTime, const Vector &waypointPosition)
  : m_time (waypointTime),
  m_position (waypointPosition)
{}
Waypoint::Waypoint ()
  : m_time (0.0),
  m_position (0,0,0)
{}

Time Waypoint::GetTime () const
{
  return m_time;
}

void Waypoint::SetTime (Time time)
{
  m_time = time;
}

Vector Waypoint::GetPosition () const
{
  return m_position;
}

void Waypoint::SetPosition (Vector pos)
{
  m_position = pos;
}

std::ostream &operator << (std::ostream &os, const Waypoint &waypoint)
{
  os << waypoint.GetTime ().GetSeconds () << "$" << waypoint.GetPosition ();
  return os;
}
std::istream &operator >> (std::istream &is, Waypoint &waypoint)
{
  char separator;
  Time time = waypoint.GetTime ();
  Vector pos = waypoint.GetPosition ();
  is >> time >> separator >> pos;
  if (separator != '$')
    {
      is.setstate (std::ios_base::failbit);
    }
  return is;
}

} // namespace ns3

