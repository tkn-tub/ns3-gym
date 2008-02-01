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
#ifndef GRID_TOPOLOGY_H
#define GRID_TOPOLOGY_H

#include <vector>
#include "ns3/ptr.h"
#include "ns3/object.h"

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
   * \param n number of objects for each row or column
   * \param deltaX distance separating two adjacent objects along the x axis.
   * \param deltaY distance separating two adjacent objects along the y axis.
   *
   * The first object is positioned at (xMin,yMin).
   */
  GridTopology (double xMin, double yMin, uint32_t n, double deltaX, double deltaY);

  /**
   * \param interfaceId the interfaceId of the position object to attach to each
   *        input object.
   */
  void SetMobilityModel (TypeId interfaceId);

  /**
   * \param begin an iterator to the first object to layout.
   * \param end an iterator to the last object to layout.
   *
   * Attach a position (the type of position is specified through 
   * the TypeId given to SetMobilityModelModel) to each input object
   * and configure its initial location with a set
   * of coordinates arranged according to a regular rectangular grid,
   * one row after the other.
   */
  template <typename T>
  void LayoutRowFirst (const T &begin, const T &end);

  /**
   * \param begin an iterator to the first object to layout.
   * \param end an iterator to the last object to layout.
   *
   * Attach a position (the type of position is specified through 
   * the TypeId given to SetMobilityModelModel) to each input object
   * and configure its initial location with a set
   * of coordinates arranged according to a regular rectangular grid,
   * one column after the other.
   */
  template <typename T>
  void LayoutColumnFirst (const T &begin, const T &end);
 private:
  GridTopology ();
  void LayoutOneRowFirst (Ptr<Object> object, uint32_t i);
  void LayoutOneColumnFirst (Ptr<Object> object, uint32_t i);
  double m_xMin;
  double m_yMin;
  uint32_t m_n;
  double m_deltaX;
  double m_deltaY;
  TypeId m_positionTypeId;
};

} // namespace ns3

namespace ns3 {

template <typename T>
void
GridTopology::LayoutRowFirst (const T &begin, const T &end)
{
  uint32_t j = 0;
  for (T i = begin; i != end; i++)
    {
      LayoutOneRowFirst (*i, j);
      j++;
    }  
}

template <typename T>
void
GridTopology::LayoutColumnFirst (const T &begin, const T &end)
{
  uint32_t j = 0;
  for (T i = begin; i != end; i++)
    {
      LayoutOneColumnFirst (*i, j);
      j++;
    }
}

} // namespace ns3

#endif /* GRID_TOPOLOGY_H */
