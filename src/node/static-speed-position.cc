/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006, 2007 INRIA
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
#include "static-speed-position.h"
#include "ns3/simulator.h"

namespace ns3 {

const InterfaceId StaticSpeedMobilityModel::iid = MakeInterfaceId ("StaticSpeedMobilityModel", MobilityModel::iid);
const ClassId StaticSpeedMobilityModel::cid = 
  MakeClassId<StaticSpeedMobilityModel,double, double> ("StaticSpeedMobilityModel", 
                                              StaticSpeedMobilityModel::iid);


StaticSpeedMobilityModel::StaticSpeedMobilityModel ()
  : m_x (0.0),
    m_y (0.0),
    m_z (0.0),
    m_dx (0.0),
    m_dy (0.0),
    m_dz (0.0),
    m_prevTime (Simulator::Now ())
{
  SetInterfaceId (StaticSpeedMobilityModel::iid);
}
StaticSpeedMobilityModel::StaticSpeedMobilityModel (double x, double y, double z)
  : m_x (x),
    m_y (y),
    m_z (z),
    m_dx (0.0),
    m_dy (0.0),
    m_dz (0.0),
    m_prevTime (Simulator::Now ())
{
  SetInterfaceId (StaticSpeedMobilityModel::iid);
}
StaticSpeedMobilityModel::StaticSpeedMobilityModel (double x, double y)
  : m_x (x),
    m_y (y),
    m_z (0.0),
    m_dx (0.0),
    m_dy (0.0),
    m_dz (0.0),
    m_prevTime (Simulator::Now ())
{
  SetInterfaceId (StaticSpeedMobilityModel::iid);
}
StaticSpeedMobilityModel::StaticSpeedMobilityModel (double x, double y, double z,
                                          double dx, double dy, double dz)
  : m_x (x),
    m_y (y),
    m_z (z),
    m_dx (dx),
    m_dy (dy),
    m_dz (dz),
    m_prevTime (Simulator::Now ())
{
  SetInterfaceId (StaticSpeedMobilityModel::iid);
}

StaticSpeedMobilityModel::~StaticSpeedMobilityModel ()
{}

void 
StaticSpeedMobilityModel::SetSpeed (double dx, double dy, double dz)
{
  bool changed = false;
  Update ();
  if (m_dx != dx || m_dy != dy || m_dz != dz)
    {
      changed = true;
    }
  m_dx = dx;
  m_dy = dy;
  m_dz = dz;
  if (changed)
    {
      NotifyCourseChange ();
    }
}

void
StaticSpeedMobilityModel::Update (void) const
{
  Time deltaTime = Simulator::Now () - m_prevTime;
  m_prevTime = Simulator::Now ();
  double deltaS = deltaTime.GetSeconds ();
  m_x += m_dx * deltaS;
  m_y += m_dy * deltaS;
  m_z += m_dz * deltaS;
}

void 
StaticSpeedMobilityModel::DoGet (double &x, double &y, double &z) const
{
  Update ();
  x = m_x;
  y = m_y;
  z = m_z;
}
void 
StaticSpeedMobilityModel::DoSet (double x, double y, double z)
{
  bool changed = false;
  Update ();
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
