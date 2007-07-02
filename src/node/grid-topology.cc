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
#include "grid-topology.h"
#include "static-position.h"

namespace ns3 {

GridTopology::GridTopology (double xMin, double yMin, uint32_t nCols, double deltaX, double deltaY)
  : m_xMin (xMin),
    m_yMin (yMin),
    m_nCols (nCols),
    m_deltaX (deltaX),
    m_deltaY (deltaY),
    m_positionClassId (StaticPosition::cid)
{}

void 
GridTopology::SetPositionModel (ClassId classId)
{
  m_positionClassId = classId;
}

void 
GridTopology::Create (std::vector<Ptr<Object> > objects)
{
  double x, y;
  uint32_t col;
  x = m_xMin;
  y = m_yMin;
  col = 0;
  for (std::vector<Ptr<Object> >::const_iterator i = objects.begin ();
       i != objects.end (); i++)
    {
      Ptr<Object> object = *i;
      object->AddInterface (ComponentManager::Create (m_positionClassId, x, y));
      x += m_deltaX;
      col++;
      if (col == m_nCols)
	{
	  col = 0;
	  x = m_xMin;
	  y += m_deltaY;
	}
    }
}

} // namespace ns3
