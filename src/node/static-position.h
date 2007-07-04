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
#ifndef STATIC_POSITION_H
#define STATIC_POSITION_H

#include "ns3/component-manager.h"
#include "position.h"

namespace ns3 {

/**
 * \brief a position model for which the current position does not
 *        change once it has been set and until it is set again 
 *        explicitely to a new value.
 */
class StaticMobilityModel : public MobilityModel 
{
public:
  static const InterfaceId iid;
  static const ClassId cid;
  /**
   * Create a position located at coordinates (0,0,0)
   */
  StaticMobilityModel ();
  /**
   * \param x x coordinate
   * \param y y coordinate
   *
   * Create a position located at coordinates (x,y,0).
   * Unit is meters
   */
  StaticMobilityModel (double x, double y);
  /**
   * \param x x coordinate
   * \param y y coordinate
   * \param z z coordinate
   *
   * Create a position located at coordinates (x,y,z).
   * Unit is meters
   */
  StaticMobilityModel (double x, double y, double z);
  virtual ~StaticMobilityModel ();

private:
  virtual void DoGet (double &x, double &y, double &z) const;
  virtual void DoSet (double x, double y, double z);
  double m_x;
  double m_y;
  double m_z;
};

}; // namespace ns3

#endif /* STATIC_POSITION_H */
