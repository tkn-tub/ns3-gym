/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
#include "rectangle.h"
#include "position.h"
#include <cmath>
#include <algorithm>

namespace ns3 {

Rectangle::Rectangle (double _xMin, double _xMax,
		      double _yMin, double _yMax)
  : xMin (_xMin),
    xMax (_xMax),
    yMin (_yMin),
    yMax (_yMax)
{}

Rectangle::Rectangle ()
  : xMin (0.0),
    xMax (0.0),
    yMin (0.0),
    yMax (0.0)
{}

bool 
Rectangle::IsInside (const Position &position) const
{
  return 
    position.x <= xMax && position.x >= xMin &&
    position.y <= yMax && position.y >= yMin;
}

Rectangle::Side 
Rectangle::GetClosestSide (const Position &position) const
{
  double xMinDist = std::abs (position.x - xMin);
  double xMaxDist = std::abs (xMax - position.x);
  double yMinDist = std::abs (position.y - yMin);
  double yMaxDist = std::abs (yMax - position.y);
  double minX = std::min (xMinDist, xMaxDist);
  double minY = std::min (yMinDist, yMaxDist);
  if (minX < minY)
    {
      if (xMinDist < xMaxDist)
        {
          return LEFT;
        }
      else
        {
          return RIGHT;
        }
    }
  else
    {
      if (yMinDist < yMaxDist)
        {
          return BOTTOM;
        }
      else
        {
          return TOP;
        }
    }
}


} // namespace ns3
