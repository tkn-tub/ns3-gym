/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
#ifndef MOBILITY_MODEL_H
#define MOBILITY_MODEL_H

#include "ns3/object.h"

namespace ns3 {

class Vector3D
{
public:
  double x;
  double y;
  double z;
};
class Position : public Vector3D
{};
class Speed : public Vector3D
{};
class Vector2D
{
public:
  double x;
  double y;
};
class Position2D : public Vector2D
{};
class Speed2D : public Vector2D
{};

/**
 * \brief keep track of the current position of an object
 *
 * All space coordinates in this class and its subclasses are
 * understood to be meters or meters/s. i.e., they are all
 * metric international units.
 */
class MobilityModel : public Object
{
public:
  static const InterfaceId iid;
  MobilityModel ();
  virtual ~MobilityModel () = 0;

  /**
   * \param x reference to floating-point variable for x coordinate.
   * \param y reference to floating-point variable for y coordinate.
   * \param z reference to floating-point variable for z coordinate.
   *
   * Store in the x, y, and z variables the current coordinates
   * managed by this position object.
   * Unit is meters
   */
  void Get (double &x, double &y, double &z) const;
  Position Get (void) const;
  /**
   * \returns the current x coordinate
   *
   * Unit is meters
   */
  double GetX (void) const;
  /**
   * \returns the current y coordinate
   *
   * Unit is meters
   */
  double GetY (void) const;
  /**
   * \returns the current z coordinate
   *
   * Unit is meters
   */
  double GetZ (void) const;

  void Add (double dx, double dy, double dz);

  void Set (double x, double y, double z);
  void Set (const Position &position);
  void SetXY (double x, double y);
  void SetX (double x);
  void SetY (double y);
  void SetZ (double z);
  /**
   * \param position a reference to another position object instance
   * \returns the distance between the two objects.
   *
   * Unit is meters
   */
  double GetDistanceFrom (const MobilityModel &position) const;
protected:
  /**
   * Must be invoked by subclasses when the course of the
   * position changes to notify course change listeners.
   */
  void NotifyCourseChange (void) const;
private:
  /**
   * \param x reference to floating-point variable for x coordinate.
   * \param y reference to floating-point variable for y coordinate.
   * \param z reference to floating-point variable for z coordinate.
   *
   * Store in the x, y, and z variables the current coordinates
   * managed by this position object. Concrete subclasses of this 
   * base class must implement this method.
   * Unit is meters
   */
  virtual void DoGet (double &x, double &y, double &z) const = 0;
  virtual void DoSet (double x, double y, double z) = 0;
};

}; // namespace ns3

#endif /* MOBILITY_MODEL_H */
