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
#include "rectangle.h"
#include "position.h"
#include "speed.h"
#include "ns3/assert.h"
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
    position.x <= this->xMax && position.x >= this->xMin &&
    position.y <= this->yMax && position.y >= this->yMin;
}

Rectangle::Side 
Rectangle::GetClosestSide (const Position &position) const
{
  double xMinDist = std::abs (position.x - this->xMin);
  double xMaxDist = std::abs (this->xMax - position.x);
  double yMinDist = std::abs (position.y - this->yMin);
  double yMaxDist = std::abs (this->yMax - position.y);
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

Position
Rectangle::CalculateIntersection (const Position &current, const Speed &speed) const
{
  double xMaxY = current.y + (this->xMax - current.x) / speed.dx * speed.dy;
  double xMinY = current.y + (this->xMin - current.x) / speed.dx * speed.dy;
  double yMaxX = current.x + (this->yMax - current.y) / speed.dy * speed.dx;
  double yMinX = current.x + (this->yMin - current.y) / speed.dy * speed.dx;
  bool xMaxOk = xMaxY <= this->yMax && xMaxY >= this->yMin;
  bool xMinOk = xMinY <= this->yMax && xMinY >= this->yMin;
  bool yMaxOk = yMaxX <= this->xMax && yMaxX >= this->xMin;
  bool yMinOk = yMinX <= this->xMax && yMinX >= this->xMin;
  if (xMaxOk && speed.dx >= 0)
    {
      return Position (this->xMax, xMaxY, 0.0);
    }
  else if (xMinOk && speed.dx <= 0)
    {
      return Position (this->xMin, xMinY, 0.0);
    }
  else if (yMaxOk && speed.dy >= 0)
    {
      return Position (yMaxX, this->yMax, 0.0);
    }
  else if (yMinOk && speed.dy <= 0)
    {
      return Position (yMinX, this->yMin, 0.0);
    }
  else
    {
      NS_ASSERT (false);
      // quiet compiler
      return Position (0.0, 0.0, 0.0);
    }

}


} // namespace ns3
