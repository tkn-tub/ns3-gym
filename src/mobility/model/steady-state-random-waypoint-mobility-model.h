/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Denis Fakhriev <fakhriev@iitp.ru>
 */
#ifndef STEADY_STATE_RANDOM_WAYPOINT_MOBILITY_MODEL_H
#define STEADY_STATE_RANDOM_WAYPOINT_MOBILITY_MODEL_H

#include "constant-velocity-helper.h"
#include "mobility-model.h"
#include "position-allocator.h"
#include "ns3/ptr.h"
#include "ns3/random-variable.h"

namespace ns3 {

/**
 * \ingroup mobility
 * \brief Steady-state random waypoint mobility model.
 *
 * This model based on random waypoint mobility (RWM) model for case when 
 * speed, pause and position are uniformly distributed random variables. 
 * The difference is that the initial values of this parameters are not 
 * from uniform distribution but from stationary distribution of RWM model.
 * The implementation of this model is 2d-specific and with nonzero nodes speeds.
 *
 * Based on NS-2 implementation by Toilers Research Group -- Colorado
 *      School of Mines (http://toilers.mines.edu). 
 * The papers related to this code are:
 *      W. Navidi and T. Camp, Stationary Distributions for the Random
 *      Waypoint Mobility Model, IEEE Transactions on Mobile Computing,
 *      vol. 3, no. 1, pp. 99-108, January-March 2004.
 *      W. Navidi, T. Camp, and N. Bauer, Improving the Accuracy of
 *      Random Waypoint Simulations Through Steady-State Initialization,
 *      Proceedings of the 15th International Conference on Modeling and
 *      Simulation (MS '04), pp. 319-326, March 2004.
 */
class SteadyStateRandomWaypointMobilityModel : public MobilityModel
{
public:
  static TypeId GetTypeId (void);
  SteadyStateRandomWaypointMobilityModel ();
protected:
  virtual void DoStart (void);
private:
  void SteadyStateStart (void);
  void SteadyStateBeginWalk (const Vector &destination);
  void Start (void);
  void BeginWalk (void);
  virtual Vector DoGetPosition (void) const;
  virtual void DoSetPosition (const Vector &position);
  virtual Vector DoGetVelocity (void) const;

  ConstantVelocityHelper m_helper;
  double m_maxSpeed;
  double m_minSpeed;
  UniformVariable m_speed;
  double m_minX;
  double m_maxX;
  double m_minY;
  double m_maxY;
  Ptr<RandomRectanglePositionAllocator> m_position;
  double m_minPause;
  double m_maxPause;
  UniformVariable m_pause;
  EventId m_event;
  bool alreadyStarted;
};

} // namespace ns3

#endif /* STEADY_STATE_RANDOM_WAYPOINT_MOBILITY_MODEL_H */
