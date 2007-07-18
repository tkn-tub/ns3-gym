#include "position.h"
#include <cmath>

namespace ns3 {


Position::Position (double _x, double _y, double _z)
  : x (_x),
    y (_y),
    z (_z)
{}

Position::Position ()
  : x (0.0),
    y (0.0),
    z (0.0)
{}

double 
CalculateDistance (const Position &a, const Position &b)
{
  double dx = b.x - a.x;
  double dy = b.y - a.y;
  double dz = b.z - a.z;
  double distance = std::sqrt (dx * dx + dy * dy + dz * dz);
  return distance;
}

} // namespace ns3
