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
  const RectangleValue *v = value.DynCast<const RectangleValue *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("expecting value of type Rectangle.");
    }
  *this = v->Get ();
}
Rectangle::operator PValue () const
{
  return PValue::Create<RectangleValue> (*this);
}



RectangleValue::RectangleValue (const Rectangle &rectangle)
  : m_rectangle (rectangle)
{}

void 
RectangleValue::Set (const Rectangle &v)
{
  m_rectangle = v;
}
Rectangle 
RectangleValue::Get (void) const
{
  return m_rectangle;
}

PValue
RectangleValue::Copy (void) const
{
  return PValue::Create<RectangleValue> (*this);
}
std::string 
RectangleValue::SerializeToString (Ptr<const ParamSpec> spec) const
{
  std::ostringstream oss;
  oss << m_rectangle.xMin << "|" << m_rectangle.xMax << "|" << m_rectangle.yMin << "|" << m_rectangle.yMax;
  return oss.str ();
}
void
RectangleValue::ReadAsDouble (std::string value, double &v, bool &ok)
{
  std::istringstream iss;
  iss.str (value);
  double local;
  iss >> local;
  bool good = !iss.bad () && !iss.fail ();
  if (good)
    {
      v = local;
    }
  else
    {
      ok = false;
    }
}
bool 
RectangleValue::DeserializeFromString (std::string value, Ptr<const ParamSpec> spec)
{
  bool ok = true;
  std::istringstream iss;
  iss.str (value);
  std::string::size_type cur = 0, next = 0;
  next = value.find ("|", cur);
  if (next == std::string::npos)
    {
      return false;
    }
  ReadAsDouble (value.substr (cur, next-cur), m_rectangle.xMin, ok);
  cur = next + 1;
  next = value.find ("|", cur);
  if (next == std::string::npos)
    {
      return false;
    }
  ReadAsDouble (value.substr (cur, next-cur), m_rectangle.xMax, ok);
  cur = next + 1;
  next = value.find ("|", cur);
  if (next == std::string::npos)
    {
      return false;
    }
  ReadAsDouble (value.substr (cur, next-cur), m_rectangle.yMin, ok);
  cur = next + 1;
  ReadAsDouble (value.substr (cur, value.size ()-cur), m_rectangle.yMax, ok);
  return ok;
}

RectangleValue::RectangleValue (PValue value)
  : m_rectangle (value)
{}
RectangleValue::operator PValue () const
{
  return m_rectangle;
}


} // namespace ns3
