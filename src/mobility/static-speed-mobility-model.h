/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006, 2007 INRIA
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
#include "static-speed-helper.h"
#include "speed.h"

namespace ns3 {

/**
 * \brief a position model for which the current speed does not
 *        change once it has been set and until it is set again 
 *        explicitely to a new value.
 */
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
   * Create a position located at coordinates (x,y,z) with
   * speed (0,0,0).
   */
  StaticSpeedMobilityModel (const Position &position);
  /**
   *
   * Create a position located at coordinates (x,y,z) with
   * speed (dx,dy,dz).
   * Unit is meters and meters/s
   */
  StaticSpeedMobilityModel (const Position &position,
                            const Speed &speed);
  virtual ~StaticSpeedMobilityModel ();

  /**
   * \param speed the new speed to set.
   *
   * Set the current speed now to (dx,dy,dz)
   * Unit is meters/s
   */
  void SetSpeed (const Speed speed);
private:
  virtual Position DoGetPosition (void) const;
  virtual void DoSetPosition (const Position &position);
  virtual Speed DoGetSpeed (void) const;
  void Update (void) const;
  StaticSpeedHelper m_helper;
};

}; // namespace ns3

#endif /* STATIC_SPEED_POSITION */
