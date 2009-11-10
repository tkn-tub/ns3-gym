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
      NS_ABORT_MSG_IF ( !m_waypoints.empty () && (m_waypoints.back ().GetTime () >= waypoint.GetTime ()),
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

  if ( now <= m_current.GetTime () )
    {
      return;
    }

  while ( now >= m_next.GetTime ()  )
    {
      if ( m_waypoints.empty () )
        {
          if ( m_current.GetTime () <= m_next.GetTime () )
            {
              m_current.SetPosition(m_next.GetPosition ());
              m_current.SetTime (now);
              m_velocity = Vector (0,0,0);
              NotifyCourseChange ();
            }
          else
            {
              m_current.SetTime (now);
            }

          return;
        }

      m_current = m_next;
      m_next = m_waypoints.front ();
      m_waypoints.pop_front ();
      newWaypoint = true;

      const double t_span = (m_next.GetTime () - m_current.GetTime ()).GetSeconds ();
      NS_ASSERT (t_span > 0);
      m_velocity.x = (m_next.GetPosition ().x - m_current.GetPosition ().x) / t_span;
      m_velocity.y = (m_next.GetPosition ().y - m_current.GetPosition ().y) / t_span;
      m_velocity.z = (m_next.GetPosition ().z - m_current.GetPosition ().z) / t_span;
    }


  const double t_diff = (now - m_current.GetTime ()).GetSeconds();
  Vector pos;
  pos.x = m_current.GetPosition ().x + m_velocity.x * t_diff;
  pos.y = m_current.GetPosition ().y + m_velocity.y * t_diff;
  pos.z = m_current.GetPosition ().z + m_velocity.z * t_diff;
  m_current.SetPosition (pos);
  m_current.SetTime (now);

  if ( newWaypoint )
    {
      NotifyCourseChange ();
    }
}
Vector
WaypointMobilityModel::DoGetPosition (void) const
{
  Update ();
  return m_current.GetPosition ();
}
void
WaypointMobilityModel::DoSetPosition (const Vector &position)
{
  const Time now = Simulator::Now ();
  Update ();
  m_current.SetTime (std::max (now, m_next.GetTime ()));
  m_current.SetPosition (position);
  m_velocity = Vector (0,0,0);
  NotifyCourseChange ();
}
void
WaypointMobilityModel::EndMobility (void)
{
  m_waypoints.clear ();
  m_current.SetTime (Seconds (std::numeric_limits<double>::infinity ()));
  m_next.SetTime (m_current.GetTime ());
  m_first = true;
}
Vector
WaypointMobilityModel::DoGetVelocity (void) const
{
  return m_velocity;
}

} // namespace ns3

