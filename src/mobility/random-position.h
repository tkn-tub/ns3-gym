/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
#ifndef RANDOM_POSITION_H
#define RANDOM_POSITION_H

#include "ns3/object.h"
#include "ns3/component-manager.h"
#include "position.h"

namespace ns3 {

class RandomVariable;

/**
 * \brief choose a position at random.
 *
 * This is a pure abstract base class.
 */
class RandomPosition : public Object
{
public:
  static const InterfaceId iid;
  RandomPosition ();
  virtual ~RandomPosition ();
  /**
   * \returns the next randomly-choosen position.
   */
  virtual Position Get (void) const = 0;
};

/**
 * \brief allocate random positions within a rectangle
 * according to a pair of random variables.
 */
class RandomRectanglePosition : public RandomPosition
{
public:
  static const ClassId cid;
  /**
   * Create a random position model based on the
   * Bind default values.
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
  virtual Position Get (void) const;
private:
  RandomVariable *m_x;
  RandomVariable *m_y;
};

/**
 * \brief allocate random positions within a disc
 * according to a pair of random variables.
 */
class RandomDiscPosition : public RandomPosition
{
public:
  static const ClassId cid;
  /**
   * Create a random position model based on the
   * Bind default values.
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
  virtual Position Get (void) const;
private:
  RandomVariable *m_theta;
  RandomVariable *m_rho;
  double m_x;
  double m_y;
};

} // namespace ns3

#endif /* RANDOM_POSITION_H */
