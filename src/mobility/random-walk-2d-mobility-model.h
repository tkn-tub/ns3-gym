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
#ifndef RANDOM_WALK_2D_MOBILITY_MODEL_H
#define RANDOM_WALK_2D_MOBILITY_MODEL_H

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/rectangle.h"
#include "ns3/random-variable.h"
#include "mobility-model.h"
#include "static-speed-helper.h"

namespace ns3 {


/**
 * \brief a 2D random walk position model
 *
 * Each instance moves with a speed and direction choosen at random
 * with the user-provided random variables until
 * either a fixed distance has been walked or until a fixed amount
 * of time.
 *
 */
class RandomWalk2dMobilityModel : public MobilityModel 
{
 public:
  static TypeId GetTypeId (void);

  enum Mode  {
    MODE_DISTANCE,
    MODE_TIME
  };

  RandomWalk2dMobilityModel ();

 private:
  void Start (void);
  void Rebound (Time timeLeft);
  void DoWalk (Time timeLeft);
  virtual void DoDispose (void);
  virtual Vector DoGetPosition (void) const;
  virtual void DoSetPosition (const Vector &position);
  virtual Vector DoGetVelocity (void) const;

  StaticSpeedHelper m_helper;
  EventId m_event;
  enum Mode m_mode;
  double m_modeDistance;
  Time m_modeTime;
  RandomVariable m_speed;
  RandomVariable m_direction;
  Rectangle m_bounds;
};


} // namespace ns3

#endif /* RANDOM_WALK_2D_MOBILITY_MODEL_H */
