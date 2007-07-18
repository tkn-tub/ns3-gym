#ifndef RANDOM_POSITION_H
#define RANDOM_POSITION_H

#include "ns3/object.h"
#include "ns3/component-manager.h"
#include "position.h"

namespace ns3 {

class RandomVariable;

class RandomPosition : public Object
{
public:
  static const InterfaceId iid;
  RandomPosition ();
  virtual ~RandomPosition ();
  virtual Position Get (void) const = 0;
};

class RandomRectanglePosition : public RandomPosition
{
public:
  static const ClassId cid;
  RandomRectanglePosition ();
  RandomRectanglePosition (const RandomVariable &x,
			   const RandomVariable &y);
  virtual ~RandomRectanglePosition ();
  virtual Position Get (void) const;
private:
  RandomVariable *m_x;
  RandomVariable *m_y;
};

class RandomDiscPosition : public RandomPosition
{
public:
  static const ClassId cid;
  RandomDiscPosition ();
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
