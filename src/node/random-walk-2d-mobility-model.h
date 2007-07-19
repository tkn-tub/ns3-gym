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
#ifndef RANDOM_WALK_2D_MOBILITY_MODEL_H
#define RANDOM_WALK_2D_MOBILITY_MODEL_H

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/component-manager.h"
#include "ns3/event-id.h"
#include "ns3/rectangle.h"
#include "mobility-model.h"
#include "static-speed-helper.h"

namespace ns3 {

class RandomVariable;

/**
 * \brief parameters to control a random walk model
 */
class RandomWalk2dMobilityModelParameters : public Object
{
 public:
  /**
   * Instantiate a set of RandomWalk parameters initialized
   * with the Bind default values.
   */
  RandomWalk2dMobilityModelParameters ();
  virtual ~RandomWalk2dMobilityModelParameters ();
  /**
   */
  void SetSpeed (const RandomVariable &speed);
  void SetDirection (const RandomVariable &direction);
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


  enum Mode  {
    MODE_DISTANCE,
    MODE_TIME
  };
 private:
  friend class RandomWalk2dMobilityModel;
  static Ptr<RandomWalk2dMobilityModelParameters> GetCurrent (void);

  enum Mode m_mode;
  double m_modeDistance;
  Time m_modeTime;
  RandomVariable *m_speed;
  RandomVariable *m_direction;
  Rectangle m_bounds;
};

/**
 * \brief a 2D random walk position model
 *
 * Each instance moves with a speed and direction choosen at random
 * in the intervals [minspeed,maxspeed] and [0,2pi] until
 * either a fixed distance has been walked or until a fixed amount
 * of time.
 *
 * The parameters of the model can be specified either with the ns3::Bind
 * function and the variables "RandomWalk2dMinSpeed", "RandomWalk2dMaxSpeed",
 * "RandomWalk2dMode", "RandomWalk2dModeDistance", and, "RandomWalk2dModeTime" or
 * with an instance of the RandomWalk2dMobilityModelParameters class which
 * must be fed to the RandomWalk2dMobilityModel constructors.
 */
class RandomWalk2dMobilityModel : public MobilityModel 
{
 public:
  static const ClassId cid;
  /**
   * Create a new position object located at position (0,0,0)
   */
  RandomWalk2dMobilityModel ();
  /**
   * Create a new position object located at position (0,0,0)
   */
  RandomWalk2dMobilityModel (Ptr<RandomWalk2dMobilityModelParameters> parameters);

 private:
  void Start (void);
  void Rebound (Time timeLeft);
  void DoWalk (Time timeLeft);
  virtual void DoDispose (void);
  virtual Position DoGet (void) const;
  virtual void DoSet (const Position &position);
  virtual Speed DoGetSpeed (void) const;

  StaticSpeedHelper m_helper;
  EventId m_event;
  Ptr<RandomWalk2dMobilityModelParameters> m_parameters;
};


} // namespace ns3

#endif /* RANDOM_WALK_2D_MOBILITY_MODEL_H */
