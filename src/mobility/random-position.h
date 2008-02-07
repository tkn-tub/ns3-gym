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
#ifndef RANDOM_POSITION_H
#define RANDOM_POSITION_H

#include "ns3/object.h"
#include "ns3/random-variable.h"
#include "vector.h"

namespace ns3 {

/**
 * \brief choose a position at random.
 *
 * This is a pure abstract base class.
 */
class RandomPosition : public Object
{
public:
  static TypeId GetTypeId (void);
  RandomPosition ();
  virtual ~RandomPosition ();
  /**
   * \returns the next randomly-choosen position.
   */
  virtual Vector Get (void) const = 0;
};

/**
 * \brief allocate random positions within a rectangle
 * according to a pair of random variables.
 */
class RandomRectanglePosition : public RandomPosition
{
public:
  static TypeId GetTypeId (void);
  /**
   * Create a random position model with construction
   * values from \valueref{RandomRectanglePositionX}, and
   * \valueref{RandomRectanglePositionY}
   */
  RandomRectanglePosition ();
  /**
   * \param x the random variable which is used to choose
   *        the x coordinates.
   * \param y the random variable which is used to choose
   *        the y coordinates.
   */
  RandomRectanglePosition (const RandomVariable &x,
			   const RandomVariable &y);
  virtual ~RandomRectanglePosition ();
  virtual Vector Get (void) const;
private:
  RandomVariable m_x;
  RandomVariable m_y;
};

/**
 * \brief allocate random positions within a disc
 * according to a pair of random variables.
 */
class RandomDiscPosition : public RandomPosition
{
public:
  static TypeId GetTypeId (void);
  /**
   * Create a random position model with construction
   * values from \valueref{RandomDiscPositionTheta},
   * \valueref{RandomDiscPositionRho}, 
   * \valueref{RandomDiscPositionX}, and,
   * \valueref{RandomDiscPositionY}.
   */
  RandomDiscPosition ();
  /**
   * \param theta the random variable used to pick
   *        the angle of the random position in polar
   *        coordinates.
   * \param rho the random variable used to pick the
   *        radius of the random position in polar
   *        coordinates.
   * \param x the x coordinate of the center of the
   *        polar coodinate system.
   * \param y the y coordinate of the center of the
   *        polar coodinate system.
   */
  RandomDiscPosition (const RandomVariable &theta,
		      const RandomVariable &rho,
		      double x, double y);
  virtual ~RandomDiscPosition ();
  virtual Vector Get (void) const;
private:
  RandomVariable m_theta;
  RandomVariable m_rho;
  double m_x;
  double m_y;
};

} // namespace ns3

#endif /* RANDOM_POSITION_H */
