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
#include "static-mobility-model.h"

namespace ns3 {

const ClassId StaticMobilityModel::cid = MakeClassId<StaticMobilityModel> ("StaticMobilityModel", 
                                                                           MobilityModel::iid);
  
StaticMobilityModel::StaticMobilityModel ()
{
  SetInterfaceId (StaticMobilityModel::iid);
}
StaticMobilityModel::StaticMobilityModel (const Position &position)
  : m_position (position)
{
  SetInterfaceId (StaticMobilityModel::iid);
}
StaticMobilityModel::~StaticMobilityModel ()
{}

Position
StaticMobilityModel::DoGet (void) const
{
  return m_position;
}
void 
StaticMobilityModel::DoSet (const Position &position)
{
  m_position = position;
  NotifyCourseChange ();
}
Speed 
StaticMobilityModel::DoGetSpeed (void) const
{
  return Speed ();
}

}; // namespace ns3
