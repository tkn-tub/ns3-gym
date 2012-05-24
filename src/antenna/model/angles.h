/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011, 2012 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef ANGLES_H
#define ANGLES_H


#include <ns3/vector.h>

namespace ns3 {


/** 
 * \brief converts degrees to radians
 * 
 * \param degrees the angle in degrees
 * 
 * \return the angle in radians
 */
double DegreesToRadians (double degrees);

/** 
 * \brief converts radians to degrees
 * 
 * \param radians the angle in radians
 * 
 * \return the angle in degrees
 */
double RadiansToDegrees (double radians);

/** 
 * 
 * struct holding the azimuth and inclination angles of spherical
 * coordinates. The notation is the one used in  "Antenna Theory - Analysis
 * and Design", C.A. Balanis, Wiley, 2nd Ed., section 2.2 "Radiation
 * pattern".
 * This notation corresponds to the standard spherical coordinates, with phi
 * measured counterclockwise in the x-y plane off the x-axis, and
 * theta measured off the z-axis. 
 * 
 *          ^
 *        z | 
 *          |_ theta
 *          | \
 *          | /|
 *          |/ |   y
 *          +-------->
 *         /  \|
 *        /___/
 *     x /  phi
 *      |/
 *
 */
struct Angles
{
  /** 
   * default constructor, will initialize phi and theta to zero
   * 
   */
  Angles ();

  /** 
   * this constructor allows to specify phi and  theta
   * 
   * \param phi the azimuth angle in radians
   * \param theta the inclination angle in radians
   * 
   */
  Angles (double phi, double theta);

  /** 
   * this constructor will initialize phi and theta by converting the
   * given 3D vector from cartesian coordinates to spherical coordinates
   * 
   * \param v the 3D vector in cartesian coordinates
   * 
   */
  Angles (Vector v);

  /** 
   * this constructor initializes an Angles instance with the angles
   * of the spherical coordinates of point v respect to point o 
   * 
   * \param v the point (in cartesian coordinates) for which the angles are determined
   * \param o the origin (in cartesian coordinates) of the spherical coordinate system
   * 
   */
  Angles (Vector v, Vector o);

  /**
   * the azimuth angle in radians
   * 
   */
  double phi;

  /**
   * the inclination angle in radians
   * 
   */
  double theta;
};



/** 
 * print a struct Angles to output
 * 
 * \param os the output stream
 * \param a the Angles struct
 * 
 * \return a reference to the output stream
 */
std::ostream& operator<< ( std::ostream& os, const Angles& a);

/** 
 * initialize a struct Angles from input
 * 
 * \param is the input stream
 * \param a the Angles struct
 * 
 * \return a reference to the input stream
 */
std::istream &operator >> (std::istream &is, Angles &a);

} // namespace ns3

#endif // ANGLES_H
