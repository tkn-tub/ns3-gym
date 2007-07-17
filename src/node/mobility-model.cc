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


Position::Position (double _x, double _y, double _z)
  : x (_x),
    y (_y),
    z (_z)
{}

MobilityModel::MobilityModel ()
{
  SetInterfaceId (MobilityModel::iid);
}

MobilityModel::~MobilityModel ()
{}

Position
MobilityModel::Get (void) const
{
  return DoGet ();
}

void 
MobilityModel::Set (double x, double y, double z)
{
  Position position (x, y, z);
  DoSet (position);
}
void 
MobilityModel::Set (const Position &position)
{
  DoSet (position);
}

double 
MobilityModel::GetDistanceFrom (const MobilityModel &other) const
{
  Position oPosition = other.DoGet ();
  Position position = DoGet ();
  double dx = oPosition.x - position.x;
  double dy = oPosition.y - position.y;
  double dz = oPosition.z - position.z;
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
