#ifndef RANDOM_2D_POSITION_H
#define RANDOM_2D_POSITION_H

#include "ns3/object.h"
#include "position-2d.h"

namespace ns3 {

class RandomVariable;

class Random2dPosition : public Object
{
public:
  virtual ~Random2dPosition ();
  virtual Position2d Get (void) const = 0;
};

class RandomRectanglePosition : public Random2dPosition
{
public:
  RandomRectanglePosition ();
  RandomRectanglePosition (const RandomVariable &x,
			   const RandomVariable &y);
  virtual ~RandomRectanglePosition ();
  virtual Position2d Get (void) const;
private:
  RandomVariable *m_x;
  RandomVariable *m_y;
};

class RandomDiscPosition : public Random2dPosition
{
public:
  RandomDiscPosition ();
  RandomDiscPosition (const RandomVariable &theta,
		      const RandomVariable &rho);
  virtual ~RandomDiscPosition ();
  virtual Position2d Get (void) const;
private:
  RandomVariable *m_theta;
  RandomVariable *m_rho;
};

} // namespace ns3

#endif /* RANDOM_2D_POSITION_H */
