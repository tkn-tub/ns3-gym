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

const InterfaceId StaticSpeedPosition::iid = MakeInterfaceId ("StaticSpeedPosition", Position::iid);

StaticSpeedPosition::StaticSpeedPosition ()
  : m_x (0.0),
    m_y (0.0),
    m_z (0.0),
    m_dx (0.0),
    m_dy (0.0),
    m_dz (0.0),
    m_prevTime (Simulator::Now ())
{}
StaticSpeedPosition::StaticSpeedPosition (double x, double y, double z)
  : m_x (x),
    m_y (y),
    m_z (z),
    m_dx (0.0),
    m_dy (0.0),
    m_dz (0.0),
    m_prevTime (Simulator::Now ())
{}
StaticSpeedPosition::StaticSpeedPosition (double x, double y, double z,
                                          double dx, double dy, double dz)
  : m_x (x),
    m_y (y),
    m_z (z),
    m_dx (dx),
    m_dy (dy),
    m_dz (dz),
    m_prevTime (Simulator::Now ())
{}

StaticSpeedPosition::~StaticSpeedPosition ()
{}

void 
StaticSpeedPosition::SetPosition (double x, double y, double z)
{
  Update ();
  m_x = x;
  m_y = y;
  m_z = z;
}

void 
StaticSpeedPosition::SetSpeed (double dx, double dy, double dz)
{
  Update ();
  m_dx = dx;
  m_dy = dy;
  m_dz = dz;
}

void
StaticSpeedPosition::Update (void) const
{
  Time deltaTime = Simulator::Now () - m_prevTime;
  m_prevTime = Simulator::Now ();
  double deltaS = deltaTime.GetSeconds ();
  m_x += m_dx * deltaS;
  m_y += m_dy * deltaS;
  m_z += m_dz * deltaS;
}

void 
StaticSpeedPosition::RealGet (double &x, double &y, double &z) const
{
  Update ();
  x = m_x;
  y = m_y;
  z = m_z;
}

}; // namespace ns3
