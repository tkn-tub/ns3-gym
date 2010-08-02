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
#ifndef WAYPOINT_MOBILITY_MODEL_H
#define WAYPOINT_MOBILITY_MODEL_H

#include <stdint.h>
#include <deque>
#include "mobility-model.h"
#include "ns3/vector.h"
#include "waypoint.h"

namespace ns3 {

/**
 * \ingroup mobility
 * \brief Waypoint-based mobility model.
 *
 * Each object determines its velocity and position at a given time
 * from a set of ns3::Waypoint objects. The position of each object
 * is not updated unless queried, and past waypoints are discarded
 * after the current simulation time greater than their time value.
 * 
 * The initial position of each object corresponds to the position of
 * the first waypoint, and the initial velocity of each object is zero.
 * Upon reaching the last waypoint, object positions becomes static and
 * velocity is zero.
 *
 * When a node is in between waypoints (in time), it moves with a constant
 * velocity between the position at the previous waypoint and the position
 * at the current waypoint. To make a node hold a certain position for a
 * time interval, two waypoints with the same position (but different times)
 * should be inserted sequentially.
 *
 * Waypoints can be added at any time, and setting the current position
 * of an object will set its velocity to zero until the next waypoint time
 * (at which time the object jumps to the next waypoint), unless there are
 * no more waypoints in which case it will not change without user
 * intervention.
 *
 */
class WaypointMobilityModel : public MobilityModel
{
 public:
  static TypeId GetTypeId (void);

  /**
   * Create a path with no waypoints at location (0,0,0).
   */
  WaypointMobilityModel ();
  virtual ~WaypointMobilityModel ();

  /**
   * \param waypoint waypoint to append to the object path.
   *
   * Add a waypoint to the path of the object. The time must
   * be greater than the previous waypoint added, otherwise
   * a fatal error occurs. The first waypoint is set as the
   * current position with a velocity of zero.
   * 
   */
  void AddWaypoint (const Waypoint &waypoint);

  /**
   * Get the waypoint that this object is traveling towards.
   */
  Waypoint GetNextWaypoint (void) const;

  /**
   * Get the number of waypoints left for this object, excluding
   * the next one.
   */
  uint32_t WaypointsLeft (void) const;

  /**
   * Clear any existing waypoints and set the current waypoint
   * time to infinity. Calling this is only an optimization and
   * not required. After calling this function, adding waypoints
   * behaves as it would for a new object.
   */
  void EndMobility (void);

 private:
  void Update (void) const;
  virtual void DoDispose (void);
  virtual Vector DoGetPosition (void) const;
  virtual void DoSetPosition (const Vector &position);
  virtual Vector DoGetVelocity (void) const;

  bool m_first;
  mutable std::deque<Waypoint> m_waypoints;
  mutable Waypoint m_current;
  mutable Waypoint m_next;
  mutable Vector m_velocity;
};

} // namespace ns3

#endif /* WAYPOINT_MOBILITY_MODEL_H */

