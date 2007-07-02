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
#include "position.h"
#include "position-set-notifier.h"
#include <math.h>

namespace ns3 {

const InterfaceId Position::iid = MakeInterfaceId ("Position", Object::iid);

Position::Position ()
{
  SetInterfaceId (Position::iid);
}

Position::~Position ()
{}

void
Position::Get (double &x, double &y, double &z) const
{
  DoGet (x,y,z);
}
double 
Position::GetX (void) const
{
  double x,y,z;
  DoGet (x,y,z);
  return x;
}
double 
Position::GetY (void) const
{
  double x, y, z;
  DoGet (x,y,z);
  return y;
}
double 
Position::GetZ (void) const
{
  double x, y, z;
  DoGet (x,y,z);
  return z;
}

void 
Position::Set (double x, double y, double z)
{
  DoSet (x, y, z);
}
void 
Position::SetXY (double x, double y)
{
  double currentX, currentY, currentZ;
  DoGet (currentX, currentY, currentZ);
  DoSet (x, y, currentZ);
 }
void 
Position::SetX (double x)
{
  double currentX, currentY, currentZ;
  DoGet (currentX, currentY, currentZ);
  DoSet (x, currentY, currentZ);
}
void 
Position::SetY (double y)
{
  double currentX, currentY, currentZ;
  DoGet (currentX, currentY, currentZ);
  DoSet (currentX, y, currentZ);
}
void 
Position::SetZ (double z)
{
  double currentX, currentY, currentZ;
  DoGet (currentX, currentY, currentZ);
  DoSet (currentX, currentY, z);
}


double 
Position::GetDistanceFrom (const Position &position) const
{
  double ox,oy,oz;
  double x,y,z;
  position.DoGet (ox,oy,oz);
  DoGet (x,y,z);
  double dx = ox - x;
  double dy = oy - y;
  double dz = oz - z;
  return sqrt (dx*dx+dy*dy+dz*dz);
}

void
Position::NotifyCourseChange (void) const
{
  Ptr<PositionSetNotifier> notifier = 
    QueryInterface<PositionSetNotifier> (PositionSetNotifier::iid);
  if (notifier != 0)
    {
      notifier->Notify (this);
    }
}

} // namespace ns3
