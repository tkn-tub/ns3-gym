/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * Copyright (C) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Nicola Baldo <nbaldo@cttc.es> (took position-allocator and turned it into building-allocator)
 */
#ifndef BUILDING_ALLOCATOR_H
#define BUILDING_ALLOCATOR_H

#include "ns3/object.h"
#include "ns3/object-factory.h"
#include "ns3/random-variable.h"
#include "ns3/vector.h"
#include "ns3/building-container.h"

namespace ns3 {

class Building;


/**
 * \ingroup buildings
 * \brief Allocate buildings on a rectangular 2d grid.
 */
class GridBuildingAllocator : public Object
{
public:
  GridBuildingAllocator ();
  virtual ~GridBuildingAllocator ();

  // inherited from Object
  static TypeId GetTypeId (void);

  /**
   * Determine whether buildings are allocated row first or column first.
   */
  enum LayoutType {
    /**
     * In row-first mode, buildings are allocated on the first row until
     * N buildings have been allocated. Then, the second row located a yMin + yDelta
     * is used to allocate buildings.
     */
    ROW_FIRST,
    /**
     * In column-first mode, buildings are allocated on the first column until
     * N buildings have been allocated. Then, the second column located a xMin + xDelta
     * is used to allocate buildings.
     */
    COLUMN_FIRST
  };

  /**
   * \param xMin the x coordinate where layout will start.
   */
  void SetMinX (double xMin);
  /**
   * \param yMin the y coordinate where layout will start
   */
  void SetMinY (double yMin);
  /**
   * \param lengthX the length of the wall of each building along the X axis.
   */
  void SetLengthX (double lengthX);
  /**
   * \param lengthY the length of the wall of each building along the X axis.
   */
  void SetLengthY (double lengthY);
  /**
   * \param deltaX the x interval between two x-consecutive buildings.
   */
  void SetDeltaX (double deltaX);
  /**
   * \param deltaY the y interval between two y-consecutive buildings.
   */
  void SetDeltaY (double deltaY);
  /**
   * \param n the number of buildings allocated on each row (or each column)
   *        before switching to the next column (or row).
   */
  void SetN (uint32_t n);
  /**
   * \param layoutType the type of layout to use (row first or column first).
   */
  void SetLayoutType (enum LayoutType layoutType);

  /**
   * \return the x coordinate of the first allocated building.
   */
  double GetMinX (void) const;
  /**
   * \return the y coordinate of the first allocated building.
   */
  double GetMinY (void) const;
  /**
   * \return the length of the wall along the X axis
   */
  double GetLengthX (void) const;
  /**
   * \return the length of the wall along the Y axis
   */
  double GetLengthY (void) const;
  /**
   * \return the x interval between two x-consecutive buildings.
   */
  double GetDeltaX (void) const;
  /**
   * \return the y interval between two y-consecutive buildings.
   */
  double GetDeltaY (void) const;
  /**
   * \return the number of buildings to allocate on each row or each column.
   */
  uint32_t GetN (void) const;
  /**
   * \return the currently-selected layout type.
   */
  enum LayoutType GetLayoutType (void) const;

  /** 
   * Set an attribute to be used for each new building to be created
   * 
   * \param n attribute name
   * \param v attribute value
   */
  void SetBuildingAttribute (std::string n, const AttributeValue &v);

  /** 
   * Create a set of buildings allocated on a grid
   * 
   * \param n the number of buildings to create
   * 
   * \return the BuildingContainer that contains the newly created buildings
   */
  BuildingContainer Create (uint32_t n) const;

private:
  mutable uint32_t m_current;
  enum LayoutType m_layoutType;
  double m_xMin;
  double m_yMin;
  uint32_t m_n;
  double m_lengthX;
  double m_lengthY;
  double m_deltaX;
  double m_deltaY;
  double m_height;

  mutable ObjectFactory m_buildingFactory;
};

} // namespace ns3

#endif /* BUILDING_ALLOCATOR_H */
