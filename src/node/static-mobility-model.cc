/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
#include "static-mobility-model.h"

namespace ns3 {

const InterfaceId StaticMobilityModel::iid = MakeInterfaceId ("StaticMobilityModel", MobilityModel::iid);
const ClassId StaticMobilityModel::cid = MakeClassId<StaticMobilityModel,double, double> ("StaticMobilityModel", 
                                                                                StaticMobilityModel::iid);

StaticMobilityModel::StaticMobilityModel ()
  : m_x (0.0), m_y (0.0), m_z (0.0)
{
  SetInterfaceId (StaticMobilityModel::iid);
}
StaticMobilityModel::StaticMobilityModel (double x, double y)
  : m_x (x), m_y (y), m_z (0.0)
{
  SetInterfaceId (StaticMobilityModel::iid);
}
StaticMobilityModel::StaticMobilityModel (double x, double y, double z)
  : m_x (x), m_y (y), m_z (z)
{
  SetInterfaceId (StaticMobilityModel::iid);
}
StaticMobilityModel::~StaticMobilityModel ()
{}

Position
StaticMobilityModel::DoGet (void) const
{
  return Position (m_x, m_y, m_z);
}
void 
StaticMobilityModel::DoSet (const Position &position)
{
  bool changed = false;
  if (m_x != position.x || m_y != position.y || m_z != position.z)
    {
      changed = true;
    }
  m_x = position.x;
  m_y = position.y;
  m_z = position.z;
  if (changed)
    {
      NotifyCourseChange ();
    }
}


}; // namespace ns3
