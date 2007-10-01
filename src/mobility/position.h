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
