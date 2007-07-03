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
#include "random-rectangle-topology.h"
#include "static-position.h"

namespace ns3 {

RandomRectangleTopology::RandomRectangleTopology (double xMin, double xMax, double yMin, double yMax)
  : m_xVariable (new UniformVariable (xMin, xMax)),
    m_yVariable (new UniformVariable (yMin, yMax)),
    m_positionModel (StaticPosition::cid)
{}
RandomRectangleTopology::RandomRectangleTopology (const RandomVariable &xVariable, 
						  const RandomVariable &yVariable)
  : m_xVariable (xVariable.Copy ()),
    m_yVariable (yVariable.Copy ()),
    m_positionModel (StaticPosition::cid)
{}
RandomRectangleTopology::~RandomRectangleTopology ()
{
  delete m_xVariable;
  delete m_yVariable;
  m_xVariable = 0;
  m_yVariable = 0;
}

void 
RandomRectangleTopology::SetPositionModel (ClassId classId)
{
  m_positionModel = classId;
}

void 
RandomRectangleTopology::LayoutOne (Ptr<Object> object)
{
  double x = m_xVariable->GetValue ();
  double y = m_yVariable->GetValue ();
  object->AddInterface (ComponentManager::Create (m_positionModel, x, y));
}


} // namespace ns3
