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

namespace ns3 {

class RandomWalkPositionParameters : public Object
{
 public:
  enum Mode {
    MODE_DISTANCE,
    MODE_TIME
  };
  RandomWalkPositionParameters ();
  bool IsDefault (void) const;
  void SetSpeedBounds (double minSpeed, double maxSpeed);
 private:
  friend class RandomWalkPosition;
  double m_xMin;
  double m_xMax;
  double m_yMin;
  double m_yMax;
  double m_minSpeed;
  double m_maxSpeed;
  enum Mode m_mode;
  double m_modeDistance;
  Time m_modeTime;
};

/**
 * \brief a random walk position model
 *
 * Each instance moves with a speed and direction choosen at random
 * in the intervals [minspeed,maxspeed] and [0,2pi] until
 * either a fixed distance has been walked or until a fixed period
 * of time.
 */
class RandomWalkPosition : public Position 
{
 public:
  /**
   * Create a new position object located at a random
   * position within the default random walk area.
   */
  RandomWalkPosition ();
  RandomWalkPosition (double x, double y);
  RandomWalkPosition (Ptr<RandomWalkPositionParameters> parameters);
  RandomWalkPosition (Ptr<RandomWalkPositionParameters> parameters,
		      double x, double y);
 private:
  virtual void DoGet (double &x, double &y, double &z) const;
  virtual void DoSet (double x, double y, double z);

  void Reset (void);
  void Update (void) const;
  static Ptr<RandomWalkPositionParameters> GetDefaultParameters (void);
  static UniformVariable m_randomDirection;

  mutable double m_x;
  mutable double m_y;
  double m_dx;
  double m_dy;
  mutable Time m_prevTime;
  Ptr<RandomWalkPositionParameters> m_parameters;
};


} // namespace ns3

#endif /* RANDOM_WALK_POSITION_H */
