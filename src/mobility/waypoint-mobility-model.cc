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
#include <limits>
#include "ns3/abort.h"
#include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"
#include "waypoint-mobility-model.h"

NS_LOG_COMPONENT_DEFINE ("WaypointMobilityModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WaypointMobilityModel);


TypeId
WaypointMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WaypointMobilityModel")
    .SetParent<MobilityModel> ()
    .SetGroupName ("Mobility")
    .AddConstructor<WaypointMobilityModel> ()
    .AddAttribute ("NextWaypoint", "The next waypoint used to determine position.",
                   TypeId::ATTR_GET,
                   WaypointValue (),
                   MakeWaypointAccessor (&WaypointMobilityModel::GetNextWaypoint),
                   MakeWaypointChecker ())
    .AddAttribute ("WaypointsLeft", "The number of waypoints remaining.",
                   TypeId::ATTR_GET,
                   UintegerValue (0),
                   MakeUintegerAccessor (&WaypointMobilityModel::WaypointsLeft),
                   MakeUintegerChecker<uint32_t> ())
    ;
  return tid;
}


WaypointMobilityModel::WaypointMobilityModel ()
 : m_first (true)
{
}
WaypointMobilityModel::~WaypointMobilityModel ()
{
}
void 
WaypointMobilityModel::DoDispose (void)
{
  MobilityModel::DoDispose ();
}
void
WaypointMobilityModel::AddWaypoint (const Waypoint &waypoint)
{
  if ( m_first )
    {
      m_first = false;
      m_current = m_next = waypoint;
    }
  else
    {
      NS_ABORT_MSG_IF ( !m_waypoints.empty () && (m_waypoints.back ().time >= waypoint.time),
                        "Waypoints must be added in ascending time order");
      m_waypoints.push_back (waypoint);
    }
}
Waypoint
WaypointMobilityModel::GetNextWaypoint (void) const
{
  Update ();
  return m_next;
}
uint32_t
WaypointMobilityModel::WaypointsLeft (void) const
{
  Update ();
  return m_waypoints.size();
}
void
WaypointMobilityModel::Update (void) const
{
  const Time now = Simulator::Now ();
  bool newWaypoint = false;

  if ( now <= m_current.time )
    {
      return;
    }

  while ( now >= m_next.time  )
    {
      if ( m_waypoints.empty () )
        {
          if ( m_current.time <= m_next.time )
            {
              m_current.position = m_next.position;
              m_current.time = now;
              m_velocity = Vector (0,0,0);
              NotifyCourseChange ();
            }
          else
            {
              m_current.time = now;
            }

          return;
        }

      m_current = m_next;
      m_next = m_waypoints.front ();
      m_waypoints.pop_front ();
      newWaypoint = true;

      const double t_span = (m_next.time - m_current.time).GetSeconds ();
      NS_ASSERT (t_span > 0);
      m_velocity.x = (m_next.position.x - m_current.position.x) / t_span;
      m_velocity.y = (m_next.position.y - m_current.position.y) / t_span;
      m_velocity.z = (m_next.position.z - m_current.position.z) / t_span;
    }


  const double t_diff = (now - m_current.time).GetSeconds();
  m_current.position.x += m_velocity.x * t_diff;
  m_current.position.y += m_velocity.y * t_diff;
  m_current.position.z += m_velocity.z * t_diff;
  m_current.time = now;

  if ( newWaypoint )
    {
      NotifyCourseChange ();
    }
}
Vector
WaypointMobilityModel::DoGetPosition (void) const
{
  Update ();
  return m_current.position;
}
void
WaypointMobilityModel::DoSetPosition (const Vector &position)
{
  const Time now = Simulator::Now ();
  Update ();
  m_current.time = std::max (now, m_next.time);
  m_current.position = position;
  m_velocity = Vector (0,0,0);
  NotifyCourseChange ();
}
void
WaypointMobilityModel::EndMobility (void)
{
  m_waypoints.clear ();
  m_current.time = Seconds (std::numeric_limits<double>::infinity ());
  m_next.time = m_current.time;
  m_first = true;
}
Vector
WaypointMobilityModel::DoGetVelocity (void) const
{
  return m_velocity;
}

} // namespace ns3

