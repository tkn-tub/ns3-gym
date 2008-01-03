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
#include "ns3/interface-id-default-value.h"

namespace ns3 {

static InterfaceIdDefaultValue
g_position ("RandomTopologyPositionType",
            "The type of initial random position in a 3d topology.",
            RandomPosition::iid (),
            "RandomRectanglePosition");

static InterfaceIdDefaultValue
g_mobility ("RandomTopologyMobilityType",
            "The type of mobility model attached to an object in a 3d topology.",
            MobilityModel::iid (),
            "StaticMobilityModel");

RandomTopology::RandomTopology ()
  : m_mobilityModel (g_mobility.GetValue ())
{
  m_positionModel = g_position.GetValue ().CreateObject ()->QueryInterface<RandomPosition> ();
}
RandomTopology::RandomTopology (Ptr<RandomPosition> positionModel, InterfaceId mobilityModel)
  : m_positionModel (positionModel),
    m_mobilityModel (mobilityModel)
{}
RandomTopology::~RandomTopology ()
{
  m_positionModel = 0;
}

void 
RandomTopology::SetMobilityModel (InterfaceId interfaceId)
{
  m_mobilityModel = interfaceId;
}

void 
RandomTopology::SetPositionModel (Ptr<RandomPosition> positionModel)
{
  m_positionModel = positionModel;
}

void 
RandomTopology::LayoutOne (Ptr<Object> object)
{
  Ptr<MobilityModel> mobility = m_mobilityModel.CreateObject ()->QueryInterface<MobilityModel> ();
  object->AddInterface (mobility);
  Vector position = m_positionModel->Get ();
  mobility->SetPosition (position);
}


} // namespace ns3
