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

TypeId 
MobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("MobilityModel")
    .SetParent<Object> ()
    .AddParameter ("position", "The current position of the mobility model.",
                   TypeId::ATTR_SGC,
                   Vector (0.0, 0.0, 0.0),
                   MakeVectorParamSpec (&MobilityModel::SetPosition,
                                        &MobilityModel::GetPosition),
                   MakeVectorChecker ())
    .AddParameter ("velocity", "The current velocity of the mobility model.",
                   TypeId::ATTR_GET,
                   Vector (0.0, 0.0, 0.0), // ignored initial value.
                   MakeVectorParamSpec (&MobilityModel::GetVelocity),
                   MakeVectorChecker ())
    ;
  return tid;
}

MobilityModel::MobilityModel ()
{}

MobilityModel::~MobilityModel ()
{}

Vector
MobilityModel::GetPosition (void) const
{
  return DoGetPosition ();
}
Vector
MobilityModel::GetVelocity (void) const
{
  return DoGetVelocity ();
}

void 
MobilityModel::SetPosition (const Vector &position)
{
  DoSetPosition (position);
}

double 
MobilityModel::GetDistanceFrom (Ptr<const MobilityModel> other) const
{
  Vector oPosition = other->DoGetPosition ();
  Vector position = DoGetPosition ();
  return CalculateDistance (position, oPosition);
}

void
MobilityModel::NotifyCourseChange (void) const
{
  Ptr<MobilityModelNotifier> notifier = GetObject<MobilityModelNotifier> ();
  if (notifier != 0)
    {
      notifier->Notify (this);
    }
}

} // namespace ns3
