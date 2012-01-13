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
 * Authors: Marco Miozzo  <marco.miozzo@cttc.es>
 *          Nicola Baldo <nbaldo@cttc.es>
 * 
 */

#include "building.h"
#include "building-list.h"

#include <ns3/enum.h>
#include <ns3/uinteger.h>
#include <ns3/log.h>
#include <ns3/assert.h>
#include <math.h>

NS_LOG_COMPONENT_DEFINE ("Building");

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
                   MakeUintegerAccessor (&Building::m_roomsX),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("roomY", "The number of rooms in the Y axis.",
                   TypeId::ATTR_GET, // allow only getting it.
                   UintegerValue (1),
                   MakeUintegerAccessor (&Building::m_roomsY),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("nFloor", "The number of floors of this building.",
                   TypeId::ATTR_GET, // allow only getting it.
                   UintegerValue (1),
                   MakeUintegerAccessor (&Building::m_floors),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Id", "The id (unique integer) of this Building.",
                   TypeId::ATTR_GET, // allow only getting it.
                   UintegerValue (0),
                   MakeUintegerAccessor (&Building::m_buildingId),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

Building::Building (double xMin, 
                    double xMax,
                    double yMin, 
                    double yMax,
                    double zMin, 
                    double zMax)
  : m_buildingBounds (xMin, xMax, yMin, yMax, zMin, zMax),
    m_floors (1),
    m_roomsX (1),
    m_roomsY (1),
    m_buildingType (Residential),
    m_externalWalls (ConcreteWithWindows)
{
  NS_LOG_FUNCTION (this);
  Construct();
}


Building::Building () 
  : m_floors (1), 
    m_roomsX (1), 
    m_roomsY (1),
    m_buildingType (Residential),
    m_externalWalls (ConcreteWithWindows)
{
  NS_LOG_FUNCTION (this);
  m_buildingBounds = Box ();
  Construct();
}

Building::~Building () 
{
  NS_LOG_FUNCTION (this);
}

void
Building::DoDispose () 
{
  NS_LOG_FUNCTION (this);
}

void
Building::Construct ()
{
  NS_LOG_FUNCTION (this);
  m_buildingId = BuildingList::Add(this);
}

uint32_t
Building::GetId (void) const
{
  NS_LOG_FUNCTION (this);
  return m_buildingId;
}

void
Building::SetBuildingType (Building::BuildingType_t t)
{
  NS_LOG_FUNCTION (this << t);
  m_buildingType = t;
}

void 
Building::SetExtWallsType (Building::ExtWallsType_t t)
{
  NS_LOG_FUNCTION (this << t);
  m_externalWalls = t;
}

void
Building::SetNFloors (uint16_t nfloors)
{
  NS_LOG_FUNCTION (this << nfloors);
  m_floors = nfloors;
}

void
Building::SetNRoomsX (uint16_t nroomx)
{
  NS_LOG_FUNCTION (this << nroomx);
  m_roomsX = nroomx;
}

void
Building::SetNRoomsY (uint16_t nroomy)
{
  NS_LOG_FUNCTION (this << nroomy);
  m_roomsY = nroomy;
}


Building::BuildingType_t 
Building::GetBuildingType () const
{
  return (m_buildingType);
}

Building::ExtWallsType_t
Building::GetExtWallsType () const
{
  return (m_externalWalls);
}

uint16_t 
Building::GetNFloors () const
{
  return (m_floors);
}

uint16_t 
Building::GetNRoomsX () const
{
  return (m_roomsX);
}

uint16_t 
Building::GetNRoomsY () const
{
  return (m_roomsY);
}

bool 
Building::IsInside (Vector position) const
{
  return m_buildingBounds.IsInside (position);
}


uint16_t 
Building::GetRoomX (Vector position) const
{
  NS_ASSERT (IsInside (position));
  uint16_t n;

  if (position.x ==  m_buildingBounds.xMax)
    {
      n = m_roomsX;
    }                                                                   
  else
    {
      double xLength = m_buildingBounds.xMax - m_buildingBounds.xMin;
      double x = position.x - m_buildingBounds.xMin;
      n = floor (m_roomsX * x/xLength) + 1;
      NS_LOG_LOGIC ("xLength=" << xLength << ", x=" << x << ", m_roomsX=" << m_roomsX);
    }
  NS_LOG_LOGIC ("RoomX: " << n);
  return n;
}

uint16_t 
Building::GetRoomY (Vector position) const
{
  NS_ASSERT (IsInside (position));
  uint16_t n;

  if (position.y ==  m_buildingBounds.yMax)
    {
      n = m_roomsY;
    }                                                                   
  else
    {
      double yLength = m_buildingBounds.yMax - m_buildingBounds.yMin;
      double y = position.y - m_buildingBounds.yMin;
      n = floor (m_roomsY * y/yLength) + 1;
      NS_LOG_LOGIC ("yLength=" << yLength << ", y=" << y << ", m_roomsY=" << m_roomsY);
    }
  NS_LOG_LOGIC ("RoomY: " << n);
  return n;
}

uint16_t 
Building::GetFloor (Vector position) const
{
  NS_ASSERT (IsInside (position));
  uint16_t n;

  if (position.z ==  m_buildingBounds.zMax)
    {
      n = m_floors;
    }                                                                   
  else
    {
      double zLength = m_buildingBounds.zMax - m_buildingBounds.zMin;
      double z = position.z - m_buildingBounds.zMin;
      n = floor (m_floors * z/zLength) + 1;
      NS_LOG_LOGIC ("zLength=" << zLength << ", z=" << z << ", m_floors=" << m_floors);
    }
  NS_LOG_LOGIC ("floor: " << n);
  return n;
}


} // namespace ns3
