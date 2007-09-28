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
#ifndef RECTANGLE_H
#define RECTANGLE_H

namespace ns3 {

class Position;
class Speed;

/**
 * \brief a 2d rectangle
 */
class Rectangle
{
public:
  enum Side {
    RIGHT,
    LEFT,
    TOP,
    BOTTOM
  };
  /**
   * \param _xMin x coordinates of left boundary.
   * \param _xMax x coordinates of right boundary.
   * \param _yMin y coordinates of bottom boundary.
   * \param _yMax y coordinates of top boundary.
   *
   * Create a rectangle.
   */
  Rectangle (double _xMin, double _xMax,
             double _yMin, double _yMax);
  /**
   * Create a zero-sized rectangle located at coordinates (0.0,0.0)
   */
  Rectangle ();
  bool IsInside (const Position &position) const;
  Side GetClosestSide (const Position &position) const;
  Position CalculateIntersection (const Position &current, const Speed &speed) const;

  double xMin;
  double xMax;
  double yMin;
  double yMax;
};

} // namespace ns3

#endif /* RECTANGLE_H */
