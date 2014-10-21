/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#ifndef NS3_VECTOR_H
#define NS3_VECTOR_H

#include "attribute.h"
#include "attribute-helper.h"

namespace ns3 {

/**
 * \brief a 3d vector
 * \see attribute_Vector3D
 */
class Vector3D
{
public:
  /**
   * \param _x x coordinate of vector
   * \param _y y coordinate of vector
   * \param _z z coordinate of vector
   *
   * Create vector (_x, _y, _z)
   */
  Vector3D (double _x, double _y, double _z);
  /**
   * Create vector (0.0, 0.0, 0.0)
   */
  Vector3D ();
  /**
   * x coordinate of vector
   */
  double x;
  /**
   * y coordinate of vector
   */
  double y;
  /**
   * z coordinate of vector
   */
  double z;
  
  friend double CalculateDistance (const Vector3D &a, const Vector3D &b);
  friend std::ostream &operator << (std::ostream &os, const Vector3D &vector);
  friend std::istream &operator >> (std::istream &is, Vector3D &vector);
};

/**
 * \brief a 2d vector
 * \see attribute_Vector2D
 */
class Vector2D
{
public:
  /**
   * \param _x x coordinate of vector
   * \param _y y coordinate of vector
   *
   * Create vector (_x, _y)
   */
  Vector2D (double _x, double _y);
  /**
   * Create vector vector (0.0, 0.0)
   */
  Vector2D ();
  /**
   * x coordinate of vector
   */
  double x;
  /**
   * y coordinate of vector
   */
  double y;

  friend double CalculateDistance (const Vector2D &a, const Vector2D &b);
  friend std::ostream &operator << (std::ostream &os, const Vector2D &vector);
  friend std::istream &operator >> (std::istream &is, Vector2D &vector);
};

/**
 * \param a one point
 * \param b another point
 * \returns the cartesian distance between a and b.
 */
double CalculateDistance (const Vector3D &a, const Vector3D &b);
/**
 * \param a one point
 * \param b another point
 * \returns the cartesian distance between a and b.
 */
double CalculateDistance (const Vector2D &a, const Vector2D &b);

ATTRIBUTE_HELPER_HEADER (Vector3D);
ATTRIBUTE_HELPER_HEADER (Vector2D);

/**
 * Output streamer.
 *
 * Vectors are written as "x:y:z".
 *
 * \param [in,out] os The stream.
 * \param [in] vector The vector to stream
 * \return The stream.
 */
std::ostream &operator << (std::ostream &os, const Vector3D &vector);
/**
 * Output streamer.
 *
 * Vectors are written as "x:y".
 *
 * \param [in,out] os The stream.
 * \param [in] vector The vector to stream
 * \return The stream.
 */
std::ostream &operator << (std::ostream &os, const Vector2D &vector);

/**
 * Input streamer.
 *
 * Vectors are expected to be in the form "x:y:z".
 *
 * \param [in,out] is The stream.
 * \param [in] vector The vector.
 * \returns The stream.
 */
std::istream &operator >> (std::istream &is, Vector3D &vector);
/**
 * Input streamer.
 *
 * Vectors are expected to be in the form "x:y".
 *
 * \param [in,out] is The stream.
 * \param [in] vector The vector.
 * \returns The stream.
 */
std::istream &operator >> (std::istream &is, Vector2D &vector);


/**
 * \relates Vector3D
 * Vector alias typedef for compatibility with mobility models
 */
typedef Vector3D Vector;
/** 
 * \ingroup attribute_Vector3D
 * Vector alias typedef for compatibility with mobility models
 */
typedef Vector3DValue VectorValue;
/** 
 * \ingroup attribute_Vector3D
 * Vector alias typedef for compatibility with mobility models
 */
typedef Vector3DChecker VectorChecker;


// Document these by hand so they go in group attribute_Vector3D  
/**
 * \ingroup attribute_Vector3D
 * \fn ns3::Ptr<const ns3::AttributeAccessor> ns3::MakeVectorAccessor (T1 a1)
 * \copydoc ns3::MakeAccessorHelper(T1)
 * \see AttributeAccessor
 */
/**
 * \ingroup attribute_Vector3D
 * \fn ns3::Ptr<const ns3::AttributeAccessor> ns3::MakeVectorAccessor (T1 a1, T2 a2)
 * \copydoc ns3::MakeAccessorHelper(T1,T2)
 * \see AttributeAccessor
 */

ATTRIBUTE_ACCESSOR_DEFINE (Vector);

/**
 * \ingroup attribute_Vector3D
 * \returns The AttributeChecker.
 * \see AttributeChecker
 */
Ptr<const AttributeChecker> MakeVectorChecker (void);  

} // namespace ns3

#endif /* NS3_VECTOR_H */
