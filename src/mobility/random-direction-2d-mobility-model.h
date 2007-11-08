/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#ifndef RANDOM_DIRECTION_MOBILITY_MODEL_H
#define RANDOM_DIRECTION_MOBILITY_MODEL_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/component-manager.h"
#include "ns3/rectangle.h"
#include "mobility-model.h"
#include "static-speed-helper.h"

namespace ns3 {

class RandomVariable;

/**
 * \brief the parameters to control a RandomDirection mobility model.
 */
class RandomDirection2dMobilityModelParameters : public Object
{
 public:
  /**
   * Create a default parameter object from Bind default values.
   */
  RandomDirection2dMobilityModelParameters ();
  /**
   * \param bounds the 2d bounds of the mobility model
   * \param speedVariable the random variable used to pick a random speed
   * \param pauseVariable the random variable used to pick a random pause delay
   */
  RandomDirection2dMobilityModelParameters (const Rectangle &bounds,
                               const RandomVariable &speedVariable,
                               const RandomVariable &pauseVariable);
  virtual ~RandomDirection2dMobilityModelParameters ();

  /**
   * \param speedVariable the random variable used to pick a random speed.
   */
  void SetSpeed (const RandomVariable &speedVariable);
  /**
   * \param pauseVariable the random variable used to pick a random pause delay.
   */
  void SetPause (const RandomVariable &pauseVariable);
  /**
   * \param bounds the 2d bounds of the mobility model.
   */
  void SetBounds (const Rectangle &bounds);
 private:
  friend class RandomDirection2dMobilityModel;

  static Ptr<RandomDirection2dMobilityModelParameters> GetCurrent (void);

  Rectangle m_bounds;
  RandomVariable *m_speedVariable;
  RandomVariable *m_pauseVariable;
};

/**
 * \brief a RandomDirection mobility model
 *
 * The movement of objects is based on random directions: each object
 * pauses for a specific delay, chooses a random direction and speed and 
 * then travels in the specific direction until it reaches one of
 * the boundaries of the model. When it reaches the boundary, it pauses,
 * selects a new direction and speed, aso.
 */
class RandomDirection2dMobilityModel : public MobilityModel
{
 public:
  static const ClassId cid;

  /**
   * Create a RandomDirection model from the default Bind values.
   */
  RandomDirection2dMobilityModel ();
  /**
   * \param parameters the parameters which control the behavior of the model.
   * Create a RandomDirection model using the parameters specified.
   */
  RandomDirection2dMobilityModel (Ptr<RandomDirection2dMobilityModelParameters> parameters);
 private:
  void Start (void);
  void ResetDirectionAndSpeed (void);
  void BeginPause (void);
  void SetDirectionAndSpeed (double direction);
  void InitializeDirectionAndSpeed (void);
  virtual void DoDispose (void);
  virtual Vector DoGetPosition (void) const;
  virtual void DoSetPosition (const Vector &position);
  virtual Vector DoGetVelocity (void) const;

  static const double PI;
  Ptr<RandomDirection2dMobilityModelParameters> m_parameters;
  EventId m_event;
  StaticSpeedHelper m_helper;
};

} // namespace ns3

#endif /* RANDOM_DIRECTION_MOBILITY_MODEL_H */
