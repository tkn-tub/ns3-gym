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
#include "mobility-model.h"
#include "mobility-model-notifier.h"
#include <math.h>

namespace ns3 {

const InterfaceId MobilityModel::iid = MakeInterfaceId ("MobilityModel", Object::iid);

MobilityModel::MobilityModel ()
{
  SetInterfaceId (MobilityModel::iid);
}

MobilityModel::~MobilityModel ()
{}

Position
MobilityModel::Get (void) const
{
  Position position;
  DoGet (position.x,position.y,position.z);
  return position;
}
double 
MobilityModel::GetX (void) const
{
  double x,y,z;
  DoGet (x,y,z);
  return x;
}
double 
MobilityModel::GetY (void) const
{
  double x, y, z;
  DoGet (x,y,z);
  return y;
}
double 
MobilityModel::GetZ (void) const
{
  double x, y, z;
  DoGet (x,y,z);
  return z;
}

void 
MobilityModel::Set (double x, double y, double z)
{
  DoSet (x, y, z);
}
void 
MobilityModel::Set (const Position &position)
{
  DoSet (position.x, position.y, position.z);
}
void 
MobilityModel::SetXY (double x, double y)
{
  double currentX, currentY, currentZ;
  DoGet (currentX, currentY, currentZ);
  DoSet (x, y, currentZ);
 }
void 
MobilityModel::SetX (double x)
{
  double currentX, currentY, currentZ;
  DoGet (currentX, currentY, currentZ);
  DoSet (x, currentY, currentZ);
}
void 
MobilityModel::SetY (double y)
{
  double currentX, currentY, currentZ;
  DoGet (currentX, currentY, currentZ);
  DoSet (currentX, y, currentZ);
}
void 
MobilityModel::SetZ (double z)
{
  double currentX, currentY, currentZ;
  DoGet (currentX, currentY, currentZ);
  DoSet (currentX, currentY, z);
}


double 
MobilityModel::GetDistanceFrom (const MobilityModel &position) const
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
MobilityModel::NotifyCourseChange (void) const
{
  Ptr<MobilityModelNotifier> notifier = 
    QueryInterface<MobilityModelNotifier> (MobilityModelNotifier::iid);
  if (notifier != 0)
    {
      notifier->Notify (this);
    }
}

} // namespace ns3
