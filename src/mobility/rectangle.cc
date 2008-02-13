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
#include "vector.h"
#include "ns3/assert.h"
#include "ns3/fatal-error.h"
#include <cmath>
#include <algorithm>
#include <sstream> 

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
Rectangle::IsInside (const Vector &position) const
{
  return 
    position.x <= this->xMax && position.x >= this->xMin &&
    position.y <= this->yMax && position.y >= this->yMin;
}

Rectangle::Side 
Rectangle::GetClosestSide (const Vector &position) const
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

Vector
Rectangle::CalculateIntersection (const Vector &current, const Vector &speed) const
{
  double xMaxY = current.y + (this->xMax - current.x) / speed.x * speed.y;
  double xMinY = current.y + (this->xMin - current.x) / speed.x * speed.y;
  double yMaxX = current.x + (this->yMax - current.y) / speed.y * speed.x;
  double yMinX = current.x + (this->yMin - current.y) / speed.y * speed.x;
  bool xMaxYOk = (xMaxY <= this->yMax && xMaxY >= this->yMin);
  bool xMinYOk = (xMinY <= this->yMax && xMinY >= this->yMin);
  bool yMaxXOk = (yMaxX <= this->xMax && yMaxX >= this->xMin);
  bool yMinXOk = (yMinX <= this->xMax && yMinX >= this->xMin);
  if (xMaxYOk && speed.x >= 0)
    {
      return Vector (this->xMax, xMaxY, 0.0);
    }
  else if (xMinYOk && speed.x <= 0)
    {
      return Vector (this->xMin, xMinY, 0.0);
    }
  else if (yMaxXOk && speed.y >= 0)
    {
      return Vector (yMaxX, this->yMax, 0.0);
    }
  else if (yMinXOk && speed.y <= 0)
    {
      return Vector (yMinX, this->yMin, 0.0);
    }
  else
    {
      NS_ASSERT (false);
      // quiet compiler
      return Vector (0.0, 0.0, 0.0);
    }

}

Rectangle::Rectangle (PValue value)
{
  *this = ClassValueHelperExtractFrom<Rectangle,RectangleValue> (value);
}
Rectangle::operator PValue () const
{
  return ClassValueHelperConvertTo<Rectangle,RectangleValue> (this);
}

std::ostream &
operator << (std::ostream &os, const Rectangle &rectangle)
{
  os << rectangle.xMin << "|" << rectangle.xMax << "|" << rectangle.yMin << "|" << rectangle.yMax;
  return os;
}
std::istream &
operator >> (std::istream &is, Rectangle &rectangle)
 {
  char c1, c2, c3;
  is >> rectangle.xMin >> c1 >> rectangle.xMax >> c2 >> rectangle.yMin >> c3 >> rectangle.yMax;
  if (c1 != '|' ||
      c2 != '|' ||
      c3 != '|')
    {
      is.setstate (std::ios_base::failbit);
    }
  return is;
}


} // namespace ns3
