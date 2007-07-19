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

class RandomDirectionParameters : public Object
{
 public:
  RandomDirectionParameters ();
  RandomDirectionParameters (const Rectangle &bounds,
			     const RandomVariable &speedVariable,
			     const RandomVariable &pauseVariable);
  virtual ~RandomDirectionParameters ();

  void SetSpeed (const RandomVariable &speedVariable);
  void SetPause (const RandomVariable &pauseVariable);
  void SetBounds (const Rectangle &bounds);
 private:
  friend class RandomDirectionMobilityModel;

  static Ptr<RandomDirectionParameters> GetCurrent (void);

  Rectangle m_bounds;
  RandomVariable *m_speedVariable;
  RandomVariable *m_pauseVariable;
};

class RandomDirectionMobilityModel : public MobilityModel
{
 public:
  static const InterfaceId iid;
  static const ClassId cid;

  RandomDirectionMobilityModel ();
  RandomDirectionMobilityModel (Ptr<RandomDirectionParameters> parameters);
 private:
  void Start (void);
  void ResetDirectionAndSpeed (void);
  void SetDirectionAndSpeed (double direction);
  void InitializeDirectionAndSpeed (void);
  virtual void DoDispose (void);
  virtual Position DoGet (void) const;
  virtual void DoSet (const Position &position);

  static const double PI;
  Ptr<RandomDirectionParameters> m_parameters;
  EventId m_event;
  StaticSpeedHelper m_helper;
};

} // namespace ns3

#endif /* RANDOM_DIRECTION_MOBILITY_MODEL_H */
