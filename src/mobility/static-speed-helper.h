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
#ifndef STATIC_SPEED_HELPER_H
#define STATIC_SPEED_HELPER_H

#include "ns3/nstime.h"
#include "vector.h"

namespace ns3 {

class Rectangle;

class StaticSpeedHelper
{
 public:
  StaticSpeedHelper ();
  StaticSpeedHelper (const Vector &position);
  StaticSpeedHelper (const Vector &position,
		     const Vector &speed);
  void InitializePosition (const Vector &position);

  void Reset (const Vector &speed);
  Vector GetCurrentPosition (const Rectangle &bounds) const;
  Vector GetCurrentPosition (void) const;
  Vector GetSpeed (void) const;
  void SetSpeed (const Vector &speed);
  void Pause (void);
  void Unpause (void);

 private:
  void Update (void) const;
  void UpdateFull (const Rectangle &rectangle) const;
  mutable Time m_lastUpdate;
  mutable Vector m_position;
  Vector m_speed;
  bool m_paused;
};

} // namespace ns3

#endif /* STATIC_SPEED_HELPER_H */
