/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006, 2007 INRIA
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
#ifndef STATIC_SPEED_MOBILITY_MODEL_H
#define STATIC_SPEED_MOBILITY_MODEL_H

#include <stdint.h>
#include "mobility-model.h"
#include "ns3/nstime.h"
#include "ns3/component-manager.h"

namespace ns3 {

class StaticSpeedMobilityModel : public MobilityModel 
{
public:
  static const InterfaceId iid;
  static const ClassId cid;
  /**
   * Create position located at coordinates (0,0,0) with
   * speed (0,0,0).
   */
  StaticSpeedMobilityModel ();
  /**
   * \param x x coordinate
   * \param y y coordinate
   *
   * Create a position located at coordinates (x,y,0) with
   * speed (0,0,0).
   * Unit is meters
   */
  StaticSpeedMobilityModel (double x, double y);
  /**
   * \param x x coordinate
   * \param y y coordinate
   * \param z z coordinate
   *
   * Create a position located at coordinates (x,y,z) with
   * speed (0,0,0).
   * Unit is meters
   */
  StaticSpeedMobilityModel (double x, double y, double z);
  /**
   * \param x x coordinate
   * \param y y coordinate
   * \param z z coordinate
   * \param dx x coordinate speed
   * \param dy y coordinate speed
   * \param dz z coordinate speed
   *
   * Create a position located at coordinates (x,y,z) with
   * speed (dx,dy,dz).
   * Unit is meters and meters/s
   */
  StaticSpeedMobilityModel (double x, double y, double z,
                       double dx, double dy, double dz);
  virtual ~StaticSpeedMobilityModel ();

  /*
   * \param dx x coordinate speed
   * \param dy y coordinate speed
   * \param dz z coordinate speed
   *
   * Set the current speed now to (dx,dy,dz)
   * Unit is meters/s
   */
  void SetSpeed (double dx, double dy, double dz);
private:
  virtual void DoGet (double &x, double &y, double &z) const;
  virtual void DoSet (double x, double y, double z);
  void Update (void) const;
  mutable double m_x;
  mutable double m_y;
  mutable double m_z;
  double m_dx;
  double m_dy;
  double m_dz;
  mutable Time m_prevTime;
};

}; // namespace ns3

#endif /* STATIC_SPEED_POSITION */
