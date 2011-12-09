/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Marco Miozzo  <marco.miozzo@cttc.es>
 * 
 */


#include <ns3/building.h>
#include <ns3/building-list.h>
#include <ns3/enum.h>
#include "ns3/uinteger.h"

namespace ns3 {


TypeId
Building::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Building")
    .SetParent<Object> ()
    .AddConstructor<Building> ()
    .AddAttribute ("roomX", "The number of rooms in the X axis.",
                   TypeId::ATTR_GET, // allow only getting it.
                   UintegerValue (4),
                   MakeUintegerAccessor (&Building::m_roomX),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("roomY", "The number of rooms in the Y axis.",
                   TypeId::ATTR_GET, // allow only getting it.
                   UintegerValue (1),
                   MakeUintegerAccessor (&Building::m_roomY),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("nFloor", "The number of floors of this building.",
                   TypeId::ATTR_GET, // allow only getting it.
                   UintegerValue (1),
                   MakeUintegerAccessor (&Building::m_floor),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Id", "The id (unique integer) of this Building.",
                   TypeId::ATTR_GET, // allow only getting it.
                   UintegerValue (0),
                   MakeUintegerAccessor (&Building::m_buildingId),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

Building::Building (double _xMin, double _xMax,
                    double _yMin, double _yMax,
                    double _zMin, double _zMax/*,
          uint8_t _nFloors, uint8_t _nRoomX, uint8_t _nRoomY*/)
  : m_floor (1),
    m_roomX (1),
    m_roomY (1),
    m_buildingType (Residential),
    m_externalWalls (ConcreteWithWindows)

{
  m_buldingBounds = Box (_xMin, _xMax, _yMin, _yMax, _zMin, _zMax);
  Construct();
}


Building::Building () 
  : m_floor (1), 
    m_roomX (1), 
    m_roomY (1),
    m_buildingType (Residential),
    m_externalWalls (ConcreteWithWindows)
{
  m_buldingBounds = Box ();
  Construct();
}

void
Building::Construct ()
{
  m_buildingId = BuildingList::Add(this);
}

void
Building::SetBuildingType (Building::BuildingType_t t)
{
  m_buildingType = t;
}

void 
Building::SetExtWallsType (Building::ExtWallsType_t t)
{
  m_externalWalls = t;
}

void
Building::SetFloorsNumber (uint8_t nfloors)
{
  m_floor = nfloors;
}

void
Building::SetNumberRoomX (uint8_t nroomx)
{
  m_roomX = nroomx;
}

void
Building::SetNumberRoomY (uint8_t nroomy)
{
  m_roomY = nroomy;
}


Building::BuildingType_t 
Building::GetBuildingType ()
{
  return (m_buildingType);
}

Building::ExtWallsType_t
Building::GetExtWallsType ()
{
  return (m_externalWalls);
}

uint8_t 
Building::GetNumberFloors ()
{
  return (m_floor);
}

uint8_t 
Building::GetNumberRoomX ()
{
  return (m_roomX);
}

uint8_t 
Building::GetNumberRoomY ()
{
  return (m_roomY);
}

Box
Building::GetBuildingBounds ()
{
  return (m_buldingBounds);
}


} // namespace ns3
