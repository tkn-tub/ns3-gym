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
#include "ns3/vector.h"
#include "ns3/building-container.h"
#include "ns3/position-allocator.h"

namespace ns3 {

class Building;


/**
 * \ingroup buildings
 * \brief Allocate buildings on a rectangular 2d grid.
 *
 * This class allows to create a set of buildings positioned on a
 * rectangular 2D grid. Under the hood, this class uses two instances
 * of GridPositionAllocator. 
 */
class GridBuildingAllocator : public Object
{
public:
  GridBuildingAllocator ();
  virtual ~GridBuildingAllocator ();

  // inherited from Object
  static TypeId GetTypeId (void);

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

  void PushAttributes () const;
  mutable uint32_t m_current;
  enum GridPositionAllocator::LayoutType m_layoutType;
  double m_xMin;
  double m_yMin;
  uint32_t m_n;
  double m_lengthX;
  double m_lengthY;
  double m_deltaX;
  double m_deltaY;
  double m_height;

  mutable ObjectFactory m_buildingFactory;
  Ptr<GridPositionAllocator> m_lowerLeftPositionAllocator;
  Ptr<GridPositionAllocator> m_upperRightPositionAllocator;
  
};

} // namespace ns3

#endif /* BUILDING_ALLOCATOR_H */
