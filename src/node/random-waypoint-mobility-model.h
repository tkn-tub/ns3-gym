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
#ifndef RANDOM_WAYPOINT_MOBILITY_MODEL_H
#define RANDOM_WAYPOINT_MOBILITY_MODEL_H

#include "static-speed-helper.h"
#include "mobility-model.h"
#include "random-position.h"
#include "ns3/ptr.h"

namespace ns3 {

class RandomVariable;

class RandomWaypointMobilityModelParameters : public Object
{
public:
  RandomWaypointMobilityModelParameters ();
  RandomWaypointMobilityModelParameters (Ptr<RandomPosition> randomPosition,
                                         const RandomVariable &speed,
                                         const RandomVariable &pause);
  void SetWaypointPositionModel (Ptr<RandomPosition> randomPosition);
  void SetSpeed (const RandomVariable &speed);
  void SetPause (const RandomVariable &pause);
private:
  friend class RandomWaypointMobilityModel;
  static Ptr<RandomWaypointMobilityModelParameters> GetCurrent (void);
  virtual void DoDispose (void);
  RandomVariable *m_speed;
  RandomVariable *m_pause;
  Ptr<RandomPosition> m_position;
};

class RandomWaypointMobilityModel : public MobilityModel
{
public:
  RandomWaypointMobilityModel ();
  RandomWaypointMobilityModel (Ptr<RandomWaypointMobilityModelParameters> parameters);
private:
  void Start (void);
  virtual Position DoGet (void) const;
  virtual void DoSet (const Position &position);
  StaticSpeedHelper m_helper;
  Ptr<RandomWaypointMobilityModelParameters> m_parameters;
  EventId m_event;
};

} // namespace ns3

#endif /* RANDOM_WAYPOINT_MOBILITY_MODEL_H */
