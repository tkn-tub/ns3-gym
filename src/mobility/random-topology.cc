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
#include "random-topology.h"
#include "random-position.h"
#include "mobility-model.h"

namespace ns3 {

static ClassIdDefaultValue
g_position ("RandomTopologyPositionType",
            "The type of initial random position in a 3d topology.",
            RandomPosition::iid,
            "RandomRectanglePosition");

static ClassIdDefaultValue
g_mobility ("RandomTopologyMobilityType",
            "The type of mobility model attached to an object in a 3d topology.",
            MobilityModel::iid,
            "StaticMobilityModel");

RandomTopology::RandomTopology ()
  : m_mobilityModel (g_mobility.GetValue ())
{
  m_positionModel = ComponentManager::Create<RandomPosition> (g_position.GetValue (), 
                                                              RandomPosition::iid);
}
RandomTopology::RandomTopology (Ptr<RandomPosition> positionModel, ClassId mobilityModel)
  : m_positionModel (positionModel),
    m_mobilityModel (mobilityModel)
{}
RandomTopology::~RandomTopology ()
{
  m_positionModel = 0;
}

void 
RandomTopology::SetMobilityModel (ClassId classId)
{
  m_mobilityModel = classId;
}

void 
RandomTopology::SetPositionModel (Ptr<RandomPosition> positionModel)
{
  m_positionModel = positionModel;
}

void 
RandomTopology::LayoutOne (Ptr<Object> object)
{
  Ptr<MobilityModel> mobility = ComponentManager::Create<MobilityModel> (m_mobilityModel, 
                                                                         MobilityModel::iid);
  Position position = m_positionModel->Get ();
  mobility->Set (position);
  object->AddInterface (mobility);
}


} // namespace ns3
