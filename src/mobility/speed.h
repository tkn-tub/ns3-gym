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
