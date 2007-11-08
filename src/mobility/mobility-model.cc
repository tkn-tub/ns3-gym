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
MobilityModel::GetPosition (void) const
{
  return DoGetPosition ();
}
Speed 
MobilityModel::GetSpeed (void) const
{
  return DoGetSpeed ();
}

void 
MobilityModel::SetPosition (const Position &position)
{
  DoSetPosition (position);
}

double 
MobilityModel::GetDistanceFrom (Ptr<const MobilityModel> other) const
{
  Position oPosition = other->DoGetPosition ();
  Position position = DoGetPosition ();
  return CalculateDistance (position, oPosition);
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
