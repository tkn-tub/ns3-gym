#ifndef POSITION_H
#define POSITION_H

namespace ns3 {

/**
 * \brief a 3d cartesian position vector
 *
 * Unit is meters.
 */
class Position
{
public:
  /**
   * \param _x x coordinate of position vector
   * \param _y y coordinate of position vector
   * \param _z z coordinate of position vector
   *
   * Create position vector (_x, _y, _z)
   */
  Position (double _x, double _y, double _z);
  /**
   * Create position vector (0.0, 0.0, 0.0)
   */
  Position ();
  /**
   * x coordinate of position vector
   */
  double x;
  /**
   * y coordinate of position vector
   */
  double y;
  /**
   * z coordinate of position vector
   */
  double z;
};

double CalculateDistance (const Position &a, const Position &b);

} // namespace ns3

#endif /* POSITION_H */
