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
  const VectorValue *v = value.DynCast<const VectorValue *> ();
  if (v == 0)
    {
      NS_FATAL_ERROR ("Expected value of type Vector");
    }
  *this = v->Get ();
}
Vector::operator PValue () const
{
  return PValue::Create<VectorValue> (*this);
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



VectorValue::VectorValue (const Vector &vector)
  : m_vector (vector)
{}

void 
VectorValue::Set (const Vector &vector)
{
  m_vector = vector;
}
Vector 
VectorValue::Get (void) const
{
  return m_vector;
}

PValue
VectorValue::Copy (void) const
{
  return PValue::Create<VectorValue> (*this);
}
std::string 
VectorValue::SerializeToString (Ptr<const ParamSpec> spec) const
{
  std::ostringstream oss;
  oss << m_vector.x << ":" << m_vector.y << ":" << m_vector.z;
  return oss.str ();
}
bool 
VectorValue::DeserializeFromString (std::string value, Ptr<const ParamSpec> spec)
{
  // XXX implement parsing
  return false;
}

VectorValue::VectorValue (PValue value)
  : m_vector (value)
{}
VectorValue::operator PValue () const
{
  return m_vector;
}


} // namespace ns3
