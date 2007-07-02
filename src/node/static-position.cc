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
#include "static-position.h"

namespace ns3 {

const InterfaceId StaticPosition::iid = MakeInterfaceId ("StaticPosition", Position::iid);
const ClassId StaticPosition::cid = MakeClassId<StaticPosition,double, double> ("StaticPosition", 
                                                                                StaticPosition::iid);

StaticPosition::StaticPosition ()
  : m_x (0.0), m_y (0.0), m_z (0.0)
{
  SetInterfaceId (StaticPosition::iid);
}
StaticPosition::StaticPosition (double x, double y)
  : m_x (x), m_y (y), m_z (0.0)
{
  SetInterfaceId (StaticPosition::iid);
}
StaticPosition::StaticPosition (double x, double y, double z)
  : m_x (x), m_y (y), m_z (z)
{
  SetInterfaceId (StaticPosition::iid);
}
StaticPosition::~StaticPosition ()
{}

void 
StaticPosition::DoGet (double &x, double &y, double &z) const
{
  x = m_x;
  y = m_y;
  z = m_z;
}
void 
StaticPosition::DoSet (double x, double y, double z)
{
  bool changed = false;
  if (m_x != x || m_y != y || m_z != z)
    {
      changed = true;
    }
  m_x = x;
  m_y = y;
  m_z = z;
  if (changed)
    {
      NotifyCourseChange ();
    }
}


}; // namespace ns3
