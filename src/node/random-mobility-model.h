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
#ifndef RANDOM_MOBILITY_MODEL_H
#define RANDOM_MOBILITY_MODEL_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/component-manager.h"
#include "mobility-model.h"
#include "mobility-model-helper.h"

namespace ns3 {

class RandomVariable;

class RandomMobilityModelParameters : public Object
{
 public:
  RandomMobilityModelParameters ();
  RandomMobilityModelParameters (double xMin, double xMax, double yMin, double yMax);
  RandomMobilityModelParameters (double xMin, double xMax, double yMin, double yMax,
                                 const RandomVariable &speedVariable);
  virtual ~RandomMobilityModelParameters ();

  void SetSpeed (const RandomVariable &speedVariable);
  void SetBounds (double xMin, double xMax, double yMin, double yMax);
  void SetWalkModelDistance (double maxDistance);
  void SetWalkModelDelay (Time maxDelay);
  void SetWaypointModelDistance (const RandomVariable &pauseVariable, double maxDistance);
  void SetWaypointModelDelay (const RandomVariable &pauseVariable, Time maxDelay);
  void SetDirectionModel (const RandomVariable &pauseVariable);
  void SetBoundlessModel (void);


  // made public because need to use it for default values.
  enum Type {
    WALK_DISTANCE,
    WALK_DELAY,
    WAYPOINT_DISTANCE,
    WAYPOINT_DELAY,
    DIRECTION,
    BOUNDLESS
  };
 private:
  void Initialize (void);
  friend class RandomMobilityModel;
  enum Type m_type;
  struct AreaBounds m_bounds;
  RandomVariable *m_speedVariable;
  RandomVariable *m_pauseVariable;
  double m_maxDistance;
  Time m_maxDelay;
};

class RandomMobilityModel : public MobilityModel
{
 public:
  static const InterfaceId iid;
  static const ClassId cid;

  RandomMobilityModel ();
  RandomMobilityModel (double x, double y);
  RandomMobilityModel (Ptr<RandomMobilityModelParameters> parameters);
  RandomMobilityModel (Ptr<RandomMobilityModelParameters> parameters, 
			   double x, double y);
 private:
  static Ptr<RandomMobilityModelParameters> GetDefaultParameters (void);
  void ResetDirectionAndSpeed (void);
  void SetDirectionAndSpeed (double direction);
  void InitializeDirectionAndSpeed (void);
  virtual void DoDispose (void);
  virtual Position DoGet (void) const;
  virtual void DoSet (const Position &position);

  static const double PI;
  Ptr<RandomMobilityModelParameters> m_parameters;
  EventId m_event;
  MobilityModelHelper m_helper;
};

} // namespace ns3

#endif /* RANDOM_MOBILITY_MODEL_H */
