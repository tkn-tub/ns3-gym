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
#ifndef GRID_TOPOLOGY_H
#define GRID_TOPOLOGY_H

#include <vector>
#include "ns3/component-manager.h"
#include "ns3/ptr.h"
#include "node.h"

namespace ns3 {

class GridTopology
{
 public:
  GridTopology (double xMin, double yMin, uint32_t nCols, double deltaX, double deltaY);

  void SetPositionModel (ClassId classId);

  /**
   * Add position to each node in vector.
   */
  void Create (std::vector<Ptr<Node> > nodes);
 private:
  GridTopology ();
  double m_xMin;
  double m_yMin;
  uint32_t m_nCols;
  double m_deltaX;
  double m_deltaY;
  ClassId m_positionClassId;
};

} // namespace ns3

#endif /* GRID_TOPOLOGY_H */
