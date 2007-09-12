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

const InterfaceId StaticSpeedMobilityModel::iid = 
  MakeInterfaceId ("StaticSpeedMobilityModel", MobilityModel::iid);
const ClassId StaticSpeedMobilityModel::cid = 
  MakeClassId<StaticSpeedMobilityModel> ("StaticSpeedMobilityModel", 
                                         StaticSpeedMobilityModel::iid);


StaticSpeedMobilityModel::StaticSpeedMobilityModel ()
{
  SetInterfaceId (StaticSpeedMobilityModel::iid);
}
StaticSpeedMobilityModel::StaticSpeedMobilityModel (const Position &position)
  : m_helper (position)
{
  SetInterfaceId (StaticSpeedMobilityModel::iid);
}
StaticSpeedMobilityModel::StaticSpeedMobilityModel (const Position &position,
                                                    const Speed &speed)
  : m_helper (position, speed)
{
  SetInterfaceId (StaticSpeedMobilityModel::iid);
}

StaticSpeedMobilityModel::~StaticSpeedMobilityModel ()
{}

void 
StaticSpeedMobilityModel::SetSpeed (const Speed speed)
{
  m_helper.SetSpeed (speed);
  NotifyCourseChange ();
}


Position
StaticSpeedMobilityModel::DoGet (void) const
{
  return m_helper.GetCurrentPosition ();
}
void 
StaticSpeedMobilityModel::DoSet (const Position &position)
{
  m_helper.InitializePosition (position);
  NotifyCourseChange ();
}
Speed 
StaticSpeedMobilityModel::DoGetSpeed (void) const
{
  return m_helper.GetSpeed ();
}

}; // namespace ns3
