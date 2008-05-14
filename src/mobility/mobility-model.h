/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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

#include "vector.h"

#include "ns3/object.h"
#include "ns3/traced-callback.h"

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
  static TypeId GetTypeId (void);
  MobilityModel ();
  virtual ~MobilityModel () = 0;

  /**
   * \returns the current position
   */
  Vector GetPosition (void) const;
  /**
   * \param position the position to set.
   */
  void SetPosition (const Vector &position);
  /**
   * \returns the current velocity.
   */
  Vector GetVelocity (void) const;
  /**
   * \param position a reference to another mobility model
   * \returns the distance between the two objects. Unit is meters.
   */
  double GetDistanceFrom (Ptr<const MobilityModel> position) const;
protected:
  /**
   * Must be invoked by subclasses when the course of the
   * position changes to notify course change listeners.
   */
  void NotifyCourseChange (void) const;
private:
  /**
   * \returns the current position.
   *
   * Concrete subclasses of this base class must 
   * implement this method.
   */
  virtual Vector DoGetPosition (void) const = 0;
  /**
   * \param position the position to set.
   *
   * Concrete subclasses of this base class must 
   * implement this method.
   */
  virtual void DoSetPosition (const Vector &position) = 0;
  /**
   * \returns the current velocity.
   *
   * Concrete subclasses of this base class must 
   * implement this method.
   */
  virtual Vector DoGetVelocity (void) const = 0;

  /**
   * Used to alert subscribers that a change in direction, velocity,
   * or position has occurred.
   */
  TracedCallback<Ptr<const MobilityModel> > m_trace;

};

}; // namespace ns3

#endif /* MOBILITY_MODEL_H */
