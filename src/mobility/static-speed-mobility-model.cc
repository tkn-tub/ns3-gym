/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006, 2007 INRIA
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
#include "static-speed-mobility-model.h"
#include "ns3/simulator.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (StaticSpeedMobilityModel);

TypeId StaticSpeedMobilityModel::iid (void)
{
  static TypeId iid = TypeId ("StaticSpeedMobilityModel")
    .SetParent<MobilityModel> ()
    .AddConstructor<StaticSpeedMobilityModel> ()
    .AddConstructor<StaticSpeedMobilityModel,const Vector &> ()
    .AddConstructor<StaticSpeedMobilityModel,const Vector &,const Vector &> ();
  return iid;
}

StaticSpeedMobilityModel::StaticSpeedMobilityModel ()
{}
StaticSpeedMobilityModel::StaticSpeedMobilityModel (const Vector &position)
  : m_helper (position)
{}
StaticSpeedMobilityModel::StaticSpeedMobilityModel (const Vector &position,
                                                    const Vector &speed)
  : m_helper (position, speed)
{}

StaticSpeedMobilityModel::~StaticSpeedMobilityModel ()
{}

void 
StaticSpeedMobilityModel::SetSpeed (const Vector &speed)
{
  m_helper.SetSpeed (speed);
  NotifyCourseChange ();
}


Vector
StaticSpeedMobilityModel::DoGetPosition (void) const
{
  return m_helper.GetCurrentPosition ();
}
void 
StaticSpeedMobilityModel::DoSetPosition (const Vector &position)
{
  m_helper.InitializePosition (position);
  NotifyCourseChange ();
}
Vector
StaticSpeedMobilityModel::DoGetVelocity (void) const
{
  return m_helper.GetVelocity ();
}

}; // namespace ns3
