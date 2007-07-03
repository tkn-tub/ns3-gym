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
#ifndef RANDOM_DIRECTION_POSITION_H
#define RANDOM_DIRECTION_POSITION_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/component-manager.h"
#include "position.h"

namespace ns3 {

class RandomVariable;

class RandomDirectionParameters : public Object
{
 public:
  RandomDirectionParameters ();
  RandomDirectionParameters (double xMin, double xMax, double yMin, double yMax);
  RandomDirectionParameters (double xMin, double xMax, double yMin, double yMax,
			     const RandomVariable &speedVariable,
			     const RandomVariable &pauseVariable);
  virtual ~RandomDirectionParameters ();

  void SetSpeed (const RandomVariable &speedVariable);
  void SetPause (const RandomVariable &pauseVariable);
  void SetBounds (double xMin, double xMax, double yMin, double yMax);
 private:
  friend class RandomDirectionPosition;
  double m_xMin;
  double m_xMax;
  double m_yMin;
  double m_yMax;
  RandomVariable *m_speedVariable;
  RandomVariable *m_pauseVariable;
  std::string m_speedVariableValue;
  std::string m_pauseVariableValue;
};

class RandomDirectionPosition : public Position
{
 public:
  static const InterfaceId iid;
  static const ClassId cid;

  RandomDirectionPosition ();
  RandomDirectionPosition (double x, double y);
  RandomDirectionPosition (Ptr<RandomDirectionParameters> parameters);
  RandomDirectionPosition (Ptr<RandomDirectionParameters> parameters, 
			   double x, double y);
 private:
  enum Side {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
  };
  static Ptr<RandomDirectionParameters> GetDefaultParameters (void);
  void ResetDirectionAndSpeed (void);
  void SetDirectionAndSpeed (double direction);
  void InitializeDirectionAndSpeed (void);
  void Update (void) const;
  bool CheckPosition (void) const;
  enum RandomDirectionPosition::Side CalculateIntersection (double &x, double &y);
  virtual void DoDispose (void);
  virtual void DoGet (double &x, double &y, double &z) const;
  virtual void DoSet (double x, double y, double z);

  static const double PI;
  Ptr<RandomDirectionParameters> m_parameters;
  mutable double m_x;
  mutable double m_y;
  double m_dx;
  double m_dy;
  mutable Time m_prevTime;
  Time m_pauseStart;
  EventId m_event;
  enum Side m_side;
};

} // namespace ns3

#endif /* RANDOM_DIRECTION_POSITION_H */
