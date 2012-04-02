/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "building-allocator.h"
#include "ns3/building.h"
#include "ns3/random-variable.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/log.h"
#include <cmath>

NS_LOG_COMPONENT_DEFINE ("BuildingAllocator");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (GridBuildingAllocator);

GridBuildingAllocator::GridBuildingAllocator ()
  : m_current (0)
{
  m_buildingFactory.SetTypeId ("ns3::Building");
}

GridBuildingAllocator::~GridBuildingAllocator ()
{
}

TypeId 
GridBuildingAllocator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::GridBuildingAllocator")
    .SetParent<Object> ()
    .AddConstructor<GridBuildingAllocator> ()
    .AddAttribute ("GridWidth", "The number of objects layed out on a line.",
                   UintegerValue (10),
                   MakeUintegerAccessor (&GridBuildingAllocator::m_n),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinX", "The x coordinate where the grid starts.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&GridBuildingAllocator::m_xMin),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MinY", "The y coordinate where the grid starts.",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&GridBuildingAllocator::m_yMin),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("LengthX", " the length of the wall of each building along the X axis.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&GridBuildingAllocator::m_lengthX),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("LengthY", " the length of the wall of each building along the X axis.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&GridBuildingAllocator::m_lengthY),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("DeltaX", "The x space between objects.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&GridBuildingAllocator::m_deltaX),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("DeltaY", "The y space between objects.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&GridBuildingAllocator::m_deltaY),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Height", "The height of the building (roof level)",
                   DoubleValue (10),
                   MakeDoubleAccessor (&GridBuildingAllocator::m_height),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("LayoutType", "The type of layout.",
                   EnumValue (ROW_FIRST),
                   MakeEnumAccessor (&GridBuildingAllocator::m_layoutType),
                   MakeEnumChecker (ROW_FIRST, "RowFirst",
                                    COLUMN_FIRST, "ColumnFirst"))
  ;
  return tid;
}

void
GridBuildingAllocator::SetMinX (double xMin)
{
  m_xMin = xMin;
}
void
GridBuildingAllocator::SetMinY (double yMin)
{
  m_yMin = yMin;
}
void
GridBuildingAllocator::SetLengthX (double lengthX)
{
  m_lengthX = lengthX;
}
void
GridBuildingAllocator::SetLengthY (double lengthY)
{
  m_lengthY = lengthY;
}

void
GridBuildingAllocator::SetDeltaX (double deltaX)
{
  m_deltaX = deltaX;
}
void
GridBuildingAllocator::SetDeltaY (double deltaY)
{
  m_deltaY = deltaY;
}
void
GridBuildingAllocator::SetN (uint32_t n)
{
  m_n = n;
}
void
GridBuildingAllocator::SetLayoutType (enum LayoutType layoutType)
{
  m_layoutType = layoutType;
}

double
GridBuildingAllocator::GetMinX (void) const
{
  return m_xMin;
}
double
GridBuildingAllocator::GetMinY (void) const
{
  return m_yMin;
}
double
GridBuildingAllocator::GetDeltaX (void) const
{
  return m_deltaX;
}
double
GridBuildingAllocator::GetDeltaY (void) const
{
  return m_deltaY;
}
uint32_t
GridBuildingAllocator::GetN (void) const
{
  return m_n;
}
enum GridBuildingAllocator::LayoutType
GridBuildingAllocator::GetLayoutType (void) const
{
  return m_layoutType;
}

void
GridBuildingAllocator::SetBuildingAttribute (std::string n, const AttributeValue &v)
{
  NS_LOG_FUNCTION (this);
  m_buildingFactory.Set (n, v);
}

BuildingContainer
GridBuildingAllocator::Create (uint32_t n) const
{
  BuildingContainer bc;
  uint32_t limit = n + m_current;
  for (; m_current < limit; ++m_current)
    {
      double bxmin = 0.0;
      double bymin = 0.0;
      switch (m_layoutType) {
      case ROW_FIRST:
        bxmin = m_xMin + (m_deltaX + m_lengthX) * (m_current % m_n);
        bymin = m_yMin + (m_deltaY + m_lengthY) * (m_current / m_n);
        break;
      case COLUMN_FIRST:
        bxmin = m_xMin + (m_deltaX + m_lengthX) * (m_current / m_n);
        bymin = m_yMin + (m_deltaY + m_lengthY) * (m_current % m_n);
        break;
      }
      double bxmax = bxmin + m_lengthX;
      double bymax = bymin + m_lengthY;
      Box box (bxmin, bxmax, bymin, bymax, 0, m_height);
      NS_LOG_LOGIC ("new building : " <<  box);
      BoxValue boxValue (box);
      m_buildingFactory.Set ("Boundaries", boxValue);
      Ptr<Building> b  = m_buildingFactory.Create<Building> ();
      //b->SetAttribute ("Boundaries", boxValue);
      bc.Add (b);     
    }
  return bc;
}


} // namespace ns3 
