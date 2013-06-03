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

#include <ns3/simulator.h>
#include <ns3/position-allocator.h>
#include <ns3/buildings-mobility-model.h>
#include <ns3/pointer.h>
#include <ns3/log.h>
#include <ns3/assert.h>

NS_LOG_COMPONENT_DEFINE ("MobilityBuildingInfo");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MobilityBuildingInfo);

TypeId
MobilityBuildingInfo::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MobilityBuildingInfo")
    .SetParent<Object> ()
    .SetGroupName ("Building")
    .AddConstructor<MobilityBuildingInfo> ();

  return tid;
}


MobilityBuildingInfo::MobilityBuildingInfo ()
{
  NS_LOG_FUNCTION (this);
  m_indoor = false;
  m_nFloor = 1;
  m_roomX = 1;
  m_roomY = 1;
}


MobilityBuildingInfo::MobilityBuildingInfo (Ptr<Building> building)
  : m_myBuilding (building)
{
  NS_LOG_FUNCTION (this);
  m_indoor = false;
  m_nFloor = 1;
  m_roomX = 1;
  m_roomY = 1;
}

bool
MobilityBuildingInfo::IsIndoor (void)
{
  NS_LOG_FUNCTION (this);
  return (m_indoor);
}

bool
MobilityBuildingInfo::IsOutdoor (void)
{
  NS_LOG_FUNCTION (this);
  return (!m_indoor);
}

void
MobilityBuildingInfo::SetIndoor (Ptr<Building> building, uint8_t nfloor, uint8_t nroomx, uint8_t nroomy)
{
  NS_LOG_FUNCTION (this);
  m_indoor = true;
  m_myBuilding = building;
  m_nFloor = nfloor;
  m_roomX = nroomx;
  m_roomY = nroomy;
  

  NS_ASSERT (m_roomX > 0);
  NS_ASSERT (m_roomX <= building->GetNRoomsX ());
  NS_ASSERT (m_roomY > 0);
  NS_ASSERT (m_roomY <= building->GetNRoomsY ());
  NS_ASSERT (m_nFloor > 0);
  NS_ASSERT (m_nFloor <= building->GetNFloors ());

}


void
MobilityBuildingInfo::SetIndoor (uint8_t nfloor, uint8_t nroomx, uint8_t nroomy)
{
  NS_LOG_FUNCTION (this);
  m_indoor = true;
  m_nFloor = nfloor;
  m_roomX = nroomx;
  m_roomY = nroomy;

  NS_ASSERT_MSG (m_myBuilding, "Node does not have any building defined");
  NS_ASSERT (m_roomX > 0);
  NS_ASSERT (m_roomX <= m_myBuilding->GetNRoomsX ());
  NS_ASSERT (m_roomY > 0);
  NS_ASSERT (m_roomY <= m_myBuilding->GetNRoomsY ());
  NS_ASSERT (m_nFloor > 0);
  NS_ASSERT (m_nFloor <= m_myBuilding->GetNFloors ());

}


void
MobilityBuildingInfo::SetOutdoor (void)
{
  NS_LOG_FUNCTION (this);
  m_indoor = false;
}

uint8_t
MobilityBuildingInfo::GetFloorNumber (void)
{
  NS_LOG_FUNCTION (this);
  return (m_nFloor);
}

uint8_t
MobilityBuildingInfo::GetRoomNumberX (void)
{
  NS_LOG_FUNCTION (this);
  return (m_roomX);
}

uint8_t
MobilityBuildingInfo::GetRoomNumberY (void)
{
  NS_LOG_FUNCTION (this);
  return (m_roomY);
}


Ptr<Building>
MobilityBuildingInfo::GetBuilding ()
{
  NS_LOG_FUNCTION (this);
  return (m_myBuilding);
}

  
} // namespace
