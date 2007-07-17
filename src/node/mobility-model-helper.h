#ifndef MOBILITY_MODEL_HELPER_H
#define MOBILITY_MODEL_HELPER_H

#include "ns3/nstime.h"

namespace ns3 {

class Position2D
{
public:
  Position2D (double _x, double _y);
  Position2D ();
  double x;
  double y;
};

struct AreaBounds
{
  double xMin;
  double xMax;
  double yMin;
  double yMax;
};

class Speed2D
{
public:
  Speed2D (double _dx, double _dy);
  Speed2D ();
  double dx;
  double dy;
};

class MobilityModelHelper
{
 public:
  enum Side {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    NONE
  };
  MobilityModelHelper ();
  void InitializePosition (const struct Position2D &position);
  Time Reset (const struct AreaBounds &bounds,
	      double direction,
	      double speed,
	      const Time &maxMovementDelay,
	      const Time &pauseDelay);
  Time Reset (const struct AreaBounds &bounds,
	      double direction,
	      double speed,
	      double maxDistance,
	      const Time &pauseDelay);
  // move in specified direction until
  // we hit the area bounds.
  // return delay until we reach the bounds.
  Time Reset (const struct AreaBounds &bounds,
	      double direction,
	      double speed,
	      const Time &pauseDelay);

  Position2D GetCurrentPosition (const struct AreaBounds &bounds) const;
  Speed2D GetSpeed (void) const;
  enum MobilityModelHelper::Side GetSide (const struct AreaBounds &bounds) const;

 private:
  double Distance (const Position2D &a, const Position2D &b) const;
  void Update (const struct AreaBounds &bounds) const;
  Position2D CalculateIntersection (const struct AreaBounds &bounds) const;
  mutable Position2D m_position;
  Speed2D m_speed;
  mutable Time m_lastUpdate;
  Time m_pauseEnd;
};

} // namespace ns3

#endif /* MOBILITY_MODEL_HELPER_H */
