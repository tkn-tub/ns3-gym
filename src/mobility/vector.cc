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
#include "vector.h"
#include "ns3/fatal-error.h"
#include <cmath>
#include <sstream>

namespace ns3 {


Vector::Vector (double _x, double _y, double _z)
  : x (_x),
    y (_y),
    z (_z)
{}

Vector::Vector ()
  : x (0.0),
    y (0.0),
    z (0.0)
{}

Vector::Vector (PValue value)
{
  *this = ClassValueHelperExtractFrom<Vector,VectorValue> (value);
}
Vector::operator PValue () const
{
  return ClassValueHelperConvertTo<Vector,VectorValue> (this);
}


double 
CalculateDistance (const Vector &a, const Vector &b)
{
  double dx = b.x - a.x;
  double dy = b.y - a.y;
  double dz = b.z - a.z;
  double distance = std::sqrt (dx * dx + dy * dy + dz * dz);
  return distance;
}

std::ostream &operator << (std::ostream &os, const Vector &vector)
{
  os << vector.x << ":" << vector.y << ":" << vector.z;
  return os;
}
std::istream &operator >> (std::istream &is, Vector &vector)
{
  char c1, c2;
  is >> vector.x >> c1 >> vector.y >> c2 >> vector.z;
  if (c1 != ':' ||
      c2 != ':')
    {
      is.setstate (std::ios_base::failbit);
    }
  return is;
}

} // namespace ns3
