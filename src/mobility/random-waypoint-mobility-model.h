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
#ifndef RANDOM_WAYPOINT_MOBILITY_MODEL_H
#define RANDOM_WAYPOINT_MOBILITY_MODEL_H

#include "constant-velocity-helper.h"
#include "mobility-model.h"
#include "position-allocator.h"
#include "ns3/ptr.h"
#include "ns3/random-variable.h"

namespace ns3 {

/**
 * \ingroup mobility
 * \brief Random waypoint mobility model.
 *
 * Each object chooses a random destination "waypoint", a random speed,
 * and a random pause time: it then pauses for the specified pause time,
 * and starts moving towards the specified destination with the specified
 * speed. Once the destination is reached the process starts again.
 *
 * The implementation of this model is not 2d-specific. i.e. if you provide
 * a 3d random waypoint position model to this mobility model, the model 
 * will still work. There is no 3d position allocator for now but it should
 * be trivial to add one.
 */
class RandomWaypointMobilityModel : public MobilityModel
{
public:
  static TypeId GetTypeId (void);
protected:
  virtual void DoStart (void);
private:
  void BeginWalk (void);
  void DoStartPrivate (void);
  virtual Vector DoGetPosition (void) const;
  virtual void DoSetPosition (const Vector &position);
  virtual Vector DoGetVelocity (void) const;

  ConstantVelocityHelper m_helper;
  Ptr<PositionAllocator> m_position;
  RandomVariable m_speed;
  RandomVariable m_pause;
  EventId m_event;
};

} // namespace ns3

#endif /* RANDOM_WAYPOINT_MOBILITY_MODEL_H */
