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

namespace ns3 {

/**
 * \brief a 2D grid of objects
 */
class GridTopology
{
 public:
  /**
   * \param xMin the left boundary where the objects will start being arranged.
   * \param yMin the lower boundary where the objects will start being arranged.
   * \param nCols number of objects for each row
   * \param deltaX distance separating two adjacent objects along the x axis.
   * \param deltaY distance separating two adjacent objects along the y axis.
   *
   * The first object is positioned at (xMin,yMin).
   */
  GridTopology (double xMin, double yMin, uint32_t nCols, double deltaX, double deltaY);

  /**
   * \param classId the classId of the position object to attach to each
   *        input object.
   */
  void SetPositionModel (ClassId classId);

  /**
   * \param objects a vector of objects
   *
   * Attach a position (the type of position is specified through 
   * the ClassId given to SetPositionModel) to each object present
   * in the input vector and configure its initial location with a set
   * of coordinates arranged according to a regular rectangular grid.
   */
  void Create (std::vector<Ptr<Object> > objects);
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
