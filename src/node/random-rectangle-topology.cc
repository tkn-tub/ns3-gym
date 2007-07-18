/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "ns3/random-variable-default-value.h"
#include "random-rectangle-topology.h"
#include "random-2d-position.h"
#include "mobility-model.h"

namespace ns3 {

static ClassIdDefaultValue
g_position ("Random2dTopologyType",
            "The type of initial random position in a 2d topology.",
            Random2dPosition::iid,
            "Rectangle");

static ClassIdDefaultValue
g_mobility ("Random2dTopologyMobilityModelType",
            "The type of mobility model attached to an object in a 2d topology.",
            MobilityModel::iid,
            "StaticMobilityModel");

RandomRectangleTopology::RandomRectangleTopology ()
  : m_mobilityModel (g_mobility.GetValue ())
{
  m_positionModel = ComponentManager::Create<Random2dPosition> (g_position.GetValue (), 
                                                                Random2dPosition::iid);
}
RandomRectangleTopology::RandomRectangleTopology (Ptr<Random2dPosition> positionModel, ClassId mobilityModel)
  : m_positionModel (positionModel),
    m_mobilityModel (mobilityModel)
{}
RandomRectangleTopology::~RandomRectangleTopology ()
{
  m_positionModel = 0;
}

void 
RandomRectangleTopology::SetMobilityModel (ClassId classId)
{
  m_mobilityModel = classId;
}

void 
RandomRectangleTopology::SetPositionModel (Ptr<Random2dPosition> positionModel)
{
  m_positionModel = positionModel;
}

void 
RandomRectangleTopology::LayoutOne (Ptr<Object> object)
{
  Position2d position2d = m_positionModel->Get ();
  Ptr<MobilityModel> mobility = ComponentManager::Create<MobilityModel> (m_mobilityModel, 
                                                                         MobilityModel::iid);
  Position position = mobility->Get ();
  position.x = position2d.x;
  position.y = position2d.y;
  mobility->Set (position);
  object->AddInterface (mobility);
}


} // namespace ns3
