/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
#ifndef RANDOM_WALK_POSITION_H
#define RANDOM_WALK_POSITION_H

#include "ns3/object.h"
#include "ns3/position.h"
#include "ns3/nstime.h"
#include "ns3/random-variable.h"
#include "ns3/component-manager.h"

namespace ns3 {

/**
 * \brief parameters to control a random walk model
 */
class RandomWalkMobilityModelParameters : public Object
{
 public:
  enum Mode {
    MODE_DISTANCE,
    MODE_TIME
  };
  /**
   * Instantiate a set of RandomWalk parameters initialized
   * with the Bind default values.
   */
  RandomWalkMobilityModelParameters ();
  /**
   * \param minSpeed the minimum speed
   * \param maxSpeed the maximum speed
   *
   * The speed of any node is chosen such that minSpeed <= speed <= maxSpeed
   */
  void SetSpeedBounds (double minSpeed, double maxSpeed);
  /**
   * \param distance the distance before a direction change
   *
   * Unit is meters
   */
  void SetModeDistance (double distance);
  /**
   * \param time the delay before a direction change.
   */
  void SetModeTime (Time time);
 private:
  bool IsDefault (void) const;
  friend class RandomWalkMobilityModel;
  double m_minSpeed;
  double m_maxSpeed;
  enum Mode m_mode;
  double m_modeDistance;
  Time m_modeTime;
};

/**
 * \brief an unbounded 2D random walk position model
 *
 * Each instance moves with a speed and direction choosen at random
 * in the intervals [minspeed,maxspeed] and [0,2pi] until
 * either a fixed distance has been walked or until a fixed amount
 * of time.
 *
 * The parameters of the model can be specified either with the ns3::Bind
 * function and the variables "RandomWalkMinSpeed", "RandomWalkMaxSpeed",
 * "RandomWalkMode", "RandomWalkModeDistance", and, "RandomWalkModeTime" or
 * with an instance of the RandomWalkMobilityModelParameters class which
 * must be fed to the RandomWalkMobilityModel constructors.
 */
class RandomWalkMobilityModel : public MobilityModel 
{
 public:
  static const InterfaceId iid;
  static const ClassId cid;
  /**
   * Create a new position object located at position (0,0,0)
   */
  RandomWalkMobilityModel ();
  /**
   * Create a new position object located at position (x,y,0)
   */
  RandomWalkMobilityModel (double x, double y);
  /**
   * Create a new position object located at position (0,0,0)
   */
  RandomWalkMobilityModel (Ptr<RandomWalkMobilityModelParameters> parameters);
  /**
   * Create a new position object located at position (x,y,0)
   */
  RandomWalkMobilityModel (Ptr<RandomWalkMobilityModelParameters> parameters,
		      double x, double y);
 private:
  virtual void DoDispose (void);
  virtual void DoGet (double &x, double &y, double &z) const;
  virtual void DoSet (double x, double y, double z);

  void Reset (void);
  void Update (void) const;
  static Ptr<RandomWalkMobilityModelParameters> GetDefaultParameters (void);
  static UniformVariable m_randomDirection;

  mutable double m_x;
  mutable double m_y;
  double m_dx;
  double m_dy;
  mutable Time m_prevTime;
  Ptr<RandomWalkMobilityModelParameters> m_parameters;
};


} // namespace ns3

#endif /* RANDOM_WALK_POSITION_H */
