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
#ifndef VECTOR_H
#define VECTOR_H

#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"

namespace ns3 {

/**
 * \brief a 3d cartesian position vector
 *
 * Unit is meters.
 */
class Vector
{
public:
  /**
   * \param _x x coordinate of vector vector
   * \param _y y coordinate of vector vector
   * \param _z z coordinate of vector vector
   *
   * Create vector vector (_x, _y, _z)
   */
  Vector (double _x, double _y, double _z);
  /**
   * Create vector vector (0.0, 0.0, 0.0)
   */
  Vector ();
  /**
   * x coordinate of vector vector
   */
  double x;
  /**
   * y coordinate of vector vector
   */
  double y;
  /**
   * z coordinate of vector vector
   */
  double z;
};

double CalculateDistance (const Vector &a, const Vector &b);

/**
 * \class ns3::VectorValue
 * \brief hold objects of type ns3::Vector
 */

ATTRIBUTE_HELPER_HEADER_2 (Vector);

std::ostream &operator << (std::ostream &os, const Vector &vector);
std::istream &operator >> (std::istream &is, Vector &vector);

} // namespace ns3

#endif /* VECTOR_H */
