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
#include "position.h"
#include "speed.h"

namespace ns3 {

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
   * \returns the current position
   * Unit is meters
   */
  Position Get (void) const;
  void Set (double x, double y, double z);
  void Set (const Position &position);
  Speed GetSpeed (void) const;
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
  virtual Position DoGet (void) const = 0;
  virtual void DoSet (const Position &position) = 0;
  virtual Speed DoGetSpeed (void) const = 0;
};

}; // namespace ns3

#endif /* MOBILITY_MODEL_H */
