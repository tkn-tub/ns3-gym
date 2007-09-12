/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "grid-topology.h"
#include "static-mobility-model.h"

namespace ns3 {

GridTopology::GridTopology (double xMin, double yMin, uint32_t n, double deltaX, double deltaY)
  : m_xMin (xMin),
    m_yMin (yMin),
    m_n (n),
    m_deltaX (deltaX),
    m_deltaY (deltaY),
    m_positionClassId (StaticMobilityModel::cid)
{}

void 
GridTopology::SetMobilityModel (ClassId classId)
{
  m_positionClassId = classId;
}

void 
GridTopology::LayoutOneRowFirst (Ptr<Object> object, uint32_t i)
{
  double x, y;
  x = m_xMin + m_deltaX * (i % m_n);
  y = m_yMin + m_deltaY * (i / m_n);
  Ptr<MobilityModel> mobility = ComponentManager::Create<MobilityModel> (m_positionClassId,
                                                                         MobilityModel::iid);
  object->AddInterface (mobility);
  mobility->Set (Position (x, y, 0.0));
}

void 
GridTopology::LayoutOneColumnFirst (Ptr<Object> object, uint32_t i)
{
  double x, y;
  x = m_xMin + m_deltaX * (i / m_n);
  y = m_yMin + m_deltaY * (i % m_n);
  Ptr<MobilityModel> mobility = ComponentManager::Create<MobilityModel> (m_positionClassId, 
                                                                         MobilityModel::iid);
  object->AddInterface (mobility);
  mobility->Set (Position (x, y, 0.0));
}


} // namespace ns3
