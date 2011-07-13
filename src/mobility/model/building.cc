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
// #include <ns3/box.h>

namespace ns3 {



Building::Building (double _xMin, double _xMax,
          double _yMin, double _yMax,
          double _zMin, double _zMax/*,
          uint8_t _nFloors, uint8_t _nRoomX, uint8_t _nRoomY*/) :
     m_floor (1),
     m_roomX (1),
     m_roomY (1),
     m_buildingType (Residential),
     m_externalWalls (ConcreteWithWindows)
     
{
  m_buldingBounds = Box (_xMin, _xMax, _yMin, _yMax, _zMin, _zMax);
}


Building::Building () 
  : m_floor (0), 
    m_roomX (0), 
    m_roomY (0),
    m_buildingType (Residential),
    m_externalWalls (ConcreteWithWindows)
{
  m_buldingBounds = Box ();
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


} // namespace ns3
