/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 Dan Broyles
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
 * Author: Dan Broyles <dbroyl01@ku.edu>
 */

#include "box.h"
#include "ns3/vector.h"
#include "ns3/assert.h"
#include "ns3/fatal-error.h"
#include <cmath>
#include <algorithm>
#include <sstream> 

namespace ns3 {

Box::Box (double _xMin, double _xMax,
          double _yMin, double _yMax,
          double _zMin, double _zMax)
  : xMin (_xMin),
    xMax (_xMax),
    yMin (_yMin),
    yMax (_yMax),
    zMin (_zMin),
    zMax (_zMax)
{
}

Box::Box ()
  : xMin (0.0),
    xMax (0.0),
    yMin (0.0),
    yMax (0.0),
    zMin (0.0),
    zMax (0.0)
{
}

bool
Box::IsInside (const Vector &position) const
{
  return
    position.x <= this->xMax && position.x >= this->xMin &&
    position.y <= this->yMax && position.y >= this->yMin &&
    position.z <= this->zMax && position.z >= this->zMin;
}

Box::Side
Box::GetClosestSide (const Vector &position) const
{
  double xMinDist = std::abs (position.x - this->xMin);
  double xMaxDist = std::abs (this->xMax - position.x);
  double yMinDist = std::abs (position.y - this->yMin);
  double yMaxDist = std::abs (this->yMax - position.y);
  double zMinDist = std::abs (position.z - this->zMin);
  double zMaxDist = std::abs (this->zMax - position.z);
  double minX = std::min (xMinDist, xMaxDist);
  double minY = std::min (yMinDist, yMaxDist);
  double minZ = std::min (zMinDist, zMaxDist);
  if (minX < minY && minX < minZ)
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
  else if (minY < minZ)
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
  else
    {
      if (zMinDist < zMaxDist)
        {
          return DOWN;
        }
      else
        {
          return UP;
        }
    }
}

Vector
Box::CalculateIntersection (const Vector &current, const Vector &speed) const
{
  NS_ASSERT (IsInside (current));
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

ATTRIBUTE_HELPER_CPP (Box);

/**
 * \brief Stream insertion operator.
 *
 * \param os the stream
 * \param box the box
 * \returns a reference to the stream
 */
std::ostream &
operator << (std::ostream &os, const Box &box)
{
  os << box.xMin << "|" << box.xMax << "|" << box.yMin << "|" << box.yMax << "|" << box.zMin << "|" << box.zMax;
  return os;
}
/**
 * \brief Stream extraction operator.
 *
 * \param is the stream
 * \param box the box
 * \returns a reference to the stream
 */
std::istream &
operator >> (std::istream &is, Box &box)
{
  char c1, c2, c3, c4, c5;
  is >> box.xMin >> c1 >> box.xMax >> c2 >> box.yMin >> c3 >> box.yMax >> c4 >> box.zMin >> c5 >> box.zMax;
  if (c1 != '|' ||
      c2 != '|' ||
      c3 != '|' ||
      c4 != '|' ||
      c5 != '|')
    {
      is.setstate (std::ios_base::failbit);
    }
  return is;
}


} // namespace ns3
