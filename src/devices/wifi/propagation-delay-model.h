/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
#ifndef PROPAGATION_DELAY_MODEL_H
#define PROPAGATION_DELAY_MODEL_H

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/nstime.h"

namespace ns3 {

class MobilityModel;

class RandomVariable;

/**
 * \brief calculate a propagation delay.
 */
class PropagationDelayModel : public Object
{
public:
  virtual ~PropagationDelayModel ();
  /**
   * \param a the source
   * \param b the destination
   * \returns the calculated propagation delay
   *
   * Calculate the propagation delay between the specified
   * source and destination.
   */
  virtual Time GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const = 0;
  /**
   * \returns the default propagation loss model as specified
   * by the PropagationDelayModelType ns3::DefaultValue.
   */
  static Ptr<PropagationDelayModel> CreateDefault (void);
};

/**
 * \brief the propagation delay is random
 */
class RandomPropagationDelayModel : public PropagationDelayModel
{
public:
  /**
   * Use the default parameters from PropagationDelayRandomDistribution.
   */
  RandomPropagationDelayModel ();
  /**
   * \param variable the random distribution to use for this
   * instance
   */
  RandomPropagationDelayModel (const RandomVariable &variable);
  virtual ~RandomPropagationDelayModel ();
  virtual Time GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;
private:
  RandomVariable *m_variable;
};

/**
 * \brief the propagation speed is constant
 */
class ConstantSpeedPropagationDelayModel : public PropagationDelayModel
{
public:
  /**
   * Use the default parameters from PropagationDelayConstantSpeed.
   */
  ConstantSpeedPropagationDelayModel ();
  /**
   * \param speed the speed (m/s) of the propagation to use for this
   * instance
   */
  ConstantSpeedPropagationDelayModel (double speed);
  virtual Time GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;
  /**
   * \param speed the new speed (m/s)
   */
  void SetSpeed (double speed);
  /**
   * \returns the current propagation speed (m/s).
   */
  double GetSpeed (void) const;
private:
  double m_speed;
};

} // namespace ns3

#endif /* PROPAGATION_DELAY_MODEL_H */
