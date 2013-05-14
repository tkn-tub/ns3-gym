/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "ns3/random-variable-stream.h"

namespace ns3 {

/**
 * \ingroup mobility
 * \brief Random waypoint mobility model.
 *
 * Each object starts by pausing at time zero for the duration governed
 * by the random variable "Pause".  After pausing, the object will pick 
 * a new waypoint (via the PositionAllocator) and a new random speed 
 * via the random variable "Speed", and will begin moving towards the 
 * waypoint at a constant speed.  When it reaches the destination, 
 * the process starts over (by pausing).
 *
 * This mobility model enforces no bounding box by itself; the 
 * PositionAllocator assigned to this object will bound the movement.
 * If the user fails to provide a pointer to a PositionAllocator to
 * be used to pick waypoints, the simulation program will assert.
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
  virtual void DoInitialize (void);
private:
  void BeginWalk (void);
  void DoInitializePrivate (void);
  virtual Vector DoGetPosition (void) const;
  virtual void DoSetPosition (const Vector &position);
  virtual Vector DoGetVelocity (void) const;
  virtual int64_t DoAssignStreams (int64_t);

  ConstantVelocityHelper m_helper;
  Ptr<PositionAllocator> m_position;
  Ptr<RandomVariableStream> m_speed;
  Ptr<RandomVariableStream> m_pause;
  EventId m_event;
};

} // namespace ns3

#endif /* RANDOM_WAYPOINT_MOBILITY_MODEL_H */
