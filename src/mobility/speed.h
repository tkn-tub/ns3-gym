/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
#ifndef SPEED_H
#define SPEED_H

namespace ns3 {

/**
 * \brief keep track of 3d cartesian speed vectors
 *
 * Unit is meters/s.
 */
class Speed 
{
public:
  /**
   * \param _dx x coordinate of speed vector
   * \param _dy y coordinate of speed vector
   * \param _dz z coordinate of speed vector
   *
   * Create speed vector (_dx, _dy, _dz)
   */
  Speed (double _dx, double _dy, double _dz);
  /**
   * Create speed vector (0.0, 0.0, 0.0)
   */
  Speed ();
  /**
   * x coordinate of speed vector
   */
  double dx;
  /**
   * y coordinate of speed vector
   */
  double dy;
  /**
   * z coordinate of speed vector
   */
  double dz;
};

} // namespace ns3

#endif /* SPEED_H */
